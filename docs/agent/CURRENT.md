# Current Task

## Goal

将 `old_project` 中与电机和分拣控制有关的内容移植到 `merge`。

当前阶段先不改移植代码，先跑通 `old_project` 的上位机模拟器 TCP 版本，模拟 20 件分拣，建立 baseline；随后规划清楚需要移植的内容和边界。

## Current State

- 上一阶段 `merge_project` 传感器、失败分类和运行态 debug 显示已归档到 `docs/agent/archive/2026-07-02-merge-runtime-debug-stage.md`。
- 当前对照工程：`old_project`。
- 当前目标工程：`merge`。
- 根目录存在 `.codegraph/`，理解和定位代码优先使用 CodeGraph。
- 尚未开始修改 `old_project` 或 `merge` 代码。

## Immediate Next Step

1. 检查 `old_project` 的模拟器、TCP 协议、分拣/电机控制入口和运行命令。
2. 跑通 `old_project` 上位机模拟器 TCP 版本，模拟 20 件分拣并保存关键日志。
3. 从日志建立 baseline：20 件输入、分类/出口分布、电机命令、完成/失败数量、异常或超时。
4. 对比 `old_project` 与 `merge` 的电机、传感器、调度器、TCP 模拟边界，输出移植清单和不移植范围。

## Verification Standard

- 如修改代码，必须运行相关检查。
- 固件代码修改后优先执行：

```bash
cd /home/kazeform/2026esp/merge
idf.py build
idf.py flash monitor
```

- 本阶段 baseline 若只运行 Python/上位机模拟，应记录具体命令、日志位置、结果摘要和未执行实机验证的原因。

## Blockers

- 待确认 `old_project` TCP 模拟是否需要真实板端运行，或是否能完全通过本地上位机模拟器建立 baseline。

