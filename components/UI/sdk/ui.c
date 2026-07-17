/*
 * ui.c - Animation helpers for anyui SDK
 *
 * This file is created and owned by anyui.
 *
 * COPYRIGHT 2025 anyui Team
 * All rights reserved.
 *
 * https://anyui.tech/
 *
 * Author: anyui Team
 */
#include "ui.h"
#include "ethernet_app.h"
#include "bsp_cam_sensor.h"
#include "esp_log.h"
#include "esp_system.h"
#include "lvgl_private.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

LV_FONT_DECLARE(lv_font_MiSans_Heavy_16_16);

extern lv_obj_t *scr_dashboard;
extern lv_obj_t *scr_dashboard_sw_detect;
extern lv_obj_t *scr_dashboard_sw_preview_overlay;
extern lv_obj_t *scr_dashboard_btn_open_motor;
extern lv_obj_t *scr_dashboard_btn_open_motor_label;
extern lv_obj_t *scr_dashboard_slider_A_speed;
extern lv_obj_t *scr_dashboard_label_A_speed_value;
extern lv_obj_t *scr_dashboard_slider_B_speed;
extern lv_obj_t *scr_dashboard_label_B_speed_value;
extern lv_obj_t *scr_dashboard_slider_C_speed;
extern lv_obj_t *scr_dashboard_label_C_speed_value;
extern lv_obj_t *scr_dashboard_slider_confidence_threshold_mian;
extern lv_obj_t *scr_dashboard_label_confidence_value_mian;
extern lv_obj_t *scr_dashboard_slider_confidence_threshold_logo;
extern lv_obj_t *scr_dashboard_label_confidence_value_logo;
extern lv_obj_t *scr_dashboard_btn_restart;
extern lv_obj_t *scr_dashboard_cont_display;
extern lv_obj_t *scr_dashboard_slider_Luminance;
extern lv_obj_t *scr_dashboard_label_runtime_Luminance_data;
extern lv_obj_t *scr_dashboard_slider_contrast;
extern lv_obj_t *scr_dashboard_label_contrast_value;
extern lv_obj_t *scr_dashboard_btn_contrast_auto;
extern lv_obj_t *scr_dashboard_slider_saturation;
extern lv_obj_t *scr_dashboard_label_saturation_value;
extern lv_obj_t *scr_dashboard_btn_saturation_auto;
extern lv_obj_t *scr_dashboard_slider_hue;
extern lv_obj_t *scr_dashboard_label_hue_value;
extern lv_obj_t *scr_dashboard_label_exposure_value;
extern lv_obj_t *scr_dashboard_label_gain_value;
extern lv_obj_t *scr_dashboard_label_wb_value;
extern lv_obj_t *scr_dashboard_label_runtime_confidence_threshole_mian;
extern lv_obj_t *scr_dashboard_label_runtime_confidence_threshole_logo;
extern lv_obj_t *scr_dashboard_cont_log_01;
extern lv_obj_t *scr_dashboard_cont_log_02;
extern lv_obj_t *scr_dashboard_cont_log_03;
extern lv_obj_t *scr_dashboard_cont_log_04;
extern lv_obj_t *scr_dashboard_cont_log_05;
extern lv_obj_t *scr_dashboard_cont_log_06;
extern lv_obj_t *scr_dashboard_cont_log_07;
extern lv_obj_t *scr_dashboard_cont_log_08;
extern lv_obj_t *scr_dashboard_cont_log_09;
extern lv_obj_t *scr_dashboard_cont_log_10;
extern lv_obj_t *scr_dashboard_cont_log_11;
extern lv_obj_t *scr_dashboard_cont_log_12;
extern lv_obj_t *scr_dashboard_cont_log_13;
extern lv_obj_t *scr_dashboard_cont_log_14;
extern lv_obj_t *scr_dashboard_cont_log_15;
extern lv_obj_t *scr_dashboard_cont_log_16;
extern lv_obj_t *scr_dashboard_cont_log_17;
extern lv_obj_t *scr_dashboard_cont_log_18;
extern lv_obj_t *scr_dashboard_cont_log_19;
extern lv_obj_t *scr_dashboard_cont_log_20;

#define UI_LOG_WIDGET_EXTERNS(n) \
    extern lv_obj_t *scr_dashboard_label_time_##n; \
    extern lv_obj_t *scr_dashboard_label_company_##n; \
    extern lv_obj_t *scr_dashboard_label_result_##n; \
    extern lv_obj_t *scr_dashboard_label_confidence_##n; \
    extern lv_obj_t *scr_dashboard_label_detail_time_##n; \
    extern lv_obj_t *scr_dashboard_label_detail_result_##n; \
    extern lv_obj_t *scr_dashboard_label_logo_conf_##n; \
    extern lv_obj_t *scr_dashboard_label_mian_conf_##n; \
    extern lv_obj_t *scr_dashboard_label_logo_inft_##n; \
    extern lv_obj_t *scr_dashboard_label_mian_inft_##n; \
    extern lv_obj_t *scr_dashboard_label_detail_badge_##n

UI_LOG_WIDGET_EXTERNS(01);
UI_LOG_WIDGET_EXTERNS(02);
UI_LOG_WIDGET_EXTERNS(03);
UI_LOG_WIDGET_EXTERNS(04);
UI_LOG_WIDGET_EXTERNS(05);
UI_LOG_WIDGET_EXTERNS(06);
UI_LOG_WIDGET_EXTERNS(07);
UI_LOG_WIDGET_EXTERNS(08);
UI_LOG_WIDGET_EXTERNS(09);
UI_LOG_WIDGET_EXTERNS(10);
UI_LOG_WIDGET_EXTERNS(11);
UI_LOG_WIDGET_EXTERNS(12);
UI_LOG_WIDGET_EXTERNS(13);
UI_LOG_WIDGET_EXTERNS(14);
UI_LOG_WIDGET_EXTERNS(15);
UI_LOG_WIDGET_EXTERNS(16);
UI_LOG_WIDGET_EXTERNS(17);
UI_LOG_WIDGET_EXTERNS(18);
UI_LOG_WIDGET_EXTERNS(19);
UI_LOG_WIDGET_EXTERNS(20);

#undef UI_LOG_WIDGET_EXTERNS

#define UI_LOG_CAPACITY 20

typedef struct {
    lv_obj_t **time;
    lv_obj_t **company;
    lv_obj_t **result;
    lv_obj_t **confidence;
    lv_obj_t **detail_time;
    lv_obj_t **detail_result;
    lv_obj_t **logo_confidence;
    lv_obj_t **waybill_confidence;
    lv_obj_t **logo_infer_time;
    lv_obj_t **waybill_infer_time;
    lv_obj_t **badge;
} ui_log_widgets_t;

#define UI_LOG_WIDGETS(n) { \
    &scr_dashboard_label_time_##n, \
    &scr_dashboard_label_company_##n, \
    &scr_dashboard_label_result_##n, \
    &scr_dashboard_label_confidence_##n, \
    &scr_dashboard_label_detail_time_##n, \
    &scr_dashboard_label_detail_result_##n, \
    &scr_dashboard_label_logo_conf_##n, \
    &scr_dashboard_label_mian_conf_##n, \
    &scr_dashboard_label_logo_inft_##n, \
    &scr_dashboard_label_mian_inft_##n, \
    &scr_dashboard_label_detail_badge_##n \
}

static const ui_log_widgets_t s_log_widgets[UI_LOG_CAPACITY] = {
    UI_LOG_WIDGETS(01), UI_LOG_WIDGETS(02), UI_LOG_WIDGETS(03), UI_LOG_WIDGETS(04),
    UI_LOG_WIDGETS(05), UI_LOG_WIDGETS(06), UI_LOG_WIDGETS(07), UI_LOG_WIDGETS(08),
    UI_LOG_WIDGETS(09), UI_LOG_WIDGETS(10), UI_LOG_WIDGETS(11), UI_LOG_WIDGETS(12),
    UI_LOG_WIDGETS(13), UI_LOG_WIDGETS(14), UI_LOG_WIDGETS(15), UI_LOG_WIDGETS(16),
    UI_LOG_WIDGETS(17), UI_LOG_WIDGETS(18), UI_LOG_WIDGETS(19), UI_LOG_WIDGETS(20),
};

#undef UI_LOG_WIDGETS

static vision_log_event_data_t s_vision_logs[UI_LOG_CAPACITY];
static size_t s_vision_log_count;

/* 文本未变则跳过 set_text，避免无谓的 free/realloc/文本重新布局/invalidate 重绘。
 * LVGL label 内部已保存当前文本，直接比对即可，无需维护影子状态。 */
static void ui_label_set_text_safe(lv_obj_t *label, const char *txt)
{
    if (!label || !txt) {
        return;
    }
    const char *cur = lv_label_get_text(label);
    if (cur && strcmp(cur, txt) == 0) {
        return;
    }
    lv_label_set_text(label, txt);
}

/* 带格式版本：先格式化到栈缓冲区再走自比较。缓冲区按当前最长文本留足余量。 */
static void ui_label_set_text_fmt_safe(lv_obj_t *label, const char *fmt, ...)
{
    if (!label) {
        return;
    }
    char buf[32];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    ui_label_set_text_safe(label, buf);
}

static void ui_slider_set_value_safe(lv_obj_t *slider, int32_t value)
{
    if (slider && lv_slider_get_value(slider) != value) {
        lv_slider_set_value(slider, value, LV_ANIM_OFF);
    }
}

static void ui_slider_set_range_safe(lv_obj_t *slider, int32_t minimum, int32_t maximum)
{
    if (slider && (lv_slider_get_min_value(slider) != minimum ||
                   lv_slider_get_max_value(slider) != maximum)) {
        lv_slider_set_range(slider, minimum, maximum);
    }
}

static void ui_arc_set_value_safe(lv_obj_t *arc, int32_t value)
{
    if (arc && lv_arc_get_value(arc) != value) {
        lv_arc_set_value(arc, value);
    }
}

