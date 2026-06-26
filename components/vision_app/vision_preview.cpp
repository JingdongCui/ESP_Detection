/**
 * @file vision_preview.cpp
 * @brief 摄像头预览渲染模块
 *
 * 负责将摄像头采集的 RGB888 帧通过 PPA 硬件加速缩放/转换为 RGB565，
 * 叠加 YOLO 检测框后送入 LVGL 图像控件显示到 LCD 屏幕上。
 *
 * 调用链总览：
 *   vision_app.cpp (视觉主循环)
 *     ├── vision_preview_init()          — 启动时初始化一次
 *     ├── vision_preview_bind_labels()   — 绑定 UI 标签
 *     ├── vision_preview_set_waiting()   — 等待首帧时显示状态
 *     ├── vision_preview_update()        — 每帧主更新（有新图像时）
 *     │     ├── ppa_convert()            — PPA 硬件缩放 + 色彩转换
 *     │     ├── cache_invalidate()       — 使 CPU 可读 PPA 写入的数据
 *     │     ├── overlay_detections()     — 在缓冲上画检测框
 *     │     │     └── draw_rect()        — 画矩形边框
 *     │     │           ├── draw_hline() — 画水平线
 *     │     │           └── draw_vline() — 画垂直线
 *     │     ├── cache_flush()            — 将 CPU 修改刷回给 DMA/LCD
 *     │     ├── update_labels_locked()   — 更新 FPS 和状态标签
 *     │     └── log_frame_if_slow()      — 慢帧性能日志
 *     └── vision_preview_update_status() — 仅更新标签（无新图像时）
 *           └── update_labels_locked()
 */

#include "vision_preview.h"

#include <algorithm>
#include <cstdio>

#include "bsp_lvgl_adapter_init.h"
#include "driver/ppa.h"
#include "esp_cache.h"
#include "esp_check.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "esp_private/esp_cache_private.h"
#include "roi_tuning.h"

extern "C" {
LV_FONT_DECLARE(lv_font_MiSansVF_14_14)
LV_FONT_DECLARE(lv_font_status_cn_24)
}

static const char *TAG = "vision_preview";

namespace {

// ============================================================
// 常量定义
// ============================================================

// RGB565 颜色值（16 位：R5 G6 B5）
constexpr uint16_t kGreen  = 0x07E0;  // 检测框颜色
constexpr uint16_t kYellow = 0xFFE0;  // 占位图边框颜色
constexpr uint16_t kCyan   = 0x07FF;  // ROI 框颜色

constexpr int     kPreviewBufCount   = 2;       // 双缓冲数量（前台显示 + 后台写入）
constexpr int     kBoxThickness      = 3;       // 检测框线宽（像素）
constexpr int     kRoiThickness      = 2;       // ROI 框线宽（像素）
constexpr int64_t kSlowFrameUs       = 80'000;  // 单帧总耗时超过此值触发告警日志（微秒）
constexpr int64_t kSlowLockUs        = 20'000;  // LVGL 锁等待超过此值触发告警日志（微秒）
constexpr int64_t kSlowStatusTotalUs = 30'000;  // 纯状态更新超过此值触发告警日志（微秒）
constexpr uint32_t kInitialLogFrames = 5;       // 前 N 帧无条件打印性能日志
constexpr uint32_t kPeriodicLogEvery = 30;      // 之后每 N 帧打印一次性能日志
constexpr int64_t kSuccessHoldUs     = 1'000'000; // 识别成功状态保持时长（微秒）
constexpr int     kCalBtnW           = 52;
constexpr int     kCalBtnH           = 26;

const char *const kClassNames[YOLOPerfStats::kClassProbCount] = {
    "极兔",
    "中通",
    "韵达",
};
constexpr const char *kStatusNoTarget = "未检测到目标";
constexpr const char *kStatusSuccess  = "识别成功";

// ============================================================
// 模块状态（单例）
// ============================================================

/**
 * 预览上下文结构体，持有本模块所有运行时资源。
 * 全局唯一实例 s_preview，零初始化。
 */
struct PreviewCtx {
    // --- LVGL 视图层 ---
    lv_obj_t    *image;                         // LVGL 图像控件，承载预览画面
    lv_obj_t    *fps_label;                     // FPS 数值标签（外部创建，此处只写入文本）
    lv_obj_t    *status_label;                  // 状态文字标签（"Tracking Active" / "No Target" 等）
    lv_obj_t    *prob_labels[YOLOPerfStats::kClassProbCount]; // ROI debug 概率标签
    lv_obj_t    *cal_btn;                       // ROI 暗光校准按钮
    lv_obj_t    *cal_label;                     // 校准按钮文本
    lv_img_dsc_t img_dsc[kPreviewBufCount];     // LVGL 图像描述符，指向对应 buf

