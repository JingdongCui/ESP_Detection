/*
 * vision_app —— 视觉链路协调（1:1 复刻 ESP-who 的零拷贝 peek 帧总线）。
 *
 * 三个独立 FreeRTOS 任务，靠一个原图 ringbuf 解耦：
 *   - fetch 任务（core0，生产者）：cam_sensor_get_frame(DQBUF) 取裸 mmap 帧
 *       → push 进 ringbuf 持有；ringbuf 满才 pop 最旧帧 cam_sensor_return_frame(QBUF)
 *       还驱动，并向所有订阅者发 NEW_FRAME。被持有的帧驱动不会重填。
 *   - 显示任务（core0）：等 NEW_FRAME → peek 最新帧 → PPA SRM 缩放转 RGB888
 *       → 画框 → PPA(scale=1.0) 把中转缓冲搬到 LCD framebuffer 视频子区域。
 *   - 推理任务（core1，vision_detect.c）：等 NEW_FRAME → peek 最新帧（同一原图）。
 *
 * 不拷贝、不持读锁：推理直接读 ringbuf 里的裸 mmap 指针。安全靠 ringbuf 深度
 * （=fb_count-2，当前 3）撑出的时间窗口保证——推理在该帧被挤成最旧并还驱动前读完即可。
 * 这是 ESP-who 原生的"概率保证"，已知风险见 docs/零拷贝peek竞态风险.md。
 */
#include <stdatomic.h>
#include <string.h>
#include "freertos/FreeRTOS.h"      // FreeRTOS 基础类型与 portMAX_DELAY
#include "freertos/task.h"          // xTaskCreatePinnedToCore / vTaskDelay
#include "freertos/idf_additions.h" // xTaskCreatePinnedToCoreWithCaps（栈落 PSRAM）
#include "freertos/event_groups.h"  // 事件组：订阅者等 NEW_FRAME
#include "freertos/semphr.h"        // 互斥量：保护 ringbuf
#include "driver/ppa.h"             // PPA SRM 硬件缩放/色彩转换
#include "esp_cache.h"              // esp_cache_msync：blit 前对 fb 视频横带 C2M writeback
#include "esp_heap_caps.h"          // heap_caps_malloc/calloc + 内部 RAM 诊断
#include "esp_log.h"                // ESP_LOGx
#include "esp_timer.h"              // esp_timer_get_time 时间戳
#include "SEGGER_RTT.h"             // 内部 RAM 分配诊断直发 RTT（ESP_LOG 走 UART 不进 rtt.log）
#include "lvgl.h"                   // 读预览容器坐标 / 可见性判断
#include "bsp_cam_sensor.h"         // 摄像头取帧/归还/缓冲数
#include "bsp_lcd.h"                // BSP_LCD_GetPanelHandle
#include "bsp_lvgl_adapter_init.h"  // BSP_LVGL_Lock/Unlock
#include "vision.h"                 // 对外接口 vision_start
#include "vision_internal.h"        // 组件内零拷贝帧总线接口
#include "vision_model.h"           // C 封装：挂 SPIFFS + 建 AppYOLO + load_model

// RGB888 每像素字节数（PPA 输出 / LCD 直刷的预览缓冲用）。
#define RGB888_BYTES_PER_PIXEL 3

// 采集任务参数：栈深度（字节）与优先级。优先级 4 高于普通后台任务，
// 保证取帧及时，但低于系统关键任务。
#define VISION_FETCH_STACK_SIZE 4096
#define VISION_FETCH_PRIORITY   4
// 显示任务参数：同栈深/优先级，与采集同钉 core0。
#define VISION_DISP_STACK_SIZE  4096
#define VISION_DISP_PRIORITY    4

// 推理任务参数：钉 core1 与采集并行。esp-dl 推理 + ROI 连通域 BFS + std::vector。
// 栈落 SRAM（普通 xTaskCreatePinnedToCore，usStackDepth 单位字节）：关掉未使用的 ThorVG
// 把 swdraw 栈 32KB→8KB、LVGL adapter 栈 32KB→16KB，腾出内部 SRAM 连续块容纳本栈。
// SRAM 栈无 PSRAM 栈的中断上下文死锁风险（2026-06-29 实测双核死锁根因正是 PSRAM 栈：
// CPU1 中断保存现场写 PSRAM 栈，遇总线被 DMA/另一核占则永久阻塞致双核死锁）。
// 12KB：实测推理高水位峰值约 4.6KB，留 2.6× 余量。
#define VISION_DET_STACK_SIZE (12 * 1024)
#define VISION_DET_PRIORITY   4

// 最多订阅者数（显示 + 推理 = 2，留点余量）。
#define VISION_MAX_SUBSCRIBERS 4

// 预览容器（视频区域）与其父 dashboard 容器，由 UI 生成器在 setupUi 内创建。
// dashboard 隐藏时整个仪表盘不可见，显示任务据此跳过渲染。
extern lv_obj_t *scr_dashboard_cont_dashboard;   // 仪表盘根容器（决定预览是否可见）
extern lv_obj_t *scr_dashboard_cont_live_vedio;  // 视频预览子容器（提供绘制坐标/尺寸）

