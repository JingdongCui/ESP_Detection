#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define VISION_UPLOAD_WIDTH 1024
#define VISION_UPLOAD_HEIGHT 600
#define VISION_UPLOAD_RGB_BYTES ((size_t)VISION_UPLOAD_WIDTH * VISION_UPLOAD_HEIGHT * 3)
#define VISION_UPLOAD_MAX_BOXES 8

typedef struct {
    uint8_t stage;
    uint8_t category;
    uint16_t score_x1000;
    uint16_t x1;
    uint16_t y1;
    uint16_t x2;
    uint16_t y2;
} vision_upload_box_t;

typedef struct vision_upload_frame {
    const uint8_t *rgb888;
    size_t rgb_len;
    uint16_t width;
    uint16_t height;
    uint32_t frame_id;
    int64_t capture_timestamp_us;
    vision_upload_box_t boxes[VISION_UPLOAD_MAX_BOXES];
    uint16_t box_count;
    uint16_t primary_class_id;
    uint16_t primary_confidence_x1000;
    uint16_t infer_time_ms;
    void *slot_token;
} vision_upload_frame_t;

typedef bool (*vision_upload_submit_cb_t)(const vision_upload_frame_t *frame, void *ctx);

void vision_upload_set_submit_callback(vision_upload_submit_cb_t cb, void *ctx);
bool vision_upload_frame_begin_encoding(const vision_upload_frame_t *frame);
bool vision_upload_frame_release(const vision_upload_frame_t *frame);
