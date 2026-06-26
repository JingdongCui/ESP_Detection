#include "sorter_core/sorter_scheduler.h"

#include "esp_timer.h"

#include <stdio.h>
#include <string.h>

static int64_t now_ms(void) { return esp_timer_get_time() / 1000; }
static void schedule(sorter_scheduler_t *s);
static bool service_state_timeouts(sorter_scheduler_t *s);
static void service_pending_transfers(sorter_scheduler_t *s);

void sorter_config_default(sorter_config_t *c)
{
    if (!c) return;
    *c = (sorter_config_t){
        .package_length_mm = 120.0f, .safe_gap_mm = 80.0f,
        .a_start_to_vision_mm = 300.0f, .a_vision_to_b_mm = 520.0f,
        .b_center_to_exit_mm = 330.0f, .c_exit_distance_mm = 360.0f,
        .transfer_timeout_mm = 700.0f, .c_min_busy_ms = 2500,
        .c_fallback_busy_ms = 8000, .handoff_delay_ms = 1000,
        .lost_timeout_min_ms = 3000, .lost_timeout_max_ms = 6000,
        .motor_a_speed_percent = 30, .motor_b_speed_percent = 35,
        .motor_c_speed_percent = 35, .max_packages = SORTER_MAX_PACKAGES,
    };
}

void sorter_scheduler_init(sorter_scheduler_t *s, const sorter_config_t *c)
{
    memset(s, 0, sizeof(*s));
    if (c) s->config = *c; else sorter_config_default(&s->config);
}

void sorter_scheduler_reset(sorter_scheduler_t *s)
{
    sorter_config_t config = s->config;
    sorter_send_fn_t send_fn = s->send_fn;
    void *send_ctx = s->send_ctx;
    memset(s, 0, sizeof(*s));
    s->config = config;
    s->send_fn = send_fn;
    s->send_ctx = send_ctx;
}

void sorter_scheduler_configure(sorter_scheduler_t *s, const sorter_config_t *c) { s->config = *c; }
void sorter_scheduler_set_sender(sorter_scheduler_t *s, sorter_send_fn_t fn, void *ctx) { s->send_fn = fn; s->send_ctx = ctx; }

size_t sorter_scheduler_active_count(const sorter_scheduler_t *s)
{
    size_t count = 0;
    for (int i = 0; i < SORTER_MAX_PACKAGES; ++i) if (s->tracks[i].occupied) ++count;
    return count;
}

static sorter_package_track_t *find_package(sorter_scheduler_t *s, int id)
{
    if (id <= 0) return NULL;
    for (int i = 0; i < SORTER_MAX_PACKAGES; ++i)
        if (s->tracks[i].occupied && s->tracks[i].id == id) return &s->tracks[i];
    return NULL;
}

static sorter_package_track_t *allocate_package(sorter_scheduler_t *s, int id)
{
    for (int i = 0; i < SORTER_MAX_PACKAGES; ++i) {
        if (!s->tracks[i].occupied) {
            memset(&s->tracks[i], 0, sizeof(s->tracks[i]));
            s->tracks[i].id = id;
            s->tracks[i].occupied = true;
            return &s->tracks[i];
        }
    }
    return NULL;
}

static sorter_package_track_t *first_in_state(sorter_scheduler_t *s, sorter_package_state_t state)
{
    sorter_package_track_t *best = NULL;
    for (int i = 0; i < SORTER_MAX_PACKAGES; ++i) {
        sorter_package_track_t *p = &s->tracks[i];
        if (p->occupied && p->state == state && (!best || p->id < best->id)) best = p;
    }
    return best;
}

static sorter_package_track_t *package_in_state(sorter_scheduler_t *s, int id, sorter_package_state_t state)
{
    sorter_package_track_t *p = find_package(s, id);
    return p && p->state == state ? p : NULL;
}

static const char *class_name(sorter_package_class_t v)
{
    switch (v) {
    case SORTER_CLASS_1: return "class1"; case SORTER_CLASS_2: return "class2";
    case SORTER_CLASS_3: return "class3"; case SORTER_CLASS_FREE: return "free";
    case SORTER_CLASS_ERROR: return "error"; case SORTER_CLASS_VISION_FAILED: return "vision_failed";
    default: return "unknown";
    }
}

