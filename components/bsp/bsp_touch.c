#include "esp_log.h"
#include "driver/i2c_master.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_touch_gt911.h"
#include "bsp_touch.h"

#define TOUCH_I2C_PORT      I2C_NUM_0
#define TOUCH_I2C_SCL_GPIO  8
#define TOUCH_I2C_SDA_GPIO  7
#define TOUCH_RST_GPIO      33
#define TOUCH_INT_GPIO      21   // 原为 32，与 Motor1(B段) PWM_A 冲突，改到 J1 空闲脚 GPIO21

#define TOUCH_H_RES         1024
#define TOUCH_V_RES         600

static const char *TAG = "bsp_touch";
static esp_lcd_touch_handle_t s_touch_handle = NULL;
static i2c_master_bus_handle_t s_i2c_bus = NULL;

static esp_err_t bsp_touch_try_init_at_addr(uint8_t addr)
{
    esp_lcd_panel_io_handle_t tp_io = NULL;
    esp_lcd_panel_io_i2c_config_t tp_io_cfg = ESP_LCD_TOUCH_IO_I2C_GT911_CONFIG();
    tp_io_cfg.dev_addr = addr;

    esp_err_t ret = esp_lcd_new_panel_io_i2c(s_i2c_bus, &tp_io_cfg, &tp_io);
    if (ret != ESP_OK) {
        ESP_LOGW(TAG, "Create GT911 IO at 0x%02X failed: %s", addr, esp_err_to_name(ret));
        return ret;
    }

    esp_lcd_touch_io_gt911_config_t gt911_cfg = {
        .dev_addr = addr,
    };
    esp_lcd_touch_config_t tp_cfg = {
        .x_max = TOUCH_H_RES,
        .y_max = TOUCH_V_RES,
        .rst_gpio_num = TOUCH_RST_GPIO,
        .int_gpio_num = TOUCH_INT_GPIO,
        .levels = {
            .reset = 0,
            .interrupt = 0,
        },
        .flags = {
            .swap_xy = 0,
            .mirror_x = 1,
            .mirror_y = 1,
        },
        .driver_data = &gt911_cfg,
    };

    ret = esp_lcd_touch_new_i2c_gt911(tp_io, &tp_cfg, &s_touch_handle);
    if (ret != ESP_OK) {
        ESP_LOGW(TAG, "GT911 init at 0x%02X failed: %s", addr, esp_err_to_name(ret));
        esp_lcd_panel_io_del(tp_io);
        s_touch_handle = NULL;
        return ret;
    }

    ESP_LOGI(TAG, "GT911 detected at I2C address 0x%02X", addr);
    return ESP_OK;
}

void BSP_Touch_Init(void)
{
    i2c_master_bus_config_t bus_cfg = {
        .i2c_port = TOUCH_I2C_PORT,
        .scl_io_num = TOUCH_I2C_SCL_GPIO,
        .sda_io_num = TOUCH_I2C_SDA_GPIO,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };
    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_cfg, &s_i2c_bus));
    ESP_LOGI(TAG, "I2C bus initialized");

    esp_err_t ret = bsp_touch_try_init_at_addr(ESP_LCD_TOUCH_IO_I2C_GT911_ADDRESS);
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG, "Touch init done");
}

esp_lcd_touch_handle_t BSP_Touch_GetHandle(void)
{
    return s_touch_handle;
}

i2c_master_bus_handle_t BSP_Touch_GetI2CBus(void)
{
    return s_i2c_bus;
}