static const char *TAG = "vision";

static atomic_bool s_detection_enabled = ATOMIC_VAR_INIT(true);
static atomic_bool s_preview_overlay_enabled = ATOMIC_VAR_INIT(true);

void vision_set_detection_enabled(bool enabled)
{
    atomic_store(&s_detection_enabled, enabled);
}

bool vision_is_detection_enabled(void)
{
    return atomic_load(&s_detection_enabled);
}

void vision_set_preview_overlay_enabled(bool enabled)
{
    atomic_store(&s_preview_overlay_enabled, enabled);
}

bool vision_is_preview_overlay_enabled(void)
{
    return atomic_load(&s_preview_overlay_enabled);
}

// ---- 预览（PPA 缩放 + 硬件搬运到 fb）资源，仅显示任务使用 ----
static ppa_client_handle_t s_ppa;       // PPA SRM 客户端句柄（缩放 + scale=1.0 搬运共用）
static ppa_client_handle_t s_snapshot_ppa;

// ---- 边沿触发带框快照（识别成功新包裹时生成，供以太网 burn-in 发送）----
// 尺寸必须与 Ethernet_app 的 SNAPSHOT_WIDTH/HEIGHT 一致（当前 640×375）。
#define BOXED_SNAPSHOT_W 640
#define BOXED_SNAPSHOT_H 375
#define BOXED_SNAPSHOT_BYTES ((size_t)BOXED_SNAPSHOT_W * BOXED_SNAPSHOT_H * 3)
static uint8_t         *s_boxed_buf;        // 640×375×3 RGB888，PSRAM，cache 对齐
static SemaphoreHandle_t s_boxed_ready;     // 二值：capture give，take 取
static SemaphoreHandle_t s_boxed_mutex;     // 保护 buffer 填充与拷出不重叠
static uint16_t         s_boxed_class_id;   // 随快照的类别 0~2（0=极兔 1=韵达 2=中通）
static uint8_t          s_boxed_conf;       // 随快照的 logo 置信度 0~100
static int s_preview_x;                 // 预览区域左上角 X（屏幕坐标）
static int s_preview_y;                 // 预览区域左上角 Y（屏幕坐标）
static int s_preview_w;                 // 预览区域宽度
static int s_preview_h;                 // 预览区域高度
static size_t s_preview_buf_size;       // 缩放输出缓冲字节数（w×h×3）

// ---- A2 LCD 直刷：中转缓冲 + 双写两块物理 framebuffer ----
static void *s_preview_buf;             // PPA 缩放输出 / 画框 / 再搬到 fb 的 RGB888 中转缓冲
// LVGL 用 DOUBLE_DIRECT 双缓冲（两块 fb 翻转），vision_start 取好两块。每帧用 PPA(scale=1.0)
// 把中转缓冲硬件搬到两块 fb 的同一视频子区域——无论 worker 翻到哪块都有最新视频，消除拖尾。
static void *s_fbs[3];                   // framebuffer 指针（DOUBLE_DIRECT 用前 2 个）
static int s_fb_count;                   // 有效 fb 数（DOUBLE_DIRECT 恒为 2）

// ---- 零拷贝帧 ringbuf（持有 fetch 出队但未归还的帧，复刻 WhoFetchNode::m_cam_fbs）----
static SemaphoreHandle_t s_ring_mutex;  // 保护 ringbuf 索引与内容
static vision_frame_t *s_ring;          // 容量 = fb_count-2 的环形数组
static int s_ring_cap;                  // 环形数组容量
static int s_ring_head;                 // 最旧帧位置（pop 端）
static int s_ring_count;                // 当前持有帧数

// ---- 订阅者事件组表（vision_start 单线程内建好，fetch 满时逐个置位）----
static EventGroupHandle_t s_subs[VISION_MAX_SUBSCRIBERS];  // 订阅者事件组数组
static int s_sub_count;                                    // 已注册订阅者数

// 订阅最新帧总线：新建一个事件组登记进订阅表并返回。
// 仅在 vision_start 单线程、任务启动前调用，故 s_subs/s_sub_count 无需加锁。
// 返回 NULL 表示订阅表已满或无内存。
EventGroupHandle_t vision_frame_subscribe(void)
{
    if (s_sub_count >= VISION_MAX_SUBSCRIBERS) {
        return NULL;  // 订阅表已满
    }
    EventGroupHandle_t eg = xEventGroupCreate();
    if (!eg) {
        return NULL;  // 无内存
    }
    s_subs[s_sub_count++] = eg;
    return eg;
}

