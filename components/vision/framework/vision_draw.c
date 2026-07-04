/*
 * vision_draw —— 画框链路（纯 C 复刻 esp-who who_detect_result_handle.cpp）。
 *
 * 部件对应：
 *   - 结果队列 m_results        → s_queue 定长环形数组 + s_mutex
 *   - save_detect_result        → vision_draw_save_result（检测侧 push）
 *   - lcd_disp_cb               → vision_draw_lcd_disp_cb（显示侧对齐取结果+画框）
 *   - draw_detect_results_on_img→ draw_results_on_buf（esp-dl draw_hollow_rectangle 的纯 C 版）
 *   - palette                   → 红色 {255,0,0}
 *
 * esp-who 用无界 std::queue；纯 C 用定长环形数组，深度 VISION_RESULT_QUEUE_DEPTH。
 * 时间戳对齐每帧消费结果，正常不积压。
 */
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include "vision_internal.h"
#include "sdk.h"                       // send_event / get_current_event_table / EVT_VISION
#include "bsp_lvgl_adapter_init.h"     // BSP_LVGL_Lock / BSP_LVGL_Unlock

#define RGB888_BYTES_PER_PIXEL 3

// 结果队列深度（单独宏，不沿用取帧 ringbuf 深度——二者语义无关）。
// 复刻 esp-who m_results：容忍检测 FPS > 显示 FPS 时的结果积压。
#define VISION_RESULT_QUEUE_DEPTH 5

// 画框线宽（复刻 esp-who 默认 line_width=2）。颜色按业务语义分配：
// 面单(WAYBILL)=绿色；logo(LOGO)按类别分色：0=极兔红、1=韵达黄、2=中通蓝。
// 注意：预览缓冲由 PPA 以 PPA_SRM_COLOR_MODE_RGB888 输出，其内存字节序为 B,G,R
// （LCD 直刷路径下 p[0]=蓝、p[1]=绿、p[2]=红，摄像头预览色正常即印证此序）。
// 故下方写入按 B,G,R 排列；传入的 (r,g,b) 按语义命名。
#define BOX_LINE_WIDTH 2

static const char *TAG = "vision_draw";

// ---- 结果队列（复刻 m_results + m_res_mutex）----
static SemaphoreHandle_t s_mutex;                       // 保护队列（独立于 LVGL 锁）
static vision_det_frame_t s_queue[VISION_RESULT_QUEUE_DEPTH];  // 环形数组
static int s_head;                                      // 最旧元素位置（pop 端）
static int s_count;                                     // 当前元素数
static vision_det_frame_t s_current;                    // 最近对齐选中的结果（复刻 m_result）

// 初始化结果队列 mutex（复刻 esp-who 构造函数里建 m_res_mutex）。
// 必须在 vision_start 单线程阶段、检测/显示任务创建之前调用——detect(core1) 与
// display(core0) 会在同一次 NEW_FRAME 后几乎同时首帧调 save/cb，懒初始化在双核下
// 有数据竞态（双建 mutex 致互斥失效 / 跨核读半初始化的 s_mutex 致崩溃），故前置建好。
bool vision_draw_init(void)
{
    s_mutex = xSemaphoreCreateMutex();
    if (!s_mutex) {
        ESP_LOGE(TAG, "no memory for result mutex");
        return false;
    }
    s_head = 0;
    s_count = 0;
    memset(&s_current, 0, sizeof(s_current));
    return true;
}

void vision_draw_save_result(const vision_det_frame_t *frame)
{
    if (!frame || !s_mutex) {
        return;
    }
    xSemaphoreTake(s_mutex, portMAX_DELAY);
    if (s_count == VISION_RESULT_QUEUE_DEPTH) {
        // 满：丢最旧（head 前移），保证 push 不阻塞——复刻"显示慢时丢旧帧结果"。
        s_head = (s_head + 1) % VISION_RESULT_QUEUE_DEPTH;
        s_count--;
    }
    int tail = (s_head + s_count) % VISION_RESULT_QUEUE_DEPTH;
    s_queue[tail] = *frame;
    s_count++;
    xSemaphoreGive(s_mutex);
}

// 在 RGB888 缓冲上画一条水平/垂直实线段（含 clip，防越界）。颜色 (r,g,b) 逐框传入。
static void draw_hline(uint8_t *buf, int w, int h, int x0, int x1, int y, int thickness,
                       uint8_t r, uint8_t g, uint8_t b)
{
    for (int t = 0; t < thickness; t++) {
        int yy = y + t;
        if (yy < 0 || yy >= h) {
            continue;
        }
        for (int x = x0; x <= x1; x++) {
            if (x < 0 || x >= w) {
                continue;
            }
            uint8_t *p = buf + ((size_t)yy * w + x) * RGB888_BYTES_PER_PIXEL;
            p[0] = b;  // 缓冲字节序 B,G,R（见颜色说明）
            p[1] = g;
            p[2] = r;
        }
    }
}

