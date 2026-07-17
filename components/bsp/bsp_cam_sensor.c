/*
 * MIPI CSI 摄像头驱动 —— V4L2 最简实现（风格对齐 bsp_lcd.c）。
 *
 * cam_sensor_init() 一步完成：
 *   esp_video_init(csi) → open(/dev/video) → S_FMT(RGB888)
 *     → REQBUFS + mmap 多缓冲 → QBUF → STREAMON
 * SCCB 复用触摸已建好的 I2C 总线（GPIO8/7 共享）。
 * 硬件参数全部 #define 在此，system_init 只调一行 cam_sensor_init()。
 */
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_ipa.h"
#include "esp_ipa_detect.h"
#include "esp_log.h"
#include "esp_video_init.h"
#include "esp_video_device.h"
#include "esp_video_isp_ioctl.h"
#include "linux/videodev2.h"
#include "bsp_touch.h"
#include "bsp_cam_sensor.h"

// V4L2 帧缓冲数量。复刻 ESP-who 零拷贝 peek：vision 层 ringbuf 深度 = CAM_SENSOR_FB_COUNT-2
// （留 2 个给驱动：1 个正在 DMA 写、1 个排队待命），推理帧在被回收前的安全距离即此深度。
// 改大可线性增大防竞态时间窗口，代价为每缓冲多占 w×h×2 字节 PSRAM。详见
// docs/零拷贝peek竞态风险.md。此宏是缓冲数的唯一来源，vision 经 getter 读取。
#define CAM_FPS_DIV_MODE  1       // 0=30fps，1=15fps（CSI 驱动通过 skip_frames 只支持整除降帧）
#define CAM_SCCB_FREQ     100000  // SCCB（摄像头控制总线）时钟频率 100 kHz

#if CAM_FPS_DIV_MODE == 0
#define CAM_TARGET_FPS    30
#elif CAM_FPS_DIV_MODE == 1
#define CAM_TARGET_FPS    15
#else
#error "CAM_FPS_DIV_MODE must be 0 (30fps) or 1 (15fps)"
#endif
#define CAM_RESET_PIN   -1       // 复位引脚，-1 表示硬件未接（不由驱动控制）
#define CAM_PWDN_PIN    -1       // 掉电引脚，-1 表示硬件未接（不由驱动控制）

#define ESP_IPA_UI_OVERRIDE_NAME "esp_ipa_ui_override"
#define CAM_ISP_ECO4_CONTRAST_BASELINE   132U
#define CAM_ISP_ECO4_SATURATION_BASELINE 136U
#define CAM_ISP_STATUS_PERIOD_MS         500
#define CAM_ISP_TASK_STACK_SIZE          1152
#define CAM_ISP_TASK_PRIORITY            4
#define CAM_ISP_STOP_WAIT_MS             1000

static const char *TAG = "bsp_cam";

typedef struct {
    bool brightness_manual;
    bool contrast_manual;
    bool saturation_manual;
    int32_t brightness;
    uint8_t contrast;
    uint8_t saturation;
} cam_isp_override_target_t;

typedef struct {
    bool brightness_valid;
    bool contrast_valid;
    bool saturation_valid;
    int32_t brightness;
    uint8_t contrast;
    uint8_t saturation;
} cam_isp_override_runtime_t;

typedef struct {
    esp_ipa_t base;
    bool contrast_was_manual;
    bool saturation_was_manual;
    bool auto_contrast_valid;
    bool auto_saturation_valid;
    uint32_t auto_contrast;
    uint32_t auto_saturation;
    cam_isp_override_runtime_t published_runtime;
} cam_isp_ui_override_t;

static portMUX_TYPE s_isp_override_lock = portMUX_INITIALIZER_UNLOCKED;
static cam_isp_override_target_t s_isp_override_target;
static cam_isp_override_runtime_t s_isp_override_runtime;

static int s_fd = -1;                      // /dev/video 文件描述符，-1 表示未打开
static int s_sensor_ctrl_fd = -1;          // 仅 ISP worker 使用的 Sensor 控制 fd
static int s_isp_ctrl_fd = -1;             // 仅 ISP worker 使用的 ISP 控制 fd
static TaskHandle_t s_isp_task;
static portMUX_TYPE s_isp_state_lock = portMUX_INITIALIZER_UNLOCKED;
static cam_sensor_isp_state_t s_isp_state;
static bool s_hue_pending;
static int32_t s_hue_target;
static bool s_isp_stop_requested;
static bool s_isp_task_stopped;
static bool s_exposure_supported;
static bool s_gain_menu_supported;
static bool s_red_balance_supported;
static bool s_blue_balance_supported;
static bool s_video_initialized;
static bool s_streaming;
static bool s_cam_initialized;
static int s_width;                        // 摄像头输出宽度（由 G_FMT 读取）
static int s_height;                       // 摄像头输出高度（由 G_FMT 读取）
static void *s_fb[CAM_SENSOR_FB_COUNT];           // mmap 缓冲首地址
static size_t s_fb_len[CAM_SENSOR_FB_COUNT];      // mmap 缓冲字节数