/* ============================================================================
 * SDK 基础设施（anyui 生成代码依赖）
 * ----------------------------------------------------------------------------
 * flag/state 修改、动画回调、屏幕切换等通用工具，与具体业务无关。
 * ==========================================================================*/

typedef struct {
    int32_t ext_x;
    int32_t ext_y;
} ui_hit_area_profile_t;

static const ui_hit_area_profile_t s_button_hit_area = {
    .ext_x = UI_HIT_BUTTON_EXT_X,
    .ext_y = UI_HIT_BUTTON_EXT_Y,
};
static const ui_hit_area_profile_t s_slider_hit_area = {
    .ext_x = UI_HIT_SLIDER_EXT_X,
    .ext_y = UI_HIT_SLIDER_EXT_Y,
};
static const ui_hit_area_profile_t s_switch_hit_area = {
    .ext_x = UI_HIT_SWITCH_EXT_X,
    .ext_y = UI_HIT_SWITCH_EXT_Y,
};

/* LVGL 原生 ext_click_area 只能四边等量扩展。先用 X/Y 较大值通过 LVGL 粗筛，
 * 再由该高级命中回调裁成左右 ext_x、上下 ext_y 的精确矩形。
 * 普通事件回调在控件类回调之后执行，因此这里也会覆盖 slider 默认的旋钮命中结果。 */
static void ui_hit_area_event_cb(lv_event_t *e)
{
    lv_hit_test_info_t *info = lv_event_get_hit_test_info(e);
    const ui_hit_area_profile_t *profile = lv_event_get_user_data(e);
    lv_obj_t *target = lv_event_get_current_target(e);
    if (!info || !profile || !target) {
        return;
    }

    lv_area_t area;
    lv_obj_get_coords(target, &area);
    area.x1 -= profile->ext_x;
    area.x2 += profile->ext_x;
    area.y1 -= profile->ext_y;
    area.y2 += profile->ext_y;
    info->res = lv_area_is_point_on(&area, info->point, 0);
}

