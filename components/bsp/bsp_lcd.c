#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_ldo_regulator.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_mipi_dsi.h"
#include "esp_lcd_ek79007.h"
#include "esp_lv_adapter_display.h"
#include "bsp_lcd.h"
#include <math.h>
#include "driver/ledc.h"
#include "hal/axi_icm_ll.h"

#define LCD_BK_LIGHT_GPIO       26
#define LCD_BK_LIGHT_ON_LEVEL   1
#define LCD_RST_GPIO            27
#define LCD_LANE_NUM            2
#define LCD_BIT_PER_PIXEL       24

#define BSP_LCD_BL_LEDC_MODE     LEDC_LOW_SPEED_MODE
#define BSP_LCD_BL_LEDC_TIMER    LEDC_TIMER_0
#define BSP_LCD_BL_LEDC_CHANNEL  LEDC_CHANNEL_0
#define BSP_LCD_BL_DUTY_RES      LEDC_TIMER_13_BIT
#define BSP_LCD_BL_DUTY_MAX      8191      // 2^13 - 1
#define BSP_LCD_BL_FREQ_HZ       5000
#define BSP_LCD_BL_GAMMA         2.2f
#define BSP_LCD_BL_FLOOR_PCT     8         // 最低亮度下限(占 max 的百分比)

#define MIPI_DSI_PHY_LDO_CHAN       3
#define MIPI_DSI_PHY_LDO_VOLTAGE_MV 2500

static const char *TAG = "bsp_lcd";
static esp_lcd_panel_handle_t s_panel_handle = NULL;

// PSRAM 带宽 QoS 调优 —— 解决摄像头 CSI + MIPI DSI 并发时 DSI FIFO underrun 蓝闪。
// ESP32-P4 内部 AXI ICM(多主控互连矩阵)下挂 PSRAM,各主控默认 arqos/awqos=0 平权轮询,
// CPU XIP 取指(Cache)与 DSI 读 framebuffer 平等抢总线 → DSI 抢不到则 FIFO 见底闪蓝。
// 这里重排仲裁权:抬高 DMA 读优先级(DSI/CSI 走 DW_GDMA),压低 Cache/CPU 让其退让。
// 依据: components/hal/esp32p4/include/hal/axi_icm_ll.h, arqos/awqos 4bit(0-15,越大越优先)。
static void bsp_psram_qos_tune(void)
{
    // DSI 显示 + 摄像头 CSI 的 DMA 读 PSRAM 抬到最高优先级 15(读=取 framebuffer 最敏感)
    axi_icm_ll_set_dw_gdma_qos_arbiter_prio(0, /*w*/8, /*r*/15);
    axi_icm_ll_set_dw_gdma_qos_arbiter_prio(1, /*w*/8, /*r*/15);
    // Cache(CPU XIP 取指/RODATA 读)压低,给 DSI 让路
    axi_icm_ll_set_cache_qos_arbiter_prio(/*w*/4, /*r*/4);
    // CPU 聚合口(含 USB/EMAC/SDMMC 等)同样压低
    axi_icm_ll_set_cpu_qos_arbiter_prio(/*w*/2, /*r*/2);
    ESP_LOGI(TAG, "[QoS] AXI ICM tuned: DW_GDMA rd=15 > Cache=4 > CPU=2");
}