static esp_err_t cam_isp_ui_override_init(esp_ipa_t *ipa,
        const esp_ipa_sensor_t *sensor,
        esp_ipa_metadata_t *metadata)
{
    cam_isp_ui_override_t *self = (cam_isp_ui_override_t *)ipa;

    (void)sensor;
    (void)metadata;

    self->auto_contrast_valid = true;
    self->auto_saturation_valid = true;
    self->auto_contrast = CAM_ISP_ECO4_CONTRAST_BASELINE;
    self->auto_saturation = CAM_ISP_ECO4_SATURATION_BASELINE;

    self->published_runtime.contrast_valid = true;
    self->published_runtime.saturation_valid = true;
    self->published_runtime.contrast = CAM_ISP_ECO4_CONTRAST_BASELINE;
    self->published_runtime.saturation = CAM_ISP_ECO4_SATURATION_BASELINE;

    portENTER_CRITICAL(&s_isp_override_lock);
    s_isp_override_runtime = self->published_runtime;
    portEXIT_CRITICAL(&s_isp_override_lock);

    return ESP_OK;
}

static void cam_isp_ui_override_process(esp_ipa_t *ipa,
        const esp_ipa_stats_t *stats,
        const esp_ipa_sensor_t *sensor,
        esp_ipa_metadata_t *metadata)
{
    cam_isp_ui_override_t *self = (cam_isp_ui_override_t *)ipa;
    cam_isp_override_target_t target;
    cam_isp_override_runtime_t runtime;

    (void)stats;
    (void)sensor;

    portENTER_CRITICAL(&s_isp_override_lock);
    target = s_isp_override_target;
    portEXIT_CRITICAL(&s_isp_override_lock);

    if (!self->contrast_was_manual && (metadata->flags & IPA_METADATA_FLAGS_CN)) {
        self->auto_contrast = metadata->contrast;
        self->auto_contrast_valid = true;
    }
    if (!self->saturation_was_manual && (metadata->flags & IPA_METADATA_FLAGS_ST)) {
        self->auto_saturation = metadata->saturation;
        self->auto_saturation_valid = true;
    }

    if (target.brightness_manual) {
        metadata->brightness = target.brightness;
        metadata->flags |= IPA_METADATA_FLAGS_BR;
    }

    if (target.contrast_manual) {
        metadata->contrast = target.contrast;
        metadata->flags |= IPA_METADATA_FLAGS_CN;
    } else if (self->contrast_was_manual && self->auto_contrast_valid) {
        metadata->contrast = self->auto_contrast;
        metadata->flags |= IPA_METADATA_FLAGS_CN;
    }

    if (target.saturation_manual) {
        metadata->saturation = target.saturation;
        metadata->flags |= IPA_METADATA_FLAGS_ST;
    } else if (self->saturation_was_manual && self->auto_saturation_valid) {
        metadata->saturation = self->auto_saturation;
        metadata->flags |= IPA_METADATA_FLAGS_ST;
    }

    runtime.brightness_valid = target.brightness_manual;
    runtime.contrast_valid = target.contrast_manual || self->auto_contrast_valid;
    runtime.saturation_valid = target.saturation_manual || self->auto_saturation_valid;
    runtime.brightness = target.brightness;
    runtime.contrast = target.contrast_manual ? target.contrast : self->auto_contrast;
    runtime.saturation = target.saturation_manual ? target.saturation : self->auto_saturation;

    if (self->published_runtime.brightness_valid != runtime.brightness_valid ||
            self->published_runtime.contrast_valid != runtime.contrast_valid ||
            self->published_runtime.saturation_valid != runtime.saturation_valid ||
            self->published_runtime.brightness != runtime.brightness ||
            self->published_runtime.contrast != runtime.contrast ||
            self->published_runtime.saturation != runtime.saturation) {
        self->published_runtime = runtime;
        portENTER_CRITICAL(&s_isp_override_lock);
        s_isp_override_runtime = runtime;
        portEXIT_CRITICAL(&s_isp_override_lock);
    }

    self->contrast_was_manual = target.contrast_manual;
    self->saturation_was_manual = target.saturation_manual;
}

static void cam_isp_ui_override_destroy(esp_ipa_t *ipa)
{
    free(ipa);
}

static const esp_ipa_ops_t s_cam_isp_ui_override_ops = {
    .init = cam_isp_ui_override_init,
    .process = cam_isp_ui_override_process,
    .destroy = cam_isp_ui_override_destroy,
};

ESP_IPA_DETECT_FN(esp_ipa_ui_override, ESP_IPA_UI_OVERRIDE_NAME)
{
    (void)config;

    cam_isp_ui_override_t *self = calloc(1, sizeof(*self));
    if (self) {
        self->base.name = ESP_IPA_UI_OVERRIDE_NAME;
        self->base.ops = &s_cam_isp_ui_override_ops;
    }

    return self ? &self->base : NULL;
}

