# Current Task

## Goal

按根目录 `goal.md` 恢复单模型约 70 ms 推理，并在稳定基线上完成上位机设备控制的板端协议。

## Current State

- 已在根目录建立 `goal.md`，记录实测数据、已排除项、A/B 调查矩阵、PSRAM 原则和验收标准。
- Ethernet RX10/TX6 可成功 Link Up、取得 192.168.10.2，并连接队友上位机。
- 当前 waybill/logo 单阶段各约 500～600 ms，级联 wall time 约 1.1～1.25 s。
- UVC 仍因 JPEG DMA 描述符内部内存不足而启动失败，列为推理恢复后的后续问题。
- 上位机第三页及 `CONTROL_JSON 0x11` 主机侧已完成，提交为 `6bcee3b`，build/CTest/offscreen 启动检查通过。
- 板端尚未处理 `0x11`，显示、ISP、检测、三路电机、上报开关和重启均待板端实现与联调。
- `goal.md` 已补充严格的分支、checkpoint、revert 和 stable tag 多 Git 备份制度。

## Next Step

- 按 `goal.md` 运行启动矩阵，优先隔离 `sort_real_io` priority 4/10 ms 轮询与 ESP-DL 双核 worker 的竞争。
- 撤销或重新论证已证实无效的 EMAC RX 降优先级实验。
- 推理稳定后实现板端 CONTROL_JSON；完成控制联调后再处理 UVC。

## Blockers

- 板端当前仍有另一个 agent 的五个未提交实验文件；开始 goal 时必须先等待其提交或建立明确的保留现场 checkpoint。
