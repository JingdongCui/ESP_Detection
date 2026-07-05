#include "esp_err.h"
#include "esp_log.h"
#include "system_init.h"
#include "bsp_lcd.h"
#include "bsp_touch.h"
#include "bsp_motor.h"
#include "bsp_encoder.h"
#include "bsp_cam_sensor.h"
#include "bsp_lvgl_adapter_init.h"
#include "setup_ui.h"
#include "ui.h"
#include "ethernet_app.h"
#include "sorting_sim_control.h"
#include "vision.h"
#include "vision_model.h"
#include "roi_tuning.h"
#include "system_monitor.h"

static const char *TAG = "system";

//硬件分拣调试模式：上位机 TCP 链路关闭，系统仅在 RTT 输出调试信息
#define SORTER_HARDWARE_DEBUG_MONITOR 1
#ifndef SORTER_TCP_LINK_ENABLE
#define SORTER_TCP_LINK_ENABLE 1
#endif
//日志过滤：1关闭所有模块日志，仅保留系统、分拣仿真、视觉、BSP 关键日志
#ifndef SORTER_LOG_FILTER_ENABLE
#define SORTER_LOG_FILTER_ENABLE 0
#endif

static void configure_monitor_logging(void)
{
#if SORTER_LOG_FILTER_ENABLE
    esp_log_level_set("*", ESP_LOG_WARN);
    esp_log_level_set("system", ESP_LOG_INFO);
    esp_log_level_set("sorting_sim", ESP_LOG_INFO);
    esp_log_level_set("bsp_motor", ESP_LOG_INFO);
    esp_log_level_set("bsp_sort_sensor", ESP_LOG_INFO);
    esp_log_level_set("bsp_encoder", ESP_LOG_INFO);
    esp_log_level_set("vision", ESP_LOG_INFO);
    esp_log_level_set("vision_model", ESP_LOG_WARN);
    esp_log_level_set("ISP_AWB", ESP_LOG_ERROR);
#endif
}

void System_Init(void)
{
    configure_monitor_logging();
    ESP_LOGI(TAG, "System initialization start");

    BSP_LCD_Init();

    BSP_Touch_Init();

    // 摄像头硬件初始化。SCCB 复用触摸已建好的 I2C 总线（GPIO8/7 共享），
    // 故必须在 BSP_Touch_Init 之后。总线获取与参数封装在 bsp_cam_sensor.c 内部。
    cam_sensor_init();

    //bsp_motor_init();

    //bsp_encoder_init();

    BSP_LVGL_AdapterInit();

    //setupUi 内部创建所有屏幕控件，必须在 LVGL 锁内调用以保证控件指针正确解析
    BSP_LVGL_Lock();
    setupUi();
    // dashboard 统一绑定：内部完成事件注册(数据→UI)与控件交互挂载(交互→业务)两层接线。
    // 复合字面量内联注入背光/校准 handler，解耦 UI 与 bsp/vision；后期加业务在此多填一个字段。
    ui_bind_dashboard(&(ui_dashboard_handlers_t){
        .brightness = BSP_LCD_SetBrightness,
        .calibration = vision_frame_dump_request,   // 诊断期临时占用：LOGO 键触发抓帧（ROI 校准仍为 stub）
        .detection_enabled = vision_set_detection_enabled,
        .preview_overlay_enabled = vision_set_preview_overlay_enabled,
        .waybill_score_threshold_get = vision_model_get_waybill_score_threshold_percent,
        .waybill_score_threshold_set = vision_model_set_waybill_score_threshold_percent,
        .logo_score_threshold_get = vision_model_get_logo_score_threshold_percent,
        .logo_score_threshold_set = vision_model_set_logo_score_threshold_percent,
    });
    BSP_LVGL_Unlock();

    //启动视觉链路：采集 + PPA 缩放 + LCD 视频区域直刷。需在 setupUi 建好预览容器后调用。
    vision_start();

    //sorting_sim_debug_start();
    //sorting_sim_control_set_motor_output_enabled(true);
    //sorting_sim_control_set_sensor_input_enabled(true);

    //启动系统监视：后台周期采集 CPU/内存/系统指标，写入快照并通过 RTT 打印
    system_monitor();

#if SORTER_TCP_LINK_ENABLE
    esp_err_t eth_ret = ethernet_app_start();
    if (eth_ret != ESP_OK) {
        ESP_LOGW(TAG, "Ethernet sorter link start failed: %s", esp_err_to_name(eth_ret));
    }
#elif SORTER_HARDWARE_DEBUG_MONITOR
    ESP_LOGI(TAG, "硬件分拣调试: 上位机TCP链路已关闭");
#endif

    ESP_LOGI(TAG, "System initialization done");
}
