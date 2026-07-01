#pragma once

#include "esp_err.h"
#include "esp_lcd_panel_ops.h"

// 屏幕分辨率（与 bsp_lcd.c 里 EK79007_1024_600_PANEL_60HZ_CONFIG 保持一致）。
// 视频写 framebuffer 子区域时需用 fb 整宽作行 stride，故在此暴露。
#define BSP_LCD_H_RES 1024
#define BSP_LCD_V_RES 600

void BSP_LCD_Init(void);
esp_lcd_panel_handle_t BSP_LCD_GetPanelHandle(void);
esp_err_t BSP_LCD_GetFrameBuffers(void **fb0, void **fb1);

// 背光默认亮度（开机值），UI 侧用独立同值宏保持组件解耦
#define BSP_LCD_BL_DEFAULT_PERCENT 80

// 设置屏幕背光亮度。percent: 0~100，内部按 gamma=2.2 感知曲线映射并保留最低下限。
// percent 越界自动 clamp 到 [0,100]。
// 非线程安全：须从单一任务（LVGL）调用，勿与其它任务并发调用。
void BSP_LCD_SetBrightness(int percent);
