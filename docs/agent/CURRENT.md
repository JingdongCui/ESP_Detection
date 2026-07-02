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
- 本轮已核对真实硬件链路和以太网模拟链路：
  - 两者核心调度器一致，最终都进入 `sorter_scheduler_*`，电机命令也统一走 `scheduler_send()` / `apply_motor_command_line()`。
  - 真实链路：S1-S4 GPIO 10 ms 轮询、20 ms 防抖；S1 打开视觉窗口；本地视觉 `vision_app.cpp` 在检测到类别时调用 `sorting_sim_control_submit_vision_class()`；无识别结果时由 S2 到达或视觉超时触发失败分配。
  - 以太网模拟：`VISION_FRAME`/`VISION_RESULT`/`SENSOR`/`DISTANCE` 文本协议进入同一个控制层和调度器。
  - 主要不一致风险：真实 GPIO 有电平/防抖/时序抖动；真实视觉可能晚于 S2；传感器真实链路不带 package id；编码器未接时真实 C 段不能依赖 `DISTANCE`，会走超时 fallback。
- 已修改识别失败分配：
  - `SORTER_CLASS_VISION_FAILED` 和直接传入的 `SORTER_CLASS_UNKNOWN` 现在进入调度器时按 class1、class2、class3、class1... 轮番改写为实际类别。
  - 视觉超时 `timeout_vision` 也使用同一个轮转分配。
- 本轮新增板端调试显示：
  - `sorting_sim_control_get_runtime_debug()` 提供调度器运行快照。
  - dashboard 的 SYS/debug 面板右下角显示：活动包裹数、最大槽位、下一个包裹 ID、下一次识别失败分配类别、当前视觉窗口、S1 视觉状态、B/C 段占用。
  - 同一区域显示最多 8 个活动包裹：`#id belt state class pos`，用于观察包裹状态机是否符合预期。
- 验证：
  - `python -m unittest esp32_sorter_sim_py.tests.test_sorter_scheduler_c`：通过。
  - `git diff --check`：通过。
  - `cd /home/kazeform/2026esp/merge_project && idf.py build`：通过；仍仅有既有 `global_statusbar` unused warning。
  - `timeout 120s idf.py flash monitor`：失败，当前环境无可用串口；`/dev/ttyS*` 均无法打开。

## Immediate Next Step

1. 连接真实 ESP32-P4 设备串口后，在 `merge_project` 执行 `idf.py flash monitor`。
2. 切到 SYS 页面，确认 debug 面板右下角的 `PACKAGES` 区域正常刷新，不遮挡按钮。
3. 切到 REAL/SENSOR ON 模式，逐个触发 S1-S4，观察 UI 状态块、`PACKAGES` 列表和 `sort sensor Sx ...` 日志是否同步变化。
4. 实机验证失败分配：连续制造 4 次无识别/识别失败，确认 UI `FAIL->` 轮转、包裹 `class` 和对应出口一致。
5. 若对照以太网模拟，编码器未接时不要用上位机 `DISTANCE` 作为成功依据；应让 C 段走 timeout fallback，才更接近真实硬件。
6. 若某路 UI 不变但日志有 raw change，继续查 active level/防抖/调度输入；若日志也无 raw change，优先查 GPIO 号、线序、电平和下拉配置。

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