// peek ringbuf 里"最新"那帧（最近一次 push 进来的），拷贝其引用信息到 out。
// 仅在读索引/结构体的瞬间持 mutex，返回后立即释放——之后读 out->buf 全程无锁。
// 返回 false 表示尚无帧（首帧前），此时 out 不被修改。
bool vision_frame_peek_latest(vision_frame_t *out)
{
    if (!s_ring_mutex || !out) {
        return false;
    }
    bool ok = false;
    xSemaphoreTake(s_ring_mutex, portMAX_DELAY);
    if (s_ring_count > 0) {
        // 最新帧 = 最近一次 push 的位置 = (head + count - 1) % cap。
        int idx = (s_ring_head + s_ring_count - 1) % s_ring_cap;
        *out = s_ring[idx];  // 拷贝的是引用结构（含裸指针），非帧数据本身
        ok = true;
    }
    xSemaphoreGive(s_ring_mutex);
    return ok;
}

// 取预览区域尺寸（检测侧据此把框从原图坐标系 rescale 到预览坐标系）。
// vision_start 设好 s_preview_w/h 后才有效；之前为 0。
void vision_get_preview_size(int *w, int *h)
{
    if (w) {
        *w = s_preview_w;
    }
    if (h) {
        *h = s_preview_h;
    }
}

esp_err_t vision_copy_latest_frame_scaled_rgb888(uint8_t *dst,
                                                 int dst_w,
                                                 int dst_h,
                                                 size_t dst_capacity,
                                                 int *src_w,
                                                 int *src_h,
                                                 size_t *out_len,
                                                 int64_t *timestamp_us)
{
    if (!dst || dst_w <= 0 || dst_h <= 0 || !s_ring_mutex || !s_snapshot_ppa) {
        return ESP_ERR_INVALID_ARG;
    }

    size_t need = (size_t)dst_w * (size_t)dst_h * RGB888_BYTES_PER_PIXEL;
    if (dst_capacity < need) {
        return ESP_ERR_INVALID_SIZE;
    }

    esp_err_t ret = ESP_ERR_NOT_FOUND;
    xSemaphoreTake(s_ring_mutex, portMAX_DELAY);
    if (s_ring_count > 0) {
        int idx = (s_ring_head + s_ring_count - 1) % s_ring_cap;
        const vision_frame_t fb = s_ring[idx];

        if (src_w) {
            *src_w = fb.width;
        }
        if (src_h) {
            *src_h = fb.height;
        }
        if (out_len) {
            *out_len = need;
        }
        if (timestamp_us) {
            *timestamp_us = fb.timestamp;
        }

        ppa_srm_oper_config_t srm = {
            .in = {
                .buffer = (void *)fb.buf,
                .pic_w = fb.width,
                .pic_h = fb.height,
                .block_w = fb.width,
                .block_h = fb.height,
                .block_offset_x = 0,
                .block_offset_y = 0,
                .srm_cm = PPA_SRM_COLOR_MODE_RGB888,
            },
            .out = {
                .buffer = dst,
                .buffer_size = need,
                .pic_w = dst_w,
                .pic_h = dst_h,
                .block_offset_x = 0,
                .block_offset_y = 0,
                .srm_cm = PPA_SRM_COLOR_MODE_RGB888,
            },
            .rotation_angle = PPA_SRM_ROTATION_ANGLE_0,
            .scale_x = (float)dst_w / fb.width,
            .scale_y = (float)dst_h / fb.height,
            .mode = PPA_TRANS_MODE_BLOCKING,
        };
        ret = ppa_do_scale_rotate_mirror(s_snapshot_ppa, &srm);
        if (ret == ESP_OK) {
            ret = esp_cache_msync(dst, need, ESP_CACHE_MSYNC_FLAG_DIR_M2C);
        }
    }
    xSemaphoreGive(s_ring_mutex);
    return ret;
}

