# Current Task

## Goal

进入 `merge_project` 阶段：进行算法改进，并在真实硬件链路上调试验证。

## Current State

- 工程：`merge_project`
- 任务起点：上一阶段蓝屏/相机链路诊断已结束，相关过程已归档到 `docs/agent/archive/2026-06-29-blue-screen-camera-diagnostics.md`。
- `merge_project` 当前已有既有未提交改动；后续修改前必须先识别并保护这些改动，避免误覆盖。
- `bug_project` 暂不作为当前主工程，保留其蓝屏诊断结论供硬件链路问题参考。

## Immediate Next Step

1. 进入 `merge_project`。
2. 按 CodeGraph 优先原则梳理算法入口、硬件链路、相机/显示/电机/传感器相关调用路径。
3. 检查当前 git 状态，区分已有改动和本轮新增改动。
4. 在改算法前建立可回退 checkpoint。
5. 逐步进行算法改进，并在真实硬件链路上执行构建、烧录、监控和现象记录。

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

- 暂无。
