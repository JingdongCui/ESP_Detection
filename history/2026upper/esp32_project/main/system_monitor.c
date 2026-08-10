#include "system_monitor.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_freertos_hooks.h"
#include "esp_heap_caps.h"
#include "SEGGER_RTT.h"
#include "soc/cache_reg.h"
#include <string.h>
#include <stdlib.h>

/* ─── PSRAM 带宽监控配置 ─── */
#define MAX_TRACKED_TASKS   16
#define PSRAM_REPORT_INTERVAL_MS  2000
#define L2_CACHE_LINE_SIZE  128
#define MONITOR_STACK_SIZE  4096

#define LCD_WIDTH           1024
#define LCD_HEIGHT          600
#define LCD_BPP             3
#define LCD_FPS             60
#define CAM_WIDTH           1024
#define CAM_HEIGHT          600
#define CAM_BPP             3
#define CAM_FPS             30

static const uint32_t DMA_LCD_BW = LCD_WIDTH * LCD_HEIGHT * LCD_BPP * LCD_FPS;
static const uint32_t DMA_CAM_BW = CAM_WIDTH * CAM_HEIGHT * CAM_BPP * CAM_FPS;

/* ─── PSRAM 带宽内部数据 ─── */
typedef struct {
    TaskHandle_t handle;
    const char  *name;
    uint32_t     miss_accum;
} task_entry_t;

static task_entry_t s_tasks[MAX_TRACKED_TASKS];
static uint8_t      s_task_count;
static uint32_t     s_last_cnt[2];
static portMUX_TYPE s_spinlock = portMUX_INITIALIZER_UNLOCKED;
static uint32_t     s_sample_ticks;

/* 报告缓存 */
static uint8_t  s_rpt_task_count;
static uint32_t s_rpt_total_bw;
static struct {
    const char *name;
    uint32_t    misses;
    uint32_t    bw;
    uint8_t     pct;
} s_rpt_tasks[MAX_TRACKED_TASKS];
/* ─── PLACEHOLDER_CONTINUE ─── */

static task_entry_t *find_or_add_task(TaskHandle_t h)
{
    for (int i = 0; i < s_task_count; i++) {
        if (s_tasks[i].handle == h) return &s_tasks[i];
    }
    if (s_task_count >= MAX_TRACKED_TASKS) return NULL;
    task_entry_t *e = &s_tasks[s_task_count++];
    e->handle = h;
    e->name = pcTaskGetName(h);
    e->miss_accum = 0;
    return e;
}

static void IRAM_ATTR tick_hook_cpu0(void)
{
    uint32_t cnt = REG_READ(CACHE_L2_DBUS0_ACS_NXTLVL_RD_CNT_REG);
    uint32_t delta = cnt - s_last_cnt[0];
    s_last_cnt[0] = cnt;

    TaskHandle_t cur = xTaskGetCurrentTaskHandle();
    portENTER_CRITICAL_ISR(&s_spinlock);
    task_entry_t *e = find_or_add_task(cur);
    if (e) e->miss_accum += delta;
    s_sample_ticks++;
    portEXIT_CRITICAL_ISR(&s_spinlock);
}

static void IRAM_ATTR tick_hook_cpu1(void)
{
    uint32_t cnt = REG_READ(CACHE_L2_DBUS1_ACS_NXTLVL_RD_CNT_REG);
    uint32_t delta = cnt - s_last_cnt[1];
    s_last_cnt[1] = cnt;

    TaskHandle_t cur = xTaskGetCurrentTaskHandle();
    portENTER_CRITICAL_ISR(&s_spinlock);
    task_entry_t *e = find_or_add_task(cur);
    if (e) e->miss_accum += delta;
    portEXIT_CRITICAL_ISR(&s_spinlock);
}

static void compute_report(void)
{
    uint32_t ticks;
    uint32_t misses[MAX_TRACKED_TASKS];
    uint8_t count;
    const char *names[MAX_TRACKED_TASKS];

    portENTER_CRITICAL(&s_spinlock);
    ticks = s_sample_ticks;
    count = s_task_count;
    for (int i = 0; i < count; i++) {
        misses[i] = s_tasks[i].miss_accum;
        names[i] = s_tasks[i].name;
        s_tasks[i].miss_accum = 0;
    }
    s_sample_ticks = 0;
    portEXIT_CRITICAL(&s_spinlock);

    if (ticks == 0) return;

    float elapsed_sec = (float)ticks / (float)configTICK_RATE_HZ;
    uint32_t total_misses = 0;
    for (int i = 0; i < count; i++) total_misses += misses[i];
/* ─── PLACEHOLDER_CONTINUE2 ─── */

    uint32_t total_cpu_bw = (elapsed_sec > 0.01f) ?
        (uint32_t)((double)total_misses * L2_CACHE_LINE_SIZE / (double)elapsed_sec) : 0;
    uint32_t total_bw = total_cpu_bw + DMA_LCD_BW + DMA_CAM_BW;

    s_rpt_task_count = count;
    s_rpt_total_bw = total_bw;

    for (int i = 0; i < count; i++) {
        s_rpt_tasks[i].name = names[i];
        s_rpt_tasks[i].misses = misses[i];
        s_rpt_tasks[i].bw = (elapsed_sec > 0.01f) ?
            (uint32_t)((double)misses[i] * L2_CACHE_LINE_SIZE / (double)elapsed_sec) : 0;
        s_rpt_tasks[i].pct = total_bw ? (uint8_t)((uint64_t)s_rpt_tasks[i].bw * 100 / total_bw) : 0;
    }
}

