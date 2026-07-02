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

## 2026-07-02 Motor Timeout Inspection

- 用户反馈 `old_project` 调试页面测试 class3/class2 或电机模拟运行时，第三条带感觉只运行约 0.5s/不到 1s，要求先检查不修改。
- 检查结果：
  - `old_project/components/Sorter_app/sorter_core/sorter_scheduler.c` 默认值为 `belt_a_timeout_ms=2000`、`belt_b_timeout_ms=750`、`belt_c_timeout_ms=750`、`handoff_delay_ms=1000`、`c_min_busy_ms=2500`、`c_fallback_busy_ms=8000`。
  - UI debug 面板显示默认 `TO A2000 B750 C750`，B/C timeout 调整按钮每次增减 250ms。
  - `CLASS2`/`CLASS3` 按钮调用 `sorting_sim_control_simulate_class()`，会创建虚拟包裹走调度器，不是直接长时间转电机。
  - `MTEST` 调用 `motor_test_task()`，每个电机直接运行 500ms 后停止，再延迟 80ms 测下一个电机。
  - `old_project` 与 `merge` 的 `sorter_scheduler.c`、`sorting_sim_control.c` 当前无 diff。
- 结论：第三条带短时间运行与旧工程代码一致，不是迁移差异；若真实硬件行程需要更长，应后续调整 B/C timeout、C fallback 或接入传感器/编码器闭环。

## 2026-07-02 Merge Timeout Defaults Update

- 用户要求在新工程把默认超时参数改为 6 秒、3 秒、3 秒。
- 修改 `merge/components/Sorter_app/sorter_core/sorter_scheduler.c`：
  - `belt_a_timeout_ms`: 2000 -> 6000
  - `belt_b_timeout_ms`: 750 -> 3000
  - `belt_c_timeout_ms`: 750 -> 3000
- 检查 `merge/components/UI/generated/setup_scr_dashboard.c` 未发现 old_project debug 面板的 `TO A2000 B750 C750` 初始文案，因此无需同步 UI 文案。
- 验证：
  - `idf.py build` 失败于 SPIFFS storage 生成，原因是未跟踪文件 `merge/model/datasets5000_kl_MOSIC_NOINT16.espdl` 文件名超过当前 `CONFIG_SPIFFS_OBJ_NAME_LEN=32`；该失败发生在 storage image 生成阶段，不是 sorter 代码编译失败。
  - `idf.py app` 成功，`Sorter_app` 重新编译并链接生成 `sample_project.bin`。
  - `idf.py -p /dev/ttyUSB0 app-flash` 成功，写入 app 分区并 hash verified。
  - `idf.py -p /dev/ttyUSB0 monitor` 成功看到新 app 启动到 `System initialization done`，Ethernet、电机 BSP、sort debug 任务启动。
- 未处理项：后续若需要恢复完整 `idf.py build`，需要处理未跟踪长文件名模型：重命名到 <=32 字符，或调整 SPIFFS object name length 并评估兼容性。