    // --- 双缓冲（分配在 PSRAM）---
    uint16_t *bufs[kPreviewBufCount];           // RGB565 像素缓冲区指针
    size_t    buf_size;                         // 单个缓冲区字节数（按 cache line 对齐后）
    size_t    cache_line_size;                  // PSRAM cache line 大小（用于对齐分配和 msync）
    int       front_idx;                        // 当前正在显示的缓冲区索引（0 或 1）

    // --- 几何参数 ---
    int32_t w;                                  // 预览宽度（像素）
    int32_t h;                                  // 预览高度（像素）

    // --- 硬件加速 ---
    ppa_client_handle_t ppa;                    // PPA（Pixel Processing Accelerator）客户端句柄

    // --- 运行时状态 ---
    bool     initialized;                       // 是否已完成初始化
    uint32_t update_count;                      // 累计更新帧数（用于周期性日志判断）
    int64_t  success_until_us;                  // 识别成功状态保持到的时间点
};

PreviewCtx s_preview = {};  // 全局单例，零初始化

// ============================================================
// 工具函数
// ============================================================

void calibration_button_event_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) != LV_EVENT_CLICKED) {
        return;
    }
    roi_tuning_request_calibration();
    if (s_preview.cal_label) {
        lv_label_set_text(s_preview.cal_label, "WAIT");
    }
}

/** 将 value 向上对齐到 align 的整数倍（align 必须是 2 的幂） */
inline size_t align_up(size_t value, size_t align)
{
    return (value + align - 1) & ~(align - 1);
}

// ============================================================
// 绘图原语 — 在 RGB565 帧缓冲上进行软件绘制
// ============================================================

/** 画水平线段：从 (x0, y) 到 (x1, y)，自动裁剪越界部分 */
void draw_hline(uint16_t *fb, int w, int h, int x0, int x1, int y, uint16_t color)
{
    if ((unsigned)y >= (unsigned)h) return;
    if (x0 > x1) std::swap(x0, x1);
    x0 = std::max(x0, 0);
    x1 = std::min(x1, w - 1);
    for (int x = x0; x <= x1; x++) {
        fb[y * w + x] = color;
    }
}

/** 画垂直线段：从 (x, y0) 到 (x, y1)，自动裁剪越界部分 */
void draw_vline(uint16_t *fb, int w, int h, int x, int y0, int y1, uint16_t color)
{
    if ((unsigned)x >= (unsigned)w) return;
    if (y0 > y1) std::swap(y0, y1);
    y0 = std::max(y0, 0);
    y1 = std::min(y1, h - 1);
    for (int y = y0; y <= y1; y++) {
        fb[y * w + x] = color;
    }
}

/** 画空心矩形边框，thickness 指定线宽（向内扩展） */
void draw_rect(uint16_t *fb, int w, int h, int x0, int y0, int x1, int y1,
               uint16_t color, int thickness)
{
    x0 = std::max(0, std::min(x0, w - 1));
    x1 = std::max(0, std::min(x1, w - 1));
    y0 = std::max(0, std::min(y0, h - 1));
    y1 = std::max(0, std::min(y1, h - 1));
    if (x0 > x1) std::swap(x0, x1);
    if (y0 > y1) std::swap(y0, y1);

    for (int t = 0; t < thickness; t++) {
        draw_hline(fb, w, h, x0, x1, y0 + t, color);  // 上边
        draw_hline(fb, w, h, x0, x1, y1 - t, color);  // 下边
        draw_vline(fb, w, h, x0 + t, y0, y1, color);  // 左边
        draw_vline(fb, w, h, x1 - t, y0, y1, color);  // 右边
    }
}

