#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/idf_additions.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "esp_check.h"
#include "esp_eth.h"
#include "esp_event.h"
#include "esp_heap_caps.h"
#include "esp_jpeg_enc.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_netif_ip_addr.h"
#include "esp_timer.h"
#include "esp_system.h"
#include "cJSON.h"
#include "ethernet_app.h"
#include "image_result_protocol.h"
#include "ethernet_init.h"
#include "sorting_sim_control.h"
#include "system_monitor.h"
#include "vision.h"
#include "vision_model.h"
#include "vision_upload.h"
#include "bsp_cam_sensor.h"
#include "bsp_lcd.h"
#include "ui.h"
#include "sdk.h"                       // send_event / get_current_event_table / EVT_ETHERNET
#include "bsp_lvgl_adapter_init.h"     // BSP_LVGL_Lock / BSP_LVGL_Unlock
#include "lwip/inet.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/tcp.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

static const char *TAG = "eth_example";

// 上位机 TCP 链路总开关；关闭时 ethernet_app_start() 保持为安全的空操作。
#ifndef SORTER_TCP_LINK_ENABLE
#define SORTER_TCP_LINK_ENABLE 1
#endif

#define HOST_IP                    "192.168.10.1"
#define LOCAL_IP                   "192.168.10.2"
#define LOCAL_NETMASK              "255.255.255.0"
#define HOST_CONTROL_PORT          5000
#define HOST_IMAGE_PORT            5001
#define CONTROL_TASK_STACK_BYTES   8192
#define IMAGE_PROD_TASK_STACK      8192
#define IMAGE_SEND_TASK_STACK      6144
#define CONTROL_TASK_PRIORITY      4
#define IMAGE_PROD_TASK_PRIORITY   3
#define IMAGE_SEND_TASK_PRIORITY   2
#define TCP_TASK_STACK_CAPS        (MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT)
#define TCP_SEND_WAIT_MS           1000
#define TCP_CONNECT_READY_BIT      BIT0
#define TCP_IMAGE_CONNECTED_BIT    BIT1
#define TCP_METRICS_INTERVAL_MS    1000
#define TCP_IMAGE_INTERVAL_MS      5000
#define TCP_IMAGE_STALE_MS         15000
#define TCP_IMAGE_SEND_CHUNK       (8 * 1024)
#define TCP_RX_BUFFER_BYTES        1024
#define TCP_RX_MAX_PAYLOAD_BYTES   512

#ifndef SORTER_TCP_SIM_LINE_OUTPUT_ENABLE
#define SORTER_TCP_SIM_LINE_OUTPUT_ENABLE 0
#endif

#define SNAPSHOT_WIDTH             VISION_UPLOAD_WIDTH
#define SNAPSHOT_HEIGHT            VISION_UPLOAD_HEIGHT
#define JPEG_OUTBUF_BYTES          (1024 * 1024)
#define JPEG_QUALITY               70
#define IMAGE_QUEUE_DEPTH          2

#define ESP_HOST_VERSION           1u
#define ESP_HOST_HEADER_SIZE       40u
#define ESP_HOST_TYPE_METRICS      0x02u
#define ESP_HOST_TYPE_TIME_SYNC    0x10u
#define ESP_HOST_TYPE_CONTROL_JSON 0x11u
#define ESP_HOST_TYPE_SIM_LINE     0x12u
#define ESP_HOST_PIXEL_RGB888      1u
#define TCP_SIM_PKG_HEARTBEAT_MS   1000


typedef struct {
    uint8_t *jpeg;
    size_t jpeg_len;
    int64_t created_ms;
    int encode_ms;
    esp_host_image_meta_v2_t meta;
    esp_host_image_box_v2_t boxes[ESP_HOST_IMAGE_MAX_BOXES];
    bool ready;
    bool sending;
} image_slot_t;

typedef struct {
    uint32_t encoded;
    uint32_t sent;
    uint32_t dropped_backpressure;
    uint32_t dropped_stale;
    uint32_t encode_fail;
    uint32_t send_fail;
    uint32_t no_frame;
    uint32_t no_connection;
    int last_encode_ms;
    int last_send_ms;
    size_t last_jpeg_bytes;
    uint32_t last_frame_id;
    uint16_t last_box_count;
} image_stats_t;

static EventGroupHandle_t s_eth_events;
static TaskHandle_t s_control_task;
static TaskHandle_t s_image_prod_task;
static TaskHandle_t s_image_send_task;
static SemaphoreHandle_t s_image_lock;
static QueueHandle_t s_encode_queue;
static SemaphoreHandle_t s_submit_gate;
static uint32_t s_tx_seq;
static volatile bool s_control_send_failed;

// 运行时可调（设置页 UI 下发）。指标发送条件 = enabled && interval_ms>0。
static volatile uint32_t s_metrics_interval_ms = TCP_METRICS_INTERVAL_MS;
static atomic_bool s_report_image_en = ATOMIC_VAR_INIT(true);
static volatile bool s_report_metrics_en = true;

static jpeg_enc_handle_t s_jpeg_enc;
static image_slot_t s_image_slots[IMAGE_QUEUE_DEPTH];
static image_stats_t s_image_stats;
static atomic_uint_least32_t s_submit_no_connection;
static atomic_uint_least32_t s_submit_queue_full;

typedef struct {
    int id;
    char signature[80];
    int64_t last_sent_ms;
} sim_pkg_filter_t;

static sim_pkg_filter_t s_sim_pkg_filters[SORTER_MAX_PACKAGES];

static uint64_t unix_time_ms(void)
{
    struct timeval tv = {0};
    gettimeofday(&tv, NULL);
    return (uint64_t)tv.tv_sec * 1000ULL + (uint64_t)(tv.tv_usec / 1000);
}

static int64_t monotonic_ms(void)
{
    return esp_timer_get_time() / 1000;
}

