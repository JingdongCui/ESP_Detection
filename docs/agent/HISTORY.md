# History

## 2026-07-04 Merge Experience Doc Expansion

- 用户要求：
  - 完善 `docs/agent/MERGE_EXPERIENCE.md`。
  - 为接下来相似合并工作做准备。
  - 记录踩过的坑、需要注意的点和经验。
- 修改前根仓库 checkpoint：
  - `b4201eb checkpoint before expanding merge experience`
- 已扩展：
  - `docs/agent/MERGE_EXPERIENCE.md`
- 新增重点内容：
  - 合并前 checkpoint、baseline、边界和小步验证原则。
  - 依赖排查要同时看 manifest、lock、managed component hash，尤其注意组件自己的 `idf_component.yml`。
  - `esp-dl 3.3.6` 是 `findlogo.espdl` 实机加载崩溃后的必要升级；但相机/视频/屏/UI adapter 等连带升级要单独证明。
  - 旧/新 ESP32-P4 revision 与 360/400MHz 配置不能混用。
  - `build/project_description.json` 可能仍指向微信临时目录，不能无脑信任旧 build。
  - `system_monitor()` 既是 UI 状态数据源又会刷日志，硬件调试时不能为干净 monitor 直接关掉数据源。
  - RGB/BGR、模型类别、概率接口和物理出口必须分开记录和映射。
  - TCP 20 包必须配合 `log_audit`，不能只看模拟器最终 ok。
  - 端口 `/dev/ttyUSB0`/`/dev/ttyACM0` 和 flash/monitor 波特率要分开确认。
- 验证：
  - 文档改动，无需 `idf.py build` / `flash monitor`。

## 2026-07-04 Merge Experience Doc Start

- 用户要求：
  - 写一个合并经验 md 的开头。
  - 后续将从另外两个对话补齐剩余内容。
  - 目标是提高后续往 `teammatesproject_old` 类似工程重做迁移的成功率。
- 修改前根仓库提交：
  - `bc1c65c Record findlogo merge archive`
- 已新增：
  - `docs/agent/MERGE_EXPERIENCE.md`
- 文档当前覆盖：
  - 合并经验文档用途。
  - 新一轮移植前的源工程/成功工程/目标工程对齐方法。
  - baseline、划边界、小步验证的迁移节奏。
  - 电机分拣链路与 findlogo 视觉链路第一轮经验摘要。
  - 后续需要从其它对话补齐的章节方向。
- 验证：
  - 文档改动，无需 `idf.py build` / `flash monitor`。

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

## 2026-07-03 Traditional ROI Cascade

- 用户要求：
  - `merge` 从两阶段模型改为传统算法 ROI 第一阶段 + `findlogo` 第二阶段。
  - 做好备份。
  - 当前板子为高版本芯片，保留当前 revision/400MHz，只要能烧录即可。
  - 左上角 LOGO 按钮改为 ROI 校准，停用当前抓帧按钮功能。
- 备份：
  - `1f3fb87 checkpoint before traditional roi cascade`
- 代码修改：
  - 新增传统 ROI 算法：`components/vision/detector/roi_algorithm.c/.h`。
  - `roi_tuning.c` 桩替换为旧工程真实 `roi_tuning.cpp`，恢复阈值 get/set/apply/default 和校准实现。
  - `components/vision/CMakeLists.txt` 加入 `roi_algorithm.c`，`roi_tuning.c` 改为 `roi_tuning.cpp`。
  - `vision_model_init()` 只加载 `findlogo.espdl`，不再加载 `det_pico_224_224_waybill.espdl`。
  - `vision_model_run()` 先用 `roi_algorithm_detect()` 生成面单红框，再裁剪 ROI 给 `findlogo`，logo 框映射回原图。
  - `main/system_init.c` 左上角 LOGO 按钮从 `vision_frame_dump_request` 改为 `roi_tuning_request_calibration`。
- 验证：
  - `idf.py build`：成功。
  - `idf.py -p /dev/ttyUSB0 -b 921600 flash`：失败，原因是 `/dev/ttyUSB0` 不存在。
  - `find /dev -maxdepth 1 \( -name 'ttyUSB*' -o -name 'ttyACM*' \)`：当前设备为 `/dev/ttyACM0`。
  - `idf.py -p /dev/ttyACM0 -b 921600 flash`：成功，芯片 `ESP32-P4 revision v3.1`，app/partition/storage hash verified。
  - `idf.py -p /dev/ttyACM0 monitor`：启动到视觉、SORTDBG、电机/传感器初始化，无 Guru Meditation。
- 观察：
  - monitor 里 `CHIP_USB_UART_RESET` 后的 `Core1 Saved PC` 显示 reset 前 core1 正在 `roi_algorithm_detect()`，这是 USB reset 保存 PC，不是 panic。
  - 当前 monitor 持续刷 `ISP_AWB: subwindow size ...` warning，影响观察；本轮未改相机/ISP 日志策略。
