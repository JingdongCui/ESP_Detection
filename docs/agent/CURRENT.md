# Current Task

## Goal

在上位机视觉检测页显示每张 JPEG 的快递类别，并用快递统计替换图片链路中的失败统计展示。

## Current State

- 上位机当前分支：`tcp-board-align`。
- 上位机修改前 checkpoint：`5adbb4f checkpoint before image category display`。
- 板端当前分支：`motor-roi`。
- 板端修改前 checkpoint：`a09b055 checkpoint before jpeg category metadata`。
- 已修改：
  - 板端 JPEG 包头复用 `reserved/reserved2` 携带分类元数据：`class_id=1/2/3` 对应极兔/中通/韵达，`reserved2` 低 8 位为置信度百分比。
  - 板端新增视觉分类快照接口，未知/无目标按极兔、置信度 0 兜底。
  - 上位机解析 JPEG 包头分类字段，写入图片历史记录，并按收到的图片记录累计极兔/中通/韵达统计。
  - 视觉检测页主图和历史列表显示类别；图片链路卡片中的“失败”统计替换为“快递统计”。
  - `DetectionPage.qml` 的 `hasSelectedFrame` 改为显式 bool，消除该页 undefined bool 警告。

## Verification

- 上位机：
  - `cmake --build --preset debug`：通过。
  - `QT_QPA_PLATFORM=offscreen timeout 8s ./build/debug/bin/esp32_host_no_inference`：可启动到 timeout，无输出。
  - 本机模拟发送 3 张 JPEG，分别带 `class_id=1/2/3`，生成 `frame_920001.jpg`、`frame_920002.jpg`、`frame_920003.jpg`。
  - `rg` 检查无 `发送失败` 或 UI `title: "失败"` 残留；仍保留正常 `JPEG 发送` 计数。
- 板端：
  - `idf.py build`：通过，app 大小 `0x4eee40`，factory 分区剩余约 18%。
  - `idf.py -p /dev/serial/by-id/... flash`：通过，ESP32-P4 revision `v1.0`，bootloader/app/partition/storage hash verified。
  - `script -qfec "timeout 120s idf.py -p /dev/serial/by-id/... monitor" /tmp/new_merge_jpeg_category_monitor.log`：启动到 `System initialization done`，未见 panic/reboot。
  - `ss` 轮询确认 `192.168.10.1:5000/5001` 均长期 `ESTABLISHED`，`latest_preview.jpg` 每 5 秒更新。
  - Python listener 解析真实板端 JPEG 包头：`type=1 seq=161 size=640x375 pixel=2 class_id=1 confidence=0 payload=42979`。

## Notes

- 快递统计口径按收到的 JPEG 图片记录计数，不等于真实包裹 ID 计数。
- 无法分类按用户确认的策略显示和统计为极兔。
- 真实板端当前测试画面未分类时会发送 `class_id=1 confidence=0`，符合兜底策略。

## Next Step

- 提交上位机和板端改动，并同步根目录 agent 文档提交。

## Blockers

- 未阻塞。
