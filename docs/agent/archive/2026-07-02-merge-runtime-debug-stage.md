# 2026-07-02 Merge Runtime Debug Stage

## Goal

- 进入 `merge_project` 阶段后，检查并增强真实分拣链路的传感器、失败分类、电机调度可观测性。
- 为后续现场调试提供 UI 与日志层面的内部状态显示。

## Work Completed

- 传感器链路：
  - BSP 通过 GPIO 读取 S1-S4，并新增 `bsp_sort_sensor_get_level()`，同时暴露原始电平和 active 状态。
  - `sorting_sim_control.c` 的真实 IO 任务每 10 ms 轮询 S1-S4，20 ms 防抖后送入调度器。
  - 增加初始化、raw change、stable change 日志，便于区分 GPIO、active level、防抖和调度输入问题。
- UI 调试：
  - dashboard SYS/debug 面板改为显示 S1-S4 四个状态块。
  - ON 为绿色，OFF 为深色，invalid 显示 `--`。
- 真实链路与以太网模拟链路对齐：
  - 两者共同进入 `components/Sorter_app/sorting_sim_control.c` 和 `sorter_core/sorter_scheduler.c`。
  - 真实链路由 GPIO 传感器与本地视觉触发；以太网模拟由 `VISION_FRAME`、`VISION_RESULT`、`SENSOR`、`DISTANCE` 文本协议触发。
  - 记录主要差异：真实 GPIO 有电平、防抖、线序、时序抖动；真实视觉可能晚于 S2；真实传感器事件不带 package id；编码器未接时不能依赖 `DISTANCE`。
- 失败识别分配：
  - `SORTER_CLASS_VISION_FAILED` 和 `SORTER_CLASS_UNKNOWN/class=none` 进入调度器时按 class1、class2、class3 轮番分配。
  - 视觉超时 `timeout_vision` 使用同一个轮转游标。
- 运行态内部信息显示：
  - 新增 `sorting_sim_control_get_runtime_debug()`，从调度器复制只读快照。
  - dashboard SYS/debug 面板右下角显示活动包裹数、最大槽位、下一个 package id、下一次失败分配类别、当前视觉窗口、S1 视觉状态、B/C owner。
  - 包裹列表最多显示 8 个活动包裹，格式为 `#id belt state class pos`。

## Verification

- `python -m unittest esp32_sorter_sim_py.tests.test_sorter_scheduler_c`：通过。
- `git diff --check`：通过。
- `cd /home/kazeform/2026esp/merge_project && idf.py build`：通过；仅有既有 `global_statusbar` unused warning。
- `timeout 120s idf.py flash monitor`：失败，当前环境无可用串口；`/dev/ttyS*` 均无法打开。

## Remaining Risks

- 真实 ESP32-P4 串口未连接，实机烧录和 monitor 尚未完成。
- 需要现场确认 SYS/debug 面板不遮挡按钮，S1-S4 状态块与真实传感器、电机动作、包裹状态机同步。
- 编码器未接时，上位机模拟发送 `DISTANCE` 不能代表真实硬件路径；真实 C 段完成应重点观察 timeout fallback。

