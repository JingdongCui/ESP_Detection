idf_component_register(
    SRCS
        "vision_app.cpp"
        "vision_preview.cpp"
        "app_yolo.cpp"
        "yolo_decode.cpp"
        "roi_tuning.cpp"
        "roi_algorithm.c"
    INCLUDE_DIRS "include"
    REQUIRES
        UI
        bsp
        Sorter_app
        esp-dl
        esp_timer
        spiffs
        esp_psram
        esp_driver_ppa
        esp_mm
)

target_compile_options(${COMPONENT_LIB} PRIVATE -ffast-math -O3)
