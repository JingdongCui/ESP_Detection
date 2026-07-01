#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 单核 CPU 利用率上限：覆盖 ESP32-P4 双核，留 1 个余量便于扩展 */
#define SYSTEM_MONITOR_MAX_CORES 2

/**
 * 系统监视指标快照。
 *
 * 由后台采集任务周期性填充，外部通过 system_monitor_get_metrics() 读取。
 * 仅包含适合上层展示的指标（CPU / 内存 / 系统三类），不含崩溃诊断信息。
 */
typedef struct {
    /* --- CPU --- */
    uint8_t  cpu_usage_total;                       /* 整机 CPU 利用率 %（0-100） */
    uint8_t  cpu_usage_per_core[SYSTEM_MONITOR_MAX_CORES]; /* 每核 CPU 利用率 % */
    uint8_t  core_count;                            /* 实际核心数 */
    uint16_t task_count;                            /* 当前任务总数 */

    /* --- 内存（字节） --- */
    uint32_t free_heap;                             /* 当前空闲堆 */
    uint32_t min_free_heap;                         /* 历史最小空闲堆（泄漏判断） */
    uint8_t  heap_usage_percent;                    /* 默认堆使用率 %（0-100） */
    uint32_t free_internal;                         /* 内部 RAM 空闲 */
    uint32_t free_psram;                            /* PSRAM 空闲 */
    uint32_t total_psram;                           /* PSRAM 总量 */
    uint32_t largest_free_block;                    /* 最大连续可分配块（碎片化指标） */

    /* --- 系统 --- */
    uint64_t uptime_ms;                             /* 自启动运行时间（毫秒） */
    float    temperature_c;                         /* 芯片温度（°C），无效时为 NAN */
    uint32_t cpu_freq_mhz;                          /* 当前 CPU 主频（MHz） */

    uint32_t seq;                                   /* 采样序号，每次采集自增 */
} system_monitor_metrics_t;

/**
 * 启动系统监视。
 *
 * 创建一个后台 FreeRTOS 任务，按固定周期采集所有指标并写入内部快照，
 * 同时通过 RTT 打印一行汇总。重复调用为幂等（已启动则直接返回 ESP_OK）。
 *
 * @return ESP_OK 成功；其他为失败（任务/资源创建失败）
 */
esp_err_t system_monitor(void);

/**
 * 停止系统监视并释放资源。主要用于测试，正常运行无需调用。
 */
void system_monitor_stop(void);

/**
 * 读取最近一次采集的指标快照（线程安全，拷贝输出）。
 *
 * @param out  输出缓冲区，不可为 NULL
 * @return ESP_OK 成功；ESP_ERR_INVALID_ARG 参数为空；ESP_ERR_INVALID_STATE 尚未启动
 */
esp_err_t system_monitor_get_metrics(system_monitor_metrics_t *out);

#ifdef __cplusplus
}
#endif
