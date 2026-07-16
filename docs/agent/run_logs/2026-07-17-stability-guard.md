# ESP-DL dispatch guard 61 分钟长稳结果

## 版本与测试窗口

- 固件提交：`56a53fd`（guard 实现为 `537cb8e`，采集器计数补充为 `56a53fd`）
- 回退标签：`backup/dl-guard-61min-pass-candidate-20260717`
- 测试窗口：2026-07-17 03:05:41 ～ 04:06:42（Asia/Shanghai）
- 连续时长：3660.192 s
- 原始证据：`2026-07-17-stability-guard-61min.json`、`2026-07-17-stability-guard-61min.log`

## 结论

本轮连续 61 分钟测试通过“无崩溃、无重启、双 TCP 通道持续在线、60 分钟末快照完整”的长稳检查：启动标记 1 次、连接检查失败 0 次、fatal 0 条、guard rejection 0 条。此前未加 guard 的版本在 2412.116 s 发生 `dl_mc0` Instruction access fault；本轮越过该窗口并跑满，但单轮通过不能证明 guard 已修复根因，且本轮没有触发 guard rejection，因此该标签只能标为 candidate，不能标 stable。

## 推理统计

共 825 个 `wb_only` 样本：

| 指标 | P50 | P95 | 最大值 |
|---|---:|---:|---:|
| waybill | 77.625 ms | 99.809 ms | 162.326 ms |
| 调用任务 CPU | 25.019 ms | 27.718 ms | 30.227 ms |
| wait | 51.906 ms | 74.210 ms | 137.507 ms |

- `>=150 ms`：17 个；`>=500 ms`：0 个。
- `corr(wb, wait)=0.9960`，`corr(wb, CPU)=0.1872`，尾延迟仍主要随 worker/调度等待变化。
- 前半程 P50/P95/max=`77.469/99.591/158.879 ms`，后半程=`77.943/100.136/162.326 ms`；未见随运行时间持续劣化，但后半程 P95 略超 100 ms。
- Goal 的 P50、总体 P95 和零 500 ms 条件通过；最大值 <=150 ms 条件失败，因此推理严格验收仍未完成。

## 连接、堆与任务水位

- 715 次连接检查全部看到 5000/5001 为 ESTABLISHED；最后一次在 3658.869 s。
- 启动与 3602 s 的 heap integrity 均为 `ok`。
- heap free：6,835,639 → 6,836,467 B；minimum 6,809,751 → 6,809,687 B，没有持续泄漏证据。
- internal free：27,315 → 26,403 B；PSRAM free：6,809,072 → 6,809,152 B。
- DMA largest 始终只有 72 B，UVC JPEG `rxlink` 连续 DMA 分配阻塞仍未解决。
- 两次均完整枚举 24 个任务。60 分钟末关键应用任务最低剩余：`dl_mc0/1=880 B (49.3%)`、`vision_det=8524 B (71.6%)`、`sort_real_io=1772 B (47.4%)`、`isp_task=1256 B (33.8%)`、`vision_disp=1256 B (33.8%)`、`sysmon=1712 B (43.4%)`。
- 所有应用任务在本轮末均保持 >=512 B 且 >=20%；系统内部 `ipc0` 为 472 B，但未低于其启动水位，也不是本轮新建应用任务。
- `vision_det`、`swdraw`、`vision_fetch`、`eth_img_prod` 等仍有 >60% 余量；在物理最坏路径、真实包裹/电机/UVC 未覆盖前不缩栈。

## 保留限制

- 本轮只是一轮 61 分钟，不等于 goal 要求的完整 60～120 分钟多场景稳定性结论。
- 未完成 5 次物理断电冷启动；此前 5 次是 CP2102N RTS reset。
- 未用真实包裹触发相机画面、检测框与完整分拣机械路径，也没有现场照片。
- UVC 仍因 DMA 最大连续块不足失败。
- Guard 仅在检测到 dispatch 指针/补码/vtable 不一致时拒绝执行；本轮 0 次拒绝，无法证明上一次 0x10 跳转的来源已消除。
