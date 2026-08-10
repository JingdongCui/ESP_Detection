idf_component_register(
    SRCS "ethernet_app.cpp"
    INCLUDE_DIRS "include"
    REQUIRES bsp UI vision_app Sorter_app esp_eth esp_netif esp_event log freertos esp_timer esp_new_jpeg
)

target_compile_options(${COMPONENT_LIB} PRIVATE -ffast-math -O3)