static const char *state_name(sorter_package_state_t v)
{
    switch (v) {
    case SORTER_STATE_WAITING_VISION: return "waiting_vision"; case SORTER_STATE_WAITING_AB: return "waiting_ab";
    case SORTER_STATE_HOLDING_AT_S2: return "holding_at_s2"; case SORTER_STATE_ON_B_TO_CLASS1: return "on_b_to_class1";
    case SORTER_STATE_ON_B_TO_S4: return "on_b_to_s4"; case SORTER_STATE_WAITING_BC: return "waiting_bc";
    case SORTER_STATE_HOLDING_AT_S4: return "holding_at_s4"; case SORTER_STATE_ON_C_EXIT: return "on_c_exit";
    case SORTER_STATE_DONE: return "done"; case SORTER_STATE_ERROR: return "error"; default: return "empty";
    }
}

static const char *belt_name(sorter_belt_t v) { return v == SORTER_BELT_A ? "A" : v == SORTER_BELT_B ? "B" : v == SORTER_BELT_C ? "C" : "?"; }
static const char *direction_name(sorter_motor_direction_t v) { return v == SORTER_MOTOR_FORWARD ? "forward" : v == SORTER_MOTOR_REVERSE ? "reverse" : v == SORTER_MOTOR_BRAKE ? "brake" : "stop"; }
static const char *scheduler_state_name(const sorter_scheduler_t *s) { return s->estop ? "estop" : s->paused_after_estop ? "paused" : "running"; }

static void emit_motor(sorter_scheduler_t *s, int id, sorter_motor_direction_t dir, int speed, bool force)
{
    if (id < 1 || id > SORTER_MAX_MOTORS) return;
    sorter_motor_command_t *last = &s->last_commands[id - 1];
    if (!force && last->motor_id == id && last->direction == dir && last->speed_percent == speed) return;
    *last = (sorter_motor_command_t){ id, dir, speed };
    char line[80];
    snprintf(line, sizeof(line), "MOTOR,id=%d,dir=%s,speed=%d", id, direction_name(dir), speed);
    if (s->send_fn) s->send_fn(s->send_ctx, line);
}

static void emit_status(sorter_scheduler_t *s, const char *reason)
{
    char line[128];
    snprintf(line, sizeof(line), "STATUS,packages=%u,state=%s,reason=%s",
             (unsigned)sorter_scheduler_active_count(s), scheduler_state_name(s), reason);
    if (s->send_fn) s->send_fn(s->send_ctx, line);
}

static void emit_package(sorter_scheduler_t *s, const sorter_package_track_t *p)
{
    char line[160];
    snprintf(line, sizeof(line), "PKG,id=%d,belt=%s,pos=%.1f,state=%s,class=%s",
             p->id, belt_name(p->belt), p->pos_mm, state_name(p->state), class_name(p->cls));
    if (s->send_fn) s->send_fn(s->send_ctx, line);
}

static void emit_fault(sorter_scheduler_t *s, const char *code, int package_id, int sensor_id)
{
    char line[96];
    snprintf(line, sizeof(line), "FAULT,code=%s,package=%d,sensor=%d", code, package_id, sensor_id);
    if (s->send_fn) s->send_fn(s->send_ctx, line);
    emit_status(s, code);
}

static bool motor_running(const sorter_scheduler_t *s, int id)
{
    if (id < 1 || id > SORTER_MAX_MOTORS) return false;
    const sorter_motor_command_t *c = &s->last_commands[id - 1];
    return c->speed_percent > 0 && (c->direction == SORTER_MOTOR_FORWARD || c->direction == SORTER_MOTOR_REVERSE);
}

static uint32_t timeout_for_speed(const sorter_scheduler_t *s, int speed)
{
    uint32_t min = s->config.lost_timeout_min_ms < s->config.lost_timeout_max_ms ? s->config.lost_timeout_min_ms : s->config.lost_timeout_max_ms;
    uint32_t max = s->config.lost_timeout_min_ms > s->config.lost_timeout_max_ms ? s->config.lost_timeout_min_ms : s->config.lost_timeout_max_ms;
    if (speed < 0) speed = 0; else if (speed > 100) speed = 100;
    return max - (uint32_t)((max - min) * speed / 100);
}

