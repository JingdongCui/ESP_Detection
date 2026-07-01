#include "monitor_internal.h"

#include <math.h>
#include "esp_timer.h"
#include "esp_log.h"
#include "esp_private/esp_clk.h"
#include "driver/temperature_sensor.h"

static const char *TAG = "sysmon.sys";

/* 温度传感器句柄：install 成功后持有，采样期间常驻 enable 状态 */
static temperature_sensor_handle_t s_temp_handle;
static bool s_temp_ready;

void monitor_sys_init(void)
{
    /* P4 量程必须落在单个预定义档位内；-10~80°C 档误差最小，覆盖常见工作温区 */
    temperature_sensor_config_t cfg = TEMPERATURE_SENSOR_CONFIG_DEFAULT(-10, 80);
    esp_err_t err = temperature_sensor_install(&cfg, &s_temp_handle);
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "temp sensor install failed: %s", esp_err_to_name(err));
        s_temp_handle = NULL;
        return;
    }
    err = temperature_sensor_enable(s_temp_handle);
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "temp sensor enable failed: %s", esp_err_to_name(err));
        temperature_sensor_uninstall(s_temp_handle);
        s_temp_handle = NULL;
        return;
    }
    s_temp_ready = true;
}

void monitor_sys_deinit(void)
{
    if (s_temp_handle) {
        if (s_temp_ready) {
            temperature_sensor_disable(s_temp_handle);
        }
        temperature_sensor_uninstall(s_temp_handle);
        s_temp_handle = NULL;
        s_temp_ready = false;
    }
}

void monitor_sys_sample(system_monitor_metrics_t *m)
{
    m->uptime_ms = (uint64_t)(esp_timer_get_time() / 1000);
    m->cpu_freq_mhz = (uint32_t)(esp_clk_cpu_freq() / 1000000);

    if (s_temp_ready) {
        float c = NAN;
        if (temperature_sensor_get_celsius(s_temp_handle, &c) == ESP_OK) {
            m->temperature_c = c;
        } else {
            m->temperature_c = NAN;
        }
    } else {
        m->temperature_c = NAN;
    }
}
