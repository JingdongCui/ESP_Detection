#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "system_init.h"
#include "bsp_lcd.h"
#include "bsp_touch.h"
#include "bsp_encoder.h"
#include "bsp_lvgl_adapter_init.h"
#include "setup_ui.h"
#include "vision_app.h"
#include "vision_preview.h"
#include "ethernet_app.h"
#include "SEGGER_RTT.h"
#include "ui_realtime_clock.h"

extern lv_obj_t *scr_dashboard_cont_live_vedio;
extern lv_obj_t *scr_dashboard_label_runtime_infer_time;
extern lv_obj_t *scr_dashboard_label_runtime_recognition_status;

static const char *TAG = "system";

static void init_dashboard_preview(void)
{
    ESP_LOGI(TAG, "setupUi start stack_hwm=%u", (unsigned)uxTaskGetStackHighWaterMark(NULL));
    setupUi();
    ESP_LOGI(TAG, "setupUi done stack_hwm=%u", (unsigned)uxTaskGetStackHighWaterMark(NULL));

    //把摄像头预览控件挂到 Dashboard 容器上
    ESP_LOGI(TAG, "vision_preview_init start");
    esp_err_t ret = vision_preview_init(scr_dashboard_cont_live_vedio, 0, 0, 640, 375);
    if (ret != ESP_OK) {
          ESP_LOGE(TAG, "Preview init failed: %s", esp_err_to_name(ret));
          return;
    }
    ESP_LOGI(TAG, "vision_preview_init done");

    // 绑定新版 UI 的耗时和识别状态标签：host 推理返回后显示一帧总耗时和类别状态。
    vision_preview_bind_labels(scr_dashboard_label_runtime_infer_time, scr_dashboard_label_runtime_recognition_status);
    vision_preview_set_waiting();
    ui_realtime_clock_start();
    ESP_LOGI(TAG, "dashboard preview init done");
}

void System_Init(void)
{
    ESP_LOGI(TAG, "System initialization start stack_hwm=%u", (unsigned)uxTaskGetStackHighWaterMark(NULL));

    BSP_LCD_Init();

    BSP_Touch_Init();

    esp_err_t encoder_ret = bsp_encoder_init();
    if (encoder_ret != ESP_OK) {
        ESP_LOGW(TAG, "Encoder BSP init failed: %s", esp_err_to_name(encoder_ret));
    }

    BSP_LVGL_AdapterInit();

    //这个lvgl锁不能放在 init_dashboard_preview 内，否则里面的 lv_obj_t * 类型的控件指针就无法被正确解析了，导致绑定标签失败
    //不加会导致屏显异常
    ESP_LOGI(TAG, "dashboard preview lock");
    BSP_LVGL_Lock();
    init_dashboard_preview();
    BSP_LVGL_Unlock();
    ESP_LOGI(TAG, "dashboard preview unlocked");

    // Host inference validation needs the scarce internal RAM for CSI/ISP and LwIP.
    // The previous sorter debug/system UI is backed up under docs/agent/archive for later re-integration.

    //启动以太网链路和静态 IP；当前只启用基础 IPv4 网络，没有创建 TCP/UDP 应用服务
    ESP_LOGI(TAG, "ethernet_app_start begin");
    ethernet_app_start();
    ESP_LOGI(TAG, "ethernet_app_start done");

    // Host-inference validation path owns the camera in Ethernet_app:
    // frame -> TCP host inference -> result -> preview overlay.
    

    ESP_LOGI(TAG, "System initialization done");
}