/**
 * 填充占位图：深灰渐变底色 + 黄色边框。
 * 在 init 时写入缓冲，让屏幕在收到首帧前不是纯黑。
 */
void fill_placeholder(uint16_t *fb, int w, int h)
{
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            uint8_t shade = (uint8_t)(18 + (x * 20 / w) + (y * 12 / h));
            uint16_t r = (shade >> 3) & 0x1F;
            uint16_t g = (shade >> 2) & 0x3F;
            uint16_t b = (shade >> 3) & 0x1F;
            fb[y * w + x] = (uint16_t)((r << 11) | (g << 5) | b);
        }
    }
    draw_rect(fb, w, h, 0, 0, w - 1, h - 1, kYellow, 2);
}

// ============================================================
// 性能计时
// ============================================================

/**
 * 帧处理各阶段的时间戳记录。
 * 构造时自动记录起始时间，各阶段完成后手动赋值。
 */
struct FrameTimings {
    int64_t start{esp_timer_get_time()};  // 帧处理开始
    int64_t convert_done{0};              // PPA 转换完成
    int64_t cache_in_done{0};             // cache 失效完成（CPU 可读）
    int64_t draw_done{0};                 // 检测框绘制完成
    int64_t cache_out_done{0};            // cache 刷回完成（DMA 可读）
    int64_t lock_start{0};                // 开始等待 LVGL 锁
    int64_t lock_acquired{0};             // 获得 LVGL 锁
    int64_t end{0};                       // LVGL 更新完成、锁释放
};

/**
 * 根据条件决定是否打印本帧性能日志。
 * 触发条件：前 5 帧 / 每 30 帧 / 总耗时 > 80ms / 锁等待 > 20ms。
 */
void log_frame_if_slow(const FrameTimings &t, size_t det_count)
{
    const int64_t total_us     = t.end - t.start;
    const int64_t lock_wait_us = t.lock_acquired - t.lock_start;
    const bool    periodic     = s_preview.update_count <= kInitialLogFrames ||
                                 (s_preview.update_count % kPeriodicLogEvery) == 0;
    if (!periodic && total_us <= kSlowFrameUs && lock_wait_us <= kSlowLockUs) return;

    // 暂时屏蔽
    // ESP_LOGI(TAG,
    //          "preview #%lu convert=%lldms cache_in=%lldms draw=%lldms "
    //          "cache_out=%lldms lvgl_wait=%lldms lvgl_update=%lldms total=%lldms det=%d",
    //          (unsigned long)s_preview.update_count,
    //          (t.convert_done - t.start) / 1000,
    //          (t.cache_in_done - t.convert_done) / 1000,
    //          (t.draw_done - t.cache_in_done) / 1000,
    //          (t.cache_out_done - t.draw_done) / 1000,
    //          lock_wait_us / 1000,
    //          (t.end - t.lock_acquired) / 1000,
    //          total_us / 1000,
    //          (int)det_count);
}

// ============================================================
// 硬件加速与 Cache 管理
// ============================================================

/**
 * 通过 PPA 硬件将摄像头原始帧（RGB888）缩放并转换为预览尺寸的 RGB565。
 * 阻塞模式：函数返回时 DMA 已完成写入 dst 缓冲。
 *
 * 调用者：vision_preview_update()
 */
esp_err_t ppa_convert(const uint8_t *src, int src_w, int src_h, uint16_t *dst)
{
    ppa_srm_oper_config_t srm = {};

    // 输入：摄像头全帧 RGB888
    srm.in.buffer      = src;
    srm.in.pic_w       = (uint32_t)src_w;
    srm.in.pic_h       = (uint32_t)src_h;
    srm.in.block_w     = (uint32_t)src_w;
    srm.in.block_h     = (uint32_t)src_h;
    srm.in.srm_cm      = PPA_SRM_COLOR_MODE_RGB888;

    // 输出：预览尺寸 RGB565
    srm.out.buffer      = dst;
    srm.out.buffer_size = s_preview.buf_size;
    srm.out.pic_w       = (uint32_t)s_preview.w;
    srm.out.pic_h       = (uint32_t)s_preview.h;
    srm.out.srm_cm      = PPA_SRM_COLOR_MODE_RGB565;

    // 缩放比例（不旋转）
    srm.rotation_angle = PPA_SRM_ROTATION_ANGLE_0;
    srm.scale_x        = (float)s_preview.w / (float)src_w;
    srm.scale_y        = (float)s_preview.h / (float)src_h;
    srm.mode           = PPA_TRANS_MODE_BLOCKING;

    return ppa_do_scale_rotate_mirror(s_preview.ppa, &srm);
}