static esp_err_t query_control_info(int fd, uint32_t id,
                                    cam_sensor_isp_control_info_t *info)
{
    if (!info) {
        return ESP_ERR_INVALID_ARG;
    }

    *info = (cam_sensor_isp_control_info_t){0};
    if (fd < 0) {
        return ESP_ERR_INVALID_STATE;
    }

    struct v4l2_query_ext_ctrl query = { .id = id };
    if (ioctl(fd, VIDIOC_QUERY_EXT_CTRL, &query) != 0 ||
            (query.flags & V4L2_CTRL_FLAG_DISABLED) != 0) {
        return ESP_ERR_NOT_SUPPORTED;
    }
    if (query.minimum < INT32_MIN || query.minimum > INT32_MAX ||
            query.maximum < INT32_MIN || query.maximum > INT32_MAX ||
            query.default_value < INT32_MIN || query.default_value > INT32_MAX ||
            query.minimum > query.maximum) {
        return ESP_ERR_NOT_SUPPORTED;
    }

    uint64_t step = query.step ? query.step : 1;
    if (step > INT32_MAX) {
        return ESP_ERR_NOT_SUPPORTED;
    }

    info->supported = true;
    info->minimum = (int32_t)query.minimum;
    info->maximum = (int32_t)query.maximum;
    info->step = (int32_t)step;
    info->default_value = (int32_t)query.default_value;
    return ESP_OK;
}

static int32_t align_control_value(const cam_sensor_isp_control_info_t *info,
                                   int32_t value)
{
    int64_t minimum = info->minimum;
    int64_t maximum = info->maximum;
    int64_t aligned = value;
    int64_t step = info->step > 0 ? info->step : 1;

    if (aligned < minimum) {
        aligned = minimum;
    } else if (aligned > maximum) {
        aligned = maximum;
    }

    aligned = minimum + ((aligned - minimum) / step) * step;
    if (aligned > maximum) {
        aligned = maximum;
    }
    return (int32_t)aligned;
}

static esp_err_t get_scalar_control(int fd, uint32_t id, int32_t *value)
{
    if (!value) {
        return ESP_ERR_INVALID_ARG;
    }
    if (fd < 0) {
        return ESP_ERR_INVALID_STATE;
    }

    struct v4l2_ext_control control = { .id = id };
    struct v4l2_ext_controls controls = {
        .count = 1,
        .controls = &control,
    };
    if (ioctl(fd, VIDIOC_G_EXT_CTRLS, &controls) != 0) {
        return ESP_FAIL;
    }

    *value = control.value;
    return ESP_OK;
}

static esp_err_t set_scalar_control(int fd, uint32_t id, int32_t value)
{
    if (fd < 0) {
        return ESP_ERR_INVALID_STATE;
    }

    struct v4l2_ext_control control = {
        .id = id,
        .value = value,
    };
    struct v4l2_ext_controls controls = {
        .count = 1,
        .controls = &control,
    };
    return ioctl(fd, VIDIOC_S_EXT_CTRLS, &controls) == 0 ? ESP_OK : ESP_FAIL;
}

static void cam_isp_sample_status(void)
{
    cam_sensor_isp_state_t sample;
    portENTER_CRITICAL(&s_isp_state_lock);
    sample = s_isp_state;
    portEXIT_CRITICAL(&s_isp_state_lock);
    sample.exposure_valid = false;
    sample.gain_valid = false;
    sample.white_balance_valid = false;

    int32_t value;
    if (s_exposure_supported &&
            get_scalar_control(s_sensor_ctrl_fd, V4L2_CID_EXPOSURE, &value) == ESP_OK && value >= 0) {
        uint64_t exposure_us = (uint64_t)(uint32_t)value * 100U;
        sample.exposure_us = exposure_us > UINT32_MAX ? UINT32_MAX : (uint32_t)exposure_us;
        sample.exposure_valid = true;
    }

    if (s_gain_menu_supported &&
            get_scalar_control(s_sensor_ctrl_fd, V4L2_CID_GAIN, &value) == ESP_OK && value >= 0) {
        struct v4l2_querymenu menu = {
            .id = V4L2_CID_GAIN,
            .index = (uint32_t)value,
        };
        if (ioctl(s_sensor_ctrl_fd, VIDIOC_QUERYMENU, &menu) == 0 &&
                menu.value >= 0 && (uint64_t)menu.value <= UINT32_MAX) {
            sample.gain_x1000 = (uint32_t)menu.value;
            sample.gain_valid = true;
        }
    }

    int32_t red;
    int32_t blue;
    if (s_red_balance_supported && s_blue_balance_supported &&
            get_scalar_control(s_isp_ctrl_fd, V4L2_CID_RED_BALANCE, &red) == ESP_OK && red >= 0 &&
            get_scalar_control(s_isp_ctrl_fd, V4L2_CID_BLUE_BALANCE, &blue) == ESP_OK && blue >= 0) {
        uint64_t red_x1000 = ((uint64_t)(uint32_t)red * 1000U + V4L2_CID_RED_BALANCE_DEN / 2U) /
                             V4L2_CID_RED_BALANCE_DEN;
        uint64_t blue_x1000 = ((uint64_t)(uint32_t)blue * 1000U + V4L2_CID_RED_BALANCE_DEN / 2U) /
                              V4L2_CID_RED_BALANCE_DEN;
        if (red_x1000 <= UINT32_MAX && blue_x1000 <= UINT32_MAX) {
            sample.red_gain_x1000 = (uint32_t)red_x1000;
            sample.blue_gain_x1000 = (uint32_t)blue_x1000;
            sample.white_balance_valid = true;
        }
    }

    portENTER_CRITICAL(&s_isp_state_lock);
    s_isp_state.exposure_valid = sample.exposure_valid;
    s_isp_state.exposure_us = sample.exposure_us;
    s_isp_state.gain_valid = sample.gain_valid;
    s_isp_state.gain_x1000 = sample.gain_x1000;
    s_isp_state.white_balance_valid = sample.white_balance_valid;
    s_isp_state.red_gain_x1000 = sample.red_gain_x1000;
    s_isp_state.blue_gain_x1000 = sample.blue_gain_x1000;
    portEXIT_CRITICAL(&s_isp_state_lock);
}

