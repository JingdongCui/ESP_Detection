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

/* ----------------------------------------------------------------------------
 * 触控命中区扩展配置：只扩大触控范围，不改变控件视觉尺寸和布局。
 * X 表示左右各扩展的像素，Y 表示上下各扩展的像素；可按屏幕触控体验统一调整。
 * 标准 button/imagebutton、slider、switch 会按 LVGL 控件类型自动分类，无需逐个登记：
 *   BUTTON 宏：按钮、图片按钮等矩形控件
 *   SLIDER 宏：亮度、阈值、速度等可拖动滑块
 *   SWITCH 宏：开关控件
 * AnyUI 若用普通 lv_obj 容器模拟可点击控件，需将指针加入 ui.c 中的 button_targets。
 * setupUi() 创建控件后，在 LVGL 锁内调用一次 ui_expand_dashboard_hit_areas() 即可。
 * --------------------------------------------------------------------------*/
#define UI_HIT_BUTTON_EXT_X 16
#define UI_HIT_BUTTON_EXT_Y 16
#define UI_HIT_SLIDER_EXT_X 12
#define UI_HIT_SLIDER_EXT_Y 24
#define UI_HIT_SWITCH_EXT_X 16
#define UI_HIT_SWITCH_EXT_Y 20

void ui_expand_dashboard_hit_areas(void);

// 开机默认亮度，与 BSP_LCD_BL_DEFAULT_PERCENT 保持同值(UI 不依赖 bsp 头故独立定义)
#define UI_BRIGHTNESS_DEFAULT_PERCENT 80

/* ----------------------------------------------------------------------------
 * 业务 handler 类型：UI 控件交互时回调到 main 注入的实现，解耦 UI 与 bsp/vision。
 * 新增一类业务交互时，在此加一个 handler typedef，并在下方结构体追加字段。
 * --------------------------------------------------------------------------*/

// 亮度回调：亮度滑块值变化时被调用，percent 为 0~100。main 注入 BSP 背光函数。
typedef void (*ui_brightness_handler_t)(int percent);

// LOGO 按键回调：点击 dashboard 左上角 LOGO 按键时被调用，无参，由 main 注入业务实现。
typedef void (*ui_calibration_handler_t)(void);

// 识别设置开关回调：enabled 为 true 表示启用对应功能。
typedef bool (*ui_bool_getter_t)(void);
typedef void (*ui_bool_switch_handler_t)(bool enabled);

// 电机速度回调：motor_index 为 0~2，对应 A/B/C，percent 为 0~100。
typedef int  (*ui_motor_speed_getter_t)(int motor_index);
typedef void (*ui_motor_speed_handler_t)(int motor_index, int percent);

// 百分比值回调：percent 为 0~100；getter 返回当前默认/运行时值，setter 写入业务层。
typedef int  (*ui_percent_getter_t)(void);
typedef void (*ui_percent_handler_t)(int percent);

// 模型信息 getter：返回 vision 已挂载模型名的静态只读字符串(如 "waybill.espdl / logo.espdl")，
// 供关于页“模型信息”标签在绑定时一次性填充。返回值须为常驻字符串，UI 不释放。
typedef const char *(*ui_model_info_getter_t)(void);

/* dashboard 业务 handler 集合：由 main 一次性填好传入 ui_bind_dashboard。
 * 用结构体而非裸参数，后期新增业务只需在此追加字段，不改 ui_bind_dashboard 签名，
 * main 端也只多填一行。未填(NULL)的 handler 对应交互被安全忽略。 */
typedef struct {
    ui_brightness_handler_t brightness;              // 亮度滑块 → 背光
    ui_calibration_handler_t calibration;            // LOGO 按键 → 注入的业务回调
    ui_bool_switch_handler_t detection_enabled;      // 检测开关 → vision 推理门控
    ui_bool_switch_handler_t preview_overlay_enabled;// 预览叠加框开关 → vision 画框门控
    ui_bool_getter_t motor_output_enabled_get;        // 电机总开关初值读取
    ui_bool_switch_handler_t motor_output_enabled_set;// 电机总开关 → 分拣输出门控
    ui_motor_speed_getter_t motor_speed_get;          // A/B/C 速度初值读取，索引 0/1/2
    ui_motor_speed_handler_t motor_speed_set;         // A/B/C 速度写入，索引 0/1/2
    ui_percent_getter_t waybill_score_threshold_get; // 面单阈值滑块默认值读取
    ui_percent_handler_t waybill_score_threshold_set;// 面单阈值滑块 → vision 模型阈值
    ui_percent_getter_t logo_score_threshold_get;    // Logo 阈值滑块默认值读取
    ui_percent_handler_t logo_score_threshold_set;   // Logo 阈值滑块 → vision 模型阈值
    ui_model_info_getter_t model_info_get;           // 关于页“模型信息”标签 → vision 已挂载模型名
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
void ui_sync_remote_control_state(int brightness, bool detection_enabled,
                                  bool preview_overlay_enabled,
                                  int waybill_threshold, int logo_threshold,
                                  bool motor_output_enabled, int motor_a_speed,
                                  int motor_b_speed, int motor_c_speed,
                                  bool report_image_enabled,
                                  bool report_metrics_enabled);

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