/**
 * Cache 失效（Memory → CPU）：PPA DMA 写完 PSRAM 后，
 * CPU cache 中可能有旧数据，需要失效才能读到 DMA 写入的新像素。
 */
inline void cache_invalidate(uint16_t *buf)
{
    esp_cache_msync(buf, s_preview.buf_size,
                    ESP_CACHE_MSYNC_FLAG_DIR_M2C | ESP_CACHE_MSYNC_FLAG_TYPE_DATA);
}

/**
 * Cache 刷回（CPU → Memory）：CPU 在缓冲上画完检测框后，
 * 需要将 cache 中的脏数据刷回 PSRAM，LCD DMA 才能读到最新内容。
 */
inline void cache_flush(uint16_t *buf)
{
    esp_cache_msync(buf, s_preview.buf_size,
                    ESP_CACHE_MSYNC_FLAG_DIR_C2M | ESP_CACHE_MSYNC_FLAG_TYPE_DATA);
}

// ============================================================
// LVGL 标签更新（必须在持有 LVGL 锁时调用）
// ============================================================

/**
 * 更新 FPS 和状态标签文本。
 * 必须在 BSP_LVGL_Lock() 保护下调用。
 *
 * 调用者：vision_preview_update() / vision_preview_update_status()
 */
void update_labels_locked(const std::vector<Detection> &detections,
                          float fps,
                          const YOLOPerfStats &perf,
                          bool detect_ok)
{
    char text[64];
    if (s_preview.fps_label) {
        snprintf(text, sizeof(text), "%.0f", fps);
        lv_label_set_text(s_preview.fps_label, text);
    }
    if (s_preview.status_label) {
        int64_t now = esp_timer_get_time();
        if (detect_ok && !detections.empty()) {
            s_preview.success_until_us = now + kSuccessHoldUs;
        }
        const char *msg = (now < s_preview.success_until_us) ? kStatusSuccess : kStatusNoTarget;
        lv_label_set_text(s_preview.status_label, msg);
    }
    for (int i = 0; i < YOLOPerfStats::kClassProbCount; i++) {
        if (!s_preview.prob_labels[i]) continue;
        float avg = perf.roi_success ? perf.class_prob_avg[i] : 0.0f;
        float max = perf.roi_success ? perf.class_prob_max[i] : 0.0f;
        snprintf(text, sizeof(text), "%s A:%5.2f%% M:%5.2f%%",
                 kClassNames[i], avg * 100.0f, max * 100.0f);
        lv_label_set_text(s_preview.prob_labels[i], text);
    }
}

// ============================================================
// 检测框叠加
// ============================================================

/**
 * 将 YOLO 检测结果的边界框按比例映射到预览尺寸，画绿色矩形。
 * 坐标映射：检测框基于原始摄像头分辨率 (src_w x src_h)，
 * 需要等比缩放到预览分辨率 (s_preview.w x s_preview.h)。
 *
 * 调用者：vision_preview_update()
 */
void overlay_detections(uint16_t *dst,
                        const std::vector<Detection> &detections,
                        int src_w, int src_h)
{
    for (const Detection &d : detections) {
        int x1 = (int)(d.x1 * s_preview.w / (float)src_w);
        int y1 = (int)(d.y1 * s_preview.h / (float)src_h);
        int x2 = (int)(d.x2 * s_preview.w / (float)src_w);
        int y2 = (int)(d.y2 * s_preview.h / (float)src_h);
        draw_rect(dst, s_preview.w, s_preview.h,
                  x1, y1, x2, y2, kGreen, kBoxThickness);
    }
}

