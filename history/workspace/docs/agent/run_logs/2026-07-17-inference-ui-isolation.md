# 推理尾延迟与 UI 优先级隔离验证（2026-07-17）

## 结论

最终候选 `60c9f8a` 已通过 5 次 RTS 硬复位、每次预热后 60 个样本的严格单阶段推理验收。300 个样本全部满足 `goal.md`：P50 67.312 ms、P95 71.393 ms、最大 132.003 ms，`>=150 ms` 与 `>=500 ms` 均为 0。

本轮定位到剩余 150 ms 尾延迟的直接调度机制：`vision_disp` 在 LVGL mutex 内执行两个阻塞 PPA framebuffer blit；priority 5 的 LVGL task 等待同一 mutex 时，FreeRTOS 优先级继承会把 `vision_disp` 临时提升到 priority 5，高于 priority 4 的 ESP-DL worker。实机任务快照曾直接捕获 `vision_disp` 的动态优先级为 5，且相邻样本出现 146.816 ms。只降低 `vision_disp` 基础优先级不能阻断继承。

最终候选把整个 UI render group 保持在推理 worker 下方：LVGL=3、swdraw=3、vision_disp=2；vision_disp 持锁时最多继承到 3，fetch/detect/dl_mc0/dl_mc1 保持 4。启动任务快照确认实际优先级分别为 `lvgl=3`、`swdraw=3`、`vision_disp=2`、`vision_fetch=4`、`vision_det=4`、`dl_mc0/1=4`。

## 可回退版本与 A/B 结果

| 版本 | 标签 | 300 样本 P50 / P95 / max | `>=150 ms` | 结论 |
|---|---|---:|---:|---|
| 原始 guard 候选 `56a53fd` | `backup/dl-guard-61min-pass-candidate-20260717` | 74.443 / 101.947 / 168.072 ms | 5 | P95/max 失败 |
| 仅降低预览基础优先级 `a928dde` | `backup/preview-priority-tail-ab-20260717` | 76.465 / 98.429 / 155.593 ms | 2 | 分布改善，max 失败 |
| 对齐 swdraw 默认优先级 `f29525d` | `backup/ui-worker-priority-tail-ab-20260717` | 未独立跑满 300 | — | 快照揭示 mutex 优先级继承 |
| UI 组完整隔离 `60c9f8a` | `backup/ui-inference-isolation-candidate-20260717` | 67.312 / 71.393 / 132.003 ms | 0 | 严格通过，当前推荐 |

`b08a1a3`（标签 `backup/dl-target-guard-candidate-20260717`）还将 ESP-DL worker guard 从“只校验 vtable 指针”增强为校验实际 `forward_args` 虚调用目标可执行性，并由 worker 重新解析、直接调用已验证目标。ELF 反汇编确认调用路径使用校验后的目标；这是针对既往 MEPC/RA/MTVAL=0x10 故障路径的止损增强，不宣称已消除造成对象损坏的上游根因。

## 五轮结果

| 轮次 | P50 | P95 | max | `>=150 ms` |
|---:|---:|---:|---:|---:|
| 1 | 67.154 ms | 71.501 ms | 81.755 ms | 0 |
| 2 | 67.280 ms | 71.980 ms | 122.134 ms | 0 |
| 3 | 67.718 ms | 71.217 ms | 72.111 ms | 0 |
| 4 | 67.133 ms | 70.397 ms | 80.151 ms | 0 |
| 5 | 67.295 ms | 72.815 ms | 132.003 ms | 0 |

原始结构化数据：

- `2026-07-17-inference-preview-priority-5x60.json`
- `2026-07-17-inference-ui-isolated-5x60.json`

## 验证边界

- 复位是 CP2102N RTS，不是真正断电冷启动。
- 优先级修改后一次推理约 70 ms，理论上会让最坏触摸/重绘延后一个推理窗口；串口与任务快照无异常，但 LCD 触摸手感仍需现场操作确认。
- 物理包裹、检测框、S1/S2/S4 与三路电机完整路径仍需拍照/录像验收。
- UVC 仍被内部 DMA 最大连续块仅 72 B、JPEG encoder rxlink 分配失败阻塞，未纳入本轮修改。
