#include "monitor_internal.h"

#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_timer.h"

static const char *TAG = "sysmon.cpu";

/* CPU 占用采样：用 ~1s 长窗口差分 + EMA 平滑。
 * idle runtime counter 仅在上下文切换时结算（tasks.c:3686），250ms 短窗口在低切换
 * 核（如空闲的 c0）上易抓到「IDLE 连续运行、尚未结算」的尾巴，导致 idle_delta 忽 0
 * 忽满 → usage 在 0/100 间乱跳。拉长到 1s 让分母覆盖足够多次结算，天然平滑；EMA 再
 * 压制跨秒残余抖动。分子分母同为 esp_timer 微秒时基（RUN_TIME_STATS_USING_ESP_TIMER）。 */
#define CPU_SAMPLE_WINDOW_US   1000000

static int64_t s_window_start_us;
static configRUN_TIME_COUNTER_TYPE s_window_core_idle[SYSTEM_MONITOR_MAX_CORES];
static bool s_has_window;
static uint8_t s_ema_usage[SYSTEM_MONITOR_MAX_CORES];
static bool s_has_ema;

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

    /* 首次调用：只锚定窗口基准，usage 保留调用方（memset 后为 0）的既有值 */
    if (!s_has_window) {
        for (int c = 0; c < cores; c++) {
            s_window_core_idle[c] = ulTaskGetIdleRunTimeCounterForCore(c);
        }
        s_window_start_us = now_us;
        s_has_window = true;
        return;
    }

    /* 窗口未满 1s：沿用上次算出的 usage（调用方的 local 快照持久保存该字段） */
    int64_t total_delta = now_us - s_window_start_us;
    if (total_delta < CPU_SAMPLE_WINDOW_US) {
        return;
    }

    uint32_t sum = 0;
    for (int c = 0; c < cores; c++) {
        configRUN_TIME_COUNTER_TYPE idle_now = ulTaskGetIdleRunTimeCounterForCore(c);
        configRUN_TIME_COUNTER_TYPE idle_delta = idle_now - s_window_core_idle[c];
        uint8_t raw = busy_percent(idle_delta, (configRUN_TIME_COUNTER_TYPE)total_delta);

        /* EMA α=0.5：ema = round((ema + raw) / 2)，兼顾平滑与响应 */
        uint8_t usage = s_has_ema ?
            (uint8_t)(((uint32_t)s_ema_usage[c] + raw + 1) / 2) : raw;
        s_ema_usage[c] = usage;

        m->cpu_usage_per_core[c] = usage;
        sum += usage;
        s_window_core_idle[c] = idle_now;
    }
    s_has_ema = true;
    m->cpu_usage_total = (uint8_t)(sum / cores);
    s_window_start_us = now_us;
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
