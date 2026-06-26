/**
 *
 * This file is created and owned by anyui.
 *
 * Version: 1.0.0
 *
 * COPYRIGHT 2026 anyui Team
 * All rights reserved.
 *
 * https://anyui.tech/
 *
 * Author: anyui Team
 */

#include "setup_ui.h"
#include "sorting_sim_control.h"
#include <stdint.h>
#include <stdio.h>


lv_obj_t * scr_dashboard = NULL;
lv_obj_t * scr_dashboard_cont_background1 = NULL;
lv_obj_t * scr_dashboard_cont_background2 = NULL;
lv_obj_t * scr_dashboard_cont_log = NULL;
lv_obj_t * scr_dashboard_cont_event_details = NULL;
lv_obj_t * scr_dashboard_label_title_event = NULL;
lv_obj_t * scr_dashboard_label_detail_result = NULL;
lv_obj_t * scr_dashboard_label_detail_event = NULL;
lv_obj_t * scr_dashboard_label_detail_confidence = NULL;
lv_obj_t * scr_dashboard_label_detail_time = NULL;
lv_obj_t * scr_dashboard_label_title_inference = NULL;
lv_obj_t * scr_dashboard_label_detail_infer_time = NULL;
lv_obj_t * scr_dashboard_label_title_result = NULL;
lv_obj_t * scr_dashboard_label_detail_status = NULL;
lv_obj_t * scr_dashboard_label_title_type = NULL;
lv_obj_t * scr_dashboard_label_title_inft = NULL;
lv_obj_t * scr_dashboard_label_title_time = NULL;
lv_obj_t * scr_dashboard_label_title_status = NULL;
lv_obj_t * scr_dashboard_cont_under2 = NULL;
lv_obj_t * scr_dashboard_cont_detail_badge = NULL;
lv_obj_t * scr_dashboard_label_detail_badge = NULL;
lv_obj_t * scr_dashboard_cont_note = NULL;
lv_obj_t * scr_dashboard_label_title_note = NULL;
lv_obj_t * scr_dashboard_cont_note_line = NULL;
lv_obj_t * scr_dashboard_label_detail_note = NULL;
lv_obj_t * scr_dashboard_cont_log_record = NULL;
lv_obj_t * scr_dashboard_cont_log_06 = NULL;
lv_obj_t * scr_dashboard_label_time_06 = NULL;
lv_obj_t * scr_dashboard_label_type_06 = NULL;
lv_obj_t * scr_dashboard_label_result_06 = NULL;
lv_obj_t * scr_dashboard_label_status_06 = NULL;
lv_obj_t * scr_dashboard_cont_log_05 = NULL;
lv_obj_t * scr_dashboard_label_time_05 = NULL;
lv_obj_t * scr_dashboard_label_type_05 = NULL;
lv_obj_t * scr_dashboard_label_result_05 = NULL;
lv_obj_t * scr_dashboard_label_status_05 = NULL;
lv_obj_t * scr_dashboard_cont_log_04 = NULL;
lv_obj_t * scr_dashboard_label_time_04 = NULL;
lv_obj_t * scr_dashboard_label_type_04 = NULL;
lv_obj_t * scr_dashboard_label_result_04 = NULL;
lv_obj_t * scr_dashboard_label_status_04 = NULL;
lv_obj_t * scr_dashboard_cont_log_03 = NULL;
lv_obj_t * scr_dashboard_label_time_03 = NULL;
lv_obj_t * scr_dashboard_label_type_03 = NULL;
lv_obj_t * scr_dashboard_label_result_03 = NULL;
lv_obj_t * scr_dashboard_label_status_03 = NULL;
lv_obj_t * scr_dashboard_cont_log_02 = NULL;
lv_obj_t * scr_dashboard_label_time_02 = NULL;
lv_obj_t * scr_dashboard_label_type_02 = NULL;
lv_obj_t * scr_dashboard_label_result_02 = NULL;
lv_obj_t * scr_dashboard_label_status_02 = NULL;
lv_obj_t * scr_dashboard_cont_log_01 = NULL;
lv_obj_t * scr_dashboard_label_time_01 = NULL;
lv_obj_t * scr_dashboard_label_type_01 = NULL;
lv_obj_t * scr_dashboard_label_result_01 = NULL;
lv_obj_t * scr_dashboard_lable_status_01 = NULL;
lv_obj_t * scr_dashboard_label_text_log = NULL;
lv_obj_t * scr_dashboard_cont_under1 = NULL;
lv_obj_t * scr_dashboard_cont_log_line = NULL;
lv_obj_t * scr_dashboard_cont_set = NULL;
lv_obj_t * scr_dashboard_cont_sys_debug = NULL;
lv_obj_t * scr_dashboard_cont_bright = NULL;
lv_obj_t * scr_dashboard_slider_bright = NULL;
lv_obj_t * scr_dashboard_label_bright = NULL;
lv_obj_t * scr_dashboard_cont_set_line = NULL;
lv_obj_t * scr_dashboard_cont_button = NULL;
lv_obj_t * scr_dashboard_imgbtn_dash = NULL;
lv_obj_t * scr_dashboard_imgbtn_dash_label = NULL;
lv_obj_t * scr_dashboard_imgbtn_set = NULL;
lv_obj_t * scr_dashboard_imgbtn_set_label = NULL;
lv_obj_t * scr_dashboard_imgbtn_log = NULL;
lv_obj_t * scr_dashboard_imgbtn_log_label = NULL;
lv_obj_t * scr_dashboard_imgbtn_sys = NULL;
lv_obj_t * scr_dashboard_imgbtn_sys_label = NULL;
lv_obj_t * scr_dashboard_cont_dashboard = NULL;
lv_obj_t * scr_dashboard_cont_container_live = NULL;
lv_obj_t * scr_dashboard_cont_live_vedio = NULL;
lv_obj_t * scr_dashboard_label_text_live = NULL;
lv_obj_t * scr_dashboard_cont_detection = NULL;
lv_obj_t * scr_dashboard_slider_runtime_detection_score = NULL;
lv_obj_t * scr_dashboard_label_text_detection = NULL;
lv_obj_t * scr_dashboard_label_runtime_detection_status = NULL;
lv_obj_t * scr_dashboard_label_runtime_detection_score = NULL;
lv_obj_t * scr_dashboard_cont_9NI8FzEe = NULL;
lv_obj_t * scr_dashboard_btn_4m5N949i = NULL;
lv_obj_t * scr_dashboard_btn_4m5N949i_label = NULL;
lv_obj_t * scr_dashboard_img_lFJKr46M = NULL;
lv_obj_t * scr_dashboard_label_shIaNFSV = NULL;
lv_obj_t * scr_dashboard_cont_data = NULL;
lv_obj_t * scr_dashboard_cont_02 = NULL;
lv_obj_t * scr_dashboard_label_extext_rpm = NULL;
lv_obj_t * scr_dashboard_label_runtime_motor_rpm = NULL;
lv_obj_t * scr_dashboard_label_text02 = NULL;
lv_obj_t * scr_dashboard_cont_01 = NULL;
lv_obj_t * scr_dashboard_label_ms = NULL;
lv_obj_t * scr_dashboard_label_runtime_infer_time = NULL;
lv_obj_t * scr_dashboard_label_text01 = NULL;
lv_obj_t * scr_dashboard_cont_03 = NULL;
lv_obj_t * scr_dashboard_label_runtime_temperature = NULL;
lv_obj_t * scr_dashboard_label_text03 = NULL;
lv_obj_t * scr_dashboard_cont_04 = NULL;
lv_obj_t * scr_dashboard_label_runtime_current_type = NULL;
lv_obj_t * scr_dashboard_label_text04 = NULL;
lv_obj_t * scr_dashboard_cont_05 = NULL;
lv_obj_t * scr_dashboard_label_runtime_total_count = NULL;
lv_obj_t * scr_dashboard_label_text05 = NULL;
lv_obj_t * scr_dashboard_cont_06 = NULL;
lv_obj_t * scr_dashboard_label_extext_fps = NULL;
lv_obj_t * scr_dashboard_label_runtime_fps = NULL;
lv_obj_t * scr_dashboard_label_text06 = NULL;
lv_obj_t * scr_dashboard_cont_circle_data = NULL;
lv_obj_t * scr_dashboard_cont_confidence = NULL;
lv_obj_t * scr_dashboard_label_text_confidence = NULL;
lv_obj_t * scr_dashboard_label_runtime_success_rate = NULL;
lv_obj_t * scr_dashboard_cont_cpu = NULL;
lv_obj_t * scr_dashboard_label_text_cpu = NULL;
lv_obj_t * scr_dashboard_label_runtime_cpu_usage = NULL;
lv_obj_t * scr_dashboard_cont_memory = NULL;
lv_obj_t * scr_dashboard_label_text_memory = NULL;
lv_obj_t * scr_dashboard_label_runtime_memory_usage = NULL;
lv_obj_t * scr_dashboard_img_title = NULL;
lv_obj_t * scr_dashboard_cont_esp32 = NULL;
lv_obj_t * scr_dashboard_label_esp32 = NULL;
lv_obj_t * scr_dashboard_cont_edge = NULL;
lv_obj_t * scr_dashboard_label_text_edge = NULL;
lv_obj_t * scr_dashboard_led_edge = NULL;
static event_table_t scr_dashboard_event_table = {0};
static lv_obj_t * scr_dashboard_arc_runtime_success_rate = NULL;
static lv_obj_t * scr_dashboard_arc_runtime_success_rate_label = NULL;
static lv_obj_t * scr_dashboard_arc_runtime_cpu_usage = NULL;
static lv_obj_t * scr_dashboard_arc_runtime_cpu_usage_label = NULL;
static lv_obj_t * scr_dashboard_arc_runtime_memory_usage = NULL;
static lv_obj_t * scr_dashboard_arc_runtime_memory_usage_label = NULL;
static lv_obj_t * scr_dashboard_debug_panel = NULL;
static lv_obj_t * scr_dashboard_debug_label_mode = NULL;
static lv_obj_t * scr_dashboard_debug_label_motor = NULL;
static lv_obj_t * scr_dashboard_debug_label_sensor = NULL;
static lv_obj_t * scr_dashboard_debug_label_speed = NULL;
static lv_obj_t * scr_dashboard_debug_label_delay = NULL;
static lv_obj_t * scr_dashboard_debug_label_timeout = NULL;
static lv_obj_t * scr_dashboard_debug_label_hw_sensors = NULL;
static lv_obj_t * scr_dashboard_debug_label_enc_a = NULL;
static lv_obj_t * scr_dashboard_debug_label_enc_b = NULL;
static lv_obj_t * scr_dashboard_debug_label_enc_c = NULL;
static lv_obj_t * scr_dashboard_debug_label_mtest = NULL;
static lv_timer_t * scr_dashboard_debug_timer = NULL;
static sorting_debug_settings_t s_dashboard_debug_settings;
static void register_sys_events(event_table_t *table);
static void init_states(void);
static void scr_dashboard_imgbtn_sys_event_handler(lv_event_t * e);
static void scr_dashboard_imgbtn_log_event_handler(lv_event_t * e);
static void scr_dashboard_imgbtn_set_event_handler(lv_event_t * e);
static void scr_dashboard_imgbtn_dash_event_handler(lv_event_t * e);
static void scr_dashboard_cont_log_01_event_handler(lv_event_t * e);
static void scr_dashboard_cont_log_02_event_handler(lv_event_t * e);
static void scr_dashboard_cont_log_03_event_handler(lv_event_t * e);
static void scr_dashboard_cont_log_04_event_handler(lv_event_t * e);
static void scr_dashboard_cont_log_05_event_handler(lv_event_t * e);
static void scr_dashboard_cont_log_06_event_handler(lv_event_t * e);
static void register_ui_events(void);
static void statebtn_device_change_state(uint8_t value);
static void scr_dashboard_statebtn_device_event_handler(lv_event_t * e);
static void dashboard_debug_refresh(void);
static void dashboard_debug_mode_event_handler(lv_event_t * e);
static void dashboard_debug_motor_event_handler(lv_event_t * e);
static void dashboard_debug_sensor_event_handler(lv_event_t * e);
static void dashboard_debug_adjust_event_handler(lv_event_t * e);
static void dashboard_debug_sim_event_handler(lv_event_t * e);
static void dashboard_debug_motor_test_event_handler(lv_event_t * e);
static void dashboard_debug_encoder_clear_event_handler(lv_event_t * e);
static void dashboard_debug_timer_cb(lv_timer_t *timer);
static void create_dashboard_debug_panel(lv_obj_t *parent);
static lv_obj_t * create_ui(void);


