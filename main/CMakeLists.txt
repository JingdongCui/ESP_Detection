idf_component_register(SRCS "main.c" "system_init.c"
                    INCLUDE_DIRS "."
                    REQUIRES bsp UI Ethernet_app Sorter_app system_monitor vision)
