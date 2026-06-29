# History

## 2026-06-29 Start Merge Project Algorithm And Hardware Link Debug

- 用户确认上一任务已经结束，要求整理并归档所有相关文档。
- 已将上一阶段 `CURRENT.md` 和 `HISTORY.md` 完整归档到：
  - `docs/agent/archive/2026-06-29-blue-screen-camera-diagnostics.md`
- 新阶段目标：
  - 对 `merge_project` 进行算法改进。
  - 在真实硬件链路上调试验证。
- 初始仓库状态：
  - 根目录原 `.git` 为空目录，已初始化根目录 git 仓库并提交归档前文档 checkpoint：`b1ff9eb checkpoint agent docs before merge project phase`。
  - `bug_project` 工作树干净。
  - `merge_project` 有既有未提交改动，后续必须先识别这些改动并避免误覆盖。
- 本次只整理文档，没有修改固件代码。

## Not Run

- 未运行 `idf.py build`：本次仅文档归档和任务切换。
- 未运行 `idf.py flash monitor`：本次未修改固件，也未进入实机测试阶段。

## 2026-06-29 Merge Project Sensor Chain And UI Debug

- 修改前按项目要求在 `merge_project` 提交既有未提交改动 checkpoint：
  - `aa169a8 checkpoint before sensor ui debug`
- 传感器链路检查结论：
  - `components/bsp/bsp_sort_sensor.c` 原先已经通过 `gpio_get_level()` 读取 S1-S4，并按 active level 转为 active。
  - `components/Sorter_app/sorting_sim_control.c` 的实时 IO 任务每 10 ms 轮询 S1-S4，20 ms 防抖，稳定后才进入调度器。
  - UI 原先只显示一行 `S2/S3/S4` 文本，且使用的状态数组下标实际从 S1 开始，显示不够清晰。
- 代码改动：
  - 新增 `bsp_sort_sensor_get_level()`，同时返回 GPIO 原始 0/1 电平和 active 状态。
  - 实时 IO 任务改用原始电平接口，并输出初始化、raw change、stable change 日志。
  - UI 调试面板改成 S1-S4 四个状态块：ON 绿色，OFF 深色/无色，invalid 为 `--`。
- 验证：
  - `cd /home/kazeform/2026esp/merge_project && idf.py build`：通过；仅保留既有 `global_statusbar` unused warning。
  - `timeout 120s idf.py flash monitor`：失败，当前环境无可用串口；`/dev/ttyS*` 均无法打开。

## 2026-06-29 Link Alignment And Vision-Failure Assignment

- 核对真实硬件链路和以太网模拟链路：
  - 真实链路和以太网模拟链路都进入 `components/Sorter_app/sorting_sim_control.c` 和 `sorter_core/sorter_scheduler.c`。
  - 真实链路的传感器来源是 `real_io_task()` 读取 S1-S4 GPIO，20 ms 防抖后调用 `sorter_scheduler_sensor()`；S1 打开视觉窗口。
  - 真实视觉来源是 `components/vision_app/vision_app.cpp`，有检测结果时调用 `sorting_sim_control_submit_vision_class()`；无检测结果时不提交，后续由视觉窗口失败逻辑处理。
  - 以太网模拟来源是 `VISION_FRAME`/`VISION_RESULT`/`SENSOR`/`DISTANCE` 文本协议，最终调用同一套调度器函数。
- 结论：
  - 上位机模拟和真实硬件在核心调度、分类、传感器状态机、电机输出路径上高度吻合。
  - 不能视为完全等价：真实 GPIO 有电平/防抖/时序风险；真实视觉有推理延迟/漏检风险；真实传感器事件不带 package id；编码器未接时真实 C 段不能依赖距离输入，只能依赖 timeout fallback。
- 代码改动：
  - 在 `sorter_scheduler_t` 中新增 `failed_class_cursor`。
  - `SORTER_CLASS_VISION_FAILED` 和 `SORTER_CLASS_UNKNOWN` 进入 `sorter_scheduler_vision_result()` 时按 class1、class2、class3 轮番改写。
  - `SORTER_STATE_WAITING_VISION` 超时也使用同一个轮转分配。
  - C 调度器测试补充失败分配顺序断言：vision_failed -> class1/class2/class3，class=none wrap 到 class1。
- 验证：
  - `python -m unittest esp32_sorter_sim_py.tests.test_sorter_scheduler_c`：通过。
  - `git diff --check` in `merge_project`：通过。
  - `cd /home/kazeform/2026esp/merge_project && idf.py build`：通过；仅保留既有 `global_statusbar` unused warning。
  - `timeout 120s idf.py flash monitor`：失败，当前环境没有可用串口；自动扫描的 `/dev/ttyS*` 均无法打开。
