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
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

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

/* ============================================================================
 * SDK 基础设施（anyui 生成代码依赖）
 * ----------------------------------------------------------------------------
 * flag/state 修改、动画回调、屏幕切换等通用工具，与具体业务无关。
 * ==========================================================================*/

void ui_flag_modify(lv_obj_t *target, int32_t flag, int value)
{
    if (value == UI_FLAG_ACTION_TOGGLE) {
        if (lv_obj_has_flag(target, flag)) {
            lv_obj_remove_flag(target, flag);
        } else {
            lv_obj_add_flag(target, flag);
        }
    } else if (value == UI_FLAG_ACTION_ADD) {
        lv_obj_add_flag(target, flag);
    } else {
        lv_obj_remove_flag(target, flag);
    }
}

void ui_state_modify(lv_obj_t *target, int32_t state, int value)
{
    if (value == UI_STATE_ACTION_TOGGLE) {
        if (lv_obj_has_state(target, state)) {
            lv_obj_remove_state(target, state);
        } else {
            lv_obj_add_state(target, state);
        }
    } else if (value == UI_STATE_ACTION_ADD) {
        lv_obj_add_state(target, state);
    } else {
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
extern lv_obj_t *scr_dashboard_slider_82EIlsYJ;
extern lv_obj_t *scr_dashboard_label_runtime_bright__data;
extern lv_obj_t *scr_dashboard_label_runtime_recognition_status;
extern lv_obj_t *scr_dashboard_label_runtime_confidence;
extern lv_obj_t *scr_dashboard_label_runtime_recognition_fps;
extern lv_obj_t *scr_dashboard_label_runtime_infer_time;
extern lv_obj_t *scr_dashboard_label_runtime_express_company;
extern lv_obj_t *scr_dashboard_label_JT_A;
extern lv_obj_t *scr_dashboard_label_JT_M;
extern lv_obj_t *scr_dashboard_label_ZT_A;
extern lv_obj_t *scr_dashboard_label_ZT_M;
extern lv_obj_t *scr_dashboard_label_YD_A;
extern lv_obj_t *scr_dashboard_label_YD_M;
extern lv_obj_t *scr_dashboard_imgbtn_logo;

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

static void ui_arc_percent_set_by_label(lv_obj_t *label, int value)
{
    if (!label) {
        return;
    }

    value = ui_clamp_percent(value);

    lv_obj_t *parent = lv_obj_get_parent(label);
    lv_obj_t *arc = ui_find_child_arc(parent);
    if (arc) {
        lv_arc_set_value(arc, value);
    }

    ui_label_set_text_fmt_safe(label, "%d%%", value);
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

    ui_arc_percent_set_by_label(scr_dashboard_label_runtime_cpu_core1_usage, monitor->cpu1_usage);
    ui_arc_percent_set_by_label(scr_dashboard_label_runtime_cpu_core2_usage, monitor->cpu2_usage);
    ui_arc_percent_set_by_label(scr_dashboard_label_cpu_02_value, monitor->cpu1_usage);
    ui_arc_percent_set_by_label(scr_dashboard_label_cpu_03_value, monitor->cpu2_usage);
    ui_arc_percent_set_by_label(scr_dashboard_label_cpu_total_value, monitor->cpu_total_usage);

    lv_slider_set_value(scr_dashboard_slider_memory_usage, ui_clamp_percent(monitor->memory_usage), LV_ANIM_OFF);
    ui_label_set_text_fmt_safe(scr_dashboard_label_memory_usage_value, "%d%%", ui_clamp_percent(monitor->memory_usage));
    ui_label_set_text_fmt_safe(scr_dashboard_label_ram_value, "%d", monitor->internal_free_kb);
    ui_label_set_text_fmt_safe(scr_dashboard_label_psram_usage_value, "%d%%", ui_clamp_percent(monitor->psram_usage));
    lv_slider_set_value(scr_dashboard_slider_psram_usage, ui_clamp_percent(monitor->psram_usage), LV_ANIM_OFF);
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

    ui_label_set_text_safe(scr_dashboard_label_runtime_recognition_status, v->status);
    ui_label_set_text_fmt_safe(scr_dashboard_label_runtime_confidence, "%d%%", v->confidence);
    ui_label_set_text_fmt_safe(scr_dashboard_label_runtime_recognition_fps, "%d.%d",
                          v->fps_x10 / 10, v->fps_x10 % 10);
    ui_label_set_text_fmt_safe(scr_dashboard_label_runtime_infer_time, "%d", v->infer_time_ms);
    ui_label_set_text_safe(scr_dashboard_label_runtime_express_company, v->company);
    ui_label_set_text_fmt_safe(scr_dashboard_label_JT_A, "A:%d.%02d%%", v->jt_a / 100, v->jt_a % 100);
    ui_label_set_text_fmt_safe(scr_dashboard_label_JT_M, "M:%d.%02d%%", v->jt_m / 100, v->jt_m % 100);
    ui_label_set_text_fmt_safe(scr_dashboard_label_ZT_A, "A:%d.%02d%%", v->zt_a / 100, v->zt_a % 100);
    ui_label_set_text_fmt_safe(scr_dashboard_label_ZT_M, "M:%d.%02d%%", v->zt_m / 100, v->zt_m % 100);
    ui_label_set_text_fmt_safe(scr_dashboard_label_YD_A, "A:%d.%02d%%", v->yd_a / 100, v->yd_a % 100);
    ui_label_set_text_fmt_safe(scr_dashboard_label_YD_M, "M:%d.%02d%%", v->yd_m / 100, v->yd_m % 100);
}

static uint32_t ui_register_vision_events(event_table_t *table)
{
    return register_event(table, EVT_VISION, EVT_VISION_RESULT_CHANGED, 0,
                          sizeof(vision_result_event_data_t), ui_vision_result_event_cb);
}

// 第一层聚合点：注册本屏所有事件回调。新增一路事件→UI更新时，在此加一行。
static void ui_register_all_events(void)
{
    event_table_t *table = get_current_event_table();
    ui_register_system_monitor_events(table);
    ui_register_vision_events(table);
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

// 亮度滑块拖动：更新百分比标签，并把新亮度交给注入的背光 handler。
static void ui_brightness_slider_event_cb(lv_event_t *e)
{
    lv_obj_t *slider = lv_event_get_target(e);
    int pct = (int)lv_slider_get_value(slider);
    pct = ui_clamp_percent(pct);

    if (scr_dashboard_label_runtime_bright__data) {
        lv_label_set_text_fmt(scr_dashboard_label_runtime_bright__data, "%d%%", pct);
    }
    if (s_handlers.brightness) {
        s_handlers.brightness(pct);
    }
}

// 给亮度滑块挂事件回调，并把滑块/标签初值同步为 UI_BRIGHTNESS_DEFAULT_PERCENT。
static void ui_attach_brightness_slider(void)
{
    if (!scr_dashboard_slider_82EIlsYJ) {
        return;
    }

    lv_obj_add_event_cb(scr_dashboard_slider_82EIlsYJ, ui_brightness_slider_event_cb,
                        LV_EVENT_VALUE_CHANGED, NULL);

    // 同步开机默认亮度：设滑块值并主动触发一次回调，使标签与背光与 UI 一致
    lv_slider_set_value(scr_dashboard_slider_82EIlsYJ, UI_BRIGHTNESS_DEFAULT_PERCENT, LV_ANIM_OFF);
    lv_obj_send_event(scr_dashboard_slider_82EIlsYJ, LV_EVENT_VALUE_CHANGED, NULL);
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

// 第二层聚合点：挂载本屏所有控件交互。新增一路 UI 交互→业务时，在此加一行。
static void ui_attach_all_widgets(void)
{
    ui_attach_brightness_slider();   // 内部同步默认亮度并触发一次回调点亮背光
    ui_attach_calibration_button();
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
    ui_register_all_events();   // 第一层：事件 → 数据 → UI 更新
    ui_attach_all_widgets();    // 第二层：UI 交互 → 触发业务调用
}
