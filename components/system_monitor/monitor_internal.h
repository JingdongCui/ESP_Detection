#pragma once

#include "system_monitor.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * 组件内部接口：三个采集器各自负责填充快照的一部分。
 * 由顶层 system_monitor.c 的采集任务按周期统一调用，外部不应直接使用。
 */

/* 逐任务 CPU 占用表：仅用于 RTT 调试打印，不进 system_monitor_metrics_t。 */
#define MONITOR_MAX_TASKS 48

typedef struct {
    char     name[16];     /* 任务名（configMAX_TASK_NAME_LEN 通常 16） */
    uint8_t  cpu_percent;  /* 本采样窗口内该任务 CPU 占用 %（0-100） */
    int8_t   core_id;      /* 所在核心；-1 表示无亲和（双核轮流），打印为 * */
    uint16_t stack_free;   /* 栈高水位剩余（words，越小越接近溢出） */
} monitor_task_stat_t;

typedef struct {
    monitor_task_stat_t tasks[MONITOR_MAX_TASKS];
    uint16_t count;        /* 实际填充的任务数（<= MONITOR_MAX_TASKS） */
} monitor_task_table_t;

/* CPU 类：整机/每核利用率、核心数、任务数。
 * 依赖两次采样差值，内部维护跨调用静态状态，需周期性调用才有意义。
 *
 * tbl 可为 NULL；非 NULL 时复用同一次 uxTaskGetSystemState 采样，
 * 用相同的时间窗口基准填充逐任务占用表，保证与每核占用数值自洽。 */
void monitor_cpu_sample(system_monitor_metrics_t *m, monitor_task_table_t *tbl);

/* 内存类：空闲堆、历史最小堆、使用率、内部 RAM、PSRAM、最大连续块。无状态。 */
void monitor_mem_sample(system_monitor_metrics_t *m);

/* 系统类：uptime、CPU 频率、芯片温度。无状态采样。 */
void monitor_sys_sample(system_monitor_metrics_t *m);

/* 系统类一次性初始化（温度传感器 install + enable）。失败不影响其他指标。 */
void monitor_sys_init(void);

/* 系统类资源释放（温度传感器 disable + uninstall）。 */
void monitor_sys_deinit(void);

#ifdef __cplusplus
}
#endif
