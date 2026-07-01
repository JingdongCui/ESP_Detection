#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "bsp_cam_sensor.h"
#include "esp_check.h"
#include "esp_eth.h"
#include "esp_event.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_netif_ip_addr.h"
#include "esp_timer.h"
#include "ethernet_app.h"
#include "ethernet_init.h"
#include "lwip/inet.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

static const char *TAG = "eth_example";

#define HOST_IP                 "192.168.10.1"
#define HOST_PORT               5000
#define TCP_TASK_STACK_BYTES    8192
#define TCP_TASK_PRIORITY       4
#define TCP_SEND_INTERVAL_MS    10000
#define TCP_CONNECT_READY_BIT   BIT0

#define ESP_HOST_MAGIC          0x32505345u
#define ESP_HOST_VERSION        1u
#define ESP_HOST_HEADER_SIZE    40u
#define ESP_HOST_TYPE_IMAGE     0x01u
#define ESP_HOST_TYPE_METRICS   0x02u
#define ESP_HOST_TYPE_TIME_SYNC 0x10u
#define ESP_HOST_PIXEL_RGB888   1u

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
static uint8_t *s_image_copy_buf;
static size_t s_image_copy_capacity;

static uint64_t unix_time_ms(void)
{
    struct timeval tv = {0};
    gettimeofday(&tv, NULL);
    return (uint64_t)tv.tv_sec * 1000ULL + (uint64_t)(tv.tv_usec / 1000);
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

static int ensure_image_copy_buffer(size_t size)
{
    if (s_image_copy_capacity >= size && s_image_copy_buf) {
        return 0;
    }
    free(s_image_copy_buf);
    s_image_copy_buf = (uint8_t *)heap_caps_malloc(size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    if (!s_image_copy_buf) {
        s_image_copy_capacity = 0;
        ESP_LOGE(TAG, "no PSRAM for image copy: %u bytes", (unsigned)size);
        return -1;
    }
    s_image_copy_capacity = size;
    return 0;
}

static int send_image_packet(int sock)
{
    uint8_t *frame = NULL;
    size_t frame_size = 0;
    int frame_w = 0;
    int frame_h = 0;

    esp_err_t ret = cam_sensor_get_frame(&frame, &frame_size, &frame_w, &frame_h, 2000);
    if (ret != ESP_OK) {
        ESP_LOGW(TAG, "get camera frame for TCP failed: %s", esp_err_to_name(ret));
        return 0;
    }

    if (ensure_image_copy_buffer(frame_size) == 0) {
        memcpy(s_image_copy_buf, frame, frame_size);
    } else {
        ret = ESP_ERR_NO_MEM;
    }
    cam_sensor_return_frame(frame);

    if (ret != ESP_OK) {
        return -1;
    }

    ESP_LOGI(TAG, "send image %dx%d %u bytes", frame_w, frame_h, (unsigned)frame_size);
    return send_packet(sock, ESP_HOST_TYPE_IMAGE, s_image_copy_buf, (uint32_t)frame_size,
                       (uint16_t)frame_w, (uint16_t)frame_h, ESP_HOST_PIXEL_RGB888);
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

static void process_rx_packet(const esp_host_packet_header_t *header, const uint8_t *payload)
{
    if (header->type == ESP_HOST_TYPE_TIME_SYNC) {
        apply_time_sync((const char *)payload, header->payload_len);
    }
}

static void poll_incoming_packets(int sock)
{
    static uint8_t rx_buf[1024];
    static size_t rx_len;

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
            break;
        }
        break;
    }

    while (rx_len >= sizeof(esp_host_packet_header_t)) {
        esp_host_packet_header_t header;
        memcpy(&header, rx_buf, sizeof(header));
        if (header.magic != ESP_HOST_MAGIC || header.version != ESP_HOST_VERSION ||
            header.header_size != ESP_HOST_HEADER_SIZE || header.payload_len > 512) {
            memmove(rx_buf, rx_buf + 1, --rx_len);
            continue;
        }
        size_t packet_len = sizeof(header) + header.payload_len;
        if (rx_len < packet_len) {
            return;
        }
        process_rx_packet(&header, rx_buf + sizeof(header));
        memmove(rx_buf, rx_buf + packet_len, rx_len - packet_len);
        rx_len -= packet_len;
    }
}

static int connect_to_host(void)
{
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (sock < 0) {
        ESP_LOGE(TAG, "socket failed errno=%d", errno);
        return -1;
    }

    struct sockaddr_in dest = {
        .sin_family = AF_INET,
        .sin_port = htons(HOST_PORT),
    };
    inet_pton(AF_INET, HOST_IP, &dest.sin_addr);

    int flags = fcntl(sock, F_GETFL, 0);
    if (flags >= 0) {
        fcntl(sock, F_SETFL, flags | O_NONBLOCK);
    }

    ESP_LOGI(TAG, "connecting to host %s:%d", HOST_IP, HOST_PORT);
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

    if (flags >= 0) {
        fcntl(sock, F_SETFL, flags);
    }

    ESP_LOGI(TAG, "connected to host");
    return sock;
}

static void tcp_client_task(void *arg)
{
    (void)arg;
    while (true) {
        xEventGroupWaitBits(s_eth_events, TCP_CONNECT_READY_BIT, pdFALSE, pdTRUE, portMAX_DELAY);

        int sock = connect_to_host();
        if (sock < 0) {
            vTaskDelay(pdMS_TO_TICKS(2000));
            continue;
        }

        int64_t next_send_ms = esp_timer_get_time() / 1000 + 1000;
        while (true) {
            poll_incoming_packets(sock);

            int64_t now_ms = esp_timer_get_time() / 1000;
            if (now_ms >= next_send_ms) {
                if (send_metrics_packet(sock) != 0 || send_image_packet(sock) != 0) {
                    ESP_LOGW(TAG, "TCP send failed errno=%d", errno);
                    break;
                }
                next_send_ms = now_ms + TCP_SEND_INTERVAL_MS;
            }
            vTaskDelay(pdMS_TO_TICKS(100));
        }

        close(sock);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

static void start_tcp_client_task(void)
{
    if (s_tcp_task) {
        return;
    }
    BaseType_t ok = xTaskCreatePinnedToCore(
        tcp_client_task,
        "eth_tcp",
        TCP_TASK_STACK_BYTES,
        NULL,
        TCP_TASK_PRIORITY,
        &s_tcp_task,
        0);
    if (ok != pdPASS) {
        ESP_LOGE(TAG, "create TCP client task failed");
    }
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

    /* 创建 ESP32-P4 EMAC + IP101 PHY 的底层 Ethernet driver。 */
    ESP_RETURN_ON_ERROR(example_eth_init(&eth_handles, &eth_port_cnt), TAG, "Ethernet init failed");
    ESP_RETURN_ON_FALSE(eth_port_cnt == 1, ESP_FAIL, TAG, "unexpected Ethernet port count: %u", eth_port_cnt);

    ESP_RETURN_ON_ERROR(esp_netif_init(), TAG, "esp-netif init failed");
    ESP_RETURN_ON_ERROR(esp_event_loop_create_default(), TAG, "default event loop init failed");

    /* 创建 TCP/IP 协议栈中的 Ethernet netif，后续 IP、ARP、ICMP 都走这个接口。 */
    esp_netif_config_t cfg = ESP_NETIF_DEFAULT_ETH();
    esp_netif_t *eth_netif = esp_netif_new(&cfg);
    ESP_RETURN_ON_FALSE(eth_netif != NULL, ESP_ERR_NO_MEM, TAG, "create Ethernet netif failed");

    /* glue 负责把底层 Ethernet driver 收发的数据接到 esp-netif/LwIP 协议栈。 */
    esp_eth_netif_glue_handle_t eth_netif_glue = esp_eth_new_netif_glue(eth_handles[0]);
    ESP_RETURN_ON_FALSE(eth_netif_glue != NULL, ESP_ERR_NO_MEM, TAG, "create Ethernet netif glue failed");
    ESP_RETURN_ON_ERROR(esp_netif_attach(eth_netif, eth_netif_glue), TAG, "attach Ethernet netif failed");

    /* attach 完成后再配置静态 IP，确保 IP 信息写入当前 Ethernet netif。 */
    ESP_RETURN_ON_ERROR(configure_static_ip(eth_netif), TAG, "configure static IP failed");

    ESP_RETURN_ON_ERROR(esp_event_handler_register(ETH_EVENT, ESP_EVENT_ANY_ID, &eth_event_handler, NULL), TAG, "register Ethernet event handler failed");
    ESP_RETURN_ON_ERROR(esp_event_handler_register(IP_EVENT, IP_EVENT_ETH_GOT_IP, &got_ip_event_handler, NULL), TAG, "register Ethernet IP event handler failed");
    ESP_RETURN_ON_ERROR(esp_eth_start(eth_handles[0]), TAG, "start Ethernet failed");
    start_tcp_client_task();

    return ESP_OK;
}
