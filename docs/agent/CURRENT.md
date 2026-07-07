# Current Task

## Goal

按用户要求把 `ESP32P4_Detection` 视觉 miss 保持次数从 3 调到 2。

## Current State

- 活跃 ESP 工程：`/home/kazeform/2026esp/ESP32P4_Detection`
- 分支：`feat/screen-uvc-stream`
- 当前板子串口：
  - `/dev/serial/by-id/usb-Espressif_USB_JTAG_serial_debug_unit_E8:F6:0A:E1:7A:D1-if00`
- 当前上位机进程：
  - `/home/kazeform/2026esp/esp32_host_no_inference/build/Desktop_Debug/bin/esp32_host_no_inference`
  - 监听 `192.168.10.1:5000` 和 `192.168.10.1:5001`
- 已确认旧基线 `23bac51 baseline recovered teammate project` 可连接 Ethernet。
- 最新版失败原因定位：
  - 之前在 LCD/camera/LVGL/vision/分拣硬件任务之后再启动 Ethernet，容易因内部 RAM 紧张导致默认 event loop 创建失败，报 `ESP_ERR_NO_MEM`。
  - 调整后又发现真实 IO task 在 Ethernet 已连接后可能创建失败，因此将 `sort_real_io` 任务栈放到 PSRAM。
- 本次代码改动：
  - 修改前先提交当前脏工作区：`947b8b5 checkpoint before restoring ethernet init`。
  - Ethernet 初始化恢复提交：`35bbca5 restore ethernet init and shorten vision miss hold`。
  - 最新 ESP 提交：`12f5c41 set vision miss hold to two`。
  - `main/system_init.c` 从 `4946a30 restore lvgl perf monitor overlay` 恢复初始化内容。
  - 恢复后的 `System_Init()` 会启动 `screen_uvc_start()`，再启动 `ethernet_app_start()` 并等待 `ethernet_app_wait_ready(5000)`，之后启动分拣调试入口、电机输出和真实传感器输入。
  - `sdkconfig` 同步恢复 `CONFIG_LV_USE_SYSMON=y`、`CONFIG_LV_USE_PERF_MONITOR=y`、`CONFIG_LV_PERF_MONITOR_ALIGN_BOTTOM_RIGHT=y`。
  - `components/vision/framework/vision_detect.c` 中 `VISION_DISPLAY_MISS_KEEP_COUNT` 从 `3` 改为 `2`。

## Verification

- `idf.py build` 通过，最终 app version 为 `12f5c41`。
- 2026-07-07 最终恢复 LVGL 性能显示配置：
  - `sdkconfig`: `CONFIG_LV_USE_SYSMON=y`
  - `sdkconfig`: `CONFIG_LV_USE_PERF_MONITOR=y`
  - `sdkconfig`: `CONFIG_LV_PERF_MONITOR_ALIGN_BOTTOM_RIGHT=y`
  - `build/config/sdkconfig.h` 已定义 `CONFIG_LV_USE_PERF_MONITOR 1` 和 `CONFIG_LV_PERF_MONITOR_ALIGN_BOTTOM_RIGHT 1`。
- 2026-07-07 已将 `35bbca5` 烧录到 `/dev/serial/by-id/usb-Espressif_USB_JTAG_serial_debug_unit_E8:F6:0A:E1:7A:D1-if00`，monitor 确认 app version `35bbca5`；随后用户要求把 miss 调到 2，本轮 `12f5c41` 只完成 build，尚未烧录。

## Next Step

- 如需实机验证最新 miss=2，需要烧录 `12f5c41` 并 monitor 确认启动、电机、Ethernet 和包裹图片发送行为。

## Blockers

- 无当前阻塞。
