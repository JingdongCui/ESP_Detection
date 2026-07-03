#include "system_monitor.h"
#include "monitor_internal.h"

#include <string.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "bsp_lvgl_adapter_init.h"
#include "SEGGER_RTT.h"
#include "sdk.h"

// 采集周期：
#define MONITOR_PERIOD_MS    250
#define MONITOR_TASK_STACK   4096
#define MONITOR_TASK_PRIO    3   /* 低于业务任务，避免抢占 */
#define MONITOR_TASK_TABLE_PERIOD_MS 5000
#define MONITOR_TASK_TABLE_INTERVAL \
    ((MONITOR_TASK_TABLE_PERIOD_MS + MONITOR_PERIOD_MS - 1) / MONITOR_PERIOD_MS)

static TaskHandle_t        s_task;

static SemaphoreHandle_t   s_lock;          /* 保护 s_metrics 的读写 */
static system_monitor_metrics_t s_metrics;  /* 最近一次快照 */

static uint32_t bytes_to_kb(uint32_t bytes)
{
    return bytes / 1024U;
}

static uint8_t psram_usage_percent(uint32_t free_psram, uint32_t total_psram)
{
    if (total_psram == 0) {
        return 0;
    }
    return (uint8_t)(((uint64_t)(total_psram - free_psram) * 100ULL) / total_psram);
}

static int temperature_c_x10(float temperature_c)
{
    if (temperature_c != temperature_c) {
        return 0;
    }
    return (int)(temperature_c * 10.0f);
}

static void format_core(char core[4], int8_t core_id)
{
    if (core_id < 0) {
        core[0] = '*';
        core[1] = '\0';
        return;
    }
    core[0] = 'c';
    core[1] = (char)('0' + core_id);
    core[2] = '\0';
}

static void post_system_ui(const system_monitor_metrics_t *m)
{
    system_monitor_event_data_t data = {
        .cpu1_usage = m->cpu_usage_per_core[0],
        .cpu2_usage = m->cpu_usage_per_core[1],
        .cpu_total_usage = m->cpu_usage_total,
        .memory_usage = m->heap_usage_percent,
        .heap_free_kb = bytes_to_kb(m->free_heap),
        .heap_min_free_kb = bytes_to_kb(m->min_free_heap),
        .heap_max_block_kb = bytes_to_kb(m->largest_free_block),
        .internal_free_kb = bytes_to_kb(m->free_internal),
        .psram_usage = psram_usage_percent(m->free_psram, m->total_psram),
        .psram_free_kb = bytes_to_kb(m->free_psram),
        .psram_total_kb = bytes_to_kb(m->total_psram),
        .ram_usage = m->heap_usage_percent,
        .runtime_sec = (int)(m->uptime_ms / 1000ULL),
        .chip_temp = temperature_c_x10(m->temperature_c),
        .cpu_freq_mhz = (int)m->cpu_freq_mhz,
        .task_count = m->task_count,
    };

    BSP_LVGL_Lock();
    send_event(get_current_event_table(), EVT_SYSTEM_MONITOR, EVT_SYSTEM_MONITOR_CHANGED,
               (uint8_t *)&data, 0);
    BSP_LVGL_Unlock();
}

