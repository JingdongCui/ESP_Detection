#include "monitor_internal.h"

#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "sysmon.cpu";

/*
 * 每核 CPU 利用率不依赖可选的 TaskStatus_t.xCoreID 字段做"忙"的计算，
 * 改用每核 IDLE 任务句柄匹配：uxTaskGetSystemState 返回的 pulTotalRunTime
 * 是统一时间基准（单一墙钟时间线），某核 IDLE 在采样窗内的运行时增量
 * / 窗口总增量 = 该核空闲占比。
 *
 * 逐任务占用表复用同一次采样：每个任务的运行时增量 / 窗口总增量 = 该任务
 * 占墙钟时间的比例（双核下所有任务之和可达 200%，属正常）。任务的核心归属
 * 取 TaskStatus_t.xCoreID（需 CONFIG_FREERTOS_VTASKLIST_INCLUDE_COREID）。
 */

static configRUN_TIME_COUNTER_TYPE s_prev_total;
static configRUN_TIME_COUNTER_TYPE s_prev_core_idle[SYSTEM_MONITOR_MAX_CORES];
static bool s_has_prev;

/* 上一轮逐任务运行时快照，按句柄匹配做差。任务动态增减时按句柄查找。 */
typedef struct {
    TaskHandle_t                handle;
    configRUN_TIME_COUNTER_TYPE runtime;
} task_prev_t;

static task_prev_t s_prev_tasks[MONITOR_MAX_TASKS];
static UBaseType_t s_prev_task_count;

static uint8_t busy_percent(configRUN_TIME_COUNTER_TYPE idle_delta,
                            configRUN_TIME_COUNTER_TYPE total_delta)
{
    if (total_delta == 0) {
        return 0;
    }
    configRUN_TIME_COUNTER_TYPE busy =
        (idle_delta >= total_delta) ? 0 : (total_delta - idle_delta);
    uint64_t pct = ((uint64_t)busy * 100ULL) / (uint64_t)total_delta;
    return (pct > 100) ? 100 : (uint8_t)pct;
}

/* 在上一轮快照里按句柄找该任务的运行时计数；找不到（新任务）返回 0。 */
static configRUN_TIME_COUNTER_TYPE prev_runtime_of(TaskHandle_t h)
{
    for (UBaseType_t i = 0; i < s_prev_task_count; i++) {
        if (s_prev_tasks[i].handle == h) {
            return s_prev_tasks[i].runtime;
        }
    }
    return 0;
}

static uint8_t task_percent(configRUN_TIME_COUNTER_TYPE task_delta,
                            configRUN_TIME_COUNTER_TYPE total_delta)
{
    if (total_delta == 0) {
        return 0;
    }
    uint64_t pct = ((uint64_t)task_delta * 100ULL) / (uint64_t)total_delta;
    return (pct > 100) ? 100 : (uint8_t)pct;  /* 单任务最多占满一个核 = 100% */
}

void monitor_cpu_sample(system_monitor_metrics_t *m, monitor_task_table_t *tbl)
{
    if (tbl != NULL) {
        tbl->count = 0;
    }

    int cores = configNUMBER_OF_CORES;
    if (cores > SYSTEM_MONITOR_MAX_CORES) {
        cores = SYSTEM_MONITOR_MAX_CORES;
    }
    m->core_count = (uint8_t)cores;

    /* 取每核 IDLE 任务句柄，用于在任务数组中识别 IDLE */
    TaskHandle_t idle_handle[SYSTEM_MONITOR_MAX_CORES] = {0};
    for (int c = 0; c < cores; c++) {
        idle_handle[c] = xTaskGetIdleTaskHandleForCore(c);
    }

    UBaseType_t num_tasks = uxTaskGetNumberOfTasks();
    m->task_count = (uint16_t)num_tasks;

    TaskStatus_t *tasks = calloc(num_tasks, sizeof(TaskStatus_t));
    if (tasks == NULL) {
        ESP_LOGW(TAG, "no mem for %u TaskStatus_t", (unsigned)num_tasks);
        return;
    }

    configRUN_TIME_COUNTER_TYPE total_runtime = 0;
    UBaseType_t actual = uxTaskGetSystemState(tasks, num_tasks, &total_runtime);

    configRUN_TIME_COUNTER_TYPE core_idle[SYSTEM_MONITOR_MAX_CORES] = {0};
    for (UBaseType_t i = 0; i < actual; i++) {
        for (int c = 0; c < cores; c++) {
            if (tasks[i].xHandle == idle_handle[c]) {
                core_idle[c] += tasks[i].ulRunTimeCounter;
                break;
            }
        }
    }

    configRUN_TIME_COUNTER_TYPE total_delta =
        s_has_prev ? (total_runtime - s_prev_total) : 0;

    /* 填充逐任务表（仅在有上一轮基准、调用方需要时） */
    if (tbl != NULL && s_has_prev) {
        UBaseType_t n = (actual > MONITOR_MAX_TASKS) ? MONITOR_MAX_TASKS : actual;
        for (UBaseType_t i = 0; i < n; i++) {
            monitor_task_stat_t *t = &tbl->tasks[i];

            configRUN_TIME_COUNTER_TYPE delta =
                tasks[i].ulRunTimeCounter - prev_runtime_of(tasks[i].xHandle);
            t->cpu_percent = task_percent(delta, total_delta);

            BaseType_t core = tasks[i].xCoreID;
            t->core_id = (core >= 0 && core < cores) ? (int8_t)core : -1;

            t->stack_free = (uint16_t)tasks[i].usStackHighWaterMark;

            const char *name = tasks[i].pcTaskName ? tasks[i].pcTaskName : "?";
            strncpy(t->name, name, sizeof(t->name) - 1);
            t->name[sizeof(t->name) - 1] = '\0';
        }
        tbl->count = (uint16_t)n;
    }

    /* 保存本轮逐任务快照供下次做差 */
    UBaseType_t saved = (actual > MONITOR_MAX_TASKS) ? MONITOR_MAX_TASKS : actual;
    for (UBaseType_t i = 0; i < saved; i++) {
        s_prev_tasks[i].handle  = tasks[i].xHandle;
        s_prev_tasks[i].runtime = tasks[i].ulRunTimeCounter;
    }
    s_prev_task_count = saved;

    free(tasks);

    if (s_has_prev) {
        uint32_t sum = 0;
        for (int c = 0; c < cores; c++) {
            configRUN_TIME_COUNTER_TYPE idle_d = core_idle[c] - s_prev_core_idle[c];
            uint8_t u = busy_percent(idle_d, total_delta);
            m->cpu_usage_per_core[c] = u;
            sum += u;
        }
        m->cpu_usage_total = (uint8_t)(sum / cores);  /* 整机 = 各核平均 */
    }

    s_prev_total = total_runtime;
    for (int c = 0; c < cores; c++) {
        s_prev_core_idle[c] = core_idle[c];
    }
    s_has_prev = true;
}
