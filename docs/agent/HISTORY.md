# History

## 2026-07-03 Findlogo Merge Report And TCP Regression

- 用户要求：
  - 写合并报告，供队友查看。
  - 报告中记录库依赖更新和 RGB 设定。
  - 跑 TCP 模式上位机模拟 20 包分拣。
  - 检查日志确认成功。
- 已新增报告：
  - `merge/docs/findlogo_merge_report.md`
- 报告记录内容：
  - 两阶段模型接入内容。
  - `findlogo.espdl` 哈希与来源。
  - `esp-dl 3.3.2` 回退实机失败，最终保留 `esp-dl 3.3.6`。
  - RGB/BGR 设定：源按 `DL_IMAGE_PIX_TYPE_BGR888` 声明，模型期望 RGB，由 ESP-DL `ImagePreprocessor` 转换。
  - logo 训练类别和 UI/概率/sorter 映射。
  - build、flash、模型加载验证。
- TCP 20 包命令：

```bash
cd /home/kazeform/2026esp
python -m esp32_sorter_sim_py --headless --transport tcp --host 192.168.10.1 --port 5000 --count 20 --class-sequence 1,2,3,2,3,1 --feed-interval 0.25 --max-active 8 --timeout 180 --stall-timeout 18 --b-idle-threshold 3 --log-file esp32_sorter_sim_py/logs/merge_findlogo_tcp_20_20260703.log
```

- 模拟器结果：
  - `RESULT ok completed=20/20`
- 审计命令：

```bash
python -m esp32_sorter_sim_py.log_audit esp32_sorter_sim_py/logs/merge_findlogo_tcp_20_20260703.log
```

- 审计结果：
  - `audit_status=ok`
  - `records=13411`
  - `warnings=0`
  - `drops=0`
  - `pose_asserts=0`
  - `desyncs=0`
  - `faults=0`
  - `transfer_events=132`
  - `transport_packets=5789`
  - `result=ok completed=20/20 active=0 created=20`
- 完成分布：
  - `class1=7`
  - `class2=7`
  - `class3=6`

## Archived

- `docs/agent/archive/2026-07-03-findlogo-merge.md`

## 2026-07-03 Motor Algorithm Optimization

- 用户要求按优化方案实现电机算法优化，优化前后都跑 TCP 20 包并审计日志。
- 修改前在 `merge` 提交 checkpoint：
  - `31fdbf8 checkpoint before motor algorithm optimization`
- 优化前干净 baseline：

```bash
cd /home/kazeform/2026esp
python -m esp32_sorter_sim_py --headless --transport tcp --host 192.168.10.1 --port 5000 --count 20 --class-sequence 1,2,3,2,3,1 --feed-interval 0.25 --max-active 8 --timeout 180 --stall-timeout 18 --b-idle-threshold 3 --log-file esp32_sorter_sim_py/logs/merge_motor_opt_before_tcp_20_20260703.log
python -m esp32_sorter_sim_py.log_audit esp32_sorter_sim_py/logs/merge_motor_opt_before_tcp_20_20260703.log
```

- baseline 结果：
  - `audit_status=ok`
  - `result=ok completed=20/20 active=0 created=20`
  - `warnings=0,drops=0,pose_asserts=0,desyncs=0,faults=0`
  - 完成分布：`class1=7,class2=7,class3=6`
- 代码优化：
  - 新增 typed scheduler event：motor/status/package/fault。
  - 新增 `sorter_protocol_format_event()`，保持外部 `MOTOR/STATUS/PKG/FAULT` 文本协议兼容。
  - `sorting_sim_control.c` 本地电机输出改为直接处理 `SORTER_EVENT_MOTOR`，不再解析本机生成的 `MOTOR,...` 字符串。
  - 删除未使用的 `SORTER_STATE_WAITING_BC`、`b_center_to_exit_mm`、`transfer_timeout_mm`、`max_packages`。
  - 未改电机速度、超时、交接、路由、传感器语义和 TCP 协议。
