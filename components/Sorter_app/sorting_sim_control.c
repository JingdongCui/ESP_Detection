#include "sorting_sim_control.h"

#include "bsp_encoder.h"
#include "bsp_motor.h"
#include "bsp_sort_sensor.h"
#include "sorter_debug_config.h"
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
static sorting_debug_mode_t s_debug_mode = SORTING_DEBUG_MODE_REAL_SENSOR;
static bool s_motor_output_enabled = true;
static bool s_sensor_input_enabled = true;
static bool s_real_io_enabled;
static bool s_hardware_observer_enabled;
static bool s_hardware_observer_init_attempted;
static bool s_real_io_task_started;
static bool s_motor_ready;
static bool s_motor_init_attempted;
static bool s_encoder_ready;
static bool s_sensor_ready;
static TaskHandle_t s_real_io_task;
static TaskHandle_t s_motor_test_task;
static bool s_motor_test_running;
static SemaphoreHandle_t s_control_lock;
static sorting_sim_send_fn_t s_downstream_send_fn;
static void *s_downstream_send_ctx;

#define REAL_IO_POLL_MS 10
#define REAL_SCHEDULER_TICK_MS 100
#define SENSOR_DEBOUNCE_MS 20

typedef struct {
    bool raw;
    bool stable;
    int raw_level;
    int64_t raw_changed_ms;
    bool initialized;
} real_sensor_state_t;

static real_sensor_state_t s_real_sensors[5];
static bool s_sensor_valid[5];
static float s_encoder_distance_mm[3];
static bool s_encoder_valid[3];
static int64_t s_last_encoder_sample_ms;
static int64_t s_last_real_scheduler_tick_ms;

typedef struct {
    int id;
    sorter_package_state_t state;
    sorter_package_class_t cls;
    sorter_belt_t belt;
    bool occupied;
} sorter_debug_package_log_t;

static sorter_debug_package_log_t s_debug_packages[SORTER_MAX_PACKAGES];

static void ensure_motor_ready_locked(void);
static void ensure_hardware_observer_locked(void);
static void fill_hardware_status_locked(sorting_hardware_status_t *status);
static void clear_encoder_distance_locked(int index);
static void start_motor_test_locked(void);
static void update_vision_s1_locked(bool active);
static void set_motor_output_enabled_locked(bool enabled);
static void process_real_sensor_event_locked(bsp_sort_sensor_id_t sensor_id, bool active);
static void tick_scheduler_locked(void);

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
    ESP_LOGD(TAG, "OUT %s", line);
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
static uint32_t clamp_timeout_ms(int value, uint32_t fallback) { return value < 50 ? fallback : value > 30000 ? 30000U : (uint32_t)value; }

static const char *class_text(sorter_package_class_t cls)
{
    switch (cls) {
    case SORTER_CLASS_1: return "CLASS1/极兔";
    case SORTER_CLASS_2: return "CLASS2/中通";
    case SORTER_CLASS_3: return "CLASS3/韵达";
    case SORTER_CLASS_FREE: return "FREE";
    case SORTER_CLASS_ERROR: return "ERROR";
    case SORTER_CLASS_VISION_FAILED: return "识别失败轮转";
    default: return "未知";
    }
}

static const char *belt_text(sorter_belt_t belt)
{
    switch (belt) {
    case SORTER_BELT_A: return "A";
    case SORTER_BELT_B: return "B";
    case SORTER_BELT_C: return "C";
    default: return "?";
    }
}

static const char *direction_text(sorter_motor_direction_t dir)
{
    switch (dir) {
    case SORTER_MOTOR_FORWARD: return "正转";
    case SORTER_MOTOR_REVERSE: return "反转";
    case SORTER_MOTOR_BRAKE: return "刹车";
    default: return "停止";
    }
}

static sorter_debug_package_log_t *debug_package_slot(int id)
{
    sorter_debug_package_log_t *empty = NULL;
    for (int i = 0; i < SORTER_MAX_PACKAGES; ++i) {
        if (s_debug_packages[i].occupied && s_debug_packages[i].id == id) {
            return &s_debug_packages[i];
        }
        if (!s_debug_packages[i].occupied && !empty) {
            empty = &s_debug_packages[i];
        }
    }
    return empty ? empty : &s_debug_packages[id > 0 ? id % SORTER_MAX_PACKAGES : 0];
}

static void log_package_event(const sorter_event_t *event)
{
    const int id = event->package.id;
    sorter_debug_package_log_t *slot = debug_package_slot(id);
    const bool changed = !slot->occupied || slot->id != id ||
                         slot->state != event->package.state ||
                         slot->cls != event->package.cls ||
                         slot->belt != event->package.belt;
    if (!changed) {
        return;
    }

    *slot = (sorter_debug_package_log_t){
        .id = id,
        .state = event->package.state,
        .cls = event->package.cls,
        .belt = event->package.belt,
        .occupied = event->package.state != SORTER_STATE_DONE && event->package.state != SORTER_STATE_ERROR,
    };

    switch (event->package.state) {
    case SORTER_STATE_WAITING_VISION:
        ESP_LOGI(TAG, "SORT 包裹#%d 创建: 等待视觉识别", id);
        break;
    case SORTER_STATE_WAITING_AB:
        ESP_LOGI(TAG, "SORT 包裹#%d 已识别: %s, 等待到达S2", id, class_text(event->package.cls));
        break;
    case SORTER_STATE_HOLDING_AT_S2:
        ESP_LOGI(TAG, "SORT 包裹#%d 到达S2: 准备A->B交接, 目标=%s", id, class_text(event->package.cls));
        break;
    case SORTER_STATE_ON_B_TO_CLASS1:
        ESP_LOGI(TAG, "SORT 包裹#%d 开始CLASS1分拣: B段正向, 目标=%s", id, class_text(event->package.cls));
        break;
    case SORTER_STATE_ON_B_TO_S4:
        ESP_LOGI(TAG, "SORT 包裹#%d 转向S4: B段反向, 目标=%s", id, class_text(event->package.cls));
        break;
    case SORTER_STATE_HOLDING_AT_S4:
        ESP_LOGI(TAG, "SORT 包裹#%d 到达S4: 准备B->C交接, 目标=%s", id, class_text(event->package.cls));
        break;
    case SORTER_STATE_ON_C_EXIT:
        ESP_LOGI(TAG, "SORT 包裹#%d 开始%s分拣: C段出料", id, class_text(event->package.cls));
        break;
    case SORTER_STATE_DONE:
        ESP_LOGI(TAG, "SORT 包裹#%d 完成: %s", id, class_text(event->package.cls));
        break;
    case SORTER_STATE_ERROR:
        ESP_LOGW(TAG, "SORT 包裹#%d 错误结束: %s", id, class_text(event->package.cls));
        break;
    default:
        ESP_LOGI(TAG, "SORT 包裹#%d 状态更新: belt=%s state=%d class=%s",
                 id, belt_text(event->package.belt), (int)event->package.state,
                 class_text(event->package.cls));
        break;
    }
}

