#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/idf_additions.h"
#include "freertos/task.h"
#include "bsp_cam_sensor.h"
#include "bsp_touch.h"
#include "esp_check.h"
#include "esp_eth.h"
#include "esp_event.h"
#include "esp_heap_caps.h"
#include "esp_jpeg_enc.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_netif_ip_addr.h"
#include "esp_timer.h"
#include "ethernet_app.h"
#include "ethernet_init.h"
#include "vision_preview.h"
#include "lwip/inet.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/tcp.h"

#include <algorithm>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <vector>

static const char *TAG = "eth_example";

#define HOST_IP                 "192.168.10.1"
#define HOST_CONTROL_PORT       5000
#define HOST_IMAGE_PORT         5001
#define TCP_TASK_STACK_BYTES    (12 * 1024)
#define TCP_TASK_PRIORITY       12
#define TCP_SEND_WAIT_MS        1000
#define TCP_FRAME_RESULT_TIMEOUT_MS 15000
#define TCP_FRAME_PAUSE_MS      0
#define TCP_METRICS_INTERVAL_MS 10000
#define TCP_RX_BUFFER_BYTES     8192
#define TCP_RX_MAX_PAYLOAD_BYTES (TCP_RX_BUFFER_BYTES - ESP_HOST_HEADER_SIZE)
#define TCP_CONNECT_READY_BIT   BIT0
#define TCP_CAMERA_READY_BIT    BIT1

#define ESP_HOST_MAGIC          0x32505345u
#define ESP_HOST_VERSION        1u
#define ESP_HOST_HEADER_SIZE    40u
#define ESP_HOST_TYPE_IMAGE     0x01u
#define ESP_HOST_TYPE_METRICS   0x02u
#define ESP_HOST_TYPE_TIME_SYNC 0x10u
#define ESP_HOST_TYPE_CONTROL   0x11u
#define ESP_HOST_TYPE_INFERENCE_RESULT 0x12u
#define ESP_HOST_PIXEL_RGB888   1u
#define ESP_HOST_PIXEL_JPEG     2u

#define VISION_CAM_SCL_PIN      8
#define VISION_CAM_SDA_PIN      7
#define VISION_CAM_PWDN_PIN     -1
#define VISION_CAM_RESET_PIN    -1
#define VISION_CAM_WIDTH        1024
#define VISION_CAM_HEIGHT       600
#define JPEG_OUTBUF_BYTES       (768 * 1024)
#define JPEG_TARGET_BYTES       (256 * 1024)

typedef enum {
    UPLOAD_FORMAT_JPEG = 0,
    UPLOAD_FORMAT_RAW_RGB888 = 1,
} upload_format_t;

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

static EventGroupHandle_t s_eth_events;
static TaskHandle_t s_tcp_task;
static uint32_t s_tx_seq;
static bool s_camera_started;
static uint8_t *s_jpeg_frame;
static jpeg_enc_handle_t s_jpeg_enc;
static StackType_t *s_tcp_task_stack;
static StaticTask_t s_tcp_task_tcb;
static volatile upload_format_t s_upload_format = UPLOAD_FORMAT_JPEG;

typedef struct {
    bool valid;
    uint32_t frame_seq;
    int image_width;
    int image_height;
    int inference_ms;
    int decode_ms;
    int host_ms;
    Detection detection;
    char label[16];
} host_inference_result_t;

static host_inference_result_t s_latest_result;

static void log_heap_state(const char *stage)
{
    ESP_LOGI(TAG, "heap[%s] internal=%lu largest_internal=%lu psram=%lu largest_default=%lu",
             stage,
             (unsigned long)heap_caps_get_free_size(MALLOC_CAP_INTERNAL),
             (unsigned long)heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL),
             (unsigned long)heap_caps_get_free_size(MALLOC_CAP_SPIRAM),
             (unsigned long)heap_caps_get_largest_free_block(MALLOC_CAP_DEFAULT));
}

static uint64_t unix_time_ms(void)
{
    struct timeval tv = {0};
    gettimeofday(&tv, NULL);
    return (uint64_t)tv.tv_sec * 1000ULL + (uint64_t)(tv.tv_usec / 1000);
}

static int wait_socket_writable(int sock)
{
    fd_set write_set;
    FD_ZERO(&write_set);
    FD_SET(sock, &write_set);
    struct timeval timeout = {
        .tv_sec = TCP_SEND_WAIT_MS / 1000,
        .tv_usec = (TCP_SEND_WAIT_MS % 1000) * 1000,
    };
    int ret = select(sock + 1, NULL, &write_set, NULL, &timeout);
    return ret > 0 && FD_ISSET(sock, &write_set) ? 0 : -1;
}

static int send_all(int sock, const void *data, size_t len)
{
    const uint8_t *p = (const uint8_t *)data;
    size_t sent_total = 0;
    while (sent_total < len) {
        int sent = send(sock, p + sent_total, len - sent_total, 0);
        if (sent < 0) {
            if (errno == EINTR) {
                continue;
            }
            if ((errno == EAGAIN || errno == EWOULDBLOCK) && wait_socket_writable(sock) == 0) {
                continue;
            }
            return -1;
        }
        if (sent == 0) {
            return -1;
        }
        sent_total += (size_t)sent;
    }
    return 0;
}

