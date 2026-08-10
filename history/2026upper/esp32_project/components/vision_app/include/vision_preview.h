#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "esp_err.h"
#include "lvgl.h"

#ifdef __cplusplus
#include <vector>
#include "app_yolo.hpp"
#endif

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t vision_preview_init(lv_obj_t *parent, int32_t x, int32_t y, int32_t w, int32_t h);
void vision_preview_set_waiting(void);
void vision_preview_bind_labels(lv_obj_t *fps_label, lv_obj_t *status_label);
void vision_preview_set_calibration_result(bool ok);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
esp_err_t vision_preview_update(const uint8_t *rgb888,
                                int src_w,
                                int src_h,
                                const std::vector<Detection> &detections,
                                float fps,
                                const YOLOPerfStats &perf,
                                bool detect_ok);
esp_err_t vision_preview_update_status(const std::vector<Detection> &detections,
                                       float fps,
                                       const YOLOPerfStats &perf,
                                       bool detect_ok);
#endif