static void cam_isp_worker(void *arg)
{
    (void)arg;
    const TickType_t period_ticks = pdMS_TO_TICKS(CAM_ISP_STATUS_PERIOD_MS);
    TickType_t next_sample = xTaskGetTickCount() + period_ticks;

    while (true) {
        TickType_t now = xTaskGetTickCount();
        TickType_t wait_ticks = (int32_t)(next_sample - now) > 0 ? next_sample - now : 0;
        (void)ulTaskNotifyTake(pdTRUE, wait_ticks);

        bool stop;
        bool hue_pending;
        int32_t hue_target;
        portENTER_CRITICAL(&s_isp_state_lock);
        stop = s_isp_stop_requested;
        hue_pending = s_hue_pending;
        hue_target = s_hue_target;
        s_hue_pending = false;
        portEXIT_CRITICAL(&s_isp_state_lock);

        if (stop) {
            break;
        }

        if (hue_pending) {
            if (set_scalar_control(s_isp_ctrl_fd, V4L2_CID_HUE, hue_target) == ESP_OK) {
                portENTER_CRITICAL(&s_isp_state_lock);
                s_isp_state.hue = hue_target;
                s_isp_state.hue_valid = true;
                portEXIT_CRITICAL(&s_isp_state_lock);
            } else {
                ESP_LOGE(TAG, "set hue=%ld failed: errno=%d", (long)hue_target, errno);
            }
        }

        now = xTaskGetTickCount();
        if ((int32_t)(now - next_sample) >= 0) {
            cam_isp_sample_status();
            do {
                next_sample += period_ticks;
            } while ((int32_t)(now - next_sample) >= 0);
        }
    }

    portENTER_CRITICAL(&s_isp_state_lock);
    s_isp_task_stopped = true;
    s_isp_task = NULL;
    portEXIT_CRITICAL(&s_isp_state_lock);
    vTaskDelete(NULL);
}

static void cam_isp_reset_state(void)
{
    portENTER_CRITICAL(&s_isp_state_lock);
    s_isp_state = (cam_sensor_isp_state_t){0};
    s_hue_pending = false;
    s_hue_target = 0;
    s_isp_stop_requested = false;
    s_isp_task_stopped = false;
    s_exposure_supported = false;
    s_gain_menu_supported = false;
    s_red_balance_supported = false;
    s_blue_balance_supported = false;
    portEXIT_CRITICAL(&s_isp_state_lock);

    portENTER_CRITICAL(&s_isp_override_lock);
    s_isp_override_target = (cam_isp_override_target_t){0};
    s_isp_override_runtime = (cam_isp_override_runtime_t){0};
    portEXIT_CRITICAL(&s_isp_override_lock);
}