static int send_packet(int sock, uint16_t type, const void *payload, uint32_t payload_len,
                       uint16_t width, uint16_t height, uint16_t pixel_format)
{
    esp_host_packet_header_t header = {
        .magic = ESP_HOST_MAGIC,
        .version = ESP_HOST_VERSION,
        .type = type,
        .header_size = ESP_HOST_HEADER_SIZE,
        .seq = ++s_tx_seq,
        .timestamp_ms = unix_time_ms(),
        .payload_len = payload_len,
        .width = width,
        .height = height,
        .pixel_format = pixel_format,
        .reserved = 0,
        .reserved2 = 0,
    };

    if (send_all(sock, &header, sizeof(header)) != 0) {
        return -1;
    }
    if (payload_len > 0 && send_all(sock, payload, payload_len) != 0) {
        return -1;
    }
    return 0;
}

static esp_err_t ensure_jpeg_encoder(int width, int height)
{
    if (s_jpeg_enc) {
        return ESP_OK;
    }

    jpeg_enc_config_t cfg = DEFAULT_JPEG_ENC_CONFIG();
    cfg.width = width;
    cfg.height = height;
    cfg.src_type = JPEG_PIXEL_FORMAT_RGB888;
    cfg.subsampling = JPEG_SUBSAMPLE_420;
    cfg.quality = 70;
    cfg.rotate = JPEG_ROTATE_0D;
    cfg.task_enable = false;
    cfg.hfm_task_priority = 13;
    cfg.hfm_task_core = 1;

    jpeg_error_t err = jpeg_enc_open(&cfg, &s_jpeg_enc);
    if (err != JPEG_ERR_OK) {
        ESP_LOGE(TAG, "jpeg encoder open failed err=%d", (int)err);
        return ESP_FAIL;
    }
    return ESP_OK;
}

