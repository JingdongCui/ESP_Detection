#include "ui_realtime_clock.h"

#include <stdio.h>
#include <time.h>

#include "bsp_lvgl_adapter_init.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lvgl.h"

extern lv_obj_t *scr_dashboard_label_detail_time;
extern lv_obj_t *scr_dashboard_label_time_01;
extern lv_obj_t *scr_dashboard_label_time_02;
extern lv_obj_t *scr_dashboard_label_time_03;
extern lv_obj_t *scr_dashboard_label_time_04;
extern lv_obj_t *scr_dashboard_label_time_05;
extern lv_obj_t *scr_dashboard_label_time_06;

static const char *TAG = "ui_clock";
static TaskHandle_t s_clock_task;

static bool format_time_offset(char out[9], int seconds_offset)
{
    time_t now = 0;
    time(&now);
    now += seconds_offset;

    struct tm tm_now = {0};
    localtime_r(&now, &tm_now);
    if (tm_now.tm_year < (2024 - 1900)) {
        snprintf(out, 9, "--:--:--");
        return false;
    }
    strftime(out, 9, "%H:%M:%S", &tm_now);
    return true;
}

static void set_label_time(lv_obj_t *label, const char *text)
{
    if (label) {
        lv_label_set_text(label, text);
    }
}

static void clock_task(void *arg)
{
    (void)arg;
    char t0[9], t1[9], t2[9], t3[9], t4[9], t5[9], t6[9];

    while (true) {
        format_time_offset(t0, 0);
        format_time_offset(t1, -5);
        format_time_offset(t2, -4);
        format_time_offset(t3, -3);
        format_time_offset(t4, -2);
        format_time_offset(t5, -1);
        format_time_offset(t6, 0);

        BSP_LVGL_Lock();
        set_label_time(scr_dashboard_label_detail_time, t0);
        set_label_time(scr_dashboard_label_time_01, t1);
        set_label_time(scr_dashboard_label_time_02, t2);
        set_label_time(scr_dashboard_label_time_03, t3);
        set_label_time(scr_dashboard_label_time_04, t4);
        set_label_time(scr_dashboard_label_time_05, t5);
        set_label_time(scr_dashboard_label_time_06, t6);
        BSP_LVGL_Unlock();

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

esp_err_t ui_realtime_clock_start(void)
{
    if (s_clock_task) {
        return ESP_OK;
    }

    BaseType_t ok = xTaskCreatePinnedToCore(
        clock_task,
        "ui_clock",
        3072,
        NULL,
        2,
        &s_clock_task,
        0);
    if (ok != pdPASS) {
        ESP_LOGE(TAG, "create ui clock task failed");
        return ESP_ERR_NO_MEM;
    }
    return ESP_OK;
}