static void ui_apply_hit_area(lv_obj_t *target, const ui_hit_area_profile_t *profile)
{
    if (!target) {
        return;
    }

    int32_t coarse_extension = profile->ext_x > profile->ext_y ? profile->ext_x : profile->ext_y;
    lv_obj_set_ext_click_area(target, coarse_extension);
    lv_obj_add_flag(target, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_add_event_cb(target, ui_hit_area_event_cb, LV_EVENT_HIT_TEST, (void *)profile);
}

/* 自动识别标准 LVGL button/imagebutton、slider、switch；新增这些标准控件无需改 SDK。
 * AnyUI 若用普通 lv_obj 容器模拟可点击控件，无法仅靠类信息识别，应将其指针加入
 * ui_expand_dashboard_hit_areas() 的 button_targets 补充数组。 */
static void ui_expand_standard_hit_areas(lv_obj_t *root)
{
    if (!root) {
        return;
    }

    bool explicitly_managed = root == scr_dashboard_btn_open_motor ||
                              root == scr_dashboard_slider_A_speed ||
                              root == scr_dashboard_slider_B_speed ||
                              root == scr_dashboard_slider_C_speed;
    if (!explicitly_managed) {
        if (lv_obj_has_class(root, &lv_slider_class)) {
            ui_apply_hit_area(root, &s_slider_hit_area);
        } else if (lv_obj_has_class(root, &lv_switch_class)) {
            ui_apply_hit_area(root, &s_switch_hit_area);
        } else if (lv_obj_has_class(root, &lv_button_class) ||
                   lv_obj_has_class(root, &lv_imagebutton_class)) {
            ui_apply_hit_area(root, &s_button_hit_area);
        }
    }

    uint32_t child_count = lv_obj_get_child_count(root);
    for (uint32_t i = 0; i < child_count; ++i) {
        ui_expand_standard_hit_areas(lv_obj_get_child(root, (int32_t)i));
    }
}

void ui_expand_dashboard_hit_areas(void)
{
    /* 普通容器实现的可点击控件放在此处；新增项只需追加指针，无需修改 system_init.c。 */
    lv_obj_t *button_targets[] = {
        scr_dashboard_cont_log_01, scr_dashboard_cont_log_02,
        scr_dashboard_cont_log_03, scr_dashboard_cont_log_04,
        scr_dashboard_cont_log_05, scr_dashboard_cont_log_06,
        scr_dashboard_cont_log_07, scr_dashboard_cont_log_08,
        scr_dashboard_cont_log_09, scr_dashboard_cont_log_10,
        scr_dashboard_cont_log_11, scr_dashboard_cont_log_12,
        scr_dashboard_cont_log_13, scr_dashboard_cont_log_14,
        scr_dashboard_cont_log_15, scr_dashboard_cont_log_16,
        scr_dashboard_cont_log_17, scr_dashboard_cont_log_18,
        scr_dashboard_cont_log_19, scr_dashboard_cont_log_20,
        scr_dashboard_btn_open_motor,
    };
    lv_obj_t *slider_targets[] = {
        scr_dashboard_slider_A_speed,
        scr_dashboard_slider_B_speed,
        scr_dashboard_slider_C_speed,
    };

    ui_expand_standard_hit_areas(scr_dashboard);
    for (uint32_t i = 0; i < sizeof(button_targets) / sizeof(button_targets[0]); ++i) {
        ui_apply_hit_area(button_targets[i], &s_button_hit_area);
    }
    for (uint32_t i = 0; i < sizeof(slider_targets) / sizeof(slider_targets[0]); ++i) {
        ui_apply_hit_area(slider_targets[i], &s_slider_hit_area);
    }
}

void ui_flag_modify(lv_obj_t *target, int32_t flag, int value)
{
    bool has_flag = lv_obj_has_flag(target, flag);
    if (value == UI_FLAG_ACTION_TOGGLE) {
        if (has_flag) {
            lv_obj_remove_flag(target, flag);
        } else {
            lv_obj_add_flag(target, flag);
        }
    } else if (value == UI_FLAG_ACTION_ADD) {
        if (!has_flag) {
            lv_obj_add_flag(target, flag);
        }
    } else if (has_flag) {
        lv_obj_remove_flag(target, flag);
    }
}

void ui_state_modify(lv_obj_t *target, int32_t state, int value)
{
    bool has_state = lv_obj_has_state(target, state);
    if (value == UI_STATE_ACTION_TOGGLE) {
        if (has_state) {
            lv_obj_remove_state(target, state);
        } else {
            lv_obj_add_state(target, state);
        }
    } else if (value == UI_STATE_ACTION_ADD) {
        if (!has_state) {
            lv_obj_add_state(target, state);
        }
    } else if (has_state) {
        lv_obj_remove_state(target, state);
    }
}

void anim_cb_transform_x(lv_anim_t *a, int32_t x)
{
    anim_user_data_t *usr = (anim_user_data_t *)a->user_data;
    lv_obj_set_x(usr->target, x);
}

void anim_cb_transform_y(lv_anim_t *a, int32_t y)
{
    anim_user_data_t *usr = (anim_user_data_t *)a->user_data;
    lv_obj_set_y(usr->target, y);
}

void anim_cb_set_width(lv_anim_t *a, int32_t width)
{
    anim_user_data_t *usr = (anim_user_data_t *)a->user_data;
    lv_obj_set_width(usr->target, width);
}

void anim_cb_set_height(lv_anim_t *a, int32_t height)
{
    anim_user_data_t *usr = (anim_user_data_t *)a->user_data;
    lv_obj_set_height(usr->target, height);
}

void anim_cb_set_opacity(lv_anim_t *a, int32_t opacity)
{
    anim_user_data_t *usr = (anim_user_data_t *)a->user_data;
    lv_obj_set_style_opa(usr->target, opacity, 0);
}

void anim_cb_set_image_angle(lv_anim_t *a, int32_t angle)
{
    anim_user_data_t *usr = (anim_user_data_t *)a->user_data;
    lv_img_set_angle(usr->target, angle);
}

void anim_cb_set_image_zoom(lv_anim_t *a, int32_t zoom)
{
    anim_user_data_t *usr = (anim_user_data_t *)a->user_data;
    lv_img_set_zoom(usr->target, zoom);
}

int32_t anim_cb_get_x(lv_anim_t *a)
{
    anim_user_data_t *usr = (anim_user_data_t *)a->user_data;
    return lv_obj_get_x_aligned(usr->target);
}

int32_t anim_cb_get_y(lv_anim_t *a)
{
    anim_user_data_t *usr = (anim_user_data_t *)a->user_data;
    return lv_obj_get_y_aligned(usr->target);
}

int32_t anim_cb_get_width(lv_anim_t *a)
{
    anim_user_data_t *usr = (anim_user_data_t *)a->user_data;
    return lv_obj_get_width(usr->target);
}

int32_t anim_cb_get_height(lv_anim_t *a)
{
    anim_user_data_t *usr = (anim_user_data_t *)a->user_data;
    return lv_obj_get_height(usr->target);
}

int32_t anim_cb_get_opacity(lv_anim_t *a)
{
    anim_user_data_t *usr = (anim_user_data_t *)a->user_data;
    return lv_obj_get_style_opa(usr->target, 0);
}

int32_t anim_cb_get_image_angle(lv_anim_t *a)
{
    anim_user_data_t *usr = (anim_user_data_t *)a->user_data;
    return lv_img_get_angle(usr->target);
}

int32_t anim_cb_get_image_zoom(lv_anim_t *a)
{
    anim_user_data_t *usr = (anim_user_data_t *)a->user_data;
    return lv_img_get_zoom(usr->target);
}

void anim_cb_free_user_data(lv_anim_t *a)
{
    anim_user_data_t *usr = (anim_user_data_t *)a->user_data;
    LV_MEM_FREE(usr);
}

void ui_scr_load_with_anim(ui_scr_load_with_anim_t *params)
{
    lv_scr_load_anim(*params->new_scr_pptr, params->anim_type, params->time, params->delay, params->auto_del);

    if (params->auto_del)
    {
        if (*params->old_scr_pptr != NULL)
        {
            ui_obj_nullify(params->old_scr_pptr);
        }
    }
}

void ui_obj_nullify(lv_obj_t **obj)
{
    *obj = NULL;
}

extern lv_obj_t *scr_dashboard_cont_system;
extern lv_obj_t *scr_dashboard_label_runtime_cpu_core1_usage;
extern lv_obj_t *scr_dashboard_label_runtime_cpu_core2_usage;
extern lv_obj_t *scr_dashboard_label_cpu_02_value;
extern lv_obj_t *scr_dashboard_label_cpu_03_value;
extern lv_obj_t *scr_dashboard_label_cpu_total_value;
extern lv_obj_t *scr_dashboard_slider_memory_usage;
extern lv_obj_t *scr_dashboard_label_memory_usage_value;
extern lv_obj_t *scr_dashboard_label_ram_value;
extern lv_obj_t *scr_dashboard_label_psram_usage_value;
extern lv_obj_t *scr_dashboard_slider_psram_usage;
extern lv_obj_t *scr_dashboard_label_psram_free_value;
extern lv_obj_t *scr_dashboard_label_psram_total_value;
extern lv_obj_t *scr_dashboard_label_heap_free_value;
extern lv_obj_t *scr_dashboard_label_heap_min_free_value;
extern lv_obj_t *scr_dashboard_label_heap_max_block_value;
extern lv_obj_t *scr_dashboard_label_runtime_value;
extern lv_obj_t *scr_dashboard_label_chip_temp_value;
extern lv_obj_t *scr_dashboard_label_cpu_freq_value;
extern lv_obj_t *scr_dashboard_label_task_count_value;
// 设置页 - 关于：运行时长(动态)、可用 PSRAM(动态)、模型信息(绑定时静态填一次)
extern lv_obj_t *scr_dashboard_label_about_runtime_value;
extern lv_obj_t *scr_dashboard_label_memory_value;
extern lv_obj_t *scr_dashboard_label_model_info_value;
extern lv_obj_t *scr_dashboard_slider_screen_brightness;
extern lv_obj_t *scr_dashboard_label_runtime_Screen_Brightness__data;
extern lv_obj_t *scr_dashboard_label_runtime_recognition_status;
extern lv_obj_t *scr_dashboard_label_runtime_confidence_mian;
extern lv_obj_t *scr_dashboard_slider_runtime_confidence_mian;
extern lv_obj_t *scr_dashboard_label_runtime_confidence_logo;
extern lv_obj_t *scr_dashboard_slider_runtime_confidence_logo;
extern lv_obj_t *scr_dashboard_label_runtime_recognition_fps;
extern lv_obj_t *scr_dashboard_label_runtime_infer_time;
extern lv_obj_t *scr_dashboard_label_init_express_company;
extern lv_obj_t *scr_dashboard_label_JT_A;
extern lv_obj_t *scr_dashboard_label_ZT_A;
extern lv_obj_t *scr_dashboard_label_YD_A;
extern lv_obj_t *scr_dashboard_slider_jt;
extern lv_obj_t *scr_dashboard_slider_zt;
extern lv_obj_t *scr_dashboard_slider_yd;
extern lv_obj_t *scr_dashboard_imgbtn_logo;
extern lv_obj_t *scr_dashboard_img_runtime_ethernet_status;
extern lv_obj_t *scr_dashboard_cont_log_page1;
extern lv_obj_t *scr_dashboard_cont_log_page2;
extern lv_obj_t *scr_dashboard_cont_log_page3;
extern lv_obj_t *scr_dashboard_cont_log_page4;
extern lv_obj_t *scr_dashboard_label_current_page;
extern lv_obj_t *scr_dashboard_imgbtn_page_back;
extern lv_obj_t *scr_dashboard_imgbtn_page_next;

// 设置页 - 网络：本机/主机 IP 显示、上报间隔滑块、图像/指标上报开关
extern lv_obj_t *scr_dashboard_label_local_ip_value;
extern lv_obj_t *scr_dashboard_label_host_ip_value;
extern lv_obj_t *scr_dashboard_slider_report_interval;
extern lv_obj_t *scr_dashboard_label_report_interval_value;
extern lv_obj_t *scr_dashboard_sw_report_image;
extern lv_obj_t *scr_dashboard_sw_report_metrics;

/* ============================================================================
 * 第一层：事件 → 数据 → UI 更新
 * ----------------------------------------------------------------------------
 * 后台任务(system_monitor / vision)产生数据后投递事件，下列回调在事件到来时
 * 把数据刷进 dashboard 的标签/弧形/滑块。方向单一：外部数据驱动 UI 显示。
 *
 * 【新增一路事件→UI更新的步骤】
 *   1) 写事件回调 static void ui_xxx_event_cb(...)，校验 len 后把 data 刷进控件；
 *   2) 写注册函数 static uint32_t ui_register_xxx_events(event_table_t *t)，
 *      调 register_event 绑定上面的回调；
 *   3) 到本层末尾的聚合点 ui_register_all_events() 里加一行调用即可。
 *   控件刷新只用 ui_label_set_text_*_safe / ui_arc_percent_set_by_label 等本层工具。
 * ==========================================================================*/

static int ui_clamp_percent(int value)
{
    if (value < 0) {
        return 0;
    }
    if (value > 100) {
        return 100;
    }
    return value;
}

static lv_obj_t *ui_find_child_arc(lv_obj_t *parent)
{
    if (!parent) {
        return NULL;
    }

#if defined(LVGL_VERSION_MAJOR) && LVGL_VERSION_MAJOR >= 9
    uint32_t count = lv_obj_get_child_count(parent);
#else
    uint32_t count = lv_obj_get_child_cnt(parent);
#endif
    for (uint32_t i = 0; i < count; i++) {
        lv_obj_t *child = lv_obj_get_child(parent, i);
        if (lv_obj_check_type(child, &lv_arc_class)) {
            return child;
        }
    }

    return NULL;
}

static void ui_arc_percent_set_by_label(lv_obj_t **arc, lv_obj_t *label, int value)
{
    if (!label) {
        return;
    }

    value = ui_clamp_percent(value);
    if (!*arc) {
        *arc = ui_find_child_arc(lv_obj_get_parent(label));
    }
    ui_arc_set_value_safe(*arc, value);
    ui_label_set_text_fmt_safe(label, "%d%%", value);
}

static lv_obj_t *s_cpu_core1_arc;
static lv_obj_t *s_cpu_core2_arc;
static lv_obj_t *s_cpu_02_arc;
static lv_obj_t *s_cpu_03_arc;
static lv_obj_t *s_cpu_total_arc;
static bool s_system_monitor_snapshot_valid;
static bool s_system_monitor_system_dirty;
static system_monitor_event_data_t s_system_monitor_snapshot;
static bool s_vision_snapshot_valid;
static vision_result_event_data_t s_vision_snapshot;

static bool ui_system_monitor_data_equal(const system_monitor_event_data_t *a,
                                         const system_monitor_event_data_t *b)
{
    return a->cpu1_usage == b->cpu1_usage &&
           a->cpu2_usage == b->cpu2_usage &&
           a->cpu_total_usage == b->cpu_total_usage &&
           a->memory_usage == b->memory_usage &&
           a->heap_free_kb == b->heap_free_kb &&
           a->heap_min_free_kb == b->heap_min_free_kb &&
           a->heap_max_block_kb == b->heap_max_block_kb &&
           a->internal_free_kb == b->internal_free_kb &&
           a->psram_usage == b->psram_usage &&
           a->psram_free_kb == b->psram_free_kb &&
           a->psram_total_kb == b->psram_total_kb &&
           a->runtime_sec == b->runtime_sec &&
           a->chip_temp == b->chip_temp &&
           a->cpu_freq_mhz == b->cpu_freq_mhz &&
           a->task_count == b->task_count;
}

static bool ui_vision_data_equal(const vision_result_event_data_t *a,
                                 const vision_result_event_data_t *b)
{
    return strncmp(a->status, b->status, sizeof(a->status)) == 0 &&
           a->confidence == b->confidence &&
           a->logo_confidence == b->logo_confidence &&
           a->fps_x10 == b->fps_x10 &&
           a->infer_time_ms == b->infer_time_ms &&
           strncmp(a->company, b->company, sizeof(a->company)) == 0 &&
           a->jt_a == b->jt_a &&
           a->zt_a == b->zt_a &&
           a->yd_a == b->yd_a;
}

static void ui_system_monitor_event_cb(uint8_t event, uint16_t code, uint16_t type,
                                       uint16_t len, uint8_t *data, uint8_t status)
{
    LV_UNUSED(event);
    LV_UNUSED(code);
    LV_UNUSED(type);
    LV_UNUSED(status);

    if (!data || len != sizeof(system_monitor_event_data_t)) {
        return;
    }

    system_monitor_event_data_t *monitor = (system_monitor_event_data_t *)data;
    bool system_visible = scr_dashboard_cont_system &&
                          !lv_obj_has_flag(scr_dashboard_cont_system, LV_OBJ_FLAG_HIDDEN);
    bool data_changed = !s_system_monitor_snapshot_valid ||
                        !ui_system_monitor_data_equal(&s_system_monitor_snapshot, monitor);
    if (!data_changed && !(system_visible && s_system_monitor_system_dirty)) {
        return;
    }
    s_system_monitor_snapshot = *monitor;
    s_system_monitor_snapshot_valid = true;

    ui_arc_percent_set_by_label(&s_cpu_core1_arc, scr_dashboard_label_runtime_cpu_core1_usage,
                                monitor->cpu1_usage);
    ui_arc_percent_set_by_label(&s_cpu_core2_arc, scr_dashboard_label_runtime_cpu_core2_usage,
                                monitor->cpu2_usage);

    if (system_visible) {
        ui_arc_percent_set_by_label(&s_cpu_02_arc, scr_dashboard_label_cpu_02_value,
                                    monitor->cpu1_usage);
        ui_arc_percent_set_by_label(&s_cpu_03_arc, scr_dashboard_label_cpu_03_value,
                                    monitor->cpu2_usage);
        ui_arc_percent_set_by_label(&s_cpu_total_arc, scr_dashboard_label_cpu_total_value,
                                    monitor->cpu_total_usage);

        ui_slider_set_value_safe(scr_dashboard_slider_memory_usage, ui_clamp_percent(monitor->memory_usage));
        ui_label_set_text_fmt_safe(scr_dashboard_label_memory_usage_value, "%d%%", ui_clamp_percent(monitor->memory_usage));
        ui_label_set_text_fmt_safe(scr_dashboard_label_ram_value, "%d", monitor->internal_free_kb);
        ui_label_set_text_fmt_safe(scr_dashboard_label_psram_usage_value, "%d%%", ui_clamp_percent(monitor->psram_usage));
        ui_slider_set_value_safe(scr_dashboard_slider_psram_usage, ui_clamp_percent(monitor->psram_usage));
        ui_label_set_text_fmt_safe(scr_dashboard_label_psram_free_value, "%d", monitor->psram_free_kb);
        ui_label_set_text_fmt_safe(scr_dashboard_label_psram_total_value, "%d", monitor->psram_total_kb);
        ui_label_set_text_fmt_safe(scr_dashboard_label_heap_free_value, "%d", monitor->heap_free_kb);
        ui_label_set_text_fmt_safe(scr_dashboard_label_heap_min_free_value, "%d", monitor->heap_min_free_kb);
        ui_label_set_text_fmt_safe(scr_dashboard_label_heap_max_block_value, "%d", monitor->heap_max_block_kb);
        ui_label_set_text_fmt_safe(scr_dashboard_label_runtime_value, "%02d:%02d:%02d", monitor->runtime_sec / 3600,
                              (monitor->runtime_sec / 60) % 60, monitor->runtime_sec % 60);
        ui_label_set_text_fmt_safe(scr_dashboard_label_chip_temp_value, "%d.%d", monitor->chip_temp / 10,
                              (monitor->chip_temp < 0 ? -monitor->chip_temp : monitor->chip_temp) % 10);
        ui_label_set_text_fmt_safe(scr_dashboard_label_cpu_freq_value, "%d", monitor->cpu_freq_mhz);
        ui_label_set_text_fmt_safe(scr_dashboard_label_task_count_value, "%d", monitor->task_count);
        s_system_monitor_system_dirty = false;
    } else {
        s_system_monitor_system_dirty = true;
    }

    // 关于页：运行时长同 dashboard(HH:MM:SS)，可用内存取 PSRAM 总量，以 MB 一位小数显示
    ui_label_set_text_fmt_safe(scr_dashboard_label_about_runtime_value, "%02d:%02d:%02d",
                          monitor->runtime_sec / 3600, (monitor->runtime_sec / 60) % 60,
                          monitor->runtime_sec % 60);
    ui_label_set_text_fmt_safe(scr_dashboard_label_memory_value, "%d.%d MB",
                          monitor->psram_total_kb / 1024, (monitor->psram_total_kb % 1024) * 10 / 1024);
}

static uint32_t ui_register_system_monitor_events(event_table_t *table)
{
    return register_event(table, EVT_SYSTEM_MONITOR, EVT_SYSTEM_MONITOR_CHANGED, 0,
                          sizeof(system_monitor_event_data_t), ui_system_monitor_event_cb);
}

static void ui_vision_result_event_cb(uint8_t event, uint16_t code, uint16_t type,
                                       uint16_t len, uint8_t *data, uint8_t status)
{
    LV_UNUSED(event);
    LV_UNUSED(code);
    LV_UNUSED(type);
    LV_UNUSED(status);

    if (!data || len != sizeof(vision_result_event_data_t)) {
        return;
    }

    vision_result_event_data_t *v = (vision_result_event_data_t *)data;
    if (s_vision_snapshot_valid && ui_vision_data_equal(&s_vision_snapshot, v)) {
        return;
    }
    s_vision_snapshot = *v;
    s_vision_snapshot_valid = true;

    ui_label_set_text_safe(scr_dashboard_label_runtime_recognition_status, v->status);
    ui_label_set_text_fmt_safe(scr_dashboard_label_runtime_confidence_mian, "%d%%", ui_clamp_percent(v->confidence));
    ui_slider_set_value_safe(scr_dashboard_slider_runtime_confidence_mian, ui_clamp_percent(v->confidence));
    ui_label_set_text_fmt_safe(scr_dashboard_label_runtime_confidence_logo, "%d%%", ui_clamp_percent(v->logo_confidence));
    ui_slider_set_value_safe(scr_dashboard_slider_runtime_confidence_logo, ui_clamp_percent(v->logo_confidence));
    ui_label_set_text_fmt_safe(scr_dashboard_label_runtime_recognition_fps, "%d.%d",
                          v->fps_x10 / 10, v->fps_x10 % 10);
    ui_label_set_text_fmt_safe(scr_dashboard_label_runtime_infer_time, "%d", v->infer_time_ms);
    ui_label_set_text_safe(scr_dashboard_label_init_express_company, v->company);
    ui_label_set_text_fmt_safe(scr_dashboard_label_JT_A, "A:%d.00%%", ui_clamp_percent(v->jt_a));
    ui_label_set_text_fmt_safe(scr_dashboard_label_ZT_A, "A:%d.00%%", ui_clamp_percent(v->zt_a));
    ui_label_set_text_fmt_safe(scr_dashboard_label_YD_A, "A:%d.00%%", ui_clamp_percent(v->yd_a));
    ui_slider_set_value_safe(scr_dashboard_slider_jt, ui_clamp_percent(v->jt_a));
    ui_slider_set_value_safe(scr_dashboard_slider_zt, ui_clamp_percent(v->zt_a));
    ui_slider_set_value_safe(scr_dashboard_slider_yd, ui_clamp_percent(v->yd_a));
}

static void ui_refresh_vision_log_slot(size_t index)
{
    const ui_log_widgets_t *widgets = &s_log_widgets[index];
    if (index >= s_vision_log_count) {
        ui_label_set_text_safe(*widgets->time, "--");
        ui_label_set_text_safe(*widgets->company, "--");
        ui_label_set_text_safe(*widgets->result, "--");
        ui_label_set_text_safe(*widgets->confidence, "--");
        ui_label_set_text_safe(*widgets->detail_time, "--");
        ui_label_set_text_safe(*widgets->detail_result, "--");
        ui_label_set_text_safe(*widgets->logo_confidence, "--");
        ui_label_set_text_safe(*widgets->waybill_confidence, "--");
        ui_label_set_text_safe(*widgets->logo_infer_time, "--");
        ui_label_set_text_safe(*widgets->waybill_infer_time, "--");
        ui_label_set_text_safe(*widgets->badge, "--");
        return;
    }

    const vision_log_event_data_t *log = &s_vision_logs[index];
    size_t time_len = strnlen(log->time, sizeof(log->time));
    const char *list_time = time_len >= 8 ? log->time + time_len - 8 : log->time;
    ui_label_set_text_safe(*widgets->time, list_time);
    ui_label_set_text_safe(*widgets->company, log->company);
    ui_label_set_text_safe(*widgets->result, log->result);
    ui_label_set_text_fmt_safe(*widgets->confidence, "%d%%", ui_clamp_percent(log->logo_confidence));
    ui_label_set_text_safe(*widgets->detail_time, log->time);
    ui_label_set_text_safe(*widgets->detail_result, log->result);
    ui_label_set_text_fmt_safe(*widgets->logo_confidence, "%d%%", ui_clamp_percent(log->logo_confidence));
    ui_label_set_text_fmt_safe(*widgets->waybill_confidence, "%d%%", ui_clamp_percent(log->waybill_confidence));
    ui_label_set_text_fmt_safe(*widgets->logo_infer_time, "%d ms", log->logo_infer_time_ms);
    ui_label_set_text_fmt_safe(*widgets->waybill_infer_time, "%d ms", log->waybill_infer_time_ms);
    ui_label_set_text_safe(*widgets->badge, log->company);
}

static void ui_vision_log_event_cb(uint8_t event, uint16_t code, uint16_t type,
                                   uint16_t len, uint8_t *data, uint8_t status)
{
    LV_UNUSED(event);
    LV_UNUSED(code);
    LV_UNUSED(type);
    LV_UNUSED(status);

    if (!data || len != sizeof(vision_log_event_data_t)) {
        return;
    }

    size_t move_count = s_vision_log_count < UI_LOG_CAPACITY
                            ? s_vision_log_count
                            : UI_LOG_CAPACITY - 1;
    if (move_count > 0) {
        memmove(&s_vision_logs[1], &s_vision_logs[0],
                move_count * sizeof(s_vision_logs[0]));
    }
    s_vision_logs[0] = *(vision_log_event_data_t *)data;
    if (s_vision_log_count < UI_LOG_CAPACITY) {
        s_vision_log_count++;
    }

    for (size_t i = 0; i < UI_LOG_CAPACITY; i++) {
        ui_refresh_vision_log_slot(i);
    }
}

static uint32_t ui_register_vision_events(event_table_t *table)
{
    uint32_t result = register_event(table, EVT_VISION, EVT_VISION_RESULT_CHANGED, 0,
                                     sizeof(vision_result_event_data_t), ui_vision_result_event_cb);
    result |= register_event(table, EVT_VISION, EVT_VISION_LOG_APPENDED, 0,
                             sizeof(vision_log_event_data_t), ui_vision_log_event_cb);
    return result;
}

/* 以太网状态：该控件是 imgbtn，RELEASED=disconnect_126x36(断开/失败)，
 * CHECKED_RELEASED=connect_126x36(已连接)。必须用 lv_imagebutton_set_state 切状态，
 * 而非 lv_obj_add/remove_state(LV_STATE_CHECKED)——后者仅在有"样式"差异时才 invalidate，
 * 而两态区别是图片源(set_src)非样式，状态位会改但永不重绘，一直停在旧图(红)。
 * imagebutton_set_state 末尾调 refr_image 无条件 invalidate，才会真正切图。
 * 纯事件驱动，断开自动重连时事件会再次到来。 */
static void ui_ethernet_event_cb(uint8_t event, uint16_t code, uint16_t type,
                                 uint16_t len, uint8_t *data, uint8_t status)
{
    LV_UNUSED(event);
    LV_UNUSED(code);
    LV_UNUSED(type);
    LV_UNUSED(status);

    if (!data || len != sizeof(ethernet_event_data_t)) {
        return;
    }
    if (!scr_dashboard_img_runtime_ethernet_status) {
        return;
    }

    ethernet_event_data_t *eth = (ethernet_event_data_t *)data;
    bool connected = lv_obj_has_state(scr_dashboard_img_runtime_ethernet_status, LV_STATE_CHECKED);
    if (connected != eth->connected) {
        lv_imagebutton_set_state(scr_dashboard_img_runtime_ethernet_status,
                                 eth->connected ? LV_IMAGEBUTTON_STATE_CHECKED_RELEASED
                                                : LV_IMAGEBUTTON_STATE_RELEASED);
    }
}

static uint32_t ui_register_ethernet_events(event_table_t *table)
{
    return register_event(table, EVT_ETHERNET, EVT_ETHERNET_STATUS_CHANGED, 0,
                          sizeof(ethernet_event_data_t), ui_ethernet_event_cb);
}

// 第一层聚合点：注册本屏所有事件回调。新增一路事件→UI更新时，在此加一行。
static void ui_register_all_events(void)
{
    event_table_t *table = get_current_event_table();
    ui_register_system_monitor_events(table);
    ui_register_vision_events(table);
    ui_register_ethernet_events(table);
    // ui_register_xxx_events(table);   // ← 新增事件注册放这里
}

/* ============================================================================
 * 第二层：UI 交互 → 触发业务调用
 * ----------------------------------------------------------------------------
 * 用户操作控件后，下列回调把动作转发给 main 注入的 handler，UI 不直接依赖
 * bsp/vision。方向单一：用户输入驱动业务执行。handler 经 s_handlers 取，
 * 为 NULL 时静默忽略(未注入则该交互无副作用)。
 *
 * 【新增一路 UI 交互→业务 的步骤】
 *   1) 在 ui.h 的 ui_dashboard_handlers_t 里加一个 handler 字段(及其 typedef)；
 *   2) 写控件事件回调 static void ui_xxx_event_cb(lv_event_t *e)，
 *      取值/校验后调 s_handlers.xxx(...)(判 NULL)；
 *   3) 写挂载函数 static void ui_attach_xxx(void)，对目标控件 add_event_cb；
 *   4) 到本层末尾的聚合点 ui_attach_all_widgets() 里加一行调用；
 *   5) main 填 ui_dashboard_handlers_t 时多赋一个字段即可，ui_bind_dashboard 签名不变。
 * ==========================================================================*/

// main 注入的业务 handler 集合，由 ui_bind_dashboard 存下，供本层各回调取用。
static ui_dashboard_handlers_t s_handlers = {0};
static bool s_dashboard_bound;

// 亮度滑块拖动：更新百分比标签，并把新亮度交给注入的背光 handler。
static void ui_brightness_slider_event_cb(lv_event_t *e)
{
    lv_obj_t *slider = lv_event_get_target(e);
    int pct = (int)lv_slider_get_value(slider);
    pct = ui_clamp_percent(pct);

    ui_label_set_text_fmt_safe(scr_dashboard_label_runtime_Screen_Brightness__data, "%d%%", pct);
    if (s_handlers.brightness) {
        s_handlers.brightness(pct);
    }
    ethernet_app_notify_control_state_changed();
}

// 给亮度滑块挂事件回调，并把滑块/标签初值同步为 UI_BRIGHTNESS_DEFAULT_PERCENT。
static void ui_attach_brightness_slider(void)
{
    if (!scr_dashboard_slider_screen_brightness) {
        return;
    }

    lv_obj_add_event_cb(scr_dashboard_slider_screen_brightness, ui_brightness_slider_event_cb,
                        LV_EVENT_VALUE_CHANGED, NULL);

    // 同步开机默认亮度：设滑块值并主动触发一次回调，使标签与背光与 UI 一致
    ui_slider_set_value_safe(scr_dashboard_slider_screen_brightness, UI_BRIGHTNESS_DEFAULT_PERCENT);
    lv_obj_send_event(scr_dashboard_slider_screen_brightness, LV_EVENT_VALUE_CHANGED, NULL);
}

// LOGO 按键点击：请求视觉任务在下一帧执行一次 ROI 自适应调参（复刻原工程 cal_btn 回调）。
static void ui_calibration_button_event_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) != LV_EVENT_CLICKED) {
        return;
    }
    if (s_handlers.calibration) {
        s_handlers.calibration();
    }
}

