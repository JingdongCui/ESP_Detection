# Goal 交付与候选版本说明（2026-07-17）

## 推荐版本

### A. 当前推荐候选：生产推理 profile

- 固件分支：`goal/inference-and-device-control`
- 固件提交：`c26dba8`
- 标签：`backup/final-production-candidate-20260717`
- Host 提交：`6bcee3b`

包含完整控制闭环、真实 IO、重连、强化 ESP-DL target guard 和 UI/推理优先级隔离。默认 `CONFIG_SCREEN_UVC_ENABLE=n`，避免已量化的 USB device 推理回退；需要时可构建 UVC 专用 profile。

5×60 共 300 样本严格通过：P50/P95/max=67.312/71.393/132.003 ms，`>=150 ms` 和 `>=500 ms` 均为 0。任务快照确认 lvgl/swdraw/vision_disp=3/3/2、fetch/detect/dl_mc0/1=4。

最终恢复生产提交并重新下载原版 managed component 后，`idf.py build` 与精确全量 `idf.py flash monitor` 再次通过，boot/app/partition/storage 均 Hash verified。随后 1×60 严格通过：P50/P95/max=67.848/71.494/72.046 ms，零 `>=150/500 ms`。性能基线 61 分钟连续性通过，但 1048 样本仍有 8 个 150～218 ms 尖峰，因此它仍是 candidate，不是 stable；物理验收也未完成。

### B. UVC 可启动实验 profile

- 固件提交：`b0e7a02`
- 标签：`backup/uvc-starts-latency-regression-20260717`

提前预留 JPEG internal-DMA descriptor 后，板端首次成功启动 JPEG/UVC/TinyUSB，解决 `no memory for jpeg encoder rxlink`。但空闲 1×60 P95/max=394.671/486.401 ms，12 个 >=150 ms；仅用于接好 USB OTG 后继续枚举/拉流实验，不用于生产推理。

### B2. UVC Full-Speed PHY 修复实验 profile

- 固件提交：`838b0ed`
- 标签：`backup/uvc-fs-starts-latency-regression-20260717`

修复 `usb_device_uvc@1.3.1` 在 ESP32-P4 Full-Speed profile 下未显式设置 `USB_PHY_SPEED_FULL` 的问题。修复前，IDF 会把未定义 device speed 兼容性重定向到 UTMI HS PHY，和 TinyUSB RHPORT0/FS 不匹配，导致 stack init 失败；修复后板端能打印 `UVC Device Start`。

该版本同时把 JPEG engine 生命周期收窄到 UVC 主机真正 start/stop stream 之间，并用 1 KiB aligned internal-DMA block 保护晚期 descriptor 分配。但即使主机未打开流、JPEG engine 从未创建，FS UVC 启动后推理仍约 463 ms；因此它只证明 FS PHY 修复和板端启动，不是生产候选，且 Full-Speed 带宽低于原 HS profile。

### C. 长稳已通过的 guard 候选

- 固件提交：`56a53fd`
- 标签：`backup/dl-guard-61min-pass-candidate-20260717`

完成一轮 3660.192 s 连续测试，无崩溃/重启/连接失败；适合在怀疑 UI 优先级调整时回退。限制是 825 样本最大 162.326 ms，超过 goal 的 150 ms 门槛，且 guard 只验证 vtable 指针，没有 `b08a1a3` 的实际虚调用目标校验。

### D. 保守功能候选：控制/ISP/重连

- 固件提交：`b1dfef5`
- 标签：`backup/control-json-isp-reconnect-candidate-20260717`

包含推理恢复与完整设备控制闭环，已经过正向、负向、畸形 JSON、状态恢复、ISP、restart 及双端口重连实板测试。不包含后续 task/heap 快照、长稳采集器和 ESP-DL guard，适合在怀疑 guard 改动时作功能回退。

限制：后续长稳测试证明同一代码链的未加 guard 版本可能在约 40 分钟发生 ESP-DL worker Instruction access fault，因此不能作为长稳版本。

### E. 最小推理修复候选

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
| `backup/ui-inference-isolation-61min-pass-candidate-20260717` | `60c9f8a` | UI 隔离长稳基线 | 连续性通过；1048 样本 max 217.760 ms |
| `backup/dl-core-timing-diagnostic-20260717` | `21ec574` | ESP-DL 双核 dispatch 细分 | 生产 20 分钟未复现尾部；UVC 下确认双 worker 立即唤醒但执行同步膨胀 |
| `backup/uvc-fs-starts-latency-regression-20260717` | `838b0ed` | FS PHY 修复及 UVC 启动 | 启动成功，但空闲推理约 463 ms，禁止生产使用 |

