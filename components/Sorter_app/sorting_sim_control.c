#include "sorting_sim_control.h"

#include "bsp_encoder.h"
#include "bsp_motor.h"
#include "bsp_sort_sensor.h"
#include "sorter_core/sorter_scheduler.h"

#include "driver/usb_serial_jtag.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *TAG = "sorting_sim";
static sorter_config_t s_config;
static sorter_scheduler_t s_scheduler;
static bool s_initialized;
static TaskHandle_t s_debug_task;
static int s_next_package_id = 1;
static bool s_vision_s1_active;
static int s_vision_package_id;
static bool s_vision_classified;
static bool s_vision_failed_seen;
static bool s_real_io_enabled;
static bool s_real_io_task_started;
static bool s_motor_ready;
static bool s_encoder_ready;
static bool s_sensor_ready;
static TaskHandle_t s_real_io_task;
static SemaphoreHandle_t s_control_lock;
static sorting_sim_send_fn_t s_downstream_send_fn;
static void *s_downstream_send_ctx;

#define REAL_IO_POLL_MS 10
#define SENSOR_DEBOUNCE_MS 20

typedef struct {
    bool raw;
    bool stable;
    int64_t raw_changed_ms;
    bool initialized;
} real_sensor_state_t;

static real_sensor_state_t s_real_sensors[5];
static float s_real_motor3_distance_mm;
static int64_t s_last_encoder_sample_ms;

static void ensure_initialized(void)
{
    if (!s_initialized) {
        sorter_config_default(&s_config);
        sorter_scheduler_init(&s_scheduler, &s_config);
        s_initialized = true;
    }
}

static void send_line(sorting_sim_send_fn_t fn, void *ctx, const char *line)
{
    if (fn) fn(ctx, line);
    ESP_LOGI(TAG, "OUT %s", line);
}

static int parse_int_field(const char *line, const char *key, int fallback)
{
    const char *p = strstr(line, key);
    if (!p) return fallback;
    return (int)strtol(p + strlen(key), NULL, 10);
}

static float parse_float_field(const char *line, const char *key, float fallback)
{
    const char *p = strstr(line, key);
    return p ? strtof(p + strlen(key), NULL) : fallback;
}

static int clamp_percent(int value) { return value < 0 ? 0 : value > 100 ? 100 : value; }
static uint32_t clamp_ms(int value, uint32_t fallback) { return value < 0 ? fallback : value > 30000 ? 30000U : (uint32_t)value; }

static void ensure_control_lock(void) { if (!s_control_lock) s_control_lock = xSemaphoreCreateMutex(); }
static void lock_control(void) { ensure_control_lock(); if (s_control_lock) xSemaphoreTake(s_control_lock, portMAX_DELAY); }
static void unlock_control(void) { if (s_control_lock) xSemaphoreGive(s_control_lock); }

static void reset_vision_window(void)
{
    s_vision_s1_active = false;
    s_vision_package_id = 0;
    s_vision_classified = false;
    s_vision_failed_seen = false;
}

static void reset_real_io_state(void)
{
    memset(s_real_sensors, 0, sizeof(s_real_sensors));
    s_real_motor3_distance_mm = 0;
    s_last_encoder_sample_ms = esp_timer_get_time() / 1000;
}

static void apply_motor_command_line(const char *line)
{
    if (!s_real_io_enabled || !s_motor_ready || strncmp(line, "MOTOR", 5) != 0) return;
    int motor_id = parse_int_field(line, "id=", 0);
    int speed = clamp_percent(parse_int_field(line, "speed=", 0));
    if (motor_id < 1 || motor_id > 3) return;
    uint8_t direction = 0;
    uint32_t out_speed = (uint32_t)speed;
    if (strstr(line, "dir=reverse")) direction = 1;
    else if (!strstr(line, "dir=forward")) out_speed = 0;
    esp_err_t ret = bsp_motor_set_speed_direction((uint8_t)(motor_id - 1), out_speed, direction);
    if (ret != ESP_OK) {
        ESP_LOGW(TAG, "apply motor %d command failed: %s", motor_id, esp_err_to_name(ret));
        s_motor_ready = false;
    }
}

static int scheduler_send(void *ctx, const char *line)
{
    (void)ctx;
    apply_motor_command_line(line);
    return s_downstream_send_fn ? s_downstream_send_fn(s_downstream_send_ctx, line) : 0;
}

