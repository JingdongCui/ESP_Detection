# Current Task

## Goal

按用户要求将未检出包裹的分类序列改回 `1,2,3,1,2,3` 循环，并烧录到板子。

## Current State

- 活跃 ESP 工程：`/home/kazeform/2026esp/ESP32P4_Detection`
- 分支：`feat/screen-uvc-stream`
- 当前板子串口：
  - `/dev/serial/by-id/usb-Espressif_USB_JTAG_serial_debug_unit_E8:F6:0A:E1:7A:D1-if00`
- 修改前按项目规则提交当前现场配置：
  - `f052c71 checkpoint current hardware runtime config`
- 本次代码改动：
  - `components/Sorter_app/sorter_core/sorter_scheduler.c`：未检出/视觉失败的实际调度分类改为 `CLASS1,CLASS2,CLASS3,CLASS1,CLASS2,CLASS3` 循环。
  - `components/Sorter_app/sorting_sim_control.c`：调试状态里的 `next_failed_class` 同步显示同一 `123123` 序列。
  - 提交：`a84dbb5 use 123123 failed package class sequence`。
- ESP 工程当前工作区干净。

## Verification

- `idf.py build` 通过。
  - app version：`a84dbb5`。
  - app 大小：`0x4fa790`，factory 分区剩余 `0x105870`，约 `17%`。
- `idf.py -p /dev/serial/by-id/usb-Espressif_USB_JTAG_serial_debug_unit_E8:F6:0A:E1:7A:D1-if00 flash` 成功。
  - 芯片：ESP32-P4 revision `v3.1`。
  - bootloader、app、partition table、storage 均 `Hash of data verified`。
- `timeout 45s idf.py -p /dev/serial/by-id/usb-Espressif_USB_JTAG_serial_debug_unit_E8:F6:0A:E1:7A:D1-if00 monitor` 已执行。
  - 已确认 app version `a84dbb5`。
  - 启动到 camera/LVGL/分拣运行日志，未见 panic/reboot。
  - monitor 中看到未检出调度开头包裹为 `class1`、下一包为 `class2`。
  - `ISP_AWB` warning 高频刷屏，monitor 输出被截断；未完整跑满六个未检出包裹验证 `123123` 全序列。

## Next Step

- 现场连续触发未检出包裹，观察分拣类别是否按 `1,2,3,1,2,3` 重复。

## Blockers

- 无当前阻塞。
