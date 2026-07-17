# Current Task

## State

2026-07-17 goal 实现、测试、候选版本与备份阶段已归档到 `docs/agent/archive/2026-07-17-goal-inference-control-uvc.md`。

## Recommended Baseline

- Firmware: `c26dba8`, tag `backup/final-production-candidate-20260717`
- Host: `6bcee3b`
- Production config: `CONFIG_SCREEN_UVC_ENABLE=n`

## Remaining External Validation

1. 五次真实物理断电冷启动。
2. LCD 触摸/亮度/ISP 画面和真实包裹、传感器、三路电机照片/录像。
3. 接入 ESP32-P4 USB OTG 后完成 UVC 主机枚举与拉流；实验 profile 有明确推理回退，不可代替生产版。
4. 若要求 61 分钟全样本 max<=150 ms，继续调查 8 个 150～218 ms wait 尖峰。

## Blockers

- 当前环境不能代替用户完成上述物理观察。
- 当前电脑未接入或未枚举 ESP32-P4 USB OTG。
- 因此不创建 `stable` 标签；推荐版本保持 candidate。