static uint8_t *get_jpeg_frame_buffer(void)
{
    if (!s_jpeg_frame) {
        s_jpeg_frame = (uint8_t *)heap_caps_malloc(JPEG_OUTBUF_BYTES, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
        if (!s_jpeg_frame) {
            ESP_LOGE(TAG, "allocate JPEG output buffer failed: %u bytes", (unsigned)JPEG_OUTBUF_BYTES);
        }
    }
    return s_jpeg_frame;
}

static esp_err_t encode_jpeg_frame(const uint8_t *frame, int frame_w, int frame_h,
                                   uint8_t **out, size_t *out_size, int *out_quality)
{
    if (!frame || !out || !out_size || frame_w <= 0 || frame_h <= 0) {
        return ESP_ERR_INVALID_ARG;
    }
    ESP_RETURN_ON_ERROR(ensure_jpeg_encoder(frame_w, frame_h), TAG, "jpeg encoder init failed");
    uint8_t *jpeg = get_jpeg_frame_buffer();
    ESP_RETURN_ON_FALSE(jpeg != NULL, ESP_ERR_NO_MEM, TAG, "jpeg output buffer unavailable");

    const int qualities[] = {70};
    const int image_size = frame_w * frame_h * 3;
    int encoded = 0;
    jpeg_error_t last_err = JPEG_ERR_FAIL;
    for (int quality : qualities) {
        jpeg_enc_set_quality(s_jpeg_enc, (uint8_t)quality);
        encoded = 0;
        last_err = jpeg_enc_process(s_jpeg_enc, frame, image_size, jpeg, JPEG_OUTBUF_BYTES, &encoded);
        if (last_err == JPEG_ERR_OK && encoded > 0) {
            *out = jpeg;
            *out_size = (size_t)encoded;
            if (out_quality) {
                *out_quality = quality;
            }
            if ((size_t)encoded <= JPEG_TARGET_BYTES || quality == qualities[(sizeof(qualities) / sizeof(qualities[0])) - 1]) {
                return ESP_OK;
            }
        }
    }

    ESP_LOGE(TAG, "jpeg encode failed err=%d encoded=%d", (int)last_err, encoded);
    return ESP_FAIL;
}

static uint32_t calculate_cpu_usage_percent(void)
{
    UBaseType_t num_tasks = uxTaskGetNumberOfTasks();
    TaskStatus_t *task_array = (TaskStatus_t *)calloc(num_tasks, sizeof(TaskStatus_t));
    if (!task_array) {
        return 0;
    }

    configRUN_TIME_COUNTER_TYPE total_runtime = 0;
    UBaseType_t actual = uxTaskGetSystemState(task_array, num_tasks, &total_runtime);
    static configRUN_TIME_COUNTER_TYPE s_prev_total;
    static configRUN_TIME_COUNTER_TYPE s_prev_idle;

    configRUN_TIME_COUNTER_TYPE idle_runtime = 0;
    for (UBaseType_t i = 0; i < actual; i++) {
        if (strncmp(task_array[i].pcTaskName, "IDLE", 4) == 0) {
            idle_runtime += task_array[i].ulRunTimeCounter;
        }
    }
    free(task_array);

    uint32_t cpu = 0;
    configRUN_TIME_COUNTER_TYPE total_delta = total_runtime - s_prev_total;
    configRUN_TIME_COUNTER_TYPE idle_delta = idle_runtime - s_prev_idle;
    if (s_prev_total != 0 && total_delta > 0) {
        uint64_t busy = (idle_delta >= total_delta) ? 0 : (uint64_t)(total_delta - idle_delta);
        cpu = (uint32_t)((busy * 100ULL) / (uint64_t)total_delta);
        if (cpu > 100) {
            cpu = 100;
        }
    }

    s_prev_total = total_runtime;
    s_prev_idle = idle_runtime;
    return cpu;
}

static int send_metrics_packet(int sock)
{
    char json[512];
    uint32_t free_heap = esp_get_free_heap_size();
    uint32_t min_heap = esp_get_minimum_free_heap_size();
    uint32_t free_internal = heap_caps_get_free_size(MALLOC_CAP_INTERNAL);
    uint32_t free_psram = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
    uint32_t total_psram = heap_caps_get_total_size(MALLOC_CAP_SPIRAM);
    uint32_t largest_free = heap_caps_get_largest_free_block(MALLOC_CAP_DEFAULT);
    uint32_t cpu_usage = calculate_cpu_usage_percent();
    int64_t uptime_ms = esp_timer_get_time() / 1000;

    int len = snprintf(json, sizeof(json),
                       "{\"seq\":%lu,\"uptime_ms\":%lld,\"cpu_usage\":%lu,"
                       "\"free_heap\":%lu,\"min_free_heap\":%lu,"
                       "\"free_internal\":%lu,\"free_psram\":%lu,"
                       "\"total_psram\":%lu,\"largest_free_block\":%lu}",
                       (unsigned long)(s_tx_seq + 1),
                       (long long)uptime_ms,
                       (unsigned long)cpu_usage,
                       (unsigned long)free_heap,
                       (unsigned long)min_heap,
                       (unsigned long)free_internal,
                       (unsigned long)free_psram,
                       (unsigned long)total_psram,
                       (unsigned long)largest_free);
    if (len <= 0 || len >= (int)sizeof(json)) {
        return -1;
    }
    return send_packet(sock, ESP_HOST_TYPE_METRICS, json, (uint32_t)len, 0, 0, 0);
}

static int64_t parse_json_int64(const char *json, const char *key, int64_t fallback)
{
    const char *p = strstr(json, key);
    if (!p) {
        return fallback;
    }
    p += strlen(key);
    while (*p && ((*p < '0' || *p > '9') && *p != '-')) {
        p++;
    }
    if (!*p) {
        return fallback;
    }
    return strtoll(p, NULL, 10);
}

static float parse_json_float(const char *json, const char *key, float fallback)
{
    const char *p = strstr(json, key);
    if (!p) {
        return fallback;
    }
    p += strlen(key);
    while (*p && ((*p < '0' || *p > '9') && *p != '-' && *p != '.')) {
        p++;
    }
    if (!*p) {
        return fallback;
    }
    return strtof(p, NULL);
}

static bool parse_json_string(const char *json, const char *key, char *out, size_t out_size)
{
    if (!out || out_size == 0) {
        return false;
    }
    const char *p = strstr(json, key);
    if (!p) {
        out[0] = '\0';
        return false;
    }
    p += strlen(key);
    p = strchr(p, '"');
    if (!p) {
        out[0] = '\0';
        return false;
    }
    p++;
    const char *end = strchr(p, '"');
    if (!end) {
        out[0] = '\0';
        return false;
    }
    size_t len = (size_t)(end - p);
    if (len >= out_size) {
        len = out_size - 1;
    }
    memcpy(out, p, len);
    out[len] = '\0';
    return true;
}

static bool parse_json_string_value(const char *json, const char *key, char *out, size_t out_size)
{
    if (!out || out_size == 0) {
        return false;
    }
    const size_t key_len = strlen(key);
    const char *p = json;
    while ((p = strstr(p, key)) != NULL) {
        const char *value = p + key_len;
        while (*value == ' ' || *value == '\t') {
            value++;
        }
        if (*value == ':') {
            p = value + 1;
            break;
        }
        p += key_len;
    }
    if (!p) {
        out[0] = '\0';
        return false;
    }

    while (*p == ' ' || *p == '\t') {
        p++;
    }
    if (*p != '"') {
        out[0] = '\0';
        return false;
    }
    p++;
    const char *end = strchr(p, '"');
    if (!end) {
        out[0] = '\0';
        return false;
    }
    size_t len = (size_t)(end - p);
    if (len >= out_size) {
        len = out_size - 1;
    }
    memcpy(out, p, len);
    out[len] = '\0';
    return true;
}

static int class_id_from_label(const char *label)
{
    if (strcmp(label, "jt") == 0 || strcmp(label, "极兔") == 0) {
        return 0;
    }
    if (strcmp(label, "zt") == 0 || strcmp(label, "中通") == 0) {
        return 1;
    }
    if (strcmp(label, "yd") == 0 || strcmp(label, "韵达") == 0) {
        return 2;
    }
    return -1;
}

static bool parse_inference_result(const char *json, size_t len, host_inference_result_t *out)
{
    if (!json || !out || len == 0) {
        return false;
    }

    uint32_t frame_seq = (uint32_t)parse_json_int64(json, "\"frame_seq\"", -1);
    int image_width = (int)parse_json_int64(json, "\"image_width\"", VISION_CAM_WIDTH);
    int image_height = (int)parse_json_int64(json, "\"image_height\"", VISION_CAM_HEIGHT);
    int inference_ms = (int)parse_json_int64(json, "\"inference_ms\"", 0);
    int decode_ms = (int)parse_json_int64(json, "\"decode_ms\"", 0);
    int host_ms = (int)parse_json_int64(json, "\"host_ms\"", 0);
    if (frame_seq == (uint32_t)-1 || image_width <= 0 || image_height <= 0) {
        return false;
    }

    memset(out, 0, sizeof(*out));
    out->valid = strstr(json, "\"detections\":[]") == NULL && strstr(json, "\"detections\": []") == NULL;
    out->frame_seq = frame_seq;
    out->image_width = image_width;
    out->image_height = image_height;
    out->inference_ms = inference_ms;
    out->decode_ms = decode_ms;
    out->host_ms = host_ms;

    if (!out->valid) {
        return true;
    }

    parse_json_string(json, "\"label\"", out->label, sizeof(out->label));
    float x = parse_json_float(json, "\"x\"", 0.0f);
    float y = parse_json_float(json, "\"y\"", 0.0f);
    float w = parse_json_float(json, "\"w\"", 0.0f);
    float h = parse_json_float(json, "\"h\"", 0.0f);
    x = std::clamp(x, 0.0f, 1.0f);
    y = std::clamp(y, 0.0f, 1.0f);
    w = std::clamp(w, 0.0f, 1.0f - x);
    h = std::clamp(h, 0.0f, 1.0f - y);

    out->detection.x1 = x * (float)VISION_CAM_WIDTH;
    out->detection.y1 = y * (float)VISION_CAM_HEIGHT;
    out->detection.x2 = (x + w) * (float)VISION_CAM_WIDTH;
    out->detection.y2 = (y + h) * (float)VISION_CAM_HEIGHT;
    out->detection.confidence = parse_json_float(json, "\"confidence\"", 0.0f);
    out->detection.class_id = class_id_from_label(out->label);
    out->valid = w > 0.0f && h > 0.0f;
    return true;
}

static esp_err_t start_camera_once(void)
{
    if (s_camera_started) {
        return ESP_OK;
    }

    cam_sensor_config_t cam_cfg = {
        .i2c_scl_pin = VISION_CAM_SCL_PIN,
        .i2c_sda_pin = VISION_CAM_SDA_PIN,
        .pwdn_pin = VISION_CAM_PWDN_PIN,
        .reset_pin = VISION_CAM_RESET_PIN,
        .frame_width = VISION_CAM_WIDTH,
        .frame_height = VISION_CAM_HEIGHT,
        .i2c_bus = BSP_Touch_GetI2CBus(),
    };

    log_heap_state("before_camera");
    ESP_RETURN_ON_ERROR(cam_sensor_init(&cam_cfg), TAG, "camera init failed");
    ESP_RETURN_ON_ERROR(cam_sensor_start(), TAG, "camera start failed");
    s_camera_started = true;
    log_heap_state("after_camera");
    if (s_eth_events) {
        xEventGroupSetBits(s_eth_events, TCP_CAMERA_READY_BIT);
    }
    return ESP_OK;
}

static void apply_time_sync(const char *json, size_t len)
{
    char buf[160];
    size_t copy_len = len < sizeof(buf) - 1 ? len : sizeof(buf) - 1;
    memcpy(buf, json, copy_len);
    buf[copy_len] = '\0';

    int64_t unix_ms = parse_json_int64(buf, "unix_ms", -1);
    int64_t tz_offset_min = parse_json_int64(buf, "tz_offset_min", 480);
    if (unix_ms <= 0) {
        ESP_LOGW(TAG, "invalid time sync payload: %s", buf);
        return;
    }

    struct timeval tv = {
        .tv_sec = (time_t)(unix_ms / 1000),
        .tv_usec = (suseconds_t)((unix_ms % 1000) * 1000),
    };
    settimeofday(&tv, NULL);

    int tz_hours = (int)(tz_offset_min / 60);
    char tz[16];
    snprintf(tz, sizeof(tz), "UTC%+d", -tz_hours);
    setenv("TZ", tz, 1);
    tzset();

    ESP_LOGI(TAG, "time synced unix_ms=%lld tz_offset_min=%lld", (long long)unix_ms, (long long)tz_offset_min);
}

static void apply_control_packet(const char *json, size_t len)
{
    char buf[256];
    size_t copy_len = std::min(len, sizeof(buf) - 1);
    memcpy(buf, json, copy_len);
    buf[copy_len] = '\0';

    char upload_format[16];
    bool has_upload_format = parse_json_string_value(buf, "\"upload_format\"", upload_format, sizeof(upload_format));
    if (!has_upload_format) {
        char command[32];
        if (parse_json_string_value(buf, "\"command\"", command, sizeof(command)) &&
            strcmp(command, "upload_format") == 0) {
            has_upload_format = parse_json_string_value(buf, "\"value\"", upload_format, sizeof(upload_format));
        }
    }
    if (has_upload_format) {
        if (strcmp(upload_format, "raw") == 0 || strcmp(upload_format, "rgb888") == 0) {
            s_upload_format = UPLOAD_FORMAT_RAW_RGB888;
            ESP_LOGI(TAG, "upload format set to raw RGB888");
        } else if (strcmp(upload_format, "jpeg") == 0) {
            s_upload_format = UPLOAD_FORMAT_JPEG;
            ESP_LOGI(TAG, "upload format set to JPEG");
        } else {
            ESP_LOGW(TAG, "unknown upload_format=%s", upload_format);
        }
    }
}

static void process_rx_packet(int sock, const esp_host_packet_header_t *header, const uint8_t *payload)
{
    (void)sock;
    if (header->type == ESP_HOST_TYPE_TIME_SYNC) {
        apply_time_sync((const char *)payload, header->payload_len);
    } else if (header->type == ESP_HOST_TYPE_INFERENCE_RESULT) {
        static char json[TCP_RX_MAX_PAYLOAD_BYTES + 1];
        size_t copy_len = std::min((size_t)header->payload_len, sizeof(json) - 1);
        memcpy(json, payload, copy_len);
        json[copy_len] = '\0';
        host_inference_result_t result = {};
        if (parse_inference_result(json, copy_len, &result)) {
            s_latest_result = result;
            ESP_LOGI(TAG, "host result seq=%lu valid=%d label=%s conf=%.3f infer=%dms decode=%dms host=%dms rx=%luB",
                     (unsigned long)result.frame_seq,
                     result.valid ? 1 : 0,
                     result.label,
                     (double)result.detection.confidence,
                     result.inference_ms,
                     result.decode_ms,
                     result.host_ms,
                     (unsigned long)header->payload_len);
        } else {
            ESP_LOGW(TAG, "invalid inference result JSON: %.*s", (int)copy_len, json);
        }
    } else if (header->type == ESP_HOST_TYPE_CONTROL) {
        apply_control_packet((const char *)payload, header->payload_len);
    }
}

static bool poll_incoming_packets(int sock, bool reset_buffer)
{
    static uint8_t rx_buf[TCP_RX_BUFFER_BYTES];
    static size_t rx_len;

    if (reset_buffer) {
        rx_len = 0;
        return true;
    }

    while (rx_len < sizeof(rx_buf)) {
        int got = recv(sock, rx_buf + rx_len, sizeof(rx_buf) - rx_len, MSG_DONTWAIT);
        if (got > 0) {
            rx_len += (size_t)got;
            continue;
        }
        if (got < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
            break;
        }
        if (got == 0) {
            return false;
        }
        return false;
    }

    while (rx_len >= sizeof(esp_host_packet_header_t)) {
        esp_host_packet_header_t header;
        memcpy(&header, rx_buf, sizeof(header));
        if (header.magic != ESP_HOST_MAGIC || header.version != ESP_HOST_VERSION ||
            header.header_size != ESP_HOST_HEADER_SIZE || header.payload_len > TCP_RX_MAX_PAYLOAD_BYTES) {
            memmove(rx_buf, rx_buf + 1, --rx_len);
            continue;
        }
        size_t packet_len = sizeof(header) + header.payload_len;
        if (rx_len < packet_len) {
            return true;
        }
        process_rx_packet(sock, &header, rx_buf + sizeof(header));
        memmove(rx_buf, rx_buf + packet_len, rx_len - packet_len);
        rx_len -= packet_len;
    }
    return true;
}

static int connect_to_host(int port, const char *name)
{
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (sock < 0) {
        ESP_LOGE(TAG, "socket failed errno=%d", errno);
        return -1;
    }

    struct sockaddr_in dest = {
        .sin_family = AF_INET,
        .sin_port = htons(port),
    };
    inet_pton(AF_INET, HOST_IP, &dest.sin_addr);

    int flags = fcntl(sock, F_GETFL, 0);
    if (flags >= 0) {
        fcntl(sock, F_SETFL, flags | O_NONBLOCK);
    }

    ESP_LOGI(TAG, "connecting %s to host %s:%d", name, HOST_IP, port);
    int ret = connect(sock, (struct sockaddr *)&dest, sizeof(dest));
    if (ret != 0) {
        if (errno != EINPROGRESS) {
            ESP_LOGW(TAG, "connect failed errno=%d", errno);
            close(sock);
            return -1;
        }

        fd_set write_set;
        FD_ZERO(&write_set);
        FD_SET(sock, &write_set);
        struct timeval timeout = {
            .tv_sec = 5,
            .tv_usec = 0,
        };
        ret = select(sock + 1, NULL, &write_set, NULL, &timeout);
        if (ret <= 0) {
            ESP_LOGW(TAG, "connect timeout ret=%d errno=%d", ret, errno);
            close(sock);
            return -1;
        }

        int err = 0;
        socklen_t err_len = sizeof(err);
        if (getsockopt(sock, SOL_SOCKET, SO_ERROR, &err, &err_len) != 0 || err != 0) {
            ESP_LOGW(TAG, "connect failed so_error=%d errno=%d", err, errno);
            close(sock);
            return -1;
        }
    }

    ESP_LOGI(TAG, "%s connected to host", name);
    if (flags >= 0) {
        fcntl(sock, F_SETFL, flags & ~O_NONBLOCK);
    }
    int one = 1;
    setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));
    struct timeval send_timeout = {
        .tv_sec = TCP_SEND_WAIT_MS / 1000,
        .tv_usec = (TCP_SEND_WAIT_MS % 1000) * 1000,
    };
    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &send_timeout, sizeof(send_timeout));
    return sock;
}