static uint32_t next_tx_seq(void)
{
    return __atomic_add_fetch(&s_tx_seq, 1, __ATOMIC_RELAXED);
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

static bool socket_peer_alive(int sock)
{
    uint8_t byte;
    int got = recv(sock, &byte, sizeof(byte), MSG_PEEK | MSG_DONTWAIT);
    if (got > 0) {
        return true;
    }
    if (got == 0) {
        return false;
    }
    return errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR;
}

static int send_all_chunked(int sock, const void *data, size_t len, size_t chunk)
{
    const uint8_t *p = (const uint8_t *)data;
    size_t sent_total = 0;
    while (sent_total < len) {
        size_t remain = len - sent_total;
        size_t want = (chunk > 0 && remain > chunk) ? chunk : remain;
        int sent = send(sock, p + sent_total, want, 0);
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
        if (chunk > 0) {
            vTaskDelay(0);
        }
    }
    return 0;
}

static int send_packet(int sock, uint16_t type, const void *payload, uint32_t payload_len,
                       uint16_t width, uint16_t height, uint16_t pixel_format,
                       uint16_t reserved, uint32_t reserved2, bool chunk_payload)
{
    esp_host_packet_header_t header = {
        .magic = ESP_HOST_MAGIC,
        .version = ESP_HOST_VERSION,
        .type = type,
        .header_size = ESP_HOST_HEADER_SIZE,
        .seq = next_tx_seq(),
        .timestamp_ms = unix_time_ms(),
        .payload_len = payload_len,
        .width = width,
        .height = height,
        .pixel_format = pixel_format,
        .reserved = reserved,
        .reserved2 = reserved2,
    };

    if (send_all_chunked(sock, &header, sizeof(header), 0) != 0) {
        return -1;
    }
    if (payload_len > 0 &&
        send_all_chunked(sock, payload, payload_len, chunk_payload ? TCP_IMAGE_SEND_CHUNK : 0) != 0) {
        return -1;
    }
    return 0;
}

static int send_image_result_v2(int sock, const image_slot_t *slot)
{
    if (!slot || !slot->jpeg || slot->jpeg_len == 0 || slot->jpeg_len > UINT32_MAX ||
        slot->meta.meta_size != sizeof(esp_host_image_meta_v2_t) ||
        slot->meta.box_size != sizeof(esp_host_image_box_v2_t) ||
        slot->meta.box_count > ESP_HOST_IMAGE_MAX_BOXES ||
        slot->meta.jpeg_len != slot->jpeg_len) {
        ESP_LOGE(TAG, "invalid image v2 slot metadata");
        return -2;
    }

    uint32_t boxes_len = slot->meta.box_count * sizeof(esp_host_image_box_v2_t);
    uint32_t payload_len = sizeof(slot->meta) + boxes_len + (uint32_t)slot->jpeg_len;
    esp_host_packet_header_t header = {
        .magic = ESP_HOST_MAGIC,
        .version = ESP_HOST_IMAGE_VERSION_V2,
        .type = ESP_HOST_TYPE_IMAGE_RESULT,
        .header_size = sizeof(esp_host_packet_header_t),
        .seq = next_tx_seq(),
        .timestamp_ms = unix_time_ms(),
        .payload_len = payload_len,
        .width = ESP_HOST_IMAGE_WIDTH,
        .height = ESP_HOST_IMAGE_HEIGHT,
        .pixel_format = ESP_HOST_PIXEL_JPEG,
    };

    if (send_all_chunked(sock, &header, sizeof(header), 0) != 0) {
        return -1;
    }
    if (send_all_chunked(sock, &slot->meta, sizeof(slot->meta), 0) != 0) {
        return -1;
    }
    if (boxes_len && send_all_chunked(sock, slot->boxes, boxes_len, 0) != 0) {
        return -1;
    }
    return send_all_chunked(sock, slot->jpeg, slot->jpeg_len, TCP_IMAGE_SEND_CHUNK);
}

static int queue_depth_locked(void)
{
    int depth = 0;
    for (int i = 0; i < IMAGE_QUEUE_DEPTH; i++) {
        if (s_image_slots[i].ready || s_image_slots[i].sending) {
            depth++;
        }
    }
    return depth;
}

static void queue_clear_ready_locked(void)
{
    for (int i = 0; i < IMAGE_QUEUE_DEPTH; i++) {
        if (s_image_slots[i].ready) {
            s_image_slots[i].ready = false;
            s_image_slots[i].jpeg_len = 0;
        }
    }
}

static void queue_drop_stale_locked(int64_t now_ms)
{
    for (int i = 0; i < IMAGE_QUEUE_DEPTH; i++) {
        image_slot_t *slot = &s_image_slots[i];
        if (slot->ready && now_ms - slot->created_ms > TCP_IMAGE_STALE_MS) {
            slot->ready = false;
            slot->jpeg_len = 0;
            s_image_stats.dropped_stale++;
        }
    }
}

static image_slot_t *queue_find_free_locked(void)
{
    for (int i = 0; i < IMAGE_QUEUE_DEPTH; i++) {
        if (!s_image_slots[i].ready && !s_image_slots[i].sending) {
            return &s_image_slots[i];
        }
    }
    return NULL;
}

static void drain_encode_queue(void)
{
    const vision_upload_frame_t *frame = NULL;
    while (s_encode_queue && xQueueReceive(s_encode_queue, &frame, 0) == pdTRUE) {
        if (!vision_upload_frame_release(frame)) {
            ESP_LOGE(TAG, "failed to release drained vision upload");
        }
    }
}

static bool enqueue_vision_upload(const vision_upload_frame_t *frame, void *ctx)
{
    (void)ctx;
    if (!frame) {
        return false;
    }
    if (!s_submit_gate || xSemaphoreTake(s_submit_gate, 0) != pdTRUE) {
        atomic_fetch_add(&s_submit_queue_full, 1);
        return false;
    }

    EventBits_t bits = s_eth_events ? xEventGroupGetBits(s_eth_events) : 0;
    bool accepted = false;
    if (!atomic_load(&s_report_image_en) || (bits & TCP_IMAGE_CONNECTED_BIT) == 0) {
        atomic_fetch_add(&s_submit_no_connection, 1);
    } else if (!s_encode_queue || xQueueSend(s_encode_queue, &frame, 0) != pdTRUE) {
        atomic_fetch_add(&s_submit_queue_full, 1);
    } else {
        accepted = true;
    }
    xSemaphoreGive(s_submit_gate);
    return accepted;
}

static image_slot_t *queue_pop_ready_locked(void)
{
    image_slot_t *best = NULL;
    for (int i = 0; i < IMAGE_QUEUE_DEPTH; i++) {
        image_slot_t *slot = &s_image_slots[i];
        if (!slot->ready) {
            continue;
        }
        if (!best || slot->created_ms < best->created_ms) {
            best = slot;
        }
    }
    if (best) {
        best->ready = false;
        best->sending = true;
    }
    return best;
}

static void snapshot_image_stats(image_stats_t *stats, int *queue_depth)
{
    if (xSemaphoreTake(s_image_lock, pdMS_TO_TICKS(20)) == pdTRUE) {
        *stats = s_image_stats;
        *queue_depth = queue_depth_locked();
        xSemaphoreGive(s_image_lock);
    } else {
        memset(stats, 0, sizeof(*stats));
        *queue_depth = -1;
    }
    stats->dropped_backpressure += atomic_load(&s_submit_queue_full);
    stats->no_connection = atomic_load(&s_submit_no_connection);
}

static int send_metrics_packet(int sock)
{
    system_monitor_metrics_t m = {0};
    bool have_monitor = system_monitor_get_metrics(&m) == ESP_OK;
    if (!have_monitor) {
        m.free_heap = esp_get_free_heap_size();
        m.min_free_heap = esp_get_minimum_free_heap_size();
        m.free_internal = heap_caps_get_free_size(MALLOC_CAP_INTERNAL);
        m.free_psram = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
        m.total_psram = heap_caps_get_total_size(MALLOC_CAP_SPIRAM);
        m.largest_free_block = heap_caps_get_largest_free_block(MALLOC_CAP_DEFAULT);
        m.uptime_ms = (uint64_t)monotonic_ms();
    }

    image_stats_t stats;
    int image_queue_depth = 0;
    snapshot_image_stats(&stats, &image_queue_depth);

    char json[1024];
    int len = snprintf(json, sizeof(json),
                       "{\"seq\":%lu,\"uptime_ms\":%llu,\"cpu_usage\":%u,"
                       "\"cpu0\":%u,\"cpu1\":%u,"
                       "\"free_heap\":%lu,\"min_free_heap\":%lu,"
                       "\"free_internal\":%lu,\"free_psram\":%lu,"
                       "\"total_psram\":%lu,\"largest_free_block\":%lu,"
                       "\"image_queue_depth\":%d,\"image_encoded\":%lu,"
                       "\"image_sent\":%lu,\"image_drop_backpressure\":%lu,"
                       "\"image_drop_stale\":%lu,\"image_encode_fail\":%lu,"
                       "\"image_send_fail\":%lu,\"image_no_frame\":%lu,"
                       "\"image_no_connection\":%lu,"
                       "\"last_image_encode_ms\":%d,\"last_image_send_ms\":%d,"
                       "\"last_image_bytes\":%u,"
                       "\"image_frame_id\":%lu,\"image_box_count\":%u,"
                       "\"image_jpeg_bytes\":%u,\"image_encode_ms\":%d,"
                       "\"image_send_ms\":%d}",
                       (unsigned long)(__atomic_load_n(&s_tx_seq, __ATOMIC_RELAXED) + 1),
                       (unsigned long long)m.uptime_ms,
                       (unsigned)m.cpu_usage_total,
                       (unsigned)m.cpu_usage_per_core[0],
                       (unsigned)m.cpu_usage_per_core[1],
                       (unsigned long)m.free_heap,
                       (unsigned long)m.min_free_heap,
                       (unsigned long)m.free_internal,
                       (unsigned long)m.free_psram,
                       (unsigned long)m.total_psram,
                       (unsigned long)m.largest_free_block,
                       image_queue_depth,
                       (unsigned long)stats.encoded,
                       (unsigned long)stats.sent,
                       (unsigned long)stats.dropped_backpressure,
                       (unsigned long)stats.dropped_stale,
                       (unsigned long)stats.encode_fail,
                       (unsigned long)stats.send_fail,
                       (unsigned long)stats.no_frame,
                       (unsigned long)stats.no_connection,
                       stats.last_encode_ms,
                       stats.last_send_ms,
                       (unsigned)stats.last_jpeg_bytes,
                       (unsigned long)stats.last_frame_id,
                       (unsigned)stats.last_box_count,
                       (unsigned)stats.last_jpeg_bytes,
                       stats.last_encode_ms,
                       stats.last_send_ms);
    if (len <= 0 || len >= (int)sizeof(json)) {
        return -1;
    }
    return send_packet(sock, ESP_HOST_TYPE_METRICS, json, (uint32_t)len, 0, 0, 0, 0, 0, false);
}

static esp_err_t ensure_image_resources(void)
{
    bool allocated_jpeg = false;
    for (int i = 0; i < IMAGE_QUEUE_DEPTH; i++) {
        if (!s_image_slots[i].jpeg) {
            s_image_slots[i].jpeg = (uint8_t *)heap_caps_aligned_alloc(
                16, JPEG_OUTBUF_BYTES, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
            ESP_RETURN_ON_FALSE(s_image_slots[i].jpeg != NULL, ESP_ERR_NO_MEM, TAG, "allocate JPEG buffer failed");
            allocated_jpeg = true;
        }
    }
    if (allocated_jpeg) {
        ESP_LOGI(TAG, "allocated %d JPEG slots (%u bytes each)",
                 IMAGE_QUEUE_DEPTH, (unsigned)JPEG_OUTBUF_BYTES);
    }
    if (!s_jpeg_enc) {
        jpeg_enc_config_t cfg = DEFAULT_JPEG_ENC_CONFIG();
        cfg.width = SNAPSHOT_WIDTH;
        cfg.height = SNAPSHOT_HEIGHT;
        cfg.src_type = JPEG_PIXEL_FORMAT_RGB888;
        cfg.subsampling = JPEG_SUBSAMPLE_444;
        cfg.quality = JPEG_QUALITY;
        cfg.rotate = JPEG_ROTATE_0D;
        cfg.task_enable = false;
        cfg.hfm_task_priority = 13;
        cfg.hfm_task_core = 1;
        jpeg_error_t err = jpeg_enc_open(&cfg, &s_jpeg_enc);
        ESP_RETURN_ON_FALSE(err == JPEG_ERR_OK, ESP_FAIL, TAG, "jpeg encoder open failed err=%d", (int)err);
    }
    return ESP_OK;
}

static uint16_t clamp_u16(uint16_t value, uint16_t maximum)
{
    return value > maximum ? maximum : value;
}

static esp_err_t produce_jpeg_snapshot(image_slot_t *slot,
                                       const vision_upload_frame_t *frame)
{
    if (!slot || !slot->jpeg || !frame || !frame->rgb888 ||
        frame->width != SNAPSHOT_WIDTH || frame->height != SNAPSHOT_HEIGHT ||
        frame->rgb_len != VISION_UPLOAD_RGB_BYTES) {
        return ESP_ERR_INVALID_ARG;
    }
    if (!vision_upload_frame_begin_encoding(frame)) {
        ESP_LOGE(TAG, "vision upload begin_encoding failed");
        return ESP_ERR_INVALID_STATE;
    }

    esp_err_t ret = ESP_FAIL;
    int64_t start_ms = monotonic_ms();
    int encoded = 0;
    jpeg_enc_set_quality(s_jpeg_enc, JPEG_QUALITY);
    jpeg_error_t err = jpeg_enc_process(s_jpeg_enc,
                                        (uint8_t *)frame->rgb888,
                                        (int)frame->rgb_len,
                                        slot->jpeg,
                                        JPEG_OUTBUF_BYTES,
                                        &encoded);
    if (err != JPEG_ERR_OK) {
        ESP_LOGW(TAG, "jpeg encode failed err=%d encoded=%d", (int)err, encoded);
        goto cleanup;
    }
    if (encoded <= 0 || encoded > JPEG_OUTBUF_BYTES) {
        ESP_LOGW(TAG, "invalid JPEG output size=%d", encoded);
        ret = ESP_ERR_INVALID_SIZE;
        goto cleanup;
    }

    memset(&slot->meta, 0, sizeof(slot->meta));
    memset(slot->boxes, 0, sizeof(slot->boxes));
    slot->meta.frame_id = frame->frame_id;
    slot->meta.capture_timestamp_us = (uint64_t)frame->capture_timestamp_us;
    slot->meta.jpeg_len = (uint32_t)encoded;
    slot->meta.primary_class_id = frame->primary_class_id;
    slot->meta.primary_confidence = clamp_u16(frame->primary_confidence_x1000, 1000);
    slot->meta.infer_time_ms = frame->infer_time_ms;
    slot->meta.source_width = frame->width;
    slot->meta.source_height = frame->height;
    slot->meta.meta_size = ESP_HOST_IMAGE_META_SIZE;
    slot->meta.box_size = ESP_HOST_IMAGE_BOX_SIZE;

    uint16_t source_max_x = frame->width - 1;
    uint16_t source_max_y = frame->height - 1;
    uint16_t input_count = frame->box_count > ESP_HOST_IMAGE_MAX_BOXES ?
                           ESP_HOST_IMAGE_MAX_BOXES : frame->box_count;
    uint16_t output_count = 0;
    for (uint16_t i = 0; i < input_count; i++) {
        uint16_t x1 = clamp_u16(frame->boxes[i].x1, source_max_x);
        uint16_t y1 = clamp_u16(frame->boxes[i].y1, source_max_y);
        uint16_t x2 = clamp_u16(frame->boxes[i].x2, source_max_x);
        uint16_t y2 = clamp_u16(frame->boxes[i].y2, source_max_y);
        if (x1 > x2 || y1 > y2) {
            continue;
        }
        esp_host_image_box_v2_t *box = &slot->boxes[output_count++];
        box->stage = frame->boxes[i].stage;
        box->category = frame->boxes[i].category;
        box->score = clamp_u16(frame->boxes[i].score_x1000, 1000);
        box->x1 = x1;
        box->y1 = y1;
        box->x2 = x2;
        box->y2 = y2;
        box->flags = 0;
        box->reserved = 0;
    }
    slot->meta.box_count = output_count;
    slot->jpeg_len = (size_t)encoded;
    slot->created_ms = monotonic_ms();
    slot->encode_ms = (int)(slot->created_ms - start_ms);
    ret = ESP_OK;

cleanup:
    if (!vision_upload_frame_release(frame)) {
        ESP_LOGE(TAG, "vision upload release failed after encoding");
        ret = ESP_ERR_INVALID_STATE;
    }
    return ret;
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

static bool sim_line_has_reason(const char *line, const char *reason)
{
    const char *p = strstr(line, "reason=");
    if (!p) {
        return false;
    }
    p += strlen("reason=");
    size_t reason_len = strlen(reason);
    return strncmp(p, reason, reason_len) == 0 && (p[reason_len] == '\0' || p[reason_len] == ',');
}

static bool parse_pkg_signature(const char *line, int *id_out, char *signature, size_t signature_len)
{
    int id = -1;
    char belt[8] = {0};
    char state[32] = {0};
    char cls[32] = {0};
    int matched = sscanf(line, "PKG,id=%d,belt=%7[^,],pos=%*f,state=%31[^,],class=%31s",
                         &id, belt, state, cls);
    if (matched != 4 || id < 0 || !signature || signature_len == 0) {
        return false;
    }
    int n = snprintf(signature, signature_len, "id=%d,belt=%s,state=%s,class=%s", id, belt, state, cls);
    if (n <= 0 || n >= (int)signature_len) {
        return false;
    }
    *id_out = id;
    return true;
}

static bool should_send_sim_line(const char *line)
{
    if (!line || line[0] == '\0') {
        return false;
    }

    if (strncmp(line, "STATUS,", 7) == 0) {
        if (sim_line_has_reason(line, "tick") ||
            sim_line_has_reason(line, "sensor1") ||
            sim_line_has_reason(line, "package_new") ||
            sim_line_has_reason(line, "vision")) {
            return false;
        }
        return true;
    }

    if (strncmp(line, "PKG,", 4) == 0) {
        int id = -1;
        char signature[80];
        if (!parse_pkg_signature(line, &id, signature, sizeof(signature))) {
            return true;
        }

        sim_pkg_filter_t *slot = &s_sim_pkg_filters[id % SORTER_MAX_PACKAGES];
        int64_t now_ms = monotonic_ms();
        bool changed = slot->id != id || strcmp(slot->signature, signature) != 0;
        bool heartbeat_due = now_ms - slot->last_sent_ms >= TCP_SIM_PKG_HEARTBEAT_MS;
        if (!changed && !heartbeat_due) {
            return false;
        }
        slot->id = id;
        strlcpy(slot->signature, signature, sizeof(slot->signature));
        slot->last_sent_ms = now_ms;
        return true;
    }

    return true;
}

static int send_sim_line_packet(void *ctx, const char *line)
{
    if (!should_send_sim_line(line)) {
        return 0;
    }
    int sock = (int)(intptr_t)ctx;
    int ret = send_packet(sock, ESP_HOST_TYPE_SIM_LINE, line, (uint32_t)strlen(line), 0, 0, 0, 0, 0, false);
    if (ret != 0) {
        s_control_send_failed = true;
        ESP_LOGW(TAG, "SIM line send failed errno=%d line=%s", errno, line);
    }
    return ret;
}

static int send_control_object(int sock, cJSON *root)
{
    char *json = cJSON_PrintUnformatted(root);
    if (!json) {
        return -1;
    }
    int ret = send_packet(sock, ESP_HOST_TYPE_CONTROL_JSON, json,
                          (uint32_t)strlen(json), 0, 0, 0, 0, 0, false);
    cJSON_free(json);
    if (ret != 0) {
        s_control_send_failed = true;
    }
    return ret;
}

static void send_control_error(int sock, const char *key, const char *message)
{
    cJSON *root = cJSON_CreateObject();
    if (!root) {
        s_control_send_failed = true;
        return;
    }
    cJSON_AddStringToObject(root, "op", "error");
    if (key) {
        cJSON_AddStringToObject(root, "key", key);
    }
    cJSON_AddStringToObject(root, "message", message ? message : "control error");
    send_control_object(sock, root);
    cJSON_Delete(root);
}

static void add_isp_capability(cJSON *caps, const char *key,
                               const cam_sensor_isp_control_info_t *info)
{
    cJSON *item = cJSON_AddObjectToObject(caps, key);
    if (!item) {
        return;
    }
    cJSON_AddBoolToObject(item, "supported", info && info->supported);
    cJSON_AddNumberToObject(item, "min", info ? info->minimum : 0);
    cJSON_AddNumberToObject(item, "max", info ? info->maximum : 0);
    cJSON_AddNumberToObject(item, "step", info && info->step > 0 ? info->step : 1);
}

static void sync_remote_control_ui(void)
{
    BSP_LVGL_Lock();
    ui_sync_remote_control_state(
        BSP_LCD_GetBrightness(), vision_is_detection_enabled(),
        vision_is_preview_overlay_enabled(),
        vision_model_get_waybill_score_threshold_percent(),
        vision_model_get_logo_score_threshold_percent(),
        ethernet_app_get_report_image_enabled(),
        ethernet_app_get_report_metrics_enabled());
    BSP_LVGL_Unlock();
}

static int send_control_state(int sock)
{
    cam_sensor_isp_state_t isp = {0};
    esp_err_t isp_err = cam_sensor_isp_get_state(&isp);
    sorting_debug_settings_t sorter = {0};
    sorting_sim_control_get_settings(&sorter);

    cJSON *root = cJSON_CreateObject();
    cJSON *settings = root ? cJSON_AddObjectToObject(root, "settings") : NULL;
    cJSON *caps = root ? cJSON_AddObjectToObject(root, "capabilities") : NULL;
    if (!root || !settings || !caps) {
        cJSON_Delete(root);
        s_control_send_failed = true;
        return -1;
    }
    cJSON_AddStringToObject(root, "op", "state");
    cJSON_AddBoolToObject(root, "ok", true);
    cJSON_AddNumberToObject(settings, "display.screen_brightness", BSP_LCD_GetBrightness());
    cJSON_AddNumberToObject(settings, "camera.brightness",
                            isp.brightness_valid ? isp.brightness : isp.brightness_info.default_value);
    cJSON_AddNumberToObject(settings, "camera.contrast",
                            isp.contrast_valid ? isp.contrast : isp.contrast_info.default_value);
    cJSON_AddBoolToObject(settings, "camera.contrast_auto", isp.contrast_auto);
    cJSON_AddNumberToObject(settings, "camera.saturation",
                            isp.saturation_valid ? isp.saturation : isp.saturation_info.default_value);
    cJSON_AddBoolToObject(settings, "camera.saturation_auto", isp.saturation_auto);
    cJSON_AddNumberToObject(settings, "camera.hue",
                            isp.hue_valid ? isp.hue : isp.hue_info.default_value);
    cJSON_AddBoolToObject(settings, "vision.detection_enabled", vision_is_detection_enabled());
    cJSON_AddBoolToObject(settings, "vision.preview_overlay_enabled", vision_is_preview_overlay_enabled());
    cJSON_AddNumberToObject(settings, "vision.waybill_threshold",
                            vision_model_get_waybill_score_threshold_percent());
    cJSON_AddNumberToObject(settings, "vision.logo_threshold",
                            vision_model_get_logo_score_threshold_percent());
    cJSON_AddNumberToObject(settings, "sorter.motor_a_speed", sorter.motor_speed_percent[0]);
    cJSON_AddNumberToObject(settings, "sorter.motor_b_speed", sorter.motor_speed_percent[1]);
    cJSON_AddNumberToObject(settings, "sorter.motor_c_speed", sorter.motor_speed_percent[2]);
    cJSON_AddBoolToObject(settings, "report.image_enabled",
                         ethernet_app_get_report_image_enabled());
    cJSON_AddBoolToObject(settings, "report.metrics_enabled",
                         ethernet_app_get_report_metrics_enabled());
    cJSON_AddStringToObject(settings, "network.local_ip", ethernet_app_get_local_ip());
    cJSON_AddStringToObject(settings, "network.host_ip", ethernet_app_get_host_ip());
    cJSON_AddStringToObject(settings, "model.info", vision_model_get_model_info_string());
    add_isp_capability(caps, "camera.brightness", &isp.brightness_info);
    add_isp_capability(caps, "camera.contrast", &isp.contrast_info);
    add_isp_capability(caps, "camera.saturation", &isp.saturation_info);
    add_isp_capability(caps, "camera.hue", &isp.hue_info);
    if (isp_err != ESP_OK) {
        ESP_LOGW(TAG, "control state ISP read failed: %s", esp_err_to_name(isp_err));
    }
    int ret = send_control_object(sock, root);
    cJSON_Delete(root);
    return ret;
}

static bool json_integer(const cJSON *item, int *value)
{
    if (!cJSON_IsNumber(item) || !isfinite(item->valuedouble) ||
        floor(item->valuedouble) != item->valuedouble ||
        item->valuedouble < INT_MIN || item->valuedouble > INT_MAX) {
        return false;
    }
    *value = (int)item->valuedouble;
    return true;
}

static esp_err_t validate_isp_value(const cam_sensor_isp_control_info_t *info, int value)
{
    if (!info || !info->supported) {
        return ESP_ERR_NOT_SUPPORTED;
    }
    if (value < info->minimum || value > info->maximum) {
        return ESP_ERR_INVALID_ARG;
    }
    int step = info->step > 0 ? info->step : 1;
    return ((value - info->minimum) % step) == 0 ? ESP_OK : ESP_ERR_INVALID_ARG;
}

static esp_err_t apply_control_set(const char *key, const cJSON *value,
                                   const char **message)
{
    int number = 0;
    bool boolean = cJSON_IsTrue(value);
    bool is_bool = cJSON_IsBool(value);
    if (strcmp(key, "display.screen_brightness") == 0) {
        if (!json_integer(value, &number) || number < 0 || number > 100) {
            *message = "brightness must be an integer from 0 to 100";
            return ESP_ERR_INVALID_ARG;
        }
        BSP_LVGL_Lock();
        BSP_LCD_SetBrightness(number);
        BSP_LVGL_Unlock();
        return ESP_OK;
    }

    cam_sensor_isp_state_t isp = {0};
    bool isp_key = strncmp(key, "camera.", 7) == 0;
    if (isp_key && cam_sensor_isp_get_state(&isp) != ESP_OK) {
        *message = "camera ISP state unavailable";
        return ESP_ERR_INVALID_STATE;
    }
    if (strcmp(key, "camera.contrast_auto") == 0 ||
        strcmp(key, "camera.saturation_auto") == 0) {
        if (!is_bool) {
            *message = "value must be boolean";
            return ESP_ERR_INVALID_ARG;
        }
        return strcmp(key, "camera.contrast_auto") == 0
            ? cam_sensor_isp_set_contrast_auto(boolean)
            : cam_sensor_isp_set_saturation_auto(boolean);
    }
    const cam_sensor_isp_control_info_t *info = NULL;
    esp_err_t (*isp_setter)(int32_t) = NULL;
    if (strcmp(key, "camera.brightness") == 0) {
        info = &isp.brightness_info; isp_setter = cam_sensor_isp_set_brightness;
    } else if (strcmp(key, "camera.contrast") == 0) {
        info = &isp.contrast_info; isp_setter = cam_sensor_isp_set_contrast;
    } else if (strcmp(key, "camera.saturation") == 0) {
        info = &isp.saturation_info; isp_setter = cam_sensor_isp_set_saturation;
    } else if (strcmp(key, "camera.hue") == 0) {
        info = &isp.hue_info; isp_setter = cam_sensor_isp_set_hue;
    }
    if (isp_setter) {
        if (!json_integer(value, &number)) {
            *message = "value must be an integer";
            return ESP_ERR_INVALID_ARG;
        }
        esp_err_t valid = validate_isp_value(info, number);
        if (valid != ESP_OK) {
            *message = valid == ESP_ERR_NOT_SUPPORTED
                ? "camera control is not supported" : "value is outside capability range or step";
            return valid;
        }
        return isp_setter(number);
    }
    if (strcmp(key, "vision.detection_enabled") == 0 ||
        strcmp(key, "vision.preview_overlay_enabled") == 0 ||
        strcmp(key, "report.image_enabled") == 0 ||
        strcmp(key, "report.metrics_enabled") == 0) {
        if (!is_bool) {
            *message = "value must be boolean";
            return ESP_ERR_INVALID_ARG;
        }
        if (strcmp(key, "vision.detection_enabled") == 0) vision_set_detection_enabled(boolean);
        else if (strcmp(key, "vision.preview_overlay_enabled") == 0) vision_set_preview_overlay_enabled(boolean);
        else if (strcmp(key, "report.image_enabled") == 0) ethernet_app_set_report_image_enabled(boolean);
        else ethernet_app_set_report_metrics_enabled(boolean);
        return ESP_OK;
    }
    if (strcmp(key, "vision.waybill_threshold") == 0 ||
        strcmp(key, "vision.logo_threshold") == 0) {
        if (!json_integer(value, &number) || number < 0 || number > 100) {
            *message = "threshold must be an integer from 0 to 100";
            return ESP_ERR_INVALID_ARG;
        }
        if (strcmp(key, "vision.waybill_threshold") == 0)
            vision_model_set_waybill_score_threshold_percent(number);
        else
            vision_model_set_logo_score_threshold_percent(number);
        return ESP_OK;
    }
    int motor = -1;
    if (strcmp(key, "sorter.motor_a_speed") == 0) motor = 0;
    else if (strcmp(key, "sorter.motor_b_speed") == 0) motor = 1;
    else if (strcmp(key, "sorter.motor_c_speed") == 0) motor = 2;
    if (motor >= 0) {
        if (!json_integer(value, &number) || number < 0 || number > 100) {
            *message = "motor speed must be an integer from 0 to 100";
            return ESP_ERR_INVALID_ARG;
        }
        sorting_debug_settings_t settings;
        sorting_sim_control_get_settings(&settings);
        settings.motor_speed_percent[motor] = number;
        sorting_sim_control_apply_settings(&settings);
        return ESP_OK;
    }
    *message = "unknown control key";
    return ESP_ERR_NOT_FOUND;
}

static void process_control_json(int sock, const uint8_t *payload, size_t len)
{
    if (!payload || len == 0 || len > TCP_RX_MAX_PAYLOAD_BYTES || memchr(payload, '\0', len)) {
        send_control_error(sock, NULL, "invalid control JSON payload");
        return;
    }
    const char *end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts((const char *)payload, len, &end, false);
    if (!root || !cJSON_IsObject(root) || end != (const char *)payload + len) {
        cJSON_Delete(root);
        send_control_error(sock, NULL, "invalid control JSON");
        return;
    }
    const cJSON *op = cJSON_GetObjectItemCaseSensitive(root, "op");
    if (!cJSON_IsString(op) || !op->valuestring) {
        cJSON_Delete(root);
        send_control_error(sock, NULL, "op must be a string");
        return;
    }
    if (strcmp(op->valuestring, "get") == 0) {
        send_control_state(sock);
        cJSON_Delete(root);
        return;
    }
    const cJSON *key_item = cJSON_GetObjectItemCaseSensitive(root, "key");
    if (!cJSON_IsString(key_item) || !key_item->valuestring || key_item->valuestring[0] == '\0') {
        cJSON_Delete(root);
        send_control_error(sock, NULL, "key must be a non-empty string");
        return;
    }
    char key[64];
    if (strlen(key_item->valuestring) >= sizeof(key)) {
        cJSON_Delete(root);
        send_control_error(sock, NULL, "control key is too long");
        return;
    }
    strcpy(key, key_item->valuestring);
    if (strcmp(op->valuestring, "set") == 0) {
        const cJSON *value = cJSON_GetObjectItemCaseSensitive(root, "value");
        if (!value || cJSON_IsNull(value)) {
            cJSON_Delete(root);
            send_control_error(sock, key, "set requires value");
            return;
        }
        const char *message = "control rejected";
        esp_err_t err = apply_control_set(key, value, &message);
        cJSON_Delete(root);
        if (err != ESP_OK) {
            send_control_error(sock, key, message);
            return;
        }
        sync_remote_control_ui();
        send_control_state(sock);
        return;
    }
    if (strcmp(op->valuestring, "action") == 0) {
        bool restart = strcmp(key, "device.restart") == 0;
        cJSON_Delete(root);
        if (!restart) {
            send_control_error(sock, key, "unknown action key");
            return;
        }
        send_control_state(sock);
        vTaskDelay(pdMS_TO_TICKS(50));
        esp_restart();
    }
    cJSON_Delete(root);
    send_control_error(sock, key, "unknown control op");
}

static void process_rx_packet(int sock, const esp_host_packet_header_t *header, const uint8_t *payload)
{
    if (header->type == ESP_HOST_TYPE_TIME_SYNC) {
        apply_time_sync((const char *)payload, header->payload_len);
    } else if (header->type == ESP_HOST_TYPE_SIM_LINE) {
        sorting_sim_control_handle_line((const char *)payload, header->payload_len,
                                        send_sim_line_packet, (void *)(intptr_t)sock);
    } else if (header->type == ESP_HOST_TYPE_CONTROL_JSON) {
        process_control_json(sock, payload, header->payload_len);
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
            ESP_LOGW(TAG, "%s connect failed errno=%d", name, errno);
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
            ESP_LOGW(TAG, "%s connect timeout ret=%d errno=%d", name, ret, errno);
            close(sock);
            return -1;
        }

        int err = 0;
        socklen_t err_len = sizeof(err);
        if (getsockopt(sock, SOL_SOCKET, SO_ERROR, &err, &err_len) != 0 || err != 0) {
            ESP_LOGW(TAG, "%s connect failed so_error=%d errno=%d", name, err, errno);
            close(sock);
            return -1;
        }
    }

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
    ESP_LOGI(TAG, "%s connected to host", name);
    return sock;
}

static void control_tcp_task(void *arg)
{
    (void)arg;
    while (true) {
        xEventGroupWaitBits(s_eth_events, TCP_CONNECT_READY_BIT, pdFALSE, pdTRUE, portMAX_DELAY);

        int sock = connect_to_host(HOST_CONTROL_PORT, "control");
        if (sock < 0) {
            vTaskDelay(pdMS_TO_TICKS(2000));
            continue;
        }
        poll_incoming_packets(sock, true);
        s_control_send_failed = false;

        int64_t next_metrics_ms = monotonic_ms() + 1000;
        while (true) {
            if (!poll_incoming_packets(sock, false)) {
                ESP_LOGW(TAG, "TCP control peer closed");
                break;
            }
            if (s_control_send_failed) {
                ESP_LOGW(TAG, "TCP control send failed; reconnecting");
                break;
            }

#if SORTER_TCP_SIM_LINE_OUTPUT_ENABLE
            sorting_sim_control_tick(send_sim_line_packet, (void *)(intptr_t)sock);
#else
            sorting_sim_control_tick(NULL, NULL);
#endif
            if (s_control_send_failed) {
                ESP_LOGW(TAG, "TCP SIM line send failed during tick; reconnecting");
                break;
            }

            int64_t now_ms = monotonic_ms();
            uint32_t interval = s_metrics_interval_ms;
            if (s_report_metrics_en && interval > 0 && now_ms >= next_metrics_ms) {
                if (send_metrics_packet(sock) != 0) {
                    ESP_LOGW(TAG, "TCP metrics send failed errno=%d", errno);
                    break;
                }
                next_metrics_ms = now_ms + interval;
            } else if (!s_report_metrics_en || interval == 0) {
                // 关闭期间持续顺延，重新开启后立即发一帧而非补发积压
                next_metrics_ms = now_ms;
            }
            vTaskDelay(pdMS_TO_TICKS(20));
        }

        close(sock);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

static void image_producer_task(void *arg)
{
    (void)arg;
    while (true) {
        const vision_upload_frame_t *frame = NULL;
        if (xQueueReceive(s_encode_queue, &frame, portMAX_DELAY) != pdTRUE) {
            continue;
        }

        EventBits_t bits = xEventGroupGetBits(s_eth_events);
        if (!atomic_load(&s_report_image_en) || (bits & TCP_IMAGE_CONNECTED_BIT) == 0) {
            atomic_fetch_add(&s_submit_no_connection, 1);
            if (!vision_upload_frame_release(frame)) {
                ESP_LOGE(TAG, "failed to release disabled/disconnected upload");
            }
            continue;
        }

        if (ensure_image_resources() != ESP_OK) {
            if (xSemaphoreTake(s_image_lock, portMAX_DELAY) == pdTRUE) {
                s_image_stats.encode_fail++;
                xSemaphoreGive(s_image_lock);
            }
            if (!vision_upload_frame_release(frame)) {
                ESP_LOGE(TAG, "failed to release upload after resource error");
            }
            continue;
        }

        image_slot_t *slot = NULL;
        if (xSemaphoreTake(s_image_lock, portMAX_DELAY) == pdTRUE) {
            queue_drop_stale_locked(monotonic_ms());
            slot = queue_find_free_locked();
            xSemaphoreGive(s_image_lock);
        }
        if (!slot) {
            if (xSemaphoreTake(s_image_lock, portMAX_DELAY) == pdTRUE) {
                s_image_stats.dropped_backpressure++;
                xSemaphoreGive(s_image_lock);
            }
            if (!vision_upload_frame_release(frame)) {
                ESP_LOGE(TAG, "failed to release backpressured upload");
            }
            continue;
        }

        esp_err_t ret = produce_jpeg_snapshot(slot, frame);
        bits = xEventGroupGetBits(s_eth_events);
        bool publish = ret == ESP_OK && atomic_load(&s_report_image_en) &&
                       (bits & TCP_IMAGE_CONNECTED_BIT) != 0;
        if (xSemaphoreTake(s_image_lock, portMAX_DELAY) == pdTRUE) {
            slot->ready = publish;
            slot->sending = false;
            if (publish) {
                s_image_stats.encoded++;
                s_image_stats.last_encode_ms = slot->encode_ms;
                s_image_stats.last_jpeg_bytes = slot->jpeg_len;
                s_image_stats.last_frame_id = slot->meta.frame_id;
                s_image_stats.last_box_count = slot->meta.box_count;
                ESP_LOGD(TAG, "queued JPEG %ux%u payload=%u encode=%dms depth=%d",
                         SNAPSHOT_WIDTH, SNAPSHOT_HEIGHT,
                         (unsigned)slot->jpeg_len, slot->encode_ms, queue_depth_locked());
            } else {
                slot->jpeg_len = 0;
                if (ret != ESP_OK) {
                    s_image_stats.encode_fail++;
                } else {
                    s_image_stats.dropped_backpressure++;
                }
            }
            xSemaphoreGive(s_image_lock);
        }
    }
}

static void image_sender_task(void *arg)
{
    (void)arg;
    while (true) {
        xEventGroupWaitBits(s_eth_events, TCP_CONNECT_READY_BIT, pdFALSE, pdTRUE, portMAX_DELAY);

        int sock = connect_to_host(HOST_IMAGE_PORT, "image");
        if (sock < 0) {
            vTaskDelay(pdMS_TO_TICKS(2000));
            continue;
        }
        if (s_submit_gate && xSemaphoreTake(s_submit_gate, portMAX_DELAY) == pdTRUE) {
            xEventGroupSetBits(s_eth_events, TCP_IMAGE_CONNECTED_BIT);
            xSemaphoreGive(s_submit_gate);
        } else {
            xEventGroupSetBits(s_eth_events, TCP_IMAGE_CONNECTED_BIT);
        }

        while (true) {
            int64_t now_ms = monotonic_ms();
            image_slot_t *slot = NULL;
            if (xSemaphoreTake(s_image_lock, portMAX_DELAY) == pdTRUE) {
                queue_drop_stale_locked(now_ms);
                slot = queue_pop_ready_locked();
                xSemaphoreGive(s_image_lock);
            }

            if (!slot) {
                if (!socket_peer_alive(sock)) {
                    ESP_LOGW(TAG, "TCP image peer closed while idle errno=%d", errno);
                    break;
                }
                vTaskDelay(pdMS_TO_TICKS(50));
                continue;
            }

            EventBits_t bits = xEventGroupGetBits(s_eth_events);
            if (!atomic_load(&s_report_image_en) || (bits & TCP_IMAGE_CONNECTED_BIT) == 0) {
                if (xSemaphoreTake(s_image_lock, portMAX_DELAY) == pdTRUE) {
                    slot->sending = false;
                    slot->jpeg_len = 0;
                    xSemaphoreGive(s_image_lock);
                }
                continue;
            }

            int64_t send_start_ms = monotonic_ms();
            int ret = send_image_result_v2(sock, slot);
            int send_ms = (int)(monotonic_ms() - send_start_ms);

            if (xSemaphoreTake(s_image_lock, portMAX_DELAY) == pdTRUE) {
                slot->sending = false;
                slot->jpeg_len = 0;
                if (ret == 0) {
                    s_image_stats.sent++;
                    s_image_stats.last_send_ms = send_ms;
                } else {
                    s_image_stats.send_fail++;
                }
                xSemaphoreGive(s_image_lock);
            }
            if (ret == -2) {
                ESP_LOGW(TAG, "discarded invalid image v2 slot");
                continue;
            }
            if (ret != 0) {
                ESP_LOGW(TAG, "TCP image send failed errno=%d", errno);
                break;
            }
            ESP_LOGD(TAG, "sent image v2 frame=%lu boxes=%u jpeg=%u send=%dms",
                     (unsigned long)slot->meta.frame_id, (unsigned)slot->meta.box_count,
                     (unsigned)slot->meta.jpeg_len, send_ms);
        }

        if (s_submit_gate && xSemaphoreTake(s_submit_gate, portMAX_DELAY) == pdTRUE) {
            xEventGroupClearBits(s_eth_events, TCP_IMAGE_CONNECTED_BIT);
            drain_encode_queue();
            xSemaphoreGive(s_submit_gate);
        } else {
            xEventGroupClearBits(s_eth_events, TCP_IMAGE_CONNECTED_BIT);
            drain_encode_queue();
        }
        if (xSemaphoreTake(s_image_lock, pdMS_TO_TICKS(100)) == pdTRUE) {
            queue_clear_ready_locked();
            xSemaphoreGive(s_image_lock);
        }
        close(sock);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

static esp_err_t start_tcp_tasks(void)
{
    if (!s_image_lock) {
        s_image_lock = xSemaphoreCreateMutex();
        ESP_RETURN_ON_FALSE(s_image_lock != NULL, ESP_ERR_NO_MEM, TAG, "create image lock failed");
    }
    if (!s_submit_gate) {
        s_submit_gate = xSemaphoreCreateMutex();
        ESP_RETURN_ON_FALSE(s_submit_gate != NULL, ESP_ERR_NO_MEM, TAG, "create submit gate failed");
    }
    if (!s_encode_queue) {
        s_encode_queue = xQueueCreate(IMAGE_QUEUE_DEPTH, sizeof(const vision_upload_frame_t *));
        ESP_RETURN_ON_FALSE(s_encode_queue != NULL, ESP_ERR_NO_MEM, TAG, "create encode queue failed");
    }

    if (!s_control_task) {
        BaseType_t ok = xTaskCreatePinnedToCoreWithCaps(
            control_tcp_task, "eth_control", CONTROL_TASK_STACK_BYTES,
            NULL, CONTROL_TASK_PRIORITY, &s_control_task, 0, TCP_TASK_STACK_CAPS);
        ESP_RETURN_ON_FALSE(ok == pdPASS, ESP_ERR_NO_MEM, TAG,
                            "create TCP control task failed, free_internal=%u free_psram=%u",
                            (unsigned)heap_caps_get_free_size(MALLOC_CAP_INTERNAL),
                            (unsigned)heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
    }
    if (!s_image_prod_task) {
        BaseType_t ok = xTaskCreatePinnedToCoreWithCaps(
            image_producer_task, "eth_img_prod", IMAGE_PROD_TASK_STACK,
            NULL, IMAGE_PROD_TASK_PRIORITY, &s_image_prod_task, 0, TCP_TASK_STACK_CAPS);
        ESP_RETURN_ON_FALSE(ok == pdPASS, ESP_ERR_NO_MEM, TAG,
                            "create TCP image producer task failed, free_internal=%u free_psram=%u",
                            (unsigned)heap_caps_get_free_size(MALLOC_CAP_INTERNAL),
                            (unsigned)heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
    }
    if (!s_image_send_task) {
        BaseType_t ok = xTaskCreatePinnedToCoreWithCaps(
            image_sender_task, "eth_img_send", IMAGE_SEND_TASK_STACK,
            NULL, IMAGE_SEND_TASK_PRIORITY, &s_image_send_task, 0, TCP_TASK_STACK_CAPS);
        ESP_RETURN_ON_FALSE(ok == pdPASS, ESP_ERR_NO_MEM, TAG,
                            "create TCP image sender task failed, free_internal=%u free_psram=%u",
                            (unsigned)heap_caps_get_free_size(MALLOC_CAP_INTERNAL),
                            (unsigned)heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
    }

    vision_upload_set_submit_callback(enqueue_vision_upload, NULL);
    return ESP_OK;
}

static esp_err_t configure_static_ip(esp_netif_t *netif)
{
    esp_netif_ip_info_t ip_info = {0};

    ESP_RETURN_ON_ERROR(esp_netif_dhcpc_stop(netif), TAG, "stop Ethernet DHCP client failed");
    ESP_RETURN_ON_FALSE(esp_netif_str_to_ip4(LOCAL_IP, &ip_info.ip) == ESP_OK, ESP_FAIL, TAG, "invalid static IP");
    ESP_RETURN_ON_FALSE(esp_netif_str_to_ip4(LOCAL_NETMASK, &ip_info.netmask) == ESP_OK, ESP_FAIL, TAG, "invalid static netmask");
    ESP_RETURN_ON_FALSE(esp_netif_str_to_ip4(HOST_IP, &ip_info.gw) == ESP_OK, ESP_FAIL, TAG, "invalid static gateway");
    ESP_RETURN_ON_ERROR(esp_netif_set_ip_info(netif, &ip_info), TAG, "set Ethernet static IP failed");

    return ESP_OK;
}

// 把以太网连接状态推给 UI（connect/disconnect 图切换）。事件回调同步派发，
// 回调内仅 imgbtn 切 state，非阻塞；LVGL 操作须在锁内。
static void post_eth_status_ui(int connected)
{
    ethernet_event_data_t data = { .connected = connected };
    BSP_LVGL_Lock();
    send_event(get_current_event_table(), EVT_ETHERNET, EVT_ETHERNET_STATUS_CHANGED,
               (uint8_t *)&data, 0);
    BSP_LVGL_Unlock();
}

static void eth_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    (void)arg;
    (void)event_base;
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
        if (s_submit_gate && xSemaphoreTake(s_submit_gate, portMAX_DELAY) == pdTRUE) {
            xEventGroupClearBits(s_eth_events, TCP_CONNECT_READY_BIT | TCP_IMAGE_CONNECTED_BIT);
            drain_encode_queue();
            xSemaphoreGive(s_submit_gate);
        } else {
            xEventGroupClearBits(s_eth_events, TCP_CONNECT_READY_BIT | TCP_IMAGE_CONNECTED_BIT);
            drain_encode_queue();
        }
        post_eth_status_ui(0);   // 链路断开：UI 切到断开/失败图
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

static void got_ip_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    (void)arg;
    (void)event_base;
    (void)event_id;
    ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
    const esp_netif_ip_info_t *ip_info = &event->ip_info;

    ESP_LOGI(TAG, "Ethernet Got IP Address");
    ESP_LOGI(TAG, "~~~~~~~~~~~");
    ESP_LOGI(TAG, "ETHIP:" IPSTR, IP2STR(&ip_info->ip));
    ESP_LOGI(TAG, "ETHMASK:" IPSTR, IP2STR(&ip_info->netmask));
    ESP_LOGI(TAG, "ETHGW:" IPSTR, IP2STR(&ip_info->gw));
    ESP_LOGI(TAG, "~~~~~~~~~~~");
    xEventGroupSetBits(s_eth_events, TCP_CONNECT_READY_BIT);
    post_eth_status_ui(1);   // 获取到 IP：UI 切到连接成功图
}

esp_err_t ethernet_app_start(void)
{
#if !SORTER_TCP_LINK_ENABLE
    ESP_LOGI(TAG, "上位机 TCP 链路已关闭");
    return ESP_OK;
#else
    uint8_t eth_port_cnt = 0;
    esp_eth_handle_t *eth_handles = NULL;

    if (!s_eth_events) {
        s_eth_events = xEventGroupCreate();
        ESP_RETURN_ON_FALSE(s_eth_events != NULL, ESP_ERR_NO_MEM, TAG, "create Ethernet event group failed");
    }

    ESP_RETURN_ON_ERROR(example_eth_init(&eth_handles, &eth_port_cnt), TAG, "Ethernet init failed");
    ESP_RETURN_ON_FALSE(eth_port_cnt == 1, ESP_FAIL, TAG, "unexpected Ethernet port count: %u", eth_port_cnt);

    ESP_RETURN_ON_ERROR(esp_netif_init(), TAG, "esp-netif init failed");
    esp_err_t loop_ret = esp_event_loop_create_default();
    ESP_RETURN_ON_FALSE(loop_ret == ESP_OK || loop_ret == ESP_ERR_INVALID_STATE,
                        loop_ret, TAG, "default event loop init failed");

    esp_netif_config_t cfg = ESP_NETIF_DEFAULT_ETH();
    esp_netif_t *eth_netif = esp_netif_new(&cfg);
    ESP_RETURN_ON_FALSE(eth_netif != NULL, ESP_ERR_NO_MEM, TAG, "create Ethernet netif failed");

    esp_eth_netif_glue_handle_t eth_netif_glue = esp_eth_new_netif_glue(eth_handles[0]);
    ESP_RETURN_ON_FALSE(eth_netif_glue != NULL, ESP_ERR_NO_MEM, TAG, "create Ethernet netif glue failed");
    ESP_RETURN_ON_ERROR(esp_netif_attach(eth_netif, eth_netif_glue), TAG, "attach Ethernet netif failed");

    ESP_RETURN_ON_ERROR(configure_static_ip(eth_netif), TAG, "configure static IP failed");

    ESP_RETURN_ON_ERROR(esp_event_handler_register(ETH_EVENT, ESP_EVENT_ANY_ID, &eth_event_handler, NULL),
                        TAG, "register Ethernet event handler failed");
    ESP_RETURN_ON_ERROR(esp_event_handler_register(IP_EVENT, IP_EVENT_ETH_GOT_IP, &got_ip_event_handler, NULL),
                        TAG, "register Ethernet IP event handler failed");
    ESP_RETURN_ON_ERROR(esp_eth_start(eth_handles[0]), TAG, "start Ethernet failed");
    ESP_RETURN_ON_ERROR(start_tcp_tasks(), TAG, "start TCP tasks failed");

    return ESP_OK;
#endif
}

const char *ethernet_app_get_local_ip(void) { return LOCAL_IP; }
const char *ethernet_app_get_host_ip(void) { return HOST_IP; }

void ethernet_app_set_metrics_interval_ms(uint32_t ms)
{
    s_metrics_interval_ms = ms;
    ESP_LOGI(TAG, "metrics interval set to %lu ms", (unsigned long)ms);
}

void ethernet_app_set_report_image_enabled(bool en)
{
    if (s_submit_gate && xSemaphoreTake(s_submit_gate, portMAX_DELAY) == pdTRUE) {
        atomic_store(&s_report_image_en, en);
        if (!en) {
            drain_encode_queue();
        }
        xSemaphoreGive(s_submit_gate);
    } else {
        atomic_store(&s_report_image_en, en);
        if (!en) {
            drain_encode_queue();
        }
    }
    if (!en && s_image_lock && xSemaphoreTake(s_image_lock, portMAX_DELAY) == pdTRUE) {
        queue_clear_ready_locked();
        xSemaphoreGive(s_image_lock);
    }
    ESP_LOGI(TAG, "image report %s", en ? "enabled" : "disabled");
}

void ethernet_app_set_report_metrics_enabled(bool en)
{
    s_report_metrics_en = en;
    ESP_LOGI(TAG, "metrics report %s", en ? "enabled" : "disabled");
}

bool ethernet_app_get_report_image_enabled(void)
{
    return atomic_load(&s_report_image_en);
}

bool ethernet_app_get_report_metrics_enabled(void)
{
    return s_report_metrics_en;
}
