#include "screen_uvc.h"

#include <string.h>
#include <sys/time.h>

#include "esp_log.h"
#include "esp_check.h"
#include "esp_err.h"
#include "esp_heap_caps.h"
#include "esp_cache.h"
#include "esp_timer.h"

#include "driver/ppa.h"
#include "driver/jpeg_encode.h"

#include "bsp_lcd.h"
#include "usb_device_uvc.h"

static const char *TAG = "screen_uvc";

// EK79007 面板分辨率（见 agentic/board.md）。DSI framebuffer 为 RGB888。
#define SCREEN_W            1024
#define SCREEN_H            600
#define RGB_BYTES_PER_PX    3
// 缓冲按最大分辨率(全屏)预分配，可覆盖主机协商到的任何更小尺寸。
#define MAX_RGB_SIZE        ((size_t)SCREEN_W * SCREEN_H * RGB_BYTES_PER_PX)
// JPEG 输出缓冲：1024x600 q80/444 经验峰值 ~300-450KB，取 768KB 留裕量。
#define JPEG_OUT_CAP        (768 * 1024)
#define UVC_XFER_CAP        JPEG_OUT_CAP
#define JPEG_QUALITY        80

static ppa_client_handle_t   s_ppa;             // SRM 客户端（做 rgb_swap + 缩放）
static jpeg_encoder_handle_t s_jpeg;            // 硬件 JPEG 编码器
static uint8_t              *s_rgb;             // PPA 输出 = JPEG 输入（R,G,B，编码器对齐）
static size_t                s_rgb_alloc;       // s_rgb 实际分配大小
static uint8_t              *s_jpeg_out;        // JPEG 码流输出缓冲
static uint8_t              *s_uvc_xfer;        // TinyUSB UVC 传输缓冲
static uvc_fb_t              s_fb;              // 交还给 UVC 的帧描述（复用单缓冲）

// 主机协商到的输出分辨率（start_cb 写入，fb_get 读取）。默认全屏。
static volatile int          s_out_w = SCREEN_W;
static volatile int          s_out_h = SCREEN_H;

// 从 DSI framebuffer 抓一帧 → PPA 硬件缩放并交换 R/B → 输出到 s_rgb（R,G,B，out_w×out_h）。
static esp_err_t capture_screen_rgb(int out_w, int out_h)
{
    void *fb0 = NULL, *fb1 = NULL;
    esp_err_t ret = BSP_LCD_GetFrameBuffers(&fb0, &fb1);
    if (ret != ESP_OK || fb0 == NULL) {
        return (ret == ESP_OK) ? ESP_ERR_INVALID_STATE : ret;
    }

    size_t out_size = (size_t)out_w * out_h * RGB_BYTES_PER_PX;
    // 抓 fb0。DOUBLE_DIRECT 抗撕裂下 LVGL 在两块 fb 间交替刷新，
    // PPA 搬运是亚毫秒级，撕裂窗口极小，用于监控足够。
    ppa_srm_oper_config_t srm = {
        .in = {
            .buffer = fb0,
            .pic_w = SCREEN_W,
            .pic_h = SCREEN_H,
            .block_w = SCREEN_W,
            .block_h = SCREEN_H,
            .block_offset_x = 0,
            .block_offset_y = 0,
            .srm_cm = PPA_SRM_COLOR_MODE_RGB888,
        },
        .out = {
            .buffer = s_rgb,
            .buffer_size = s_rgb_alloc,
            .pic_w = out_w,
            .pic_h = out_h,
            .block_offset_x = 0,
            .block_offset_y = 0,
            .srm_cm = PPA_SRM_COLOR_MODE_RGB888,
        },
        .rotation_angle = PPA_SRM_ROTATION_ANGLE_0,
        .scale_x = (float)out_w / SCREEN_W,
        .scale_y = (float)out_h / SCREEN_H,
        // framebuffer 内存序为 B,G,R；置位后 PPA 硬件把 R/B 交换成编码器要的 R,G,B。
        .rgb_swap = true,
        .mode = PPA_TRANS_MODE_BLOCKING,
    };
    ret = ppa_do_scale_rotate_mirror(s_ppa, &srm);
    if (ret != ESP_OK) {
        return ret;
    }
    // PPA 写的是 PSRAM，回读前把该区间从内存同步进 cache。
    return esp_cache_msync(s_rgb, out_size, ESP_CACHE_MSYNC_FLAG_DIR_M2C);
}

