#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "roi_algorithm.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int y_min;
    int sat_approx_max;
    int rgb_delta_max;
    int min_channel_min;
} roi_tuning_thresholds_t;

typedef struct {
    int region_x;
    int region_y;
    int region_w;
    int region_h;
    int sample_count;
    int y_p10;
    int sat_p90;
    int delta_p90;
    int min_channel_p10;
    roi_tuning_thresholds_t before;
    roi_tuning_thresholds_t after;
} roi_tuning_calibration_result_t;

roi_tuning_thresholds_t roi_tuning_get_thresholds(void);
void roi_tuning_set_thresholds(const roi_tuning_thresholds_t *thresholds);
void roi_tuning_apply_thresholds(roi_algorithm_config_t *cfg);
void roi_tuning_set_defaults_from_config(const roi_algorithm_config_t *cfg);
void roi_tuning_request_calibration(void);
bool roi_tuning_consume_calibration_request(void);
bool roi_tuning_calibrate_from_rgb888(
    const uint8_t *rgb888,
    int src_w,
    int src_h,
    int src_stride_bytes,
    roi_tuning_calibration_result_t *result);

#ifdef __cplusplus
}
#endif
