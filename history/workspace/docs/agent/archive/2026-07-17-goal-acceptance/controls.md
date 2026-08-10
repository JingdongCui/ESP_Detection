# Host—板端控制闭环验收

## 已验证的软件与实板闭环

- `get`、全部约定 `set`、`device.restart`，以及 state/capabilities/error 回包。
- 屏幕亮度通过 LVGL/UI 上下文调度；四项 ISP、两个自动开关、检测、叠框、两个阈值、A/B/C 速度、图像/指标上报均接入真实板端函数。
- 未知 key、错误 JSON、错误类型、越界、错误 step、不支持项均返回 error 且不改变状态；后续合法请求仍能执行。
- metrics 关闭后 control 保持可用并可重新开启；restart 后 5000/5001 恢复并重新同步 state。
- Host 原生窗口中完成 detection false→true 可逆操作，维护页日志与板端状态闭环一致。

## 证据

- 实板工具：`ESP32P4_Detection/tools/control_protocol_integration.py`。
- Host 协议测试：CTest 1/1 通过。
- 截图与说明：本目录 `screenshots/` 及 `docs/agent/run_logs/2026-07-17-visual.md`。
- 功能回退点：`backup/control-json-isp-reconnect-candidate-20260717`。

## 未完成的物理观测

- LCD 亮度真实视觉变化。
- ISP brightness/contrast/saturation/hue 对预览画面的实际影响。
- 三路 PWM 对三台实体电机分别改变且不串路。
- 阈值在边界真实包裹上的检出变化，以及叠框在板端/Host 画面的真实隐藏与恢复。

所以控制协议和业务函数链路通过，但 goal 要求的全部“实机动作/观测”仍未通过。
