#include "esp_log.h"
#include "esp_lv_adapter.h"
#include "bsp_lcd.h"
#include "bsp_touch.h"
#include "bsp_lvgl_adapter_init.h"

#define LCD_H_RES   1024
#define LCD_V_RES   600

static const char *TAG = "bsp_lvgl";

static lv_display_t *s_disp = NULL;
static lv_indev_t   *s_touch_indev = NULL;

void BSP_LVGL_AdapterInit(void)
{
    // 初始化 LVGL 适配器（创建 tick 定时器 + 任务）
    esp_lv_adapter_config_t adapter_cfg = ESP_LV_ADAPTER_DEFAULT_CONFIG();
    adapter_cfg.task_stack_size = 32 * 1024;
    adapter_cfg.task_priority   = 5;
    adapter_cfg.task_core_id    = 1;
    ESP_ERROR_CHECK(esp_lv_adapter_init(&adapter_cfg));

    // 注册 MIPI DSI 显示屏
    esp_lv_adapter_display_config_t disp_cfg = {
        .panel    = BSP_LCD_GetPanelHandle(),
        .panel_io = NULL,
        .profile  = {
            .interface             = ESP_LV_ADAPTER_PANEL_IF_MIPI_DSI,
            .rotation              = ESP_LV_ADAPTER_ROTATE_0,
            .hor_res               = LCD_H_RES,
            .ver_res               = LCD_V_RES,
            .buffer_height         = 50,
            .use_psram             = false,
            .enable_ppa_accel      = true,
            .require_double_buffer = false,
            .mono_layout           = ESP_LV_ADAPTER_MONO_LAYOUT_NONE,
        },
        .tear_avoid_mode = ESP_LV_ADAPTER_TEAR_AVOID_MODE_TRIPLE_PARTIAL,
        .te_sync         = ESP_LV_ADAPTER_TE_SYNC_DISABLED(),
    };
    s_disp = esp_lv_adapter_register_display(&disp_cfg);
    ESP_ERROR_CHECK(s_disp == NULL ? ESP_FAIL : ESP_OK);
    ESP_LOGI(TAG, "Display registered (%dx%d)", LCD_H_RES, LCD_V_RES);

    // 注册 GT911 触摸
    esp_lv_adapter_touch_config_t touch_cfg =
        ESP_LV_ADAPTER_TOUCH_DEFAULT_CONFIG(s_disp, BSP_Touch_GetHandle());
    s_touch_indev = esp_lv_adapter_register_touch(&touch_cfg);
    ESP_ERROR_CHECK(s_touch_indev == NULL ? ESP_FAIL : ESP_OK);
    ESP_LOGI(TAG, "Touch registered");

    // 启动 LVGL 任务
    ESP_ERROR_CHECK(esp_lv_adapter_start());
    ESP_LOGI(TAG, "LVGL adapter started");
}

lv_display_t *BSP_LVGL_GetDisplay(void)
{
    return s_disp;
}

lv_indev_t *BSP_LVGL_GetTouchIndev(void)
{
    return s_touch_indev;
}

void BSP_LVGL_Lock(void)
{
    ESP_ERROR_CHECK(esp_lv_adapter_lock(-1));
}

void BSP_LVGL_Unlock(void)
{
    esp_lv_adapter_unlock();
}