static void print_monitor_rtt(const system_monitor_metrics_t *m, monitor_task_table_t *tbl,
                              bool print_task_table)
{
    /* 一行汇总，便于在 rtt.log 中观察趋势 */
    SEGGER_RTT_printf(0,
        "[SYSMON #%u] CPU %u%% (c0 %u%% c1 %u%%) tasks=%u | "
        "heap free=%u min=%u use=%u%% frag_blk=%u | "
        "int=%u psram %u/%u | up=%us freq=%uMHz temp=",
        (unsigned)m->seq,
        (unsigned)m->cpu_usage_total,
        (unsigned)m->cpu_usage_per_core[0],
        (unsigned)m->cpu_usage_per_core[1],
        (unsigned)m->task_count,
        (unsigned)m->free_heap,
        (unsigned)m->min_free_heap,
        (unsigned)m->heap_usage_percent,
        (unsigned)m->largest_free_block,
        (unsigned)m->free_internal,
        (unsigned)m->free_psram,
        (unsigned)m->total_psram,
        (unsigned)(m->uptime_ms / 1000),
        (unsigned)m->cpu_freq_mhz);

    /* SEGGER_RTT_printf 不支持 %f，温度单独整数化输出（保留一位小数） */
    if (m->temperature_c == m->temperature_c) { /* 非 NAN */
        int t10 = (int)(m->temperature_c * 10.0f);
        SEGGER_RTT_printf(0, "%d.%dC\n", t10 / 10, (t10 < 0 ? -t10 : t10) % 10);
    } else {
        SEGGER_RTT_printf(0, "N/A\n");
    }

    if (!print_task_table || tbl->count == 0) {
        return;
    }

    for (uint16_t i = 1; i < tbl->count; i++) {
        monitor_task_stat_t key = tbl->tasks[i];
        int j = (int)i - 1;
        while (j >= 0 && tbl->tasks[j].cpu_percent < key.cpu_percent) {
            tbl->tasks[j + 1] = tbl->tasks[j];
            j--;
        }
        tbl->tasks[j + 1] = key;
    }

    char line[64];
    snprintf(line, sizeof(line), "  %-16s %-4s %4s %5s\n",
             "TASK", "CORE", "CPU%", "STK");
    SEGGER_RTT_WriteString(0, line);

    for (uint16_t i = 0; i < tbl->count; i++) {
        const monitor_task_stat_t *t = &tbl->tasks[i];
        char core[4];
        format_core(core, t->core_id);
        snprintf(line, sizeof(line), "  %-16s %-4s %3u%% %5u\n",
                 t->name, core,
                 (unsigned)t->cpu_percent,
                 (unsigned)t->stack_free);
        SEGGER_RTT_WriteString(0, line);
    }
}

static void monitor_task(void *arg)
{
    (void)arg;
    monitor_sys_init();

    system_monitor_metrics_t local;
    memset(&local, 0, sizeof(local));

    static monitor_task_table_t task_tbl;
    bool print_task_table = false;

    TickType_t last = xTaskGetTickCount();
    for (;;) {
        local.seq++;
        monitor_cpu_sample(&local);
        monitor_mem_sample(&local);
        monitor_sys_sample(&local);

        if ((local.seq % MONITOR_TASK_TABLE_INTERVAL) == 0) {
            monitor_task_table_sample(&local, &task_tbl);
            print_task_table = true;
        }

        /* 发布到共享快照 */
        if (xSemaphoreTake(s_lock, portMAX_DELAY) == pdTRUE) {
            s_metrics = local;
            xSemaphoreGive(s_lock);
        }

        post_system_ui(&local);
        print_monitor_rtt(&local, &task_tbl, print_task_table);
        print_task_table = false;

        vTaskDelayUntil(&last, pdMS_TO_TICKS(MONITOR_PERIOD_MS));
    }
}

esp_err_t system_monitor(void)
{
    if (s_task != NULL) {
        return ESP_OK;  /* 幂等：已启动 */
    }

    if (s_lock == NULL) {
        s_lock = xSemaphoreCreateMutex();
        if (s_lock == NULL) {
            return ESP_ERR_NO_MEM;
        }
    }

    BaseType_t ok = xTaskCreate(monitor_task, "sysmon",
                                MONITOR_TASK_STACK, NULL,
                                MONITOR_TASK_PRIO, &s_task);
    if (ok != pdPASS) {
        s_task = NULL;
        return ESP_FAIL;
    }
    return ESP_OK;
}

void system_monitor_stop(void)
{
    if (s_task) {
        vTaskDelete(s_task);
        s_task = NULL;
    }
    monitor_sys_deinit();
}

esp_err_t system_monitor_get_metrics(system_monitor_metrics_t *out)
{
    if (out == NULL) {
        return ESP_ERR_INVALID_ARG;
    }
    if (s_lock == NULL || s_task == NULL) {
        return ESP_ERR_INVALID_STATE;
    }
    if (xSemaphoreTake(s_lock, portMAX_DELAY) == pdTRUE) {
        *out = s_metrics;
        xSemaphoreGive(s_lock);
        return ESP_OK;
    }
    return ESP_FAIL;
}
