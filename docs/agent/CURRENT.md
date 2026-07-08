# Current Task

## Goal

按用户要求修改 ESP32P4_Detection 发图逻辑：连续 3 次 miss 后才重新允许发图，避免同一个包裹重复发图，并完成构建、烧录、运行验证。

## Current State

- 活跃 ESP 工程：`/home/kazeform/2026esp/ESP32P4_Detection`
- 修改文件：`components/vision/framework/vision_detect.c`
- ESP 提交：`e82a832 rearm snapshots after three misses`
- 已烧录 app version：`e82a832`

## Implementation Notes

- `VISION_DISPLAY_MISS_KEEP_COUNT` 从 `2` 改为 `3`。
- 删除按 `vision_package_id` 变化立即发图的 gating。
- 新增 `s_snapshot_capture_armed` 发图闸门：
  - 启动/检测关闭后置为 armed。
  - 第一次有效命中时抓图并 disarm。
  - 连续第 3 次 miss 时重新 armed。
  - 连续命中或短暂 1-2 帧 miss 不会再次抓图。
- `vision_package_id` 仍用于 RTT 日志输出，不再决定是否发图。

## Verification

- `git diff --check` 通过。
- `idf.py build` 通过。
  - app version：`e82a832`
  - app size：`0x526c80`
  - factory 分区剩余：`0xd9380`，约 `14%`
- 使用端口 `/dev/serial/by-id/usb-Espressif_USB_JTAG_serial_debug_unit_E8:F6:0A:E1:7A:D1-if00` 烧录成功。
  - 芯片：ESP32-P4 revision `v3.1`
  - bootloader/app/partition/storage 均 `Hash of data verified`
- 使用 TTY monitor 跑 50 秒，确认：
  - app version `e82a832`
  - 启动到 camera/LVGL/Ethernet image task。
  - 未见 panic/reboot。
  - 无新包裹快照时 image producer 正常报 `JPEG snapshot skipped: ESP_ERR_NOT_FOUND`。

## Next Step

- 现场用真实包裹验证：同一包裹连续命中只发一次；离开画面并连续 miss 3 次后，下一个包裹才重新发图。

## Blockers

- 无代码阻塞。
- 当前缺少真实包裹触发，未实测重复发图抑制效果。
