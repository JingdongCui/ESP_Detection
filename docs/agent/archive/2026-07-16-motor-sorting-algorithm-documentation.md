# 2026-07-16 电机分拣算法文档

## 目标

基于 `ESP32P4_Detection` 第 9 包当前源码，编写电机分拣算法实现文档，重点解释状态机归属、包裹状态转换和具体函数调用链。

## 工作区准备

- 用户要求开始本任务时，根工作区仍有上一轮第 9 包导入和资料归档的未提交改动。
- 按 Git 规则，先将上一轮根工作区整理结果提交为 `e8616ec chore: checkpoint package 9 workspace reorganization`。
- 活跃固件工程 `ESP32P4_Detection` 是独立 Git 仓库，当前基线提交为 `367e0c7`。

## 源码分析

- 活跃工程存在 `.codegraph/`，优先使用 `codegraph explore` 和 `codegraph node` 追踪实现。
- `sorter_package_state_t` 存在每个 `sorter_package_track_t` 内，主状态机属于包裹对象。
- `sorter_scheduler_t` 同时管理最多 8 个包裹状态机，并通过 `b_owner`、`c_owner` 仲裁 B/C 传送带共享资源。
- 传送带没有独立状态枚举；调度器另有 `estop`、`paused_after_estop` 全局运行状态。
- 已核对以下完整链路：
  - `System_Init()` 启动硬件、视觉和分拣控制；
  - `real_io_task()` 轮询 S1～S4、读取编码器并周期 tick；
  - S1 建包和视觉窗口绑定；
  - 视觉模型类别到调度类别的非顺序映射；
  - S2 的 A→B 两阶段交接和第一次分流；
  - S3 的 1 类完成路径；
  - S4 的 B→C 两阶段交接和第二次分流；
  - C 编码器/超时完成路径；
  - `emit_motor()` 到 MCPWM 比较器的真实电机输出链路。

## 交付物

- 新增 `docs/motor_sorting_algorithm.md`，共 446 行。
- 文档包含：
  - 状态归属结论；
  - 物理对象和软件编号；
  - 核心结构体字段；
  - 包裹状态表和转换图；
  - 启动、传感器、视觉、调度、电机函数调用链；
  - 多包裹并行和 B/C 资源仲裁；
  - 超时兜底、默认参数和当前 GPIO 配置影响；
  - 一个 2 类包裹的完整执行示例；
  - 关键函数索引和日志排查顺序。

## 重要实现结论

- 当前 S3 和三路编码器 GPIO 都配置为 `-1`，因此 1 类出口和 C 段当前分别依赖 B/C 状态超时兜底。
- 当前 `belt_c_timeout_ms=2000` 小于 `c_min_busy_ms=2500`。默认情况下 C 段通用状态超时通常早于编码器距离完成条件；文档明确记录了这个参数关系，未修改固件逻辑。
- A 段安全间距函数已存在，但 `pos_mm` 当前没有按编码器连续积分，真实节拍边界主要依靠 S2 暂扣与 B/C owner 仲裁。

## 验证

- `git diff --check` 通过。
- 文档引用的 8 个源码文件均存在。
- 使用 `rg` 确认文档涉及的主要函数名在当前源码中存在。
- 确认文档只有一个一级标题。
- 确认归档前 `CURRENT.md` 不超过 200 行。
- 本任务只修改 Markdown 文档和 Agent 记录，没有修改固件代码，因此未执行 `idf.py build`、`idf.py flash monitor`。

## 结果

文档编写和静态校验完成，无阻塞。