// UVC 主机请求一帧：抓屏(缩放到协商尺寸)→修色→硬件 JPEG，填 s_fb 返回。失败返回 NULL。
static uvc_fb_t *uvc_fb_get_cb(void *cb_ctx)
{
    (void)cb_ctx;
    int out_w = s_out_w, out_h = s_out_h;
    if (capture_screen_rgb(out_w, out_h) != ESP_OK) {
        return NULL;
    }

    jpeg_encode_cfg_t cfg = {
        .width = out_w,
        .height = out_h,
        .src_type = JPEG_ENCODE_IN_FORMAT_RGB888,
        .sub_sample = JPEG_DOWN_SAMPLING_YUV444,   // 444 不做色度下采样，UI/文字最清晰
        .image_quality = JPEG_QUALITY,
    };
    uint32_t out_len = 0;
    size_t in_size = (size_t)out_w * out_h * RGB_BYTES_PER_PX;
    esp_err_t ret = jpeg_encoder_process(s_jpeg, &cfg,
                                         s_rgb, in_size,
                                         s_jpeg_out, JPEG_OUT_CAP, &out_len);
    if (ret != ESP_OK || out_len == 0) {
        ESP_LOGW(TAG, "jpeg encode fail: %s len=%u", esp_err_to_name(ret), (unsigned)out_len);
        return NULL;
    }

    struct timeval tv;
    gettimeofday(&tv, NULL);
    s_fb.buf = s_jpeg_out;
    s_fb.len = out_len;
    s_fb.width = out_w;
    s_fb.height = out_h;
    s_fb.format = UVC_FORMAT_JPEG;
    s_fb.timestamp = tv;
    return &s_fb;
}

static esp_err_t uvc_start_cb(uvc_format_t format, int width, int height, int rate, void *cb_ctx)
{
    (void)cb_ctx;
    ESP_LOGI(TAG, "UVC start: fmt=%d %dx%d @%dfps", (int)format, width, height, rate);
    if (format != UVC_FORMAT_JPEG) {
        ESP_LOGE(TAG, "unsupported UVC format %d (only JPEG)", (int)format);
        return ESP_ERR_NOT_SUPPORTED;
    }
    // 记录主机协商分辨率；夹到全屏上限（缓冲按全屏预分配）。
    if (width <= 0 || width > SCREEN_W || height <= 0 || height > SCREEN_H) {
        ESP_LOGW(TAG, "negotiated %dx%d out of range, clamp to %dx%d", width, height, SCREEN_W, SCREEN_H);
        width = SCREEN_W;
        height = SCREEN_H;
    }
    s_out_w = width;
    s_out_h = height;
    return ESP_OK;
}

static void uvc_fb_return_cb(uvc_fb_t *fb, void *cb_ctx)
{
    (void)fb; (void)cb_ctx;
    // 单缓冲同步模型：TinyUSB 传完本帧才会再次 fb_get，无需额外处理。
}

static void uvc_stop_cb(void *cb_ctx)
{
    (void)cb_ctx;
    ESP_LOGI(TAG, "UVC stop (host closed stream)");
}

esp_err_t screen_uvc_start(void)
{
    // 1) JPEG 编码输入缓冲（同时作 PPA 输出）：按全屏最大尺寸用编码器对齐分配。
    jpeg_encode_memory_alloc_cfg_t in_mem = { .buffer_direction = JPEG_ENC_ALLOC_INPUT_BUFFER };
    s_rgb = (uint8_t *)jpeg_alloc_encoder_mem(MAX_RGB_SIZE, &in_mem, &s_rgb_alloc);
    if (!s_rgb) {
        ESP_LOGE(TAG, "alloc rgb input buffer failed (%u bytes)", (unsigned)MAX_RGB_SIZE);
        return ESP_ERR_NO_MEM;
    }

    // 2) JPEG 输出 + UVC 传输缓冲（PSRAM）。
    s_jpeg_out = (uint8_t *)heap_caps_malloc(JPEG_OUT_CAP, MALLOC_CAP_SPIRAM);
    s_uvc_xfer = (uint8_t *)heap_caps_malloc(UVC_XFER_CAP, MALLOC_CAP_SPIRAM);
    if (!s_jpeg_out || !s_uvc_xfer) {
        ESP_LOGE(TAG, "alloc jpeg/uvc buffer failed");
        return ESP_ERR_NO_MEM;
    }

    // 3) PPA SRM 客户端。
    ppa_client_config_t ppa_cfg = { .oper_type = PPA_OPERATION_SRM };
    ESP_RETURN_ON_ERROR(ppa_register_client(&ppa_cfg, &s_ppa), TAG, "ppa register");

    // 4) 硬件 JPEG 编码引擎。
    jpeg_encode_engine_cfg_t eng = { .intr_priority = 0, .timeout_ms = 100 };
    ESP_RETURN_ON_ERROR(jpeg_new_encoder_engine(&eng, &s_jpeg), TAG, "jpeg engine");

    // 5) 配置并初始化 UVC device。
    uvc_device_config_t cfg = {
        .uvc_buffer = s_uvc_xfer,
        .uvc_buffer_size = UVC_XFER_CAP,
        .start_cb = uvc_start_cb,
        .fb_get_cb = uvc_fb_get_cb,
        .fb_return_cb = uvc_fb_return_cb,
        .stop_cb = uvc_stop_cb,
        .cb_ctx = NULL,
    };
    ESP_RETURN_ON_ERROR(uvc_device_config(0, &cfg), TAG, "uvc config");
    ESP_RETURN_ON_ERROR(uvc_device_init(), TAG, "uvc init");

    ESP_LOGI(TAG, "screen UVC stream started: default %dx%d MJPEG q%d", SCREEN_W, SCREEN_H, JPEG_QUALITY);
    return ESP_OK;
}
