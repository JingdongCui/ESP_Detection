idf_component_register(
    SRCS "system_monitor.c"
         "monitor_cpu.c"
         "monitor_mem.c"
         "monitor_sys.c"
         "SEGGER_RTT.c"
         "SEGGER_RTT_printf.c"
    INCLUDE_DIRS "include"
    PRIV_INCLUDE_DIRS "."
    REQUIRES esp_common UI bsp
    PRIV_REQUIRES freertos esp_timer esp_hw_support esp_system driver log heap
)