static void set_scheduler_sender_locked(sorting_sim_send_fn_t fn, void *ctx)
{
    s_downstream_send_fn = fn;
    s_downstream_send_ctx = ctx;
    sorter_scheduler_set_sender(&s_scheduler, scheduler_send, NULL);
}

static void reset_control_locked(void)
{
    sorter_scheduler_reset(&s_scheduler);
    s_next_package_id = 1;
    reset_vision_window();
    reset_real_io_state();
}

static void real_io_task(void *arg)
{
    (void)arg;
    const bsp_sort_sensor_id_t sensor_ids[] = { BSP_SORT_SENSOR_S2, BSP_SORT_SENSOR_S3, BSP_SORT_SENSOR_S4 };
    while (true) {
        if (!s_real_io_enabled) { vTaskDelay(pdMS_TO_TICKS(100)); continue; }
        int64_t now = esp_timer_get_time() / 1000;
        if (s_sensor_ready) {
            for (size_t i = 0; i < sizeof(sensor_ids) / sizeof(sensor_ids[0]); ++i) {
                bsp_sort_sensor_id_t sensor_id = sensor_ids[i];
                bool active = false;
                esp_err_t ret = bsp_sort_sensor_get_state(sensor_id, &active);
                if (ret != ESP_OK && ret != ESP_ERR_NOT_FOUND) {
                    ESP_LOGW(TAG, "read sort sensor S%u failed: %s", (unsigned)sensor_id, esp_err_to_name(ret));
                    continue;
                }
                real_sensor_state_t *state = &s_real_sensors[(size_t)sensor_id];
                if (!state->initialized) {
                    state->raw = active; state->stable = active; state->raw_changed_ms = now; state->initialized = true; continue;
                }
                if (state->raw != active) { state->raw = active; state->raw_changed_ms = now; continue; }
                if (state->stable != state->raw && now - state->raw_changed_ms >= SENSOR_DEBOUNCE_MS) {
                    state->stable = state->raw;
                    lock_control();
                    sorter_scheduler_sensor(&s_scheduler, (int)sensor_id, state->stable, 0);
                    unlock_control();
                }
            }
        }
        if (s_encoder_ready) {
            float speed_cm_s = 0;
            esp_err_t ret = bsp_encoder_get_speed_cm_s(BSP_ENCODER_0, &speed_cm_s);
            if (ret == ESP_OK) {
                if (s_last_encoder_sample_ms == 0) s_last_encoder_sample_ms = now;
                float dt_s = (float)(now - s_last_encoder_sample_ms) / 1000.0f;
                s_last_encoder_sample_ms = now;
                if (dt_s > 0 && dt_s < 1) {
                    s_real_motor3_distance_mm += fabsf(speed_cm_s) * 10.0f * dt_s;
                    lock_control();
                    sorter_scheduler_distance(&s_scheduler, 3, s_real_motor3_distance_mm);
                    unlock_control();
                }
            } else {
                ESP_LOGW(TAG, "read encoder 0 speed failed: %s", esp_err_to_name(ret));
                s_encoder_ready = false;
            }
        }
        vTaskDelay(pdMS_TO_TICKS(REAL_IO_POLL_MS));
    }
}

static void start_real_io_task_locked(void)
{
    if (s_real_io_task_started) return;
    BaseType_t ok = xTaskCreatePinnedToCore(real_io_task, "sort_real_io", 4096, NULL, 4, &s_real_io_task, 0);
    if (ok == pdPASS) s_real_io_task_started = true;
    else ESP_LOGE(TAG, "create real IO task failed");
}

static void set_real_io_enabled_locked(bool enabled)
{
    if (enabled) {
        esp_err_t ret = bsp_motor_init();
        s_motor_ready = ret == ESP_OK;
        if (!s_motor_ready) ESP_LOGW(TAG, "real IO motor output disabled: %s", esp_err_to_name(ret));
        ret = bsp_encoder_init();
        s_encoder_ready = ret == ESP_OK;
        if (!s_encoder_ready) ESP_LOGW(TAG, "real IO encoder distance disabled: %s", esp_err_to_name(ret));
        ret = bsp_sort_sensor_init();
        s_sensor_ready = ret == ESP_OK;
        if (!s_sensor_ready) ESP_LOGW(TAG, "real IO sort sensors disabled: %s", esp_err_to_name(ret));
        reset_real_io_state();
        start_real_io_task_locked();
    }
    s_real_io_enabled = enabled;
    ESP_LOGI(TAG, "real IO %s", enabled ? "enabled" : "disabled");
}

