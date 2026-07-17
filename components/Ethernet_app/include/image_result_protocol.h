#pragma once

#include <stdint.h>

#define ESP_HOST_MAGIC                  0x32505345u
#define ESP_HOST_IMAGE_VERSION_V2       2u
#define ESP_HOST_TYPE_IMAGE_RESULT      0x01u
#define ESP_HOST_PIXEL_JPEG             2u
#define ESP_HOST_IMAGE_WIDTH            1024u
#define ESP_HOST_IMAGE_HEIGHT           600u
#define ESP_HOST_IMAGE_META_SIZE        32u
#define ESP_HOST_IMAGE_BOX_SIZE         16u
#define ESP_HOST_IMAGE_MAX_BOXES        8u

#if __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
#error "image result v2 wire protocol requires little-endian target"
#endif

typedef struct __attribute__((packed)) {
    uint32_t magic;
    uint16_t version;
    uint16_t type;
    uint32_t header_size;
    uint32_t seq;
    uint64_t timestamp_ms;
    uint32_t payload_len;
    uint16_t width;
    uint16_t height;
    uint16_t pixel_format;
    uint16_t reserved;
    uint32_t reserved2;
} esp_host_packet_header_t;

typedef struct __attribute__((packed)) {
    uint32_t frame_id;
    uint64_t capture_timestamp_us;
    uint32_t jpeg_len;
    uint16_t box_count;
    uint16_t primary_class_id;
    uint16_t primary_confidence;
    uint16_t infer_time_ms;
    uint16_t source_width;
    uint16_t source_height;
    uint16_t meta_size;
    uint16_t box_size;
} esp_host_image_meta_v2_t;

typedef struct __attribute__((packed)) {
    uint8_t stage;
    uint8_t category;
    uint16_t score;
    uint16_t x1;
    uint16_t y1;
    uint16_t x2;
    uint16_t y2;
    uint16_t flags;
    uint16_t reserved;
} esp_host_image_box_v2_t;

_Static_assert(sizeof(esp_host_packet_header_t) == 40, "packet header must be 40 bytes");
_Static_assert(sizeof(esp_host_image_meta_v2_t) == 32, "image metadata must be 32 bytes");
_Static_assert(sizeof(esp_host_image_box_v2_t) == 16, "image box must be 16 bytes");
