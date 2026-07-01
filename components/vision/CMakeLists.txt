idf_component_register(
    SRCS
        "framework/vision_app.c"
        "framework/vision_detect.c"
        "framework/vision_draw.c"
        # detector 层：真实 esp-dl 两级级联推理（vision_model.cpp）+ ROI 调优。
        "detector/vision_model.cpp"
        "detector/roi_tuning.c"
    INCLUDE_DIRS
        "framework"
        "detector"
    REQUIRES
        bsp
        UI
        esp_driver_ppa
        esp_lcd
        system_monitor
        esp_timer
        esp_psram
        esp_mm
        esp-dl
        spiffs
)

target_compile_options(${COMPONENT_LIB} PRIVATE -ffast-math -O3)