## 已完成测试

- 固件 `idf.py build` 通过；完整 `idf.py flash monitor` 与后续 app-flash 均 Hash verified。
- CONTROL_JSON：全部设置项、能力、非法 key/JSON/type/range/step、恢复、restart、5000/5001 重连实板通过。
- Host：CMake build、CTest 1/1、原生 Qt 四页可见检查通过；UI detection false→true 闭环有截图与日志。
- 推理：5×60 共 300 样本；另有 1×100 CPU/wait 诊断；两轮各 61 分钟长稳原始日志均保留。
- 当前推荐候选长稳：3660.192 s，825 样本，启动 1、fatal 0、guard rejection 0、连接失败 0；60 分钟末 24/24 task 与 heap integrity=ok。
- 最终 UI 隔离候选 5×60：300 样本 P50/P95/max=67.312/71.393/132.003 ms，全部严格门槛通过。
- UI 隔离 61 分钟：3660.179 s、boot 1、fatal/guard/连接失败均 0、末尾 task/heap 完整；1048 样本 P50/P95/max=67.308/74.273/217.760 ms，8 个 >=150 ms、0 个 >=500 ms。
- UVC 专用 profile：JPEG/UVC/TinyUSB 启动成功；空闲 1×60 性能回退已量化并保存。
- ESP-DL core timing：生产 20 分钟 353 个抽样日志，max 72.506 ms、零 >=150 ms；UVC HS 5 分钟中 203 个 slow dispatch 的 worker wake 平均仅 50.6/22.2 us，而 exec 平均 85.146/80.036 ms，排除普通任务唤醒/优先级竞争为 UVC 回退主因。
- UVC FS 对照：未修 PHY 时 stack init 失败但早期 JPEG engine 已使 max 217.844 ms；显式 Full-Speed PHY 后 UVC stack 启动成功，但未创建 JPEG engine 时推理仍约 463 ms。综合说明 JPEG engine 与 USB Device controller 都会独立触发当前 ESP-DL/PSRAM 性能冲突。
- 最终生产 v2：重新执行 `idf.py build`、全量 `idf.py flash monitor` 和 1×60；P50/P95/max=67.848/71.494/72.046 ms，四项严格门槛全通过。
- Python 工具 `py_compile` 通过；长稳 JSON 结构、样本数、快照数、连接和 fatal/rejection 断言通过。

详细证据见：

- `docs/agent/run_logs/2026-07-17-stability-guard.md`
- `docs/agent/run_logs/2026-07-17-stability.md`
- `docs/agent/run_logs/2026-07-17-visual.md`
- `docs/agent/run_logs/2026-07-17-inference-hard-reset-5x60.json`
- `docs/agent/run_logs/2026-07-17-inference-ui-isolation.md`
- `docs/agent/run_logs/2026-07-17-inference-ui-isolated-5x60.json`
- `docs/agent/run_logs/2026-07-17-final-stability-and-uvc.md`
- `docs/agent/run_logs/2026-07-17-dl-core-timing-20min.json`
- `docs/agent/run_logs/2026-07-17-dl-core-timing-uvc-5min.json`
- `docs/agent/run_logs/2026-07-17-dl-core-timing-uvc-fs-5min.json`
- `docs/agent/run_logs/2026-07-17-final-production-v2-latency-1x60.json`

## 未完成与现场验收清单

1. 做 5 次真正物理断电冷启动；当前 5 次 reset 是 CP2102N RTS，不可冒充断电。
2. 用真实包裹覆盖 camera→检测框→分类→S1/S2/S4→三路电机完整路径并拍照/录像；当前视觉页无包裹图。
3. 在 LCD 上实际操作触摸与页面切换，确认 UI priority 3 没有明显手感回退。
4. 接好 USB OTG，继续 UVC 主机枚举、MJPEG 拉流、画质/帧率和视频稳定性；板端 rxlink 内存和 FS PHY 选择问题已解决，但启用任一 USB Device controller 都会造成显著推理回退。
5. 若要求长稳全样本 max<=150 ms，继续诊断剩余 8 个高相关 wait 尖峰。

在完成这些项目之前，不创建 `stable` 标签，不删除失败证据，也不把自动重启后的恢复计为连续长稳通过。