void vision_boxed_snapshot_capture(const uint8_t *src, int src_w, int src_h,
                                   const vision_model_det_t *dets, int det_count,
                                   uint16_t class_id, uint8_t confidence_pct)
{
    if (!s_boxed_buf || !s_boxed_mutex || !s_boxed_ready ||
        !src || src_w <= 0 || src_h <= 0 || !s_snapshot_ppa) {
        return;
    }
    // 忙于上一张消费（take 持锁 memcpy 中）则本次跳过，绝不阻塞检测任务。
    if (xSemaphoreTake(s_boxed_mutex, 0) != pdTRUE) {
        return;
    }

    // 1) PPA 硬件缩放 src(src_w×src_h) → s_boxed_buf(640×375)，RGB888。
    ppa_srm_oper_config_t srm = {
        .in = {
            .buffer = (void *)src,
            .pic_w = src_w,
            .pic_h = src_h,
            .block_w = src_w,
            .block_h = src_h,
            .block_offset_x = 0,
            .block_offset_y = 0,
            .srm_cm = PPA_SRM_COLOR_MODE_RGB888,
        },
        .out = {
            .buffer = s_boxed_buf,
            .buffer_size = BOXED_SNAPSHOT_BYTES,
            .pic_w = BOXED_SNAPSHOT_W,
            .pic_h = BOXED_SNAPSHOT_H,
            .block_offset_x = 0,
            .block_offset_y = 0,
            .srm_cm = PPA_SRM_COLOR_MODE_RGB888,
        },
        .rotation_angle = PPA_SRM_ROTATION_ANGLE_0,
        .scale_x = (float)BOXED_SNAPSHOT_W / src_w,
        .scale_y = (float)BOXED_SNAPSHOT_H / src_h,
        .mode = PPA_TRANS_MODE_BLOCKING,
    };
    if (ppa_do_scale_rotate_mirror(s_snapshot_ppa, &srm) != ESP_OK) {
        xSemaphoreGive(s_boxed_mutex);
        return;
    }
    // PPA 输出对 CPU 画框可见：M2C 使 CPU 读到 PPA 写入的像素。
    esp_cache_msync(s_boxed_buf, BOXED_SNAPSHOT_BYTES, ESP_CACHE_MSYNC_FLAG_DIR_M2C);

    // 2) 原图坐标框 → 640×375 坐标，clip，stage/category 透传，构造临时帧。
    vision_det_frame_t tmp = {0};
    float sx = (float)BOXED_SNAPSHOT_W / src_w;
    float sy = (float)BOXED_SNAPSHOT_H / src_h;
    int kept = 0;
    for (int i = 0; i < det_count && kept < VISION_MAX_BOXES; i++) {
        int x1 = (int)(dets[i].box[0] * sx);
        int y1 = (int)(dets[i].box[1] * sy);
        int x2 = (int)(dets[i].box[2] * sx);
        int y2 = (int)(dets[i].box[3] * sy);
        if (x1 < 0) x1 = 0;
        if (x1 > BOXED_SNAPSHOT_W - 1) x1 = BOXED_SNAPSHOT_W - 1;
        if (y1 < 0) y1 = 0;
        if (y1 > BOXED_SNAPSHOT_H - 1) y1 = BOXED_SNAPSHOT_H - 1;
        if (x2 < 0) x2 = 0;
        if (x2 > BOXED_SNAPSHOT_W - 1) x2 = BOXED_SNAPSHOT_W - 1;
        if (y2 < 0) y2 = 0;
        if (y2 > BOXED_SNAPSHOT_H - 1) y2 = BOXED_SNAPSHOT_H - 1;
        tmp.items[kept].category = dets[i].category;
        tmp.items[kept].score    = dets[i].score;
        tmp.items[kept].box[0]   = x1;
        tmp.items[kept].box[1]   = y1;
        tmp.items[kept].box[2]   = x2;
        tmp.items[kept].box[3]   = y2;
        tmp.items[kept].stage    = dets[i].stage;
        kept++;
    }
    tmp.count = kept;

    // 3) burn-in 画框（面单绿框打底、logo 分类色框覆盖）。
    vision_draw_boxes_rgb888(s_boxed_buf, BOXED_SNAPSHOT_W, BOXED_SNAPSHOT_H, &tmp);
    // 画框是 CPU 写：C2M writeback，保证后续 JPEG 编码器（DMA）读到最新像素。
    esp_cache_msync(s_boxed_buf, BOXED_SNAPSHOT_BYTES,
                    ESP_CACHE_MSYNC_FLAG_DIR_C2M | ESP_CACHE_MSYNC_FLAG_UNALIGNED);

    s_boxed_class_id = class_id;
    s_boxed_conf = confidence_pct;
    xSemaphoreGive(s_boxed_mutex);
    // 二值信号量：未被消费时再次 give 保持 signaled（只留最新一张，符合边沿语义）。
    xSemaphoreGive(s_boxed_ready);
}

bool vision_boxed_snapshot_take(uint8_t *dst, size_t dst_capacity,
                                uint16_t *class_id_out, uint8_t *conf_out,
                                uint32_t timeout_ms)
{
    if (!s_boxed_ready || !s_boxed_mutex || !s_boxed_buf ||
        !dst || dst_capacity < BOXED_SNAPSHOT_BYTES) {
        return false;
    }
    if (xSemaphoreTake(s_boxed_ready, pdMS_TO_TICKS(timeout_ms)) != pdTRUE) {
        return false;  // 超时内无新包裹
    }
    // 持锁只做一次 memcpy 拷出，尽快释放，缩短与 capture 的竞争窗口。
    xSemaphoreTake(s_boxed_mutex, portMAX_DELAY);
    memcpy(dst, s_boxed_buf, BOXED_SNAPSHOT_BYTES);
    if (class_id_out) *class_id_out = s_boxed_class_id;
    if (conf_out)     *conf_out = s_boxed_conf;
    xSemaphoreGive(s_boxed_mutex);
    return true;
}

