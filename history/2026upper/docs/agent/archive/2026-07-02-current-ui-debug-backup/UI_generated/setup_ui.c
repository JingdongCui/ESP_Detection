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

void setupUi(void) {
    lv_obj_t * scr_dashboard = setup_scr_dashboard();
    lv_scr_load(scr_dashboard);
    push_scr(setup_scr_dashboard);
}
