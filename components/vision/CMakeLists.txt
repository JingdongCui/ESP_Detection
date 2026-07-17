idf_component_register(
    SRCS
        "framework/vision_app.c"
        "framework/vision_detect.c"
        "framework/vision_draw.c"
        "framework/vision_stable_frame.c"
        # detector 层：真实 esp-dl 两级级联推理（vision_model.cpp）。
        "detector/vision_model.cpp"
    INCLUDE_DIRS
        "framework"
        "detector"
    REQUIRES
        bsp
        UI
        Sorter_app
        esp_driver_ppa
        esp_lcd
        system_monitor
        esp_timer
        esp_psram
        esp_mm
        esp-dl
        spiffs
        nvs_flash
)

target_compile_options(${COMPONENT_LIB} PRIVATE -ffast-math -O3)