// fetch 任务把新帧并入 ringbuf：满则先 pop 最旧帧 QBUF 还驱动，再 push 新帧。
// 复刻 WhoFetchNode::update_ringbuf——被 ringbuf 持有的帧驱动不会重填。
static void ring_update(const vision_frame_t *fb)
{
    uint8_t *to_return = NULL;  // 待归还驱动的最旧帧指针（出锁后再 QBUF）
    xSemaphoreTake(s_ring_mutex, portMAX_DELAY);
    if (s_ring_count == s_ring_cap) {
        // 满：弹出最旧帧（head 处），记下指针待出锁后归还驱动，head 前移。
        to_return = (uint8_t *)s_ring[s_ring_head].buf;
        s_ring_head = (s_ring_head + 1) % s_ring_cap;
        s_ring_count--;
    }
    // push 到队尾：tail = (head + count) % cap。
    int tail = (s_ring_head + s_ring_count) % s_ring_cap;
    s_ring[tail] = *fb;
    s_ring_count++;
    xSemaphoreGive(s_ring_mutex);

    // QBUF 放到锁外：归还后驱动方可重填该缓冲，故必须确保它已不在 ringbuf 内。
    if (to_return) {
        cam_sensor_return_frame(to_return);
    }
}

// 查询 ringbuf 是否已满（锁内读 count）。fetch 据此决定是否发 NEW_FRAME。
static inline bool ring_is_full(void)
{
    bool full;
    xSemaphoreTake(s_ring_mutex, portMAX_DELAY);
    full = (s_ring_count == s_ring_cap);
    xSemaphoreGive(s_ring_mutex);
    return full;
}

// fetch 任务主循环：取帧 → 并入 ringbuf（满则归还最旧）→ 满时向所有订阅者发 NEW_FRAME。
// 复刻 WhoFetchNode::task——ringbuf 未满时不发事件，使各订阅者拿到的"最新帧"已隔开
// 整个深度，给慢推理留出回收前的读完时间窗口。
static void vision_fetch_task(void *arg)
{
    (void)arg;
    while (true) {
        uint8_t *frame = NULL;  // 出队帧的裸 mmap 指针
        size_t len = 0;         // 帧字节数
        int w = 0;              // 帧宽
        int h = 0;              // 帧高

        // 阻塞取一帧（DQBUF）。失败则短延时重试，避免空转刷屏。
        if (cam_sensor_get_frame(&frame, &len, &w, &h, 0) != ESP_OK) {
            vTaskDelay(pdMS_TO_TICKS(5));
            continue;
        }

        // 组装帧引用并入 ringbuf（满则内部 pop 最旧帧并 QBUF 还驱动）。
        vision_frame_t fb = {
            .buf = frame,
            .width = w,
            .height = h,
            .len = len,
            .timestamp = esp_timer_get_time(),  // 采集时刻，结果时序对齐备用
        };
        ring_update(&fb);

        // 仅在 ringbuf 满后才广播 NEW_FRAME——此时最新帧与最旧帧隔满整个深度。
        if (ring_is_full()) {
            for (int i = 0; i < s_sub_count; i++) {
                xEventGroupSetBits(s_subs[i], VISION_NEW_FRAME);
            }
        }
    }
}

// 用 PPA(scale=1.0) 把中转缓冲 s_preview_buf 硬件搬运到 framebuffer 的视频子区域。
// 等价于"可写子区域的硬件 memcpy"：输入输出同为 RGB888、同尺寸，仅靠 out.block_offset
// 定位到 fb 内 (s_preview_x, s_preview_y) 处；out.pic_w/pic_h 给整屏尺寸作行 stride。
//
// blit 前先 writeback：修复 CPU 圆环乱跳（详见 docs/CPU圆环乱跳根因分析.md）。PPA 写 fb
// 前会对 output 做 M2C invalidate，范围是"视频顶行起、跨整屏宽、高=视频块高"的整条横带。
// 这会丢弃 LVGL DOUBLE_DIRECT 的 refr_sync_areas 用 CPU 复制进该 fb、但本帧未再失效因而
// 未被 flush 写回的 arc dirty cache → 该 fb 的 arc 区 PSRAM 停留旧值 → 两块 fb arc 不一致
// → 翻转乱跳。对策：blit 前先对同一条横带做 C2M writeback，把残留的 arc dirty cache 落
// PSRAM；随后 PPA 的 M2C invalidate 只作废 cache（PSRAM 已是新值，arc 不回退），DMA 只写
// 视频区，arc 区 PSRAM 始终保持新值，两块 fb 一致。横带地址/长度与 PPA M2C 窗口逐字节对齐。
static esp_err_t blit_preview_to_fb(void *fb)
{
    // 先 writeback PPA 即将 invalidate 的整条视频横带（含与视频同行的 arc 区）。
    uint8_t *band = (uint8_t *)fb +
                    (size_t)s_preview_y * BSP_LCD_H_RES * RGB888_BYTES_PER_PIXEL;
    size_t band_len = (size_t)s_preview_h * BSP_LCD_H_RES * RGB888_BYTES_PER_PIXEL;
    esp_err_t cret = esp_cache_msync(band, band_len,
                                     ESP_CACHE_MSYNC_FLAG_DIR_C2M | ESP_CACHE_MSYNC_FLAG_UNALIGNED);
    if (cret != ESP_OK) {
        return cret;
    }

    ppa_srm_oper_config_t srm = {
        .in = {
            .buffer = s_preview_buf,
            .pic_w = s_preview_w,
            .pic_h = s_preview_h,
            .block_w = s_preview_w,
            .block_h = s_preview_h,
            .block_offset_x = 0,
            .block_offset_y = 0,
            .srm_cm = PPA_SRM_COLOR_MODE_RGB888,
        },
        .out = {
            .buffer = fb,
            .buffer_size = (uint32_t)BSP_LCD_H_RES * BSP_LCD_V_RES * RGB888_BYTES_PER_PIXEL,
            .pic_w = BSP_LCD_H_RES,          // 整屏宽作行 stride
            .pic_h = BSP_LCD_V_RES,
            .block_offset_x = s_preview_x,   // 落到 fb 内预览区域左上角
            .block_offset_y = s_preview_y,
            .srm_cm = PPA_SRM_COLOR_MODE_RGB888,
        },
        .rotation_angle = PPA_SRM_ROTATION_ANGLE_0,
        .scale_x = 1.0f,                     // 不缩放，纯搬运
        .scale_y = 1.0f,
        .mode = PPA_TRANS_MODE_BLOCKING,
    };
    return ppa_do_scale_rotate_mirror(s_ppa, &srm);
}

