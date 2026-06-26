#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int down_w;
    int down_h;

    int y_min;
    int sat_approx_max;
    int rgb_delta_max;
    int min_channel_min;

    int close_w;
    int close_h;
    int close_iter;
    int dilate_w;
    int dilate_h;
    int dilate_iter;

    int min_area_per_10000;
    int max_area_per_10000;
    int min_white_per_10000;

    int padding_per_10000;
    int padding_px;
    int min_roi_w;
    int min_roi_h;
} roi_algorithm_config_t;

typedef struct {
    int x;
    int y;
    int w;
    int h;
    int area_ratio_per_10000;
    int white_ratio_per_10000;
    bool success;
    int fail_reason;
} roi_algorithm_result_t;

typedef struct {
    uint8_t *mask;
    uint8_t *tmp;
    uint16_t *queue;
    size_t pixel_count;
} roi_algorithm_workspace_t;

enum {
    ROI_ALGORITHM_OK = 0,
    ROI_ALGORITHM_FAIL_BAD_ARG = 1,
    ROI_ALGORITHM_FAIL_NO_COMPONENT = 2,
    ROI_ALGORITHM_FAIL_AREA_RANGE = 3,
    ROI_ALGORITHM_FAIL_SPARSE = 4,
    ROI_ALGORITHM_FAIL_TOO_SMALL = 5,
};

roi_algorithm_config_t roi_algorithm_default_config(void);

bool roi_algorithm_detect(
    const uint8_t *rgb888,
    int src_w,
    int src_h,
    int src_stride_bytes,
    const roi_algorithm_config_t *cfg,
    roi_algorithm_workspace_t *work,
    roi_algorithm_result_t *out);

void roi_algorithm_letterbox(
    const uint8_t *rgb888,
    int src_w,
    int src_h,
    int src_stride_bytes,
    const roi_algorithm_result_t *roi,
    uint8_t *dst_rgb888,
    int dst_w,
    int dst_h,
    int dst_stride_bytes,
    uint8_t pad_r,
    uint8_t pad_g,
    uint8_t pad_b);

#ifdef __cplusplus
}
#endif