void BSP_LCD_Init(void)
{
    // 背光 LEDC PWM (GPIO26)，开机置默认亮度
#if LCD_BK_LIGHT_GPIO >= 0
    ledc_timer_config_t bl_timer = {
        .speed_mode      = BSP_LCD_BL_LEDC_MODE,
        .duty_resolution = BSP_LCD_BL_DUTY_RES,
        .timer_num       = BSP_LCD_BL_LEDC_TIMER,
        .freq_hz         = BSP_LCD_BL_FREQ_HZ,
        .clk_cfg         = LEDC_AUTO_CLK,
    };
    ESP_ERROR_CHECK(ledc_timer_config(&bl_timer));

    ledc_channel_config_t bl_ch = {
        .gpio_num   = LCD_BK_LIGHT_GPIO,
        .speed_mode = BSP_LCD_BL_LEDC_MODE,
        .channel    = BSP_LCD_BL_LEDC_CHANNEL,
        .intr_type  = LEDC_INTR_DISABLE,
        .timer_sel  = BSP_LCD_BL_LEDC_TIMER,
        .duty       = 0,
        .hpoint     = 0,
        .flags.output_invert = (LCD_BK_LIGHT_ON_LEVEL == 0) ? 1 : 0,
    };
    ESP_ERROR_CHECK(ledc_channel_config(&bl_ch));

    BSP_LCD_SetBrightness(BSP_LCD_BL_DEFAULT_PERCENT);
    ESP_LOGI(TAG, "Backlight LEDC PWM on, default %d%%", BSP_LCD_BL_DEFAULT_PERCENT);
#endif

    // MIPI DSI PHY 供电
    esp_ldo_channel_handle_t ldo_mipi_phy = NULL;
    esp_ldo_channel_config_t ldo_cfg = {
        .chan_id    = MIPI_DSI_PHY_LDO_CHAN,
        .voltage_mv = MIPI_DSI_PHY_LDO_VOLTAGE_MV,
    };
    ESP_ERROR_CHECK(esp_ldo_acquire_channel(&ldo_cfg, &ldo_mipi_phy));
    ESP_LOGI(TAG, "MIPI DSI PHY powered on");

    // DSI 总线
    esp_lcd_dsi_bus_handle_t mipi_dsi_bus;
    esp_lcd_dsi_bus_config_t bus_cfg = EK79007_PANEL_BUS_DSI_2CH_CONFIG();
    ESP_ERROR_CHECK(esp_lcd_new_dsi_bus(&bus_cfg, &mipi_dsi_bus));

    // DBI IO
    esp_lcd_panel_io_handle_t mipi_dbi_io;
    esp_lcd_dbi_io_config_t dbi_cfg = EK79007_PANEL_IO_DBI_CONFIG();
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_dbi(mipi_dsi_bus, &dbi_cfg, &mipi_dbi_io));

    // 根据防撕裂模式和旋转角度调整帧缓冲数量
    uint8_t num_fbs = esp_lv_adapter_get_required_frame_buffer_count(
        ESP_LV_ADAPTER_TEAR_AVOID_MODE_DOUBLE_DIRECT,
        ESP_LV_ADAPTER_ROTATE_0
    );
    // DPI 时序 + 面板
    esp_lcd_dpi_panel_config_t dpi_cfg = EK79007_1024_600_PANEL_60HZ_CONFIG(LCD_COLOR_PIXEL_FORMAT_RGB888);
    dpi_cfg.num_fbs = num_fbs;
    dpi_cfg.flags.use_dma2d = 1;
    ek79007_vendor_config_t vendor_cfg = {
        .mipi_config = {
            .dsi_bus    = mipi_dsi_bus,
            .dpi_config = &dpi_cfg,
            .lane_num   = LCD_LANE_NUM,
        },
    };
    esp_lcd_panel_dev_config_t panel_cfg = {
        .reset_gpio_num = LCD_RST_GPIO,
        .rgb_ele_order  = LCD_RGB_ELEMENT_ORDER_RGB,
        .bits_per_pixel = LCD_BIT_PER_PIXEL,
        .vendor_config  = &vendor_cfg,
    };
    esp_lcd_panel_handle_t panel;
    ESP_ERROR_CHECK(esp_lcd_new_panel_ek79007(mipi_dbi_io, &panel_cfg, &panel));
    s_panel_handle = panel;
    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel));

    // PSRAM 带宽 QoS 调优:重排 AXI ICM 仲裁权,让 DSI/CSI DMA 优先于 CPU 取指
    bsp_psram_qos_tune();

    ESP_LOGI(TAG, "LCD init done");
}

void BSP_LCD_SetBrightness(int percent)
{
    if (percent < 0)   percent = 0;
    if (percent > 100) percent = 100;

    // gamma 感知映射 + 最低下限: duty = floor + (max-floor) * (pct/100)^gamma
    const float floor_duty = BSP_LCD_BL_DUTY_MAX * (BSP_LCD_BL_FLOOR_PCT / 100.0f);
    const float span = BSP_LCD_BL_DUTY_MAX - floor_duty;
    float norm = powf(percent / 100.0f, BSP_LCD_BL_GAMMA);
    uint32_t duty = (uint32_t)(floor_duty + span * norm + 0.5f);
    if (duty > BSP_LCD_BL_DUTY_MAX) {
        duty = BSP_LCD_BL_DUTY_MAX;
    }

    esp_err_t err = ledc_set_duty(BSP_LCD_BL_LEDC_MODE, BSP_LCD_BL_LEDC_CHANNEL, duty);
    if (err == ESP_OK) {
        err = ledc_update_duty(BSP_LCD_BL_LEDC_MODE, BSP_LCD_BL_LEDC_CHANNEL);
    }
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "set brightness failed: %s", esp_err_to_name(err));
    }
}

esp_lcd_panel_handle_t BSP_LCD_GetPanelHandle(void)
{
    return s_panel_handle;
}

esp_err_t BSP_LCD_GetFrameBuffers(void **fb0, void **fb1)
{
    if (!s_panel_handle) {
        return ESP_ERR_INVALID_STATE;
    }
    
    return esp_lcd_dpi_panel_get_frame_buffer(s_panel_handle, 2, fb0 , fb1);
}