// 给 dashboard 左上角 LOGO 按键挂 CLICKED 事件回调，点击触发一次 ROI 校准请求。
static void ui_attach_calibration_button(void)
{
    if (!scr_dashboard_imgbtn_logo) {
        return;
    }
    lv_obj_add_event_cb(scr_dashboard_imgbtn_logo, ui_calibration_button_event_cb,
                        LV_EVENT_CLICKED, NULL);
}

static void ui_detection_switch_event_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) != LV_EVENT_VALUE_CHANGED) {
        return;
    }

    bool enabled = lv_obj_has_state(lv_event_get_target(e), LV_STATE_CHECKED);
    if (s_handlers.detection_enabled) {
        s_handlers.detection_enabled(enabled);
    }

    ui_state_modify(scr_dashboard_sw_preview_overlay, LV_STATE_CHECKED,
                    enabled ? UI_STATE_ACTION_ADD : UI_STATE_ACTION_REMOVE);
    if (s_handlers.preview_overlay_enabled) {
        s_handlers.preview_overlay_enabled(enabled);
    }
    ui_state_modify(scr_dashboard_sw_report_image, LV_STATE_CHECKED,
                    enabled ? UI_STATE_ACTION_ADD : UI_STATE_ACTION_REMOVE);
    ethernet_app_set_report_image_enabled(enabled);
    ethernet_app_notify_control_state_changed();
}