static void log_status_event(const sorter_event_t *event)
{
    const char *reason = event->status.reason ? event->status.reason : "";
    if (strcmp(reason, "tick") == 0 || strcmp(reason, "sensor1") == 0) {
        return;
    }
    if (strcmp(reason, "package_new") == 0 || strcmp(reason, "vision") == 0) {
        return;
    }
    ESP_LOGI(TAG, "SORT 状态: reason=%s active=%u",
             reason, (unsigned)event->status.active_count);
}

static void log_scheduler_event(const sorter_event_t *event)
{
    if (!event) {
        return;
    }
    switch (event->type) {
    case SORTER_EVENT_MOTOR:
        ESP_LOGI(TAG, "SORT 电机M%d: %s %d%%",
                 event->motor.motor_id,
                 direction_text(event->motor.direction),
                 event->motor.speed_percent);
        break;
    case SORTER_EVENT_PACKAGE:
        log_package_event(event);
        break;
    case SORTER_EVENT_STATUS:
        log_status_event(event);
        break;
    case SORTER_EVENT_FAULT:
        ESP_LOGW(TAG, "SORT 故障: code=%s package=%d sensor=S%d",
                 event->fault.code ? event->fault.code : "",
                 event->fault.package_id,
                 event->fault.sensor_id);
        break;
    default:
        break;
    }
}

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
    memset(s_sensor_valid, 0, sizeof(s_sensor_valid));
    memset(s_encoder_valid, 0, sizeof(s_encoder_valid));
    memset(s_encoder_distance_mm, 0, sizeof(s_encoder_distance_mm));
    memset(s_debug_packages, 0, sizeof(s_debug_packages));
    s_last_encoder_sample_ms = esp_timer_get_time() / 1000;
    s_last_real_scheduler_tick_ms = s_last_encoder_sample_ms;
}

static void clear_scheduler_motor_cache_locked(void)
{
    memset(s_scheduler.last_commands, 0, sizeof(s_scheduler.last_commands));
}

static bool external_sim_input_allowed(void)
{
    return s_debug_mode == SORTING_DEBUG_MODE_ETHERNET_SIM;
}

static bool real_sensor_input_allowed(void)
{
    return s_debug_mode == SORTING_DEBUG_MODE_REAL_SENSOR && s_sensor_input_enabled;
}

static bool real_encoder_input_allowed(void)
{
    return s_debug_mode == SORTING_DEBUG_MODE_REAL_SENSOR && s_sensor_input_enabled;
}

static void apply_motor_event(const sorter_motor_command_t *command)
{
    if (!command) return;
    if (!s_motor_output_enabled) return;
    ensure_motor_ready_locked();
    if (!s_motor_ready) return;
    int motor_id = command->motor_id;
    int speed = clamp_percent(command->speed_percent);
    if (motor_id < 1 || motor_id > 3) return;
    uint8_t direction = 0;
    uint32_t out_speed = (uint32_t)speed;
    if (command->direction == SORTER_MOTOR_REVERSE) direction = 1;
    else if (command->direction != SORTER_MOTOR_FORWARD) out_speed = 0;
    esp_err_t ret = bsp_motor_set_speed_direction((uint8_t)(motor_id - 1), out_speed, direction);
    if (ret != ESP_OK) {
        ESP_LOGW(TAG, "apply motor %d command failed: %s", motor_id, esp_err_to_name(ret));
        s_motor_ready = false;
    }
}

static int scheduler_event(void *ctx, const sorter_event_t *event)
{
    (void)ctx;
    log_scheduler_event(event);
    if (event && event->type == SORTER_EVENT_MOTOR) {
        apply_motor_event(&event->motor);
    }
    return 0;
}

static int scheduler_send(void *ctx, const char *line)
{
    (void)ctx;
    return s_downstream_send_fn ? s_downstream_send_fn(s_downstream_send_ctx, line) : 0;
}

static void set_scheduler_sender_locked(sorting_sim_send_fn_t fn, void *ctx)
{
    s_downstream_send_fn = fn;
    s_downstream_send_ctx = ctx;
    sorter_scheduler_set_event_handler(&s_scheduler, scheduler_event, NULL);
    sorter_scheduler_set_sender(&s_scheduler, scheduler_send, NULL);
}

static void ensure_scheduler_motor_sender_locked(void)
{
    sorter_scheduler_set_event_handler(&s_scheduler, scheduler_event, NULL);
    sorter_scheduler_set_sender(&s_scheduler, scheduler_send, NULL);
}

