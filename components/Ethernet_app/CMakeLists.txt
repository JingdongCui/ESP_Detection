idf_component_register(
    SRCS "ethernet_app.c"
    INCLUDE_DIRS "include"
    REQUIRES bsp UI Sorter_app system_monitor vision esp_eth esp_netif esp_event log freertos esp_timer esp_new_jpeg
)
