#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SORTER_MAX_PACKAGES 8
#define SORTER_MAX_MOTORS 3

typedef enum {
    SORTER_CLASS_UNKNOWN = 0,
    SORTER_CLASS_1,
    SORTER_CLASS_2,
    SORTER_CLASS_3,
    SORTER_CLASS_FREE,
    SORTER_CLASS_ERROR,
    SORTER_CLASS_VISION_FAILED,
} sorter_package_class_t;

typedef enum {
    SORTER_STATE_EMPTY = 0,
    SORTER_STATE_WAITING_VISION,
    SORTER_STATE_WAITING_AB,
    SORTER_STATE_HOLDING_AT_S2,
    SORTER_STATE_ON_B_TO_CLASS1,
    SORTER_STATE_ON_B_TO_S4,
    SORTER_STATE_WAITING_BC,
    SORTER_STATE_HOLDING_AT_S4,
    SORTER_STATE_ON_C_EXIT,
    SORTER_STATE_DONE,
    SORTER_STATE_ERROR,
} sorter_package_state_t;

typedef enum { SORTER_BELT_A = 1, SORTER_BELT_B, SORTER_BELT_C } sorter_belt_t;
typedef enum { SORTER_MOTOR_STOP = 0, SORTER_MOTOR_FORWARD, SORTER_MOTOR_REVERSE, SORTER_MOTOR_BRAKE } sorter_motor_direction_t;

typedef struct {
    float package_length_mm;
    float safe_gap_mm;
    float a_start_to_vision_mm;
    float a_vision_to_b_mm;
    float b_center_to_exit_mm;
    float c_exit_distance_mm;
    float transfer_timeout_mm;
    uint32_t c_min_busy_ms;
    uint32_t c_fallback_busy_ms;
    uint32_t handoff_delay_ms;
    uint32_t lost_timeout_min_ms;
    uint32_t lost_timeout_max_ms;
    uint32_t belt_a_timeout_ms;
    uint32_t belt_b_timeout_ms;
    uint32_t belt_c_timeout_ms;
    int motor_a_speed_percent;
    int motor_b_speed_percent;
    int motor_c_speed_percent;
    int max_packages;
} sorter_config_t;

typedef struct {
    int id;
    sorter_package_class_t cls;
    sorter_package_state_t state;
    sorter_belt_t belt;
    float pos_mm;
    int64_t state_enter_ms;
    uint32_t state_timeout_ms;
    int64_t handoff_ready_ms;
    float c_start_dist_mm;
    int64_t c_start_ms;
    int64_t c_busy_until_ms;
    int64_t c_fallback_until_ms;
    bool c_distance_started;
    bool occupied;
} sorter_package_track_t;

typedef struct {
    int motor_id;
    sorter_motor_direction_t direction;
    int speed_percent;
} sorter_motor_command_t;

typedef int (*sorter_send_fn_t)(void *ctx, const char *line);

typedef struct {
    sorter_config_t config;
    sorter_package_track_t tracks[SORTER_MAX_PACKAGES];
    int b_owner;
    int c_owner;
    sorter_motor_direction_t b_direction;
    sorter_motor_direction_t c_direction;
    sorter_send_fn_t send_fn;
    void *send_ctx;
    bool estop;
    bool paused_after_estop;
    bool s2_sensor_active;
    bool s4_sensor_active;
    int s2_held_package_id;
    int s4_held_package_id;
    sorter_motor_command_t last_commands[SORTER_MAX_MOTORS];
} sorter_scheduler_t;

void sorter_config_default(sorter_config_t *config);
void sorter_scheduler_init(sorter_scheduler_t *scheduler, const sorter_config_t *config);
void sorter_scheduler_reset(sorter_scheduler_t *scheduler);
void sorter_scheduler_configure(sorter_scheduler_t *scheduler, const sorter_config_t *config);
void sorter_scheduler_set_sender(sorter_scheduler_t *scheduler, sorter_send_fn_t send_fn, void *ctx);
void sorter_scheduler_package_new(sorter_scheduler_t *scheduler, int id);
void sorter_scheduler_vision_result(sorter_scheduler_t *scheduler, int id, sorter_package_class_t cls);
void sorter_scheduler_sensor(sorter_scheduler_t *scheduler, int sensor_id, bool active, int package_id);
void sorter_scheduler_distance(sorter_scheduler_t *scheduler, int motor_id, float distance_mm);
void sorter_scheduler_estop(sorter_scheduler_t *scheduler, bool enabled);
void sorter_scheduler_tick(sorter_scheduler_t *scheduler);
size_t sorter_scheduler_active_count(const sorter_scheduler_t *scheduler);
bool sorter_scheduler_has_s2_candidate_before(const sorter_scheduler_t *scheduler, int package_id);
sorter_package_class_t sorter_package_class_from_int(int value, bool free_flag, bool error_flag);

#ifdef __cplusplus
}
#endif
