# Current Task

## Goal

进入 `merge_project` 阶段：进行算法改进，并在真实硬件链路上调试验证。

## Current State

- 工程：`merge_project`
- 任务起点：上一阶段蓝屏/相机链路诊断已结束，相关过程已归档到 `docs/agent/archive/2026-06-29-blue-screen-camera-diagnostics.md`。
- `merge_project` 既有未提交改动已在本轮修改前提交为 checkpoint：`aa169a8 checkpoint before sensor ui debug`。
- `bug_project` 暂不作为当前主工程，保留其蓝屏诊断结论供硬件链路问题参考。
- 本轮已检查并增强 `merge_project` 传感器链路：
  - BSP 确认从 GPIO 读取 S1-S4 电平，并新增原始电平读取接口。
  - 实时 IO 任务轮询 S1-S4，输出初始化、原始电平跳变、防抖后稳定跳变日志。
  - UI 调试面板已改为 S1-S4 四个有色状态块，ON 绿色，OFF 深色/无色，invalid 显示 `--`。
- `idf.py build` 已通过。
- `idf.py flash monitor` 未执行成功：当前环境无可用串口，自动扫描的 `/dev/ttyS*` 均打不开。

## Immediate Next Step

1. 连接真实 ESP32-P4 设备串口后，在 `merge_project` 执行 `idf.py flash monitor`。
2. 切到 REAL/SENSOR ON 模式，逐个触发 S1-S4，观察 UI 状态块和 `sort sensor Sx ...` 日志是否同步变化。
3. 若某路 UI 不变但日志有 raw change，继续查 active level/防抖/调度输入；若日志也无 raw change，优先查 GPIO 号、线序、电平和下拉配置。

## Verification Standard

- 代码修改后必须运行相关检查。
- 优先执行：

```bash
cd /home/kazeform/2026esp/merge_project
idf.py build
idf.py flash monitor
```

- 如果无法执行实机验证，必须在 `HISTORY.md` 记录原因。
- 每轮硬件调试记录 commit、配置差异、串口日志、用户肉眼现象和失败原因。

## Blockers

- 当前环境没有可用串口，无法完成烧录和 monitor 实机验证。