static uint32_t timeout_for_state(const sorter_scheduler_t *s, sorter_package_state_t state)
{
    if (state == SORTER_STATE_WAITING_VISION || state == SORTER_STATE_WAITING_AB) return timeout_for_speed(s, s->config.motor_a_speed_percent);
    if (state == SORTER_STATE_HOLDING_AT_S2 || state == SORTER_STATE_ON_B_TO_CLASS1 || state == SORTER_STATE_ON_B_TO_S4) return timeout_for_speed(s, s->config.motor_b_speed_percent);
    if (state == SORTER_STATE_HOLDING_AT_S4) return timeout_for_speed(s, s->config.motor_c_speed_percent);
    return 0;
}

static void transition_state(sorter_scheduler_t *s, sorter_package_track_t *p, sorter_package_state_t state)
{
    p->state = state; p->state_enter_ms = now_ms(); p->state_timeout_ms = timeout_for_state(s, state);
}

static void release_reservations(sorter_scheduler_t *s, int id)
{
    if (s->b_owner == id) { s->b_owner = 0; s->b_direction = SORTER_MOTOR_STOP; }
    if (s->c_owner == id) { s->c_owner = 0; s->c_direction = SORTER_MOTOR_STOP; }
}

static bool reserve_b(sorter_scheduler_t *s, int id, sorter_motor_direction_t dir)
{
    if (s->b_owner != 0 && s->b_owner != id) return false;
    s->b_owner = id; s->b_direction = dir; return true;
}

static bool reserve_c(sorter_scheduler_t *s, int id, sorter_motor_direction_t dir)
{
    if (s->c_owner != 0 && s->c_owner != id) return false;
    s->c_owner = id; s->c_direction = dir; return true;
}

static bool can_move_a_package(const sorter_scheduler_t *s, const sorter_package_track_t *moving)
{
    for (int i = 0; i < SORTER_MAX_PACKAGES; ++i) {
        const sorter_package_track_t *o = &s->tracks[i];
        if (!o->occupied || o->id == moving->id || o->belt != SORTER_BELT_A) continue;
        if (o->pos_mm > moving->pos_mm && o->pos_mm - moving->pos_mm < s->config.package_length_mm + s->config.safe_gap_mm) return false;
    }
    return true;
}

static bool s2_blocks_a(const sorter_scheduler_t *s)
{
    for (int i = 0; i < SORTER_MAX_PACKAGES; ++i) {
        const sorter_package_track_t *p = &s->tracks[i];
        if (p->occupied && p->state == SORTER_STATE_HOLDING_AT_S2 && p->handoff_ready_ms == 0 && s->b_owner != p->id) return true;
    }
    return false;
}

static bool release_s2_to_b(sorter_scheduler_t *s, sorter_package_track_t *p)
{
    if (p->state != SORTER_STATE_HOLDING_AT_S2 || p->handoff_ready_ms != 0) return false;
    sorter_motor_direction_t dir = (p->cls == SORTER_CLASS_1 || p->cls == SORTER_CLASS_FREE || p->cls == SORTER_CLASS_VISION_FAILED) ? SORTER_MOTOR_FORWARD : SORTER_MOTOR_REVERSE;
    if (!reserve_b(s, p->id, dir)) return false;
    if (s->s2_held_package_id == p->id) s->s2_held_package_id = 0;
    emit_motor(s, 1, SORTER_MOTOR_FORWARD, s->config.motor_a_speed_percent, true);
    emit_motor(s, 2, SORTER_MOTOR_STOP, 0, false);
    return true;
}

static bool release_s4_to_c(sorter_scheduler_t *s, sorter_package_track_t *p)
{
    if (p->state != SORTER_STATE_HOLDING_AT_S4 || p->handoff_ready_ms != 0) return false;
    sorter_motor_direction_t dir = p->cls == SORTER_CLASS_2 ? SORTER_MOTOR_REVERSE : SORTER_MOTOR_FORWARD;
    if (!reserve_c(s, p->id, dir)) return false;
    if (s->s4_held_package_id == p->id) s->s4_held_package_id = 0;
    emit_motor(s, 2, SORTER_MOTOR_REVERSE, s->config.motor_b_speed_percent, true);
    emit_motor(s, 3, SORTER_MOTOR_STOP, 0, false);
    return true;
}

