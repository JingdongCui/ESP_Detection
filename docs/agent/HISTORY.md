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