static sorter_package_class_t parse_vision_class(const char *line)
{
    bool free_flag = strstr(line, "free=1") != NULL;
    bool error_flag = strstr(line, "class=error") != NULL;
    if (strstr(line, "class=none")) return SORTER_CLASS_UNKNOWN;
    return sorter_package_class_from_int(parse_int_field(line, "class=", 0), free_flag, error_flag);
}

static bool is_success_class(sorter_package_class_t cls)
{
    return cls == SORTER_CLASS_1 || cls == SORTER_CLASS_2 || cls == SORTER_CLASS_3 || cls == SORTER_CLASS_FREE;
}

static void fail_current_vision_window(void)
{
    if (s_vision_package_id <= 0 || s_vision_classified) return;
    (void)s_vision_failed_seen;
    sorter_scheduler_vision_result(&s_scheduler, s_vision_package_id, SORTER_CLASS_VISION_FAILED);
    s_vision_classified = true;
}

void sorting_sim_control_reset(void)
{
    ensure_initialized();
    lock_control();
    reset_control_locked();
    unlock_control();
}

void sorting_sim_control_handle_line(const char *line, size_t len, sorting_sim_send_fn_t send_fn, void *send_ctx)
{
    ensure_initialized();
    char buf[192];
    size_t copy_len = len < sizeof(buf) - 1 ? len : sizeof(buf) - 1;
    memcpy(buf, line, copy_len); buf[copy_len] = '\0';
    char *newline = strpbrk(buf, "\r\n");
    if (newline) *newline = '\0';
    if (!buf[0]) return;
    ESP_LOGI(TAG, "IN %s", buf);
    lock_control();
    set_scheduler_sender_locked(send_fn, send_ctx);

#define RETURN_UNLOCK() do { unlock_control(); return; } while (0)
    if (strncmp(buf, "HELP", 4) == 0) {
        send_line(send_fn, send_ctx, "HELP,VISION_FRAME s1/free/class | CONFIG a_speed/b_speed/c_speed/handoff_delay_ms/lost_timeout_min_ms/lost_timeout_max_ms/real_io | PACKAGE_NEW id | VISION_RESULT id/free/class | SENSOR id/state | DISTANCE motor/dist | RESET | ESTOP");
        RETURN_UNLOCK();
    }
    if (strncmp(buf, "CONFIG", 6) == 0) {
        bool real_io_field_present = strstr(buf, "real_io=") != NULL;
        bool next_real_io = parse_int_field(buf, "real_io=", s_real_io_enabled ? 1 : 0) != 0;
        s_config.motor_a_speed_percent = clamp_percent(parse_int_field(buf, "a_speed=", s_config.motor_a_speed_percent));
        s_config.motor_b_speed_percent = clamp_percent(parse_int_field(buf, "b_speed=", s_config.motor_b_speed_percent));
        s_config.motor_c_speed_percent = clamp_percent(parse_int_field(buf, "c_speed=", s_config.motor_c_speed_percent));
        s_config.handoff_delay_ms = clamp_ms(parse_int_field(buf, "handoff_delay_ms=", (int)s_config.handoff_delay_ms), s_config.handoff_delay_ms);
        s_config.lost_timeout_min_ms = clamp_ms(parse_int_field(buf, "lost_timeout_min_ms=", (int)s_config.lost_timeout_min_ms), s_config.lost_timeout_min_ms);
        s_config.lost_timeout_max_ms = clamp_ms(parse_int_field(buf, "lost_timeout_max_ms=", (int)s_config.lost_timeout_max_ms), s_config.lost_timeout_max_ms);
        sorter_scheduler_configure(&s_scheduler, &s_config);
        if (next_real_io != s_real_io_enabled) set_real_io_enabled_locked(next_real_io);
        char out[240];
        if (real_io_field_present) {
            snprintf(out, sizeof(out), "STATUS,packages=%u,state=running,reason=config,a_speed=%d,b_speed=%d,c_speed=%d,handoff_delay_ms=%u,lost_timeout_min_ms=%u,lost_timeout_max_ms=%u,real_io=%d",
                     (unsigned)sorter_scheduler_active_count(&s_scheduler), s_config.motor_a_speed_percent,
                     s_config.motor_b_speed_percent, s_config.motor_c_speed_percent, (unsigned)s_config.handoff_delay_ms,
                     (unsigned)s_config.lost_timeout_min_ms, (unsigned)s_config.lost_timeout_max_ms, s_real_io_enabled ? 1 : 0);
        } else {
            snprintf(out, sizeof(out), "STATUS,packages=%u,state=running,reason=config,a_speed=%d,b_speed=%d,c_speed=%d,handoff_delay_ms=%u,lost_timeout_min_ms=%u,lost_timeout_max_ms=%u",
                     (unsigned)sorter_scheduler_active_count(&s_scheduler), s_config.motor_a_speed_percent,
                     s_config.motor_b_speed_percent, s_config.motor_c_speed_percent, (unsigned)s_config.handoff_delay_ms,
                     (unsigned)s_config.lost_timeout_min_ms, (unsigned)s_config.lost_timeout_max_ms);
        }
        send_line(send_fn, send_ctx, out);
        RETURN_UNLOCK();
    }
    if (strncmp(buf, "RESET", 5) == 0) {
        reset_control_locked(); set_scheduler_sender_locked(send_fn, send_ctx); sorter_scheduler_tick(&s_scheduler); RETURN_UNLOCK();
    }
    if (strncmp(buf, "ESTOP", 5) == 0) {
        sorter_scheduler_estop(&s_scheduler, parse_int_field(buf, "state=", 0) != 0); RETURN_UNLOCK();
    }
    if (strncmp(buf, "PACKAGE_NEW", 11) == 0) {
        int id = parse_int_field(buf, "id=", 0);
        if (id <= 0) id = s_next_package_id++;
        else if (id >= s_next_package_id) s_next_package_id = id + 1;
        sorter_scheduler_package_new(&s_scheduler, id); RETURN_UNLOCK();
    }
    if (strncmp(buf, "PACKAGE_LOST", 12) == 0) {
        send_line(send_fn, send_ctx, "STATUS,packages=0,state=running,reason=legacy_package_lost_rejected"); RETURN_UNLOCK();
    }
    if (strncmp(buf, "VISION_RESULT", 13) == 0) {
        sorter_scheduler_vision_result(&s_scheduler, parse_int_field(buf, "id=", 0), parse_vision_class(buf)); RETURN_UNLOCK();
    }
    if (strncmp(buf, "VISION_FRAME", 12) == 0) {
        bool s1_active = parse_int_field(buf, "s1=", 0) != 0;
        sorter_package_class_t cls = parse_vision_class(buf);
        if (s1_active && !s_vision_s1_active) {
            fail_current_vision_window(); s_vision_package_id = s_next_package_id++;
            s_vision_classified = false; s_vision_failed_seen = false;
            sorter_scheduler_package_new(&s_scheduler, s_vision_package_id);
        }
        if (s_vision_package_id > 0 && !s_vision_classified) {
            if (is_success_class(cls)) {
                sorter_scheduler_vision_result(&s_scheduler, s_vision_package_id, cls); s_vision_classified = true;
            } else if (cls == SORTER_CLASS_ERROR) {
                s_vision_failed_seen = true; fail_current_vision_window();
            }
        }
        s_vision_s1_active = s1_active; RETURN_UNLOCK();
    }
    if (strncmp(buf, "SENSOR", 6) == 0) {
        if (s_real_io_enabled) { ESP_LOGI(TAG, "ignore simulated SENSOR while real_io=1"); RETURN_UNLOCK(); }
        int sensor_id = parse_int_field(buf, "id=", 0);
        bool active = parse_int_field(buf, "state=", 0) != 0;
        int package_id = parse_int_field(buf, "package=", 0);
        bool close_current_at_s2 = false;
        if (sensor_id == 2 && active && s_vision_package_id > 0 &&
            !sorter_scheduler_has_s2_candidate_before(&s_scheduler, s_vision_package_id)) {
            fail_current_vision_window(); close_current_at_s2 = true;
        }
        sorter_scheduler_sensor(&s_scheduler, sensor_id, active, package_id);
        if (close_current_at_s2) reset_vision_window();
        RETURN_UNLOCK();
    }
    if (strncmp(buf, "DISTANCE", 8) == 0) {
        if (s_real_io_enabled) { ESP_LOGI(TAG, "ignore simulated DISTANCE while real_io=1"); RETURN_UNLOCK(); }
        sorter_scheduler_distance(&s_scheduler, parse_int_field(buf, "motor=", 0), parse_float_field(buf, "dist=", 0));
        RETURN_UNLOCK();
    }
    send_line(send_fn, send_ctx, "STATUS,packages=0,state=running,reason=unknown_command");
    RETURN_UNLOCK();
#undef RETURN_UNLOCK
}

