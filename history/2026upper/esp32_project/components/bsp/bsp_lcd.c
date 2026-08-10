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

#define LCD_BK_LIGHT_GPIO       26
#define LCD_BK_LIGHT_ON_LEVEL   1
#define LCD_RST_GPIO            27
#define LCD_LANE_NUM            2
#define LCD_BIT_PER_PIXEL       24

#define MIPI_DSI_PHY_LDO_CHAN       3
#define MIPI_DSI_PHY_LDO_VOLTAGE_MV 2500

static const char *TAG = "bsp_lcd";
static esp_lcd_panel_handle_t s_panel_handle = NULL;

void BSP_LCD_Init(void)
{
    // 背光
#if LCD_BK_LIGHT_GPIO >= 0
    gpio_config_t bk_cfg = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << LCD_BK_LIGHT_GPIO,
    };
    ESP_ERROR_CHECK(gpio_config(&bk_cfg));
    gpio_set_level(LCD_BK_LIGHT_GPIO, LCD_BK_LIGHT_ON_LEVEL);
    ESP_LOGI(TAG, "Backlight on");
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
        ESP_LV_ADAPTER_TEAR_AVOID_MODE_TRIPLE_PARTIAL,
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

    ESP_LOGI(TAG, "LCD init done");
}

esp_lcd_panel_handle_t BSP_LCD_GetPanelHandle(void)
{
    return s_panel_handle;
}

esp_err_t BSP_LCD_GetFrameBuffers(void **fb0, void **fb1, void **fb2)
{
    if (!s_panel_handle) {
        return ESP_ERR_INVALID_STATE;
    }
    return esp_lcd_dpi_panel_get_frame_buffer(s_panel_handle, 3, fb0, fb1, fb2);
}
