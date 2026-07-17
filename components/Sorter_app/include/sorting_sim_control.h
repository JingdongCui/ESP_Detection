#pragma once

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "sorter_core/sorter_scheduler.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*sorting_sim_send_fn_t)(void *ctx, const char *line);

typedef enum {
    SORTING_DEBUG_MODE_ETHERNET_SIM = 0,
    SORTING_DEBUG_MODE_REAL_SENSOR,
    SORTING_DEBUG_MODE_TIMED_ONLY,
} sorting_debug_mode_t;

typedef struct {
    sorting_debug_mode_t mode;
    bool motor_output_enabled;
    bool sensor_input_enabled;
    int motor_speed_percent[3];
    uint32_t handoff_delay_ms;
    uint32_t belt_timeout_ms[3];
} sorting_debug_settings_t;

typedef struct {
    bool motor_test_running;
    bool sensor_valid[4];
    bool sensor_active[4];
    bool encoder_valid[3];
    float encoder_distance_mm[3];
} sorting_hardware_status_t;

typedef struct {
    int id;
    sorter_package_class_t cls;
    sorter_package_state_t state;
    sorter_belt_t belt;
    float pos_mm;
    bool occupied;
} sorting_package_debug_t;

typedef struct {
    size_t active_count;
    int max_packages;
    int next_package_id;
    int vision_package_id;
    bool vision_classified;
    bool vision_s1_active;
    int b_owner;
    int c_owner;
    sorter_package_class_t next_failed_class;
    sorting_package_debug_t packages[SORTER_MAX_PACKAGES];
} sorting_runtime_debug_t;

void sorting_sim_control_reset(void);
void sorting_sim_control_handle_line(const char *line, size_t len, sorting_sim_send_fn_t send_fn, void *send_ctx);
void sorting_sim_control_tick(sorting_sim_send_fn_t send_fn, void *send_ctx);
void sorting_sim_control_get_settings(sorting_debug_settings_t *settings);
void sorting_sim_control_apply_settings(const sorting_debug_settings_t *settings);
void sorting_sim_control_set_mode(sorting_debug_mode_t mode);
bool sorting_sim_control_get_motor_output_enabled(void);
void sorting_sim_control_set_motor_output_enabled(bool enabled);
int sorting_sim_control_get_motor_speed_percent(int motor_index);
void sorting_sim_control_set_motor_speed_percent(int motor_index, int percent);
void sorting_sim_control_set_sensor_input_enabled(bool enabled);
void sorting_sim_control_submit_vision_category(int model_category, float confidence);
void sorting_sim_control_simulate_class(sorter_package_class_t cls, sorting_sim_send_fn_t send_fn, void *send_ctx);
void sorting_sim_control_get_hardware_status(sorting_hardware_status_t *status);
void sorting_sim_control_get_runtime_debug(sorting_runtime_debug_t *status);
void sorting_sim_control_start_motor_test(void);
void sorting_sim_control_clear_encoder_distance(int index);

#ifdef __cplusplus
}
#endif