static void ui_preview_overlay_switch_event_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) != LV_EVENT_VALUE_CHANGED) {
        return;
    }

    lv_obj_t *target = lv_event_get_target(e);
    bool enabled = lv_obj_has_state(target, LV_STATE_CHECKED) &&
                   lv_obj_has_state(scr_dashboard_sw_detect, LV_STATE_CHECKED);
    if (!enabled) {
        ui_state_modify(target, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
    }
    if (s_handlers.preview_overlay_enabled) {
        s_handlers.preview_overlay_enabled(enabled);
    }
    ethernet_app_notify_control_state_changed();
}

static void ui_attach_detection_switches(void)
{
    if (scr_dashboard_sw_detect) {
        lv_obj_add_event_cb(scr_dashboard_sw_detect, ui_detection_switch_event_cb,
                            LV_EVENT_VALUE_CHANGED, NULL);
        ui_state_modify(scr_dashboard_sw_detect, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        lv_obj_send_event(scr_dashboard_sw_detect, LV_EVENT_VALUE_CHANGED, NULL);
    }
    if (scr_dashboard_sw_preview_overlay) {
        lv_obj_add_event_cb(scr_dashboard_sw_preview_overlay, ui_preview_overlay_switch_event_cb,
                            LV_EVENT_VALUE_CHANGED, NULL);
        ui_state_modify(scr_dashboard_sw_preview_overlay, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        lv_obj_send_event(scr_dashboard_sw_preview_overlay, LV_EVENT_VALUE_CHANGED, NULL);
    }
}

static void ui_motor_output_button_event_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) != LV_EVENT_VALUE_CHANGED) {
        return;
    }

    bool enabled = lv_obj_has_state(lv_event_get_target(e), LV_STATE_CHECKED);
    ui_label_set_text_safe(scr_dashboard_btn_open_motor_label, enabled ? "开启" : "关闭");
    if (s_handlers.motor_output_enabled_set) {
        s_handlers.motor_output_enabled_set(enabled);
    }
    ethernet_app_notify_control_state_changed();
}

static void ui_motor_speed_slider_event_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) != LV_EVENT_VALUE_CHANGED) {
        return;
    }

    int motor_index = (int)(intptr_t)lv_event_get_user_data(e);
    lv_obj_t *slider = lv_event_get_target(e);
    lv_obj_t *labels[] = {
        scr_dashboard_label_A_speed_value,
        scr_dashboard_label_B_speed_value,
        scr_dashboard_label_C_speed_value,
    };
    int pct = ui_clamp_percent((int)lv_slider_get_value(slider));
    ui_label_set_text_fmt_safe(labels[motor_index], "%d%%", pct);
    if (s_handlers.motor_speed_set) {
        s_handlers.motor_speed_set(motor_index, pct);
    }
    ethernet_app_notify_control_state_changed();
}

static void ui_attach_motor_controls(void)
{
    if (scr_dashboard_btn_open_motor) {
        lv_obj_add_event_cb(scr_dashboard_btn_open_motor, ui_motor_output_button_event_cb,
                            LV_EVENT_VALUE_CHANGED, NULL);
        bool enabled = s_handlers.motor_output_enabled_get &&
                       s_handlers.motor_output_enabled_get();
        ui_state_modify(scr_dashboard_btn_open_motor, LV_STATE_CHECKED,
                        enabled ? UI_STATE_ACTION_ADD : UI_STATE_ACTION_REMOVE);
        lv_obj_send_event(scr_dashboard_btn_open_motor, LV_EVENT_VALUE_CHANGED, NULL);
    }

    lv_obj_t *sliders[] = {
        scr_dashboard_slider_A_speed,
        scr_dashboard_slider_B_speed,
        scr_dashboard_slider_C_speed,
    };
    for (int motor_index = 0; motor_index < 3; ++motor_index) {
        if (!sliders[motor_index]) {
            continue;
        }
        lv_obj_add_event_cb(sliders[motor_index], ui_motor_speed_slider_event_cb,
                            LV_EVENT_VALUE_CHANGED, (void *)(intptr_t)motor_index);
        int pct = s_handlers.motor_speed_get ? s_handlers.motor_speed_get(motor_index) : 0;
        ui_slider_set_value_safe(sliders[motor_index], ui_clamp_percent(pct));
        lv_obj_send_event(sliders[motor_index], LV_EVENT_VALUE_CHANGED, NULL);
    }
}

