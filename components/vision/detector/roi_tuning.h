/*
 * roi_tuning —— ROI 颜色阈值自适应校准接口（占位桩，等待重写）。
 *
 * 原 detector 层 roi_algorithm / roi_tuning.cpp 已删除。本桩提供
 * vision_detect.c（每帧消费校准请求 + 校准）与 system_init.c（dashboard LOGO 按键
 * 触发请求）编译所需的接口签名与安全默认行为：
 *   - request：置请求标记（线程安全的简单标记即可，桩用原子/普通标记）
 *   - consume：取并清标记，桩恒返回 false（不触发校准）
 *   - calibrate_from_rgb888：桩不做实际采样，填零结果返回 false
 * 重写时：把校准统计逻辑（连通域采样、阈值重估）填回 roi_tuning.c，结构体字段按需扩展。
 */
#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// 一组 ROI 颜色阈值统计（校准前/后各一份，用于日志对比）。
typedef struct {
    int y_min;            // 亮度下限
    int sat_approx_max;   // 近似饱和度上限
    int rgb_delta_max;    // RGB 通道极差上限
    int min_channel_min;  // 最小通道下限
} roi_tuning_thresholds_t;

// 一次校准的完整结果（供 vision_detect.c 打印 before/after 对比）。
typedef struct {
    int region_x;         // 采样区域左上 x
    int region_y;         // 采样区域左上 y
    int region_w;         // 采样区域宽
    int region_h;         // 采样区域高
    int sample_count;     // 实际采样像素数
    roi_tuning_thresholds_t before;  // 校准前阈值
    roi_tuning_thresholds_t after;   // 校准后阈值
} roi_tuning_calibration_result_t;

// dashboard LOGO 按键 handler：请求下一帧执行 ROI 校准。签名须为 void(void)
// 以匹配 ui_calibration_handler_t。桩仅置标记。
void roi_tuning_request_calibration(void);

// 推理任务每帧调用：取并清校准请求标记。返回 true 表示本帧应执行校准。桩恒返回 false。
bool roi_tuning_consume_calibration_request(void);

// 用当前 RGB888 帧重估 ROI 阈值，结果填入 out。返回 true 表示校准成功。
// 桩不做实际采样，填零并返回 false。
bool roi_tuning_calibrate_from_rgb888(const uint8_t *buf, int width, int height,
                                      int stride, roi_tuning_calibration_result_t *out);

#ifdef __cplusplus
}
#endif
