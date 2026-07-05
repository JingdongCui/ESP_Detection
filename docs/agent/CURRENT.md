# Current Task

## Goal

修正上位机系统维护页端口卡片布局，并调整 `new_merge` 默认 TCP 策略：默认开启上位机通信链路，默认关闭 Ethernet 模拟分拣状态输出。

## Current State

- 上位机当前分支：`tcp-board-align`。
- 上位机修改前 checkpoint：`34338dd checkpoint before maintenance layout fix`。
- 板端当前分支：`motor-roi`。
- 板端修改前 checkpoint：`5e507e6 checkpoint before tcp default split`。
- 已修改：
  - `ReservePage.qml` 的链路端口区域改为专用紧凑 `PortCard`，避免 4 张 `MetricCard` 在固定 276 高度面板内溢出错位。
  - `SORTER_TCP_LINK_ENABLE` 默认改回 `1`，重烧录后默认启动上位机 TCP control/image 链路。
  - 新增 `SORTER_TCP_SIM_LINE_OUTPUT_ENABLE`，默认 `0`；默认仍运行分拣 tick，但不把 tick 过程的模拟状态线发到上位机。
  - 上位机发来的 `CONFIG` / `HW_STATUS` 等 SIM line 控制命令仍会处理并可回复。

## Verification

- 上位机：
  - `cmake --build --preset debug`：通过。
  - `QT_QPA_PLATFORM=offscreen timeout 8s ./build/debug/bin/esp32_host_no_inference`：可启动到 timeout。
  - 验证监听：`192.168.10.1:5000/5001` 均 LISTEN。
- 板端：
  - `idf.py build`：通过，app 大小 `0x4eecc0`，factory 分区剩余约 18%。
  - `idf.py -p /dev/serial/by-id/... flash`：通过，ESP32-P4 revision `v1.0`，bootloader/app/partition/storage hash verified。
  - `script -qfec "timeout 120s idf.py -p /dev/serial/by-id/... monitor" /tmp/new_merge_tcp_default_monitor.log`：到 `System initialization done`，120 秒窗口未见 panic/reboot。
  - `ss` 轮询确认 `192.168.10.1:5000` 与 `192.168.10.1:5001` 均长期 `ESTABLISHED`。
  - `~/Documents/ESP32Host/images/latest_preview.jpg` 更新时间到 `2026-07-05 19:39:56 +0800`，说明 JPEG 图像链路实际接收。

## Notes

- 上位机 offscreen 验证时仍可见既有 Qt 警告：`QTcpServer` 跨线程 child 创建、部分导航按钮 binding loop、DetectionPage 一个 undefined bool；本轮未处理这些非本次根因问题。
- 旧固件重烧录后连接不上，是因为上一阶段把 `SORTER_TCP_LINK_ENABLE` 默认设成 `0`，导致 `ethernet_app_start()` 不执行。

## Next Step

- 提交上位机和板端改动，并同步根目录 agent 文档提交。

## Blockers

- 未阻塞。
