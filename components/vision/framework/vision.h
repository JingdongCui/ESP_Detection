#pragma once

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 启动视觉链路。采集 + PPA 缩放/转 RGB888 + PPA 搬运到 LCD framebuffer 视频子区域。
 * 必须在 cam_sensor_init() 与 setupUi() 之后调用。 */
esp_err_t vision_start(void);

#ifdef __cplusplus
}
#endif
