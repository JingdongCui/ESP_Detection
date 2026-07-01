#pragma once

#include <stdint.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

//电机初始化
esp_err_t bsp_motor_init(void);
//电机比较值设置底层
esp_err_t bsp_motor_set_compare(uint8_t motor_id, uint32_t pwm_a_compare, uint32_t pwm_b_compare);
//设置指定电机速度和方向，speed范围0-100，direction为0正转、1反转
esp_err_t bsp_motor_set_speed_direction(uint8_t motor_id, uint32_t speed, uint8_t direction);

#ifdef __cplusplus
}
#endif