static esp_err_t cam_isp_init_controls(void)
{
    cam_sensor_isp_state_t state = {
        .contrast_auto = true,
        .saturation_auto = true,
    };

    s_sensor_ctrl_fd = open(ESP_VIDEO_MIPI_CSI_DEVICE_NAME, O_RDWR);
    if (s_sensor_ctrl_fd < 0) {
        ESP_LOGW(TAG, "open sensor control %s failed: errno=%d",
                 ESP_VIDEO_MIPI_CSI_DEVICE_NAME, errno);
    } else {
        struct v4l2_query_ext_ctrl query = { .id = V4L2_CID_EXPOSURE };
        s_exposure_supported = ioctl(s_sensor_ctrl_fd, VIDIOC_QUERY_EXT_CTRL, &query) == 0 &&
                               (query.flags & V4L2_CTRL_FLAG_DISABLED) == 0;

        query = (struct v4l2_query_ext_ctrl){ .id = V4L2_CID_GAIN };
        s_gain_menu_supported = ioctl(s_sensor_ctrl_fd, VIDIOC_QUERY_EXT_CTRL, &query) == 0 &&
                                (query.flags & V4L2_CTRL_FLAG_DISABLED) == 0 &&
                                query.type == V4L2_CTRL_TYPE_INTEGER_MENU;
    }

    s_isp_ctrl_fd = open(ESP_VIDEO_ISP1_DEVICE_NAME, O_RDWR);
    if (s_isp_ctrl_fd < 0) {
        ESP_LOGW(TAG, "open ISP control %s failed: errno=%d",
                 ESP_VIDEO_ISP1_DEVICE_NAME, errno);
    } else {
        (void)query_control_info(s_isp_ctrl_fd, V4L2_CID_BRIGHTNESS, &state.brightness_info);
        (void)query_control_info(s_isp_ctrl_fd, V4L2_CID_CONTRAST, &state.contrast_info);
        (void)query_control_info(s_isp_ctrl_fd, V4L2_CID_SATURATION, &state.saturation_info);
        (void)query_control_info(s_isp_ctrl_fd, V4L2_CID_HUE, &state.hue_info);

        struct v4l2_query_ext_ctrl query = { .id = V4L2_CID_RED_BALANCE };
        s_red_balance_supported = ioctl(s_isp_ctrl_fd, VIDIOC_QUERY_EXT_CTRL, &query) == 0 &&
                                  (query.flags & V4L2_CTRL_FLAG_DISABLED) == 0;
        query = (struct v4l2_query_ext_ctrl){ .id = V4L2_CID_BLUE_BALANCE };
        s_blue_balance_supported = ioctl(s_isp_ctrl_fd, VIDIOC_QUERY_EXT_CTRL, &query) == 0 &&
                                   (query.flags & V4L2_CTRL_FLAG_DISABLED) == 0;

        if (state.hue_info.supported) {
            int32_t hue;
            if (get_scalar_control(s_isp_ctrl_fd, V4L2_CID_HUE, &hue) != ESP_OK) {
                hue = state.hue_info.default_value;
            }
            hue = align_control_value(&state.hue_info, hue);
            state.hue = hue;
            state.hue_valid = true;
            s_hue_target = hue;
        }
    }

    cam_isp_override_target_t target = {0};
    portENTER_CRITICAL(&s_isp_override_lock);
    cam_isp_override_runtime_t runtime = s_isp_override_runtime;
    target.brightness_manual = state.brightness_info.supported;
    target.brightness = runtime.brightness_valid ? runtime.brightness :
                        (state.brightness_info.supported ?
                         align_control_value(&state.brightness_info, state.brightness_info.default_value) : 0);
    runtime.brightness_valid = state.brightness_info.supported;
    runtime.brightness = target.brightness;
    target.contrast = runtime.contrast_valid ? runtime.contrast :
                      (state.contrast_info.supported ?
                       (uint8_t)align_control_value(&state.contrast_info, state.contrast_info.default_value) : 0);
    target.saturation = runtime.saturation_valid ? runtime.saturation :
                        (state.saturation_info.supported ?
                         (uint8_t)align_control_value(&state.saturation_info, state.saturation_info.default_value) : 0);
    s_isp_override_target = target;
    s_isp_override_runtime = runtime;
    portEXIT_CRITICAL(&s_isp_override_lock);

    portENTER_CRITICAL(&s_isp_state_lock);
    s_isp_state = state;
    portEXIT_CRITICAL(&s_isp_state_lock);

    if (s_sensor_ctrl_fd < 0 && s_isp_ctrl_fd < 0) {
        return ESP_OK;
    }

    if (xTaskCreate(cam_isp_worker, "cam_isp", CAM_ISP_TASK_STACK_SIZE, NULL,
                    CAM_ISP_TASK_PRIORITY, &s_isp_task) != pdPASS) {
        s_isp_task = NULL;
        ESP_LOGE(TAG, "create ISP worker failed");
        return ESP_ERR_NO_MEM;
    }
    return ESP_OK;
}

static esp_err_t cam_isp_stop_worker(void)
{
    portENTER_CRITICAL(&s_isp_state_lock);
    TaskHandle_t task = s_isp_task;
    s_cam_initialized = false;
    if (task) {
        s_isp_stop_requested = true;
    }
    portEXIT_CRITICAL(&s_isp_state_lock);
    if (task) {
        xTaskNotifyGive(task);
    }
    if (!task) {
        return ESP_OK;
    }

    TickType_t waited = 0;
    const TickType_t delay_ticks = pdMS_TO_TICKS(1) > 0 ? pdMS_TO_TICKS(1) : 1;
    const TickType_t timeout_ticks = pdMS_TO_TICKS(CAM_ISP_STOP_WAIT_MS);
    while (waited < timeout_ticks) {
        bool stopped;
        portENTER_CRITICAL(&s_isp_state_lock);
        stopped = s_isp_task_stopped;
        portEXIT_CRITICAL(&s_isp_state_lock);
        if (stopped) {
            return ESP_OK;
        }
        vTaskDelay(delay_ticks);
        waited += delay_ticks;
    }

    ESP_LOGE(TAG, "ISP worker stop timed out; control fds kept open");
    return ESP_ERR_TIMEOUT;
}

static esp_err_t cam_sensor_cleanup(void)
{
    esp_err_t result = cam_isp_stop_worker();
    if (result != ESP_OK) {
        return result;
    }
    if (s_sensor_ctrl_fd >= 0) {
        close(s_sensor_ctrl_fd);
        s_sensor_ctrl_fd = -1;
    }
    if (s_isp_ctrl_fd >= 0) {
        close(s_isp_ctrl_fd);
        s_isp_ctrl_fd = -1;
    }

    if (s_fd >= 0 && s_streaming) {
        enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        if (ioctl(s_fd, VIDIOC_STREAMOFF, &type) != 0 && result == ESP_OK) {
            result = ESP_FAIL;
        }
        s_streaming = false;
    }
    for (int i = 0; i < CAM_SENSOR_FB_COUNT; i++) {
        if (s_fb[i] && s_fb[i] != MAP_FAILED) {
            munmap(s_fb[i], s_fb_len[i]);
        }
        s_fb[i] = NULL;
        s_fb_len[i] = 0;
    }
    if (s_fd >= 0) {
        close(s_fd);
        s_fd = -1;
    }
    if (s_video_initialized) {
        esp_video_deinit();
        s_video_initialized = false;
    }
    s_width = 0;
    s_height = 0;
    if (!s_isp_task) {
        cam_isp_reset_state();
    }
    return result;
}

