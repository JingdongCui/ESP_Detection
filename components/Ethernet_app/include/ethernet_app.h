#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t ethernet_app_start(void);

// 本机 / 主机 IP（当前为静态配置，上电即固定）。返回以 '\0' 结尾的常量字符串。
const char *ethernet_app_get_local_ip(void);
const char *ethernet_app_get_host_ip(void);

// 指标上报周期（毫秒）。0 表示关闭指标上报。
void ethernet_app_set_metrics_interval_ms(uint32_t ms);

// 图像 / 指标上报总开关。
void ethernet_app_set_report_image_enabled(bool en);
void ethernet_app_set_report_metrics_enabled(bool en);
bool ethernet_app_get_report_image_enabled(void);
bool ethernet_app_get_report_metrics_enabled(void);

#ifdef __cplusplus
}
#endif