static void complete_s2_handoff(sorter_scheduler_t *s, sorter_package_track_t *p)
{
    sorter_motor_direction_t dir = (p->cls == SORTER_CLASS_1 || p->cls == SORTER_CLASS_FREE || p->cls == SORTER_CLASS_VISION_FAILED) ? SORTER_MOTOR_FORWARD : SORTER_MOTOR_REVERSE;
    p->belt = SORTER_BELT_B; p->pos_mm = 0; p->handoff_ready_ms = 0;
    transition_state(s, p, dir == SORTER_MOTOR_FORWARD ? SORTER_STATE_ON_B_TO_CLASS1 : SORTER_STATE_ON_B_TO_S4);
    emit_motor(s, 2, dir, s->config.motor_b_speed_percent, false);
}

static void complete_s4_handoff(sorter_scheduler_t *s, sorter_package_track_t *p)
{
    sorter_motor_direction_t dir = p->cls == SORTER_CLASS_2 ? SORTER_MOTOR_REVERSE : SORTER_MOTOR_FORWARD;
    p->belt = SORTER_BELT_C; p->pos_mm = 0; p->handoff_ready_ms = 0; p->c_start_dist_mm = 0;
    p->c_start_ms = now_ms(); p->c_busy_until_ms = p->c_start_ms + s->config.c_min_busy_ms;
    p->c_fallback_until_ms = p->c_start_ms + s->config.c_fallback_busy_ms; p->c_distance_started = false;
    transition_state(s, p, SORTER_STATE_ON_C_EXIT);
    if (s->b_owner == p->id) { s->b_owner = 0; s->b_direction = SORTER_MOTOR_STOP; }
    emit_motor(s, 2, SORTER_MOTOR_STOP, 0, false);
    emit_motor(s, 3, dir, s->config.motor_c_speed_percent, false);
}

static sorter_package_track_t *first_filtered(sorter_scheduler_t *s, sorter_package_state_t state, int mode)
{
    sorter_package_track_t *best = NULL;
    for (int i = 0; i < SORTER_MAX_PACKAGES; ++i) {
        sorter_package_track_t *p = &s->tracks[i];
        if (!p->occupied || p->state != state) continue;
        if (mode == 1 && p->handoff_ready_ms == 0) continue;
        if (mode == 2 && (p->handoff_ready_ms != 0 || (state == SORTER_STATE_HOLDING_AT_S2 ? s->b_owner : s->c_owner) != p->id)) continue;
        if (!best || p->id < best->id) best = p;
    }
    return best;
}

static void service_pending_transfers(sorter_scheduler_t *s)
{
    int64_t now = now_ms();
    sorter_package_track_t *p = first_filtered(s, SORTER_STATE_HOLDING_AT_S4, 1);
    if (p && now >= p->handoff_ready_ms) { complete_s4_handoff(s, p); emit_status(s, "s4_released"); }
    p = first_filtered(s, SORTER_STATE_HOLDING_AT_S2, 1);
    if (p && now >= p->handoff_ready_ms) { complete_s2_handoff(s, p); emit_status(s, "s2_released"); }
    p = s->s4_held_package_id > 0 ? package_in_state(s, s->s4_held_package_id, SORTER_STATE_HOLDING_AT_S4) : first_in_state(s, SORTER_STATE_HOLDING_AT_S4);
    if (p && p->handoff_ready_ms == 0 && s->c_owner != p->id) { if (release_s4_to_c(s, p)) emit_status(s, "b_to_c"); } else if (!p) s->s4_held_package_id = 0;
    p = s->s2_held_package_id > 0 ? package_in_state(s, s->s2_held_package_id, SORTER_STATE_HOLDING_AT_S2) : first_in_state(s, SORTER_STATE_HOLDING_AT_S2);
    if (p && p->handoff_ready_ms == 0 && s->b_owner != p->id) { if (release_s2_to_b(s, p)) emit_status(s, "a_to_b"); } else if (!p) s->s2_held_package_id = 0;
}

