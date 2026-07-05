#pragma once

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

// 启动全屏 UVC 视频流：分配缓冲、初始化 PPA/硬件 JPEG/UVC device。
// 成功后主机(PC)可将本设备识别为 USB 摄像头并拉取 1024x600 MJPEG。
// 需在 BSP_LCD_Init 与 setupUi 之后调用（依赖 DSI framebuffer 已建立）。
esp_err_t screen_uvc_start(void);

#ifdef __cplusplus
}
#endif
