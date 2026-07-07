# Current Task

## Goal

让 `ESP32P4_Detection` 最新版本在电机/真实分拣链路开机默认启动的同时，Ethernet 上位机连接仍能稳定建立；同时按用户打断要求关闭 `sdkconfig` 中 LVGL perf monitor bottom right 显示。

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
- 当前代码改动：
  - `ethernet_app_early_init()` 保持早期创建默认 event loop/event group。
  - 新增 `ethernet_app_wait_ready(timeout_ms)`，等待 control/image 连接准备位。
  - `System_Init()` 中先启动 Ethernet 并最多等待 5 秒 ready，再开 `sorting_sim_debug_start()`、电机输出和真实传感器输入。
  - `sort_real_io` 任务使用 `xTaskCreatePinnedToCoreWithCaps()`，栈放到 `MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT`。
  - `sdkconfig` 中关闭 `CONFIG_LV_USE_PERF_MONITOR`，不再启用 bottom right perf monitor。

## Verification

- `idf.py build` 通过。
- `idf.py -p /dev/serial/by-id/usb-Espressif_USB_JTAG_serial_debug_unit_E8:F6:0A:E1:7A:D1-if00 flash` 成功。
- 80 秒 monitor 关键日志：
  - `Ethernet Started`
  - `Ethernet Link Up`
  - `ETHIP:192.168.10.2`
  - `control connected to host`
  - `image connected to host`
  - `Ethernet sorter link ready before sorter hardware start`
  - `SORT 电机M1: 正转 65%`
  - `SORT 真实硬件链路已启用`
- 主机端 `ss` 确认：
  - `192.168.10.1:5000` ESTABLISHED 到 `192.168.10.2`
  - `192.168.10.1:5001` ESTABLISHED 到 `192.168.10.2`
- 本次验证未再出现：
  - `create event loop queue failed`
  - `Ethernet sorter link start failed: ESP_ERR_NO_MEM`
  - `create real IO task failed`

## Next Step

- 继续现场实机验证传感器实际电平与分拣节拍。
- 当前日志中 S2/S4 初始 active 会触发 `sensor_without_package`，需要结合现场遮挡/接线确认是否为正常默认电平。

## Blockers

- 无当前阻塞。
