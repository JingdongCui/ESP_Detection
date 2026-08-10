/*
 * MIPI CSI 摄像头驱动接口。
 *
 * 当前实现使用 ESP-IDF esp_video/V4L2 链路：
 *   esp_video_init(csi) → open(/dev/video) → S_FMT(RGB888)
 *     → REQBUFS + mmap → STREAMON。
 *
 * 为兼容现有调用方，保留 cam_sensor_config_t、cam_sensor_init(&cfg)
 * 和 cam_sensor_start()/stop() 形状；V4L2 实现在 init 内已启动推流，
 * start/stop 仅作为兼容 no-op/停流入口。
 */
#pragma once

#include <stdint.h>
#include <stddef.h>
#include "esp_err.h"
#include "driver/i2c_master.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================
 * 摄像头配置结构体
 *
 * 在 main.cpp 的 cam_cfg 变量中修改这些值。
 * ============================================================ */
typedef struct {
    int i2c_scl_pin;   /* SCCB (I2C) 时钟引脚, 默认 GPIO 8 */
    int i2c_sda_pin;   /* SCCB (I2C) 数据引脚, 默认 GPIO 7 */
    int pwdn_pin;      /* 电源控制引脚, -1 = 不使用 */
    int reset_pin;     /* 硬复位引脚, -1 = 不使用 */
    int frame_width;   /* 输出宽度, 如 1024 */
    int frame_height;  /* 输出高度, 如 600 */
    i2c_master_bus_handle_t i2c_bus; /* 可选：复用外部 I2C/SCCB 总线 */
} cam_sensor_config_t;

/* ============================================================
 * 生命周期 API
 * ============================================================ */

/** 初始化摄像头并启动 V4L2 推流。config 仅保留兼容，实际 SCCB 复用 BSP_Touch_GetI2CBus()。 */
esp_err_t cam_sensor_init(const cam_sensor_config_t *config);

/** 兼容入口：V4L2 链路已在 init 内 STREAMON。 */
esp_err_t cam_sensor_start(void);

/** 停止推流并释放资源。 */
esp_err_t cam_sensor_stop(void);

/** 释放 cam_sensor_init() 分配的所有资源。 */
esp_err_t cam_sensor_deinit(void);

/* ============================================================
 * 帧访问 API
 * ============================================================ */

/**
 * 阻塞等待下一帧 RGB888 图像就绪。
 *
 * @param[out] data       指向 width × height × 3 字节 PSRAM 缓冲
 * @param[out] size       缓冲字节数 (= width × height × 3)
 * @param[out] width      实际帧宽度
 * @param[out] height     实际帧高度
 * @param[in]  timeout_ms 超时（毫秒）, 0 = 永久等待
 *
 * @return ESP_OK 成功
 *         ESP_ERR_TIMEOUT 超时
 *
 * 重要：调用完毕后必须调用 cam_sensor_return_frame() 归还帧缓冲，
 *       否则后续帧无法捕获。
 */
esp_err_t cam_sensor_get_frame(uint8_t **data, size_t *size,
                                int *width, int *height,
                                uint32_t timeout_ms);

/**
 * 归还帧缓冲 — 将帧标记为可复用。
 *
 * @param data  cam_sensor_get_frame() 返回的指针
 */
esp_err_t cam_sensor_return_frame(uint8_t *data);

int cam_sensor_get_fb_count(void);

#ifdef __cplusplus
}
#endif