static void ui_set_waybill_threshold_text(int pct)
{
    pct = ui_clamp_percent(pct);
    ui_label_set_text_fmt_safe(scr_dashboard_label_confidence_value_mian, "%d%%", pct);
    ui_label_set_text_fmt_safe(scr_dashboard_label_runtime_confidence_threshole_mian, "%d%%", pct);
}

static void ui_set_logo_threshold_text(int pct)
{
    pct = ui_clamp_percent(pct);
    ui_label_set_text_fmt_safe(scr_dashboard_label_confidence_value_logo, "%d%%", pct);
    ui_label_set_text_fmt_safe(scr_dashboard_label_runtime_confidence_threshole_logo, "%d%%", pct);
}

static void ui_waybill_threshold_slider_event_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) != LV_EVENT_VALUE_CHANGED) {
        return;
    }
    int pct = ui_clamp_percent((int)lv_slider_get_value(lv_event_get_target(e)));
    ui_set_waybill_threshold_text(pct);
    if (s_handlers.waybill_score_threshold_set) {
        s_handlers.waybill_score_threshold_set(pct);
    }
    ethernet_app_notify_control_state_changed();
}

static void ui_logo_threshold_slider_event_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) != LV_EVENT_VALUE_CHANGED) {
        return;
    }
    int pct = ui_clamp_percent((int)lv_slider_get_value(lv_event_get_target(e)));
    ui_set_logo_threshold_text(pct);
    if (s_handlers.logo_score_threshold_set) {
        s_handlers.logo_score_threshold_set(pct);
    }
    ethernet_app_notify_control_state_changed();
}

static void ui_attach_confidence_threshold_sliders(void)
{
    if (scr_dashboard_slider_confidence_threshold_mian) {
        lv_obj_add_event_cb(scr_dashboard_slider_confidence_threshold_mian,
                            ui_waybill_threshold_slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
        int pct = s_handlers.waybill_score_threshold_get ?
                  s_handlers.waybill_score_threshold_get() : 0;
        ui_slider_set_value_safe(scr_dashboard_slider_confidence_threshold_mian,
                                 ui_clamp_percent(pct));
        lv_obj_send_event(scr_dashboard_slider_confidence_threshold_mian, LV_EVENT_VALUE_CHANGED, NULL);
    }
    if (scr_dashboard_slider_confidence_threshold_logo) {
        lv_obj_add_event_cb(scr_dashboard_slider_confidence_threshold_logo,
                            ui_logo_threshold_slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
        int pct = s_handlers.logo_score_threshold_get ?
                  s_handlers.logo_score_threshold_get() : 0;
        ui_slider_set_value_safe(scr_dashboard_slider_confidence_threshold_logo,
                                 ui_clamp_percent(pct));
        lv_obj_send_event(scr_dashboard_slider_confidence_threshold_logo, LV_EVENT_VALUE_CHANGED, NULL);
    }
}

static const char *s_ui_isp_tag = "ui_isp";
static bool s_isp_programmatic_update;
static bool s_isp_brightness_pending;
static bool s_isp_contrast_pending;
static bool s_isp_saturation_pending;
static bool s_isp_hue_pending;
static int s_isp_brightness_target;
static int s_isp_contrast_target;
static int s_isp_saturation_target;
static int s_isp_hue_target;
static cam_sensor_isp_state_t s_isp_last_state;
static bool s_isp_last_state_valid;
static lv_timer_t *s_isp_refresh_timer;

static bool ui_isp_control_info_equal(const cam_sensor_isp_control_info_t *a,
                                      const cam_sensor_isp_control_info_t *b)
{
    return a->supported == b->supported &&
           a->minimum == b->minimum &&
           a->maximum == b->maximum &&
           a->step == b->step &&
           a->default_value == b->default_value;
}

static bool ui_isp_state_equal(const cam_sensor_isp_state_t *a,
                               const cam_sensor_isp_state_t *b)
{
    return ui_isp_control_info_equal(&a->brightness_info, &b->brightness_info) &&
           ui_isp_control_info_equal(&a->contrast_info, &b->contrast_info) &&
           ui_isp_control_info_equal(&a->saturation_info, &b->saturation_info) &&
           ui_isp_control_info_equal(&a->hue_info, &b->hue_info) &&
           a->contrast_auto == b->contrast_auto &&
           a->saturation_auto == b->saturation_auto &&
           a->brightness_valid == b->brightness_valid &&
           a->contrast_valid == b->contrast_valid &&
           a->saturation_valid == b->saturation_valid &&
           a->hue_valid == b->hue_valid &&
           a->brightness == b->brightness &&
           a->contrast == b->contrast &&
           a->saturation == b->saturation &&
           a->hue == b->hue &&
           a->exposure_valid == b->exposure_valid &&
           a->gain_valid == b->gain_valid &&
           a->white_balance_valid == b->white_balance_valid &&
           a->exposure_us == b->exposure_us &&
           a->gain_x1000 == b->gain_x1000 &&
           a->red_gain_x1000 == b->red_gain_x1000 &&
           a->blue_gain_x1000 == b->blue_gain_x1000;
}

static int ui_align_isp_value(const cam_sensor_isp_control_info_t *info, int value)
{
    int step = info->step > 0 ? info->step : 1;
    if (value < info->minimum) {
        value = info->minimum;
    } else if (value > info->maximum) {
        value = info->maximum;
    }
    return info->minimum + ((value - info->minimum) / step) * step;
}

static void ui_set_isp_slider_value(lv_obj_t *slider, lv_obj_t *label, int value)
{
    ui_slider_set_value_safe(slider, value);
    ui_label_set_text_fmt_safe(label, "%d", value);
}

static void ui_set_obj_state(lv_obj_t *obj, lv_state_t state, bool enabled)
{
    ui_state_modify(obj, state, enabled ? UI_STATE_ACTION_ADD : UI_STATE_ACTION_REMOVE);
}

static void ui_set_isp_control_enabled(lv_obj_t *slider, lv_obj_t *button,
                                       bool supported, bool automatic)
{
    ui_set_obj_state(slider, LV_STATE_DISABLED, !supported || automatic);
    if (button) {
        ui_set_obj_state(button, LV_STATE_DISABLED, !supported);
        ui_set_obj_state(button, LV_STATE_CHECKED, automatic);
    }
}

static void ui_apply_isp_state(const cam_sensor_isp_state_t *state)
{
    s_isp_last_state = *state;
    s_isp_last_state_valid = true;
    s_isp_programmatic_update = true;

    if (state->brightness_info.supported) {
        ui_slider_set_range_safe(scr_dashboard_slider_Luminance,
                                state->brightness_info.minimum, state->brightness_info.maximum);
        ui_set_obj_state(scr_dashboard_slider_Luminance, LV_STATE_DISABLED, false);
        if (!s_isp_brightness_pending ||
                (state->brightness_valid && state->brightness == s_isp_brightness_target)) {
            s_isp_brightness_pending = false;
            if (state->brightness_valid) {
                ui_set_isp_slider_value(scr_dashboard_slider_Luminance,
                                        scr_dashboard_label_runtime_Luminance_data, state->brightness);
            }
        }
    } else {
        s_isp_brightness_pending = false;
        ui_set_obj_state(scr_dashboard_slider_Luminance, LV_STATE_DISABLED, true);
        ui_label_set_text_safe(scr_dashboard_label_runtime_Luminance_data, "--");
    }

    if (state->contrast_info.supported) {
        ui_slider_set_range_safe(scr_dashboard_slider_contrast,
                                state->contrast_info.minimum, state->contrast_info.maximum);
        if (state->contrast_auto) {
            s_isp_contrast_pending = false;
            if (state->contrast_valid) {
                ui_set_isp_slider_value(scr_dashboard_slider_contrast,
                                        scr_dashboard_label_contrast_value, state->contrast);
            } else {
                ui_label_set_text_safe(scr_dashboard_label_contrast_value, "--");
            }
        } else if (!s_isp_contrast_pending ||
                   (state->contrast_valid && state->contrast == s_isp_contrast_target)) {
            s_isp_contrast_pending = false;
            if (state->contrast_valid) {
                ui_set_isp_slider_value(scr_dashboard_slider_contrast,
                                        scr_dashboard_label_contrast_value, state->contrast);
            }
        }
    } else {
        s_isp_contrast_pending = false;
        ui_label_set_text_safe(scr_dashboard_label_contrast_value, "--");
    }
    ui_set_isp_control_enabled(scr_dashboard_slider_contrast,
                               scr_dashboard_btn_contrast_auto,
                               state->contrast_info.supported, state->contrast_auto);

    if (state->saturation_info.supported) {
        ui_slider_set_range_safe(scr_dashboard_slider_saturation,
                                state->saturation_info.minimum, state->saturation_info.maximum);
        if (state->saturation_auto) {
            s_isp_saturation_pending = false;
            if (state->saturation_valid) {
                ui_set_isp_slider_value(scr_dashboard_slider_saturation,
                                        scr_dashboard_label_saturation_value, state->saturation);
            } else {
                ui_label_set_text_safe(scr_dashboard_label_saturation_value, "--");
            }
        } else if (!s_isp_saturation_pending ||
                   (state->saturation_valid && state->saturation == s_isp_saturation_target)) {
            s_isp_saturation_pending = false;
            if (state->saturation_valid) {
                ui_set_isp_slider_value(scr_dashboard_slider_saturation,
                                        scr_dashboard_label_saturation_value, state->saturation);
            }
        }
    } else {
        s_isp_saturation_pending = false;
        ui_label_set_text_safe(scr_dashboard_label_saturation_value, "--");
    }
    ui_set_isp_control_enabled(scr_dashboard_slider_saturation,
                               scr_dashboard_btn_saturation_auto,
                               state->saturation_info.supported, state->saturation_auto);

    if (state->hue_info.supported) {
        ui_slider_set_range_safe(scr_dashboard_slider_hue,
                                state->hue_info.minimum, state->hue_info.maximum);
        ui_set_obj_state(scr_dashboard_slider_hue, LV_STATE_DISABLED, false);
        if (!s_isp_hue_pending || (state->hue_valid && state->hue == s_isp_hue_target)) {
            s_isp_hue_pending = false;
            if (state->hue_valid) {
                ui_set_isp_slider_value(scr_dashboard_slider_hue,
                                        scr_dashboard_label_hue_value, state->hue);
            }
        }
    } else {
        s_isp_hue_pending = false;
        ui_set_obj_state(scr_dashboard_slider_hue, LV_STATE_DISABLED, true);
        ui_label_set_text_safe(scr_dashboard_label_hue_value, "--");
    }

    if (state->exposure_valid) {
        ui_label_set_text_fmt_safe(scr_dashboard_label_exposure_value, "%u.%u ms",
                                   (unsigned)(state->exposure_us / 1000U),
                                   (unsigned)((state->exposure_us % 1000U) / 100U));
    } else {
        ui_label_set_text_safe(scr_dashboard_label_exposure_value, "--");
    }
    if (state->gain_valid) {
        ui_label_set_text_fmt_safe(scr_dashboard_label_gain_value, "%u.%02ux",
                                   (unsigned)(state->gain_x1000 / 1000U),
                                   (unsigned)((state->gain_x1000 % 1000U) / 10U));
    } else {
        ui_label_set_text_safe(scr_dashboard_label_gain_value, "--");
    }
    if (state->white_balance_valid) {
        ui_label_set_text_fmt_safe(scr_dashboard_label_wb_value, "R %u.%02u / B %u.%02u",
                                   (unsigned)(state->red_gain_x1000 / 1000U),
                                   (unsigned)((state->red_gain_x1000 % 1000U) / 10U),
                                   (unsigned)(state->blue_gain_x1000 / 1000U),
                                   (unsigned)((state->blue_gain_x1000 % 1000U) / 10U));
    } else {
        ui_label_set_text_safe(scr_dashboard_label_wb_value, "--");
    }

    s_isp_programmatic_update = false;
}

static void ui_isp_slider_event_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) != LV_EVENT_VALUE_CHANGED || s_isp_programmatic_update) {
        return;
    }

    lv_obj_t *slider = lv_event_get_target(e);
    if (lv_obj_has_state(slider, LV_STATE_DISABLED)) {
        return;
    }

    int value = (int)lv_slider_get_value(slider);
    const cam_sensor_isp_control_info_t *info;
    lv_obj_t *label;
    esp_err_t (*setter)(int32_t);
    bool *pending;
    int *target;

    if (slider == scr_dashboard_slider_Luminance) {
        info = &s_isp_last_state.brightness_info;
        label = scr_dashboard_label_runtime_Luminance_data;
        setter = cam_sensor_isp_set_brightness;
        pending = &s_isp_brightness_pending;
        target = &s_isp_brightness_target;
    } else if (slider == scr_dashboard_slider_contrast) {
        if (lv_obj_has_state(scr_dashboard_btn_contrast_auto, LV_STATE_CHECKED)) {
            return;
        }
        info = &s_isp_last_state.contrast_info;
        label = scr_dashboard_label_contrast_value;
        setter = cam_sensor_isp_set_contrast;
        pending = &s_isp_contrast_pending;
        target = &s_isp_contrast_target;
    } else if (slider == scr_dashboard_slider_saturation) {
        if (lv_obj_has_state(scr_dashboard_btn_saturation_auto, LV_STATE_CHECKED)) {
            return;
        }
        info = &s_isp_last_state.saturation_info;
        label = scr_dashboard_label_saturation_value;
        setter = cam_sensor_isp_set_saturation;
        pending = &s_isp_saturation_pending;
        target = &s_isp_saturation_target;
    } else {
        info = &s_isp_last_state.hue_info;
        label = scr_dashboard_label_hue_value;
        setter = cam_sensor_isp_set_hue;
        pending = &s_isp_hue_pending;
        target = &s_isp_hue_target;
    }

    value = ui_align_isp_value(info, value);
    if ((int)lv_slider_get_value(slider) != value) {
        s_isp_programmatic_update = true;
        lv_slider_set_value(slider, value, LV_ANIM_OFF);
        s_isp_programmatic_update = false;
    }
    ui_label_set_text_fmt_safe(label, "%d", value);

    esp_err_t err = setter(value);
    if (err == ESP_OK) {
        *target = value;
        *pending = true;
        ethernet_app_notify_control_state_changed();
    } else {
        *pending = false;
        ESP_LOGW(s_ui_isp_tag, "set ISP value failed: %s", esp_err_to_name(err));
        ui_apply_isp_state(&s_isp_last_state);
    }
}

