# Current Task

## Goal

按根目录 `goal.md` 完成 ESP32-P4 推理恢复、上位机设备控制闭环、重连、UVC 与长稳验收，并保留可回退版本。

## Current State

- 固件工作分支：`goal/inference-and-device-control`，当前提交 `56a53fd`。
- 推理回退根因已定位：`sort_dbg` USB Serial/JTAG 调试监视任务会使 ESP-DL 双核阶段约慢 5 倍；真实分拣 IO 不是根因。
- 生产配置关闭 `SORTER_HARDWARE_DEBUG_MONITOR` 后，真实 S1/S2/S4 与电机保持启用。5 次 RTS 硬复位、每次预热后 60 个样本的预验收结果为：300 样本 P50 74.443 ms、P95 101.947 ms、max 168.072 ms，5 个样本 >=150 ms、0 个样本 >=500 ms。系统性 500～600 ms 回退已消除，但严格 P95/max 门槛未通过，且 RTS 硬复位不等于物理断电冷启动，因此仍是 candidate。
- 已实现板端 `CONTROL_JSON` 类型 `0x11`：get/state、set、restart、完整 capability、严格类型/范围/step 校验和错误响应。
- 已覆盖显示亮度、四项 ISP、auto 开关、检测/叠加、双阈值、三路电机、图像/metrics 上报及设备重启。
- 已修复图像通道空闲时不感知 Host FIN 的重连缺陷；连续测试可在约 3 秒内重新取得 5000/5001 双通道。
- 已启用 `components/bsp/sc2336_ui_p4_eco4.json`。该文件与官方 ECO4 默认画质参数唯一差异是追加 UI IPA override；此前文件存在但 Kconfig 仍选默认配置，导致 ISP setter 不生效。
- 集成测试器：`ESP32P4_Detection/tools/control_protocol_integration.py`；正向/负向/畸形 JSON/恢复/ISP/restart/reconnect 均已在实板通过。
- 推理采集器：`ESP32P4_Detection/tools/inference_latency_acceptance.py`；原始 5x60 证据为 `docs/agent/run_logs/2026-07-17-inference-hard-reset-5x60.json`。
- 增强采集器后的 1x100 诊断轮全部通过：P50 75.680 ms、P95 94.113 ms、max 110.665 ms、零 >=150 ms；wait P50/P95/max 为 49.891/69.700/87.139 ms。`wb_us` 与 wait 的 Pearson 相关系数 0.9907，与调用任务 CPU 的相关系数仅 0.1058，支持尾延迟来自调度/worker 等待而非调用任务计算变慢；但本轮未复现之前 5/300 的异常，不能据此宣告稳定。
- 已增加一次性 UART 任务/堆验收快照与导出器 `tools/system_acceptance_snapshot.py`。实板导出 24/24 个任务：20 个内部栈、4 个 PSRAM 栈、0 个未知；`eth_control`、`eth_img_send`、`eth_img_prod`、`cam_isp` 的实际栈地址均在 PSRAM，`vision_det`、`dl_mc0/1`、`sort_real_io` 均在内部 SRAM。
- 启动阶段所有应用任务最低剩余均 >=512 B 且 >=20%；但该快照不是 60 分钟历史水位，不能完成长稳验收。当前多项任务剩余 >60%，应在覆盖最坏路径与长稳后再缩栈或说明保留原因。
- 堆完整性为 ok；free/min/largest=6836391/6810219/6684660 B，internal free/largest=26923/21492 B，PSRAM free/largest=6809820/6684660 B。DMA free 仅 1791 B、largest 仅 76 B，直接解释 UVC JPEG `rxlink` 连续 DMA 分配失败。
- 61 分钟长稳已执行但失败：3660.059 s 内采 826 个 wb_only 样本，P50 75.533 ms、P95 101.564 ms、max 161.204 ms、15 个 >=150 ms、0 个 >=500 ms；约 40 分钟时 Core0 `Instruction access fault`，MEPC/RA=0x10，SP 位于 `dl_mc0` 栈。板端 SW reset 后 Host 自动恢复双通道和推理。
- panic 栈中地址解析到 `DualCoreWorkerTask`、`Module::forward_args`、`std::function` 与 depthwise-conv 汇编；`dl_mc0` 启动最低剩余 928/1785 B 且 panic dump 仍有大量 A5，普通栈耗尽不是首要解释。精确的无效函数/虚调用目标来源尚未定位。
- 候选诊断防线 `537cb8e` 为 ESP-DL worker dispatch 保存 op/args 补码与预期 vtable，worker 执行前校验；`56a53fd` 让长稳采集器统计 rejection。它用于阻止已检测到的坏派发和保留证据，不宣称已定位根因。
- Guard 版本第二轮 61 分钟长稳完整通过连续运行检查：3660.192 s、启动 1 次、715 次双端口检查零失败、fatal 0、guard rejection 0，且在 3602 s 取得第二份 24/24 任务及 heap 快照。825 样本 P50/P95/max=77.625/99.809/162.326 ms，17 个 >=150 ms、0 个 >=500 ms；总体 P95 通过但 max 仍失败，不能标 stable。
- 60 分钟末 heap integrity=ok，free/min=6836467/6809687 B；所有应用任务仍 >=512 B 且 >=20%，最低是 `dl_mc0/1=880/1785 B (49.3%)`。DMA largest 仍仅 72 B，UVC 阻塞不变。
- 固件 `idf.py build`、全量 flash、后续 app-flash 均通过 Hash 校验；ESP32-P4 revision v1.0。
- Host 未改代码，`cmake --build build -j` 与 CTest 1/1 通过；GUI PID 3266170 已恢复，5000/5001 与 192.168.10.2 均为 ESTABLISHED。
- Host 原生 Qt 四页已截图。第三页上下区所有控制项和板端状态可见；通过 UI 短暂关闭并恢复 detection，第四页运行日志明确记录 false→true，最终双 TCP 通道仍 ESTABLISHED。第二页因现场未触发包裹识别边沿而无图，不能计为相机/检测框视觉通过。
- UVC 仍在 `jpeg_new_encoder_engine(): no memory for jpeg encoder rxlink` 失败，按 goal 顺序留到控制稳定后处理。

