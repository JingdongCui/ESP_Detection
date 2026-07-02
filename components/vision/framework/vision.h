#pragma once

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 启动视觉链路。采集 + PPA 缩放/转 RGB888 + PPA 搬运到 LCD framebuffer 视频子区域。
 * 必须在 cam_sensor_init() 与 setupUi() 之后调用。 */
esp_err_t vision_start(void);

/* 诊断抓帧：置位一个请求标志，推理任务在下一帧把"进推理前"的整帧原始 RGB888
 * memcpy 到常驻 PSRAM buffer 并做 C2M cache 回写，通过 RTT 打印其地址与字节数，
 * 供主机经 JTAG dump_image 拉成 .bin。由 UI 业务按键（dashboard LOGO 键）触发。 */
void vision_frame_dump_request(void);

#ifdef __cplusplus
}
#endif
