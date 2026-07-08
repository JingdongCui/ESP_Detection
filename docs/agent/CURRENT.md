# Current Task

## Goal

按用户要求调整 ESP32P4_Detection：停掉硬件分拣启动，把性能监视提前启动，并完成构建、烧录、运行验证。

## Current State

- 活跃 ESP 工程：`/home/kazeform/2026esp/ESP32P4_Detection`
- 修改文件：`main/system_init.c`
- ESP 提交：`e361f65 disable sorter hardware and start monitor earlier`
- 已烧录 app version：`e361f65`

## Implementation Notes

- `system_monitor()` 已前移到 `setupUi()` / `ui_bind_dashboard()` / `BSP_LVGL_Unlock()` 之后立即启动。
  - 这样 UI 事件表已建立，性能监视可安全推送 CPU/内存指标。
  - 启动时机早于 `vision_start()`、`screen_uvc_start()`、`ethernet_app_start()`。
- 硬件分拣启动已停用：
  - 不再调用 `sorting_sim_debug_start()`。
  - 不再调用 `sorting_sim_control_set_motor_output_enabled(true)`。
  - 不再调用 `sorting_sim_control_set_sensor_input_enabled(true)`。
- Ethernet 上位机链路、图像链路、metrics 链路保留。

## Verification

- `git diff --check` 通过。
- `idf.py build` 通过。
  - app version：`e361f65`
  - app size：`0x524440`
  - factory 分区剩余：`0xdbbc0`，约 `14%`
- 使用端口 `/dev/serial/by-id/usb-Espressif_USB_JTAG_serial_debug_unit_E8:F6:0A:E1:7A:D1-if00` 烧录成功。
  - 芯片：ESP32-P4 revision `v3.1`
  - bootloader/app/partition/storage 均 `Hash of data verified`
- 使用 TTY monitor 跑 60 秒，确认：
  - app version `e361f65`
  - UI/LVGL 初始化后出现 `temperature_sensor` 初始化，说明 `system_monitor()` 已提前启动。
  - 启动到 camera/LVGL/Ethernet image task。
  - 未见 panic/reboot。
- monitor 被 `ISP_AWB` warning 高频刷屏并截断；本轮主要依赖代码检查确认硬件分拣启动调用已移除。

## Next Step

- 现场确认电机不上电、不读取真实分拣传感器输入；上位机仍应接收 metrics 和图像链路。

## Blockers

- 无代码阻塞。
- 未接正式上位机和真实硬件负载做完整现场闭环。