static void release_package(sorter_scheduler_t *s, sorter_package_track_t *p, sorter_package_state_t final_state)
{
    int id = p->id;
    if (s->s2_held_package_id == id) s->s2_held_package_id = 0;
    if (s->s4_held_package_id == id) s->s4_held_package_id = 0;
    transition_state(s, p, final_state); emit_package(s, p); release_reservations(s, id); memset(p, 0, sizeof(*p));
    if (sorter_scheduler_active_count(s) == 0) {
        emit_motor(s, 2, SORTER_MOTOR_STOP, 0, false); emit_motor(s, 3, SORTER_MOTOR_STOP, 0, false);
        memset(&s->last_commands[0], 0, sizeof(s->last_commands[0]));
    }
}

static void complete_c_package(sorter_scheduler_t *s, sorter_package_track_t *p)
{
    if (s->b_owner == p->id) emit_motor(s, 2, SORTER_MOTOR_STOP, 0, false);
    emit_motor(s, 3, SORTER_MOTOR_STOP, 0, false); release_reservations(s, p->id);
    sorter_package_class_t cls = p->cls; release_package(s, p, SORTER_STATE_DONE);
    emit_status(s, cls == SORTER_CLASS_2 ? "class2_done" : "class3_done"); service_pending_transfers(s); schedule(s);
}

void sorter_scheduler_package_new(sorter_scheduler_t *s, int id)
{
    if (s->estop) { emit_status(s, "ignored_estop"); return; }
    if (find_package(s, id)) { emit_fault(s, "duplicate_package", id, 0); return; }
    sorter_package_track_t *p = allocate_package(s, id);
    if (!p) { emit_fault(s, "queue_full", id, 0); return; }
    p->belt = SORTER_BELT_A; transition_state(s, p, SORTER_STATE_WAITING_VISION); emit_status(s, "package_new");
    while (service_state_timeouts(s)) {} schedule(s);
}

void sorter_scheduler_vision_result(sorter_scheduler_t *s, int id, sorter_package_class_t cls)
{
    if (s->estop) { emit_status(s, "ignored_estop"); return; }
    sorter_package_track_t *p = find_package(s, id);
    if (!p || p->state != SORTER_STATE_WAITING_VISION) { emit_fault(s, "vision_without_package", id, 0); return; }
    p->cls = cls;
    if (cls == SORTER_CLASS_ERROR) { release_package(s, p, SORTER_STATE_ERROR); emit_status(s, "vision_error"); return; }
    transition_state(s, p, SORTER_STATE_WAITING_AB); emit_status(s, "vision");
    while (service_state_timeouts(s)) {} schedule(s);
}