static void register_sys_events(event_table_t *table) {
}
static void init_states(void) {
    extern lv_obj_t * global_statusbar;
    set_current_event_table(&scr_dashboard_event_table);
}
static void scr_dashboard_imgbtn_sys_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED: {
        ui_flag_modify(scr_dashboard_cont_dashboard, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_set, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_log, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_sys_debug, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_REMOVE);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_sys, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &sys_a_212x66, NULL);
        ui_state_modify(scr_dashboard_imgbtn_sys, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_log, LV_IMGBTN_STATE_RELEASED, NULL, &logc_212x66, NULL);
        ui_state_modify(scr_dashboard_imgbtn_log, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_set, LV_IMGBTN_STATE_RELEASED, NULL, &setc_212x66, NULL);
        ui_state_modify(scr_dashboard_imgbtn_set, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_dash, LV_IMGBTN_STATE_RELEASED, NULL, &dash_c_212x66, NULL);
        ui_state_modify(scr_dashboard_imgbtn_dash, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        break;
    }
    default: {
        break;
    }
    }
}
static void scr_dashboard_imgbtn_log_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED: {
        ui_flag_modify(scr_dashboard_cont_log, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_set, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_dashboard, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_sys_debug, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_sys, LV_IMGBTN_STATE_RELEASED, NULL, &sysc_212x66, NULL);
        ui_state_modify(scr_dashboard_imgbtn_sys, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_log, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &log_a_212x66, NULL);
        ui_state_modify(scr_dashboard_imgbtn_log, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_set, LV_IMGBTN_STATE_RELEASED, NULL, &setc_212x66, NULL);
        ui_state_modify(scr_dashboard_imgbtn_set, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_dash, LV_IMGBTN_STATE_RELEASED, NULL, &dash_c_212x66, NULL);
        ui_state_modify(scr_dashboard_imgbtn_dash, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        break;
    }
    default: {
        break;
    }
    }
}
static void scr_dashboard_imgbtn_set_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED: {
        ui_flag_modify(scr_dashboard_cont_log, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_set, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_dashboard, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_sys_debug, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_sys, LV_IMGBTN_STATE_RELEASED, NULL, &sysc_212x66, NULL);
        ui_state_modify(scr_dashboard_imgbtn_sys, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_log, LV_IMGBTN_STATE_RELEASED, NULL, &logc_212x66, NULL);
        ui_state_modify(scr_dashboard_imgbtn_log, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_set, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &set_a_212x66, NULL);
        ui_state_modify(scr_dashboard_imgbtn_set, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_dash, LV_IMGBTN_STATE_RELEASED, NULL, &dash_c_212x66, NULL);
        ui_state_modify(scr_dashboard_imgbtn_dash, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        break;
    }
    default: {
        break;
    }
    }
}
static void scr_dashboard_imgbtn_dash_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED: {
        ui_flag_modify(scr_dashboard_cont_dashboard, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_set, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_log, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_sys_debug, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_dash, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &dash_a_212x66, NULL);
        ui_state_modify(scr_dashboard_imgbtn_dash, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_sys, LV_IMGBTN_STATE_RELEASED, NULL, &sysc_212x66, NULL);
        ui_state_modify(scr_dashboard_imgbtn_sys, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_log, LV_IMGBTN_STATE_RELEASED, NULL, &logc_212x66, NULL);
        ui_state_modify(scr_dashboard_imgbtn_log, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_set, LV_IMGBTN_STATE_RELEASED, NULL, &setc_212x66, NULL);
        ui_state_modify(scr_dashboard_imgbtn_set, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        break;
    }
    default: {
        break;
    }
    }
}
static void scr_dashboard_cont_log_01_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED: {
        lv_label_set_text(scr_dashboard_label_detail_time, "09:42:18");
        lv_label_set_text(scr_dashboard_label_detail_event, "分类识别");
        lv_label_set_text(scr_dashboard_label_detail_result, "韵达 中");
        lv_label_set_text(scr_dashboard_label_detail_confidence, "96%");
        lv_label_set_text(scr_dashboard_label_detail_infer_time, "72ms");
        lv_label_set_text(scr_dashboard_label_detail_status, "成功");
        lv_label_set_text(scr_dashboard_label_detail_note, "韵达包裹分类成功。");
        lv_label_set_text(scr_dashboard_label_detail_badge, "Success");
        ui_state_modify(scr_dashboard_cont_detail_badge, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_label_detail_badge, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_01, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
        ui_state_modify(scr_dashboard_cont_log_02, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_03, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_04, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_05, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_06, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        break;
    }
    default: {
        break;
    }
    }
}
static void scr_dashboard_cont_log_02_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED: {
        lv_label_set_text(scr_dashboard_label_detail_time, "09:42:19");
        lv_label_set_text(scr_dashboard_label_detail_event, "检测到包裹");
        lv_label_set_text(scr_dashboard_label_detail_result, "传送中");
        lv_label_set_text(scr_dashboard_label_detail_confidence, "--");
        lv_label_set_text(scr_dashboard_label_detail_infer_time, "--");
        lv_label_set_text(scr_dashboard_label_detail_status, "运行中");
        lv_label_set_text(scr_dashboard_label_detail_note, "检测到新包裹，正在传送。");
        lv_label_set_text(scr_dashboard_label_detail_badge, "Running");
        ui_state_modify(scr_dashboard_cont_detail_badge, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_label_detail_badge, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_02, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
        ui_state_modify(scr_dashboard_cont_log_01, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_03, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_04, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_05, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_06, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        break;
    }
    default: {
        break;
    }
    }
}
static void scr_dashboard_cont_log_03_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED: {
        lv_label_set_text(scr_dashboard_label_detail_time, "09:42:19");
        lv_label_set_text(scr_dashboard_label_detail_event, "推理完成");
        lv_label_set_text(scr_dashboard_label_detail_result, "72ms");
        lv_label_set_text(scr_dashboard_label_detail_confidence, "96%");
        lv_label_set_text(scr_dashboard_label_detail_infer_time, "72ms");
        lv_label_set_text(scr_dashboard_label_detail_status, "成功");
        lv_label_set_text(scr_dashboard_label_detail_note, "推理流程完成。");
        lv_label_set_text(scr_dashboard_label_detail_badge, "Success");
        ui_state_modify(scr_dashboard_cont_detail_badge, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_label_detail_badge, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_03, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
        ui_state_modify(scr_dashboard_cont_log_01, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_02, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_04, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_05, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_06, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        break;
    }
    default: {
        break;
    }
    }
}
static void scr_dashboard_cont_log_04_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED: {
        lv_label_set_text(scr_dashboard_label_detail_time, "09:42:20");
        lv_label_set_text(scr_dashboard_label_detail_event, "分类识别");
        lv_label_set_text(scr_dashboard_label_detail_result, "极兔 中");
        lv_label_set_text(scr_dashboard_label_detail_confidence, "96%");
        lv_label_set_text(scr_dashboard_label_detail_infer_time, "72ms");
        lv_label_set_text(scr_dashboard_label_detail_status, "成功");
        lv_label_set_text(scr_dashboard_label_detail_note, "极兔包裹分类成功。");
        lv_label_set_text(scr_dashboard_label_detail_badge, "Success");
        ui_state_modify(scr_dashboard_cont_detail_badge, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_label_detail_badge, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_04, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
        ui_state_modify(scr_dashboard_cont_log_01, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_02, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_03, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_05, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_06, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        break;
    }
    default: {
        break;
    }
    }
}
static void scr_dashboard_cont_log_05_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED: {
        lv_label_set_text(scr_dashboard_label_detail_time, "09:42:20");
        lv_label_set_text(scr_dashboard_label_detail_event, "置信度较低");
        lv_label_set_text(scr_dashboard_label_detail_result, "置信度过低");
        lv_label_set_text(scr_dashboard_label_detail_confidence, "71%");
        lv_label_set_text(scr_dashboard_label_detail_infer_time, "72ms");
        lv_label_set_text(scr_dashboard_label_detail_status, "警告");
        lv_label_set_text(scr_dashboard_label_detail_note, "置信度低于阈值，请人工复核。");
        lv_label_set_text(scr_dashboard_label_detail_badge, "Warning");
        ui_state_modify(scr_dashboard_cont_detail_badge, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
        ui_state_modify(scr_dashboard_label_detail_badge, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
        ui_state_modify(scr_dashboard_cont_log_05, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
        ui_state_modify(scr_dashboard_cont_log_01, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_02, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_03, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_04, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_06, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        break;
    }
    default: {
        break;
    }
    }
}
static void scr_dashboard_cont_log_06_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED: {
        lv_label_set_text(scr_dashboard_label_detail_time, "09:42:21");
        lv_label_set_text(scr_dashboard_label_detail_event, "检测到包裹");
        lv_label_set_text(scr_dashboard_label_detail_result, "传送中");
        lv_label_set_text(scr_dashboard_label_detail_confidence, "--");
        lv_label_set_text(scr_dashboard_label_detail_infer_time, "--");
        lv_label_set_text(scr_dashboard_label_detail_status, "运行中");
        lv_label_set_text(scr_dashboard_label_detail_note, "检测到新包裹，正在传送。");
        lv_label_set_text(scr_dashboard_label_detail_badge, "Running");
        ui_state_modify(scr_dashboard_cont_detail_badge, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_label_detail_badge, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_06, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
        ui_state_modify(scr_dashboard_cont_log_01, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_02, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_03, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_04, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_05, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        break;
    }
    default: {
        break;
    }
    }
}
static void register_ui_events(void) {
    lv_obj_add_event_cb(scr_dashboard_imgbtn_sys, scr_dashboard_imgbtn_sys_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(scr_dashboard_imgbtn_log, scr_dashboard_imgbtn_log_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(scr_dashboard_imgbtn_set, scr_dashboard_imgbtn_set_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(scr_dashboard_imgbtn_dash, scr_dashboard_imgbtn_dash_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(scr_dashboard_cont_log_01, scr_dashboard_cont_log_01_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(scr_dashboard_cont_log_02, scr_dashboard_cont_log_02_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(scr_dashboard_cont_log_03, scr_dashboard_cont_log_03_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(scr_dashboard_cont_log_04, scr_dashboard_cont_log_04_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(scr_dashboard_cont_log_05, scr_dashboard_cont_log_05_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(scr_dashboard_cont_log_06, scr_dashboard_cont_log_06_event_handler, LV_EVENT_ALL, NULL);
}
static void statebtn_device_change_state(uint8_t value) {
    switch (value) {
    case 0: {
        lv_img_set_src(scr_dashboard_img_lFJKr46M, &GREEN_20x20);
        lv_label_set_text(scr_dashboard_label_shIaNFSV, "RUNNING");
        break;
    }
    case 1: {
        lv_img_set_src(scr_dashboard_img_lFJKr46M, &RED_20x20);
        lv_label_set_text(scr_dashboard_label_shIaNFSV, "STOPPED");
        break;
    }
    case 2: {
        lv_img_set_src(scr_dashboard_img_lFJKr46M, &YELLOW_20x20);
        lv_label_set_text(scr_dashboard_label_shIaNFSV, "WARNING");
        break;
    }
    default: {
        break;
    }
    }
}
static void scr_dashboard_statebtn_device_event_handler(lv_event_t * e) {
    static uint8_t statebtn_device = 0;
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED: {
        ++statebtn_device;
        statebtn_device %= 3;
        statebtn_device_change_state(statebtn_device);
        break;
    }
    default: {
        break;
    }
    }
}

static const char *dashboard_debug_mode_text(sorting_debug_mode_t mode)
{
    switch (mode) {
    case SORTING_DEBUG_MODE_REAL_SENSOR:
        return "REAL";
    case SORTING_DEBUG_MODE_TIMED_ONLY:
        return "TIMED";
    default:
        return "ETH";
    }
}

static lv_obj_t *dashboard_debug_create_label(lv_obj_t *parent, int x, int y, int w, const char *text)
{
    lv_obj_t *label = lv_label_create(parent);
    lv_obj_set_pos(label, x, y);
    lv_obj_set_width(label, w);
    lv_label_set_text(label, text);
    lv_label_set_long_mode(label, LV_LABEL_LONG_CLIP);
    lv_obj_set_style_text_color(label, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(label, &lv_font_Misans_Heavy_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    return label;
}

static lv_obj_t *dashboard_debug_create_button(lv_obj_t *parent, int x, int y, int w, int h,
                                               const char *text, lv_event_cb_t cb, void *user_data)
{
    lv_obj_t *button = lv_btn_create(parent);
    lv_obj_set_pos(button, x, y);
    lv_obj_set_size(button, w, h);
    lv_obj_set_style_radius(button, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(button, lv_color_hex(0x244A4F), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(button, 230, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_t *label = lv_label_create(button);
    lv_label_set_text(label, text);
    lv_obj_center(label);
    lv_obj_set_style_text_font(label, &lv_font_Misans_Heavy_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(button, cb, LV_EVENT_CLICKED, user_data);
    return button;
}

static void dashboard_debug_apply(void)
{
    sorting_sim_control_apply_settings(&s_dashboard_debug_settings);
    sorting_sim_control_get_settings(&s_dashboard_debug_settings);
}

static const char *dashboard_debug_state_text(bool valid, bool active)
{
    if (!valid) return "--";
    return active ? "ON" : "OFF";
}

static void dashboard_debug_refresh(void)
{
    sorting_sim_control_get_settings(&s_dashboard_debug_settings);
    sorting_hardware_status_t hw_status;
    sorting_sim_control_get_hardware_status(&hw_status);
    char text[96];
    snprintf(text, sizeof(text), "MODE %s", dashboard_debug_mode_text(s_dashboard_debug_settings.mode));
    lv_label_set_text(scr_dashboard_debug_label_mode, text);
    snprintf(text, sizeof(text), "MOTOR %s", s_dashboard_debug_settings.motor_output_enabled ? "ON" : "OFF");
    lv_label_set_text(scr_dashboard_debug_label_motor, text);
    snprintf(text, sizeof(text), "SENSOR %s", s_dashboard_debug_settings.sensor_input_enabled ? "ON" : "OFF");
    lv_label_set_text(scr_dashboard_debug_label_sensor, text);
    snprintf(text, sizeof(text), "SPD A%02d B%02d C%02d",
             s_dashboard_debug_settings.motor_speed_percent[0],
             s_dashboard_debug_settings.motor_speed_percent[1],
             s_dashboard_debug_settings.motor_speed_percent[2]);
    lv_label_set_text(scr_dashboard_debug_label_speed, text);
    snprintf(text, sizeof(text), "SAFE %ums", (unsigned)s_dashboard_debug_settings.handoff_delay_ms);
    lv_label_set_text(scr_dashboard_debug_label_delay, text);
    snprintf(text, sizeof(text), "TO A%u B%u C%u",
             (unsigned)s_dashboard_debug_settings.belt_timeout_ms[0],
             (unsigned)s_dashboard_debug_settings.belt_timeout_ms[1],
             (unsigned)s_dashboard_debug_settings.belt_timeout_ms[2]);
    lv_label_set_text(scr_dashboard_debug_label_timeout, text);
    snprintf(text, sizeof(text), "S2 %s  S3 %s  S4 %s",
             dashboard_debug_state_text(hw_status.sensor_valid[0], hw_status.sensor_active[0]),
             dashboard_debug_state_text(hw_status.sensor_valid[1], hw_status.sensor_active[1]),
             dashboard_debug_state_text(hw_status.sensor_valid[2], hw_status.sensor_active[2]));
    lv_label_set_text(scr_dashboard_debug_label_hw_sensors, text);
    snprintf(text, sizeof(text), "ENC A %s%.1f mm", hw_status.encoder_valid[0] ? "" : "-- ",
             (double)hw_status.encoder_distance_mm[0]);
    lv_label_set_text(scr_dashboard_debug_label_enc_a, text);
    snprintf(text, sizeof(text), "ENC B %s%.1f mm", hw_status.encoder_valid[1] ? "" : "-- ",
             (double)hw_status.encoder_distance_mm[1]);
    lv_label_set_text(scr_dashboard_debug_label_enc_b, text);
    snprintf(text, sizeof(text), "ENC C %s%.1f mm", hw_status.encoder_valid[2] ? "" : "-- ",
             (double)hw_status.encoder_distance_mm[2]);
    lv_label_set_text(scr_dashboard_debug_label_enc_c, text);
    snprintf(text, sizeof(text), "MTEST %s", hw_status.motor_test_running ? "RUN" : "IDLE");
    lv_label_set_text(scr_dashboard_debug_label_mtest, text);
}

static void dashboard_debug_mode_event_handler(lv_event_t * e)
{
    (void)e;
    dashboard_debug_refresh();
    s_dashboard_debug_settings.mode = (sorting_debug_mode_t)((s_dashboard_debug_settings.mode + 1) % 3);
    if (s_dashboard_debug_settings.mode == SORTING_DEBUG_MODE_TIMED_ONLY) {
        s_dashboard_debug_settings.sensor_input_enabled = false;
    } else if (s_dashboard_debug_settings.mode == SORTING_DEBUG_MODE_REAL_SENSOR) {
        s_dashboard_debug_settings.sensor_input_enabled = true;
    }
    dashboard_debug_apply();
    dashboard_debug_refresh();
}

static void dashboard_debug_motor_event_handler(lv_event_t * e)
{
    (void)e;
    dashboard_debug_refresh();
    s_dashboard_debug_settings.motor_output_enabled = !s_dashboard_debug_settings.motor_output_enabled;
    dashboard_debug_apply();
    dashboard_debug_refresh();
}

static void dashboard_debug_sensor_event_handler(lv_event_t * e)
{
    (void)e;
    dashboard_debug_refresh();
    s_dashboard_debug_settings.sensor_input_enabled = !s_dashboard_debug_settings.sensor_input_enabled;
    if (!s_dashboard_debug_settings.sensor_input_enabled) {
        s_dashboard_debug_settings.mode = SORTING_DEBUG_MODE_TIMED_ONLY;
    } else if (s_dashboard_debug_settings.mode == SORTING_DEBUG_MODE_TIMED_ONLY) {
        s_dashboard_debug_settings.mode = SORTING_DEBUG_MODE_REAL_SENSOR;
    }
    dashboard_debug_apply();
    dashboard_debug_refresh();
}

static void dashboard_debug_adjust_event_handler(lv_event_t * e)
{
    intptr_t op = (intptr_t)lv_event_get_user_data(e);
    dashboard_debug_refresh();
    if (op >= 0 && op < 3) s_dashboard_debug_settings.motor_speed_percent[op] += 5;
    else if (op >= 10 && op < 13) s_dashboard_debug_settings.motor_speed_percent[op - 10] -= 5;
    else if (op == 20) s_dashboard_debug_settings.handoff_delay_ms += 100;
    else if (op == 21 && s_dashboard_debug_settings.handoff_delay_ms >= 150) s_dashboard_debug_settings.handoff_delay_ms -= 100;
    else if (op >= 30 && op < 33) s_dashboard_debug_settings.belt_timeout_ms[op - 30] += 250;
    else if (op >= 40 && op < 43 && s_dashboard_debug_settings.belt_timeout_ms[op - 40] >= 300) s_dashboard_debug_settings.belt_timeout_ms[op - 40] -= 250;
    dashboard_debug_apply();
    dashboard_debug_refresh();
}

static void dashboard_debug_sim_event_handler(lv_event_t * e)
{
    intptr_t cls = (intptr_t)lv_event_get_user_data(e);
    sorting_sim_control_simulate_class((sorter_package_class_t)cls, NULL, NULL);
    dashboard_debug_refresh();
}

static void dashboard_debug_motor_test_event_handler(lv_event_t * e)
{
    (void)e;
    sorting_sim_control_start_motor_test();
    dashboard_debug_refresh();
}

static void dashboard_debug_encoder_clear_event_handler(lv_event_t * e)
{
    intptr_t index = (intptr_t)lv_event_get_user_data(e);
    sorting_sim_control_clear_encoder_distance((int)index);
    dashboard_debug_refresh();
}

static void dashboard_debug_timer_cb(lv_timer_t *timer)
{
    (void)timer;
    dashboard_debug_refresh();
}

static void create_dashboard_debug_panel(lv_obj_t *parent)
{
    scr_dashboard_debug_panel = lv_obj_create(parent);
    lv_obj_set_pos(scr_dashboard_debug_panel, 24, 12);
    lv_obj_set_size(scr_dashboard_debug_panel, 962, 392);
    lv_obj_set_scrollbar_mode(scr_dashboard_debug_panel, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_bg_color(scr_dashboard_debug_panel, lv_color_hex(0x172323), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_debug_panel, 210, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_debug_panel, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_debug_panel, lv_color_hex(0x45666A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_debug_panel, 8, LV_PART_MAIN | LV_STATE_DEFAULT);

    dashboard_debug_create_label(scr_dashboard_debug_panel, 18, 14, 130, "SORT DEBUG");
    scr_dashboard_debug_label_mode = dashboard_debug_create_label(scr_dashboard_debug_panel, 162, 14, 90, "MODE ETH");
    scr_dashboard_debug_label_motor = dashboard_debug_create_label(scr_dashboard_debug_panel, 258, 14, 95, "MOTOR ON");
    scr_dashboard_debug_label_sensor = dashboard_debug_create_label(scr_dashboard_debug_panel, 360, 14, 110, "SENSOR ON");
    scr_dashboard_debug_label_mtest = dashboard_debug_create_label(scr_dashboard_debug_panel, 478, 14, 100, "MTEST IDLE");
    dashboard_debug_create_button(scr_dashboard_debug_panel, 596, 8, 68, 30, "MODE", dashboard_debug_mode_event_handler, NULL);
    dashboard_debug_create_button(scr_dashboard_debug_panel, 672, 8, 76, 30, "MOTOR", dashboard_debug_motor_event_handler, NULL);
    dashboard_debug_create_button(scr_dashboard_debug_panel, 756, 8, 68, 30, "SENS", dashboard_debug_sensor_event_handler, NULL);
    dashboard_debug_create_button(scr_dashboard_debug_panel, 832, 8, 96, 30, "MTEST", dashboard_debug_motor_test_event_handler, NULL);

    scr_dashboard_debug_label_speed = dashboard_debug_create_label(scr_dashboard_debug_panel, 18, 64, 180, "SPD A30 B35 C35");
    dashboard_debug_create_button(scr_dashboard_debug_panel, 220, 58, 46, 30, "A-", dashboard_debug_adjust_event_handler, (void *)10);
    dashboard_debug_create_button(scr_dashboard_debug_panel, 272, 58, 46, 30, "A+", dashboard_debug_adjust_event_handler, (void *)0);
    dashboard_debug_create_button(scr_dashboard_debug_panel, 336, 58, 46, 30, "B-", dashboard_debug_adjust_event_handler, (void *)11);
    dashboard_debug_create_button(scr_dashboard_debug_panel, 388, 58, 46, 30, "B+", dashboard_debug_adjust_event_handler, (void *)1);
    dashboard_debug_create_button(scr_dashboard_debug_panel, 452, 58, 46, 30, "C-", dashboard_debug_adjust_event_handler, (void *)12);
    dashboard_debug_create_button(scr_dashboard_debug_panel, 504, 58, 46, 30, "C+", dashboard_debug_adjust_event_handler, (void *)2);

    scr_dashboard_debug_label_delay = dashboard_debug_create_label(scr_dashboard_debug_panel, 18, 108, 130, "SAFE 1000ms");
    dashboard_debug_create_button(scr_dashboard_debug_panel, 154, 102, 46, 30, "-S", dashboard_debug_adjust_event_handler, (void *)21);
    dashboard_debug_create_button(scr_dashboard_debug_panel, 206, 102, 46, 30, "+S", dashboard_debug_adjust_event_handler, (void *)20);
    scr_dashboard_debug_label_timeout = dashboard_debug_create_label(scr_dashboard_debug_panel, 286, 108, 210, "TO A2000 B750 C750");
    dashboard_debug_create_button(scr_dashboard_debug_panel, 506, 102, 42, 30, "A-", dashboard_debug_adjust_event_handler, (void *)40);
    dashboard_debug_create_button(scr_dashboard_debug_panel, 552, 102, 42, 30, "A+", dashboard_debug_adjust_event_handler, (void *)30);
    dashboard_debug_create_button(scr_dashboard_debug_panel, 604, 102, 42, 30, "B-", dashboard_debug_adjust_event_handler, (void *)41);
    dashboard_debug_create_button(scr_dashboard_debug_panel, 650, 102, 42, 30, "B+", dashboard_debug_adjust_event_handler, (void *)31);
    dashboard_debug_create_button(scr_dashboard_debug_panel, 702, 102, 42, 30, "C-", dashboard_debug_adjust_event_handler, (void *)42);
    dashboard_debug_create_button(scr_dashboard_debug_panel, 748, 102, 42, 30, "C+", dashboard_debug_adjust_event_handler, (void *)32);

    scr_dashboard_debug_label_hw_sensors = dashboard_debug_create_label(scr_dashboard_debug_panel, 18, 158, 330, "S2 --  S3 --  S4 --");
    scr_dashboard_debug_label_enc_a = dashboard_debug_create_label(scr_dashboard_debug_panel, 18, 204, 190, "ENC A --");
    dashboard_debug_create_button(scr_dashboard_debug_panel, 214, 198, 58, 30, "CLR", dashboard_debug_encoder_clear_event_handler, (void *)0);
    scr_dashboard_debug_label_enc_b = dashboard_debug_create_label(scr_dashboard_debug_panel, 304, 204, 190, "ENC B --");
    dashboard_debug_create_button(scr_dashboard_debug_panel, 500, 198, 58, 30, "CLR", dashboard_debug_encoder_clear_event_handler, (void *)1);
    scr_dashboard_debug_label_enc_c = dashboard_debug_create_label(scr_dashboard_debug_panel, 590, 204, 190, "ENC C --");
    dashboard_debug_create_button(scr_dashboard_debug_panel, 786, 198, 58, 30, "CLR", dashboard_debug_encoder_clear_event_handler, (void *)2);

    dashboard_debug_create_button(scr_dashboard_debug_panel, 18, 294, 92, 40, "CLASS1", dashboard_debug_sim_event_handler, (void *)SORTER_CLASS_1);
    dashboard_debug_create_button(scr_dashboard_debug_panel, 122, 294, 92, 40, "CLASS2", dashboard_debug_sim_event_handler, (void *)SORTER_CLASS_2);
    dashboard_debug_create_button(scr_dashboard_debug_panel, 226, 294, 92, 40, "CLASS3", dashboard_debug_sim_event_handler, (void *)SORTER_CLASS_3);
    dashboard_debug_create_button(scr_dashboard_debug_panel, 330, 294, 92, 40, "FREE", dashboard_debug_sim_event_handler, (void *)SORTER_CLASS_FREE);
    dashboard_debug_create_button(scr_dashboard_debug_panel, 434, 294, 92, 40, "ERROR", dashboard_debug_sim_event_handler, (void *)SORTER_CLASS_ERROR);
    dashboard_debug_refresh();
    if (!scr_dashboard_debug_timer) {
        scr_dashboard_debug_timer = lv_timer_create(dashboard_debug_timer_cb, 500, NULL);
    }
}

static lv_obj_t * create_ui(void) {
    LV_LOG_USER("Initializing scr_dashboard ...");
    scr_dashboard = lv_obj_create(NULL);
    lv_obj_set_scrollbar_mode(scr_dashboard, LV_SCROLLBAR_MODE_OFF);
    // Add style for scr_dashboard - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_background1
    scr_dashboard_cont_background1 = lv_obj_create(scr_dashboard);
    lv_obj_set_x(scr_dashboard_cont_background1, 2);
    lv_obj_set_y(scr_dashboard_cont_background1, 2);
    lv_obj_set_width(scr_dashboard_cont_background1, 1020);
    lv_obj_set_height(scr_dashboard_cont_background1, 596);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_background1, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_background1, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_background1 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_background1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_background1, 58, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_background1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_background1, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_background1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_background1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_background1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_background1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_background1, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_background1, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_background1, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_background1, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_background1, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_cont_background2
    scr_dashboard_cont_background2 = lv_obj_create(scr_dashboard_cont_background1);
    lv_obj_set_x(scr_dashboard_cont_background2, 2);
    lv_obj_set_y(scr_dashboard_cont_background2, 2);
    lv_obj_set_width(scr_dashboard_cont_background2, 1012);
    lv_obj_set_height(scr_dashboard_cont_background2, 588);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_background2, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_background2, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_background2 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_background2, lv_color_hex(0x0B1517), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_background2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_background2, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_background2, 104, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_background2, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_background2, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_background2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_background2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_background2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_background2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(scr_dashboard_cont_background2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(scr_dashboard_cont_background2, 111, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(scr_dashboard_cont_background2, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(scr_dashboard_cont_background2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_background2, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_background2, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_background2, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_background2, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_background2, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_cont_log
    scr_dashboard_cont_log = lv_obj_create(scr_dashboard_cont_background2);
    lv_obj_set_x(scr_dashboard_cont_log, 3);
    lv_obj_set_y(scr_dashboard_cont_log, 69);
    lv_obj_set_width(scr_dashboard_cont_log, 1010);
    lv_obj_set_height(scr_dashboard_cont_log, 436);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_log, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_log, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_log - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_log, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_cont_event_details
    scr_dashboard_cont_event_details = lv_obj_create(scr_dashboard_cont_log);
    lv_obj_set_x(scr_dashboard_cont_event_details, 601);
    lv_obj_set_y(scr_dashboard_cont_event_details, 68);
    lv_obj_set_width(scr_dashboard_cont_event_details, 392);
    lv_obj_set_height(scr_dashboard_cont_event_details, 350);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_event_details, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_event_details, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_event_details - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_event_details, lv_color_hex(0x232C2C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_event_details, lv_color_hex(0x070A0A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_event_details, 180, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_event_details, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_event_details, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_event_details, 46, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_event_details, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_event_details, 16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_event_details, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_event_details, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_event_details, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_event_details, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_event_details, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_event_details, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_event_details, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_event_details, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_event_details, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_title_event
    scr_dashboard_label_title_event = lv_label_create(scr_dashboard_cont_event_details);
    lv_obj_set_x(scr_dashboard_label_title_event, 26);
    lv_obj_set_y(scr_dashboard_label_title_event, 9);
    lv_obj_set_width(scr_dashboard_label_title_event, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_title_event, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_title_event, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_title_event, "事件详情");
    lv_label_set_long_mode(scr_dashboard_label_title_event, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_title_event - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_title_event, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_title_event, &lv_font_Misans_Heavy_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_title_event, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_detail_result
    scr_dashboard_label_detail_result = lv_label_create(scr_dashboard_cont_event_details);
    lv_obj_set_x(scr_dashboard_label_detail_result, 225);
    lv_obj_set_y(scr_dashboard_label_detail_result, 126);
    lv_obj_set_width(scr_dashboard_label_detail_result, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_detail_result, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_result, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_result, "传送中");
    lv_label_set_long_mode(scr_dashboard_label_detail_result, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_result - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_result, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_result, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_result, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_detail_event
    scr_dashboard_label_detail_event = lv_label_create(scr_dashboard_cont_event_details);
    lv_obj_set_x(scr_dashboard_label_detail_event, 225);
    lv_obj_set_y(scr_dashboard_label_detail_event, 90);
    lv_obj_set_width(scr_dashboard_label_detail_event, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_detail_event, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_event, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_event, "检测到包裹");
    lv_label_set_long_mode(scr_dashboard_label_detail_event, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_event - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_event, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_detail_event, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_event, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_event, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_detail_confidence
    scr_dashboard_label_detail_confidence = lv_label_create(scr_dashboard_cont_event_details);
    lv_obj_set_x(scr_dashboard_label_detail_confidence, 225);
    lv_obj_set_y(scr_dashboard_label_detail_confidence, 162);
    lv_obj_set_width(scr_dashboard_label_detail_confidence, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_detail_confidence, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_confidence, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_confidence, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_confidence, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_confidence - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_confidence, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_confidence, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_confidence, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_detail_time
    scr_dashboard_label_detail_time = lv_label_create(scr_dashboard_cont_event_details);
    lv_obj_set_x(scr_dashboard_label_detail_time, 225);
    lv_obj_set_y(scr_dashboard_label_detail_time, 54);
    lv_obj_set_width(scr_dashboard_label_detail_time, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_detail_time, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_time, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_time, "09:42:19");
    lv_label_set_long_mode(scr_dashboard_label_detail_time, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_time - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_time, lv_color_hex(0x718A92), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_time, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_time, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_title_inference
    scr_dashboard_label_title_inference = lv_label_create(scr_dashboard_cont_event_details);
    lv_obj_set_x(scr_dashboard_label_title_inference, 72);
    lv_obj_set_y(scr_dashboard_label_title_inference, 162);
    lv_obj_set_width(scr_dashboard_label_title_inference, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_title_inference, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_title_inference, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_title_inference, "置信度");
    lv_label_set_long_mode(scr_dashboard_label_title_inference, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_title_inference - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_title_inference, lv_color_hex(0x7F98A0), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_title_inference, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_title_inference, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_detail_infer_time
    scr_dashboard_label_detail_infer_time = lv_label_create(scr_dashboard_cont_event_details);
    lv_obj_set_x(scr_dashboard_label_detail_infer_time, 225);
    lv_obj_set_y(scr_dashboard_label_detail_infer_time, 198);
    lv_obj_set_width(scr_dashboard_label_detail_infer_time, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_detail_infer_time, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_infer_time, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_infer_time, "72ms");
    lv_label_set_long_mode(scr_dashboard_label_detail_infer_time, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_infer_time - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_infer_time, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_infer_time, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_infer_time, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_title_result
    scr_dashboard_label_title_result = lv_label_create(scr_dashboard_cont_event_details);
    lv_obj_set_x(scr_dashboard_label_title_result, 72);
    lv_obj_set_y(scr_dashboard_label_title_result, 126);
    lv_obj_set_width(scr_dashboard_label_title_result, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_title_result, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_title_result, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_title_result, "结果");
    lv_label_set_long_mode(scr_dashboard_label_title_result, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_title_result - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_title_result, lv_color_hex(0x7F98A0), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_title_result, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_title_result, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_detail_status
    scr_dashboard_label_detail_status = lv_label_create(scr_dashboard_cont_event_details);
    lv_obj_set_x(scr_dashboard_label_detail_status, 225);
    lv_obj_set_y(scr_dashboard_label_detail_status, 233);
    lv_obj_set_width(scr_dashboard_label_detail_status, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_detail_status, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_status, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_status, "运行中");
    lv_label_set_long_mode(scr_dashboard_label_detail_status, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_status - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_status, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_status, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_status, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_title_type
    scr_dashboard_label_title_type = lv_label_create(scr_dashboard_cont_event_details);
    lv_obj_set_x(scr_dashboard_label_title_type, 72);
    lv_obj_set_y(scr_dashboard_label_title_type, 90);
    lv_obj_set_width(scr_dashboard_label_title_type, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_title_type, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_title_type, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_title_type, "事件类型");
    lv_label_set_long_mode(scr_dashboard_label_title_type, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_title_type - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_title_type, lv_color_hex(0x7F98A0), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_title_type, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_title_type, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_title_inft
    scr_dashboard_label_title_inft = lv_label_create(scr_dashboard_cont_event_details);
    lv_obj_set_x(scr_dashboard_label_title_inft, 72);
    lv_obj_set_y(scr_dashboard_label_title_inft, 198);
    lv_obj_set_width(scr_dashboard_label_title_inft, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_title_inft, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_title_inft, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_title_inft, "推理耗时");
    lv_label_set_long_mode(scr_dashboard_label_title_inft, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_title_inft - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_title_inft, lv_color_hex(0x7F98A0), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_title_inft, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_title_inft, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_title_time
    scr_dashboard_label_title_time = lv_label_create(scr_dashboard_cont_event_details);
    lv_obj_set_x(scr_dashboard_label_title_time, 72);
    lv_obj_set_y(scr_dashboard_label_title_time, 54);
    lv_obj_set_width(scr_dashboard_label_title_time, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_title_time, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_title_time, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_title_time, "时间");
    lv_label_set_long_mode(scr_dashboard_label_title_time, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_title_time - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_title_time, lv_color_hex(0x7F98A0), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_title_time, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_title_time, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_title_status
    scr_dashboard_label_title_status = lv_label_create(scr_dashboard_cont_event_details);
    lv_obj_set_x(scr_dashboard_label_title_status, 72);
    lv_obj_set_y(scr_dashboard_label_title_status, 233);
    lv_obj_set_width(scr_dashboard_label_title_status, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_title_status, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_title_status, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_title_status, "状态");
    lv_label_set_long_mode(scr_dashboard_label_title_status, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_title_status - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_title_status, lv_color_hex(0x7F98A0), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_title_status, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_title_status, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_under2
    scr_dashboard_cont_under2 = lv_obj_create(scr_dashboard_cont_event_details);
    lv_obj_set_x(scr_dashboard_cont_under2, 36);
    lv_obj_set_y(scr_dashboard_cont_under2, 31);
    lv_obj_set_width(scr_dashboard_cont_under2, 36);
    lv_obj_set_height(scr_dashboard_cont_under2, 2);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_under2, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_under2, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_under2 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_under2, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_under2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_under2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_under2, 999, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_under2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_under2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_under2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_under2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_under2, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_under2, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_under2, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_under2, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_under2, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_cont_detail_badge
    scr_dashboard_cont_detail_badge = lv_obj_create(scr_dashboard_cont_event_details);
    lv_obj_set_x(scr_dashboard_cont_detail_badge, 302);
    lv_obj_set_y(scr_dashboard_cont_detail_badge, 9);
    lv_obj_set_width(scr_dashboard_cont_detail_badge, 72);
    lv_obj_set_height(scr_dashboard_cont_detail_badge, 24);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_detail_badge, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_detail_badge, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_detail_badge - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_detail_badge, lv_color_hex(0x232C2C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_badge, 220, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_detail_badge, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_detail_badge, 66, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_badge, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_detail_badge, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_badge, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_badge, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_badge, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_badge, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_cont_detail_badge - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_cont_detail_badge, lv_color_hex(0x3A2A12), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_badge, 220, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_cont_detail_badge, lv_color_hex(0xE8B84A), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_cont_detail_badge, 120, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_badge, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_cont_detail_badge, 12, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_badge, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_badge, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_badge, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_badge, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    ui_flag_modify(scr_dashboard_cont_detail_badge, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_detail_badge
    scr_dashboard_label_detail_badge = lv_label_create(scr_dashboard_cont_detail_badge);
    lv_obj_set_x(scr_dashboard_label_detail_badge, 11);
    lv_obj_set_y(scr_dashboard_label_detail_badge, 5);
    lv_obj_set_width(scr_dashboard_label_detail_badge, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_detail_badge, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_badge, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_badge, "Success");
    lv_label_set_long_mode(scr_dashboard_label_detail_badge, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_badge - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_badge, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_detail_badge, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_label_detail_badge - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_text_color(scr_dashboard_label_detail_badge, lv_color_hex(0xE8B84A), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(scr_dashboard_label_detail_badge, 255, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_badge, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_badge, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_badge, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_badge, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_CHECKED);
    // Create scr_dashboard_cont_note
    scr_dashboard_cont_note = lv_obj_create(scr_dashboard_cont_event_details);
    lv_obj_set_x(scr_dashboard_cont_note, 12);
    lv_obj_set_y(scr_dashboard_cont_note, 261);
    lv_obj_set_width(scr_dashboard_cont_note, 362);
    lv_obj_set_height(scr_dashboard_cont_note, 80);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_note, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_note, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_note - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_note, lv_color_hex(0x3F5250), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_note, 62, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_note, lv_color_hex(0x4C6770), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_note, 28, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_note, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_note, 14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_note, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_note, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_note, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_note, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_note, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_note, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_note, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_note, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_note, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_title_note
    scr_dashboard_label_title_note = lv_label_create(scr_dashboard_cont_note);
    lv_obj_set_x(scr_dashboard_label_title_note, 23);
    lv_obj_set_y(scr_dashboard_label_title_note, 12);
    lv_obj_set_width(scr_dashboard_label_title_note, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_title_note, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_title_note, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_title_note, "备注");
    lv_label_set_long_mode(scr_dashboard_label_title_note, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_title_note - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_title_note, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_title_note, &lv_font_Misans_Heavy_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_title_note, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_note_line
    scr_dashboard_cont_note_line = lv_obj_create(scr_dashboard_cont_note);
    lv_obj_set_x(scr_dashboard_cont_note_line, 15);
    lv_obj_set_y(scr_dashboard_cont_note_line, 13);
    lv_obj_set_width(scr_dashboard_cont_note_line, 2);
    lv_obj_set_height(scr_dashboard_cont_note_line, 12);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_note_line, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_note_line, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_note_line - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_note_line, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_note_line, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_note_line, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_note_line, 999, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_note_line, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_note_line, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_note_line, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_note_line, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_note_line, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_note_line, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_note_line, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_note_line, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_note_line, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_detail_note
    scr_dashboard_label_detail_note = lv_label_create(scr_dashboard_cont_note);
    lv_obj_set_x(scr_dashboard_label_detail_note, 62);
    lv_obj_set_y(scr_dashboard_label_detail_note, 32);
    lv_obj_set_width(scr_dashboard_label_detail_note, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_detail_note, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_note, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_note, "包裹分类成功。");
    lv_label_set_long_mode(scr_dashboard_label_detail_note, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_note - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_note, lv_color_hex(0xA7BCC3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_note, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_note, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_log_record
    scr_dashboard_cont_log_record = lv_obj_create(scr_dashboard_cont_log);
    lv_obj_set_x(scr_dashboard_cont_log_record, 13);
    lv_obj_set_y(scr_dashboard_cont_log_record, 68);
    lv_obj_set_width(scr_dashboard_cont_log_record, 572);
    lv_obj_set_height(scr_dashboard_cont_log_record, 350);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_log_record, LV_SCROLLBAR_MODE_ACTIVE);
    lv_obj_set_layout(scr_dashboard_cont_log_record, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_log_record - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_record, lv_color_hex(0x232C2C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_log_record, lv_color_hex(0x070A0A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_record, 108, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_log_record, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_record, lv_color_hex(0x4C6770), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_record, 44, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_record, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_log_record, 16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_record, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_record, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_record, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_record, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_log_record, LV_OBJ_FLAG_SCROLL_CHAIN_VER, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_log_record, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_record, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_cont_log_06
    scr_dashboard_cont_log_06 = lv_obj_create(scr_dashboard_cont_log_record);
    lv_obj_set_x(scr_dashboard_cont_log_06, 12);
    lv_obj_set_y(scr_dashboard_cont_log_06, 328);
    lv_obj_set_width(scr_dashboard_cont_log_06, 534);
    lv_obj_set_height(scr_dashboard_cont_log_06, 44);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_log_06, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_log_06, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_log_06 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_06, lv_color_hex(0x1E2426), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_06, 160, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_06, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_06, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_06, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_log_06, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_06, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_06, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_06, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_06, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_cont_log_06 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_06, lv_color_hex(0x3F5250), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_log_06, lv_color_hex(0x1A2827), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_06, 100, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_log_06, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_06, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_06, 80, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_06, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_cont_log_06, 4, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_06, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_06, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_06, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_06, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    ui_flag_modify(scr_dashboard_cont_log_06, LV_OBJ_FLAG_CHECKABLE, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_log_06, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_06, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_06, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_06, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_06, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_time_06
    scr_dashboard_label_time_06 = lv_label_create(scr_dashboard_cont_log_06);
    lv_obj_set_x(scr_dashboard_label_time_06, 27);
    lv_obj_set_y(scr_dashboard_label_time_06, 15);
    lv_obj_set_width(scr_dashboard_label_time_06, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_time_06, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_time_06, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_time_06, "09:42:21");
    lv_label_set_long_mode(scr_dashboard_label_time_06, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_time_06 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_time_06, lv_color_hex(0x718A92), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_time_06, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_time_06, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_type_06
    scr_dashboard_label_type_06 = lv_label_create(scr_dashboard_cont_log_06);
    lv_obj_set_x(scr_dashboard_label_type_06, 152);
    lv_obj_set_y(scr_dashboard_label_type_06, 14);
    lv_obj_set_width(scr_dashboard_label_type_06, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_type_06, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_type_06, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_type_06, "检测到包裹");
    lv_label_set_long_mode(scr_dashboard_label_type_06, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_type_06 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_type_06, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_type_06, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_type_06, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_result_06
    scr_dashboard_label_result_06 = lv_label_create(scr_dashboard_cont_log_06);
    lv_obj_set_x(scr_dashboard_label_result_06, 311);
    lv_obj_set_y(scr_dashboard_label_result_06, 14);
    lv_obj_set_width(scr_dashboard_label_result_06, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_result_06, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_result_06, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_result_06, "传送中");
    lv_label_set_long_mode(scr_dashboard_label_result_06, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_result_06 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_result_06, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_result_06, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_result_06, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_status_06
    scr_dashboard_label_status_06 = lv_label_create(scr_dashboard_cont_log_06);
    lv_obj_set_x(scr_dashboard_label_status_06, 454);
    lv_obj_set_y(scr_dashboard_label_status_06, 15);
    lv_obj_set_width(scr_dashboard_label_status_06, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_status_06, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_status_06, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_status_06, "运行中");
    lv_label_set_long_mode(scr_dashboard_label_status_06, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_status_06 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_status_06, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_status_06, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_status_06, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_log_05
    scr_dashboard_cont_log_05 = lv_obj_create(scr_dashboard_cont_log_record);
    lv_obj_set_x(scr_dashboard_cont_log_05, 12);
    lv_obj_set_y(scr_dashboard_cont_log_05, 271);
    lv_obj_set_width(scr_dashboard_cont_log_05, 534);
    lv_obj_set_height(scr_dashboard_cont_log_05, 44);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_log_05, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_log_05, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_log_05 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_05, lv_color_hex(0x1E2426), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_05, 160, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_05, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_05, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_05, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_log_05, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_05, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_05, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_05, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_05, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_cont_log_05 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_05, lv_color_hex(0x3F5250), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_log_05, lv_color_hex(0x1A2827), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_05, 100, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_log_05, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_05, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_05, 80, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_05, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_cont_log_05, 4, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_05, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_05, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_05, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_05, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    ui_flag_modify(scr_dashboard_cont_log_05, LV_OBJ_FLAG_CHECKABLE, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_log_05, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_05, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_05, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_05, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_05, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_time_05
    scr_dashboard_label_time_05 = lv_label_create(scr_dashboard_cont_log_05);
    lv_obj_set_x(scr_dashboard_label_time_05, 27);
    lv_obj_set_y(scr_dashboard_label_time_05, 15);
    lv_obj_set_width(scr_dashboard_label_time_05, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_time_05, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_time_05, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_time_05, "09:42:20");
    lv_label_set_long_mode(scr_dashboard_label_time_05, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_time_05 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_time_05, lv_color_hex(0x718A92), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_time_05, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_time_05, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_type_05
    scr_dashboard_label_type_05 = lv_label_create(scr_dashboard_cont_log_05);
    lv_obj_set_x(scr_dashboard_label_type_05, 152);
    lv_obj_set_y(scr_dashboard_label_type_05, 14);
    lv_obj_set_width(scr_dashboard_label_type_05, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_type_05, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_type_05, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_type_05, "置信度较低");
    lv_label_set_long_mode(scr_dashboard_label_type_05, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_type_05 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_type_05, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_type_05, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_type_05, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_result_05
    scr_dashboard_label_result_05 = lv_label_create(scr_dashboard_cont_log_05);
    lv_obj_set_x(scr_dashboard_label_result_05, 322);
    lv_obj_set_y(scr_dashboard_label_result_05, 14);
    lv_obj_set_width(scr_dashboard_label_result_05, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_result_05, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_result_05, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_result_05, "71%");
    lv_label_set_long_mode(scr_dashboard_label_result_05, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_result_05 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_result_05, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_result_05, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_result_05, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_status_05
    scr_dashboard_label_status_05 = lv_label_create(scr_dashboard_cont_log_05);
    lv_obj_set_x(scr_dashboard_label_status_05, 462);
    lv_obj_set_y(scr_dashboard_label_status_05, 14);
    lv_obj_set_width(scr_dashboard_label_status_05, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_status_05, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_status_05, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_status_05, "警告");
    lv_label_set_long_mode(scr_dashboard_label_status_05, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_status_05 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_status_05, lv_color_hex(0xE8B84A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_status_05, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_status_05, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_log_04
    scr_dashboard_cont_log_04 = lv_obj_create(scr_dashboard_cont_log_record);
    lv_obj_set_x(scr_dashboard_cont_log_04, 12);
    lv_obj_set_y(scr_dashboard_cont_log_04, 214);
    lv_obj_set_width(scr_dashboard_cont_log_04, 534);
    lv_obj_set_height(scr_dashboard_cont_log_04, 44);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_log_04, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_log_04, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_log_04 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_04, lv_color_hex(0x1E2426), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_04, 160, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_04, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_04, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_04, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_log_04, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_04, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_04, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_04, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_04, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_cont_log_04 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_04, lv_color_hex(0x3F5250), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_log_04, lv_color_hex(0x1A2827), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_04, 100, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_log_04, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_04, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_04, 80, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_04, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_cont_log_04, 4, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_04, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_04, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_04, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_04, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    ui_flag_modify(scr_dashboard_cont_log_04, LV_OBJ_FLAG_CHECKABLE, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_log_04, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_04, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_04, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_04, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_04, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_time_04
    scr_dashboard_label_time_04 = lv_label_create(scr_dashboard_cont_log_04);
    lv_obj_set_x(scr_dashboard_label_time_04, 27);
    lv_obj_set_y(scr_dashboard_label_time_04, 15);
    lv_obj_set_width(scr_dashboard_label_time_04, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_time_04, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_time_04, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_time_04, "09:42:20");
    lv_label_set_long_mode(scr_dashboard_label_time_04, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_time_04 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_time_04, lv_color_hex(0x718A92), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_time_04, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_time_04, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_type_04
    scr_dashboard_label_type_04 = lv_label_create(scr_dashboard_cont_log_04);
    lv_obj_set_x(scr_dashboard_label_type_04, 157);
    lv_obj_set_y(scr_dashboard_label_type_04, 14);
    lv_obj_set_width(scr_dashboard_label_type_04, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_type_04, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_type_04, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_type_04, "分类识别");
    lv_label_set_long_mode(scr_dashboard_label_type_04, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_type_04 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_type_04, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_type_04, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_type_04, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_result_04
    scr_dashboard_label_result_04 = lv_label_create(scr_dashboard_cont_log_04);
    lv_obj_set_x(scr_dashboard_label_result_04, 296);
    lv_obj_set_y(scr_dashboard_label_result_04, 14);
    lv_obj_set_width(scr_dashboard_label_result_04, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_result_04, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_result_04, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_result_04, "极兔  96%  中");
    lv_label_set_long_mode(scr_dashboard_label_result_04, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_result_04 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_result_04, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_result_04, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_result_04, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_status_04
    scr_dashboard_label_status_04 = lv_label_create(scr_dashboard_cont_log_04);
    lv_obj_set_x(scr_dashboard_label_status_04, 462);
    lv_obj_set_y(scr_dashboard_label_status_04, 14);
    lv_obj_set_width(scr_dashboard_label_status_04, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_status_04, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_status_04, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_status_04, "成功");
    lv_label_set_long_mode(scr_dashboard_label_status_04, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_status_04 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_status_04, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_status_04, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_status_04, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_log_03
    scr_dashboard_cont_log_03 = lv_obj_create(scr_dashboard_cont_log_record);
    lv_obj_set_x(scr_dashboard_cont_log_03, 12);
    lv_obj_set_y(scr_dashboard_cont_log_03, 157);
    lv_obj_set_width(scr_dashboard_cont_log_03, 534);
    lv_obj_set_height(scr_dashboard_cont_log_03, 44);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_log_03, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_log_03, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_log_03 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_03, lv_color_hex(0x1E2426), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_03, 160, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_03, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_03, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_03, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_log_03, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_03, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_03, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_03, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_03, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_cont_log_03 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_03, lv_color_hex(0x3F5250), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_log_03, lv_color_hex(0x1A2827), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_03, 100, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_log_03, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_03, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_03, 80, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_03, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_cont_log_03, 4, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_03, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_03, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_03, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_03, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    ui_flag_modify(scr_dashboard_cont_log_03, LV_OBJ_FLAG_CHECKABLE, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_log_03, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_03, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_03, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_03, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_03, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_time_03
    scr_dashboard_label_time_03 = lv_label_create(scr_dashboard_cont_log_03);
    lv_obj_set_x(scr_dashboard_label_time_03, 27);
    lv_obj_set_y(scr_dashboard_label_time_03, 15);
    lv_obj_set_width(scr_dashboard_label_time_03, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_time_03, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_time_03, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_time_03, "09:42:19");
    lv_label_set_long_mode(scr_dashboard_label_time_03, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_time_03 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_time_03, lv_color_hex(0x718A92), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_time_03, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_time_03, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_type_03
    scr_dashboard_label_type_03 = lv_label_create(scr_dashboard_cont_log_03);
    lv_obj_set_x(scr_dashboard_label_type_03, 157);
    lv_obj_set_y(scr_dashboard_label_type_03, 14);
    lv_obj_set_width(scr_dashboard_label_type_03, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_type_03, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_type_03, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_type_03, "推理完成");
    lv_label_set_long_mode(scr_dashboard_label_type_03, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_type_03 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_type_03, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_type_03, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_type_03, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_result_03
    scr_dashboard_label_result_03 = lv_label_create(scr_dashboard_cont_log_03);
    lv_obj_set_x(scr_dashboard_label_result_03, 311);
    lv_obj_set_y(scr_dashboard_label_result_03, 14);
    lv_obj_set_width(scr_dashboard_label_result_03, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_result_03, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_result_03, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_result_03, "72ms");
    lv_label_set_long_mode(scr_dashboard_label_result_03, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_result_03 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_result_03, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_result_03, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_result_03, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_status_03
    scr_dashboard_label_status_03 = lv_label_create(scr_dashboard_cont_log_03);
    lv_obj_set_x(scr_dashboard_label_status_03, 462);
    lv_obj_set_y(scr_dashboard_label_status_03, 14);
    lv_obj_set_width(scr_dashboard_label_status_03, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_status_03, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_status_03, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_status_03, "成功");
    lv_label_set_long_mode(scr_dashboard_label_status_03, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_status_03 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_status_03, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_status_03, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_status_03, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_log_02
    scr_dashboard_cont_log_02 = lv_obj_create(scr_dashboard_cont_log_record);
    lv_obj_set_x(scr_dashboard_cont_log_02, 12);
    lv_obj_set_y(scr_dashboard_cont_log_02, 100);
    lv_obj_set_width(scr_dashboard_cont_log_02, 534);
    lv_obj_set_height(scr_dashboard_cont_log_02, 44);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_log_02, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_log_02, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_log_02 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_02, lv_color_hex(0x1E2426), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_02, 160, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_02, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_02, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_02, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_log_02, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_02, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_02, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_02, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_02, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_cont_log_02 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_02, lv_color_hex(0x3F5250), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_log_02, lv_color_hex(0x1A2827), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_02, 100, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_log_02, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_02, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_02, 80, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_02, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_cont_log_02, 4, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_02, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_02, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_02, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_02, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    ui_flag_modify(scr_dashboard_cont_log_02, LV_OBJ_FLAG_CHECKABLE, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_log_02, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_02, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_02, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_02, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_02, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_time_02
    scr_dashboard_label_time_02 = lv_label_create(scr_dashboard_cont_log_02);
    lv_obj_set_x(scr_dashboard_label_time_02, 27);
    lv_obj_set_y(scr_dashboard_label_time_02, 15);
    lv_obj_set_width(scr_dashboard_label_time_02, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_time_02, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_time_02, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_time_02, "09:42:19");
    lv_label_set_long_mode(scr_dashboard_label_time_02, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_time_02 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_time_02, lv_color_hex(0x718A92), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_time_02, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_time_02, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_type_02
    scr_dashboard_label_type_02 = lv_label_create(scr_dashboard_cont_log_02);
    lv_obj_set_x(scr_dashboard_label_type_02, 152);
    lv_obj_set_y(scr_dashboard_label_type_02, 14);
    lv_obj_set_width(scr_dashboard_label_type_02, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_type_02, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_type_02, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_type_02, "检测到包裹");
    lv_label_set_long_mode(scr_dashboard_label_type_02, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_type_02 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_type_02, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_type_02, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_type_02, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_result_02
    scr_dashboard_label_result_02 = lv_label_create(scr_dashboard_cont_log_02);
    lv_obj_set_x(scr_dashboard_label_result_02, 311);
    lv_obj_set_y(scr_dashboard_label_result_02, 14);
    lv_obj_set_width(scr_dashboard_label_result_02, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_result_02, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_result_02, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_result_02, "传送中");
    lv_label_set_long_mode(scr_dashboard_label_result_02, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_result_02 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_result_02, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_result_02, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_result_02, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_status_02
    scr_dashboard_label_status_02 = lv_label_create(scr_dashboard_cont_log_02);
    lv_obj_set_x(scr_dashboard_label_status_02, 454);
    lv_obj_set_y(scr_dashboard_label_status_02, 15);
    lv_obj_set_width(scr_dashboard_label_status_02, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_status_02, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_status_02, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_status_02, "运行中");
    lv_label_set_long_mode(scr_dashboard_label_status_02, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_status_02 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_status_02, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_status_02, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_status_02, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_log_01
    scr_dashboard_cont_log_01 = lv_obj_create(scr_dashboard_cont_log_record);
    lv_obj_set_x(scr_dashboard_cont_log_01, 12);
    lv_obj_set_y(scr_dashboard_cont_log_01, 44);
    lv_obj_set_width(scr_dashboard_cont_log_01, 534);
    lv_obj_set_height(scr_dashboard_cont_log_01, 44);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_log_01, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_log_01, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_log_01 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_01, lv_color_hex(0x1E2426), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_01, 160, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_01, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_01, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_01, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_log_01, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_01, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_01, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_01, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_01, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_cont_log_01 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_01, lv_color_hex(0x3F5250), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_log_01, lv_color_hex(0x1A2827), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_01, 100, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_log_01, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_01, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_01, 80, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_01, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_cont_log_01, 4, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_01, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_01, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_01, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_01, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    ui_flag_modify(scr_dashboard_cont_log_01, LV_OBJ_FLAG_CHECKABLE, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_log_01, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_01, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_01, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_01, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_01, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_time_01
    scr_dashboard_label_time_01 = lv_label_create(scr_dashboard_cont_log_01);
    lv_obj_set_x(scr_dashboard_label_time_01, 27);
    lv_obj_set_y(scr_dashboard_label_time_01, 15);
    lv_obj_set_width(scr_dashboard_label_time_01, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_time_01, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_time_01, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_time_01, "09:42:18");
    lv_label_set_long_mode(scr_dashboard_label_time_01, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_time_01 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_time_01, lv_color_hex(0x718A92), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_time_01, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_time_01, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_type_01
    scr_dashboard_label_type_01 = lv_label_create(scr_dashboard_cont_log_01);
    lv_obj_set_x(scr_dashboard_label_type_01, 157);
    lv_obj_set_y(scr_dashboard_label_type_01, 14);
    lv_obj_set_width(scr_dashboard_label_type_01, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_type_01, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_type_01, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_type_01, "分类识别");
    lv_label_set_long_mode(scr_dashboard_label_type_01, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_type_01 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_type_01, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_type_01, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_type_01, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_result_01
    scr_dashboard_label_result_01 = lv_label_create(scr_dashboard_cont_log_01);
    lv_obj_set_x(scr_dashboard_label_result_01, 296);
    lv_obj_set_y(scr_dashboard_label_result_01, 14);
    lv_obj_set_width(scr_dashboard_label_result_01, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_result_01, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_result_01, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_result_01, "韵达  96%  中");
    lv_label_set_long_mode(scr_dashboard_label_result_01, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_result_01 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_result_01, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_result_01, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_result_01, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_lable_status_01
    scr_dashboard_lable_status_01 = lv_label_create(scr_dashboard_cont_log_01);
    lv_obj_set_x(scr_dashboard_lable_status_01, 462);
    lv_obj_set_y(scr_dashboard_lable_status_01, 14);
    lv_obj_set_width(scr_dashboard_lable_status_01, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_lable_status_01, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_lable_status_01, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_lable_status_01, "成功");
    lv_label_set_long_mode(scr_dashboard_lable_status_01, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_lable_status_01 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_lable_status_01, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_lable_status_01, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_lable_status_01, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_text_log
    scr_dashboard_label_text_log = lv_label_create(scr_dashboard_cont_log_record);
    lv_obj_set_x(scr_dashboard_label_text_log, 26);
    lv_obj_set_y(scr_dashboard_label_text_log, 9);
    lv_obj_set_width(scr_dashboard_label_text_log, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_text_log, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_text_log, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_text_log, "日志记录");
    lv_label_set_long_mode(scr_dashboard_label_text_log, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_text_log - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_text_log, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_text_log, &lv_font_Misans_Heavy_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_text_log, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_under1
    scr_dashboard_cont_under1 = lv_obj_create(scr_dashboard_cont_log_record);
    lv_obj_set_x(scr_dashboard_cont_under1, 36);
    lv_obj_set_y(scr_dashboard_cont_under1, 29);
    lv_obj_set_width(scr_dashboard_cont_under1, 36);
    lv_obj_set_height(scr_dashboard_cont_under1, 2);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_under1, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_under1, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_under1 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_under1, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_under1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_under1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_under1, 999, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_under1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_under1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_under1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_under1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_under1, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_under1, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_under1, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_under1, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_under1, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_cont_log_line
    scr_dashboard_cont_log_line = lv_obj_create(scr_dashboard_cont_log);
    lv_obj_set_x(scr_dashboard_cont_log_line, 13);
    lv_obj_set_y(scr_dashboard_cont_log_line, 12);
    lv_obj_set_width(scr_dashboard_cont_log_line, 980);
    lv_obj_set_height(scr_dashboard_cont_log_line, 40);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_log_line, LV_SCROLLBAR_MODE_ON);
    lv_obj_set_layout(scr_dashboard_cont_log_line, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_log_line - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_line, lv_color_hex(0x232C2C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_log_line, lv_color_hex(0x1A2827), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_line, 108, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_log_line, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_line, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_line, 60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_line, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_log_line, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_line, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_line, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_line, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_line, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_log_line, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_line, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_line, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_line, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_line, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_cont_set
    scr_dashboard_cont_set = lv_obj_create(scr_dashboard_cont_background2);
    lv_obj_set_x(scr_dashboard_cont_set, 3);
    lv_obj_set_y(scr_dashboard_cont_set, 85);
    lv_obj_set_width(scr_dashboard_cont_set, 1010);
    lv_obj_set_height(scr_dashboard_cont_set, 423);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_set, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_set, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_set - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_opa(scr_dashboard_cont_set, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_set, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_set, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_set, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_set, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_set, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_set, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_set, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_set, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_set, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_set, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_cont_bright
    scr_dashboard_cont_bright = lv_obj_create(scr_dashboard_cont_set);
    lv_obj_set_x(scr_dashboard_cont_bright, 257);
    lv_obj_set_y(scr_dashboard_cont_bright, 107);
    lv_obj_set_width(scr_dashboard_cont_bright, 700);
    lv_obj_set_height(scr_dashboard_cont_bright, 65);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_bright, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_bright, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_bright - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_bright, lv_color_hex(0x2D4145), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_bright, lv_color_hex(0x314144), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_bright, 96, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_bright, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_bright, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_bright, 46, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_bright, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_bright, 14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_bright, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_bright, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_bright, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_bright, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_bright, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_bright, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_bright, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_bright, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_bright, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_slider_bright
    scr_dashboard_slider_bright = lv_slider_create(scr_dashboard_cont_bright);
    lv_obj_set_x(scr_dashboard_slider_bright, 139);
    lv_obj_set_y(scr_dashboard_slider_bright, 32);
    lv_obj_set_width(scr_dashboard_slider_bright, 500);
    lv_obj_set_height(scr_dashboard_slider_bright, 8);
    lv_obj_set_scrollbar_mode(scr_dashboard_slider_bright, LV_SCROLLBAR_MODE_OFF);
    lv_slider_set_range(scr_dashboard_slider_bright, 0, 100);
    lv_slider_set_value(scr_dashboard_slider_bright, 20, LV_ANIM_OFF);
    // Add style for scr_dashboard_slider_bright - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_slider_bright, lv_color_hex(0x05161A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_slider_bright, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_slider_bright - LV_PART_INDICATOR | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_slider_bright, lv_color_hex(0x217A96), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_slider_bright, lv_color_hex(0x84D5E0), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_slider_bright, 200, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_slider_bright, LV_GRAD_DIR_HOR, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_slider_bright - LV_PART_KNOB | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_slider_bright, lv_color_hex(0xBDD3D6), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_slider_bright, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_bright
    scr_dashboard_label_bright = lv_label_create(scr_dashboard_cont_bright);
    lv_obj_set_x(scr_dashboard_label_bright, 24);
    lv_obj_set_y(scr_dashboard_label_bright, 10);
    lv_obj_set_width(scr_dashboard_label_bright, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_bright, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_bright, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_bright, "亮度");
    lv_label_set_long_mode(scr_dashboard_label_bright, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_bright - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_bright, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_bright, 164, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_bright, &lv_font_Misans_Heavy_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_bright, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_set_line
    scr_dashboard_cont_set_line = lv_obj_create(scr_dashboard_cont_set);
    lv_obj_set_x(scr_dashboard_cont_set_line, 24);
    lv_obj_set_y(scr_dashboard_cont_set_line, 12);
    lv_obj_set_width(scr_dashboard_cont_set_line, 200);
    lv_obj_set_height(scr_dashboard_cont_set_line, 320);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_set_line, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_set_line, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_set_line - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_set_line, lv_color_hex(0x232C2C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_set_line, lv_color_hex(0x111919), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_set_line, 156, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_set_line, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_set_line, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_set_line, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_set_line, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_set_line, 16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_set_line, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_set_line, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_set_line, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_set_line, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_set_line, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_set_line, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_set_line, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_set_line, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_set_line, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_cont_sys_debug
    scr_dashboard_cont_sys_debug = lv_obj_create(scr_dashboard_cont_background2);
    lv_obj_set_x(scr_dashboard_cont_sys_debug, 3);
    lv_obj_set_y(scr_dashboard_cont_sys_debug, 85);
    lv_obj_set_width(scr_dashboard_cont_sys_debug, 1010);
    lv_obj_set_height(scr_dashboard_cont_sys_debug, 423);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_sys_debug, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_sys_debug, LV_LAYOUT_NONE);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_sys_debug, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_sys_debug, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_sys_debug, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_sys_debug, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_sys_debug, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_sys_debug, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_sys_debug, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_sys_debug, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_sys_debug, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_sys_debug, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_sys_debug, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_sys_debug, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
    create_dashboard_debug_panel(scr_dashboard_cont_sys_debug);
    // Create scr_dashboard_cont_button
    scr_dashboard_cont_button = lv_obj_create(scr_dashboard_cont_background2);
    lv_obj_set_x(scr_dashboard_cont_button, -1);
    lv_obj_set_y(scr_dashboard_cont_button, 523);
    lv_obj_set_width(scr_dashboard_cont_button, 1018);
    lv_obj_set_height(scr_dashboard_cont_button, 70);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_button, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_button, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_button - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_button, lv_color_hex(0x1E2426), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_button, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_button, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_button, 60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_button, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_button, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_button, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_button, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_button, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_button, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_imgbtn_dash
    scr_dashboard_imgbtn_dash = lv_imgbtn_create(scr_dashboard_cont_button);
    lv_obj_set_x(scr_dashboard_imgbtn_dash, 38);
    lv_obj_set_y(scr_dashboard_imgbtn_dash, 1);
    lv_obj_set_width(scr_dashboard_imgbtn_dash, 212);
    lv_obj_set_height(scr_dashboard_imgbtn_dash, 66);
    lv_obj_set_scrollbar_mode(scr_dashboard_imgbtn_dash, LV_SCROLLBAR_MODE_OFF);
    scr_dashboard_imgbtn_dash_label = lv_label_create(scr_dashboard_imgbtn_dash);
    lv_label_set_text(scr_dashboard_imgbtn_dash_label, "");
    lv_label_set_long_mode(scr_dashboard_imgbtn_dash_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(scr_dashboard_imgbtn_dash_label, LV_PCT(100));
    lv_obj_set_align(scr_dashboard_imgbtn_dash_label, LV_ALIGN_CENTER);
    lv_imgbtn_set_src(scr_dashboard_imgbtn_dash, LV_IMGBTN_STATE_RELEASED, NULL, &dash_c_212x66, NULL);
    lv_imgbtn_set_src(scr_dashboard_imgbtn_dash, LV_IMGBTN_STATE_PRESSED, NULL, &dash_c2_212x66, NULL);
    lv_imgbtn_set_src(scr_dashboard_imgbtn_dash, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &dash_a_212x66, NULL);
    lv_obj_add_flag(scr_dashboard_imgbtn_dash, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_set_style_text_align(scr_dashboard_imgbtn_dash, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_imgbtn_dash, LV_OBJ_FLAG_CHECKABLE, UI_FLAG_ACTION_ADD);
    ui_state_modify(scr_dashboard_imgbtn_dash, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
    // Create scr_dashboard_imgbtn_set
    scr_dashboard_imgbtn_set = lv_imgbtn_create(scr_dashboard_cont_button);
    lv_obj_set_x(scr_dashboard_imgbtn_set, 281);
    lv_obj_set_y(scr_dashboard_imgbtn_set, 1);
    lv_obj_set_width(scr_dashboard_imgbtn_set, 212);
    lv_obj_set_height(scr_dashboard_imgbtn_set, 66);
    lv_obj_set_scrollbar_mode(scr_dashboard_imgbtn_set, LV_SCROLLBAR_MODE_OFF);
    scr_dashboard_imgbtn_set_label = lv_label_create(scr_dashboard_imgbtn_set);
    lv_label_set_text(scr_dashboard_imgbtn_set_label, "");
    lv_label_set_long_mode(scr_dashboard_imgbtn_set_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(scr_dashboard_imgbtn_set_label, LV_PCT(100));
    lv_obj_set_align(scr_dashboard_imgbtn_set_label, LV_ALIGN_CENTER);
    lv_imgbtn_set_src(scr_dashboard_imgbtn_set, LV_IMGBTN_STATE_RELEASED, NULL, &setc_212x66, NULL);
    lv_imgbtn_set_src(scr_dashboard_imgbtn_set, LV_IMGBTN_STATE_PRESSED, NULL, &setc1_212x66, NULL);
    lv_obj_add_flag(scr_dashboard_imgbtn_set, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_set_style_text_align(scr_dashboard_imgbtn_set, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_imgbtn_set, LV_OBJ_FLAG_CHECKABLE, UI_FLAG_ACTION_ADD);
    // Create scr_dashboard_imgbtn_log
    scr_dashboard_imgbtn_log = lv_imgbtn_create(scr_dashboard_cont_button);
    lv_obj_set_x(scr_dashboard_imgbtn_log, 524);
    lv_obj_set_y(scr_dashboard_imgbtn_log, 1);
    lv_obj_set_width(scr_dashboard_imgbtn_log, 212);
    lv_obj_set_height(scr_dashboard_imgbtn_log, 66);
    lv_obj_set_scrollbar_mode(scr_dashboard_imgbtn_log, LV_SCROLLBAR_MODE_OFF);
    scr_dashboard_imgbtn_log_label = lv_label_create(scr_dashboard_imgbtn_log);
    lv_label_set_text(scr_dashboard_imgbtn_log_label, "");
    lv_label_set_long_mode(scr_dashboard_imgbtn_log_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(scr_dashboard_imgbtn_log_label, LV_PCT(100));
    lv_obj_set_align(scr_dashboard_imgbtn_log_label, LV_ALIGN_CENTER);
    lv_imgbtn_set_src(scr_dashboard_imgbtn_log, LV_IMGBTN_STATE_RELEASED, NULL, &logc_212x66, NULL);
    lv_imgbtn_set_src(scr_dashboard_imgbtn_log, LV_IMGBTN_STATE_PRESSED, NULL, &logc1_212x66, NULL);
    lv_obj_add_flag(scr_dashboard_imgbtn_log, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_set_style_text_align(scr_dashboard_imgbtn_log, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_imgbtn_log, LV_OBJ_FLAG_CHECKABLE, UI_FLAG_ACTION_ADD);
    // Create scr_dashboard_imgbtn_sys
    scr_dashboard_imgbtn_sys = lv_imgbtn_create(scr_dashboard_cont_button);
    lv_obj_set_x(scr_dashboard_imgbtn_sys, 767);
    lv_obj_set_y(scr_dashboard_imgbtn_sys, 1);
    lv_obj_set_width(scr_dashboard_imgbtn_sys, 212);
    lv_obj_set_height(scr_dashboard_imgbtn_sys, 66);
    lv_obj_set_scrollbar_mode(scr_dashboard_imgbtn_sys, LV_SCROLLBAR_MODE_OFF);
    scr_dashboard_imgbtn_sys_label = lv_label_create(scr_dashboard_imgbtn_sys);
    lv_label_set_text(scr_dashboard_imgbtn_sys_label, "");
    lv_label_set_long_mode(scr_dashboard_imgbtn_sys_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(scr_dashboard_imgbtn_sys_label, LV_PCT(100));
    lv_obj_set_align(scr_dashboard_imgbtn_sys_label, LV_ALIGN_CENTER);
    lv_imgbtn_set_src(scr_dashboard_imgbtn_sys, LV_IMGBTN_STATE_RELEASED, NULL, &sysc_212x66, NULL);
    lv_imgbtn_set_src(scr_dashboard_imgbtn_sys, LV_IMGBTN_STATE_PRESSED, NULL, &sysc1_212x66, NULL);
    lv_obj_add_flag(scr_dashboard_imgbtn_sys, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_set_style_text_align(scr_dashboard_imgbtn_sys, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_imgbtn_sys, LV_OBJ_FLAG_CHECKABLE, UI_FLAG_ACTION_ADD);
    // Create scr_dashboard_cont_dashboard
    scr_dashboard_cont_dashboard = lv_obj_create(scr_dashboard_cont_background2);
    lv_obj_set_x(scr_dashboard_cont_dashboard, 6);
    lv_obj_set_y(scr_dashboard_cont_dashboard, 69);
    lv_obj_set_width(scr_dashboard_cont_dashboard, 1000);
    lv_obj_set_height(scr_dashboard_cont_dashboard, 444);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_dashboard, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_dashboard, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_dashboard - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_dashboard, lv_color_hex(0x0B1517), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_dashboard, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_dashboard, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_dashboard, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_dashboard, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_dashboard, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_dashboard, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_dashboard, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_dashboard, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_dashboard, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_dashboard, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_dashboard, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_cont_container_live
    scr_dashboard_cont_container_live = lv_obj_create(scr_dashboard_cont_dashboard);
    lv_obj_set_x(scr_dashboard_cont_container_live, 4);
    lv_obj_set_y(scr_dashboard_cont_container_live, 5);
    lv_obj_set_width(scr_dashboard_cont_container_live, 640);
    lv_obj_set_height(scr_dashboard_cont_container_live, 410);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_container_live, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_container_live, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_container_live - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_container_live, lv_color_hex(0x232C2C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_container_live, lv_color_hex(0x070A0A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_container_live, 220, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_container_live, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_container_live, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_container_live, 60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_container_live, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_container_live, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_container_live, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_container_live, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_container_live, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_container_live, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_container_live, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_container_live, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_container_live, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_container_live, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_container_live, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_cont_live_vedio
    scr_dashboard_cont_live_vedio = lv_obj_create(scr_dashboard_cont_container_live);
    lv_obj_set_x(scr_dashboard_cont_live_vedio, -1);
    lv_obj_set_y(scr_dashboard_cont_live_vedio, 34);
    lv_obj_set_width(scr_dashboard_cont_live_vedio, 640);
    lv_obj_set_height(scr_dashboard_cont_live_vedio, 375);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_live_vedio, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_live_vedio, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_live_vedio - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_live_vedio, lv_color_hex(0x0B1517), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_live_vedio, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_live_vedio, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_live_vedio, 80, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_live_vedio, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_live_vedio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_live_vedio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_live_vedio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_live_vedio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_live_vedio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_live_vedio, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_live_vedio, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_live_vedio, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_live_vedio, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_live_vedio, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_text_live
    scr_dashboard_label_text_live = lv_label_create(scr_dashboard_cont_container_live);
    lv_obj_set_x(scr_dashboard_label_text_live, 13);
    lv_obj_set_y(scr_dashboard_label_text_live, 9);
    lv_obj_set_width(scr_dashboard_label_text_live, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_text_live, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_text_live, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_text_live, "实时监测");
    lv_label_set_long_mode(scr_dashboard_label_text_live, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_text_live - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_text_live, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_text_live, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(scr_dashboard_label_text_live, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(scr_dashboard_label_text_live, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_text_live, &lv_font_Misans_Heavy_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_text_live, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_detection
    scr_dashboard_cont_detection = lv_obj_create(scr_dashboard_cont_dashboard);
    lv_obj_set_x(scr_dashboard_cont_detection, 652);
    lv_obj_set_y(scr_dashboard_cont_detection, 5);
    lv_obj_set_width(scr_dashboard_cont_detection, 340);
    lv_obj_set_height(scr_dashboard_cont_detection, 88);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_detection, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_detection, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_detection - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_detection, lv_color_hex(0x3F5250), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_detection, lv_color_hex(0x1E2426), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detection, 108, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_detection, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_detection, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_detection, 60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_detection, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_detection, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detection, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detection, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detection, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detection, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_detection, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detection, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detection, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detection, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detection, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_slider_runtime_detection_score
    scr_dashboard_slider_runtime_detection_score = lv_slider_create(scr_dashboard_cont_detection);
    lv_obj_set_x(scr_dashboard_slider_runtime_detection_score, 88);
    lv_obj_set_y(scr_dashboard_slider_runtime_detection_score, 68);
    lv_obj_set_width(scr_dashboard_slider_runtime_detection_score, 230);
    lv_obj_set_height(scr_dashboard_slider_runtime_detection_score, 6);
    lv_obj_set_scrollbar_mode(scr_dashboard_slider_runtime_detection_score, LV_SCROLLBAR_MODE_OFF);
    lv_slider_set_range(scr_dashboard_slider_runtime_detection_score, 0, 100);
    lv_slider_set_value(scr_dashboard_slider_runtime_detection_score, 45, LV_ANIM_OFF);
    // Add style for scr_dashboard_slider_runtime_detection_score - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_slider_runtime_detection_score, lv_color_hex(0x0B0E0E), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_slider_runtime_detection_score, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_slider_runtime_detection_score - LV_PART_INDICATOR | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_slider_runtime_detection_score, lv_color_hex(0x21966F), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_slider_runtime_detection_score, lv_color_hex(0x70D296), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_slider_runtime_detection_score, 200, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_slider_runtime_detection_score, LV_GRAD_DIR_HOR, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_slider_runtime_detection_score - LV_PART_KNOB | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_slider_runtime_detection_score, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_slider_runtime_detection_score, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_slider_runtime_detection_score, LV_OBJ_FLAG_CLICKABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_slider_runtime_detection_score, LV_OBJ_FLAG_SNAPPABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_slider_runtime_detection_score, LV_OBJ_FLAG_CLICK_FOCUSABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_slider_runtime_detection_score, LV_OBJ_FLAG_GESTURE_BUBBLE, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_text_detection
    scr_dashboard_label_text_detection = lv_label_create(scr_dashboard_cont_detection);
    lv_obj_set_x(scr_dashboard_label_text_detection, 23);
    lv_obj_set_y(scr_dashboard_label_text_detection, 9);
    lv_obj_set_width(scr_dashboard_label_text_detection, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_text_detection, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_text_detection, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_text_detection, "检测结果");
    lv_label_set_long_mode(scr_dashboard_label_text_detection, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_text_detection - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_text_detection, lv_color_hex(0x8EA5AD), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_text_detection, &lv_font_Misans_Heavy_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_text_detection, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_runtime_detection_status
    scr_dashboard_label_runtime_detection_status = lv_label_create(scr_dashboard_cont_detection);
    lv_obj_set_x(scr_dashboard_label_runtime_detection_status, 35);
    lv_obj_set_y(scr_dashboard_label_runtime_detection_status, 32);
    lv_obj_set_width(scr_dashboard_label_runtime_detection_status, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_runtime_detection_status, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_runtime_detection_status, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_runtime_detection_status, "NOMAL");
    lv_label_set_long_mode(scr_dashboard_label_runtime_detection_status, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_runtime_detection_status - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_runtime_detection_status, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(scr_dashboard_label_runtime_detection_status, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(scr_dashboard_label_runtime_detection_status, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_runtime_detection_status, &lv_font_MiSans_Heavy_24_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_runtime_detection_status, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_runtime_detection_score
    scr_dashboard_label_runtime_detection_score = lv_label_create(scr_dashboard_cont_detection);
    lv_obj_set_x(scr_dashboard_label_runtime_detection_score, 23);
    lv_obj_set_y(scr_dashboard_label_runtime_detection_score, 64);
    lv_obj_set_width(scr_dashboard_label_runtime_detection_score, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_runtime_detection_score, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_runtime_detection_score, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_runtime_detection_score, "4.7/5.9");
    lv_label_set_long_mode(scr_dashboard_label_runtime_detection_score, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_runtime_detection_score - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_runtime_detection_score, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(scr_dashboard_label_runtime_detection_score, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(scr_dashboard_label_runtime_detection_score, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_runtime_detection_score, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_runtime_detection_score, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_9NI8FzEe
    scr_dashboard_cont_9NI8FzEe = lv_obj_create(scr_dashboard_cont_detection);
    lv_obj_set_x(scr_dashboard_cont_9NI8FzEe, 154);
    lv_obj_set_y(scr_dashboard_cont_9NI8FzEe, 0);
    lv_obj_set_width(scr_dashboard_cont_9NI8FzEe, 180);
    lv_obj_set_height(scr_dashboard_cont_9NI8FzEe, 40);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_9NI8FzEe, LV_SCROLLBAR_MODE_OFF);
    // Add style for scr_dashboard_cont_9NI8FzEe - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_9NI8FzEe, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_9NI8FzEe, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_9NI8FzEe, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_9NI8FzEe, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_9NI8FzEe, lv_color_hex(0x193B51), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_9NI8FzEe, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_9NI8FzEe, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_9NI8FzEe, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_9NI8FzEe, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_9NI8FzEe, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_9NI8FzEe, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_9NI8FzEe, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(scr_dashboard_cont_9NI8FzEe, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(scr_dashboard_cont_9NI8FzEe, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(scr_dashboard_cont_9NI8FzEe, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(scr_dashboard_cont_9NI8FzEe, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(scr_dashboard_cont_9NI8FzEe, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(scr_dashboard_cont_9NI8FzEe, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_btn_4m5N949i
    scr_dashboard_btn_4m5N949i = lv_btn_create(scr_dashboard_cont_detection);
    lv_obj_set_x(scr_dashboard_btn_4m5N949i, 154);
    lv_obj_set_y(scr_dashboard_btn_4m5N949i, 0);
    lv_obj_set_width(scr_dashboard_btn_4m5N949i, 168);
    lv_obj_set_height(scr_dashboard_btn_4m5N949i, 40);
    scr_dashboard_btn_4m5N949i_label = lv_label_create(scr_dashboard_btn_4m5N949i);
    lv_obj_set_scrollbar_mode(scr_dashboard_btn_4m5N949i, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_btn_4m5N949i_label, "");
    lv_obj_set_style_pad_all(scr_dashboard_btn_4m5N949i, 0, LV_STATE_DEFAULT);
    lv_obj_align(scr_dashboard_btn_4m5N949i_label, LV_ALIGN_CENTER, 0, 0);
    // Add style for scr_dashboard_btn_4m5N949i - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_btn_4m5N949i, lv_color_hex(0x2095f5), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_btn_4m5N949i, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_btn_4m5N949i, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_btn_4m5N949i, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_btn_4m5N949i, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_btn_4m5N949i, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_btn_4m5N949i, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_btn_4m5N949i, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(scr_dashboard_btn_4m5N949i, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(scr_dashboard_btn_4m5N949i, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(scr_dashboard_btn_4m5N949i, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(scr_dashboard_btn_4m5N949i, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(scr_dashboard_btn_4m5N949i, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(scr_dashboard_btn_4m5N949i, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(scr_dashboard_btn_4m5N949i, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_btn_4m5N949i, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_img_lFJKr46M
    scr_dashboard_img_lFJKr46M = lv_img_create(scr_dashboard_cont_detection);
    lv_img_set_src(scr_dashboard_img_lFJKr46M, NULL);
    lv_obj_set_x(scr_dashboard_img_lFJKr46M, 286);
    lv_obj_set_y(scr_dashboard_img_lFJKr46M, 10);
    lv_obj_set_width(scr_dashboard_img_lFJKr46M, 20);
    lv_obj_set_height(scr_dashboard_img_lFJKr46M, 20);
    lv_obj_set_scrollbar_mode(scr_dashboard_img_lFJKr46M, LV_SCROLLBAR_MODE_OFF);
    lv_img_set_pivot(scr_dashboard_img_lFJKr46M, 50, 50);
    lv_img_set_angle(scr_dashboard_img_lFJKr46M, 0);
    // Add style for scr_dashboard_img_lFJKr46M - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_img_recolor(scr_dashboard_img_lFJKr46M, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor_opa(scr_dashboard_img_lFJKr46M, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(scr_dashboard_img_lFJKr46M, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_shIaNFSV
    scr_dashboard_label_shIaNFSV = lv_label_create(scr_dashboard_cont_detection);
    lv_obj_set_x(scr_dashboard_label_shIaNFSV, 184);
    lv_obj_set_y(scr_dashboard_label_shIaNFSV, 5);
    lv_obj_set_width(scr_dashboard_label_shIaNFSV, 132);
    lv_obj_set_height(scr_dashboard_label_shIaNFSV, 25);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_shIaNFSV, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_shIaNFSV, "status");
    lv_label_set_long_mode(scr_dashboard_label_shIaNFSV, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_shIaNFSV - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_label_shIaNFSV, lv_color_hex(0x2095f5), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_label_shIaNFSV, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_label_shIaNFSV, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_label_shIaNFSV, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_label_shIaNFSV, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_label_shIaNFSV, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_label_shIaNFSV, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_label_shIaNFSV, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_label_shIaNFSV, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(scr_dashboard_label_shIaNFSV, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(scr_dashboard_label_shIaNFSV, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(scr_dashboard_label_shIaNFSV, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(scr_dashboard_label_shIaNFSV, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(scr_dashboard_label_shIaNFSV, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(scr_dashboard_label_shIaNFSV, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(scr_dashboard_label_shIaNFSV, lv_color_hex(0xC1CBD8), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(scr_dashboard_label_shIaNFSV, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(scr_dashboard_label_shIaNFSV, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_shIaNFSV, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_shIaNFSV, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Init state for statebtn_device@statebtn
    lv_label_set_text(scr_dashboard_label_shIaNFSV, "RUNNING");
    lv_obj_set_style_text_font(scr_dashboard_label_shIaNFSV, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(scr_dashboard_label_shIaNFSV, "STOPPED");
    lv_obj_set_style_text_font(scr_dashboard_label_shIaNFSV, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(scr_dashboard_label_shIaNFSV, "WARNING");
    lv_obj_set_style_text_font(scr_dashboard_label_shIaNFSV, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_img_set_src(scr_dashboard_img_lFJKr46M, &GREEN_20x20);
    lv_label_set_text(scr_dashboard_label_shIaNFSV, "RUNNING");
    lv_obj_add_event_cb(scr_dashboard_btn_4m5N949i, scr_dashboard_statebtn_device_event_handler, LV_EVENT_ALL, NULL);
    // Create scr_dashboard_cont_data
    scr_dashboard_cont_data = lv_obj_create(scr_dashboard_cont_dashboard);
    lv_obj_set_x(scr_dashboard_cont_data, 652);
    lv_obj_set_y(scr_dashboard_cont_data, 204);
    lv_obj_set_width(scr_dashboard_cont_data, 340);
    lv_obj_set_height(scr_dashboard_cont_data, 211);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_data, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_data, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_data - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_data, lv_color_hex(0x454545), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_data, lv_color_hex(0x1E2426), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_data, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_data, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_data, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_data, 40, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_data, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_data, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_data, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_data, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_data, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_data, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_data, LV_OBJ_FLAG_CLICKABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_data, LV_OBJ_FLAG_SNAPPABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_data, LV_OBJ_FLAG_CLICK_FOCUSABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_data, LV_OBJ_FLAG_GESTURE_BUBBLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_data, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_data, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_data, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_data, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_data, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_cont_02
    scr_dashboard_cont_02 = lv_obj_create(scr_dashboard_cont_data);
    lv_obj_set_x(scr_dashboard_cont_02, 174);
    lv_obj_set_y(scr_dashboard_cont_02, 8);
    lv_obj_set_width(scr_dashboard_cont_02, 160);
    lv_obj_set_height(scr_dashboard_cont_02, 60);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_02, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_02, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_02 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_02, lv_color_hex(0x2C3538), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_02, 160, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_02, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_02, 60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_02, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_02, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_02, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_02, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_02, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_02, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_extext_rpm
    scr_dashboard_label_extext_rpm = lv_label_create(scr_dashboard_cont_02);
    lv_obj_set_x(scr_dashboard_label_extext_rpm, 86);
    lv_obj_set_y(scr_dashboard_label_extext_rpm, 40);
    lv_obj_set_width(scr_dashboard_label_extext_rpm, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_extext_rpm, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_extext_rpm, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_extext_rpm, "RPM");
    lv_label_set_long_mode(scr_dashboard_label_extext_rpm, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_extext_rpm - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_extext_rpm, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_extext_rpm, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_extext_rpm, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_extext_rpm, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_runtime_motor_rpm
    scr_dashboard_label_runtime_motor_rpm = lv_label_create(scr_dashboard_cont_02);
    lv_obj_set_x(scr_dashboard_label_runtime_motor_rpm, 32);
    lv_obj_set_y(scr_dashboard_label_runtime_motor_rpm, 28);
    lv_obj_set_width(scr_dashboard_label_runtime_motor_rpm, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_runtime_motor_rpm, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_runtime_motor_rpm, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_runtime_motor_rpm, "104");
    lv_label_set_long_mode(scr_dashboard_label_runtime_motor_rpm, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_runtime_motor_rpm - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_runtime_motor_rpm, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_runtime_motor_rpm, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(scr_dashboard_label_runtime_motor_rpm, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(scr_dashboard_label_runtime_motor_rpm, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_runtime_motor_rpm, &lv_font_MiSans_Heavy_24_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_runtime_motor_rpm, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_text02
    scr_dashboard_label_text02 = lv_label_create(scr_dashboard_cont_02);
    lv_obj_set_x(scr_dashboard_label_text02, 14);
    lv_obj_set_y(scr_dashboard_label_text02, 8);
    lv_obj_set_width(scr_dashboard_label_text02, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_text02, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_text02, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_text02, "电机转速");
    lv_label_set_long_mode(scr_dashboard_label_text02, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_text02 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_text02, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_text02, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_text02, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_text02, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_01
    scr_dashboard_cont_01 = lv_obj_create(scr_dashboard_cont_data);
    lv_obj_set_x(scr_dashboard_cont_01, 4);
    lv_obj_set_y(scr_dashboard_cont_01, 8);
    lv_obj_set_width(scr_dashboard_cont_01, 160);
    lv_obj_set_height(scr_dashboard_cont_01, 60);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_01, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_01, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_01 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_01, lv_color_hex(0x2C3538), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_01, 160, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_01, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_01, 60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_01, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_01, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_01, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_01, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_01, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_01, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_ms
    scr_dashboard_label_ms = lv_label_create(scr_dashboard_cont_01);
    lv_obj_set_x(scr_dashboard_label_ms, 80);
    lv_obj_set_y(scr_dashboard_label_ms, 40);
    lv_obj_set_width(scr_dashboard_label_ms, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_ms, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_ms, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_ms, "ms");
    lv_label_set_long_mode(scr_dashboard_label_ms, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_ms - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_ms, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_ms, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_ms, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_ms, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_runtime_infer_time
    scr_dashboard_label_runtime_infer_time = lv_label_create(scr_dashboard_cont_01);
    lv_obj_set_x(scr_dashboard_label_runtime_infer_time, 42);
    lv_obj_set_y(scr_dashboard_label_runtime_infer_time, 28);
    lv_obj_set_width(scr_dashboard_label_runtime_infer_time, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_runtime_infer_time, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_runtime_infer_time, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_runtime_infer_time, "73");
    lv_label_set_long_mode(scr_dashboard_label_runtime_infer_time, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_runtime_infer_time - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_runtime_infer_time, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_runtime_infer_time, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(scr_dashboard_label_runtime_infer_time, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(scr_dashboard_label_runtime_infer_time, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_runtime_infer_time, &lv_font_MiSans_Heavy_24_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_runtime_infer_time, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_text01
    scr_dashboard_label_text01 = lv_label_create(scr_dashboard_cont_01);
    lv_obj_set_x(scr_dashboard_label_text01, 14);
    lv_obj_set_y(scr_dashboard_label_text01, 8);
    lv_obj_set_width(scr_dashboard_label_text01, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_text01, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_text01, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_text01, "推理耗时");
    lv_label_set_long_mode(scr_dashboard_label_text01, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_text01 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_text01, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_text01, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_text01, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_text01, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_03
    scr_dashboard_cont_03 = lv_obj_create(scr_dashboard_cont_data);
    lv_obj_set_x(scr_dashboard_cont_03, 4);
    lv_obj_set_y(scr_dashboard_cont_03, 77);
    lv_obj_set_width(scr_dashboard_cont_03, 160);
    lv_obj_set_height(scr_dashboard_cont_03, 60);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_03, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_03, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_03 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_03, lv_color_hex(0x2C3538), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_03, 160, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_03, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_03, 60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_03, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_03, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_03, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_03, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_03, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_03, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_runtime_temperature
    scr_dashboard_label_runtime_temperature = lv_label_create(scr_dashboard_cont_03);
    lv_obj_set_x(scr_dashboard_label_runtime_temperature, 42);
    lv_obj_set_y(scr_dashboard_label_runtime_temperature, 29);
    lv_obj_set_width(scr_dashboard_label_runtime_temperature, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_runtime_temperature, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_runtime_temperature, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_runtime_temperature, "28");
    lv_label_set_long_mode(scr_dashboard_label_runtime_temperature, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_runtime_temperature - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_runtime_temperature, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_runtime_temperature, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(scr_dashboard_label_runtime_temperature, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(scr_dashboard_label_runtime_temperature, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_runtime_temperature, &lv_font_MiSans_Heavy_24_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_runtime_temperature, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_text03
    scr_dashboard_label_text03 = lv_label_create(scr_dashboard_cont_03);
    lv_obj_set_x(scr_dashboard_label_text03, 14);
    lv_obj_set_y(scr_dashboard_label_text03, 8);
    lv_obj_set_width(scr_dashboard_label_text03, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_text03, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_text03, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_text03, "设备温度");
    lv_label_set_long_mode(scr_dashboard_label_text03, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_text03 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_text03, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_text03, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_text03, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_text03, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_04
    scr_dashboard_cont_04 = lv_obj_create(scr_dashboard_cont_data);
    lv_obj_set_x(scr_dashboard_cont_04, 174);
    lv_obj_set_y(scr_dashboard_cont_04, 77);
    lv_obj_set_width(scr_dashboard_cont_04, 160);
    lv_obj_set_height(scr_dashboard_cont_04, 60);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_04, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_04, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_04 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_04, lv_color_hex(0x2C3538), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_04, 160, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_04, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_04, 60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_04, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_04, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_04, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_04, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_04, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_04, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_runtime_current_type
    scr_dashboard_label_runtime_current_type = lv_label_create(scr_dashboard_cont_04);
    lv_obj_set_x(scr_dashboard_label_runtime_current_type, 30);
    lv_obj_set_y(scr_dashboard_label_runtime_current_type, 28);
    lv_obj_set_width(scr_dashboard_label_runtime_current_type, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_runtime_current_type, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_runtime_current_type, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_runtime_current_type, "极兔·中");
    lv_label_set_long_mode(scr_dashboard_label_runtime_current_type, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_runtime_current_type - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_runtime_current_type, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_runtime_current_type, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(scr_dashboard_label_runtime_current_type, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(scr_dashboard_label_runtime_current_type, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_runtime_current_type, &lv_font_MiSans_Heavy_24_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_runtime_current_type, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_text04
    scr_dashboard_label_text04 = lv_label_create(scr_dashboard_cont_04);
    lv_obj_set_x(scr_dashboard_label_text04, 14);
    lv_obj_set_y(scr_dashboard_label_text04, 8);
    lv_obj_set_width(scr_dashboard_label_text04, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_text04, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_text04, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_text04, "当前识别类型");
    lv_label_set_long_mode(scr_dashboard_label_text04, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_text04 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_text04, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_text04, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_text04, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_text04, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_05
    scr_dashboard_cont_05 = lv_obj_create(scr_dashboard_cont_data);
    lv_obj_set_x(scr_dashboard_cont_05, 4);
    lv_obj_set_y(scr_dashboard_cont_05, 145);
    lv_obj_set_width(scr_dashboard_cont_05, 160);
    lv_obj_set_height(scr_dashboard_cont_05, 60);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_05, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_05, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_05 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_05, lv_color_hex(0x2C3538), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_05, 160, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_05, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_05, 60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_05, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_05, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_05, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_05, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_05, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_05, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_runtime_total_count
    scr_dashboard_label_runtime_total_count = lv_label_create(scr_dashboard_cont_05);
    lv_obj_set_x(scr_dashboard_label_runtime_total_count, 41);
    lv_obj_set_y(scr_dashboard_label_runtime_total_count, 29);
    lv_obj_set_width(scr_dashboard_label_runtime_total_count, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_runtime_total_count, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_runtime_total_count, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_runtime_total_count, "12");
    lv_label_set_long_mode(scr_dashboard_label_runtime_total_count, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_runtime_total_count - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_runtime_total_count, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_runtime_total_count, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(scr_dashboard_label_runtime_total_count, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(scr_dashboard_label_runtime_total_count, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_runtime_total_count, &lv_font_MiSans_Heavy_24_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_runtime_total_count, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_text05
    scr_dashboard_label_text05 = lv_label_create(scr_dashboard_cont_05);
    lv_obj_set_x(scr_dashboard_label_text05, 14);
    lv_obj_set_y(scr_dashboard_label_text05, 8);
    lv_obj_set_width(scr_dashboard_label_text05, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_text05, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_text05, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_text05, "累计分拣件数");
    lv_label_set_long_mode(scr_dashboard_label_text05, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_text05 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_text05, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_text05, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_text05, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_text05, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_06
    scr_dashboard_cont_06 = lv_obj_create(scr_dashboard_cont_data);
    lv_obj_set_x(scr_dashboard_cont_06, 174);
    lv_obj_set_y(scr_dashboard_cont_06, 145);
    lv_obj_set_width(scr_dashboard_cont_06, 160);
    lv_obj_set_height(scr_dashboard_cont_06, 60);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_06, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_06, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_06 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_06, lv_color_hex(0x2C3538), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_06, 160, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_06, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_06, 60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_06, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_06, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_06, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_06, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_06, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_06, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_extext_fps
    scr_dashboard_label_extext_fps = lv_label_create(scr_dashboard_cont_06);
    lv_obj_set_x(scr_dashboard_label_extext_fps, 80);
    lv_obj_set_y(scr_dashboard_label_extext_fps, 40);
    lv_obj_set_width(scr_dashboard_label_extext_fps, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_extext_fps, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_extext_fps, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_extext_fps, "FPS");
    lv_label_set_long_mode(scr_dashboard_label_extext_fps, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_extext_fps - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_extext_fps, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_extext_fps, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_extext_fps, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_extext_fps, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_runtime_fps
    scr_dashboard_label_runtime_fps = lv_label_create(scr_dashboard_cont_06);
    lv_obj_set_x(scr_dashboard_label_runtime_fps, 42);
    lv_obj_set_y(scr_dashboard_label_runtime_fps, 28);
    lv_obj_set_width(scr_dashboard_label_runtime_fps, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_runtime_fps, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_runtime_fps, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_runtime_fps, "60");
    lv_label_set_long_mode(scr_dashboard_label_runtime_fps, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_runtime_fps - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_runtime_fps, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_runtime_fps, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(scr_dashboard_label_runtime_fps, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(scr_dashboard_label_runtime_fps, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_runtime_fps, &lv_font_MiSans_Heavy_24_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_runtime_fps, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_text06
    scr_dashboard_label_text06 = lv_label_create(scr_dashboard_cont_06);
    lv_obj_set_x(scr_dashboard_label_text06, 14);
    lv_obj_set_y(scr_dashboard_label_text06, 8);
    lv_obj_set_width(scr_dashboard_label_text06, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_text06, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_text06, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_text06, "实时帧率");
    lv_label_set_long_mode(scr_dashboard_label_text06, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_text06 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_text06, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_text06, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_text06, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_text06, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_circle_data
    scr_dashboard_cont_circle_data = lv_obj_create(scr_dashboard_cont_dashboard);
    lv_obj_set_x(scr_dashboard_cont_circle_data, 652);
    lv_obj_set_y(scr_dashboard_cont_circle_data, 100);
    lv_obj_set_width(scr_dashboard_cont_circle_data, 340);
    lv_obj_set_height(scr_dashboard_cont_circle_data, 104);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_circle_data, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_circle_data, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_circle_data - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_circle_data, lv_color_hex(0x1E2426), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_circle_data, lv_color_hex(0x454545), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_circle_data, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_circle_data, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_circle_data, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_circle_data, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_circle_data, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_circle_data, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_circle_data, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_circle_data, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_circle_data, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_circle_data, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_circle_data, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_circle_data, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_cont_confidence
    scr_dashboard_cont_confidence = lv_obj_create(scr_dashboard_cont_circle_data);
    lv_obj_set_x(scr_dashboard_cont_confidence, 119);
    lv_obj_set_y(scr_dashboard_cont_confidence, -2);
    lv_obj_set_width(scr_dashboard_cont_confidence, 100);
    lv_obj_set_height(scr_dashboard_cont_confidence, 104);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_confidence, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_confidence, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_confidence - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_confidence, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_confidence, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_confidence, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_confidence, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_confidence, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_confidence, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_confidence, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_confidence, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_confidence, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_confidence, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_confidence, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_confidence, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_text_confidence
    scr_dashboard_label_text_confidence = lv_label_create(scr_dashboard_cont_confidence);
    lv_obj_set_x(scr_dashboard_label_text_confidence, 8);
    lv_obj_set_y(scr_dashboard_label_text_confidence, 86);
    lv_obj_set_width(scr_dashboard_label_text_confidence, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_text_confidence, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_text_confidence, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_text_confidence, "CONFIDENCE");
    lv_label_set_long_mode(scr_dashboard_label_text_confidence, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_text_confidence - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_text_confidence, lv_color_hex(0x7F98A0), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_text_confidence, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_text_confidence, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_arc_runtime_success_rate
    scr_dashboard_arc_runtime_success_rate = lv_arc_create(scr_dashboard_cont_confidence);
    lv_obj_set_x(scr_dashboard_arc_runtime_success_rate, 8);
    lv_obj_set_y(scr_dashboard_arc_runtime_success_rate, 8);
    lv_obj_set_width(scr_dashboard_arc_runtime_success_rate, 80);
    lv_obj_set_height(scr_dashboard_arc_runtime_success_rate, 80);
    lv_obj_set_scrollbar_mode(scr_dashboard_arc_runtime_success_rate, LV_SCROLLBAR_MODE_OFF);
    lv_arc_set_mode(scr_dashboard_arc_runtime_success_rate, LV_ARC_MODE_NORMAL);
    lv_arc_set_range(scr_dashboard_arc_runtime_success_rate, 0, 100);
    lv_arc_set_bg_angles(scr_dashboard_arc_runtime_success_rate, 135, 45);
    lv_arc_set_value(scr_dashboard_arc_runtime_success_rate, 96);
    lv_arc_set_rotation(scr_dashboard_arc_runtime_success_rate, 0);
    scr_dashboard_arc_runtime_success_rate_label = lv_label_create(scr_dashboard_arc_runtime_success_rate);
    lv_label_set_text_fmt(scr_dashboard_arc_runtime_success_rate_label, "%ld", lv_arc_get_value(scr_dashboard_arc_runtime_success_rate));
    lv_arc_align_obj_to_angle(scr_dashboard_arc_runtime_success_rate, scr_dashboard_arc_runtime_success_rate_label, 25);
    lv_obj_add_flag(scr_dashboard_arc_runtime_success_rate_label, LV_OBJ_FLAG_HIDDEN);
    // Add style for scr_dashboard_arc_runtime_success_rate - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_arc_color(scr_dashboard_arc_runtime_success_rate, lv_color_hex(0x454545), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(scr_dashboard_arc_runtime_success_rate, 155, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(scr_dashboard_arc_runtime_success_rate, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(scr_dashboard_arc_runtime_success_rate, true, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_arc_runtime_success_rate - LV_PART_INDICATOR | LV_STATE_DEFAULT
    lv_obj_set_style_arc_color(scr_dashboard_arc_runtime_success_rate, lv_color_hex(0x71CF97), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(scr_dashboard_arc_runtime_success_rate, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(scr_dashboard_arc_runtime_success_rate, 6, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(scr_dashboard_arc_runtime_success_rate, true, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_arc_runtime_success_rate - LV_PART_KNOB | LV_STATE_DEFAULT
    lv_obj_set_style_bg_opa(scr_dashboard_arc_runtime_success_rate, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_arc_runtime_success_rate, LV_OBJ_FLAG_CLICKABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_arc_runtime_success_rate, LV_OBJ_FLAG_SNAPPABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_arc_runtime_success_rate, LV_OBJ_FLAG_CLICK_FOCUSABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_arc_runtime_success_rate, LV_OBJ_FLAG_GESTURE_BUBBLE, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_runtime_success_rate
    scr_dashboard_label_runtime_success_rate = lv_label_create(scr_dashboard_cont_confidence);
    lv_obj_set_x(scr_dashboard_label_runtime_success_rate, 34);
    lv_obj_set_y(scr_dashboard_label_runtime_success_rate, 41);
    lv_obj_set_width(scr_dashboard_label_runtime_success_rate, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_runtime_success_rate, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_runtime_success_rate, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_runtime_success_rate, "96%");
    lv_label_set_long_mode(scr_dashboard_label_runtime_success_rate, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_runtime_success_rate - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_runtime_success_rate, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_runtime_success_rate, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_runtime_success_rate, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_runtime_success_rate, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_cpu
    scr_dashboard_cont_cpu = lv_obj_create(scr_dashboard_cont_circle_data);
    lv_obj_set_x(scr_dashboard_cont_cpu, 11);
    lv_obj_set_y(scr_dashboard_cont_cpu, -2);
    lv_obj_set_width(scr_dashboard_cont_cpu, 100);
    lv_obj_set_height(scr_dashboard_cont_cpu, 104);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_cpu, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_cpu, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_cpu - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_cpu, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_cpu, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_cpu, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_cpu, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_cpu, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_cpu, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_cpu, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_cpu, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_cpu, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_cpu, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_cpu, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_cpu, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_text_cpu
    scr_dashboard_label_text_cpu = lv_label_create(scr_dashboard_cont_cpu);
    lv_obj_set_x(scr_dashboard_label_text_cpu, 35);
    lv_obj_set_y(scr_dashboard_label_text_cpu, 86);
    lv_obj_set_width(scr_dashboard_label_text_cpu, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_text_cpu, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_text_cpu, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_text_cpu, "CPU");
    lv_label_set_long_mode(scr_dashboard_label_text_cpu, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_text_cpu - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_text_cpu, lv_color_hex(0x7F98A0), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_text_cpu, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_text_cpu, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_arc_runtime_cpu_usage
    scr_dashboard_arc_runtime_cpu_usage = lv_arc_create(scr_dashboard_cont_cpu);
    lv_obj_set_x(scr_dashboard_arc_runtime_cpu_usage, 8);
    lv_obj_set_y(scr_dashboard_arc_runtime_cpu_usage, 8);
    lv_obj_set_width(scr_dashboard_arc_runtime_cpu_usage, 80);
    lv_obj_set_height(scr_dashboard_arc_runtime_cpu_usage, 80);
    lv_obj_set_scrollbar_mode(scr_dashboard_arc_runtime_cpu_usage, LV_SCROLLBAR_MODE_OFF);
    lv_arc_set_mode(scr_dashboard_arc_runtime_cpu_usage, LV_ARC_MODE_NORMAL);
    lv_arc_set_range(scr_dashboard_arc_runtime_cpu_usage, 0, 100);
    lv_arc_set_bg_angles(scr_dashboard_arc_runtime_cpu_usage, 135, 45);
    lv_arc_set_value(scr_dashboard_arc_runtime_cpu_usage, 52);
    lv_arc_set_rotation(scr_dashboard_arc_runtime_cpu_usage, 0);
    scr_dashboard_arc_runtime_cpu_usage_label = lv_label_create(scr_dashboard_arc_runtime_cpu_usage);
    lv_label_set_text_fmt(scr_dashboard_arc_runtime_cpu_usage_label, "%ld", lv_arc_get_value(scr_dashboard_arc_runtime_cpu_usage));
    lv_arc_align_obj_to_angle(scr_dashboard_arc_runtime_cpu_usage, scr_dashboard_arc_runtime_cpu_usage_label, 25);
    lv_obj_add_flag(scr_dashboard_arc_runtime_cpu_usage_label, LV_OBJ_FLAG_HIDDEN);
    // Add style for scr_dashboard_arc_runtime_cpu_usage - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_arc_color(scr_dashboard_arc_runtime_cpu_usage, lv_color_hex(0x454545), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(scr_dashboard_arc_runtime_cpu_usage, 155, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(scr_dashboard_arc_runtime_cpu_usage, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(scr_dashboard_arc_runtime_cpu_usage, true, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_arc_runtime_cpu_usage - LV_PART_INDICATOR | LV_STATE_DEFAULT
    lv_obj_set_style_arc_color(scr_dashboard_arc_runtime_cpu_usage, lv_color_hex(0x3FB2D1), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(scr_dashboard_arc_runtime_cpu_usage, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(scr_dashboard_arc_runtime_cpu_usage, 6, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(scr_dashboard_arc_runtime_cpu_usage, true, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_arc_runtime_cpu_usage - LV_PART_KNOB | LV_STATE_DEFAULT
    lv_obj_set_style_bg_opa(scr_dashboard_arc_runtime_cpu_usage, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_arc_runtime_cpu_usage, LV_OBJ_FLAG_CLICKABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_arc_runtime_cpu_usage, LV_OBJ_FLAG_SNAPPABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_arc_runtime_cpu_usage, LV_OBJ_FLAG_CLICK_FOCUSABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_arc_runtime_cpu_usage, LV_OBJ_FLAG_GESTURE_BUBBLE, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_runtime_cpu_usage
    scr_dashboard_label_runtime_cpu_usage = lv_label_create(scr_dashboard_cont_cpu);
    lv_obj_set_x(scr_dashboard_label_runtime_cpu_usage, 34);
    lv_obj_set_y(scr_dashboard_label_runtime_cpu_usage, 41);
    lv_obj_set_width(scr_dashboard_label_runtime_cpu_usage, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_runtime_cpu_usage, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_runtime_cpu_usage, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_runtime_cpu_usage, "42%");
    lv_label_set_long_mode(scr_dashboard_label_runtime_cpu_usage, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_runtime_cpu_usage - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_runtime_cpu_usage, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_runtime_cpu_usage, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_runtime_cpu_usage, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_runtime_cpu_usage, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_memory
    scr_dashboard_cont_memory = lv_obj_create(scr_dashboard_cont_circle_data);
    lv_obj_set_x(scr_dashboard_cont_memory, 226);
    lv_obj_set_y(scr_dashboard_cont_memory, -2);
    lv_obj_set_width(scr_dashboard_cont_memory, 100);
    lv_obj_set_height(scr_dashboard_cont_memory, 104);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_memory, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_memory, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_memory - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_memory, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_memory, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_memory, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_memory, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_memory, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_memory, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_memory, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_memory, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_memory, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_memory, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_memory, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_memory, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_text_memory
    scr_dashboard_label_text_memory = lv_label_create(scr_dashboard_cont_memory);
    lv_obj_set_x(scr_dashboard_label_text_memory, 21);
    lv_obj_set_y(scr_dashboard_label_text_memory, 86);
    lv_obj_set_width(scr_dashboard_label_text_memory, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_text_memory, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_text_memory, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_text_memory, "MEMORY");
    lv_label_set_long_mode(scr_dashboard_label_text_memory, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_text_memory - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_text_memory, lv_color_hex(0x7F98A0), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_text_memory, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_text_memory, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_arc_runtime_memory_usage
    scr_dashboard_arc_runtime_memory_usage = lv_arc_create(scr_dashboard_cont_memory);
    lv_obj_set_x(scr_dashboard_arc_runtime_memory_usage, 8);
    lv_obj_set_y(scr_dashboard_arc_runtime_memory_usage, 8);
    lv_obj_set_width(scr_dashboard_arc_runtime_memory_usage, 80);
    lv_obj_set_height(scr_dashboard_arc_runtime_memory_usage, 80);
    lv_obj_set_scrollbar_mode(scr_dashboard_arc_runtime_memory_usage, LV_SCROLLBAR_MODE_OFF);
    lv_arc_set_mode(scr_dashboard_arc_runtime_memory_usage, LV_ARC_MODE_NORMAL);
    lv_arc_set_range(scr_dashboard_arc_runtime_memory_usage, 0, 100);
    lv_arc_set_bg_angles(scr_dashboard_arc_runtime_memory_usage, 135, 45);
    lv_arc_set_value(scr_dashboard_arc_runtime_memory_usage, 68);
    lv_arc_set_rotation(scr_dashboard_arc_runtime_memory_usage, 0);
    scr_dashboard_arc_runtime_memory_usage_label = lv_label_create(scr_dashboard_arc_runtime_memory_usage);
    lv_label_set_text_fmt(scr_dashboard_arc_runtime_memory_usage_label, "%ld", lv_arc_get_value(scr_dashboard_arc_runtime_memory_usage));
    lv_arc_align_obj_to_angle(scr_dashboard_arc_runtime_memory_usage, scr_dashboard_arc_runtime_memory_usage_label, 25);
    lv_obj_add_flag(scr_dashboard_arc_runtime_memory_usage_label, LV_OBJ_FLAG_HIDDEN);
    // Add style for scr_dashboard_arc_runtime_memory_usage - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_arc_color(scr_dashboard_arc_runtime_memory_usage, lv_color_hex(0x454545), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(scr_dashboard_arc_runtime_memory_usage, 155, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(scr_dashboard_arc_runtime_memory_usage, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(scr_dashboard_arc_runtime_memory_usage, true, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_arc_runtime_memory_usage - LV_PART_INDICATOR | LV_STATE_DEFAULT
    lv_obj_set_style_arc_color(scr_dashboard_arc_runtime_memory_usage, lv_color_hex(0xEBE186), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(scr_dashboard_arc_runtime_memory_usage, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(scr_dashboard_arc_runtime_memory_usage, 6, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(scr_dashboard_arc_runtime_memory_usage, true, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_arc_runtime_memory_usage - LV_PART_KNOB | LV_STATE_DEFAULT
    lv_obj_set_style_bg_opa(scr_dashboard_arc_runtime_memory_usage, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_arc_runtime_memory_usage, LV_OBJ_FLAG_CLICKABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_arc_runtime_memory_usage, LV_OBJ_FLAG_SNAPPABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_arc_runtime_memory_usage, LV_OBJ_FLAG_CLICK_FOCUSABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_arc_runtime_memory_usage, LV_OBJ_FLAG_GESTURE_BUBBLE, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_runtime_memory_usage
    scr_dashboard_label_runtime_memory_usage = lv_label_create(scr_dashboard_cont_memory);
    lv_obj_set_x(scr_dashboard_label_runtime_memory_usage, 34);
    lv_obj_set_y(scr_dashboard_label_runtime_memory_usage, 41);
    lv_obj_set_width(scr_dashboard_label_runtime_memory_usage, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_runtime_memory_usage, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_runtime_memory_usage, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_runtime_memory_usage, "68%");
    lv_label_set_long_mode(scr_dashboard_label_runtime_memory_usage, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_runtime_memory_usage - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_runtime_memory_usage, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_runtime_memory_usage, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_runtime_memory_usage, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_runtime_memory_usage, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_img_title
    scr_dashboard_img_title = lv_img_create(scr_dashboard_cont_background2);
    lv_img_set_src(scr_dashboard_img_title, &title_1_800x60);
    lv_obj_set_x(scr_dashboard_img_title, -2);
    lv_obj_set_y(scr_dashboard_img_title, 0);
    lv_obj_set_width(scr_dashboard_img_title, 800);
    lv_obj_set_height(scr_dashboard_img_title, 60);
    lv_obj_set_scrollbar_mode(scr_dashboard_img_title, LV_SCROLLBAR_MODE_OFF);
    lv_img_set_pivot(scr_dashboard_img_title, 50, 50);
    lv_img_set_angle(scr_dashboard_img_title, 0);
    // Create scr_dashboard_cont_esp32
    scr_dashboard_cont_esp32 = lv_obj_create(scr_dashboard_cont_background2);
    lv_obj_set_x(scr_dashboard_cont_esp32, 787);
    lv_obj_set_y(scr_dashboard_cont_esp32, 16);
    lv_obj_set_width(scr_dashboard_cont_esp32, 84);
    lv_obj_set_height(scr_dashboard_cont_esp32, 28);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_esp32, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_esp32, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_esp32 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_esp32, lv_color_hex(0x454545), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_esp32, 60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_esp32, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_esp32, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_esp32, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_esp32, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_esp32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_esp32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_esp32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_esp32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_esp32, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_esp32, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_esp32, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_esp32, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_esp32, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_esp32
    scr_dashboard_label_esp32 = lv_label_create(scr_dashboard_cont_esp32);
    lv_obj_set_x(scr_dashboard_label_esp32, 12);
    lv_obj_set_y(scr_dashboard_label_esp32, 8);
    lv_obj_set_width(scr_dashboard_label_esp32, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_esp32, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_esp32, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_esp32, "ESP32-P4");
    lv_label_set_long_mode(scr_dashboard_label_esp32, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_esp32 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_esp32, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_esp32, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_esp32, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_esp32, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_edge
    scr_dashboard_cont_edge = lv_obj_create(scr_dashboard_cont_background2);
    lv_obj_set_x(scr_dashboard_cont_edge, 883);
    lv_obj_set_y(scr_dashboard_cont_edge, 16);
    lv_obj_set_width(scr_dashboard_cont_edge, 118);
    lv_obj_set_height(scr_dashboard_cont_edge, 28);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_edge, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_edge, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_edge - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_edge, lv_color_hex(0x0B242B), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_edge, 140, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_edge, lv_color_hex(0x70D296), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_edge, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_edge, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_edge, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_edge, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_edge, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_edge, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_edge, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_edge, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_edge, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_edge, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_edge, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_edge, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_text_edge
    scr_dashboard_label_text_edge = lv_label_create(scr_dashboard_cont_edge);
    lv_obj_set_x(scr_dashboard_label_text_edge, 29);
    lv_obj_set_y(scr_dashboard_label_text_edge, 8);
    lv_obj_set_width(scr_dashboard_label_text_edge, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_text_edge, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_text_edge, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_text_edge, "Edge Online");
    lv_label_set_long_mode(scr_dashboard_label_text_edge, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_text_edge - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_text_edge, lv_color_hex(0x70D296), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_text_edge, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_text_edge, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_text_edge, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_led_edge
    scr_dashboard_led_edge = lv_led_create(scr_dashboard_cont_edge);
    lv_obj_set_pos(scr_dashboard_led_edge, 13, 12);
    lv_obj_set_size(scr_dashboard_led_edge, 4, 4);
    lv_obj_set_scrollbar_mode(scr_dashboard_led_edge, LV_SCROLLBAR_MODE_OFF);
    lv_led_set_brightness(scr_dashboard_led_edge, 160);
    lv_led_set_color(scr_dashboard_led_edge, lv_color_hex(0x70D296));
    return scr_dashboard;
}
lv_obj_t * setup_scr_dashboard(void) {
    if (scr_dashboard != NULL) {
        init_states();
        return scr_dashboard;
    }
    create_ui();
    register_ui_events();
    register_sys_events(&scr_dashboard_event_table);
    init_states();
    return scr_dashboard;
}
