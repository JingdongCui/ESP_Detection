#include "esp_log.h"
#include "system_init.h"
#include "bsp_lcd.h"
#include "bsp_touch.h"
#include "bsp_motor.h"
#include "bsp_encoder.h"
#include "bsp_lvgl_adapter_init.h"
#include "setup_ui.h"
#include "vision_app.h"
#include "vision_preview.h"
#include "ethernet_app.h"
#include "sorting_sim_control.h"
#include "SEGGER_RTT.h"
#include "ui_realtime_clock.h"

extern lv_obj_t *scr_dashboard_cont_live_vedio;
extern lv_obj_t *scr_dashboard_label_runtime_fps;
extern lv_obj_t *scr_dashboard_label_runtime_detection_status;

static const char *TAG = "system";

static void init_dashboard_preview(void)
{
    setupUi();

    //把摄像头预览控件挂到 Dashboard 容器上
    esp_err_t ret = vision_preview_init(scr_dashboard_cont_live_vedio, 0, 0, 640, 375);
    if (ret != ESP_OK) {
          ESP_LOGE(TAG, "Preview init failed: %s", esp_err_to_name(ret));
          return;
    }

    //绑定外部创建的 FPS 和检测结果标签控件
    vision_preview_bind_labels(scr_dashboard_label_runtime_fps, scr_dashboard_label_runtime_detection_status);
    vision_preview_set_waiting();
    ui_realtime_clock_start();
}

void System_Init(void)
{
    ESP_LOGI(TAG, "System initialization start");

    BSP_LCD_Init();

    BSP_Touch_Init();

    esp_err_t encoder_ret = bsp_encoder_init();
    if (encoder_ret != ESP_OK) {
        ESP_LOGW(TAG, "Encoder BSP init failed: %s", esp_err_to_name(encoder_ret));
    }

    BSP_LVGL_AdapterInit();

    //这个lvgl锁不能放在 init_dashboard_preview 内，否则里面的 lv_obj_t * 类型的控件指针就无法被正确解析了，导致绑定标签失败
    //不加会导致屏显异常
    BSP_LVGL_Lock();
    init_dashboard_preview();
    BSP_LVGL_Unlock();

    //启动以太网链路和静态 IP；当前只启用基础 IPv4 网络，没有创建 TCP/UDP 应用服务
    ethernet_app_start();
    sorting_sim_debug_start();
    sorting_sim_control_set_motor_output_enabled(true);

    vision_app_start();
    

    ESP_LOGI(TAG, "System initialization done");
}
