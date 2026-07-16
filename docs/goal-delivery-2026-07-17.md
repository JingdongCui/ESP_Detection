# Goal 交付与候选版本说明（2026-07-17）

## 推荐版本

### A. 当前推荐候选：UI/推理隔离 + 强化 guard + 控制闭环

- 固件分支：`goal/inference-and-device-control`
- 固件提交：`60c9f8a`
- 标签：`backup/ui-inference-isolation-candidate-20260717`
- Host 提交：`6bcee3b`

包含此前完整控制闭环、真实 IO、重连和诊断能力，并增强 ESP-DL worker 实际虚调用目标校验。该版本还把 LVGL/swdraw/preview UI render group 完整保持在 priority-4 推理 worker 下方，阻断 LVGL mutex 优先级继承把预览 blit 提升到 priority 5 的尾延迟路径。

5×60 共 300 样本严格通过：P50/P95/max=67.312/71.393/132.003 ms，`>=150 ms` 和 `>=500 ms` 均为 0。任务快照确认 lvgl/swdraw/vision_disp=3/3/2、fetch/detect/dl_mc0/1=4。

限制：它仍是 candidate，不是 stable。最终候选的 61 分钟长稳正在重跑；此前 guard 版长稳通过不能代替新调度配置的长稳结果。也未证明此前 `dl_mc0` 跳转 0x10 的上游根因已消除；UVC、真正断电冷启动、LCD 触摸手感和物理包裹/电机验收未完成。

### B. 长稳已通过的 guard 候选

- 固件提交：`56a53fd`
- 标签：`backup/dl-guard-61min-pass-candidate-20260717`

完成一轮 3660.192 s 连续测试，无崩溃/重启/连接失败；适合在怀疑 UI 优先级调整时回退。限制是 825 样本最大 162.326 ms，超过 goal 的 150 ms 门槛，且 guard 只验证 vtable 指针，没有 `b08a1a3` 的实际虚调用目标校验。

### C. 保守功能候选：控制/ISP/重连

- 固件提交：`b1dfef5`
- 标签：`backup/control-json-isp-reconnect-candidate-20260717`

包含推理恢复与完整设备控制闭环，已经过正向、负向、畸形 JSON、状态恢复、ISP、restart 及双端口重连实板测试。不包含后续 task/heap 快照、长稳采集器和 ESP-DL guard，适合在怀疑 guard 改动时作功能回退。

限制：后续长稳测试证明同一代码链的未加 guard 版本可能在约 40 分钟发生 ESP-DL worker Instruction access fault，因此不能作为长稳版本。

### D. 最小推理修复候选

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
| `backup/dl-target-guard-candidate-20260717` | `b08a1a3` | 强化 worker 虚调用防线 | 校验并直接调用已验证的实际 target |
| `backup/preview-priority-tail-ab-20260717` | `a928dde` | 预览优先级 A/B | P95 改善但 max 155.593 ms 失败 |
| `backup/ui-worker-priority-tail-ab-20260717` | `f29525d` | swdraw/LVGL 诊断 | 揭示 mutex 优先级继承路径 |

## 已完成测试

- 固件 `idf.py build` 通过；完整 `idf.py flash monitor` 与后续 app-flash 均 Hash verified。
- CONTROL_JSON：全部设置项、能力、非法 key/JSON/type/range/step、恢复、restart、5000/5001 重连实板通过。
- Host：CMake build、CTest 1/1、原生 Qt 四页可见检查通过；UI detection false→true 闭环有截图与日志。
- 推理：5×60 共 300 样本；另有 1×100 CPU/wait 诊断；两轮各 61 分钟长稳原始日志均保留。
- 当前推荐候选长稳：3660.192 s，825 样本，启动 1、fatal 0、guard rejection 0、连接失败 0；60 分钟末 24/24 task 与 heap integrity=ok。
- 最终 UI 隔离候选 5×60：300 样本 P50/P95/max=67.312/71.393/132.003 ms，全部严格门槛通过。
- Python 工具 `py_compile` 通过；长稳 JSON 结构、样本数、快照数、连接和 fatal/rejection 断言通过。

详细证据见：

- `docs/agent/run_logs/2026-07-17-stability-guard.md`
- `docs/agent/run_logs/2026-07-17-stability.md`
- `docs/agent/run_logs/2026-07-17-visual.md`
- `docs/agent/run_logs/2026-07-17-inference-hard-reset-5x60.json`
- `docs/agent/run_logs/2026-07-17-inference-ui-isolation.md`
- `docs/agent/run_logs/2026-07-17-inference-ui-isolated-5x60.json`

## 未完成与现场验收清单

1. 做 5 次真正物理断电冷启动；当前 5 次 reset 是 CP2102N RTS，不可冒充断电。
2. 用真实包裹覆盖 camera→检测框→分类→S1/S2/S4→三路电机完整路径并拍照/录像；当前视觉页无包裹图。
3. 完成当前最终候选的 61 分钟真实 IO 长稳，观察是否出现 fatal 或 `dl_guard worker guard rejected`。
4. 在 LCD 上实际操作触摸与页面切换，确认 UI priority 3 没有明显手感回退。
5. 上述稳定后再处理 UVC。当前 DMA largest 仅 72 B，JPEG encoder `rxlink` 无法分配，尚无 USB 枚举/视频稳定性结果。

在完成这些项目之前，不创建 `stable` 标签，不删除失败证据，也不把自动重启后的恢复计为连续长稳通过。
