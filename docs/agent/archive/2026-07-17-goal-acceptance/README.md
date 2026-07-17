# Goal 验收证据索引（2026-07-17）

## 结论

当前结论为 **软件候选已交付，goal 总验收未通过**。推荐固件是 `c26dba8`（标签 `backup/final-production-candidate-20260717`），Host 是 `6bcee3b`；生产配置为 `CONFIG_SCREEN_UVC_ENABLE=n`。未创建 `stable` 标签。

## 版本与构建产物

- 根仓库：`4bd95bb0f45adbdee551cba4f6b530aee199c29f`（本目录提交后会前移）。
- 固件仓库：`c26dba8427651fa04da004beb0211eb5eb1e6920`。
- Host 仓库：`6bcee3b83a543d94aab65d6952ef7f907daba552`。
- 最终构建 ELF：`ESP32P4_Detection/build/sample_project.elf`，SHA256 `04c0b56bca60f58374fc327c54374b89f3e12de65a6627fd32773233e7c58d8c`。
- 最终 app bin：SHA256 `61a559bc52c85b82fb808e709cd4944e5ee07bac6f3ea9fe8a4f766af38f5d52`。
- 串口：CP2102N `10c4:ea60`，序列号 `7ee2f3966ac3ee11be78b90f9e1b1c54`，稳定路径见交付文档。
- 最终 `idf.py build` 通过；全量 `idf.py flash monitor` 的 bootloader、partition、app、storage 均 Hash verified。
- Host CMake build、CTest 1/1、原生 Qt 四页检查通过。

## 本目录内容

- `inference.csv`：严格 5×60 的 300 个 waybill-only 样本；列中显式标记没有 logo/真实包裹负载。
- `tasks.csv`：实板 24 个任务的栈地址、水位和内存归属。
- `memory.csv`：同次快照的 heap/internal/DMA/PSRAM 数据。
- `connection.md`：现有连接与恢复证据以及缺失的物理循环。
- `controls.md`：控制闭环测试范围与尚需现场观察的动作。
- `stability.md`：61 分钟测试、性能门槛和失败边界。
- `screenshots/`：Host 原生窗口截图副本及说明；没有板端 LCD/真实包裹实拍。

## 未通过的硬门槛

1. 5 次启动是 CP2102N RTS reset，不是物理断电冷启动。
2. 300 样本是 `wb_only`，没有真实包裹触发的 waybill+logo 级联及六类规定负载覆盖。
3. 61 分钟全量样本最大值 217.760 ms，超过 150 ms；有 8 个 `>=150 ms`，虽然没有 `>=500 ms`。
4. 未执行 Host 前/后启动各 5 次、Host 重开 10 次、网线拔插 5 次、远程重启 5 次的完整矩阵。
5. 未实测 S1/S2/S4 各 50 次、编码器与三路电机机械闭环。
6. 未取得 LCD 触摸/亮度/ISP/检测框/真实包裹/电机照片或录像。
7. 当前电脑只枚举 CP2102N 串口，未枚举 ESP32-P4 USB OTG；UVC 主机拉流未验收。

因此，此目录是可复核的“已做/未做”证据索引，不是通过证书。完整候选矩阵见 `docs/goal-delivery-2026-07-17.md`，原始材料见 `docs/agent/run_logs/`。
