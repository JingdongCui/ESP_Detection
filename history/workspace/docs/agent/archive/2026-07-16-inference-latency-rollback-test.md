# 推理耗时回退与 A/B 实验

## 目标

判断推理从约 70ms 稳定升至约 500ms 是硬件临时不稳定，还是软件版本/诊断改动导致。

## 测试条件

- 同一块 ESP32-P4 revision v1.0。
- 同一串口 `/dev/ttyUSB0`、同一相机、LCD、供电和网络连接状态。
- 各回退版本仅增加低频 UART 墙钟耗时日志，不改变任务、内存或初始化行为。
- 均执行 `idf.py build`、plain `idf.py flash` 并 monitor 实测。

## 结果

1. 第 9 包导入基线 `367e0c7`
   - 连续采样约 70–90ms，常见值 74–84ms。
   - 未出现接近 500ms 的稳定异常。
2. 真实 IO 栈迁移到 PSRAM 的提交 `e1cbd5d`
   - 启动前约 72ms；系统启动后约 69–186ms，多数处于 70–160ms。
   - 有波动变慢，但没有稳定 500ms。
3. 当前正式提交 `0efa82a`，使用原版 ESP-DL 3.3.7
   - 系统初始化前后连续约 72–75ms。
4. 当前工作区的 ESP-DL worker 临时 `+1` 优先级实验
   - 稳定约 459–476ms。
   - 强制关闭摄像头预览 PPA 后仍为约 459–469ms，因此预览不是该稳定异常的原因。

## 结论

- 同一硬件在第 9 包基线和干净当前提交上都恢复到约 70ms，可排除硬件临时不稳定。
- 稳定约 500ms 与未提交的 ESP-DL worker 优先级实验直接相关，而不是 Git 中的当前业务功能。
- 把 worker 从调用者优先级 4 提高到 5 后，会与 core1 的 LVGL priority 5 形成确定性的调度交互；该改动不是修复方案。
- `vision_det` 的任务 runtime 只统计父任务，不包含 `dl_mc0/dl_mc1` 的计算时间，旧日志中的 `wall-parent_cpu` 不能直接等同于“被抢占/等待时间”。

## 收尾

- 撤销 ESP-DL worker `+1` 补丁、自动应用 CMake 文件和强制暂停预览诊断。
- 校验主工程 `git status` 干净，managed ESP-DL 源文件哈希恢复为原版：`4dcfc3e13c6f71c9b2ad7a6be17ed2cb0141b3575f027848b5bce233eda3a40f`。
- 删除所有临时 worktree。
- 重新构建并烧录干净 `0efa82a`，monitor 到 `System initialization done`；干净版本推理约 72–75ms。
