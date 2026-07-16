# Goal 交付与候选版本说明（2026-07-17）

## 推荐版本

### A. 当前推荐候选：guard + 控制闭环

- 固件分支：`goal/inference-and-device-control`
- 固件提交：`56a53fd`
- 标签：`backup/dl-guard-61min-pass-candidate-20260717`
- Host 提交：`6bcee3b`

包含 70 ms 级推理恢复、真实 S1/S2/S4 与电机、CONTROL_JSON 全控制面、ISP 自定义配置、双 TCP 空闲断线重连、启动及 60 分钟 task/heap 快照，以及 ESP-DL dispatch 完整性防线。本版完成一轮 3660.192 s 连续测试，无崩溃/重启/连接失败，推荐用于继续联调和演示前验证。

限制：它仍是 candidate，不是 stable。单轮没有触发 guard，未证明此前 `dl_mc0` 跳转 0x10 的根因已消除；825 样本最大 162.326 ms，超过 goal 的 150 ms 门槛；UVC 和物理验收未完成。

### B. 保守功能候选：控制/ISP/重连

- 固件提交：`b1dfef5`
- 标签：`backup/control-json-isp-reconnect-candidate-20260717`

包含推理恢复与完整设备控制闭环，已经过正向、负向、畸形 JSON、状态恢复、ISP、restart 及双端口重连实板测试。不包含后续 task/heap 快照、长稳采集器和 ESP-DL guard，适合在怀疑 guard 改动时作功能回退。

限制：后续长稳测试证明同一代码链的未加 guard 版本可能在约 40 分钟发生 ESP-DL worker Instruction access fault，因此不能作为长稳版本。

### C. 最小推理修复候选

- 固件提交：`6ad4fd5`
- 标签：`backup/inference-70ms-candidate-20260717`

只关闭会干扰 ESP-DL 双核 worker 的 `sort_dbg` USB Serial/JTAG 调试监视任务，保留真实分拣 IO 与电机。适合验证“500～600 ms 回退是否由调试监视引起”的最小 A/B 基线。

限制：没有板端 CONTROL_JSON、ISP 选择修复和后续重连/长稳增强，不适合作为完整交付版。

## 其他证据点

| 标签 | 提交 | 用途 | 结论 |
|---|---|---|---|
| `backup/goal-start-20260717` | 起点 | 原始现场回退 | 可还原 |
| `backup/goal-300-sample-tested-candidate-20260717` | `2260596` | 5×60 RTS reset 统计 | P50 74.443、P95 101.947、max 168.072 ms |
| `backup/inference-strict-pass-1x100-candidate-20260717` | `8f034e0` | CPU/wait 诊断 | 单轮 100 样本严格通过；不能代表多轮稳定 |
| `backup/task-heap-snapshot-candidate-20260717` | `27c8b83` | 24 任务栈/内存归属 | task/heap integrity 通过，量化 UVC DMA 阻塞 |
| `backup/61min-failed-evidence-20260717` | `0150722` | 保留崩溃证据 | 约 2412 s Instruction access fault；禁止作为候选 |
| `backup/ab-sorter-off-20260717` | A/B | 关闭 sorter 诊断 | 仅诊断，不含真实 IO，禁止生产使用 |

## 已完成测试

- 固件 `idf.py build` 通过；完整 `idf.py flash monitor` 与后续 app-flash 均 Hash verified。
- CONTROL_JSON：全部设置项、能力、非法 key/JSON/type/range/step、恢复、restart、5000/5001 重连实板通过。
- Host：CMake build、CTest 1/1、原生 Qt 四页可见检查通过；UI detection false→true 闭环有截图与日志。
- 推理：5×60 共 300 样本；另有 1×100 CPU/wait 诊断；两轮各 61 分钟长稳原始日志均保留。
- 当前推荐候选长稳：3660.192 s，825 样本，启动 1、fatal 0、guard rejection 0、连接失败 0；60 分钟末 24/24 task 与 heap integrity=ok。
- Python 工具 `py_compile` 通过；长稳 JSON 结构、样本数、快照数、连接和 fatal/rejection 断言通过。

详细证据见：

- `docs/agent/run_logs/2026-07-17-stability-guard.md`
- `docs/agent/run_logs/2026-07-17-stability.md`
- `docs/agent/run_logs/2026-07-17-visual.md`
- `docs/agent/run_logs/2026-07-17-inference-hard-reset-5x60.json`

## 未完成与现场验收清单

1. 做 5 次真正物理断电冷启动；当前 5 次 reset 是 CP2102N RTS，不可冒充断电。
2. 用真实包裹覆盖 camera→检测框→分类→S1/S2/S4→三路电机完整路径并拍照/录像；当前视觉页无包裹图。
3. 继续至少一轮真实 IO 的 60～120 分钟长稳，观察是否出现 fatal 或 `dl_guard worker guard rejected`。
4. 将单阶段最大值收敛到 <=150 ms；当前总体 P95 已通过但 max 失败。
5. 上述稳定后再处理 UVC。当前 DMA largest 仅 72 B，JPEG encoder `rxlink` 无法分配，尚无 USB 枚举/视频稳定性结果。

在完成这些项目之前，不创建 `stable` 标签，不删除失败证据，也不把自动重启后的恢复计为连续长稳通过。