// 显示任务主循环：等 NEW_FRAME → peek 最新帧 → 锁外 PPA 缩放转 RGB888 + 画框 → 搬到 fb。
// PPA 缩放(耗时大头)与画框始终放锁外，避免饿死 LVGL worker。可见性查两次：锁外早退
// （隐藏时省 PPA 带宽，仅优化），锁内复查（防切页竞态，保正确性）。
static void vision_display_task(void *arg)
{
    EventGroupHandle_t eg = (EventGroupHandle_t)arg;  // 本任务订阅得到的事件组

    while (true) {
        // 等 fetch 发布新帧；pdTRUE 清除事件位，无新帧时阻塞不空转。
        xEventGroupWaitBits(eg, VISION_NEW_FRAME, pdTRUE, pdFALSE, portMAX_DELAY);

        // 取最新帧裸指针（ring 自带 mutex，与 LVGL 锁无关，放锁外）；首帧前无帧则跳过。
        vision_frame_t fb;
        if (!vision_frame_peek_latest(&fb)) {
            continue;
        }

        // 锁外可见性早退：仅作优化（隐藏时连 PPA 缩放都省，省带宽），非正确性保证——
        // 真正防切页竞态的复查在下方锁内。最坏误判一帧可见性，无正确性影响，不值得为它持锁。
        if (lv_obj_has_flag(scr_dashboard_cont_dashboard, LV_OBJ_FLAG_HIDDEN)) {
            continue;
        }

        // PPA 缩放到中转缓冲 s_preview_buf，画框，再搬到物理 fb 的视频子区域。
        ppa_srm_oper_config_t srm = {
            .in = {
                .buffer = (void *)fb.buf,   // 输入：原图裸 mmap 指针
                .pic_w = fb.width,
                .pic_h = fb.height,
                .block_w = fb.width,        // 整图作为一个 block 处理
                .block_h = fb.height,
                .block_offset_x = 0,
                .block_offset_y = 0,
                .srm_cm = PPA_SRM_COLOR_MODE_RGB888,
            },
            .out = {
                .buffer = s_preview_buf,    // 输出：预览缓冲
                .buffer_size = s_preview_buf_size,
                .pic_w = s_preview_w,
                .pic_h = s_preview_h,
                .block_offset_x = 0,
                .block_offset_y = 0,
                .srm_cm = PPA_SRM_COLOR_MODE_RGB888,
            },
            .rotation_angle = PPA_SRM_ROTATION_ANGLE_0,     // 不旋转
            .scale_x = (float)s_preview_w / fb.width,       // 按预览/原图比例缩放
            .scale_y = (float)s_preview_h / fb.height,
            .mode = PPA_TRANS_MODE_BLOCKING,                // 阻塞至缩放完成
        };

        // PPA 缩放放锁外：只写私有 s_preview_buf，与 panel framebuffer 无关，不碰
        // worker 争用的 LCD/DSI 资源。这步是耗时大头（阻塞至缩放完成），若纳入锁会
        // 周期性饿死 worker → timer 堆积 → worker 睡眠延迟塌到 1ms 忙轮询 → CPU 暴增。
        if (ppa_do_scale_rotate_mirror(s_ppa, &srm) != ESP_OK) {
            continue;
        }

        // 画框：按帧时间戳对齐取检测结果，叠到 s_preview_buf（复刻 lcd_disp_cb）。
        // 纯内存操作、只碰私有中转缓冲（检测结果走自带 mutex，与 LVGL 锁无关），放锁外。
        vision_draw_lcd_disp_cb((uint8_t *)s_preview_buf, s_preview_w, s_preview_h, fb.timestamp);

        // 把中转缓冲硬件搬到两块 fb 的视频子区域，纳入 LVGL 锁，与 lvgl worker 严格互斥。
        // DOUBLE_DIRECT 两块都写 → 无论 worker 翻到哪块都有最新视频，消除拖尾。每块 blit 前
        // 先对该 fb 视频横带 C2M writeback（见 blit_preview_to_fb）。锁内复查 HIDDEN 防切页遮挡。
        BSP_LVGL_Lock();
        if (!lv_obj_has_flag(scr_dashboard_cont_dashboard, LV_OBJ_FLAG_HIDDEN)) {
            for (int i = 0; i < s_fb_count; i++) {
                if (blit_preview_to_fb(s_fbs[i]) != ESP_OK) {
                    break;
                }
            }
        }
        BSP_LVGL_Unlock();
    }
}