void sorter_scheduler_sensor(sorter_scheduler_t *s, int sid, bool active, int pid)
{
    if (s->estop) return;
    while (service_state_timeouts(s)) {}
    sorter_package_track_t *p = NULL;
    if (sid == 1) { if (active) emit_status(s, "sensor1"); return; }
    if (sid == 2) {
        if (!active) {
            s->s2_sensor_active = false; p = package_in_state(s, pid, SORTER_STATE_HOLDING_AT_S2);
            if (!p && pid <= 0) p = first_filtered(s, SORTER_STATE_HOLDING_AT_S2, 2);
            if (p && p->handoff_ready_ms == 0 && s->b_owner == p->id) {
                p->handoff_ready_ms = now_ms() + s->config.handoff_delay_ms; emit_status(s, "s2_clear_delay"); service_pending_transfers(s); schedule(s);
            }
            return;
        }
        if (pid <= 0 && s->s2_sensor_active && !first_in_state(s, SORTER_STATE_HOLDING_AT_S2)) return;
        s->s2_sensor_active = true; p = package_in_state(s, pid, SORTER_STATE_WAITING_AB);
        if (!p) p = package_in_state(s, pid, SORTER_STATE_HOLDING_AT_S2);
        if (!p && pid > 0 && (package_in_state(s, pid, SORTER_STATE_ON_B_TO_CLASS1) || package_in_state(s, pid, SORTER_STATE_ON_B_TO_S4))) return;
        if (!p && pid <= 0) { p = first_in_state(s, SORTER_STATE_WAITING_AB); if (!p) p = first_in_state(s, SORTER_STATE_HOLDING_AT_S2); }
        if (!p) { emit_fault(s, pid > 0 ? "sensor_missed" : "sensor_without_package", pid, sid); return; }
        if (p->state == SORTER_STATE_HOLDING_AT_S2 && p->handoff_ready_ms > 0) return;
        if (p->state == SORTER_STATE_WAITING_AB) transition_state(s, p, SORTER_STATE_HOLDING_AT_S2);
        p->pos_mm = s->config.a_start_to_vision_mm + s->config.a_vision_to_b_mm; emit_motor(s, 1, SORTER_MOTOR_STOP, 0, false);
        if (!release_s2_to_b(s, p)) { s->s2_held_package_id = p->id; emit_status(s, "s2_held"); return; }
        emit_status(s, "a_to_b"); return;
    }
    if (sid == 3) {
        if (!active) return;
        p = package_in_state(s, pid, SORTER_STATE_ON_B_TO_CLASS1);
        if (!p && pid <= 0) p = first_in_state(s, SORTER_STATE_ON_B_TO_CLASS1);
        if (!p) { emit_fault(s, pid > 0 ? "sensor_missed" : "sensor_without_package", pid, sid); return; }
        emit_motor(s, 2, SORTER_MOTOR_STOP, 0, false); release_reservations(s, p->id); release_package(s, p, SORTER_STATE_DONE);
        emit_status(s, "class1_done"); service_pending_transfers(s); memset(&s->last_commands[0], 0, sizeof(s->last_commands[0])); schedule(s); return;
    }
    if (sid == 4) {
        if (!active) {
            s->s4_sensor_active = false; p = package_in_state(s, pid, SORTER_STATE_HOLDING_AT_S4);
            if (!p && pid <= 0) p = first_filtered(s, SORTER_STATE_HOLDING_AT_S4, 2);
            if (p && p->handoff_ready_ms == 0 && s->c_owner == p->id) {
                p->handoff_ready_ms = now_ms() + s->config.handoff_delay_ms; emit_status(s, "s4_clear_delay"); service_pending_transfers(s); schedule(s);
            }
            return;
        }
        if (pid <= 0 && s->s4_sensor_active && !first_in_state(s, SORTER_STATE_HOLDING_AT_S4)) return;
        s->s4_sensor_active = true; p = package_in_state(s, pid, SORTER_STATE_ON_B_TO_S4);
        if (!p) p = package_in_state(s, pid, SORTER_STATE_HOLDING_AT_S4);
        if (!p && pid > 0 && package_in_state(s, pid, SORTER_STATE_ON_C_EXIT)) return;
        if (!p && pid <= 0) { p = first_in_state(s, SORTER_STATE_ON_B_TO_S4); if (!p) p = first_in_state(s, SORTER_STATE_HOLDING_AT_S4); }
        if (!p) { emit_fault(s, pid > 0 ? "sensor_missed" : "sensor_without_package", pid, sid); return; }
        if (p->state == SORTER_STATE_HOLDING_AT_S4 && p->handoff_ready_ms > 0) return;
        if (p->state == SORTER_STATE_ON_B_TO_S4) transition_state(s, p, SORTER_STATE_HOLDING_AT_S4);
        emit_motor(s, 2, SORTER_MOTOR_STOP, 0, false);
        if (!release_s4_to_c(s, p)) { s->s4_held_package_id = p->id; emit_status(s, "s4_held"); service_pending_transfers(s); return; }
        emit_status(s, "b_to_c"); service_pending_transfers(s); return;
    }
    emit_fault(s, "sensor_without_package", 0, sid);
}

void sorter_scheduler_distance(sorter_scheduler_t *s, int motor_id, float distance)
{
    if (s->estop || motor_id != 3) return;
    while (service_state_timeouts(s)) {}
    sorter_package_track_t *p = first_in_state(s, SORTER_STATE_ON_C_EXIT);
    if (!p) return;
    if (!p->c_distance_started) { p->c_start_dist_mm = distance; p->c_distance_started = true; return; }
    if (distance - p->c_start_dist_mm >= s->config.c_exit_distance_mm && now_ms() >= p->c_busy_until_ms) complete_c_package(s, p);
}

