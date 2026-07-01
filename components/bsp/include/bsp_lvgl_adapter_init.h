#pragma once

#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

void         BSP_LVGL_AdapterInit(void);
lv_display_t *BSP_LVGL_GetDisplay(void);
lv_indev_t   *BSP_LVGL_GetTouchIndev(void);
void         BSP_LVGL_Lock(void);
void         BSP_LVGL_Unlock(void);

#ifdef __cplusplus
}
#endif
