#pragma once

#include "esp_err.h"

/**
 * 初始化系统监控（启动 PSRAM 带宽采样的 tick hook 和后台任务）
 * 在 app_main 中调用一次
 */
esp_err_t system_monitor_init(void);

/**
 * 打印堆内存统计（RTT 输出）
 */
void system_monitor_print_memory(void);

/**
 * 打印所有 RTOS 任务的堆栈水位和 CPU 占用（RTT 输出）
 */
void system_monitor_print_tasks(void);

/**
 * 打印 PSRAM 带宽报告（RTT 输出）
 * 注意：此函数由内部后台任务周期调用，也可手动调用获取即时报告
 */
void system_monitor_print_psram(void);