static void draw_vline(uint8_t *buf, int w, int h, int x, int y0, int y1, int thickness,
                       uint8_t r, uint8_t g, uint8_t b)
{
    for (int t = 0; t < thickness; t++) {
        int xx = x + t;
        if (xx < 0 || xx >= w) {
            continue;
        }
        for (int y = y0; y <= y1; y++) {
            if (y < 0 || y >= h) {
                continue;
            }
            uint8_t *p = buf + ((size_t)y * w + xx) * RGB888_BYTES_PER_PIXEL;
            p[0] = b;  // 缓冲字节序 B,G,R（见颜色说明）
            p[1] = g;
            p[2] = r;
        }
    }
}

// 纯 C 复刻 dl::image::draw_hollow_rectangle：上下横线 + 左右竖线。颜色 (r,g,b) 逐框传入。
static void draw_hollow_rect(uint8_t *buf, int w, int h, int x1, int y1, int x2, int y2,
                             uint8_t r, uint8_t g, uint8_t b)
{
    draw_hline(buf, w, h, x1, x2, y1, BOX_LINE_WIDTH, r, g, b);                 // 上边
    draw_hline(buf, w, h, x1, x2, y2 - BOX_LINE_WIDTH + 1, BOX_LINE_WIDTH, r, g, b);  // 下边
    draw_vline(buf, w, h, x1, y1, y2, BOX_LINE_WIDTH, r, g, b);                 // 左边
    draw_vline(buf, w, h, x2 - BOX_LINE_WIDTH + 1, y1, y2, BOX_LINE_WIDTH, r, g, b);  // 右边
}

// 复刻 draw_detect_results_on_img：遍历结果画框（无 keypoint、无文字）。
// 第一遍画面单绿色框，第二遍画 logo 分类色框，保证 logo 覆盖在面单之上。
static void draw_logo_box(uint8_t *buf, int w, int h, const vision_det_result_t *item)
{
    uint8_t r = 255;
    uint8_t g = 0;
    uint8_t b = 0;
    if (item->category == 1) {
        r = 255;
        g = 255;
        b = 0;
    } else if (item->category == 2) {
        r = 0;
        g = 0;
        b = 255;
    }
    const int *box = item->box;
    draw_hollow_rect(buf, w, h, box[0], box[1], box[2], box[3], r, g, b);
}

static void draw_results_on_buf(uint8_t *buf, int w, int h, const vision_det_frame_t *frame)
{
    // 第一遍画面单绿色框，第二遍画 logo 分类色框，保证 logo 覆盖在面单之上。
    for (int i = 0; i < frame->count; i++) {
        if (frame->items[i].stage != VISION_STAGE_WAYBILL) {
            continue;
        }
        const int *box = frame->items[i].box;
        draw_hollow_rect(buf, w, h, box[0], box[1], box[2], box[3], 0, 255, 0);
    }
    for (int i = 0; i < frame->count; i++) {
        if (frame->items[i].stage != VISION_STAGE_LOGO) {
            continue;
        }
        draw_logo_box(buf, w, h, &frame->items[i]);
    }
}

void vision_draw_lcd_disp_cb(uint8_t *preview_buf, int preview_w, int preview_h,
                             int64_t disp_timestamp)
{
    if (!preview_buf || !s_mutex) {
        return;
    }
    xSemaphoreTake(s_mutex, portMAX_DELAY);
    // 时间戳对齐（复刻 lcd_disp_cb 的 skip-future）：
    // 队首结果 timestamp <= 显示帧 timestamp 就取并 pop，直到结果比帧更新为止。
    // 留下的 s_current 是最贴合当前显示帧的结果。
    while (s_count > 0) {
        vision_det_frame_t *front = &s_queue[s_head];
        if (front->timestamp <= disp_timestamp) {
            s_current = *front;
            s_head = (s_head + 1) % VISION_RESULT_QUEUE_DEPTH;
            s_count--;
        } else {
            break;
        }
    }
    vision_det_frame_t to_draw = s_current;  // 拷出，缩短持锁时间
    xSemaphoreGive(s_mutex);

    draw_results_on_buf(preview_buf, preview_w, preview_h, &to_draw);

    // 投递与本显示帧对齐的文本（detect 已填好整份 ev，含空帧的"无目标"）。
    // 持一次 LVGL 锁，独立于显示任务后续的 blit 锁段——避免锁嵌套与持锁时长叠加。
    // send_event 是同步回调派发（见 evt.c），ui_vision_result_event_cb 仅 label set，
    // 非阻塞。全系统持 LVGL 锁的只剩 display 与 LVGL worker，回到 esp-who 式单持锁者。
    BSP_LVGL_Lock();
    send_event(get_current_event_table(), EVT_VISION, EVT_VISION_RESULT_CHANGED,
               (uint8_t *)&to_draw.ev, 0);
    BSP_LVGL_Unlock();
}