// 摄像头初始化：esp_video_init(csi) → open → S_FMT(RGB888)
//   → REQBUFS + mmap 多缓冲 → QBUF → STREAMON。
// 调用前提：BSP_Touch_Init 已建好 I2C 总线（SCCB 复用之）。
esp_err_t cam_sensor_init(void)
{
    if (s_cam_initialized || s_video_initialized || s_fd >= 0 || s_isp_task) {
        return ESP_ERR_INVALID_STATE;
    }

    cam_isp_reset_state();
    i2c_master_bus_handle_t i2c_bus = BSP_Touch_GetI2CBus();
    if (!i2c_bus) {
        ESP_LOGE(TAG, "I2C bus not ready (call BSP_Touch_Init first)");
        return ESP_ERR_INVALID_STATE;
    }

    // SCCB 复用触摸的 I2C 总线（init_sccb=false）
    esp_video_init_csi_config_t csi_config = {
        .sccb_config = {
            .init_sccb = false,
            .i2c_handle = i2c_bus,
            .freq = CAM_SCCB_FREQ,
        },
        .reset_pin = CAM_RESET_PIN,
        .pwdn_pin = CAM_PWDN_PIN,
    };
    esp_video_init_config_t cam_config = { .csi = &csi_config };
    esp_err_t ret = esp_video_init(&cam_config);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "esp_video_init failed: %s", esp_err_to_name(ret));
        return ret;
    }
    s_video_initialized = true;

    // 打开 MIPI CSI 视频设备节点（只读，仅负责采集）。
    s_fd = open(ESP_VIDEO_MIPI_CSI_DEVICE_NAME, O_RDONLY);
    if (s_fd < 0) {
        ESP_LOGE(TAG, "open %s failed: errno=%d", ESP_VIDEO_MIPI_CSI_DEVICE_NAME, errno);
        ret = ESP_FAIL;
        goto fail;
    }

    // 先 G_FMT 读出驱动默认分辨率，再 S_FMT 把像素格式改为 RGB888
    struct v4l2_format format = { .type = V4L2_BUF_TYPE_VIDEO_CAPTURE };
    if (ioctl(s_fd, VIDIOC_G_FMT, &format) != 0) {
        ESP_LOGE(TAG, "VIDIOC_G_FMT failed");
        ret = ESP_FAIL;
        goto fail;
    }
    s_width = format.fmt.pix.width;
    s_height = format.fmt.pix.height;
    // 用 RGB888（V4L2_PIX_FMT_RGB24）而非 RGB565：让 ISP 全程按 8bit/通道输出，
    // 避免输出前砍成 16bit 再由 PPA 补零扩回 24bit（白只到 248、色阶变粗、画面发灰）。
    format.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB24;
    if (ioctl(s_fd, VIDIOC_S_FMT, &format) != 0) {
        ESP_LOGE(TAG, "VIDIOC_S_FMT RGB888 failed");
        ret = ESP_FAIL;
        goto fail;
    }

    struct v4l2_streamparm parm = { .type = V4L2_BUF_TYPE_VIDEO_CAPTURE };
    parm.parm.capture.capability = V4L2_CAP_TIMEPERFRAME;
    parm.parm.capture.timeperframe.numerator = 1;
    parm.parm.capture.timeperframe.denominator = CAM_TARGET_FPS;
    if (ioctl(s_fd, VIDIOC_S_PARM, &parm) != 0) {
        ESP_LOGW(TAG, "VIDIOC_S_PARM %dfps unsupported, keep sensor default", CAM_TARGET_FPS);
    } else if (ioctl(s_fd, VIDIOC_G_PARM, &parm) != 0) {
        ESP_LOGW(TAG, "VIDIOC_G_PARM failed after setting fps");
    }

    // 申请缓冲（REQBUFS），再逐个 QUERYBUF 取偏移、mmap 映射到用户空间、
    // QBUF 入队等待填充。三步缺一不可。
    struct v4l2_requestbuffers req = {
        .count = CAM_SENSOR_FB_COUNT,
        .type = V4L2_BUF_TYPE_VIDEO_CAPTURE,
        .memory = V4L2_MEMORY_MMAP,
    };
    if (ioctl(s_fd, VIDIOC_REQBUFS, &req) != 0) {
        ESP_LOGE(TAG, "VIDIOC_REQBUFS failed");
        ret = ESP_FAIL;
        goto fail;
    }
    for (int i = 0; i < CAM_SENSOR_FB_COUNT; i++) {
        struct v4l2_buffer buf = {
            .type = V4L2_BUF_TYPE_VIDEO_CAPTURE,
            .memory = V4L2_MEMORY_MMAP,
            .index = i,
        };
        if (ioctl(s_fd, VIDIOC_QUERYBUF, &buf) != 0) {
            ESP_LOGE(TAG, "VIDIOC_QUERYBUF %d failed", i);
            ret = ESP_FAIL;
            goto fail;
        }
        s_fb_len[i] = buf.length;
        s_fb[i] = mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, s_fd, buf.m.offset);
        if (s_fb[i] == MAP_FAILED) {
            ESP_LOGE(TAG, "mmap %d failed", i);
            s_fb[i] = NULL;
            ret = ESP_FAIL;
            goto fail;
        }
        if (ioctl(s_fd, VIDIOC_QBUF, &buf) != 0) {
            ESP_LOGE(TAG, "VIDIOC_QBUF %d failed", i);
            ret = ESP_FAIL;
            goto fail;
        }
    }

    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(s_fd, VIDIOC_STREAMON, &type) != 0) {
        ESP_LOGE(TAG, "VIDIOC_STREAMON failed");
        ret = ESP_FAIL;
        goto fail;
    }
    s_streaming = true;

    // 控制 fd 在流配置完成后打开；初始化查询完成后由 worker 独占 ioctl。
    ret = cam_isp_init_controls();
    if (ret != ESP_OK) {
        goto fail;
    }

    portENTER_CRITICAL(&s_isp_state_lock);
    s_cam_initialized = true;
    portEXIT_CRITICAL(&s_isp_state_lock);
    ESP_LOGI(TAG, "camera init: %dx%d RGB888, %d buffers, %dfps, streaming",
             s_width, s_height, CAM_SENSOR_FB_COUNT,
             parm.parm.capture.timeperframe.denominator / parm.parm.capture.timeperframe.numerator);
    return ESP_OK;