void sorting_sim_control_tick(sorting_sim_send_fn_t send_fn, void *send_ctx)
{
    ensure_initialized();
    lock_control();
    set_scheduler_sender_locked(send_fn, send_ctx);
    sorter_scheduler_tick(&s_scheduler);
    unlock_control();
}

static int serial_send(void *ctx, const char *line)
{
    (void)ctx;
    printf("SIMOUT %s\n", line); fflush(stdout); return 0;
}

static void run_debug_script(const char *name, int cls)
{
    char line[96];
    printf("SIMTEST %s start\n", name);
    sorting_sim_control_handle_line("RESET", strlen("RESET"), serial_send, NULL);
    sorting_sim_control_handle_line("VISION_FRAME,s1=1,free=0,class=none", strlen("VISION_FRAME,s1=1,free=0,class=none"), serial_send, NULL);
    snprintf(line, sizeof(line), "VISION_FRAME,s1=1,free=0,class=%d", cls);
    sorting_sim_control_handle_line(line, strlen(line), serial_send, NULL);
    sorting_sim_control_handle_line("VISION_FRAME,s1=0,free=0,class=none", strlen("VISION_FRAME,s1=0,free=0,class=none"), serial_send, NULL);
    sorting_sim_control_handle_line("SENSOR,id=2,state=1", strlen("SENSOR,id=2,state=1"), serial_send, NULL);
    if (cls == 1) sorting_sim_control_handle_line("SENSOR,id=3,state=1", strlen("SENSOR,id=3,state=1"), serial_send, NULL);
    else {
        sorting_sim_control_handle_line("SENSOR,id=4,state=1", strlen("SENSOR,id=4,state=1"), serial_send, NULL);
        sorting_sim_control_handle_line("DISTANCE,motor=3,dist=0.0", strlen("DISTANCE,motor=3,dist=0.0"), serial_send, NULL);
        sorting_sim_control_handle_line("DISTANCE,motor=3,dist=370.0", strlen("DISTANCE,motor=3,dist=370.0"), serial_send, NULL);
    }
    printf("SIMTEST %s end\n", name); fflush(stdout);
}

