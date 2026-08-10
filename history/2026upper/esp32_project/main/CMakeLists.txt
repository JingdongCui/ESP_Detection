idf_component_register(SRCS "SEGGER_RTT_printf.c" "SEGGER_RTT.c" "main.c" "system_init.c" "system_monitor.c" "ui_realtime_clock.c"
                    INCLUDE_DIRS "."
                    REQUIRES bsp UI vision_app Ethernet_app Sorter_app)

target_link_libraries(${COMPONENT_LIB} INTERFACE "-u __wrap_esp_ipa_detect_get_array")
