/*
 * vision 组件内共享接口 —— 零拷贝 peek 帧总线（1:1 复刻 ESP-who）。
 *
 * 机制（对齐 ESP-who who_frame_cap_node）：
 *   fetch 任务（生产者）：DQBUF 取帧 → push 进 ringbuf 持有；ringbuf 满才 pop 最旧帧
 *   QBUF 还驱动，并向所有订阅者发 NEW_FRAME。被 ringbuf 持有的帧驱动不会重填。
 *   订阅者（显示/推理）：等 NEW_FRAME → peek ringbuf 里"最新"那帧的裸 mmap 指针，
 *   直接读，不拷贝、不持锁、不归还。安全靠 ringbuf 深度（=fb_count-2）撑出的时间窗口
 *   保证——推理在该帧被挤成最旧并 QBUF 还驱动前读完即可。详见
 *   docs/零拷贝peek竞态风险.md（概率保证，已知风险）。
 */
#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "evt.h"           // vision_result_event_data_t（UI 文本投递结构）
#include "vision.h"        // vision_classification_t
#include "vision_model.h"  // vision_stage_t（级联阶段，画框据此分色）

#ifdef __cplusplus
extern "C" {
#endif

// 订阅者等待的事件位：fetch 任务在 ringbuf 满时置位。
#define VISION_NEW_FRAME (1 << 0)

// 一帧的零拷贝引用：buf 指向 V4L2 mmap 区域（裸指针），有效期 = 该帧仍在 ringbuf 内。
// 订阅者 peek 后直接读 buf，不归还；越界靠深度时间窗口避免（见上）。
typedef struct {
    const uint8_t *buf;   // RGB888 V4L2 mmap buffer pointer
    int width;
    int height;
    size_t len;           // valid bytes (bytesused)
    int64_t timestamp;    // capture time from esp_timer_get_time(), in us
    uint32_t frame_id;    // new generation for every successful DQBUF
} vision_frame_t;

typedef struct {
    vision_frame_t frame;
    bool acquired;
} vision_frame_ref_t;

// 订阅最新帧总线：返回一个新建的事件组，fetch 在 ringbuf 满时对其置 NEW_FRAME。
// 必须在 vision fetch 任务启动前、由 vision_start 在建各订阅任务前调用，
// 把返回的事件组作为任务参数传入，避免启动竞态。返回 NULL 表示订阅表已满或无内存。
EventGroupHandle_t vision_frame_subscribe(void);

// peek ringbuf 里"最新"那帧（刚 push 进来的），填入 out。
// 仅在取指针的瞬间持 mutex，返回后立即释放——之后读 buf 全程无锁。
// 返回 false 表示尚无帧（首帧前），此时 out 不被修改。
bool vision_frame_peek_latest(vision_frame_t *out);

// Temporarily pin the latest frame until vision_frame_release().
bool vision_frame_acquire_latest(vision_frame_ref_t *out);
void vision_frame_release(vision_frame_ref_t *ref);

typedef struct vision_stable_slot vision_stable_slot_t;

esp_err_t vision_stable_frame_init(void);
vision_stable_slot_t *vision_stable_frame_acquire(void);
esp_err_t vision_stable_frame_copy_from_ref(vision_stable_slot_t *slot,
                                             const vision_frame_ref_t *ref);
uint8_t *vision_stable_frame_pixels(vision_stable_slot_t *slot);
bool vision_stable_frame_set_inferencing(vision_stable_slot_t *slot);
bool vision_stable_frame_submit(vision_stable_slot_t *slot,
                                const vision_model_det_t *dets,
                                int det_count,
                                uint16_t primary_class_id,
                                uint16_t primary_confidence_x1000,
                                uint16_t infer_time_ms);
bool vision_stable_frame_discard(vision_stable_slot_t *slot);

// 一个检测框（纯 C 复刻 dl::detect::result_t，目标检测无 keypoint）。
// box 为 [left_up_x, left_up_y, right_down_x, right_down_y]，预览坐标系。
typedef struct {
    int category;   // 类别索引
    float score;    // 置信度
    int box[4];     // 边界框：左上 x,y 右下 x,y
    vision_stage_t stage;  // 所属级联阶段（画框据此分色：面单红 / logo 绿）
} vision_det_result_t;

// 单帧检测结果集（复刻 WhoDetect::result_t {det_res, timestamp}）。
// timestamp 与产出该结果的帧的 vision_frame_t.timestamp 一致，供显示侧对齐。
#define VISION_MAX_BOXES 8  // 单帧最多框数（假框阶段仅 1，留余量）
typedef struct {
    vision_det_result_t items[VISION_MAX_BOXES];  // 画框坐标（预览坐标系）
    int count;
    int64_t timestamp;                            // 显示侧时间戳对齐用
    vision_result_event_data_t ev;                // 投文本用，detect 一次性填好整份
} vision_det_frame_t;

// 初始化结果队列 mutex 与状态（复刻 esp-who 构造函数建 m_res_mutex）。
// 必须在 vision_start 单线程阶段、检测/显示任务创建前调用。返回 false 表示无内存。
bool vision_draw_init(void);

// 检测侧 push 一帧结果进队列（复刻 WhoDetectResultLCDDisp::save_detect_result）。
// 内部加 mutex 保护，可跨任务调用。
void vision_draw_save_result(const vision_det_frame_t *frame);

bool vision_draw_get_latest_classification(vision_classification_t *out);

// 显示侧回调：按 disp_timestamp 对齐取最贴合的结果，在 preview_buf(RGB888) 上画框
// （复刻 lcd_disp_cb + draw_detect_results_on_img）。须在持显示锁、draw_bitmap 前调用。
// preview_buf 尺寸为 preview_w × preview_h × 3 字节。
void vision_draw_lcd_disp_cb(uint8_t *preview_buf, int preview_w, int preview_h,
                             int64_t disp_timestamp);

// 在任意 RGB888 缓冲上画检测框（面单绿框打底、logo 分类色框覆盖），复用显示侧同一画框逻辑。
// buf 尺寸为 w×h×3；frame->items[].box 必须已是 buf 坐标系（调用方负责缩放/clip）。
void vision_draw_boxes_rgb888(uint8_t *buf, int w, int h, const vision_det_frame_t *frame);

// 检测侧在「新包裹识别成功」上升沿调用：用当前原图帧 src(src_w×src_h RGB888)
// 硬件缩放到 640×375，按同比例映射 dets[] 原图坐标框并 burn-in，生成带框快照，
// 经信号量交给以太网侧消费。忙于上一张消费时本次静默跳过（不阻塞检测任务）。
void vision_get_preview_size(int *w, int *h);

// 推理任务入口（vision_detect.c 实现，vision_start 内创建任务时引用）。
// 参数为本任务订阅得到的事件组（EventGroupHandle_t）。
void vision_detect_task(void *arg);

#ifdef __cplusplus
}
#endif
