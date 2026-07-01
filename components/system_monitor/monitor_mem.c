#include "monitor_internal.h"

#include "esp_heap_caps.h"

void monitor_mem_sample(system_monitor_metrics_t *m)
{
    /* 默认堆：当前空闲、历史最小、总量（用于使用率） */
    size_t free_default  = heap_caps_get_free_size(MALLOC_CAP_DEFAULT);
    size_t total_default = heap_caps_get_total_size(MALLOC_CAP_DEFAULT);

    m->free_heap          = (uint32_t)free_default;
    m->min_free_heap      = (uint32_t)heap_caps_get_minimum_free_size(MALLOC_CAP_DEFAULT);
    m->largest_free_block = (uint32_t)heap_caps_get_largest_free_block(MALLOC_CAP_DEFAULT);

    if (total_default > 0) {
        uint64_t used = (uint64_t)(total_default - free_default);
        uint64_t pct = (used * 100ULL) / (uint64_t)total_default;
        m->heap_usage_percent = (pct > 100) ? 100 : (uint8_t)pct;
    } else {
        m->heap_usage_percent = 0;
    }

    /* 内部 RAM 空闲 */
    m->free_internal = (uint32_t)heap_caps_get_free_size(MALLOC_CAP_INTERNAL);

    /* PSRAM 空闲 / 总量（无 PSRAM 时返回 0） */
    m->free_psram  = (uint32_t)heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
    m->total_psram = (uint32_t)heap_caps_get_total_size(MALLOC_CAP_SPIRAM);
}