static void ui_isp_auto_button_event_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) != LV_EVENT_VALUE_CHANGED || s_isp_programmatic_update) {
        return;
    }

    lv_obj_t *button = lv_event_get_target(e);
    bool enabled = lv_obj_has_state(button, LV_STATE_CHECKED);
    esp_err_t err;

    if (button == scr_dashboard_btn_contrast_auto) {
        s_isp_contrast_pending = false;
        ui_set_isp_control_enabled(scr_dashboard_slider_contrast, button,
                                   s_isp_last_state.contrast_info.supported, enabled);
        err = cam_sensor_isp_set_contrast_auto(enabled);
    } else {
        s_isp_saturation_pending = false;
        ui_set_isp_control_enabled(scr_dashboard_slider_saturation, button,
                                   s_isp_last_state.saturation_info.supported, enabled);
        err = cam_sensor_isp_set_saturation_auto(enabled);
    }

    if (err != ESP_OK) {
        ESP_LOGW(s_ui_isp_tag, "set ISP auto mode failed: %s", esp_err_to_name(err));
        ui_apply_isp_state(&s_isp_last_state);
    } else {
        ethernet_app_notify_control_state_changed();
    }
}

static void ui_isp_refresh_timer_cb(lv_timer_t *timer)
{
    (void)timer;
    if (!scr_dashboard_cont_display ||
            lv_obj_has_flag(scr_dashboard_cont_display, LV_OBJ_FLAG_HIDDEN)) {
        return;
    }

    cam_sensor_isp_state_t state;
    esp_err_t err = cam_sensor_isp_get_state(&state);
    if (err == ESP_OK &&
            (!s_isp_last_state_valid || !ui_isp_state_equal(&s_isp_last_state, &state))) {
        ui_apply_isp_state(&state);
    }
}

static void ui_attach_isp_controls(void)
{
    lv_obj_add_event_cb(scr_dashboard_slider_Luminance, ui_isp_slider_event_cb,
                        LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(scr_dashboard_slider_contrast, ui_isp_slider_event_cb,
                        LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(scr_dashboard_slider_saturation, ui_isp_slider_event_cb,
                        LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(scr_dashboard_slider_hue, ui_isp_slider_event_cb,
                        LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(scr_dashboard_btn_contrast_auto, ui_isp_auto_button_event_cb,
                        LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(scr_dashboard_btn_saturation_auto, ui_isp_auto_button_event_cb,
                        LV_EVENT_VALUE_CHANGED, NULL);

    lv_obj_t *status_labels[] = {
        scr_dashboard_label_exposure_value,
        scr_dashboard_label_gain_value,
        scr_dashboard_label_wb_value,
    };
    for (size_t i = 0; i < sizeof(status_labels) / sizeof(status_labels[0]); ++i) {
        lv_obj_set_x(status_labels[i], 8);
        lv_obj_set_width(status_labels[i], 194);
        lv_label_set_long_mode(status_labels[i], LV_LABEL_LONG_CLIP);
        lv_obj_set_style_text_font(status_labels[i], &lv_font_MiSans_Heavy_16_16,
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_align(status_labels[i], LV_TEXT_ALIGN_CENTER,
                                    LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    cam_sensor_isp_state_t state;
    esp_err_t err = cam_sensor_isp_get_state(&state);
    if (err == ESP_OK) {
        ui_apply_isp_state(&state);
    } else {
        state = (cam_sensor_isp_state_t){0};
        ui_apply_isp_state(&state);
        ESP_LOGW(s_ui_isp_tag, "get ISP state failed: %s", esp_err_to_name(err));
    }
    if (!s_isp_refresh_timer) {
        s_isp_refresh_timer = lv_timer_create(ui_isp_refresh_timer_cb, 1000, NULL);
    }
}

// 上报间隔滑块拖动：单位秒，0 = 关闭指标上报。更新标签并把毫秒值下发给以太网层。
static void ui_report_interval_slider_event_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) != LV_EVENT_VALUE_CHANGED) {
        return;
    }
    int sec = (int)lv_slider_get_value(lv_event_get_target(e));
    if (sec < 0) {
        sec = 0;
    }
    ui_label_set_text_fmt_safe(scr_dashboard_label_report_interval_value, "%02ds", sec);
    ethernet_app_set_metrics_interval_ms((uint32_t)sec * 1000u);
}

// 图像上报开关：直接切换以太网层的图像上报总开关。
static void ui_report_image_switch_event_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) != LV_EVENT_VALUE_CHANGED) {
        return;
    }

    lv_obj_t *target = lv_event_get_target(e);
    bool enabled = lv_obj_has_state(target, LV_STATE_CHECKED) &&
                   lv_obj_has_state(scr_dashboard_sw_detect, LV_STATE_CHECKED);
    if (!enabled) {
        ui_state_modify(target, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
    }
    ethernet_app_set_report_image_enabled(enabled);
    ethernet_app_notify_control_state_changed();
}

// 指标上报开关：直接切换以太网层的指标上报总开关。
static void ui_report_metrics_switch_event_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) != LV_EVENT_VALUE_CHANGED) {
        return;
    }
    ethernet_app_set_report_metrics_enabled(lv_obj_has_state(lv_event_get_target(e), LV_STATE_CHECKED));
    ethernet_app_notify_control_state_changed();
}