static void monitor_task(void *arg)
{
    int cycle = 0;
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(PSRAM_REPORT_INTERVAL_MS));
        compute_report();
        system_monitor_print_psram();

        cycle++;
        if (cycle % 3 == 0) {
            system_monitor_print_memory();
            system_monitor_print_tasks();
        }
    }
}

/* ═══════════════════════════════════════════
 *  公开 API
 * ═══════════════════════════════════════════ */

esp_err_t system_monitor_init(void)
{
    /* 使能 L2 Cache 数据总线计数器
     * NXTLVL_RD_CNT 记录 L2 真正向 PSRAM 发起读取的次数（cache line 粒度 = 128B）
     * 这是实际物理 PSRAM 流量，不是 L1 miss */
    REG_SET_BIT(CACHE_L2_CACHE_ACS_CNT_CTRL_REG, CACHE_L2_DBUS0_CNT_ENA);
    REG_SET_BIT(CACHE_L2_CACHE_ACS_CNT_CTRL_REG, CACHE_L2_DBUS1_CNT_ENA);

    s_last_cnt[0] = REG_READ(CACHE_L2_DBUS0_ACS_NXTLVL_RD_CNT_REG);
    s_last_cnt[1] = REG_READ(CACHE_L2_DBUS1_ACS_NXTLVL_RD_CNT_REG);

    esp_register_freertos_tick_hook_for_cpu(tick_hook_cpu0, 0);
    esp_register_freertos_tick_hook_for_cpu(tick_hook_cpu1, 1);

    xTaskCreatePinnedToCore(monitor_task, "sys_mon", MONITOR_STACK_SIZE,
                            NULL, 1, NULL, 0);
    return ESP_OK;
}

void system_monitor_print_memory(void)
{
    SEGGER_RTT_printf(0, "\n=== Memory Stats ===\n");
    SEGGER_RTT_printf(0, "Free heap: %u bytes\n", esp_get_free_heap_size());
    SEGGER_RTT_printf(0, "Min free ever: %u bytes\n", esp_get_minimum_free_heap_size());
    SEGGER_RTT_printf(0, "Free internal: %u bytes\n",
        heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
    SEGGER_RTT_printf(0, "Free PSRAM: %u bytes\n",
        heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
    SEGGER_RTT_printf(0, "Largest free block: %u bytes\n",
        heap_caps_get_largest_free_block(MALLOC_CAP_DEFAULT));
}

void system_monitor_print_tasks(void)
{
    UBaseType_t num_tasks = uxTaskGetNumberOfTasks();
    TaskStatus_t *task_array = malloc(num_tasks * sizeof(TaskStatus_t));
    if (!task_array) return;

    configRUN_TIME_COUNTER_TYPE total_runtime;
    UBaseType_t actual = uxTaskGetSystemState(task_array, num_tasks, &total_runtime);

    SEGGER_RTT_printf(0, "\n=== Task Stack & CPU ===\n");
    SEGGER_RTT_printf(0, "%-16s %4s %6s %10s %5s\n", "Task", "Prio", "Stack", "Ticks", "CPU%");
    SEGGER_RTT_printf(0, "-------------------------------------------------------\n");

    for (UBaseType_t i = 0; i < actual; i++) {
        uint32_t pct = 0;
        if (total_runtime > 0) {
            pct = (uint32_t)((task_array[i].ulRunTimeCounter * 100ULL) / total_runtime);
        }
        SEGGER_RTT_printf(0, "%-16s %4u %5u  %10u  %3u%%\n",
            task_array[i].pcTaskName,
            (unsigned)task_array[i].uxCurrentPriority,
            (unsigned)task_array[i].usStackHighWaterMark,
            (unsigned)(task_array[i].ulRunTimeCounter & 0xFFFFFFFF),
            pct);
    }

    free(task_array);
}

void system_monitor_print_psram(void)
{
    if (s_rpt_task_count == 0) return;

    SEGGER_RTT_printf(0, "\n=== PSRAM Bandwidth Report ===\n");
    SEGGER_RTT_printf(0, "Total: %u KB/s (CPU: %u + LCD_DMA: %u + CAM_DMA: %u)\n",
        s_rpt_total_bw / 1024,
        (s_rpt_total_bw - DMA_LCD_BW - DMA_CAM_BW) / 1024,
        DMA_LCD_BW / 1024, DMA_CAM_BW / 1024);
    SEGGER_RTT_printf(0, "%-16s %10s %8s %5s\n", "Task", "Misses", "KB/s", "%%");
    SEGGER_RTT_printf(0, "----------------------------------------------\n");

    for (int i = 0; i < s_rpt_task_count; i++) {
        if (s_rpt_tasks[i].misses == 0) continue;
        SEGGER_RTT_printf(0, "%-16s %10u %8u %4u%%\n",
            s_rpt_tasks[i].name, s_rpt_tasks[i].misses,
            s_rpt_tasks[i].bw / 1024, s_rpt_tasks[i].pct);
    }
    SEGGER_RTT_printf(0, "=== End Report ===\n");
}
