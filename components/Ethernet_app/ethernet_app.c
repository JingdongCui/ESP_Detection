#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/idf_additions.h"
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
#include "ethernet_app.h"
#include "ethernet_init.h"
#include "sorting_sim_control.h"
#include "system_monitor.h"
#include "vision.h"
#include "lwip/inet.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/tcp.h"
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

static const char *TAG = "eth_example";

#define HOST_IP                    "192.168.10.1"
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

#define SNAPSHOT_WIDTH             640
#define SNAPSHOT_HEIGHT            375
#define SNAPSHOT_RGB_BYTES         (SNAPSHOT_WIDTH * SNAPSHOT_HEIGHT * 3)
#define SNAPSHOT_CACHE_ALIGNMENT   64
#define JPEG_OUTBUF_BYTES          (512 * 1024)
#define JPEG_QUALITY               60
#define IMAGE_QUEUE_DEPTH          2

#define ESP_HOST_MAGIC             0x32505345u
#define ESP_HOST_VERSION           1u
#define ESP_HOST_HEADER_SIZE       40u
#define ESP_HOST_TYPE_IMAGE        0x01u
#define ESP_HOST_TYPE_METRICS      0x02u
#define ESP_HOST_TYPE_TIME_SYNC    0x10u
#define ESP_HOST_TYPE_SIM_LINE     0x12u
#define ESP_HOST_PIXEL_RGB888      1u
#define ESP_HOST_PIXEL_JPEG        2u
#define TCP_SIM_PKG_HEARTBEAT_MS   1000

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

typedef struct {
    uint8_t *jpeg;
    size_t jpeg_len;
    int64_t created_ms;
    int encode_ms;
    int src_w;
    int src_h;
    uint16_t class_id;
    uint8_t confidence_pct;
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
    int last_encode_ms;
    int last_send_ms;
    size_t last_jpeg_bytes;
} image_stats_t;

static EventGroupHandle_t s_eth_events;
static TaskHandle_t s_control_task;
static TaskHandle_t s_image_prod_task;
static TaskHandle_t s_image_send_task;
static SemaphoreHandle_t s_image_lock;
static uint32_t s_tx_seq;
static volatile bool s_control_send_failed;

static uint8_t *s_snapshot_rgb;
static jpeg_enc_handle_t s_jpeg_enc;
static image_slot_t s_image_slots[IMAGE_QUEUE_DEPTH];
static image_stats_t s_image_stats;

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

static void queue_clear_locked(void)
{
    for (int i = 0; i < IMAGE_QUEUE_DEPTH; i++) {
        s_image_slots[i].ready = false;
        s_image_slots[i].sending = false;
        s_image_slots[i].jpeg_len = 0;
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

    char json[768];
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
                       "\"last_image_encode_ms\":%d,\"last_image_send_ms\":%d,"
                       "\"last_image_bytes\":%u}",
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
                       stats.last_encode_ms,
                       stats.last_send_ms,
                       (unsigned)stats.last_jpeg_bytes);
    if (len <= 0 || len >= (int)sizeof(json)) {
        return -1;
    }
    return send_packet(sock, ESP_HOST_TYPE_METRICS, json, (uint32_t)len, 0, 0, 0, 0, 0, false);
}

