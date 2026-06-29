#include "bsp_sort_sensor.h"

#include "sorter_debug_config.h"
#include "driver/gpio.h"
#include "esp_check.h"
#include "esp_log.h"

#define BSP_SORT_SENSOR_COUNT 4

static const char *TAG = "bsp_sort_sensor";

typedef struct {
    bsp_sort_sensor_id_t id;
    int gpio;
    int active_level;
} bsp_sort_sensor_gpio_config_t;

static const bsp_sort_sensor_gpio_config_t s_sensor_gpio_configs[BSP_SORT_SENSOR_COUNT] = {
    { .id = BSP_SORT_SENSOR_S1, .gpio = SORTER_SENSOR_S1_GPIO, .active_level = SORTER_SENSOR_S1_ACTIVE_LEVEL },
    { .id = BSP_SORT_SENSOR_S2, .gpio = SORTER_SENSOR_S2_GPIO, .active_level = SORTER_SENSOR_S2_ACTIVE_LEVEL },
    { .id = BSP_SORT_SENSOR_S3, .gpio = SORTER_SENSOR_S3_GPIO, .active_level = SORTER_SENSOR_S3_ACTIVE_LEVEL },
    { .id = BSP_SORT_SENSOR_S4, .gpio = SORTER_SENSOR_S4_GPIO, .active_level = SORTER_SENSOR_S4_ACTIVE_LEVEL },
};

static bool s_initialized;

static const bsp_sort_sensor_gpio_config_t *find_sensor_config(bsp_sort_sensor_id_t sensor_id)
{
    for (int i = 0; i < BSP_SORT_SENSOR_COUNT; ++i) {
        if (s_sensor_gpio_configs[i].id == sensor_id) {
            return &s_sensor_gpio_configs[i];
        }
    }
    return NULL;
}

esp_err_t bsp_sort_sensor_init(void)
{
    if (s_initialized) {
        return ESP_OK;
    }

    for (int i = 0; i < BSP_SORT_SENSOR_COUNT; ++i) {
        const bsp_sort_sensor_gpio_config_t *config = &s_sensor_gpio_configs[i];
        if (config->gpio < 0) {
            ESP_LOGW(TAG, "sort sensor S%u GPIO is not configured", (unsigned)config->id);
            continue;
        }
        ESP_RETURN_ON_FALSE(GPIO_IS_VALID_GPIO(config->gpio), ESP_ERR_INVALID_ARG, TAG,
                            "sort sensor S%u GPIO %d is invalid", (unsigned)config->id, config->gpio);

        gpio_config_t io_config = {
            .pin_bit_mask = 1ULL << config->gpio,
            .mode = GPIO_MODE_INPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_ENABLE,
            .intr_type = GPIO_INTR_DISABLE,
        };
        ESP_RETURN_ON_ERROR(gpio_config(&io_config), TAG, "configure sort sensor S%u failed", (unsigned)config->id);
        ESP_LOGI(TAG, "sort sensor S%u configured on GPIO %d, active level %d",
                 (unsigned)config->id, config->gpio, config->active_level);
    }

    s_initialized = true;
    return ESP_OK;
}

esp_err_t bsp_sort_sensor_get_state(bsp_sort_sensor_id_t sensor_id, bool *active)
{
    ESP_RETURN_ON_FALSE(active, ESP_ERR_INVALID_ARG, TAG, "active pointer is NULL");
    ESP_RETURN_ON_FALSE(s_initialized, ESP_ERR_INVALID_STATE, TAG, "sort sensor BSP is not initialized");

    const bsp_sort_sensor_gpio_config_t *config = find_sensor_config(sensor_id);
    ESP_RETURN_ON_FALSE(config, ESP_ERR_INVALID_ARG, TAG, "invalid sort sensor id: %u", (unsigned)sensor_id);

    if (config->gpio < 0) {
        *active = false;
        return ESP_ERR_NOT_FOUND;
    }

    *active = gpio_get_level(config->gpio) == config->active_level;
    return ESP_OK;
}