// 分配零拷贝 ringbuf：容量 = cam_sensor_get_fb_count()-2（留 2 个给驱动）。
static bool ring_init(void)
{
    int fb_count = cam_sensor_get_fb_count();
    s_ring_cap = fb_count - 2;  // 深度：1 个给驱动 DMA 写、1 个排队，其余可被持有
    if (s_ring_cap < 1) {
        ESP_LOGE(TAG, "fb_count %d too small for zero-copy ringbuf", fb_count);
        return false;
    }
    s_ring = heap_caps_calloc(s_ring_cap, sizeof(vision_frame_t), MALLOC_CAP_DEFAULT);
    if (!s_ring) {
        ESP_LOGE(TAG, "no memory for frame ringbuf");
        return false;
    }
    s_ring_head = 0;
    s_ring_count = 0;
    return true;
}

// 启动视觉链路：建 ringbuf + mutex → 读 UI 视频区域、申请 RGB888 预览缓冲、注册 PPA
// → 为显示/推理各订阅一个事件组 → 建 fetch(core0)+显示(core0)+推理(core1) 三任务。
// 摄像头硬件初始化已在 system_init 完成，此处不重复。
esp_err_t vision_start(void)
{
    // 预览容器必须已由 setupUi 建好，否则取不到绘制坐标。
    if (!scr_dashboard_cont_live_vedio) {
        ESP_LOGE(TAG, "preview container not ready");
        return ESP_ERR_INVALID_STATE;
    }

    // 建 ringbuf 互斥量与环形数组。
    s_ring_mutex = xSemaphoreCreateMutex();
    if (!s_ring_mutex || !ring_init()) {
        ESP_LOGE(TAG, "no memory for frame bus");
        return ESP_ERR_NO_MEM;
    }

    // 读预览容器的屏幕坐标与尺寸（需先刷新布局拿到真实 coords）。
    BSP_LVGL_Lock();
    lv_obj_update_layout(scr_dashboard_cont_live_vedio);
    lv_area_t coords;
    lv_obj_get_coords(scr_dashboard_cont_live_vedio, &coords);
    // A2：视频直写物理 fb，需容器在屏幕中的绝对坐标与整块尺寸。
    s_preview_x = coords.x1;
    s_preview_y = coords.y1;
    s_preview_w = lv_area_get_width(&coords);
    s_preview_h = lv_area_get_height(&coords);
    BSP_LVGL_Unlock();

    s_preview_buf_size = (size_t)s_preview_w * s_preview_h * RGB888_BYTES_PER_PIXEL;

    // 注册 PPA SRM 客户端（缩放 + 色彩转换），两方案共用。
    ppa_client_config_t ppa_cfg = { .oper_type = PPA_OPERATION_SRM };
    esp_err_t ret = ppa_register_client(&ppa_cfg, &s_ppa);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "ppa_register_client failed: %s", esp_err_to_name(ret));
        return ret;
    }
    ret = ppa_register_client(&ppa_cfg, &s_snapshot_ppa);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "snapshot ppa_register_client failed: %s", esp_err_to_name(ret));
        return ret;
    }

    // PPA 输出经 2D-DMA 写入，缓冲必须带 DMA 能力，否则 ppa_do_scale_rotate_mirror
    // 校验失败返回错误（现象：capture PPA-FAIL）。与 ethernet 侧 s_snapshot_rgb 一致。
    s_boxed_buf = heap_caps_aligned_alloc(64, BOXED_SNAPSHOT_BYTES,
                                          MALLOC_CAP_SPIRAM | MALLOC_CAP_DMA | MALLOC_CAP_8BIT);
    s_boxed_ready = xSemaphoreCreateBinary();
    s_boxed_mutex = xSemaphoreCreateMutex();
    if (!s_boxed_buf || !s_boxed_ready || !s_boxed_mutex) {
        ESP_LOGE(TAG, "boxed snapshot init failed (buf=%p ready=%p mtx=%p)",
                 s_boxed_buf, s_boxed_ready, s_boxed_mutex);
        // 快照失败不阻断视觉主链路：capture/take 内部有 NULL 保护，直接放行继续。
    }

    // A2：中转缓冲（需 DMA 能力供 PPA 搬运），放 PSRAM。
    s_preview_buf = heap_caps_malloc(s_preview_buf_size, MALLOC_CAP_DMA | MALLOC_CAP_SPIRAM);
    if (!s_preview_buf) {
        ESP_LOGE(TAG, "no memory for preview buffer");
        return ESP_ERR_NO_MEM;
    }

    // 取 LVGL 的 framebuffer 指针。fb 地址在 panel init 后固定，
    // 此处一次取好供显示任务循环使用。
    s_fbs[0] = s_fbs[1] = NULL;
    ret = BSP_LCD_GetFrameBuffers(&s_fbs[0], &s_fbs[1]);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "get framebuffers failed: %s", esp_err_to_name(ret));
        return ret;
    }
    s_fb_count = 0;
    for (int i = 0; i < 3; i++) {
        if (s_fbs[i]) {
            s_fb_count++;
        }
    }

    // 初始化画框结果队列（建 mutex、清队列）。须在任务创建前的单线程阶段做——
    // 否则 detect(core1)/display(core0) 首帧并发懒初始化会数据竞态。
    if (!vision_draw_init()) {
        ESP_LOGE(TAG, "vision_draw_init failed");
        return ESP_ERR_NO_MEM;
    }

    // 初始化识别模型（挂载 /storage SPIFFS + new AppYOLO + load_model 默认模型）。
    // 同样须在 detect 任务创建前的单线程阶段做，避免首帧推理时模型未就绪。
    if (!vision_model_init()) {
        ESP_LOGE(TAG, "vision_model_init failed");
        return ESP_ERR_NO_MEM;
    }

    // 在任务启动前订阅，避免 fetch 已开始发事件而订阅者尚未注册的启动竞态。
    EventGroupHandle_t disp_eg = vision_frame_subscribe();  // 显示任务的事件组
    EventGroupHandle_t det_eg = vision_frame_subscribe();   // 推理任务的事件组
    if (!disp_eg || !det_eg) {
        ESP_LOGE(TAG, "subscribe frame bus failed");
        return ESP_ERR_NO_MEM;
    }

    // 建采集任务（core0，生产者）。
    BaseType_t ok = xTaskCreatePinnedToCore(
        vision_fetch_task, "vision_fetch",
        VISION_FETCH_STACK_SIZE, NULL, VISION_FETCH_PRIORITY, NULL, 0);
    if (ok != pdPASS) {
        ESP_LOGE(TAG, "create fetch task failed");
        return ESP_ERR_NO_MEM;
    }

    // 建显示任务（core0），事件组作参数传入。
    ok = xTaskCreatePinnedToCore(
        vision_display_task, "vision_disp",
        VISION_DISP_STACK_SIZE, disp_eg, VISION_DISP_PRIORITY, NULL, 0);
    if (ok != pdPASS) {
        ESP_LOGE(TAG, "create display task failed");
        return ESP_ERR_NO_MEM;
    }

    // 建推理任务（core1，与采集并行），事件组作参数传入。
    // 创建前打印内部 RAM 诊断：确认关 ThorVG + 下调 LVGL 栈后，内部 RAM 连续块已能容纳本栈。
    SEGGER_RTT_printf(0,
        "[vision_start] INTERNAL RAM: free=%u largest_block=%u | DMA-capable: free=%u largest_block=%u | det_stack=%u\n",
        (unsigned)heap_caps_get_free_size(MALLOC_CAP_INTERNAL),
        (unsigned)heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL),
        (unsigned)heap_caps_get_free_size(MALLOC_CAP_INTERNAL | MALLOC_CAP_DMA),
        (unsigned)heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL | MALLOC_CAP_DMA),
        (unsigned)VISION_DET_STACK_SIZE);

    // 栈落 SRAM（普通 xTaskCreatePinnedToCore）：12KB 在关 ThorVG + 下调 LVGL 栈腾出的连续块内。
    // 避免 PSRAM 栈的中断上下文死锁风险，理由见 VISION_DET_STACK_SIZE 处注释。
    BaseType_t det_ok = xTaskCreatePinnedToCore(
        vision_detect_task, "vision_det",
        VISION_DET_STACK_SIZE, det_eg, VISION_DET_PRIORITY, NULL, 1);
    if (det_ok != pdPASS) {
        ESP_LOGE(TAG, "create detect task failed");
        SEGGER_RTT_printf(0, "[vision_start] create detect task FAILED (det_ok=%d)\n", (int)det_ok);
        return ESP_ERR_NO_MEM;
    }
    SEGGER_RTT_printf(0, "[vision_start] detect task created OK (stack in SRAM, %u bytes)\n",
                      (unsigned)VISION_DET_STACK_SIZE);

    ESP_LOGI(TAG, "vision started: preview %dx%d at %d,%d, ring depth %d",
             s_preview_w, s_preview_h, s_preview_x, s_preview_y, s_ring_cap);
    return ESP_OK;
}

bool vision_get_latest_classification(vision_classification_t *out)
{
    return vision_draw_get_latest_classification(out);
}
