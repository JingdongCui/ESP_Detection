/*
 * roi_tuning 占位桩实现（等待重写）。
 * 原 detector 层 ROI 连通域采样/阈值重估（roi_algorithm / roi_tuning.cpp）已删除。
 * 本文件让 dashboard 校准按键与推理任务的校准消费点能编译：请求标记照常置/取，
 * 但 consume 恒返回 false（不触发校准），calibrate 填零返回 false。
 * 重写时把真实校准逻辑填回，并按需扩展 roi_tuning_calibration_result_t。
 */
#include <string.h>
#include "stdatomic.h"
#include "roi_tuning.h"

// 校准请求标记：dashboard 按键置位（core0），推理任务消费（core1），用原子保证跨核可见。
static atomic_bool s_cal_request = ATOMIC_VAR_INIT(false);

void roi_tuning_request_calibration(void)
{
    atomic_store(&s_cal_request, true);
}

bool roi_tuning_consume_calibration_request(void)
{
    // 桩：取并清标记，但恒返回 false——校准逻辑未就绪，不触发空校准。
    // （仍消费标记，避免重写后遗留陈旧请求；返回 false 让 vision_detect 跳过校准分支。）
    atomic_exchange(&s_cal_request, false);
    return false;
}

bool roi_tuning_calibrate_from_rgb888(const uint8_t *buf, int width, int height,
                                      int stride, roi_tuning_calibration_result_t *out)
{
    (void)buf;
    (void)width;
    (void)height;
    (void)stride;
    if (out) {
        memset(out, 0, sizeof(*out));
    }
    return false;   // 桩：校准未实现
}
