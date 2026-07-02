# History

## 2026-07-02 Start Old Project Motor Baseline

- 用户要求归档上一阶段文档，开始新任务。
- 新阶段目标：
  - 将 `old_project` 中与电机有关的内容移植到 `merge`。
  - 当前先跑通 `old_project` 的上位机模拟器 TCP 版本，模拟 20 件分拣，建立 baseline。
  - baseline 后规划清楚移植内容和边界。
- 已将上一阶段整理归档到：
  - `docs/agent/archive/2026-07-02-merge-runtime-debug-stage.md`

## 2026-07-02 Old Project Baseline

- 按用户要求直接烧录 `old_project` 到 `/dev/ttyUSB0`；因当前板为 ESP32-P4 rev v1.0，将 `old_project/sdkconfig.defaults` revision 从 v3.01 降到 rev 0/上限 199。
- `old_project` build、flash、monitor 成功，启动日志确认 firmware 可在 rev v1.0 板运行。
- TCP 20 件 baseline 命令保存日志到 `esp32_sorter_sim_py/logs/old_project_tcp_baseline_20_rev0_20260702.log`。
- baseline 结果：
  - `RESULT ok completed=20/20`
  - audit: warnings=0, drops=0, pose_asserts=0, desyncs=0, faults=0
  - done: class1=7, class2=7, class3=6
  - motor command counts: M1 forward30=30 stop=21; M2 forward35=7 reverse35=26 stop=34; M3 forward35=6 reverse35=7 stop=14

## 2026-07-02 Merge Sorter Migration

- 按迁移规划在 `merge` 执行实现，迁移边界：
  - 移植 `Sorter_app` 调度器和 `sorting_sim_control`。
  - 移植 BSP 电机、编码器、分拣传感器和 sorter debug config。
  - 移植 Ethernet TCP 应用，保留上位机模拟协议。
  - 接入 `vision_detect.c` 的 LOGO 0/1/2 到 sorter class1/2/3。
  - 在 `system_init.c` 启动 Ethernet sorter link、debug 任务并启用 motor output。
  - 不移植 UI 大改和非分拣链路；保持当前 `merge` 视觉、LCD、UI 主体。
- `merge` build 成功；仅有既有 generated UI unused variable warning。
- `idf.py -p /dev/ttyUSB0 flash monitor` 实机启动成功：
  - chip revision v1.0，min rev v0.0，max rev v1.99。
  - Ethernet static IP `192.168.10.2`，连接模拟器目标 `192.168.10.1:5000`。
  - motor 1 GPIO A=3/B=2，motor 2 A=32/B=36，motor 3 A=4/B=5。
  - motor output enabled。
- 迁移后 TCP 20 件模拟日志：
  - `esp32_sorter_sim_py/logs/merge_tcp_migration_20_rev0_20260702.log`
  - `RESULT ok completed=20/20`
  - audit: warnings=0, drops=0, pose_asserts=0, desyncs=0, faults=0
  - done: class1=7, class2=7, class3=6
  - connected-session motor command counts: M1 forward30=29 stop=21; M2 forward35=7 reverse35=26 stop=34; M3 forward35=6 reverse35=7 stop=14
- 注意：迁移版 TCP 日志比旧 baseline 少 1 次 M1 forward30，是因为模拟器在固件已启动后接入，启动阶段的 M1 初始输出没有被 TCP 连接记录；monitor boot 日志已确认真实输出启用。
