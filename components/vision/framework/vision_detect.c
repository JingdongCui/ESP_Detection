/*
 * vision_detect —— 推理任务（假框阶段，纯 C 复刻 WhoDetect::task 链路）。
 *
 * 角色对齐 ESP-who 的订阅者节点（who_detect）：等 fetch 在 ringbuf 满时发的
 * NEW_FRAME 事件，peek ringbuf 里最新那帧的裸 mmap 指针直接读，不拷贝、不归还。
 *
 * 复刻部件：
 *   - peek 最新帧                → vision_frame_peek_latest（原图坐标系）
 *   - 造结果                     → 假框碰壁反弹（临时替代 model->run，见下方接入点）
 *   - rescale_detect_result      → rescale_box（原图坐标系 → 预览坐标系 + clip）
 *   - m_result_cb(save)          → vision_draw_save_result
 *
 * 假框在原图坐标系移动、碰壁反弹，经 rescale 落到预览坐标系，验证整条映射链。
 *
 * 模型接入只改本文件：把下方「造假框」整块替换为
 *   模型加载 → ImagePreprocessor → model->run → 后处理/NMS → 填 result.items[]。
 * 输入图即 fb.buf（fb.width × fb.height RGB888 裸 mmap 指针）。其余（peek、rescale、
 * save、时间戳对齐）保持不变。注意：peek 返回的 buf 是裸 mmap 指针，需在该帧被回收前
 * 读完（深度时间窗口，见 docs/零拷贝peek竞态风险.md）；若推理耗时不可控，应在此
 * 处先拷贝再处理。model->run 是 C++，纯 C 调用需 C++ 薄封装暴露 C 接口。
 */
#include <string.h>
#include <stdatomic.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_timer.h"
#include "esp_heap_caps.h"
#include "esp_cache.h"
#include "SEGGER_RTT.h"
#include "vision.h"                    // 对外接口：vision_frame_dump_request（供 UI 按键触发）
#include "vision_internal.h"           // 已含 evt.h → vision_result_event_data_t
#include "vision_model.h"              // C 封装层：vision_model_run / 耗时 / 三类概率
#include "roi_tuning.h"                // ROI 颜色阈值校准（C 接口，extern "C"）

//允许miss次数
#define VISION_DISPLAY_MISS_KEEP_COUNT 5

static atomic_bool s_frame_dump_request = ATOMIC_VAR_INIT(false);
static uint8_t    *s_frame_dump_buf;   // 常驻 PSRAM，按需扩容后不释放
static size_t      s_frame_dump_cap;

static vision_det_frame_t s_display_last_hit;
static bool s_display_has_last_hit;
static int s_display_miss_count;

void vision_frame_dump_request(void)
{
    atomic_store(&s_frame_dump_request, true);
}

