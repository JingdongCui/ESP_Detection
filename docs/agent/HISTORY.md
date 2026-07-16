# History

- 上位机打包与 Dolphin 修复阶段已归档到 `docs/agent/archive/2026-07-16-host-package-dolphin-repair.md`。
- 2026-07-16：记录推理延时回退 goal。上位机连接状态下实测 waybill 约 553～619 ms、logo 约 515～594 ms、级联 wall 约 1.10～1.25 s，wait 约 0.90～0.96 s。
- 已知 Ethernet 应用任务与 ISP worker 栈可放 PSRAM；推理栈不可放 PSRAM。TCP 重连及 EMAC RX 优先级已基本排除，下一步优先验证真实 IO 任务与 ESP-DL worker 的同优先级竞争。
- 2026-07-16：上位机设备控制第三页与 CONTROL_JSON 主机侧完成，Host checkpoint=`c85829d`、功能提交=`6bcee3b`；板端协议因并发 agent 未提交实验而尚未开始。
- 2026-07-16：扩展 `goal.md`，将未完成的板端控制协议、字段/函数映射、联调标准和多 Git checkpoint/tag/revert 规则纳入后续 goal。
- 2026-07-16：记录队友的 JTAG/RTT 栈水位数据。19 个已枚举任务合计约 87.48 KiB内部栈；RTT #5020 中 dl_mc0/dl_mc1 CPU约 38%/37%。快照与当前 PSRAM 栈实验的版本对应关系待用 ELF SHA复核。
- 2026-07-17：完善 `goal.md` 验收协议。推理要求5次冷启动、预热后300+样本、单阶段P50 60～85 ms/P95<=100 ms/max<=150 ms且零500 ms；增加全任务栈地址/水位/内存归属、Host—板端控制、连接恢复、真实IO、60分钟至2小时长稳和截图证据要求。
- 2026-07-17：固件建立 `goal/inference-and-device-control`；保存队友实验 `30aa3cf`，撤销无效 EMAC RX 调度实验 `e95058d`。
- A/B 关闭整个 sorter 后推理恢复 66～84 ms；进一步确认只关闭 `sort_dbg` USB Serial/JTAG monitor、保留真实传感器/电机时同样恢复到约 70 ms。根因锁定为调试监视任务干扰 ESP-DL 双核 worker，而非真实 IO 轮询。
- 生产修复 `6ad4fd5`：`SORTER_HARDWARE_DEBUG_MONITOR=0`；标签 `backup/inference-70ms-candidate-20260717`。多数实测 65～77 ms，观察到少量 145/152 ms，未见 500 ms，正式 300 样本统计待完成。
- `0cc3beb` 实现板端 CONTROL_JSON 0x11，覆盖显示、ISP、视觉、阈值、三路电机、上报开关和 restart；严格校验非法 JSON/类型/范围/step。
- `9da0e7c` 新增实板协议测试器；首次测试揭示图像 socket 空闲不检查 FIN。`88ce66f` 增加非阻塞 `MSG_PEEK`，连续双通道重连通过。
- ISP 正向测试发现仓库的 `sc2336_ui_p4_eco4.json` 从未被 Kconfig 选中；`6ded3af` 切到自定义配置并修正 brightness manual target。自定义 JSON 与官方 ECO4 默认参数只有追加 UI override 一处差异。
- 三轮完整 CONTROL_JSON 测试通过；另一次 `--restart` 验证 restart state ack、板端完整重启、5000/5001 重连和重启后 get 均通过。测试均恢复原值，异常路径增加 best-effort restore。
- `88ce66f` 后串口明确记录 `TCP image peer closed while idle`；Host 重启后 5000/5001 均重新 ESTABLISHED。
- 固件 build、全量 flash 和 app-flash 通过 Hash 校验；Host build 与 packetprotocol_tests 1/1 通过。候选标签 `backup/control-json-isp-reconnect-candidate-20260717` 指向 `b1dfef5`。