// 网络设置页：上电填 IP 显示，挂上报间隔滑块与两个开关，并同步初值到以太网层。
static void ui_attach_network_controls(void)
{
    // 本机 / 主机 IP 为静态配置，上电即固定，直接填显示标签。
    ui_label_set_text_safe(scr_dashboard_label_local_ip_value, ethernet_app_get_local_ip());
    ui_label_set_text_safe(scr_dashboard_label_host_ip_value, ethernet_app_get_host_ip());

    if (scr_dashboard_slider_report_interval) {
        lv_obj_add_event_cb(scr_dashboard_slider_report_interval,
                            ui_report_interval_slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
        // 上报间隔范围 0-10s（覆盖 anyui 生成值，重生成不丢）。
        ui_slider_set_range_safe(scr_dashboard_slider_report_interval, 0, 10);
        // 同步以太网层默认间隔（1s），设滑块值并触发一次回调使标签一致。
        ui_slider_set_value_safe(scr_dashboard_slider_report_interval, 1);
        lv_obj_send_event(scr_dashboard_slider_report_interval, LV_EVENT_VALUE_CHANGED, NULL);
    }
    if (scr_dashboard_sw_report_image) {
        lv_obj_add_event_cb(scr_dashboard_sw_report_image,
                            ui_report_image_switch_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
        ui_state_modify(scr_dashboard_sw_report_image, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        lv_obj_send_event(scr_dashboard_sw_report_image, LV_EVENT_VALUE_CHANGED, NULL);
    }
    if (scr_dashboard_sw_report_metrics) {
        lv_obj_add_event_cb(scr_dashboard_sw_report_metrics,
                            ui_report_metrics_switch_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
        ui_state_modify(scr_dashboard_sw_report_metrics, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
        lv_obj_send_event(scr_dashboard_sw_report_metrics, LV_EVENT_VALUE_CHANGED, NULL);
    }
}

// 关于页静态信息：模型信息为编译期固定的已挂载模型名，绑定时经 getter 填一次即可。
static void ui_attach_about_info(void)
{
    if (scr_dashboard_label_model_info_value && s_handlers.model_info_get) {
        const char *info = s_handlers.model_info_get();
        if (info) {
            ui_label_set_text_safe(scr_dashboard_label_model_info_value, info);
        }
        // anyui 生成的值列 x=571 只够短文本；模型名较长会在容器(宽669)右侧被截断，
        // 故左移到 x=470 让 "waybill.espdl / logo.espdl" 完整显示(覆盖生成值，重生成不丢)。
        lv_obj_set_x(scr_dashboard_label_model_info_value, 470);
    }
}

static void system_restart(void)
{
    esp_restart();
}

static void ui_restart_button_event_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) != LV_EVENT_CLICKED) {
        return;
    }
    system_restart();
}

static void ui_attach_restart_button(void)
{
    if (scr_dashboard_btn_restart) {
        lv_obj_add_event_cb(scr_dashboard_btn_restart, ui_restart_button_event_cb,
                            LV_EVENT_CLICKED, NULL);
    }
}

#define UI_LOG_PAGE_COUNT 4

static int s_log_page = 1;

static void ui_show_log_page(int page)
{
    if (page < 1) {
        page = 1;
    } else if (page > UI_LOG_PAGE_COUNT) {
        page = UI_LOG_PAGE_COUNT;
    }

    s_log_page = page;

    lv_obj_t *pages[UI_LOG_PAGE_COUNT] = {
        scr_dashboard_cont_log_page1,
        scr_dashboard_cont_log_page2,
        scr_dashboard_cont_log_page3,
        scr_dashboard_cont_log_page4,
    };

    for (int i = 0; i < UI_LOG_PAGE_COUNT; i++) {
        if (pages[i]) {
            ui_flag_modify(pages[i], LV_OBJ_FLAG_HIDDEN,
                           (i + 1 == s_log_page) ? UI_FLAG_ACTION_REMOVE : UI_FLAG_ACTION_ADD);
        }
    }

    ui_label_set_text_fmt_safe(scr_dashboard_label_current_page, "%d/%d", s_log_page, UI_LOG_PAGE_COUNT);
}

static void ui_log_page_button_event_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) != LV_EVENT_CLICKED) {
        return;
    }

    lv_obj_t *target = lv_event_get_target(e);
    if (target == scr_dashboard_imgbtn_page_back) {
        ui_show_log_page(s_log_page - 1);
    } else if (target == scr_dashboard_imgbtn_page_next) {
        ui_show_log_page(s_log_page + 1);
    }
}

static void ui_attach_log_page_buttons(void)
{
    if (scr_dashboard_imgbtn_page_back) {
        lv_obj_add_event_cb(scr_dashboard_imgbtn_page_back, ui_log_page_button_event_cb,
                            LV_EVENT_CLICKED, NULL);
    }
    if (scr_dashboard_imgbtn_page_next) {
        lv_obj_add_event_cb(scr_dashboard_imgbtn_page_next, ui_log_page_button_event_cb,
                            LV_EVENT_CLICKED, NULL);
    }
    ui_show_log_page(1);
}

// 第二层聚合点：挂载本屏所有控件交互。新增一路 UI 交互→业务时，在此加一行。
static void ui_attach_all_widgets(void)
{
    ui_attach_brightness_slider();   // 内部同步默认亮度并触发一次回调点亮背光
    ui_attach_calibration_button();
    ui_attach_detection_switches();
    ui_attach_motor_controls();
    ui_attach_confidence_threshold_sliders();
    ui_attach_isp_controls();
    ui_attach_network_controls();    // 网络设置页：IP 显示 + 上报间隔/图像/指标控件
    ui_attach_about_info();          // 关于页：模型信息静态填充
    ui_attach_restart_button();
    ui_attach_log_page_buttons();
    // ui_attach_xxx();              // ← 新增控件交互挂载放这里
}

/* ============================================================================
 * 统一绑定入口
 * ----------------------------------------------------------------------------
 * 由 main 在 setupUi() 之后、LVGL 锁内调用一次。本函数只做两件事，保持稳定：
 *   1) 存下 main 注入的业务 handler 集合；
 *   2) 依次跑两层聚合点。
 * 后期扩展只动对应层的聚合点(ui_register_all_events / ui_attach_all_widgets)，
 * 本入口和签名都不需要改。
 * ==========================================================================*/
void ui_bind_dashboard(const ui_dashboard_handlers_t *handlers)
{
    if (handlers) {
        s_handlers = *handlers;
    }
    if (s_dashboard_bound) {
        return;
    }

    ui_register_all_events();   // 第一层：事件 → 数据 → UI 更新
    ui_attach_all_widgets();    // 第二层：UI 交互 → 触发业务调用
    s_dashboard_bound = true;
}

void ui_sync_remote_control_state(int brightness, bool detection_enabled,
                                  bool preview_overlay_enabled,
                                  int waybill_threshold, int logo_threshold,
                                  bool motor_output_enabled, int motor_a_speed,
                                  int motor_b_speed, int motor_c_speed,
                                  bool report_image_enabled,
                                  bool report_metrics_enabled)
{
    brightness = ui_clamp_percent(brightness);
    waybill_threshold = ui_clamp_percent(waybill_threshold);
    logo_threshold = ui_clamp_percent(logo_threshold);
    motor_a_speed = ui_clamp_percent(motor_a_speed);
    motor_b_speed = ui_clamp_percent(motor_b_speed);
    motor_c_speed = ui_clamp_percent(motor_c_speed);

    ui_slider_set_value_safe(scr_dashboard_slider_screen_brightness, brightness);
    ui_label_set_text_fmt_safe(scr_dashboard_label_runtime_Screen_Brightness__data,
                               "%d%%", brightness);
    ui_state_modify(scr_dashboard_sw_detect, LV_STATE_CHECKED,
                    detection_enabled ? UI_STATE_ACTION_ADD : UI_STATE_ACTION_REMOVE);
    ui_state_modify(scr_dashboard_sw_preview_overlay, LV_STATE_CHECKED,
                    detection_enabled && preview_overlay_enabled
                        ? UI_STATE_ACTION_ADD : UI_STATE_ACTION_REMOVE);
    ui_slider_set_value_safe(scr_dashboard_slider_confidence_threshold_mian,
                             waybill_threshold);
    ui_set_waybill_threshold_text(waybill_threshold);
    ui_slider_set_value_safe(scr_dashboard_slider_confidence_threshold_logo,
                             logo_threshold);
    ui_set_logo_threshold_text(logo_threshold);
    ui_state_modify(scr_dashboard_btn_open_motor, LV_STATE_CHECKED,
                    motor_output_enabled
                        ? UI_STATE_ACTION_ADD : UI_STATE_ACTION_REMOVE);
    ui_label_set_text_safe(scr_dashboard_btn_open_motor_label,
                           motor_output_enabled ? "开启" : "关闭");
    ui_slider_set_value_safe(scr_dashboard_slider_A_speed, motor_a_speed);
    ui_label_set_text_fmt_safe(scr_dashboard_label_A_speed_value, "%d%%", motor_a_speed);
    ui_slider_set_value_safe(scr_dashboard_slider_B_speed, motor_b_speed);
    ui_label_set_text_fmt_safe(scr_dashboard_label_B_speed_value, "%d%%", motor_b_speed);
    ui_slider_set_value_safe(scr_dashboard_slider_C_speed, motor_c_speed);
    ui_label_set_text_fmt_safe(scr_dashboard_label_C_speed_value, "%d%%", motor_c_speed);
    ui_state_modify(scr_dashboard_sw_report_image, LV_STATE_CHECKED,
                    detection_enabled && report_image_enabled
                        ? UI_STATE_ACTION_ADD : UI_STATE_ACTION_REMOVE);
    ui_state_modify(scr_dashboard_sw_report_metrics, LV_STATE_CHECKED,
                    report_metrics_enabled ? UI_STATE_ACTION_ADD : UI_STATE_ACTION_REMOVE);
}
