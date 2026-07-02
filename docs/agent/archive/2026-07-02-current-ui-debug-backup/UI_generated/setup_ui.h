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

#ifndef __SETUP_UI_H__
#define __SETUP_UI_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"
#include "sdk.h"
#include "animations.h"


extern lv_img_dsc_t const dash_c2_212x66;

extern lv_img_dsc_t const dash_c_212x66;

extern lv_img_dsc_t const dash_a_212x66;

extern lv_img_dsc_t const sysc_212x66;

extern lv_img_dsc_t const logc_212x66;

extern lv_img_dsc_t const setc_212x66;

extern lv_img_dsc_t const setc1_212x66;

extern lv_img_dsc_t const set_a_212x66;

extern lv_img_dsc_t const logc1_212x66;

extern lv_img_dsc_t const log_a_212x66;

extern lv_img_dsc_t const sysc1_212x66;

extern lv_img_dsc_t const sys_a_212x66;

extern lv_img_dsc_t const GREEN_20x20;

extern lv_img_dsc_t const RED_20x20;

extern lv_img_dsc_t const YELLOW_20x20;

extern lv_img_dsc_t const title_1_800x60;


LV_FONT_DECLARE(lv_font_MiSansVF_14_14);
LV_FONT_DECLARE(lv_font_Misans_Heavy_14_14);
LV_FONT_DECLARE(lv_font_MiSans_Heavy_24_24);

lv_obj_t * setup_scr_dashboard(void);
void setupUi(void);



#ifdef __cplusplus
}
#endif
#endif
