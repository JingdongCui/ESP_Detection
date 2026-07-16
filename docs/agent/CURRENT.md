# Current Task

## Goal

恢复合入队友板端代码后的单模型推理延时：当前约 500～600 ms，目标约 70 ms。

## Current State

- 已在根目录建立 `goal.md`，记录实测数据、已排除项、A/B 调查矩阵、PSRAM 原则和验收标准。
- Ethernet RX10/TX6 可成功 Link Up、取得 192.168.10.2，并连接队友上位机。
- 当前 waybill/logo 单阶段各约 500～600 ms，级联 wall time 约 1.1～1.25 s。
- UVC 仍因 JPEG DMA 描述符内部内存不足而启动失败，列为推理恢复后的后续问题。

## Next Step

- 按 `goal.md` 运行启动矩阵，优先隔离 `sort_real_io` priority 4/10 ms 轮询与 ESP-DL 双核 worker 的竞争。
- 撤销或重新论证已证实无效的 EMAC RX 降优先级实验。

## Blockers

- 无；需要继续实机 A/B 和串口计时。