fail:
    {
        esp_err_t cleanup_ret = cam_sensor_cleanup();
        return cleanup_ret == ESP_OK ? ret : cleanup_ret;
    }
}

// 取一帧：VIDIOC_DQBUF 从队列出队一个已填充的缓冲。
// 返回的 data 指向 mmap 区域（零拷贝），用完必须调 cam_sensor_return_frame 归还。
// timeout_ms 当前未使用——V4L2 DQBUF 会阻塞至帧就绪。
esp_err_t cam_sensor_get_frame(uint8_t **data, size_t *size,
                               int *width, int *height,
                               uint32_t timeout_ms)
{
    (void)timeout_ms;  // V4L2 DQBUF 阻塞至帧就绪
    struct v4l2_buffer buf = {
        .type = V4L2_BUF_TYPE_VIDEO_CAPTURE,
        .memory = V4L2_MEMORY_MMAP,
    };
    if (ioctl(s_fd, VIDIOC_DQBUF, &buf) != 0) {
        return ESP_FAIL;
    }
    if (data)   *data = (uint8_t *)s_fb[buf.index];
    if (size)   *size = buf.bytesused;
    if (width)  *width = s_width;
    if (height) *height = s_height;
    return ESP_OK;
}

// 归还帧：按缓冲地址反查 index，VIDIOC_QBUF 重新入队供后续复用。
// data 必须是 cam_sensor_get_frame 返回过的指针，否则返回 ESP_ERR_INVALID_ARG。
esp_err_t cam_sensor_return_frame(uint8_t *data)
{
    for (int i = 0; i < CAM_SENSOR_FB_COUNT; i++) {
        if (s_fb[i] == (void *)data) {
            struct v4l2_buffer buf = {
                .type = V4L2_BUF_TYPE_VIDEO_CAPTURE,
                .memory = V4L2_MEMORY_MMAP,
                .index = i,
            };
            return ioctl(s_fd, VIDIOC_QBUF, &buf) == 0 ? ESP_OK : ESP_FAIL;
        }
    }
    return ESP_ERR_INVALID_ARG;
}

esp_err_t cam_sensor_isp_get_state(cam_sensor_isp_state_t *state)
{
    if (!state) {
        return ESP_ERR_INVALID_ARG;
    }

    portENTER_CRITICAL(&s_isp_state_lock);
    if (!s_cam_initialized) {
        portEXIT_CRITICAL(&s_isp_state_lock);
        return ESP_ERR_INVALID_STATE;
    }
    *state = s_isp_state;
    portEXIT_CRITICAL(&s_isp_state_lock);

    portENTER_CRITICAL(&s_isp_override_lock);
    cam_isp_override_runtime_t runtime = s_isp_override_runtime;
    portEXIT_CRITICAL(&s_isp_override_lock);
    state->brightness_valid = runtime.brightness_valid;
    state->contrast_valid = runtime.contrast_valid;
    state->saturation_valid = runtime.saturation_valid;
    if (runtime.brightness_valid) {
        state->brightness = runtime.brightness;
    }
    if (runtime.contrast_valid) {
        state->contrast = runtime.contrast;
    }
    if (runtime.saturation_valid) {
        state->saturation = runtime.saturation;
    }
    return ESP_OK;
}

