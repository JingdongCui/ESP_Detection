# Current Task

## Goal

将 `old_project` 中与电机和分拣控制有关的内容移植到 `merge`。

当前阶段已进入实现：把 `old_project` 的电机分拣链路移植到 `merge`，覆盖真实硬件链路和以太网 TCP 模拟链路，并以 20 件 TCP 模拟作为迁移验收基线。

## Current State

- 上一阶段 `merge_project` 传感器、失败分类和运行态 debug 显示已归档到 `docs/agent/archive/2026-07-02-merge-runtime-debug-stage.md`。
- 当前对照工程：`old_project`。
- 当前目标工程：`merge`。
- 根目录存在 `.codegraph/`，理解和定位代码优先使用 CodeGraph。
- `old_project` 已降低 ESP32-P4 revision 门槛并完成烧录、TCP 20 件 baseline：
  - `RESULT ok completed=20/20`
  - audit: warnings=0, drops=0, desyncs=0, faults=0
  - done: class1=7, class2=7, class3=6
- `merge` 已完成第一轮移植：
  - 新增 `components/Sorter_app`，包含调度器和 `sorting_sim_control`。
  - BSP 电机、编码器、分拣传感器配置按 `old_project` 对齐。
  - Ethernet TCP 应用按 `old_project` 迁入，默认连接模拟器 `192.168.10.1:5000`。
  - `vision_detect.c` 将 LOGO 分类 0/1/2 提交到分拣调度器 class1/2/3。
  - `system_init.c` 启动 Ethernet sorter link、debug 任务，并启用真实电机输出。
  - `sdkconfig.defaults` 已把 ESP32-P4 最低 revision 降为 0，最高 revision 限为 199，匹配当前 v1.0 板。
- `merge` 验证结果：
  - `idf.py build` 成功；仅保留既有 UI generated unused variable warning。
  - `/dev/ttyUSB0` flash/monitor 启动成功，boot 日志确认芯片 rev v1.0、Ethernet static IP `192.168.10.2`、电机 GPIO 初始化并启用。
  - TCP 20 件模拟完成：`RESULT ok completed=20/20`。
  - audit: warnings=0, drops=0, desyncs=0, faults=0。
  - done: class1=7, class2=7, class3=6。

## Immediate Next Step

1. 提交 `merge` 移植代码和根目录 agent 文档。
2. 后续真实硬件联调时重点验证：
   - S1/S3 分拣传感器 GPIO37/GPIO38 与 console UART boot 日志提示的潜在冲突。
   - 编码器当前仍沿用 `old_project` 虚拟配置，真实硬件 C 段完成主要依赖 fallback，若需要距离闭环需补真实 encoder GPIO。
   - 默认模拟模式会优先接受 TCP `VISION_FRAME`/`SENSOR`/`DISTANCE`，真实 IO 模式需通过配置切换。

## Verification Standard

- 固件代码修改后优先执行：

```bash
cd /home/kazeform/2026esp/merge
idf.py build
idf.py flash monitor
```

- 迁移 TCP 20 件验证命令：

```bash
cd /home/kazeform/2026esp
python -m esp32_sorter_sim_py --headless --transport tcp --host 192.168.10.1 --port 5000 --count 20 --class-sequence 1,2,3,2,3,1 --feed-interval 0.25 --max-active 8 --timeout 180 --stall-timeout 18 --b-idle-threshold 3 --log-file esp32_sorter_sim_py/logs/merge_tcp_migration_20_rev0_20260702.log
python -m esp32_sorter_sim_py.log_audit esp32_sorter_sim_py/logs/merge_tcp_migration_20_rev0_20260702.log
```

## Blockers

- 当前无阻塞。
- 风险记录：本次 TCP 迁移模拟是在固件已启动后接入，因此 TCP 日志少记录一次启动阶段的 M1 初始 forward；启动 monitor 已确认真实电机初始化和输出启用。
