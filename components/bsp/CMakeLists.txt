idf_component_register(
    SRCS "bsp_encoder.c" "bsp_lcd.c" "bsp_touch.c" "bsp_lvgl_adapter_init.c" "ethernet_init.c" "bsp_motor.c" "bsp_cam_sensor.c" "bsp_sort_sensor.c"
    INCLUDE_DIRS "include"
    REQUIRES driver esp_lcd esp_hw_support espressif__esp_lcd_ek79007 esp_timer freertos lvgl espressif__esp_lvgl_adapter esp_driver_ppa esp_mm esp_driver_i2c espressif__esp_lcd_touch_gt911 esp_driver_cam esp_driver_isp espressif__esp_sccb_intf esp_eth espressif__esp_video
    PRIV_REQUIRES espressif__esp_cam_sensor espressif__esp_ipa log esp_driver_mcpwm esp_driver_pcnt pid_ctrl esp_driver_ledc
)

target_link_libraries(
    ${COMPONENT_LIB} INTERFACE
    "-u __esp_ipa_detect_fn_esp_ipa_ui_override"
)

target_include_directories(__idf_bsp PRIVATE
    $ENV{IDF_PATH}/components/esp_lcd/dsi/include
)

target_compile_options(__idf_bsp PRIVATE -ffast-math -O3)