## Backups

- 起点标签：`backup/goal-start-20260717`。
- 推理候选：`backup/inference-70ms-candidate-20260717`（`6ad4fd5`）。
- 控制/ISP/重连候选：`backup/control-json-isp-reconnect-candidate-20260717`（`b1dfef5`）。
- 300 样本预验收候选：`backup/goal-300-sample-tested-candidate-20260717`（`2260596`）。
- 单轮严格通过诊断候选：`backup/inference-strict-pass-1x100-candidate-20260717`（`8f034e0`，仍非 stable）。
- 任务/内存证据候选：`backup/task-heap-snapshot-candidate-20260717`（`27c8b83`）。
- 长稳失败证据点：`backup/61min-failed-evidence-20260717`（`0150722`，不可作为 stable）。
- Guard 单轮长稳候选：`backup/dl-guard-61min-pass-candidate-20260717`（`56a53fd`，单轮通过但尾延迟/根因/物理验收未完成）。
- A/B 仅供诊断：`backup/ab-sorter-off-20260717`，不可作为生产版本。

## Next Step

1. 继续复现并定位 `dl_mc0` 无效函数/虚调用目标 0x10；guard 单轮 61 分钟未复现也未拒绝，仍需多轮/真实 IO 路径验证 op/args 生命周期、模型对象/vtable 与内存破坏。
2. 使用物理断电完成 5 次真正冷启动；启动任务/内存证据已补齐，后续需在 60 分钟长稳结束再次导出历史最低水位与最低 heap。
3. Host 软件 UI/状态回显与截图已完成；继续做屏幕亮度、ISP 画面、检测框/包裹图和三路真实电机的现场实拍观察。
4. 断网/Host 杀进程恢复已做协议级测试；继续跑真实 IO/包裹路径 60～120 分钟并观察 guard rejection，不能用本次单轮通过替代多场景长稳。
5. 控制面稳定后处理 UVC JPEG DMA 内存不足，再做 USB 枚举和视频稳定性验收。

## Blockers

- UVC JPEG DMA internal-memory allocation remains unresolved；启动时 DMA 最大连续块实测仅 76 B。
- 300 样本硬复位预验收已完成，但 P95/max 未通过；物理断电冷启动、长稳、视觉证据仍需要继续占用实板与现场观察，当前不能把 candidate 标为 stable。
- 原版 61 分钟长稳在约 40 分钟发生一次 `dl_mc0` Instruction access fault；guard 候选虽单轮跑满且取得 60 分钟快照，但未触发 guard、未证明根因消除，且 max=162.326 ms 仍超严格门槛。
