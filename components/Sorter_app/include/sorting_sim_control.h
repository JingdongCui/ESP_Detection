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

void sorting_sim_control_reset(void);
void sorting_sim_control_handle_line(const char *line, size_t len, sorting_sim_send_fn_t send_fn, void *send_ctx);
void sorting_sim_control_tick(sorting_sim_send_fn_t send_fn, void *send_ctx);
void sorting_sim_debug_start(void);
void sorting_sim_control_get_settings(sorting_debug_settings_t *settings);
void sorting_sim_control_apply_settings(const sorting_debug_settings_t *settings);
void sorting_sim_control_set_mode(sorting_debug_mode_t mode);
void sorting_sim_control_set_motor_output_enabled(bool enabled);
void sorting_sim_control_set_sensor_input_enabled(bool enabled);
void sorting_sim_control_simulate_class(sorter_package_class_t cls, sorting_sim_send_fn_t send_fn, void *send_ctx);

#ifdef __cplusplus
}
#endif
