#include "sorter_core/sorter_protocol.h"

#include <stdio.h>

static const char *class_name(sorter_package_class_t v)
{
    switch (v) {
    case SORTER_CLASS_1: return "class1";
    case SORTER_CLASS_2: return "class2";
    case SORTER_CLASS_3: return "class3";
    case SORTER_CLASS_FREE: return "free";
    case SORTER_CLASS_ERROR: return "error";
    case SORTER_CLASS_VISION_FAILED: return "vision_failed";
    default: return "unknown";
    }
}

static const char *state_name(sorter_package_state_t v)
{
    switch (v) {
    case SORTER_STATE_WAITING_VISION: return "waiting_vision";
    case SORTER_STATE_WAITING_AB: return "waiting_ab";
    case SORTER_STATE_HOLDING_AT_S2: return "holding_at_s2";
    case SORTER_STATE_ON_B_TO_CLASS1: return "on_b_to_class1";
    case SORTER_STATE_ON_B_TO_S4: return "on_b_to_s4";
    case SORTER_STATE_HOLDING_AT_S4: return "holding_at_s4";
    case SORTER_STATE_ON_C_EXIT: return "on_c_exit";
    case SORTER_STATE_DONE: return "done";
    case SORTER_STATE_ERROR: return "error";
    default: return "empty";
    }
}

static const char *belt_name(sorter_belt_t v)
{
    return v == SORTER_BELT_A ? "A" : v == SORTER_BELT_B ? "B" : v == SORTER_BELT_C ? "C" : "?";
}

static const char *direction_name(sorter_motor_direction_t v)
{
    return v == SORTER_MOTOR_FORWARD ? "forward" :
           v == SORTER_MOTOR_REVERSE ? "reverse" :
           v == SORTER_MOTOR_BRAKE ? "brake" : "stop";
}

int sorter_protocol_format_event(const sorter_event_t *event, char *buf, size_t len)
{
    if (!event || !buf || len == 0) {
        return -1;
    }
    switch (event->type) {
    case SORTER_EVENT_MOTOR:
        return snprintf(buf, len, "MOTOR,id=%d,dir=%s,speed=%d",
                        event->motor.motor_id,
                        direction_name(event->motor.direction),
                        event->motor.speed_percent);
    case SORTER_EVENT_STATUS:
        return snprintf(buf, len, "STATUS,packages=%u,state=%s,reason=%s",
                        (unsigned)event->status.active_count,
                        event->status.scheduler_state ? event->status.scheduler_state : "running",
                        event->status.reason ? event->status.reason : "");
    case SORTER_EVENT_PACKAGE:
        return snprintf(buf, len, "PKG,id=%d,belt=%s,pos=%.1f,state=%s,class=%s",
                        event->package.id,
                        belt_name(event->package.belt),
                        (double)event->package.pos_mm,
                        state_name(event->package.state),
                        class_name(event->package.cls));
    case SORTER_EVENT_FAULT:
        return snprintf(buf, len, "FAULT,code=%s,package=%d,sensor=%d",
                        event->fault.code ? event->fault.code : "",
                        event->fault.package_id,
                        event->fault.sensor_id);
    default:
        return -1;
    }
}