void sorter_scheduler_estop(sorter_scheduler_t *s, bool enabled)
{
    s->estop = enabled;
    if (enabled) {
        s->paused_after_estop = true;
        emit_motor(s, 1, SORTER_MOTOR_BRAKE, 0, false); emit_motor(s, 2, SORTER_MOTOR_BRAKE, 0, false); emit_motor(s, 3, SORTER_MOTOR_BRAKE, 0, false); emit_status(s, "estop"); return;
    }
    s->paused_after_estop = false;
    emit_motor(s, 1, SORTER_MOTOR_STOP, 0, false); emit_motor(s, 2, SORTER_MOTOR_STOP, 0, false); emit_motor(s, 3, SORTER_MOTOR_STOP, 0, false); emit_status(s, "estop_release");
    while (service_state_timeouts(s)) {} schedule(s);
}

bool sorter_scheduler_has_s2_candidate_before(const sorter_scheduler_t *s, int id)
{
    for (int i = 0; i < SORTER_MAX_PACKAGES; ++i) {
        const sorter_package_track_t *p = &s->tracks[i];
        if (p->occupied && (p->state == SORTER_STATE_WAITING_AB || p->state == SORTER_STATE_HOLDING_AT_S2) && (id <= 0 || p->id < id)) return true;
    }
    return false;
}

static bool service_state_timeouts(sorter_scheduler_t *s)
{
    if (s->estop || s->paused_after_estop) return false;
    int64_t now = now_ms();
    for (int i = 0; i < SORTER_MAX_PACKAGES; ++i) {
        sorter_package_track_t *p = &s->tracks[i];
        if (p->occupied && p->state == SORTER_STATE_ON_C_EXIT && p->c_fallback_until_ms > 0 && now >= p->c_fallback_until_ms) {
            emit_status(s, "timeout_c_exit"); complete_c_package(s, p); return true;
        }
        if (!p->occupied || p->state_timeout_ms == 0) continue;
        bool running = true;
        if (p->state == SORTER_STATE_WAITING_VISION || p->state == SORTER_STATE_WAITING_AB) running = motor_running(s, 1);
        else if (p->state == SORTER_STATE_HOLDING_AT_S2) running = s->b_owner == p->id && motor_running(s, 1);
        else if (p->state == SORTER_STATE_ON_B_TO_CLASS1 || p->state == SORTER_STATE_ON_B_TO_S4) running = s->b_owner == p->id && motor_running(s, 2);
        else if (p->state == SORTER_STATE_HOLDING_AT_S4) running = s->c_owner == p->id && motor_running(s, 2);
        if (!running) { p->state_enter_ms = now; continue; }
        if (now - p->state_enter_ms < (int64_t)p->state_timeout_ms) continue;
        switch (p->state) {
        case SORTER_STATE_WAITING_VISION:
            p->cls = SORTER_CLASS_VISION_FAILED; transition_state(s, p, SORTER_STATE_WAITING_AB); emit_status(s, "timeout_vision"); return true;
        case SORTER_STATE_WAITING_AB:
            transition_state(s, p, SORTER_STATE_HOLDING_AT_S2); p->pos_mm = s->config.a_start_to_vision_mm + s->config.a_vision_to_b_mm; emit_motor(s, 1, SORTER_MOTOR_STOP, 0, false);
            if (release_s2_to_b(s, p)) { transition_state(s, p, SORTER_STATE_HOLDING_AT_S2); emit_status(s, "timeout_a_to_b"); }
            else { s->s2_held_package_id = p->id; emit_status(s, "timeout_s2_held"); } return true;
        case SORTER_STATE_HOLDING_AT_S2:
            if (p->handoff_ready_ms > 0 && now >= p->handoff_ready_ms) { complete_s2_handoff(s, p); emit_status(s, "s2_released"); return true; }
            if (p->handoff_ready_ms == 0 && s->b_owner == p->id) { s->s2_sensor_active = false; complete_s2_handoff(s, p); emit_status(s, "timeout_s2_released"); return true; }
            if (p->handoff_ready_ms == 0 && release_s2_to_b(s, p)) { transition_state(s, p, SORTER_STATE_HOLDING_AT_S2); emit_status(s, "timeout_a_to_b"); return true; }
            s->s2_held_package_id = p->id; transition_state(s, p, SORTER_STATE_HOLDING_AT_S2); emit_status(s, "timeout_s2_held"); return true;
        case SORTER_STATE_ON_B_TO_CLASS1:
            emit_motor(s, 2, SORTER_MOTOR_STOP, 0, false); release_reservations(s, p->id); emit_status(s, "timeout_class1"); release_package(s, p, SORTER_STATE_DONE); emit_status(s, "class1_done"); service_pending_transfers(s); return true;
        case SORTER_STATE_ON_B_TO_S4:
            transition_state(s, p, SORTER_STATE_HOLDING_AT_S4); emit_motor(s, 2, SORTER_MOTOR_STOP, 0, false);
            if (release_s4_to_c(s, p)) { s->s4_sensor_active = false; complete_s4_handoff(s, p); emit_status(s, "timeout_b_to_c"); service_pending_transfers(s); }
            else { s->s4_held_package_id = p->id; emit_status(s, "timeout_s4_held"); service_pending_transfers(s); } return true;
        case SORTER_STATE_HOLDING_AT_S4:
            if (p->handoff_ready_ms > 0 && now >= p->handoff_ready_ms) { complete_s4_handoff(s, p); emit_status(s, "s4_released"); service_pending_transfers(s); return true; }
            if (p->handoff_ready_ms == 0 && s->c_owner == p->id) { s->s4_sensor_active = false; complete_s4_handoff(s, p); emit_status(s, "timeout_s4_released"); service_pending_transfers(s); return true; }
            if (p->handoff_ready_ms == 0 && release_s4_to_c(s, p)) { s->s4_sensor_active = false; complete_s4_handoff(s, p); emit_status(s, "timeout_b_to_c"); service_pending_transfers(s); return true; }
            s->s4_held_package_id = p->id; transition_state(s, p, SORTER_STATE_HOLDING_AT_S4); emit_status(s, "timeout_s4_held"); return true;
        case SORTER_STATE_ON_C_EXIT: emit_status(s, "timeout_c_exit"); complete_c_package(s, p); return true;
        default: break;
        }
    }
    return false;
}