static bool wait_for_frame_result(int sock, uint32_t frame_seq, host_inference_result_t *result)
{
    int64_t deadline_us = esp_timer_get_time() + (int64_t)TCP_FRAME_RESULT_TIMEOUT_MS * 1000LL;
    while (esp_timer_get_time() < deadline_us) {
        if (!poll_incoming_packets(sock, false)) {
            return false;
        }
        if (s_latest_result.frame_seq == frame_seq) {
            if (result) {
                *result = s_latest_result;
            }
            return true;
        }
        vTaskDelay(pdMS_TO_TICKS(1));
    }
    return false;
}

static void render_host_result(const uint8_t *frame, int frame_w, int frame_h,
                               const host_inference_result_t *result, int64_t total_us)
{
    static std::vector<Detection> detections;
    detections.clear();
    if (detections.capacity() == 0) {
        detections.reserve(1);
    }
    if (result && result->valid) {
        detections.push_back(result->detection);
    }

    YOLOPerfStats perf = {};
    perf.total_us = total_us;
    perf.run_us = result ? (int64_t)result->inference_ms * 1000LL : 0;
    perf.target_confidence = result && result->valid ? result->detection.confidence : 0.0f;
    perf.detect_count = result ? result->frame_seq : 0;

    esp_err_t ret = vision_preview_update(frame, frame_w, frame_h, detections, -1.0f, perf,
                                          result != NULL && result->valid);
    if (ret != ESP_OK) {
        ESP_LOGW(TAG, "preview host result failed: %s", esp_err_to_name(ret));
    }
}

