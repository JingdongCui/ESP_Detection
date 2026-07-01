idf_component_register(
    SRCS "ethernet_app.c"
    INCLUDE_DIRS "include"
    REQUIRES bsp esp_eth esp_netif esp_event log freertos esp_timer
)
