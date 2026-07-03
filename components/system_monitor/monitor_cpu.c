#include "monitor_internal.h"

#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_timer.h"

static const char *TAG = "sysmon.cpu";

static int64_t s_prev_cpu_time_us;
static configRUN_TIME_COUNTER_TYPE s_prev_core_idle[SYSTEM_MONITOR_MAX_CORES];
static bool s_has_cpu_prev;

typedef struct {
    TaskHandle_t                handle;
    configRUN_TIME_COUNTER_TYPE runtime;
} task_prev_t;

static task_prev_t s_prev_tasks[MONITOR_MAX_TASKS];
static UBaseType_t s_prev_task_count;
static int64_t s_prev_task_time_us;
static bool s_has_task_prev;

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

static uint8_t task_percent(configRUN_TIME_COUNTER_TYPE task_delta,
                            configRUN_TIME_COUNTER_TYPE total_delta)
{
    if (total_delta == 0) {
        return 0;
    }
    uint64_t pct = ((uint64_t)task_delta * 100ULL) / (uint64_t)total_delta;
    return (pct > 100) ? 100 : (uint8_t)pct;
}

static configRUN_TIME_COUNTER_TYPE prev_runtime_of(TaskHandle_t h)
{
    for (UBaseType_t i = 0; i < s_prev_task_count; i++) {
        if (s_prev_tasks[i].handle == h) {
            return s_prev_tasks[i].runtime;
        }
    }
    return 0;
}

void monitor_cpu_sample(system_monitor_metrics_t *m)
{
    int cores = configNUMBER_OF_CORES;
    if (cores > SYSTEM_MONITOR_MAX_CORES) {
        cores = SYSTEM_MONITOR_MAX_CORES;
    }
    m->core_count = (uint8_t)cores;

    int64_t now_us = esp_timer_get_time();
    configRUN_TIME_COUNTER_TYPE total_delta = s_has_cpu_prev ?
        (configRUN_TIME_COUNTER_TYPE)(now_us - s_prev_cpu_time_us) : 0;

    uint32_t sum = 0;
    for (int c = 0; c < cores; c++) {
        configRUN_TIME_COUNTER_TYPE idle_runtime = ulTaskGetIdleRunTimeCounterForCore(c);
        if (s_has_cpu_prev) {
            configRUN_TIME_COUNTER_TYPE idle_delta = idle_runtime - s_prev_core_idle[c];
            uint8_t usage = busy_percent(idle_delta, total_delta);
            m->cpu_usage_per_core[c] = usage;
            sum += usage;
        }
        s_prev_core_idle[c] = idle_runtime;
    }

    if (s_has_cpu_prev) {
        m->cpu_usage_total = (uint8_t)(sum / cores);
    }

    s_prev_cpu_time_us = now_us;
    s_has_cpu_prev = true;
}

void monitor_task_table_sample(system_monitor_metrics_t *m, monitor_task_table_t *tbl)
{
    if (tbl == NULL) {
        return;
    }
    tbl->count = 0;

    int cores = configNUMBER_OF_CORES;
    if (cores > SYSTEM_MONITOR_MAX_CORES) {
        cores = SYSTEM_MONITOR_MAX_CORES;
    }

    UBaseType_t num_tasks = uxTaskGetNumberOfTasks();
    m->task_count = (uint16_t)num_tasks;

    TaskStatus_t *tasks = calloc(num_tasks, sizeof(TaskStatus_t));
    if (tasks == NULL) {
        ESP_LOGW(TAG, "no mem for %u TaskStatus_t", (unsigned)num_tasks);
        return;
    }

    configRUN_TIME_COUNTER_TYPE ignored_total = 0;
    UBaseType_t actual = uxTaskGetSystemState(tasks, num_tasks, &ignored_total);
    int64_t now_us = esp_timer_get_time();
    configRUN_TIME_COUNTER_TYPE total_delta = s_has_task_prev ?
        (configRUN_TIME_COUNTER_TYPE)(now_us - s_prev_task_time_us) : 0;

    if (s_has_task_prev) {
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

    UBaseType_t saved = (actual > MONITOR_MAX_TASKS) ? MONITOR_MAX_TASKS : actual;
    for (UBaseType_t i = 0; i < saved; i++) {
        s_prev_tasks[i].handle  = tasks[i].xHandle;
        s_prev_tasks[i].runtime = tasks[i].ulRunTimeCounter;
    }
    s_prev_task_count = saved;
    s_prev_task_time_us = now_us;
    s_has_task_prev = true;

    free(tasks);
}
