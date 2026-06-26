#pragma once

#include "esp_err.h"
#include "esp_lcd_panel_ops.h"

void BSP_LCD_Init(void);
esp_lcd_panel_handle_t BSP_LCD_GetPanelHandle(void);
esp_err_t BSP_LCD_GetFrameBuffers(void **fb0, void **fb1, void **fb2);
