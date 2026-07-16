# History

- 上位机打包与 Dolphin 修复阶段已归档到 `docs/agent/archive/2026-07-16-host-package-dolphin-repair.md`。
- 2026-07-16：记录推理延时回退 goal。上位机连接状态下实测 waybill 约 553～619 ms、logo 约 515～594 ms、级联 wall 约 1.10～1.25 s，wait 约 0.90～0.96 s。
- 已知 Ethernet 应用任务与 ISP worker 栈可放 PSRAM；推理栈不可放 PSRAM。TCP 重连及 EMAC RX 优先级已基本排除，下一步优先验证真实 IO 任务与 ESP-DL worker 的同优先级竞争。
- 2026-07-16：上位机设备控制第三页与 CONTROL_JSON 主机侧完成，Host checkpoint=`c85829d`、功能提交=`6bcee3b`；板端协议因并发 agent 未提交实验而尚未开始。
- 2026-07-16：扩展 `goal.md`，将未完成的板端控制协议、字段/函数映射、联调标准和多 Git checkpoint/tag/revert 规则纳入后续 goal。
- 2026-07-16：记录队友的 JTAG/RTT 栈水位数据。19 个已枚举任务合计约 87.48 KiB内部栈；RTT #5020 中 dl_mc0/dl_mc1 CPU约 38%/37%。快照与当前 PSRAM 栈实验的版本对应关系待用 ELF SHA复核。
- 2026-07-17：完善 `goal.md` 验收协议。推理要求5次冷启动、预热后300+样本、单阶段P50 60～85 ms/P95<=100 ms/max<=150 ms且零500 ms；增加全任务栈地址/水位/内存归属、Host—板端控制、连接恢复、真实IO、60分钟至2小时长稳和截图证据要求。
