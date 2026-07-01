#pragma once

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

// 编码器编号，用于区分 3 路 PCNT 计数资源。
typedef enum {
    BSP_ENCODER_0 = 0,
    BSP_ENCODER_1,
    BSP_ENCODER_2,
} bsp_encoder_id_t;

// 初始化 3 路编码器 PCNT 资源，默认初始化后立即开始计数和周期测速。
esp_err_t bsp_encoder_init(void);
// 读取最近一次采样换算出的车轮速度，单位 cm/s。
esp_err_t bsp_encoder_get_speed_cm_s(bsp_encoder_id_t encoder_id, float *speed_cm_s);

#ifdef __cplusplus
}
#endif