static int send_frame_wait_result(int image_sock, int control_sock)
{
    uint8_t *frame = NULL;
    size_t frame_size = 0;
    int frame_w = 0;
    int frame_h = 0;

    int64_t start_us = esp_timer_get_time();
    esp_err_t ret = cam_sensor_get_frame(&frame, &frame_size, &frame_w, &frame_h, 2000);
    int64_t dq_done_us = esp_timer_get_time();
    if (ret != ESP_OK) {
        ESP_LOGW(TAG, "get camera frame failed: %s", esp_err_to_name(ret));
        return 0;
    }

    const uint32_t frame_seq = s_tx_seq + 1;
    s_latest_result.frame_seq = 0;
    s_latest_result.valid = false;

    const upload_format_t upload_format = s_upload_format;
    uint8_t *payload = frame;
    size_t payload_size = frame_size;
    uint16_t pixel_format = ESP_HOST_PIXEL_RGB888;
    int jpeg_quality = 0;
    const char *format_name = "raw";
    if (upload_format == UPLOAD_FORMAT_JPEG) {
        ret = encode_jpeg_frame(frame, frame_w, frame_h, &payload, &payload_size, &jpeg_quality);
        if (ret != ESP_OK) {
            cam_sensor_return_frame(frame);
            return -1;
        }
        pixel_format = ESP_HOST_PIXEL_JPEG;
        format_name = "jpeg";
    }
    int64_t encode_done_us = esp_timer_get_time();

    ESP_LOGI(TAG, "send frame seq=%lu fmt=%s camera=%dx%d raw=%u payload=%u bytes q=%d",
             (unsigned long)frame_seq,
             format_name,
             frame_w,
             frame_h,
             (unsigned)frame_size,
             (unsigned)payload_size,
             jpeg_quality);
    int send_ret = send_packet(image_sock, ESP_HOST_TYPE_IMAGE, payload, (uint32_t)payload_size,
                               frame_w, frame_h, pixel_format);
    int64_t send_done_us = esp_timer_get_time();
    if (send_ret != 0) {
        cam_sensor_return_frame(frame);
        return -1;
    }

    host_inference_result_t result = {};
    bool got_result = wait_for_frame_result(control_sock, frame_seq, &result);
    int64_t wait_done_us = esp_timer_get_time();
    int64_t total_us = wait_done_us - start_us;
    if (got_result) {
        render_host_result(frame, frame_w, frame_h, &result, total_us);
        int64_t preview_done_us = esp_timer_get_time();
        ESP_LOGI(TAG,
                 "frame seq=%lu fmt=%s total=%lldms dq=%lldms encode=%lldms send=%lldms wait=%lldms preview=%lldms infer=%dms host=%dms payload=%u q=%d det=%d",
                 (unsigned long)frame_seq,
                 format_name,
                 (long long)(total_us / 1000),
                 (long long)((dq_done_us - start_us) / 1000),
                 (long long)((encode_done_us - dq_done_us) / 1000),
                 (long long)((send_done_us - encode_done_us) / 1000),
                 (long long)((wait_done_us - send_done_us) / 1000),
                 (long long)((preview_done_us - wait_done_us) / 1000),
                 result.inference_ms,
                 result.host_ms,
                 (unsigned)payload_size,
                 jpeg_quality,
                 result.valid ? 1 : 0);
    } else {
        render_host_result(frame, frame_w, frame_h, NULL, total_us);
        int64_t preview_done_us = esp_timer_get_time();
        ESP_LOGW(TAG,
                 "frame seq=%lu fmt=%s inference timeout total=%lldms dq=%lldms encode=%lldms send=%lldms wait=%lldms preview=%lldms payload=%u q=%d",
                 (unsigned long)frame_seq,
                 format_name,
                 (long long)(total_us / 1000),
                 (long long)((dq_done_us - start_us) / 1000),
                 (long long)((encode_done_us - dq_done_us) / 1000),
                 (long long)((send_done_us - encode_done_us) / 1000),
                 (long long)((wait_done_us - send_done_us) / 1000),
                 (long long)((preview_done_us - wait_done_us) / 1000),
                 (unsigned)payload_size,
                 jpeg_quality);
    }

    cam_sensor_return_frame(frame);
    return got_result ? 0 : -1;
}

