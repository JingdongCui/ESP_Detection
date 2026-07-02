/*
 * ui.h - Animation helpers for anyui SDK
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
#ifndef __UI_H__
#define __UI_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"
#include "mem.h"
#include "evt.h"

#define UI_FLAG_ACTION_ADD 0
#define UI_FLAG_ACTION_REMOVE 1
#define UI_FLAG_ACTION_TOGGLE 2

void ui_flag_modify(lv_obj_t *target, int32_t flag, int value);

#define UI_STATE_ACTION_ADD 0
#define UI_STATE_ACTION_REMOVE 1
#define UI_STATE_ACTION_TOGGLE 2

void ui_state_modify(lv_obj_t *target, int32_t state, int value);

typedef struct {
    lv_obj_t *target;
} anim_user_data_t;

void anim_cb_transform_x(lv_anim_t *a, int32_t x);
void anim_cb_transform_y(lv_anim_t *a, int32_t y);
void anim_cb_set_width(lv_anim_t *a, int32_t width);
void anim_cb_set_height(lv_anim_t *a, int32_t height);
void anim_cb_set_opacity(lv_anim_t *a, int32_t opacity);
void anim_cb_set_image_angle(lv_anim_t *a, int32_t angle);
void anim_cb_set_image_zoom(lv_anim_t *a, int32_t zoom);

int32_t anim_cb_get_x(lv_anim_t *a);
int32_t anim_cb_get_y(lv_anim_t *a);
int32_t anim_cb_get_width(lv_anim_t *a);
int32_t anim_cb_get_height(lv_anim_t *a);
int32_t anim_cb_get_opacity(lv_anim_t *a);
int32_t anim_cb_get_image_zoom(lv_anim_t *a);
int32_t anim_cb_get_image_angle(lv_anim_t *a);

void anim_cb_free_user_data(lv_anim_t *a);

void ui_obj_nullify(lv_obj_t **obj);

typedef struct {
    lv_obj_t **new_scr_pptr;
    lv_obj_t **old_scr_pptr;
    lv_scr_load_anim_t anim_type;
    uint32_t time;
    uint32_t delay;
    bool auto_del;
} ui_scr_load_with_anim_t;

void ui_scr_load_with_anim(ui_scr_load_with_anim_t *params);

// 开机默认亮度，与 BSP_LCD_BL_DEFAULT_PERCENT 保持同值(UI 不依赖 bsp 头故独立定义)
#define UI_BRIGHTNESS_DEFAULT_PERCENT 80

/* ----------------------------------------------------------------------------
 * 业务 handler 类型：UI 控件交互时回调到 main 注入的实现，解耦 UI 与 bsp/vision。
 * 新增一类业务交互时，在此加一个 handler typedef，并在下方结构体追加字段。
 * --------------------------------------------------------------------------*/

// 亮度回调：亮度滑块值变化时被调用，percent 为 0~100。main 注入 BSP 背光函数。
typedef void (*ui_brightness_handler_t)(int percent);

// 校准回调：点击 dashboard 左上角 LOGO 按键时被调用，无参。main 注入
// vision 的 roi_tuning_request_calibration，请求下一帧执行 ROI 自适应调参。
typedef void (*ui_calibration_handler_t)(void);

/* dashboard 业务 handler 集合：由 main 一次性填好传入 ui_bind_dashboard。
 * 用结构体而非裸参数，后期新增业务只需在此追加字段，不改 ui_bind_dashboard 签名，
 * main 端也只多填一行。未填(NULL)的 handler 对应交互被安全忽略。 */
typedef struct {
    ui_brightness_handler_t brightness;    // 亮度滑块 → 背光
    ui_calibration_handler_t calibration;  // LOGO 按键 → ROI 校准
    // 新增业务 handler 在此追加，例如：
    // ui_xxx_handler_t xxx;
} ui_dashboard_handlers_t;

/* dashboard 统一绑定入口：一次性完成本屏所有「事件注册」与「控件交互挂载」。
 * 内部分两层(详见 ui.c 分区注释)：
 *   第一层(数据→UI)：注册 system_monitor/vision 事件，到来时刷新标签/弧形/滑块。
 *   第二层(交互→业务)：挂载亮度滑块、LOGO 按键事件，把用户操作转发给 handlers 里的回调。
 * 必须在 setupUi() 之后、LVGL 锁内调用(内部要读控件指针、挂控件事件)。
 * handlers 可为 NULL(则所有业务交互被忽略)；单个字段为 NULL 时对应交互被忽略。 */
void ui_bind_dashboard(const ui_dashboard_handlers_t *handlers);

/* Check LVGL version, LVGL 9.x has `LVGL_VERSION_MAJOR` defined */
#if defined(LVGL_VERSION_MAJOR) && LVGL_VERSION_MAJOR >= 9
/* LVGL 9.x */
#else
/* LVGL 8.x */
#define lv_obj_remove_flag lv_obj_clear_flag
#define lv_obj_remove_state lv_obj_clear_state
#endif

#ifdef __cplusplus
}
#endif

#endif
