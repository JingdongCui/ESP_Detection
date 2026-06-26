idf_component_register(
    SRCS "ethernet_app.c"
    INCLUDE_DIRS "include"
    REQUIRES bsp Sorter_app esp_eth esp_netif esp_event log freertos esp_timer
)

target_compile_definitions(${COMPONENT_LIB} PRIVATE SORTING_SIM_TCP_SEND_IMAGES=0)
