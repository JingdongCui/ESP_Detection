idf_component_register(
    SRCS "sorter_core/sorter_scheduler.c" "sorting_sim_control.c"
    INCLUDE_DIRS "include"
    REQUIRES bsp log freertos esp_timer esp_driver_usb_serial_jtag
)