static void debug_task(void *arg)
{
    (void)arg;
    char line[192];
    size_t len = 0;
    printf("\nSORTDBG ready. Commands: help, reset, test1, test2, test3, or raw protocol lines.\n"); fflush(stdout);
    while (true) {
        uint8_t bytes[32];
        int got = usb_serial_jtag_read_bytes(bytes, sizeof(bytes), pdMS_TO_TICKS(100));
        if (got <= 0) continue;
        for (int i = 0; i < got; ++i) {
            int ch = bytes[i];
            if (ch == '\r' || ch == '\n') {
                line[len] = '\0';
                if (len > 0) {
                    if (strcmp(line, "help") == 0) sorting_sim_control_handle_line("HELP", 4, serial_send, NULL);
                    else if (strcmp(line, "reset") == 0) sorting_sim_control_handle_line("RESET", 5, serial_send, NULL);
                    else if (strcmp(line, "test1") == 0) run_debug_script("class1", 1);
                    else if (strcmp(line, "test2") == 0) run_debug_script("class2", 2);
                    else if (strcmp(line, "test3") == 0) run_debug_script("class3", 3);
                    else sorting_sim_control_handle_line(line, len, serial_send, NULL);
                }
                len = 0;
            } else if (len < sizeof(line) - 1) {
                if (ch == 8 || ch == 127) { if (len > 0) --len; }
                else line[len++] = (char)ch;
            }
        }
    }
}

void sorting_sim_debug_start(void)
{
    if (s_debug_task) return;
    usb_serial_jtag_driver_config_t config = { .rx_buffer_size = 512, .tx_buffer_size = 512 };
    esp_err_t ret = usb_serial_jtag_driver_install(&config);
    if (ret != ESP_OK && ret != ESP_ERR_INVALID_STATE)
        ESP_LOGW(TAG, "USB Serial/JTAG debug driver install failed: %s", esp_err_to_name(ret));
    xTaskCreatePinnedToCore(debug_task, "sort_dbg", 4096, NULL, 3, &s_debug_task, 0);
}