static void tick_scheduler_locked(void)
{
    ensure_scheduler_motor_sender_locked();
    sorter_scheduler_tick(&s_scheduler);
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
    const bsp_sort_sensor_id_t sensor_ids[] = { BSP_SORT_SENSOR_S1, BSP_SORT_SENSOR_S2, BSP_SORT_SENSOR_S3, BSP_SORT_SENSOR_S4 };
    while (true) {
        if (!s_real_io_enabled && !s_hardware_observer_enabled) { vTaskDelay(pdMS_TO_TICKS(100)); continue; }
        int64_t now = esp_timer_get_time() / 1000;
        if (s_sensor_ready) {
            for (size_t i = 0; i < sizeof(sensor_ids) / sizeof(sensor_ids[0]); ++i) {
                bsp_sort_sensor_id_t sensor_id = sensor_ids[i];
                bool active = false;
                int raw_level = 0;
                esp_err_t ret = bsp_sort_sensor_get_level(sensor_id, &raw_level, &active);
                bool valid = ret == ESP_OK;
                if (ret != ESP_OK && ret != ESP_ERR_NOT_FOUND) {
                    ESP_LOGW(TAG, "read sort sensor S%u failed: %s", (unsigned)sensor_id, esp_err_to_name(ret));
                    continue;
                }
                real_sensor_state_t *state = &s_real_sensors[(size_t)sensor_id];
                if (!state->initialized) {
                    state->raw = active; state->stable = active; state->raw_level = raw_level;
                    state->raw_changed_ms = now; state->initialized = true;
                    ESP_LOGI(TAG, "SORT 传感器S%u 初始化: raw=%d active=%d valid=%d",
                             (unsigned)sensor_id, raw_level, active ? 1 : 0, valid ? 1 : 0);
                    lock_control();
                    ensure_scheduler_motor_sender_locked();
                    s_sensor_valid[(size_t)sensor_id] = valid;
                    if (real_sensor_input_allowed() && valid) {
                        process_real_sensor_event_locked(sensor_id, active);
                    }
                    unlock_control();
                    continue;
                }
                if (state->raw != active || state->raw_level != raw_level) {
                    ESP_LOGD(TAG, "sort sensor S%u raw change raw_level=%d active=%d stable=%d valid=%d",
                             (unsigned)sensor_id, raw_level, active ? 1 : 0,
                             state->stable ? 1 : 0, valid ? 1 : 0);
                    state->raw = active;
                    state->raw_level = raw_level;
                    state->raw_changed_ms = now;
                    continue;
                }
                if (state->stable != state->raw && now - state->raw_changed_ms >= SENSOR_DEBOUNCE_MS) {
                    state->stable = state->raw;
                    ESP_LOGI(TAG, "SORT 传感器S%u 稳定变化: raw=%d active=%d debounce=%ums",
                             (unsigned)sensor_id, state->raw_level, state->stable ? 1 : 0,
                             (unsigned)SENSOR_DEBOUNCE_MS);
                    lock_control();
                    ensure_scheduler_motor_sender_locked();
                    s_sensor_valid[(size_t)sensor_id] = valid;
                    if (real_sensor_input_allowed() && valid) {
                        process_real_sensor_event_locked(sensor_id, state->stable);
                    }
                    unlock_control();
                } else {
                    lock_control();
                    s_sensor_valid[(size_t)sensor_id] = valid;
                    unlock_control();
                }
            }
        }
        if (s_encoder_ready) {
            if (s_last_encoder_sample_ms == 0) s_last_encoder_sample_ms = now;
            float dt_s = (float)(now - s_last_encoder_sample_ms) / 1000.0f;
            s_last_encoder_sample_ms = now;
            if (dt_s > 0 && dt_s < 1) {
                for (bsp_encoder_id_t i = BSP_ENCODER_0; i <= BSP_ENCODER_2; ++i) {
                    float speed_cm_s = 0;
                    esp_err_t ret = bsp_encoder_get_speed_cm_s(i, &speed_cm_s);
                    lock_control();
                    ensure_scheduler_motor_sender_locked();
                    s_encoder_valid[i] = ret == ESP_OK;
                    if (ret == ESP_OK) {
                        s_encoder_distance_mm[i] += fabsf(speed_cm_s) * 10.0f * dt_s;
                        if (i == BSP_ENCODER_2 && real_encoder_input_allowed()) {
                            sorter_scheduler_distance(&s_scheduler, 3, s_encoder_distance_mm[i]);
                        }
                    }
                    unlock_control();
                    if (ret != ESP_OK) {
                        ESP_LOGW(TAG, "read encoder %d speed failed: %s", (int)i, esp_err_to_name(ret));
                    }
                }
            }
        }
        if (now - s_last_real_scheduler_tick_ms >= REAL_SCHEDULER_TICK_MS) {
            lock_control();
            if (real_sensor_input_allowed() && sorter_scheduler_active_count(&s_scheduler) > 0) {
                tick_scheduler_locked();
            }
            unlock_control();
            s_last_real_scheduler_tick_ms = now;
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

static void ensure_motor_ready_locked(void)
{
    if (s_motor_ready || s_motor_init_attempted) return;
    s_motor_init_attempted = true;
    esp_err_t ret = bsp_motor_init();
    s_motor_ready = ret == ESP_OK;
    if (s_motor_ready) {
        ret = bsp_motor_set_output_enabled(s_motor_output_enabled);
        if (ret != ESP_OK) {
            ESP_LOGW(TAG, "set initial motor output state failed: %s", esp_err_to_name(ret));
            s_motor_ready = false;
        }
    }
    if (!s_motor_ready) ESP_LOGW(TAG, "real motor output disabled: %s", esp_err_to_name(ret));
}

static void ensure_real_input_ready_locked(void)
{
    if (!real_sensor_input_allowed()) return;
    ensure_hardware_observer_locked();
}

static void ensure_hardware_observer_locked(void)
{
    if (s_hardware_observer_init_attempted) {
        start_real_io_task_locked();
        return;
    }
    s_hardware_observer_init_attempted = true;
    esp_err_t ret = bsp_encoder_init();
    s_encoder_ready = ret == ESP_OK;
    if (!s_encoder_ready) ESP_LOGW(TAG, "hardware encoder observation disabled: %s", esp_err_to_name(ret));
    ret = bsp_sort_sensor_init();
    s_sensor_ready = ret == ESP_OK;
    if (!s_sensor_ready) ESP_LOGW(TAG, "hardware sort sensor observation disabled: %s", esp_err_to_name(ret));
    if (s_last_encoder_sample_ms == 0) s_last_encoder_sample_ms = esp_timer_get_time() / 1000;
    s_hardware_observer_enabled = s_encoder_ready || s_sensor_ready;
    s_last_real_scheduler_tick_ms = esp_timer_get_time() / 1000;
    start_real_io_task_locked();
}

static void set_real_io_enabled_locked(bool enabled)
{
    if (enabled) {
        ensure_real_input_ready_locked();
    }
    s_real_io_enabled = enabled;
    ESP_LOGI(TAG, "SORT 真实硬件链路%s", enabled ? "已启用" : "已关闭");
}

static void set_motor_output_enabled_locked(bool enabled)
{
    bool changed = s_motor_output_enabled != enabled;
    s_motor_output_enabled = enabled;
    if (changed) {
        clear_scheduler_motor_cache_locked();
    }
    ensure_motor_ready_locked();
    if (!s_motor_ready) {
        return;
    }
    esp_err_t ret = bsp_motor_set_output_enabled(enabled);
    if (ret != ESP_OK) {
        ESP_LOGW(TAG, "set motor output %s failed: %s",
                 enabled ? "enabled" : "disabled", esp_err_to_name(ret));
        s_motor_ready = false;
    }
}

static void fill_settings_locked(sorting_debug_settings_t *settings)
{
    if (!settings) return;
    *settings = (sorting_debug_settings_t){
        .mode = s_debug_mode,
        .motor_output_enabled = s_motor_output_enabled,
        .sensor_input_enabled = s_sensor_input_enabled,
        .motor_speed_percent = {
            s_config.motor_a_speed_percent,
            s_config.motor_b_speed_percent,
            s_config.motor_c_speed_percent,
        },
        .handoff_delay_ms = s_config.handoff_delay_ms,
        .belt_timeout_ms = {
            s_config.belt_a_timeout_ms,
            s_config.belt_b_timeout_ms,
            s_config.belt_c_timeout_ms,
        },
    };
}

static void apply_settings_locked(const sorting_debug_settings_t *settings)
{
    if (!settings) return;
    sorting_debug_mode_t mode = settings->mode;
    if (mode < SORTING_DEBUG_MODE_ETHERNET_SIM || mode > SORTING_DEBUG_MODE_TIMED_ONLY) {
        mode = SORTING_DEBUG_MODE_ETHERNET_SIM;
    }

    s_debug_mode = mode;
    set_motor_output_enabled_locked(settings->motor_output_enabled);
    s_sensor_input_enabled = mode == SORTING_DEBUG_MODE_TIMED_ONLY ? false : settings->sensor_input_enabled;
    s_config.motor_a_speed_percent = clamp_percent(settings->motor_speed_percent[0]);
    s_config.motor_b_speed_percent = clamp_percent(settings->motor_speed_percent[1]);
    s_config.motor_c_speed_percent = clamp_percent(settings->motor_speed_percent[2]);
    s_config.handoff_delay_ms = clamp_timeout_ms((int)settings->handoff_delay_ms, s_config.handoff_delay_ms);
    s_config.belt_a_timeout_ms = clamp_timeout_ms((int)settings->belt_timeout_ms[0], s_config.belt_a_timeout_ms);
    s_config.belt_b_timeout_ms = clamp_timeout_ms((int)settings->belt_timeout_ms[1], s_config.belt_b_timeout_ms);
    s_config.belt_c_timeout_ms = clamp_timeout_ms((int)settings->belt_timeout_ms[2], s_config.belt_c_timeout_ms);
    sorter_scheduler_configure(&s_scheduler, &s_config);

    ensure_motor_ready_locked();
    set_real_io_enabled_locked(real_sensor_input_allowed());
    if (s_motor_output_enabled) {
        tick_scheduler_locked();
    }
}

static const char *debug_mode_name(sorting_debug_mode_t mode)
{
    switch (mode) {
    case SORTING_DEBUG_MODE_REAL_SENSOR: return "real";
    case SORTING_DEBUG_MODE_TIMED_ONLY: return "timed";
    default: return "ethernet";
    }
}

static sorting_debug_mode_t parse_debug_mode(const char *line, sorting_debug_mode_t fallback)
{
    const char *p = strstr(line, "mode=");
    if (!p) return fallback;
    p += strlen("mode=");
    if (strncmp(p, "real", 4) == 0) return SORTING_DEBUG_MODE_REAL_SENSOR;
    if (strncmp(p, "timed", 5) == 0) return SORTING_DEBUG_MODE_TIMED_ONLY;
    if (strncmp(p, "ethernet", 8) == 0) return SORTING_DEBUG_MODE_ETHERNET_SIM;
    int value = (int)strtol(p, NULL, 10);
    if (value >= SORTING_DEBUG_MODE_ETHERNET_SIM && value <= SORTING_DEBUG_MODE_TIMED_ONLY) {
        return (sorting_debug_mode_t)value;
    }
    return fallback;
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

static void open_vision_window_locked(void)
{
    fail_current_vision_window();
    s_vision_package_id = s_next_package_id++;
    s_vision_classified = false;
    s_vision_failed_seen = false;
    sorter_scheduler_package_new(&s_scheduler, s_vision_package_id);
}

static void update_vision_s1_locked(bool active)
{
    if (active && !s_vision_s1_active) {
        open_vision_window_locked();
    }
    s_vision_s1_active = active;
}

static void process_real_sensor_event_locked(bsp_sort_sensor_id_t sensor_id, bool active)
{
    if (sensor_id == BSP_SORT_SENSOR_S1) {
        update_vision_s1_locked(active);
        return;
    }

    bool close_current_at_s2 = false;
    if (sensor_id == BSP_SORT_SENSOR_S2 && active && s_vision_package_id > 0 &&
        !sorter_scheduler_has_s2_candidate_before(&s_scheduler, s_vision_package_id)) {
        fail_current_vision_window();
        close_current_at_s2 = true;
    }
    sorter_scheduler_sensor(&s_scheduler, (int)sensor_id, active, 0);
    if (close_current_at_s2) {
        reset_vision_window();
    }
}

static void classify_current_vision_window_locked(sorter_package_class_t cls)
{
    if (s_vision_package_id <= 0 || s_vision_classified) return;
    if (is_success_class(cls)) {
        sorter_scheduler_vision_result(&s_scheduler, s_vision_package_id, cls);
        s_vision_classified = true;
    } else if (cls == SORTER_CLASS_ERROR) {
        s_vision_failed_seen = true;
        fail_current_vision_window();
    }
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
    ESP_LOGD(TAG, "IN %s", buf);
    lock_control();
    set_scheduler_sender_locked(send_fn, send_ctx);

#define RETURN_UNLOCK() do { unlock_control(); return; } while (0)
    if (strncmp(buf, "HELP", 4) == 0) {
        send_line(send_fn, send_ctx, "HELP,VISION_FRAME s1/free/class | CONFIG mode=ethernet|real|timed a_speed/b_speed/c_speed/handoff_delay_ms/a_timeout_ms/b_timeout_ms/c_timeout_ms/motor_output/sensor_input/real_io | PACKAGE_NEW id | VISION_RESULT id/free/class | SENSOR id/state | DISTANCE motor/dist | MOTOR_TEST | HW_STATUS | ENC_CLEAR index | RESET | ESTOP");
        RETURN_UNLOCK();
    }
    if (strncmp(buf, "CONFIG", 6) == 0) {
        sorting_debug_settings_t settings;
        fill_settings_locked(&settings);
        bool real_io_field_present = strstr(buf, "real_io=") != NULL;
        if (real_io_field_present) {
            settings.mode = parse_int_field(buf, "real_io=", s_real_io_enabled ? 1 : 0) != 0 ?
                            SORTING_DEBUG_MODE_REAL_SENSOR : SORTING_DEBUG_MODE_ETHERNET_SIM;
        }
        settings.mode = parse_debug_mode(buf, settings.mode);
        settings.motor_output_enabled = parse_int_field(buf, "motor_output=", settings.motor_output_enabled ? 1 : 0) != 0;
        settings.sensor_input_enabled = parse_int_field(buf, "sensor_input=", settings.sensor_input_enabled ? 1 : 0) != 0;
        settings.motor_speed_percent[0] = parse_int_field(buf, "a_speed=", settings.motor_speed_percent[0]);
        settings.motor_speed_percent[1] = parse_int_field(buf, "b_speed=", settings.motor_speed_percent[1]);
        settings.motor_speed_percent[2] = parse_int_field(buf, "c_speed=", settings.motor_speed_percent[2]);
        settings.handoff_delay_ms = clamp_ms(parse_int_field(buf, "handoff_delay_ms=", (int)settings.handoff_delay_ms), settings.handoff_delay_ms);
        settings.belt_timeout_ms[0] = clamp_timeout_ms(parse_int_field(buf, "a_timeout_ms=", (int)settings.belt_timeout_ms[0]), settings.belt_timeout_ms[0]);
        settings.belt_timeout_ms[1] = clamp_timeout_ms(parse_int_field(buf, "b_timeout_ms=", (int)settings.belt_timeout_ms[1]), settings.belt_timeout_ms[1]);
        settings.belt_timeout_ms[2] = clamp_timeout_ms(parse_int_field(buf, "c_timeout_ms=", (int)settings.belt_timeout_ms[2]), settings.belt_timeout_ms[2]);
        s_config.lost_timeout_min_ms = clamp_ms(parse_int_field(buf, "lost_timeout_min_ms=", (int)s_config.lost_timeout_min_ms), s_config.lost_timeout_min_ms);
        s_config.lost_timeout_max_ms = clamp_ms(parse_int_field(buf, "lost_timeout_max_ms=", (int)s_config.lost_timeout_max_ms), s_config.lost_timeout_max_ms);
        apply_settings_locked(&settings);
        char out[320];
        snprintf(out, sizeof(out), "STATUS,packages=%u,state=running,reason=config,mode=%s,a_speed=%d,b_speed=%d,c_speed=%d,handoff_delay_ms=%u,a_timeout_ms=%u,b_timeout_ms=%u,c_timeout_ms=%u,motor_output=%d,sensor_input=%d,real_io=%d",
                 (unsigned)sorter_scheduler_active_count(&s_scheduler), debug_mode_name(s_debug_mode),
                 s_config.motor_a_speed_percent, s_config.motor_b_speed_percent, s_config.motor_c_speed_percent,
                 (unsigned)s_config.handoff_delay_ms, (unsigned)s_config.belt_a_timeout_ms,
                 (unsigned)s_config.belt_b_timeout_ms, (unsigned)s_config.belt_c_timeout_ms,
                 s_motor_output_enabled ? 1 : 0, s_sensor_input_enabled ? 1 : 0, s_real_io_enabled ? 1 : 0);
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
        if (!external_sim_input_allowed()) { ESP_LOGI(TAG, "ignore simulated PACKAGE_NEW while mode=%s", debug_mode_name(s_debug_mode)); RETURN_UNLOCK(); }
        int id = parse_int_field(buf, "id=", 0);
        if (id <= 0) id = s_next_package_id++;
        else if (id >= s_next_package_id) s_next_package_id = id + 1;
        sorter_scheduler_package_new(&s_scheduler, id); RETURN_UNLOCK();
    }
    if (strncmp(buf, "PACKAGE_LOST", 12) == 0) {
        send_line(send_fn, send_ctx, "STATUS,packages=0,state=running,reason=legacy_package_lost_rejected"); RETURN_UNLOCK();
    }
    if (strncmp(buf, "VISION_RESULT", 13) == 0) {
        sorter_package_class_t cls = parse_vision_class(buf);
        if (external_sim_input_allowed()) {
            sorter_scheduler_vision_result(&s_scheduler, parse_int_field(buf, "id=", 0), cls);
        } else if (real_sensor_input_allowed()) {
            classify_current_vision_window_locked(cls);
        } else {
            ESP_LOGI(TAG, "ignore VISION_RESULT while mode=%s", debug_mode_name(s_debug_mode));
        }
        RETURN_UNLOCK();
    }
    if (strncmp(buf, "VISION_FRAME", 12) == 0) {
        bool s1_active = parse_int_field(buf, "s1=", 0) != 0;
        sorter_package_class_t cls = parse_vision_class(buf);
        if (external_sim_input_allowed()) {
            update_vision_s1_locked(s1_active);
            classify_current_vision_window_locked(cls);
        } else if (real_sensor_input_allowed()) {
            classify_current_vision_window_locked(cls);
        } else {
            ESP_LOGI(TAG, "ignore VISION_FRAME while mode=%s", debug_mode_name(s_debug_mode));
        }
        RETURN_UNLOCK();
    }
    if (strncmp(buf, "SENSOR", 6) == 0) {
        if (!external_sim_input_allowed()) { ESP_LOGI(TAG, "ignore simulated SENSOR while mode=%s", debug_mode_name(s_debug_mode)); RETURN_UNLOCK(); }
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
        if (!external_sim_input_allowed()) { ESP_LOGI(TAG, "ignore simulated DISTANCE while mode=%s", debug_mode_name(s_debug_mode)); RETURN_UNLOCK(); }
        sorter_scheduler_distance(&s_scheduler, parse_int_field(buf, "motor=", 0), parse_float_field(buf, "dist=", 0));
        RETURN_UNLOCK();
    }
    if (strncmp(buf, "MOTOR_TEST", 10) == 0) {
        start_motor_test_locked();
        send_line(send_fn, send_ctx, "STATUS,packages=0,state=running,reason=motor_test_started");
        RETURN_UNLOCK();
    }
    if (strncmp(buf, "HW_STATUS", 9) == 0) {
        sorting_hardware_status_t status;
        fill_hardware_status_locked(&status);
        char out[224];
        snprintf(out, sizeof(out),
                 "HW_STATUS,mtest=%d,s1=%d,s2=%d,s3=%d,s4=%d,s1_valid=%d,s2_valid=%d,s3_valid=%d,s4_valid=%d,enc_a=%.1f,enc_b=%.1f,enc_c=%.1f,enc_a_valid=%d,enc_b_valid=%d,enc_c_valid=%d",
                 status.motor_test_running ? 1 : 0,
                 status.sensor_active[0] ? 1 : 0, status.sensor_active[1] ? 1 : 0, status.sensor_active[2] ? 1 : 0, status.sensor_active[3] ? 1 : 0,
                 status.sensor_valid[0] ? 1 : 0, status.sensor_valid[1] ? 1 : 0, status.sensor_valid[2] ? 1 : 0, status.sensor_valid[3] ? 1 : 0,
                 (double)status.encoder_distance_mm[0], (double)status.encoder_distance_mm[1], (double)status.encoder_distance_mm[2],
                 status.encoder_valid[0] ? 1 : 0, status.encoder_valid[1] ? 1 : 0, status.encoder_valid[2] ? 1 : 0);
        send_line(send_fn, send_ctx, out);
        RETURN_UNLOCK();
    }
    if (strncmp(buf, "ENC_CLEAR", 9) == 0) {
        int index = parse_int_field(buf, "index=", 0);
        clear_encoder_distance_locked(index);
        send_line(send_fn, send_ctx, "STATUS,packages=0,state=running,reason=encoder_clear");
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

void sorting_sim_control_get_settings(sorting_debug_settings_t *settings)
{
    ensure_initialized();
    lock_control();
    fill_settings_locked(settings);
    unlock_control();
}

void sorting_sim_control_apply_settings(const sorting_debug_settings_t *settings)
{
    ensure_initialized();
    lock_control();
    apply_settings_locked(settings);
    unlock_control();
}

void sorting_sim_control_set_mode(sorting_debug_mode_t mode)
{
    sorting_debug_settings_t settings;
    sorting_sim_control_get_settings(&settings);
    settings.mode = mode;
    sorting_sim_control_apply_settings(&settings);
}

void sorting_sim_control_set_motor_output_enabled(bool enabled)
{
    sorting_debug_settings_t settings;
    sorting_sim_control_get_settings(&settings);
    settings.motor_output_enabled = enabled;
    sorting_sim_control_apply_settings(&settings);
}

void sorting_sim_control_set_sensor_input_enabled(bool enabled)
{
    sorting_debug_settings_t settings;
    sorting_sim_control_get_settings(&settings);
    settings.sensor_input_enabled = enabled;
    if (!enabled && settings.mode == SORTING_DEBUG_MODE_REAL_SENSOR) {
        settings.mode = SORTING_DEBUG_MODE_TIMED_ONLY;
    }
    sorting_sim_control_apply_settings(&settings);
}

// 模型 logo category（0=极兔 1=韵达 2=中通）→ 分拣枚举。
// 两套次序不同（控制层 CLASS_2=中通、CLASS_3=韵达），故为非顺序错位映射：
// 必须按公司名对齐，不能简化成 category+1。映射表与 SORTER_CLASS_* 枚举定义
// 同处本组件，改枚举时一眼可见需同步——vision 层不再持有此细节。
static sorter_package_class_t vision_category_to_class(int model_category)
{
    switch (model_category) {
    case 0: return SORTER_CLASS_1;  // 极兔
    case 1: return SORTER_CLASS_3;  // 韵达
    case 2: return SORTER_CLASS_2;  // 中通
    default: return SORTER_CLASS_UNKNOWN;
    }
}

void sorting_sim_control_submit_vision_category(int model_category, float confidence)
{
    ensure_initialized();
    sorter_package_class_t cls = vision_category_to_class(model_category);
    lock_control();
    if (!real_sensor_input_allowed()) {
        ESP_LOGD(TAG, "ignore local vision category while mode=%s sensor_input=%d",
                 debug_mode_name(s_debug_mode), s_sensor_input_enabled ? 1 : 0);
        unlock_control();
        return;
    }
    ensure_scheduler_motor_sender_locked();
    if (s_vision_package_id <= 0 || s_vision_classified) {
        ESP_LOGD(TAG, "ignore local vision cat=%d cls=%d conf=%.3f package=%d classified=%d",
                 model_category, (int)cls, (double)confidence, s_vision_package_id, s_vision_classified ? 1 : 0);
        unlock_control();
        return;
    }
    ESP_LOGI(TAG, "SORT 视觉识别: 包裹#%d %s conf=%.1f%%",
             s_vision_package_id, class_text(cls), (double)(confidence * 100.0f));
    classify_current_vision_window_locked(cls);
    unlock_control();
}

void sorting_sim_control_simulate_class(sorter_package_class_t cls, sorting_sim_send_fn_t send_fn, void *send_ctx)
{
    ensure_initialized();
    lock_control();
    set_scheduler_sender_locked(send_fn, send_ctx);
    fail_current_vision_window();
    int id = s_next_package_id++;
    s_vision_package_id = id;
    s_vision_classified = true;
    s_vision_failed_seen = cls == SORTER_CLASS_ERROR || cls == SORTER_CLASS_VISION_FAILED;
    s_vision_s1_active = false;
    sorter_scheduler_package_new(&s_scheduler, id);
    sorter_scheduler_vision_result(&s_scheduler, id, cls);
    sorter_scheduler_tick(&s_scheduler);
    unlock_control();
}

static void fill_hardware_status_locked(sorting_hardware_status_t *status)
{
    if (!status) return;
    ensure_hardware_observer_locked();
    memset(status, 0, sizeof(*status));
    status->motor_test_running = s_motor_test_running;
    const int sensor_ids[4] = { BSP_SORT_SENSOR_S1, BSP_SORT_SENSOR_S2, BSP_SORT_SENSOR_S3, BSP_SORT_SENSOR_S4 };
    for (int i = 0; i < 4; ++i) {
        int sensor_id = sensor_ids[i];
        status->sensor_valid[i] = s_sensor_valid[sensor_id];
        status->sensor_active[i] = s_real_sensors[sensor_id].stable;
        if (i < 3) {
            status->encoder_valid[i] = s_encoder_valid[i];
            status->encoder_distance_mm[i] = s_encoder_distance_mm[i];
        }
    }
}

static sorter_package_class_t failed_class_from_cursor(int cursor)
{
    switch (cursor % 3) {
    case 1: return SORTER_CLASS_2;
    case 2: return SORTER_CLASS_3;
    default: return SORTER_CLASS_1;
    }
}

static void fill_runtime_debug_locked(sorting_runtime_debug_t *status)
{
    if (!status) return;
    memset(status, 0, sizeof(*status));
    status->active_count = sorter_scheduler_active_count(&s_scheduler);
    status->max_packages = SORTER_MAX_PACKAGES;
    status->next_package_id = s_next_package_id;
    status->vision_package_id = s_vision_package_id;
    status->vision_classified = s_vision_classified;
    status->vision_s1_active = s_vision_s1_active;
    status->b_owner = s_scheduler.b_owner;
    status->c_owner = s_scheduler.c_owner;
    status->next_failed_class = failed_class_from_cursor(s_scheduler.failed_class_cursor);
    for (int i = 0; i < SORTER_MAX_PACKAGES; ++i) {
        const sorter_package_track_t *track = &s_scheduler.tracks[i];
        status->packages[i] = (sorting_package_debug_t){
            .id = track->id,
            .cls = track->cls,
            .state = track->state,
            .belt = track->belt,
            .pos_mm = track->pos_mm,
            .occupied = track->occupied,
        };
    }
}

void sorting_sim_control_get_hardware_status(sorting_hardware_status_t *status)
{
    ensure_initialized();
    lock_control();
    fill_hardware_status_locked(status);
    unlock_control();
}

void sorting_sim_control_get_runtime_debug(sorting_runtime_debug_t *status)
{
    ensure_initialized();
    lock_control();
    fill_runtime_debug_locked(status);
    unlock_control();
}

static void clear_encoder_distance_locked(int index)
{
    if (index < 0 || index >= 3) return;
    s_encoder_distance_mm[index] = 0.0f;
}

void sorting_sim_control_clear_encoder_distance(int index)
{
    ensure_initialized();
    lock_control();
    clear_encoder_distance_locked(index);
    unlock_control();
}

static void motor_test_task(void *arg)
{
    (void)arg;
    int speeds[3];
    lock_control();
    speeds[0] = s_config.motor_a_speed_percent;
    speeds[1] = s_config.motor_b_speed_percent;
    speeds[2] = s_config.motor_c_speed_percent;
    ensure_motor_ready_locked();
    bool ready = s_motor_ready && s_motor_output_enabled;
    unlock_control();

    if (ready) {
        for (uint8_t i = 0; i < 3; ++i) {
            esp_err_t ret = bsp_motor_set_speed_direction(i, (uint32_t)clamp_percent(speeds[i]), 0);
            if (ret != ESP_OK) {
                ESP_LOGW(TAG, "motor test start motor %u failed: %s", (unsigned)i + 1, esp_err_to_name(ret));
                break;
            }
            vTaskDelay(pdMS_TO_TICKS(500));
            ret = bsp_motor_set_speed_direction(i, 0, 0);
            if (ret != ESP_OK) {
                ESP_LOGW(TAG, "motor test stop motor %u failed: %s", (unsigned)i + 1, esp_err_to_name(ret));
                break;
            }
            vTaskDelay(pdMS_TO_TICKS(80));
        }
        for (uint8_t i = 0; i < 3; ++i) {
            (void)bsp_motor_set_speed_direction(i, 0, 0);
        }
    }

    lock_control();
    s_motor_test_running = false;
    s_motor_test_task = NULL;
    unlock_control();
    vTaskDelete(NULL);
}

static void start_motor_test_locked(void)
{
    if (s_motor_test_running) return;
    s_motor_test_running = true;
    BaseType_t ok = xTaskCreatePinnedToCore(motor_test_task, "motor_test", 4096, NULL, 5, &s_motor_test_task, 0);
    if (ok != pdPASS) {
        s_motor_test_running = false;
        s_motor_test_task = NULL;
        ESP_LOGE(TAG, "create motor test task failed");
    }
}

void sorting_sim_control_start_motor_test(void)
{
    ensure_initialized();
    lock_control();
    start_motor_test_locked();
    unlock_control();
}

static int serial_send(void *ctx, const char *line)
{
    (void)ctx;
    printf("SIMOUT %s\n", line); fflush(stdout); return 0;
}

static void debug_wait_and_tick(uint32_t wait_ms)
{
    if (wait_ms > 0) {
        vTaskDelay(pdMS_TO_TICKS(wait_ms));
    }
    sorting_sim_control_tick(serial_send, NULL);
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
    sorting_sim_control_handle_line("SENSOR,id=2,state=0", strlen("SENSOR,id=2,state=0"), serial_send, NULL);
    debug_wait_and_tick(s_config.handoff_delay_ms + 100);
    if (cls == 1) sorting_sim_control_handle_line("SENSOR,id=3,state=1", strlen("SENSOR,id=3,state=1"), serial_send, NULL);
    else {
        sorting_sim_control_handle_line("SENSOR,id=4,state=1", strlen("SENSOR,id=4,state=1"), serial_send, NULL);
        sorting_sim_control_handle_line("SENSOR,id=4,state=0", strlen("SENSOR,id=4,state=0"), serial_send, NULL);
        debug_wait_and_tick(s_config.handoff_delay_ms + 100);
        sorting_sim_control_handle_line("DISTANCE,motor=3,dist=0.0", strlen("DISTANCE,motor=3,dist=0.0"), serial_send, NULL);
        debug_wait_and_tick(s_config.c_min_busy_ms + 100);
        sorting_sim_control_handle_line("DISTANCE,motor=3,dist=370.0", strlen("DISTANCE,motor=3,dist=370.0"), serial_send, NULL);
    }
    printf("SIMTEST %s end\n", name); fflush(stdout);
}

static void serial_tick_active(void)
{
    ensure_initialized();
    lock_control();
    if (sorter_scheduler_active_count(&s_scheduler) > 0) {
        if (s_downstream_send_fn && s_downstream_send_fn != serial_send) {
            unlock_control();
            return;
        }
        set_scheduler_sender_locked(serial_send, NULL);
        sorter_scheduler_tick(&s_scheduler);
    }
    unlock_control();
}

static void debug_task(void *arg)
{
    (void)arg;
    char line[192];
    size_t len = 0;
    int64_t next_tick_ms = 0;
    printf("\nSORTDBG ready. Commands: help, reset, test1, test2, test3, or raw protocol lines.\n"); fflush(stdout);
    while (true) {
        uint8_t bytes[32];
        int got = usb_serial_jtag_read_bytes(bytes, sizeof(bytes), pdMS_TO_TICKS(100));
        if (got > 0) {
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
        int64_t now_ms = esp_timer_get_time() / 1000;
        if (now_ms >= next_tick_ms) {
            serial_tick_active();
            next_tick_ms = now_ms + 100;
        }
    }
}

void sorting_sim_debug_start(void)
{
#if SORTER_DEBUG_ENABLE_USB_SERIAL
    if (s_debug_task) return;
    usb_serial_jtag_driver_config_t config = { .rx_buffer_size = 512, .tx_buffer_size = 512 };
    esp_err_t ret = usb_serial_jtag_driver_install(&config);
    if (ret != ESP_OK && ret != ESP_ERR_INVALID_STATE)
        ESP_LOGW(TAG, "USB Serial/JTAG debug driver install failed: %s", esp_err_to_name(ret));
    xTaskCreatePinnedToCore(debug_task, "sort_dbg", 4096, NULL, 3, &s_debug_task, 0);
#else
    ESP_LOGI(TAG, "USB Serial/JTAG sorter debug disabled");
#endif
}