static esp_err_t cam_isp_set_auto(bool contrast, bool enabled)
{
    portENTER_CRITICAL(&s_isp_state_lock);
    if (!s_cam_initialized) {
        portEXIT_CRITICAL(&s_isp_state_lock);
        return ESP_ERR_INVALID_STATE;
    }
    bool supported = contrast ? s_isp_state.contrast_info.supported :
                     s_isp_state.saturation_info.supported;
    if (!supported) {
        portEXIT_CRITICAL(&s_isp_state_lock);
        return ESP_ERR_NOT_SUPPORTED;
    }

    portENTER_CRITICAL(&s_isp_override_lock);
    if (contrast) {
        if (!enabled && s_isp_override_runtime.contrast_valid) {
            s_isp_override_target.contrast = s_isp_override_runtime.contrast;
        }
        s_isp_override_target.contrast_manual = !enabled;
        s_isp_state.contrast_auto = enabled;
    } else {
        if (!enabled && s_isp_override_runtime.saturation_valid) {
            s_isp_override_target.saturation = s_isp_override_runtime.saturation;
        }
        s_isp_override_target.saturation_manual = !enabled;
        s_isp_state.saturation_auto = enabled;
    }
    portEXIT_CRITICAL(&s_isp_override_lock);
    portEXIT_CRITICAL(&s_isp_state_lock);
    return ESP_OK;
}

esp_err_t cam_sensor_isp_set_contrast_auto(bool enabled)
{
    return cam_isp_set_auto(true, enabled);
}

esp_err_t cam_sensor_isp_set_saturation_auto(bool enabled)
{
    return cam_isp_set_auto(false, enabled);
}

esp_err_t cam_sensor_isp_set_brightness(int32_t value)
{
    portENTER_CRITICAL(&s_isp_state_lock);
    if (!s_cam_initialized) {
        portEXIT_CRITICAL(&s_isp_state_lock);
        return ESP_ERR_INVALID_STATE;
    }
    cam_sensor_isp_control_info_t info = s_isp_state.brightness_info;
    if (!info.supported) {
        portEXIT_CRITICAL(&s_isp_state_lock);
        return ESP_ERR_NOT_SUPPORTED;
    }

    int32_t aligned = align_control_value(&info, value);
    portENTER_CRITICAL(&s_isp_override_lock);
    s_isp_override_target.brightness = aligned;
    portEXIT_CRITICAL(&s_isp_override_lock);
    portEXIT_CRITICAL(&s_isp_state_lock);
    return ESP_OK;
}

static esp_err_t cam_isp_set_manual_value(bool contrast, int32_t value)
{
    portENTER_CRITICAL(&s_isp_state_lock);
    if (!s_cam_initialized) {
        portEXIT_CRITICAL(&s_isp_state_lock);
        return ESP_ERR_INVALID_STATE;
    }
    cam_sensor_isp_control_info_t info = contrast ? s_isp_state.contrast_info :
                                         s_isp_state.saturation_info;
    if (!info.supported) {
        portEXIT_CRITICAL(&s_isp_state_lock);
        return ESP_ERR_NOT_SUPPORTED;
    }

    int32_t aligned = align_control_value(&info, value);
    if (aligned < 0 || aligned > UINT8_MAX) {
        portEXIT_CRITICAL(&s_isp_state_lock);
        return ESP_ERR_NOT_SUPPORTED;
    }

    portENTER_CRITICAL(&s_isp_override_lock);
    if (contrast) {
        s_isp_override_target.contrast = (uint8_t)aligned;
    } else {
        s_isp_override_target.saturation = (uint8_t)aligned;
    }
    portEXIT_CRITICAL(&s_isp_override_lock);
    portEXIT_CRITICAL(&s_isp_state_lock);
    return ESP_OK;
}

esp_err_t cam_sensor_isp_set_contrast(int32_t value)
{
    return cam_isp_set_manual_value(true, value);
}

esp_err_t cam_sensor_isp_set_saturation(int32_t value)
{
    return cam_isp_set_manual_value(false, value);
}

esp_err_t cam_sensor_isp_set_hue(int32_t value)
{
    portENTER_CRITICAL(&s_isp_state_lock);
    bool initialized = s_cam_initialized;
    cam_sensor_isp_control_info_t info = s_isp_state.hue_info;
    portEXIT_CRITICAL(&s_isp_state_lock);
    if (!initialized) {
        return ESP_ERR_INVALID_STATE;
    }
    if (!info.supported) {
        return ESP_ERR_NOT_SUPPORTED;
    }

    int32_t aligned = align_control_value(&info, value);
    portENTER_CRITICAL(&s_isp_state_lock);
    if (!s_cam_initialized || !s_isp_task || s_isp_stop_requested) {
        portEXIT_CRITICAL(&s_isp_state_lock);
        return ESP_ERR_INVALID_STATE;
    }
    TaskHandle_t task = s_isp_task;
    s_hue_target = aligned;
    s_hue_pending = true;
    portEXIT_CRITICAL(&s_isp_state_lock);
    xTaskNotifyGive(task);
    return ESP_OK;
}

// 返回 V4L2 帧缓冲总数（CAM_SENSOR_FB_COUNT）。vision 层据此推导 ringbuf 深度 = 返回值-2，
// 把缓冲数的唯一定义留在本文件，避免两处宏不一致。
int cam_sensor_get_fb_count(void)
{
    return CAM_SENSOR_FB_COUNT;
}

// 先请求 ISP worker 自退并等待确认，再关闭控制 fd；随后停流并释放采集资源。
esp_err_t cam_sensor_deinit(void)
{
    return cam_sensor_cleanup();
}