static void schedule(sorter_scheduler_t *s)
{
    if (s->estop || s->paused_after_estop) return;
    service_pending_transfers(s);
    bool should_run = !s2_blocks_a(s), has_package = false, movable = false;
    if (should_run) {
        for (int i = 0; i < SORTER_MAX_PACKAGES; ++i) {
            sorter_package_track_t *p = &s->tracks[i];
            if (!p->occupied || p->belt != SORTER_BELT_A) continue;
            has_package = true;
            if ((p->state == SORTER_STATE_WAITING_VISION || p->state == SORTER_STATE_WAITING_AB) && can_move_a_package(s, p)) movable = true;
        }
        should_run = !has_package || movable;
    }
    emit_motor(s, 1, should_run ? SORTER_MOTOR_FORWARD : SORTER_MOTOR_STOP, should_run ? s->config.motor_a_speed_percent : 0, false);
    emit_status(s, "tick");
    for (int i = 0; i < SORTER_MAX_PACKAGES; ++i) if (s->tracks[i].occupied) emit_package(s, &s->tracks[i]);
}

void sorter_scheduler_tick(sorter_scheduler_t *s)
{
    if (!s->estop && !s->paused_after_estop) { while (service_state_timeouts(s)) {} service_pending_transfers(s); schedule(s); }
}

sorter_package_class_t sorter_package_class_from_int(int value, bool free_flag, bool error_flag)
{
    if (free_flag) return SORTER_CLASS_FREE;
    if (error_flag) return SORTER_CLASS_ERROR;
    if (value == 0) return SORTER_CLASS_UNKNOWN;
    if (value == 1) return SORTER_CLASS_1;
    if (value == 2) return SORTER_CLASS_2;
    if (value == 3) return SORTER_CLASS_3;
    return SORTER_CLASS_ERROR;
}