static esp_err_t ensure_image_resources(void)
{
    if (!s_snapshot_rgb) {
        s_snapshot_rgb = (uint8_t *)heap_caps_aligned_alloc(
            SNAPSHOT_CACHE_ALIGNMENT, SNAPSHOT_RGB_BYTES, MALLOC_CAP_SPIRAM | MALLOC_CAP_DMA | MALLOC_CAP_8BIT);
        ESP_RETURN_ON_FALSE(s_snapshot_rgb != NULL, ESP_ERR_NO_MEM, TAG, "allocate snapshot buffer failed");
    }
    for (int i = 0; i < IMAGE_QUEUE_DEPTH; i++) {
        if (!s_image_slots[i].jpeg) {
            s_image_slots[i].jpeg = (uint8_t *)heap_caps_aligned_alloc(
                16, JPEG_OUTBUF_BYTES, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
            ESP_RETURN_ON_FALSE(s_image_slots[i].jpeg != NULL, ESP_ERR_NO_MEM, TAG, "allocate JPEG buffer failed");
        }
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

static esp_err_t produce_jpeg_snapshot(image_slot_t *slot)
{
    int src_w = 0;
    int src_h = 0;
    size_t rgb_len = 0;
    int64_t frame_ts = 0;
    int64_t start_ms = monotonic_ms();

    esp_err_t ret = vision_copy_latest_frame_scaled_rgb888(s_snapshot_rgb,
                                                           SNAPSHOT_WIDTH,
                                                           SNAPSHOT_HEIGHT,
                                                           SNAPSHOT_RGB_BYTES,
                                                           &src_w,
                                                           &src_h,
                                                           &rgb_len,
                                                           &frame_ts);
    if (ret != ESP_OK) {
        (void)frame_ts;
        return ret;
    }

    int encoded = 0;
    jpeg_enc_set_quality(s_jpeg_enc, JPEG_QUALITY);
    jpeg_error_t err = jpeg_enc_process(s_jpeg_enc,
                                        s_snapshot_rgb,
                                        (int)rgb_len,
                                        slot->jpeg,
                                        JPEG_OUTBUF_BYTES,
                                        &encoded);
    if (err != JPEG_ERR_OK || encoded <= 0) {
        ESP_LOGW(TAG, "jpeg encode failed err=%d encoded=%d", (int)err, encoded);
        return ESP_FAIL;
    }

    slot->jpeg_len = (size_t)encoded;
    slot->created_ms = monotonic_ms();
    slot->encode_ms = (int)(slot->created_ms - start_ms);
    slot->src_w = src_w;
    slot->src_h = src_h;
    vision_classification_t cls = {
        .class_id = 1,
        .confidence_pct = 0,
        .valid = false,
    };
    if (vision_get_latest_classification(&cls)) {
        if (cls.class_id < 1 || cls.class_id > 3) {
            cls.class_id = 1;
        }
        if (cls.confidence_pct < 0) {
            cls.confidence_pct = 0;
        } else if (cls.confidence_pct > 100) {
            cls.confidence_pct = 100;
        }
    }
    slot->class_id = (uint16_t)cls.class_id;
    slot->confidence_pct = (uint8_t)cls.confidence_pct;
    return ESP_OK;
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

static void process_rx_packet(int sock, const esp_host_packet_header_t *header, const uint8_t *payload)
{
    if (header->type == ESP_HOST_TYPE_TIME_SYNC) {
        apply_time_sync((const char *)payload, header->payload_len);
    } else if (header->type == ESP_HOST_TYPE_SIM_LINE) {
        sorting_sim_control_handle_line((const char *)payload, header->payload_len,
                                        send_sim_line_packet, (void *)(intptr_t)sock);
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
            if (now_ms >= next_metrics_ms) {
                if (send_metrics_packet(sock) != 0) {
                    ESP_LOGW(TAG, "TCP metrics send failed errno=%d", errno);
                    break;
                }
                next_metrics_ms = now_ms + TCP_METRICS_INTERVAL_MS;
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
    int64_t next_image_ms = monotonic_ms() + TCP_IMAGE_INTERVAL_MS;

    while (true) {
        xEventGroupWaitBits(s_eth_events, TCP_CONNECT_READY_BIT, pdFALSE, pdTRUE, portMAX_DELAY);
        int64_t now_ms = monotonic_ms();
        if (now_ms < next_image_ms) {
            vTaskDelay(pdMS_TO_TICKS(100));
            continue;
        }
        next_image_ms = now_ms + TCP_IMAGE_INTERVAL_MS;

        EventBits_t bits = xEventGroupGetBits(s_eth_events);
        if ((bits & TCP_IMAGE_CONNECTED_BIT) == 0) {
            continue;
        }
        if (ensure_image_resources() != ESP_OK) {
            if (xSemaphoreTake(s_image_lock, pdMS_TO_TICKS(50)) == pdTRUE) {
                s_image_stats.encode_fail++;
                xSemaphoreGive(s_image_lock);
            }
            vTaskDelay(pdMS_TO_TICKS(1000));
            continue;
        }

        image_slot_t *slot = NULL;
        if (xSemaphoreTake(s_image_lock, portMAX_DELAY) == pdTRUE) {
            queue_drop_stale_locked(now_ms);
            slot = queue_find_free_locked();
            if (!slot) {
                s_image_stats.dropped_backpressure++;
            }
            xSemaphoreGive(s_image_lock);
        }
        if (!slot) {
            continue;
        }

        esp_err_t ret = produce_jpeg_snapshot(slot);
        if (xSemaphoreTake(s_image_lock, portMAX_DELAY) == pdTRUE) {
            EventBits_t current_bits = xEventGroupGetBits(s_eth_events);
            if (ret == ESP_OK && (current_bits & TCP_IMAGE_CONNECTED_BIT) != 0) {
                slot->ready = true;
                slot->sending = false;
                s_image_stats.encoded++;
                s_image_stats.last_encode_ms = slot->encode_ms;
                s_image_stats.last_jpeg_bytes = slot->jpeg_len;
                ESP_LOGD(TAG, "queued JPEG %ux%u from %dx%d payload=%u encode=%dms depth=%d",
                         SNAPSHOT_WIDTH, SNAPSHOT_HEIGHT, slot->src_w, slot->src_h,
                         (unsigned)slot->jpeg_len, slot->encode_ms, queue_depth_locked());
            } else {
                slot->ready = false;
                slot->sending = false;
                slot->jpeg_len = 0;
                if (ret == ESP_OK) {
                    s_image_stats.dropped_backpressure++;
                } else if (ret == ESP_ERR_NOT_FOUND) {
                    s_image_stats.no_frame++;
                } else {
                    s_image_stats.encode_fail++;
                }
                ESP_LOGW(TAG, "JPEG snapshot skipped: %s", esp_err_to_name(ret));
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
        xEventGroupSetBits(s_eth_events, TCP_IMAGE_CONNECTED_BIT);

        while (true) {
            int64_t now_ms = monotonic_ms();
            image_slot_t *slot = NULL;
            if (xSemaphoreTake(s_image_lock, portMAX_DELAY) == pdTRUE) {
                queue_drop_stale_locked(now_ms);
                slot = queue_pop_ready_locked();
                xSemaphoreGive(s_image_lock);
            }

            if (!slot) {
                vTaskDelay(pdMS_TO_TICKS(50));
                continue;
            }

            int64_t send_start_ms = monotonic_ms();
            uint32_t image_meta = slot->confidence_pct;
            int ret = send_packet(sock, ESP_HOST_TYPE_IMAGE, slot->jpeg, (uint32_t)slot->jpeg_len,
                                  SNAPSHOT_WIDTH, SNAPSHOT_HEIGHT, ESP_HOST_PIXEL_JPEG,
                                  slot->class_id, image_meta, true);
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
            if (ret != 0) {
                ESP_LOGW(TAG, "TCP image send failed errno=%d", errno);
                break;
            }
            ESP_LOGD(TAG, "sent JPEG %ux%u send=%dms", SNAPSHOT_WIDTH, SNAPSHOT_HEIGHT, send_ms);
        }

        xEventGroupClearBits(s_eth_events, TCP_IMAGE_CONNECTED_BIT);
        if (xSemaphoreTake(s_image_lock, pdMS_TO_TICKS(100)) == pdTRUE) {
            queue_clear_locked();
            xSemaphoreGive(s_image_lock);
        }
        close(sock);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

static void start_tcp_tasks(void)
{
    if (!s_image_lock) {
        s_image_lock = xSemaphoreCreateMutex();
        if (!s_image_lock) {
            ESP_LOGE(TAG, "create image lock failed");
            return;
        }
    }

    if (!s_control_task) {
        BaseType_t ok = xTaskCreatePinnedToCoreWithCaps(
            control_tcp_task, "eth_control", CONTROL_TASK_STACK_BYTES,
            NULL, CONTROL_TASK_PRIORITY, &s_control_task, 0, TCP_TASK_STACK_CAPS);
        if (ok != pdPASS) {
            ESP_LOGE(TAG, "create TCP control task failed, free_internal=%u free_psram=%u",
                     (unsigned)heap_caps_get_free_size(MALLOC_CAP_INTERNAL),
                     (unsigned)heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
        }
    }
    if (!s_image_prod_task) {
        BaseType_t ok = xTaskCreatePinnedToCoreWithCaps(
            image_producer_task, "eth_img_prod", IMAGE_PROD_TASK_STACK,
            NULL, IMAGE_PROD_TASK_PRIORITY, &s_image_prod_task, 0, TCP_TASK_STACK_CAPS);
        if (ok != pdPASS) {
            ESP_LOGE(TAG, "create TCP image producer task failed, free_internal=%u free_psram=%u",
                     (unsigned)heap_caps_get_free_size(MALLOC_CAP_INTERNAL),
                     (unsigned)heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
        }
    }
    if (!s_image_send_task) {
        BaseType_t ok = xTaskCreatePinnedToCoreWithCaps(
            image_sender_task, "eth_img_send", IMAGE_SEND_TASK_STACK,
            NULL, IMAGE_SEND_TASK_PRIORITY, &s_image_send_task, 0, TCP_TASK_STACK_CAPS);
        if (ok != pdPASS) {
            ESP_LOGE(TAG, "create TCP image sender task failed, free_internal=%u free_psram=%u",
                     (unsigned)heap_caps_get_free_size(MALLOC_CAP_INTERNAL),
                     (unsigned)heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
        }
    }
}

static esp_err_t configure_static_ip(esp_netif_t *netif)
{
    esp_netif_ip_info_t ip_info = {0};

    ESP_RETURN_ON_ERROR(esp_netif_dhcpc_stop(netif), TAG, "stop Ethernet DHCP client failed");
    ESP_RETURN_ON_FALSE(esp_netif_str_to_ip4("192.168.10.2", &ip_info.ip) == ESP_OK, ESP_FAIL, TAG, "invalid static IP");
    ESP_RETURN_ON_FALSE(esp_netif_str_to_ip4("255.255.255.0", &ip_info.netmask) == ESP_OK, ESP_FAIL, TAG, "invalid static netmask");
    ESP_RETURN_ON_FALSE(esp_netif_str_to_ip4("192.168.10.1", &ip_info.gw) == ESP_OK, ESP_FAIL, TAG, "invalid static gateway");
    ESP_RETURN_ON_ERROR(esp_netif_set_ip_info(netif, &ip_info), TAG, "set Ethernet static IP failed");

    return ESP_OK;
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
        xEventGroupClearBits(s_eth_events, TCP_CONNECT_READY_BIT | TCP_IMAGE_CONNECTED_BIT);
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
}

esp_err_t ethernet_app_start(void)
{
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
    start_tcp_tasks();

    return ESP_OK;
}