void vision_detect_task(void *arg)
{
    EventGroupHandle_t eg = (EventGroupHandle_t)arg;
    uint32_t frame_seq = 0;

    // fps 时间窗口平均：累计窗口内帧数，每 ~500ms 算一次平均值，期间显示上次稳定值。
    // 避免单帧瞬时 fps(10000000/dt) 在帧间隔极小时跳到几百的尖峰。
    int64_t fps_win_start_us = 0;
    int      fps_win_frames  = 0;
    int      fps10           = 0;   // 当前稳定帧率 ×10，跨迭代保持

    while (true) {
        xEventGroupWaitBits(eg, VISION_NEW_FRAME, pdTRUE, pdFALSE, portMAX_DELAY);

        vision_frame_t fb;
        if (!vision_frame_peek_latest(&fb)) {
            continue;
        }

        frame_seq++;
        int64_t now_us = esp_timer_get_time();
        // 时间窗口平均 fps：窗口内累计帧数，跨过 500ms 边界时结算平均值。
        if (fps_win_start_us == 0) {
            fps_win_start_us = now_us;
        }
        fps_win_frames++;
        int64_t win_dt = now_us - fps_win_start_us;
        if (win_dt >= 500000LL) {   // 500ms 窗口
            fps10 = (int)((int64_t)fps_win_frames * 10000000LL / win_dt);
            fps_win_start_us = now_us;
            fps_win_frames = 0;
        }
    
        // ROI 颜色阈值校准由 dashboard LOGO 按键触发（非每帧），避免常态化校准的耗时与堆压力。
        // 复刻原工程 vision_app.cpp：consume 请求标记，命中则用当前 RGB888 帧重估阈值，
        // 写入 roi_tuning 全局；下一次 vision_model_run 内 app_yolo 会 apply 这套新阈值。
        if (roi_tuning_consume_calibration_request()) {
            roi_tuning_calibration_result_t cal = {0};
            bool cal_ok = roi_tuning_calibrate_from_rgb888(
                fb.buf, fb.width, fb.height, fb.width * 3, &cal);
            SEGGER_RTT_printf(0,
                "[vision_det] ROI CAL %s area=(%d,%d %dx%d) px=%d old=(Y%d S%d D%d M%d) new=(Y%d S%d D%d M%d)\n",
                cal_ok ? "OK" : "FAIL",
                cal.region_x, cal.region_y, cal.region_w, cal.region_h, cal.sample_count,
                cal.before.y_min, cal.before.sat_approx_max, cal.before.rgb_delta_max, cal.before.min_channel_min,
                cal.after.y_min, cal.after.sat_approx_max, cal.after.rgb_delta_max, cal.after.min_channel_min);
        }

        // ===== 诊断抓帧：命中 LOGO 键请求则锁定"进推理前"的整帧原始 RGB888 =====
        // memcpy 脱离 V4L2 mmap 到固定 PSRAM buffer，再 C2M cache writeback——否则 CPU
        // 写入滞留 cache，OpenOCD 从 PSRAM 物理读会拿到旧数据。RTT 打印地址+尺寸，
        // 主机据此用 dump_image 经 JTAG 拉成 .bin（紧凑 w×h×3、行优先、RGB 顺序）。
        if (atomic_exchange(&s_frame_dump_request, false)) {
            size_t need = (size_t)fb.width * fb.height * 3;
            if (s_frame_dump_cap < need) {
                heap_caps_free(s_frame_dump_buf);
                s_frame_dump_buf = heap_caps_malloc(need, MALLOC_CAP_SPIRAM);
                s_frame_dump_cap = s_frame_dump_buf ? need : 0;
            }
            if (s_frame_dump_buf) {
                memcpy(s_frame_dump_buf, fb.buf, need);
                esp_cache_msync(s_frame_dump_buf, need,
                                ESP_CACHE_MSYNC_FLAG_DIR_C2M | ESP_CACHE_MSYNC_FLAG_UNALIGNED);
                SEGGER_RTT_printf(0,
                    "[FRAME_DUMP] addr=0x%08x w=%d h=%d bytes=%u\n"
                    "[FRAME_DUMP] cmd: dump_image <file> 0x%08x %u\n",
                    (unsigned)(uintptr_t)s_frame_dump_buf, fb.width, fb.height, (unsigned)need,
                    (unsigned)(uintptr_t)s_frame_dump_buf, (unsigned)need);
            } else {
                SEGGER_RTT_printf(0, "[FRAME_DUMP] PSRAM alloc FAIL for %u bytes\n", (unsigned)need);
            }
        }

        // ===== 真实推理：原图 RGB888 → ROI/前处理/model->run/后处理 → 原图坐标框 =====
        vision_model_det_t dets[VISION_MAX_BOXES];
        int n = 0;
        bool detection_enabled = vision_is_detection_enabled();
        if (detection_enabled) {
            n = vision_model_run(fb.buf, fb.width, fb.height, dets, VISION_MAX_BOXES);
            if (n < 0) {
                n = 0;  // 推理失败按无目标处理
            }
        }

        // rescale 原图坐标 → 预览坐标，逐框 clip（复刻 rescale_detect_result + limit_box）。
        vision_det_frame_t result = {0};
        result.timestamp = fb.timestamp;  // 与帧对齐，供显示侧时间戳对齐
        int pw = 0, ph = 0;
        vision_get_preview_size(&pw, &ph);
        int kept = 0;
        if (detection_enabled && pw > 0 && ph > 0 && fb.width > 0 && fb.height > 0) {
            float sx = (float)pw / fb.width;
            float sy = (float)ph / fb.height;
            for (int i = 0; i < n && kept < VISION_MAX_BOXES; i++) {
                int x1 = (int)(dets[i].box[0] * sx);
                int y1 = (int)(dets[i].box[1] * sy);
                int x2 = (int)(dets[i].box[2] * sx);
                int y2 = (int)(dets[i].box[3] * sy);
                if (x1 < 0) x1 = 0;
                if (x1 > pw - 1) x1 = pw - 1;
                if (y1 < 0) y1 = 0;
                if (y1 > ph - 1) y1 = ph - 1;
                if (x2 < 0) x2 = 0;
                if (x2 > pw - 1) x2 = pw - 1;
                if (y2 < 0) y2 = 0;
                if (y2 > ph - 1) y2 = ph - 1;
                result.items[kept].category = dets[i].category;
                result.items[kept].score    = dets[i].score;
                result.items[kept].box[0]   = x1;
                result.items[kept].box[1]   = y1;
                result.items[kept].box[2]   = x2;
                result.items[kept].box[3]   = y2;
                result.items[kept].stage    = dets[i].stage;  // 透传级联阶段供画框分色
                kept++;
            }
        }
        result.count = kept;

        // ===== 组装 UI 文本数据进 result.ev（不投递，随结果进队列，由显示侧对齐后 send）=====
        int best_waybill = -1;
        int best_logo = -1;
        for (int i = 0; i < result.count; i++) {
            if (result.items[i].stage == VISION_STAGE_WAYBILL) {
                if (best_waybill < 0 || result.items[i].score > result.items[best_waybill].score) {
                    best_waybill = i;
                }
            } else if (result.items[i].stage == VISION_STAGE_LOGO) {
                if (best_logo < 0 || result.items[i].score > result.items[best_logo].score) {
                    best_logo = i;
                }
            }
        }
        result.ev.confidence = best_waybill >= 0 ? (int)(result.items[best_waybill].score * 100.0f) : 0;
        result.ev.logo_confidence = best_logo >= 0 ? (int)(result.items[best_logo].score * 100.0f) : 0;
        if (!detection_enabled) {
            strcpy(result.ev.status, "检测关闭");
            strcpy(result.ev.company, "--");
        } else if (best_logo >= 0) {
            strcpy(result.ev.status, "识别成功");
            // 类别名映射：0=极兔 1=韵达 2=中通
            static const char *kClassName[3] = {"极兔", "韵达", "中通"};
            int cat = result.items[best_logo].category;
            if (cat >= 0 && cat < 3) {
                strcpy(result.ev.company, kClassName[cat]);
            } else {
                strcpy(result.ev.company, "--");
            }
        } else {
            strcpy(result.ev.status, "无目标");
            strcpy(result.ev.company, "--");
        }
        result.ev.fps_x10 = fps10;
        if (detection_enabled) {
            result.ev.infer_time_ms = vision_model_last_infer_ms();
            // 三类概率×100（极兔/韵达/中通），A/M 暂同填概率值（占位，后续可分平均/峰值）
            int jt = 0, zt = 0, yd = 0;
            vision_model_get_class_probs(&jt, &zt, &yd);
            result.ev.jt_a = jt;
            result.ev.zt_a = zt;
            result.ev.yd_a = yd;
        }

        if (!detection_enabled) {
            s_display_has_last_hit = false;
            s_display_miss_count = 0;
        } else if (best_waybill >= 0 && best_logo >= 0) {
            s_display_last_hit = result;
            s_display_has_last_hit = true;
            s_display_miss_count = 0;
        } else if (s_display_has_last_hit && s_display_miss_count < VISION_DISPLAY_MISS_KEEP_COUNT) {
            s_display_miss_count++;
            vision_det_frame_t held = s_display_last_hit;
            held.timestamp = result.timestamp;
            held.ev.fps_x10 = result.ev.fps_x10;
            held.ev.infer_time_ms = result.ev.infer_time_ms;
            held.ev.jt_a = result.ev.jt_a;
            held.ev.zt_a = result.ev.zt_a;
            held.ev.yd_a = result.ev.yd_a;
            result = held;
        } else {
            s_display_has_last_hit = false;
            s_display_miss_count = 0;
        }

        // push 进结果队列（detect 全程只持轻量 s_mutex，不碰 LVGL 锁）。
        vision_draw_save_result(&result);

        SEGGER_RTT_printf(0, "[vision_det] frame #%u %dx%d boxes=%d infer=%dms %d.%d fps\n",
                          (unsigned)frame_seq, fb.width, fb.height,
                          result.count, result.ev.infer_time_ms,
                          fps10 / 10, fps10 % 10);
    }
}
