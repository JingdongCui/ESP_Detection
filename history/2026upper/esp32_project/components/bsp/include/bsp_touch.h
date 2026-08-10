#pragma once

#include "esp_err.h"
#include "driver/i2c_master.h"
#include "esp_lcd_touch.h"

#ifdef __cplusplus
extern "C" {
#endif

void BSP_Touch_Init(void);
esp_lcd_touch_handle_t BSP_Touch_GetHandle(void);
i2c_master_bus_handle_t BSP_Touch_GetI2CBus(void);

#ifdef __cplusplus
}
#endif