static void tcp_client_task(void *arg)
{
    (void)arg;
    lwip_socket_thread_init();
    ESP_LOGI(TAG, "TCP task LwIP thread semaphore initialized");

    while (true) {
        xEventGroupWaitBits(s_eth_events,
                            TCP_CONNECT_READY_BIT | TCP_CAMERA_READY_BIT,
                            pdFALSE,
                            pdTRUE,
                            portMAX_DELAY);

        int control_sock = connect_to_host(HOST_CONTROL_PORT, "control");
        if (control_sock < 0) {
            vTaskDelay(pdMS_TO_TICKS(2000));
            continue;
        }
        int image_sock = connect_to_host(HOST_IMAGE_PORT, "image");
        if (image_sock < 0) {
            close(control_sock);
            vTaskDelay(pdMS_TO_TICKS(2000));
            continue;
        }
        poll_incoming_packets(control_sock, true);
        int64_t next_metrics_us = esp_timer_get_time() + (int64_t)TCP_METRICS_INTERVAL_MS * 1000LL;
        while (true) {
            if (!poll_incoming_packets(control_sock, false)) {
                ESP_LOGW(TAG, "TCP control peer closed");
                break;
            }

            int64_t now_us = esp_timer_get_time();
            if (now_us >= next_metrics_us) {
                if (send_metrics_packet(control_sock) != 0) {
                    ESP_LOGW(TAG, "TCP metrics send failed errno=%d", errno);
                    break;
                }
                next_metrics_us = now_us + (int64_t)TCP_METRICS_INTERVAL_MS * 1000LL;
            }
            if (send_frame_wait_result(image_sock, control_sock) != 0) {
                ESP_LOGW(TAG, "TCP frame loop failed errno=%d", errno);
                break;
            }
            if (TCP_FRAME_PAUSE_MS > 0) {
                vTaskDelay(pdMS_TO_TICKS(TCP_FRAME_PAUSE_MS));
            }
        }

        close(image_sock);
        close(control_sock);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

static void start_tcp_client_task(void)
{
    if (s_tcp_task) {
        return;
    }

    if (!s_tcp_task_stack) {
        s_tcp_task_stack = (StackType_t *)heap_caps_aligned_alloc(
            sizeof(StackType_t),
            TCP_TASK_STACK_BYTES,
            MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
        if (!s_tcp_task_stack) {
            ESP_LOGE(TAG, "allocate TCP task stack failed: %u bytes", (unsigned)TCP_TASK_STACK_BYTES);
            log_heap_state("tcp_stack_alloc_failed");
            return;
        }
    }

    s_tcp_task = xTaskCreateStaticPinnedToCore(
        tcp_client_task,
        "eth_tcp",
        TCP_TASK_STACK_BYTES / sizeof(StackType_t),
        NULL,
        TCP_TASK_PRIORITY,
        s_tcp_task_stack,
        &s_tcp_task_tcb,
        0);
    if (!s_tcp_task) {
        ESP_LOGE(TAG, "create TCP client task failed");
        log_heap_state("tcp_task_create_failed");
        heap_caps_free(s_tcp_task_stack);
        s_tcp_task_stack = NULL;
        return;
    }

    ESP_LOGI(TAG, "TCP client task started stack=%u bytes", (unsigned)TCP_TASK_STACK_BYTES);
}

/*
 * 配置板子的固定 IPv4 地址。
 * 电脑端网卡设置为 192.168.10.1，板子固定为 192.168.10.2，二者处于同一网段。
 */
static esp_err_t configure_static_ip(esp_netif_t *netif)
{
    esp_netif_ip_info_t ip_info = {0};

    /* 使用静态 IP 时必须先关闭 DHCP client，否则后续 set_ip_info 会被 DHCP 覆盖。 */
    ESP_RETURN_ON_ERROR(esp_netif_dhcpc_stop(netif), TAG, "stop Ethernet DHCP client failed");
    ESP_RETURN_ON_FALSE(esp_netif_str_to_ip4("192.168.10.2", &ip_info.ip) == ESP_OK, ESP_FAIL, TAG, "invalid static IP");
    ESP_RETURN_ON_FALSE(esp_netif_str_to_ip4("255.255.255.0", &ip_info.netmask) == ESP_OK, ESP_FAIL, TAG, "invalid static netmask");
    ESP_RETURN_ON_FALSE(esp_netif_str_to_ip4("192.168.10.1", &ip_info.gw) == ESP_OK, ESP_FAIL, TAG, "invalid static gateway");
    ESP_RETURN_ON_ERROR(esp_netif_set_ip_info(netif, &ip_info), TAG, "set Ethernet static IP failed");

    return ESP_OK;
}

/* Ethernet 链路事件回调：只负责打印网线插拔、驱动启动/停止等链路状态。 */
static void eth_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    uint8_t mac_addr[6] = {0};
    esp_eth_handle_t eth_handle = *(esp_eth_handle_t *)event_data;

    switch (event_id) {
    case ETHERNET_EVENT_CONNECTED:
        esp_eth_ioctl(eth_handle, ETH_CMD_G_MAC_ADDR, mac_addr);
        ESP_LOGI(TAG, "Ethernet Link Up");
        ESP_LOGI(TAG, "Ethernet HW Addr %02x:%02x:%02x:%02x:%02x:%02x",
                 mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
        break;
    case ETHERNET_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "Ethernet Link Down");
        break;
    case ETHERNET_EVENT_START:
        ESP_LOGI(TAG, "Ethernet Started");
        break;
    case ETHERNET_EVENT_STOP:
        ESP_LOGI(TAG, "Ethernet Stopped");
        break;
    default:
        break;
    }
}

/* IP 事件回调：静态 IP 设置成功并绑定到网卡后，会在这里打印最终生效的地址。 */
static void got_ip_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
    const esp_netif_ip_info_t *ip_info = &event->ip_info;

    ESP_LOGI(TAG, "Ethernet Got IP Address");
    ESP_LOGI(TAG, "~~~~~~~~~~~");
    ESP_LOGI(TAG, "ETHIP:" IPSTR, IP2STR(&ip_info->ip));
    ESP_LOGI(TAG, "ETHMASK:" IPSTR, IP2STR(&ip_info->netmask));
    ESP_LOGI(TAG, "ETHGW:" IPSTR, IP2STR(&ip_info->gw));
    ESP_LOGI(TAG, "~~~~~~~~~~~");
    xEventGroupSetBits(s_eth_events, TCP_CONNECT_READY_BIT);
}

/*
 * Ethernet 应用层入口。
 * 初始化顺序：底层驱动 → esp-netif → 绑定 glue → 设置静态 IP → 注册事件 → 启动网卡。
 */
esp_err_t ethernet_app_start(void)
{
    uint8_t eth_port_cnt = 0;
    esp_eth_handle_t *eth_handles = NULL;

    if (!s_eth_events) {
        s_eth_events = xEventGroupCreate();
        ESP_RETURN_ON_FALSE(s_eth_events != NULL, ESP_ERR_NO_MEM, TAG, "create Ethernet event group failed");
    }

    ESP_LOGI(TAG, "esp_netif_init begin");
    ESP_RETURN_ON_ERROR(esp_netif_init(), TAG, "esp-netif init failed");
    ESP_LOGI(TAG, "esp_netif_init done");

    /*
     * LwIP allocates a per-thread socket semaphore the first time a task uses the
     * socket API. Create the TCP task right after esp-netif init so that semaphore
     * is reserved before the camera consumes most internal RAM.
     */
    start_tcp_client_task();

    /*
     * MIPI CSI/ISP must reserve its controller, DMA channel and small internal queues
     * before Ethernet driver/socket buffers fragment internal heap. Keep the camera
     * alive for the process lifetime; TCP reconnects only recreate sockets.
     */
    ESP_LOGI(TAG, "camera service start begin");
    ESP_RETURN_ON_ERROR(start_camera_once(), TAG, "camera service start failed");
    ESP_LOGI(TAG, "camera service start done");

    ESP_LOGI(TAG, "event loop create begin");
    ESP_RETURN_ON_ERROR(esp_event_loop_create_default(), TAG, "default event loop init failed");
    ESP_LOGI(TAG, "event loop create done");

    /* 创建 ESP32-P4 EMAC + IP101 PHY 的底层 Ethernet driver。 */
    ESP_LOGI(TAG, "example_eth_init begin");
    ESP_RETURN_ON_ERROR(example_eth_init(&eth_handles, &eth_port_cnt), TAG, "Ethernet init failed");
    ESP_LOGI(TAG, "example_eth_init done ports=%u", eth_port_cnt);
    ESP_RETURN_ON_FALSE(eth_port_cnt == 1, ESP_FAIL, TAG, "unexpected Ethernet port count: %u", eth_port_cnt);

    /* 创建 TCP/IP 协议栈中的 Ethernet netif，后续 IP、ARP、ICMP 都走这个接口。 */
    ESP_LOGI(TAG, "create Ethernet netif");
    esp_netif_config_t cfg = ESP_NETIF_DEFAULT_ETH();
    esp_netif_t *eth_netif = esp_netif_new(&cfg);
    ESP_RETURN_ON_FALSE(eth_netif != NULL, ESP_ERR_NO_MEM, TAG, "create Ethernet netif failed");

    /* glue 负责把底层 Ethernet driver 收发的数据接到 esp-netif/LwIP 协议栈。 */
    ESP_LOGI(TAG, "attach Ethernet netif");
    esp_eth_netif_glue_handle_t eth_netif_glue = esp_eth_new_netif_glue(eth_handles[0]);
    ESP_RETURN_ON_FALSE(eth_netif_glue != NULL, ESP_ERR_NO_MEM, TAG, "create Ethernet netif glue failed");
    ESP_RETURN_ON_ERROR(esp_netif_attach(eth_netif, eth_netif_glue), TAG, "attach Ethernet netif failed");

    /* attach 完成后再配置静态 IP，确保 IP 信息写入当前 Ethernet netif。 */
    ESP_LOGI(TAG, "configure static IP");
    ESP_RETURN_ON_ERROR(configure_static_ip(eth_netif), TAG, "configure static IP failed");

    ESP_LOGI(TAG, "register Ethernet handlers");
    ESP_RETURN_ON_ERROR(esp_event_handler_register(ETH_EVENT, ESP_EVENT_ANY_ID, &eth_event_handler, NULL), TAG, "register Ethernet event handler failed");
    ESP_RETURN_ON_ERROR(esp_event_handler_register(IP_EVENT, IP_EVENT_ETH_GOT_IP, &got_ip_event_handler, NULL), TAG, "register Ethernet IP event handler failed");
    ESP_LOGI(TAG, "esp_eth_start begin");
    ESP_RETURN_ON_ERROR(esp_eth_start(eth_handles[0]), TAG, "start Ethernet failed");
    ESP_LOGI(TAG, "esp_eth_start done");

    return ESP_OK;
}