void overlay_roi(uint16_t *dst, const YOLOPerfStats &perf, int src_w, int src_h)
{
    if (!perf.roi_success || perf.roi_w <= 0 || perf.roi_h <= 0) {
        return;
    }
    int x1 = (int)(perf.roi_x * s_preview.w / (float)src_w);
    int y1 = (int)(perf.roi_y * s_preview.h / (float)src_h);
    int x2 = (int)((perf.roi_x + perf.roi_w) * s_preview.w / (float)src_w);
    int y2 = (int)((perf.roi_y + perf.roi_h) * s_preview.h / (float)src_h);
    draw_rect(dst, s_preview.w, s_preview.h, x1, y1, x2, y2, kCyan, kRoiThickness);
}

}  // namespace

// ============================================================
// 公开 API
// ============================================================

/**
 * 初始化把摄像头预览控件挂到 Dashboard 容器上。分配双缓冲、注册 PPA、创建 LVGL 图像控件。
 * 仅在首次调用时执行，重复调用直接返回 ESP_OK。
 *
 * @param parent  LVGL 父容器
 * @param x, y    图像控件在父容器中的位置
 * @param w, h    预览分辨率（像素）
 *
 * 调用者：system_init.c → vision_preview_init()
 */
esp_err_t vision_preview_init(lv_obj_t *parent, int32_t x, int32_t y, int32_t w, int32_t h)
{
    if (s_preview.initialized) return ESP_OK;
    ESP_RETURN_ON_FALSE(parent && w > 0 && h > 0, ESP_ERR_INVALID_ARG, TAG, "invalid preview args");

    // 获取 PSRAM cache line 大小，用于后续对齐分配
    ESP_RETURN_ON_ERROR(esp_cache_get_alignment(MALLOC_CAP_SPIRAM, &s_preview.cache_line_size),
                        TAG, "cache alignment failed");
    s_preview.w        = w;
    s_preview.h        = h;
    s_preview.buf_size = align_up((size_t)w * h * sizeof(uint16_t), s_preview.cache_line_size);

    // 分配双缓冲并填充占位图
    for (int i = 0; i < kPreviewBufCount; i++) {
        s_preview.bufs[i] = (uint16_t *)heap_caps_aligned_alloc(
            s_preview.cache_line_size, s_preview.buf_size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
        ESP_RETURN_ON_FALSE(s_preview.bufs[i], ESP_ERR_NO_MEM, TAG, "preview buffer alloc failed");
        fill_placeholder(s_preview.bufs[i], w, h);
        esp_cache_msync(s_preview.bufs[i], s_preview.buf_size, ESP_CACHE_MSYNC_FLAG_DIR_C2M);

        // 配置 LVGL 图像描述符指向该缓冲
        s_preview.img_dsc[i].header.cf = LV_COLOR_FORMAT_RGB565;
        s_preview.img_dsc[i].header.w  = w;
        s_preview.img_dsc[i].header.h  = h;
        s_preview.img_dsc[i].data_size = (uint32_t)((size_t)w * h * sizeof(uint16_t));
        s_preview.img_dsc[i].data      = (const uint8_t *)s_preview.bufs[i];
    }

    // 注册 PPA 客户端（缩放/旋转/镜像操作类型）
    ppa_client_config_t ppa_cfg = {};
    ppa_cfg.oper_type           = PPA_OPERATION_SRM;
    ESP_RETURN_ON_ERROR(ppa_register_client(&ppa_cfg, &s_preview.ppa), TAG, "PPA register failed");

    // 创建 LVGL 图像控件并放到最底层（预览在 UI 元素下方）
    s_preview.image = lv_img_create(parent);
    lv_obj_set_pos(s_preview.image, x, y);
    lv_obj_set_size(s_preview.image, w, h);
    lv_img_set_src(s_preview.image, &s_preview.img_dsc[0]);
    lv_obj_move_background(s_preview.image);

    for (int i = 0; i < YOLOPerfStats::kClassProbCount; i++) {
        s_preview.prob_labels[i] = lv_label_create(parent);
        lv_obj_set_pos(s_preview.prob_labels[i], 8, h - 62 + i * 19);
        lv_obj_set_size(s_preview.prob_labels[i], 188, 18);
        char text[64];
        snprintf(text, sizeof(text), "%s A: 0.00%% M: 0.00%%", kClassNames[i]);
        lv_label_set_text(s_preview.prob_labels[i], text);
        lv_label_set_long_mode(s_preview.prob_labels[i], LV_LABEL_LONG_CLIP);
        lv_obj_set_style_bg_color(s_preview.prob_labels[i], lv_color_hex(0x061016), LV_PART_MAIN);
        lv_obj_set_style_bg_opa(s_preview.prob_labels[i], 150, LV_PART_MAIN);
        lv_obj_set_style_pad_left(s_preview.prob_labels[i], 3, LV_PART_MAIN);
        lv_obj_set_style_pad_top(s_preview.prob_labels[i], 1, LV_PART_MAIN);
        lv_obj_set_style_text_color(s_preview.prob_labels[i], lv_color_hex(0xD9FFF4), LV_PART_MAIN);
        lv_obj_set_style_text_font(s_preview.prob_labels[i], &lv_font_MiSansVF_14_14, LV_PART_MAIN);
    }

    s_preview.cal_btn = lv_btn_create(parent);
    lv_obj_set_pos(s_preview.cal_btn, x + w - kCalBtnW - 8, y + h - 92);
    lv_obj_set_size(s_preview.cal_btn, kCalBtnW, kCalBtnH);
    lv_obj_set_scrollbar_mode(s_preview.cal_btn, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_radius(s_preview.cal_btn, 4, LV_PART_MAIN);
    lv_obj_set_style_bg_color(s_preview.cal_btn, lv_color_hex(0x113640), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(s_preview.cal_btn, 210, LV_PART_MAIN);
    lv_obj_set_style_pad_all(s_preview.cal_btn, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(s_preview.cal_btn, 1, LV_PART_MAIN);
    lv_obj_set_style_border_color(s_preview.cal_btn, lv_color_hex(0x4FE6E0), LV_PART_MAIN);
    lv_obj_add_event_cb(s_preview.cal_btn, calibration_button_event_cb, LV_EVENT_CLICKED, NULL);
    s_preview.cal_label = lv_label_create(s_preview.cal_btn);
    lv_label_set_text(s_preview.cal_label, "CAL");
    lv_obj_set_style_text_color(s_preview.cal_label, lv_color_hex(0xD9FFF4), LV_PART_MAIN);
    lv_obj_set_style_text_font(s_preview.cal_label, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_center(s_preview.cal_label);

    s_preview.front_idx   = 0;
    s_preview.initialized = true;
    ESP_LOGI(TAG, "preview initialized: %ldx%ld", (long)w, (long)h);
    return ESP_OK;
}

/**
 * 绑定外部创建的 FPS 和状态标签控件。
 * 绑定后 update 函数会自动更新这些标签的文本。
 *
 * 调用者：system_init.c → vision_preview_bind_labels()
 */
void vision_preview_bind_labels(lv_obj_t *fps_label, lv_obj_t *status_label)
{
    s_preview.fps_label    = fps_label;
    s_preview.status_label = status_label;
    if (s_preview.status_label) {
        lv_obj_set_style_text_font(s_preview.status_label, &lv_font_status_cn_24, LV_PART_MAIN);
    }
}

/**
 * 将状态标签设为“未检测到目标”，用于等待首帧期间的 UI 反馈。
 *
 * 调用者：system_init.c → vision_preview_set_waiting()
 */
void vision_preview_set_waiting(void)
{
    if (!s_preview.initialized) return;
    if (s_preview.status_label) {
        lv_label_set_text(s_preview.status_label, kStatusNoTarget);
    }
}

void vision_preview_set_calibration_result(bool ok)
{
    if (!s_preview.initialized || !s_preview.cal_label) return;
    BSP_LVGL_Lock();
    lv_label_set_text(s_preview.cal_label, ok ? "OK" : "FAIL");
    BSP_LVGL_Unlock();
}

/**
 * 仅更新标签（无新摄像头帧时调用）。
 * 不触发图像缓冲切换，开销极低。
 * 包含慢路径告警：锁等待 > 20ms 或总耗时 > 30ms 时打印日志。
 *
 * 调用者：vision_app.cpp 主循环（当本轮无新帧可渲染时）
 */
esp_err_t vision_preview_update_status(const std::vector<Detection> &detections,
                                       float fps,
                                       const YOLOPerfStats &perf,
                                       bool detect_ok)
{
    if (!s_preview.initialized) return ESP_ERR_INVALID_STATE;

    const int64_t t_lock_start = esp_timer_get_time();
    BSP_LVGL_Lock();
    const int64_t t_lock_acquired = esp_timer_get_time();
    update_labels_locked(detections, fps, perf, detect_ok);
    BSP_LVGL_Unlock();
    const int64_t t_end = esp_timer_get_time();

    const int64_t lock_wait_us = t_lock_acquired - t_lock_start;
    const int64_t total_us     = t_end - t_lock_start;
    if (lock_wait_us > kSlowLockUs || total_us > kSlowStatusTotalUs) {
        ESP_LOGI(TAG,
                 "preview status lvgl_wait=%lldms lvgl_update=%lldms total=%lldms det=%d",
                 lock_wait_us                     / 1000,
                 (t_end - t_lock_acquired)        / 1000,
                 total_us                         / 1000,
                 (int)detections.size());
    }
    return ESP_OK;
}

/**
 * 每帧主更新：PPA 转换 → cache 失效 → 画检测框 → cache 刷回 → 切换缓冲 → 更新标签。
 * 这是预览模块的核心函数，每收到一帧新的摄像头图像时调用。
 *
 * 处理流程：
 *   1. 选择 back buffer（当前未显示的那个）
 *   2. PPA 硬件将 RGB888 缩放转换为 RGB565 写入 back buffer
 *   3. cache_invalidate — 让 CPU 能读到 PPA DMA 写入的新数据
 *   4. overlay_detections — CPU 在 back buffer 上画绿色检测框
 *   5. cache_flush — 将 CPU 的修改刷回 PSRAM 供 LCD DMA 读取
 *   6. 获取 LVGL 锁，切换图像源到 back buffer，刷新标签，释放锁
 *   7. 交换 front/back 索引，记录帧计数，按需打印性能日志
 *
 * 调用者：vision_app.cpp 主循环（当本轮有新帧时）
 */
esp_err_t vision_preview_update(const uint8_t *rgb888,
                                int src_w,
                                int src_h,
                                const std::vector<Detection> &detections,
                                float fps,
                                const YOLOPerfStats &perf,
                                bool detect_ok)
{
    if (!s_preview.initialized || !rgb888 || src_w <= 0 || src_h <= 0) {
        return ESP_ERR_INVALID_STATE;
    }

    const int back_idx = 1 - s_preview.front_idx;
    uint16_t  *dst     = s_preview.bufs[back_idx];

    FrameTimings t;

    // 阶段 1：PPA 硬件缩放 + 色彩转换（RGB888 → RGB565）
    esp_err_t ret = ppa_convert(rgb888, src_w, src_h, dst);
    t.convert_done = esp_timer_get_time();
    if (ret != ESP_OK) {
        ESP_LOGW(TAG, "PPA preview convert failed: %s", esp_err_to_name(ret));
        return ret;
    }

    // 阶段 2：失效 CPU cache，使 CPU 能读到 PPA 写入的像素
    cache_invalidate(dst);
    t.cache_in_done = esp_timer_get_time();

    // 阶段 3：在 back buffer 上叠加 ROI 框和检测框
    overlay_roi(dst, perf, src_w, src_h);
    overlay_detections(dst, detections, src_w, src_h);
    t.draw_done = esp_timer_get_time();

    // 阶段 4：将 CPU 修改刷回 PSRAM（LCD DMA 从 PSRAM 读取）
    cache_flush(dst);
    t.cache_out_done = esp_timer_get_time();

    // 阶段 5：获取 LVGL 锁，切换显示源，更新标签
    t.lock_start = esp_timer_get_time();
    BSP_LVGL_Lock();
    t.lock_acquired = esp_timer_get_time();
    lv_img_set_src(s_preview.image, &s_preview.img_dsc[back_idx]);
    lv_obj_invalidate(s_preview.image);
    update_labels_locked(detections, fps, perf, detect_ok);
    BSP_LVGL_Unlock();
    t.end = esp_timer_get_time();

    // 阶段 6：交换前后缓冲索引，记录帧数，按需打印性能日志
    s_preview.front_idx = back_idx;
    s_preview.update_count++;
    log_frame_if_slow(t, detections.size());
    return ESP_OK;
}
