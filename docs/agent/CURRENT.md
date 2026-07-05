# Current Task

## Goal

优化 `new_merge` TCP 图片/CPU 数据链路，降低 CPU 占用和发送窗口峰值：
- CPU/内存 metrics 实时发送。
- 图片允许延迟，测试阶段按 5 秒一张。
- 允许双 TCP 通道和压缩图片。

## Current State

- `new_merge` 分支：`motor-roi`。
- 修改前已在 `new_merge` 提交现场 checkpoint：
  - `0559655 checkpoint before tcp cpu optimization`
- 已实现：
  - TCP control 通道：`192.168.10.1:5000`，用于 time sync、SIM_LINE、sorter tick、metrics。
  - TCP image 通道：`192.168.10.1:5001`，用于低优先级 JPEG 图片发送。
  - metrics 周期：1 秒。
  - image 测试周期：5 秒。
  - image 尺寸：`640x375` RGB888 快照，经 `esp_new_jpeg` 编码为 JPEG。
  - JPEG quality：60，subsample：4:4:4，避免 `640x375` 奇数高度在 4:2:0 下不确定。
  - image queue 深度：2；host 慢或断线时不无限排队。
  - Ethernet metrics 不再调用 `uxTaskGetSystemState()`，改读 `system_monitor_get_metrics()` 缓存。
  - Ethernet image 不再直接 `cam_sensor_get_frame()`，改通过 vision 新增的 `vision_copy_latest_frame_scaled_rgb888()` 从最新 ring frame 生成 PPA 缩放快照。
  - PPA 写入 snapshot 后执行 M2C cache invalidate，确保 JPEG 读取新图。
  - `main/system_init.c` 新增 `SORTER_TCP_LINK_ENABLE=1`，调试模式下也可启动 TCP；`system_monitor()` 在 Ethernet 前启动。
- 静态检查：
  - `components/Ethernet_app` 内不再出现 `cam_sensor_get_frame`。
  - `components/Ethernet_app` 内不再出现 `uxTaskGetSystemState`。

## Verification

- `cd /home/kazeform/2026esp/new_merge && idf.py build`：通过。
- 生成 app：`build/sample_project.bin`，大小 `0x4fbed0`，factory 分区剩余约 17%。
- 当前无 `/dev/ttyACM*` 或 `/dev/ttyUSB*`，无法执行 `idf.py flash monitor`。

## Immediate Next Step

- 等板子接入后执行：
  - `idf.py -p <port> -b 921600 flash`
  - `idf.py -p <port> monitor`
  - 上位机同时监听 `5000` metrics/control 与 `5001` JPEG image。

## Blockers

- 当前没有板子/串口设备，无法实机验证 Ethernet 连接、JPEG 编码耗时、CPU 峰值和 image queue 行为。
