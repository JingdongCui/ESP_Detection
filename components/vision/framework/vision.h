#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
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

void vision_set_detection_enabled(bool enabled);
bool vision_is_detection_enabled(void);
void vision_set_preview_overlay_enabled(bool enabled);
bool vision_is_preview_overlay_enabled(void);

typedef struct {
    int class_id;
    int confidence_pct;
    bool valid;
} vision_classification_t;

bool vision_get_latest_classification(vision_classification_t *out);

esp_err_t vision_copy_latest_frame_scaled_rgb888(uint8_t *dst,
                                                 int dst_w,
                                                 int dst_h,
                                                 size_t dst_capacity,
                                                 int *src_w,
                                                 int *src_h,
                                                 size_t *out_len,
                                                 int64_t *timestamp_us);

// 阻塞等待一张「识别成功边沿·带框」RGB888 快照并拷贝到 dst（640×375×3）。
// 该快照由 vision 检测侧在新包裹首次命中时生成（面单+logo 框已 burn-in）。
// class_id_out 取值 1~3（极兔/韵达/中通），conf_out 为 logo 置信度 0~100。
// 返回 true 表示 dst 已填入一帧；false 表示 timeout_ms 内无新包裹。
bool vision_boxed_snapshot_take(uint8_t *dst, size_t dst_capacity,
                                uint16_t *class_id_out, uint8_t *conf_out,
                                uint32_t timeout_ms);

#ifdef __cplusplus
}
#endif