- 验证：
  - `idf.py build` 成功。
  - `idf.py -p /dev/ttyUSB0 -b 921600 flash` 成功，app/storage hash verified。
  - `idf.py -p /dev/ttyUSB0 monitor` 使用 115200 启动正常，模型加载、Ethernet、SORTDBG、电机 BSP 初始化正常，无 Guru Meditation。
- 优化后 TCP 20 包：

```bash
cd /home/kazeform/2026esp
python -m esp32_sorter_sim_py --headless --transport tcp --host 192.168.10.1 --port 5000 --count 20 --class-sequence 1,2,3,2,3,1 --feed-interval 0.25 --max-active 8 --timeout 180 --stall-timeout 18 --b-idle-threshold 3 --log-file esp32_sorter_sim_py/logs/merge_motor_opt_after_tcp_20_20260703.log
python -m esp32_sorter_sim_py.log_audit esp32_sorter_sim_py/logs/merge_motor_opt_after_tcp_20_20260703.log
```

- 优化后结果：
  - `audit_status=ok`
  - `result=ok completed=20/20 active=0 created=20`
  - `warnings=0,drops=0,pose_asserts=0,desyncs=0,faults=0`
  - 完成分布：`class1=7,class2=7,class3=6`

## 2026-07-03 Motor Speed 60 And Teammate Report Update

- 用户要求：
  - 更新给队友看的报告。
  - 默认电机速度全部改为 60%。
  - 报告说明 UI 做了什么、旧 UI 哪些没迁移、电机分拣哪些参数可实时调、模拟器分拣怎么用。
  - 完成后跑 TCP 20 包模拟并审计日志。
- 已修改：
  - `merge/components/Sorter_app/sorter_core/sorter_scheduler.c`
    - 默认 `motor_a_speed_percent/motor_b_speed_percent/motor_c_speed_percent` 从 `30/35/35` 改为 `60/60/60`。
  - `merge/docs/motor_algorithm_review.md`
    - 增补 UI notes。
    - 增补旧 UI 未迁移项。
    - 增补 TCP/serial `CONFIG` 可实时调参数。
    - 增补 TCP 模拟器使用命令和成功标准。
  - `merge/docs/findlogo_merge_report.md`
    - 增补 follow-up 说明，指向电机算法报告并记录默认 60%、UI 边界、模拟器 60% 参数。
- 验证待执行：
  - `idf.py build`：成功。
  - `idf.py -p /dev/ttyUSB0 -b 921600 flash`：成功，app/storage hash verified。
  - `idf.py -p /dev/ttyUSB0 monitor`：115200 启动正常，模型、Ethernet、SORTDBG、电机 BSP 初始化完成，无 Guru Meditation。
  - 启动日志看到 `bsp_motor: motor 1 compare A=0 B=60`。
- TCP 20 包模拟命令：

```bash
cd /home/kazeform/2026esp
python -m esp32_sorter_sim_py --headless --transport tcp --host 192.168.10.1 --port 5000 --count 20 --class-sequence 1,2,3,2,3,1 --feed-interval 0.25 --max-active 8 --timeout 180 --stall-timeout 18 --b-idle-threshold 3 --motor-a-speed 60 --motor-b-speed 60 --motor-c-speed 60 --log-file esp32_sorter_sim_py/logs/merge_motor_speed60_tcp_20_20260703.log
python -m esp32_sorter_sim_py.log_audit esp32_sorter_sim_py/logs/merge_motor_speed60_tcp_20_20260703.log
```

- TCP 20 包结果：
  - `RESULT ok completed=20/20`
  - `audit_status=ok`
  - `warnings=0,drops=0,pose_asserts=0,desyncs=0,faults=0`
  - `transfer_events=132`
  - `transport_packets=4533`
  - 完成分布：`class1=7,class2=7,class3=6`
  - 日志确认 `CONFIG,a_speed=60,b_speed=60,c_speed=60`
