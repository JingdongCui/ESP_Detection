# Current Task

## Goal

按用户要求修改未检出包裹的分类序列：从固定韵达改为 `1,2,3,2,3` 循环，并烧录到板子。

## Current State

- 活跃 ESP 工程：`/home/kazeform/2026esp/ESP32P4_Detection`
- 分支：`feat/screen-uvc-stream`
- 当前板子串口：
  - `/dev/serial/by-id/usb-Espressif_USB_JTAG_serial_debug_unit_E8:F6:0A:E1:7A:D1-if00`
- 本次代码改动：
  - `components/Sorter_app/sorter_core/sorter_scheduler.c`：未检出/视觉失败的实际调度分类改为 `CLASS1,CLASS2,CLASS3,CLASS2,CLASS3` 循环。
  - `components/Sorter_app/sorting_sim_control.c`：调试状态里的 `next_failed_class` 同步显示同一 `12323` 序列。
  - 提交：`cad27e5 use 12323 failed package class sequence`。
- 当前工作区仍有本轮未处理的现场差异：
  - `components/bsp/include/sorter_debug_config.h`: A 电机默认速度为 `60`。
  - `main/system_init.c`: `system_monitor()` 被注释，`SORTER_TCP_LINK_ENABLE` 分支被 `#if 0` 关闭。
  - 因这些差异未提交，最终烧录 app version 显示为 `cad27e5-dirty`。

## Verification

- `idf.py build` 通过。
  - app version：`cad27e5-dirty`。
  - app 大小：`0x4fa790`，factory 分区剩余 `0x105870`，约 `17%`。
- `idf.py -p /dev/serial/by-id/usb-Espressif_USB_JTAG_serial_debug_unit_E8:F6:0A:E1:7A:D1-if00 flash` 成功。
  - 芯片：ESP32-P4 revision `v3.1`。
  - bootloader、app、partition table、storage 均 `Hash of data verified`。
- `timeout 45s idf.py -p /dev/serial/by-id/usb-Espressif_USB_JTAG_serial_debug_unit_E8:F6:0A:E1:7A:D1-if00 monitor` 已执行。
  - 已确认 app version `cad27e5-dirty`。
  - 启动到 camera/LVGL 初始化，未见 panic/reboot。
  - `ISP_AWB` warning 高频刷屏，monitor 输出被截断；未做真实包裹触发闭环验证。

## Next Step

- 现场用未检出包裹连续触发，观察分拣类别是否按 `1,2,3,2,3` 重复。

## Blockers

- 无当前阻塞。
