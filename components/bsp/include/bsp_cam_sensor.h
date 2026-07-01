/*
 * MIPI CSI 摄像头驱动接口。
 *
 * 风格对齐 bsp_lcd.h：cam_sensor_init() 一步完成总线、传感器、CSI/ISP
 * 初始化并启动推流（内部用 #define 写死硬件参数，SCCB 复用触摸 I2C 总线）。
 * 之后 get_frame / return_frame 循环取帧，cam_sensor_deinit() 释放资源。
 */
#pragma once

#include <stdint.h>
#include <stddef.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/** 初始化摄像头并启动推流（含 SCCB 总线复用、CSI/ISP 配置、V4L2 缓冲）。 */
esp_err_t cam_sensor_init(void);

/** 停止推流并释放所有资源。 */
esp_err_t cam_sensor_deinit(void);

/**
 * 阻塞等待下一帧 RGB565 图像就绪。
 *
 * @param[out] data       指向 width × height × 2 字节缓冲
 * @param[out] size       缓冲字节数
 * @param[out] width      实际帧宽度
 * @param[out] height     实际帧高度
 * @param[in]  timeout_ms 超时（毫秒）, 0 = 永久等待
 *
 * 重要：用完必须 cam_sensor_return_frame() 归还，否则后续帧无法捕获。
 */
esp_err_t cam_sensor_get_frame(uint8_t **data, size_t *size,
                               int *width, int *height,
                               uint32_t timeout_ms);

/** 归还帧缓冲 —— 标记为可复用。 */
esp_err_t cam_sensor_return_frame(uint8_t *data);

/** 返回 V4L2 帧缓冲总数。vision 层据此推导零拷贝 ringbuf 深度 = 返回值-2。 */
int cam_sensor_get_fb_count(void);

#ifdef __cplusplus
}
#endif
