# Current Task

## Goal

按根目录 `goal.md` 完成 ESP32-P4 推理恢复、上位机设备控制闭环、重连、UVC 与长稳验收，并保留可回退版本。

## Current State

- 固件工作分支：`goal/inference-and-device-control`，当前提交 `b1dfef5`。
- 推理回退根因已定位：`sort_dbg` USB Serial/JTAG 调试监视任务会使 ESP-DL 双核阶段约慢 5 倍；真实分拣 IO 不是根因。
- 生产配置关闭 `SORTER_HARDWARE_DEBUG_MONITOR` 后，真实 S1/S2/S4 与电机保持启用，waybill 单阶段多数约 65～77 ms；已见少量约 145/152 ms 样本，尚未完成 5 次冷启动、300+ 样本正式统计，因此仍是 candidate。
- 已实现板端 `CONTROL_JSON` 类型 `0x11`：get/state、set、restart、完整 capability、严格类型/范围/step 校验和错误响应。
- 已覆盖显示亮度、四项 ISP、auto 开关、检测/叠加、双阈值、三路电机、图像/metrics 上报及设备重启。
- 已修复图像通道空闲时不感知 Host FIN 的重连缺陷；连续测试可在约 3 秒内重新取得 5000/5001 双通道。
- 已启用 `components/bsp/sc2336_ui_p4_eco4.json`。该文件与官方 ECO4 默认画质参数唯一差异是追加 UI IPA override；此前文件存在但 Kconfig 仍选默认配置，导致 ISP setter 不生效。
- 集成测试器：`ESP32P4_Detection/tools/control_protocol_integration.py`；正向/负向/畸形 JSON/恢复/ISP/restart/reconnect 均已在实板通过。
- 固件 `idf.py build`、全量 flash、后续 app-flash 均通过 Hash 校验；ESP32-P4 revision v1.0。
- Host 未改代码，`cmake --build build -j` 与 CTest 1/1 通过；GUI PID 3266170 已恢复，5000/5001 与 192.168.10.2 均为 ESTABLISHED。
- UVC 仍在 `jpeg_new_encoder_engine(): no memory for jpeg encoder rxlink` 失败，按 goal 顺序留到控制稳定后处理。

## Backups

- 起点标签：`backup/goal-start-20260717`。
- 推理候选：`backup/inference-70ms-candidate-20260717`（`6ad4fd5`）。
- 控制/ISP/重连候选：`backup/control-json-isp-reconnect-candidate-20260717`（`b1dfef5`）。
- A/B 仅供诊断：`backup/ab-sorter-off-20260717`，不可作为生产版本。

## Next Step

1. 自动采集 5 次冷启动、预热后 300+ 推理样本，计算 P50/P95/max 并确认零 500 ms 回退。
2. 补任务栈地址/水位、内外 RAM、CPU 与最低 heap 证据；确认 152 ms 是否可复现并消除或调整候选。
3. 用 Host 第三页逐项做 UI 可见闭环和截图/实拍，尤其是屏幕、相机画面、检测框和三路真实电机。
4. 完成断网/Host 杀进程/恢复与 60～120 分钟真实 IO 长稳。
5. 控制面稳定后处理 UVC JPEG DMA 内存不足，再做 USB 枚举和视频稳定性验收。

## Blockers

- UVC JPEG DMA internal-memory allocation remains unresolved.
- 300 样本/5 冷启动、长稳、视觉证据需要继续占用实板与现场观察；当前不能把 candidate 标为 stable。
