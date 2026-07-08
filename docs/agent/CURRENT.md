# Current Task

## Goal

按用户要求调通 ESP32P4_Detection：上位机自动连接后能收到图像链路和性能参数，并完成构建、烧录、现场链路验证。

## Current State

- 活跃 ESP 工程：`/home/kazeform/2026esp/ESP32P4_Detection`
- 活跃上位机工程：`/home/kazeform/2026esp/esp32_host_no_inference`
- 已确认 host 工程启动时会自动监听：
  - control/metrics：TCP `5000`
  - image：TCP `5001`
- 本轮 ESP 提交：
  - `9ecbe05 checkpoint current sorter timing config`
  - `92c32eb enable ethernet image and metrics autostart`
- 已烧录 app version：`92c32eb`

## Implementation Notes

- `main/system_init.c` 恢复上位机 TCP 链路启动：
  - `ethernet_app_start()`
  - `ethernet_app_wait_ready(SORTER_ETHERNET_READY_WAIT_MS)`
- 恢复 `system_monitor()`，放在 Ethernet ready 和分拣硬件启动之后，避免提前占用内部 RAM。
- 保持开机分拣硬件入口：
  - `sorting_sim_debug_start()`
  - `sorting_sim_control_set_motor_output_enabled(true)`
  - `sorting_sim_control_set_sensor_input_enabled(true)`
- host 侧未改代码；代码检查确认 `HostController` 构造函数中启动网络线程并调用 `startServer()`。

## Verification

- `idf.py build` 通过。
  - app version：`92c32eb`
  - app size：`0x526d80`
  - factory 分区剩余：`0xd9280`，约 `14%`
- 使用端口 `/dev/serial/by-id/usb-Espressif_USB_JTAG_serial_debug_unit_E8:F6:0A:E1:7A:D1-if00` 烧录成功。
  - 芯片：ESP32-P4 revision `v3.1`
  - bootloader/app/partition/storage 均 `Hash of data verified`
- `idf.py monitor` 需 TTY；使用 TTY monitor 跑 60 秒，确认：
  - app version `92c32eb`
  - 启动到 camera/LVGL/网络重连阶段，未见 panic/reboot。
  - host 未监听时，板端持续重试连接 `192.168.10.1:5000/5001`。
- 临时 TCP 接收器验证：
  - 主机网口 `enp3s0` 为 `192.168.10.1/24`。
  - 接收器监听 `5000/5001` 后，板端 control 自动连接成功。
  - control 通道收到 1 秒周期 metrics 包，包含 CPU/heap/PSRAM/image stats。
  - hard reset 板子后，冷启动自动连上 `5000` 和 `5001` 两路。
- 图像 payload 未现场验证到 JPEG 数据，原因是测试时没有触发“识别成功的新包裹”快照；已验证 image TCP 自动连接成功。

## Next Step

- 现场打开正式 Qt host 程序后，应看到设备自动在线、性能参数刷新。
- 放入可识别包裹触发新包裹快照后，host 应收到 JPEG 图像。

## Blockers

- 无代码阻塞。
- 当前仅缺少真实包裹触发来闭环验证 JPEG payload。
