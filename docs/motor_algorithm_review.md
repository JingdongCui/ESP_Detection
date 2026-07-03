# 电机算法和分拣交付报告

日期：2026-07-03

## 本次结论

本轮完成了电机分拣算法结构优化，并把默认电机速度统一改为 60%。

当前默认值：

```text
A 电机：60%
B 电机：60%
C 电机：60%
```

最新验证结果：

```text
idf.py build：通过
idf.py -p /dev/ttyUSB0 -b 921600 flash：通过，app/storage hash verified
monitor 115200：启动正常，无 Guru Meditation
TCP 20 包模拟：RESULT ok completed=20/20
日志审计：audit_status=ok
warnings=0,drops=0,pose_asserts=0,desyncs=0,faults=0
完成分布：class1=7,class2=7,class3=6
```

验证日志：

```text
reports/merge_motor_speed60_tcp_20_20260703.log
reports/merge_motor_speed60_tcp_20_20260703.audit.txt
```

## 电机分拣链路结构

当前分拣链路分三层：

1. 调度核心
   - `components/Sorter_app/sorter_core/sorter_scheduler.c`
   - `components/Sorter_app/include/sorter_core/sorter_scheduler.h`
   - 负责包裹状态、皮带占用、超时、类别路由、分拣事件。
2. 控制桥接层
   - `components/Sorter_app/sorting_sim_control.c`
   - `components/Sorter_app/include/sorting_sim_control.h`
   - 负责 TCP/串口命令解析、真实/模拟模式选择、真实传感器轮询、电机输出、调试接口。
3. 硬件 BSP
   - `components/bsp/bsp_motor.c`
   - `components/bsp/include/bsp_motor.h`
   - `components/bsp/include/sorter_debug_config.h`
   - 负责 MCPWM 初始化、速度/方向转 PWM、输出使能、GPIO 反向配置。

## 算法能力

当前 scheduler 支持：

- 最多 `SORTER_MAX_PACKAGES=8` 个活动包裹。
- 三个电机 / 三段皮带。
- S1 创建视觉窗口和包裹。
- 视觉结果提交 class1/class2/class3。
- 识别失败时按 class1、class2、class3 轮转分配。
- S2 处理 A 到 B 的交接。
- S3 完成 class1。
- S4 处理 B 到 C 的交接。
- C 段通过编码器距离或 fallback timeout 完成 class2/class3。
- B/C owner 占用，避免两个包裹同时抢同一段皮带。
- S2/S4 清空后的 handoff delay。
- 各段皮带 fallback timeout。
- 急停。
- TCP 模拟和真实传感器模式共用同一调度核心。

## 已完成的优化

本轮优化点：

- scheduler 内部新增 typed event：
  - `SORTER_EVENT_MOTOR`
  - `SORTER_EVENT_STATUS`
  - `SORTER_EVENT_PACKAGE`
  - `SORTER_EVENT_FAULT`
- 外部 TCP/debug 文本协议保持不变。
- 新增 `sorter_protocol_format_event()`，统一把 typed event 格式化为原来的文本协议。
- 本地电机输出直接处理 `SORTER_EVENT_MOTOR`，不再解析本机刚生成的 `MOTOR,...` 字符串。
- 删除未使用的算法表面：
  - `SORTER_STATE_WAITING_BC`
  - `sorter_config_t.b_center_to_exit_mm`
  - `sorter_config_t.transfer_timeout_mm`
  - `sorter_config_t.max_packages`
- 后续参数调整：A/B/C 默认电机速度统一为 `60%`。

这些优化没有改变外部 TCP 协议、类别语义、传感器语义和物理出口语义。

## UI 做了什么

当前 `merge` 生成 UI 保留视觉系统主界面：

- 实时检测页：
  - live preview 区域。
  - 检测状态。
  - 置信度。
  - 推理耗时。
  - 当前快递公司。
  - JT/ZT/YD 概率和计数。
- 日志页：
  - 识别日志。
  - 推理详情。
- 设置页：
  - 网络设置。
  - 亮度。
  - 检测阈值。
  - 检测开关。
  - 预览叠加框。
  - 原有通用控制/open-loop/PID 控件。
- 系统页：
  - 内存、CPU、堆、运行时间、芯片温度、任务数、以太网状态。

UI 公司名已按 findlogo 训练类别修正：

```text
cat0 = 极兔
cat1 = 韵达
cat2 = 中通
```

物理出口仍保持旧机器语义：

```text
极兔/JT -> CLASS1
中通/ZT -> CLASS2
韵达/YD -> CLASS3
```

因此 sorter 提交映射为：

```text
cat0 -> CLASS1
cat1 -> CLASS3
cat2 -> CLASS2
```

## 旧 UI 没有迁移过来的内容

旧工程里的分拣手动控制 UI 没有整体搬到当前生成 UI。

当前屏幕上没有：

- `CLASS1/CLASS2/CLASS3` 手动注入按钮。
- `MOTOR_TEST` 按钮。
- 编码器清零按钮。
- S1-S4 传感器状态块。
- 活动包裹列表。
- A/B/C 电机速度编辑控件。
- A/B/C timeout 编辑控件。

原因：当前 UI 源主要来自视觉 dashboard，分拣调试能力保留在 TCP/串口协议和 C API 中，未做完整屏幕面板。

后续如果要补屏幕面板，可以接这些现有接口：

- `sorting_sim_control_get_settings()`
- `sorting_sim_control_apply_settings()`
- `sorting_sim_control_get_runtime_debug()`
- `sorting_sim_control_get_hardware_status()`

## 可实时调节的分拣参数

通过 TCP 或 USB 串口发送 `CONFIG`，无需重新烧录。

可调参数：

- `mode=ethernet|real|timed`
  - `ethernet`：TCP 上位机模拟模式。
  - `real`：真实 S1-S4 传感器和真实视觉提交。
  - `timed`：调试/定时模式，外部模拟传感器输入关闭。
- `a_speed=<0..100>`
  - A 电机速度百分比。
- `b_speed=<0..100>`
  - B 电机速度百分比。
- `c_speed=<0..100>`
  - C 电机速度百分比。
- `handoff_delay_ms=<50..30000>`
  - S2/S4 清空后继续交接的延时。
- `a_timeout_ms=<50..30000>`
  - A 段 fallback timeout。
- `b_timeout_ms=<50..30000>`
  - B 段 fallback timeout。
- `c_timeout_ms=<50..30000>`
  - C 段 fallback timeout。
- `lost_timeout_min_ms=<0..30000>`
  - 包裹丢失判断最小超时。
- `lost_timeout_max_ms=<0..30000>`
  - 包裹丢失判断最大超时。
- `motor_output=0|1`
  - 物理电机输出使能。
- `sensor_input=0|1`
  - 真实传感器输入使能。
- `real_io=0|1`
  - 旧兼容字段；`1` 选择 real，`0` 选择 ethernet。

示例：

```text
CONFIG,mode=ethernet,a_speed=60,b_speed=60,c_speed=60,handoff_delay_ms=1000,a_timeout_ms=6000,b_timeout_ms=3000,c_timeout_ms=3000,motor_output=1,sensor_input=1
```

其他常用命令：

```text
RESET
ESTOP,state=0|1
MOTOR_TEST
HW_STATUS
ENC_CLEAR,index=0|1|2
PACKAGE_NEW,id=<n>
VISION_RESULT,id=<n>,class=<1|2|3|none|error>
VISION_FRAME,s1=<0|1>,class=<1|2|3|none|error>
SENSOR,id=<1|2|3|4>,state=<0|1>,package=<n>
DISTANCE,motor=<1|2|3>,dist=<mm>
```

## 上位机模拟器怎么用

网络假设：

```text
板端 IP：192.168.10.2
上位机/网关 IP：192.168.10.1
TCP port：5000
```

运行 20 包 TCP 模拟：

```bash
cd /path/to/teammate_handoff_20260703
python -m esp32_sorter_sim_py --headless --transport tcp --host 192.168.10.1 --port 5000 --count 20 --class-sequence 1,2,3,2,3,1 --feed-interval 0.25 --max-active 8 --timeout 180 --stall-timeout 18 --b-idle-threshold 3 --motor-a-speed 60 --motor-b-speed 60 --motor-c-speed 60 --log-file reports/merge_motor_speed60_tcp_20_rerun.log
python -m esp32_sorter_sim_py.log_audit reports/merge_motor_speed60_tcp_20_rerun.log
```

注意：模拟器启动时会发 `CONFIG` 覆盖板端参数。验证当前 60% 默认速度路径时，必须显式带：

```text
--motor-a-speed 60 --motor-b-speed 60 --motor-c-speed 60
```

成功标准：

```text
RESULT ok completed=20/20
audit_status=ok
warnings=0
drops=0
pose_asserts=0
desyncs=0
faults=0
完成分布：class1=7,class2=7,class3=6
```

## 本轮验证摘要

构建：

```text
idf.py build：通过
```

烧录：

```text
idf.py -p /dev/ttyUSB0 -b 921600 flash：通过
app/storage hash verified
```

monitor：

```text
waybill 模型加载成功
findlogo 模型加载成功
Ethernet 获取 IP 192.168.10.2
SORTDBG ready
motor BSP initialized
motor output enabled
启动日志可见 motor 1 compare A=0 B=60
无 Guru Meditation
```

TCP 20 包：

```text
log: reports/merge_motor_speed60_tcp_20_20260703.log
audit: reports/merge_motor_speed60_tcp_20_20260703.audit.txt
audit_status=ok
result=ok completed=20/20 active=0 created=20
warnings=0,drops=0,pose_asserts=0,desyncs=0,faults=0
transfer_events=132
transport_packets=4533
完成分布：class1=7,class2=7,class3=6
```

## 后续风险

- TCP 模拟器使用理想化的 package id、传感器事件和距离事件，不能完全代表真实 GPIO 线序、active level、防抖和编码器硬件。
- 真实硬件仍需要现场看 `sort sensor Sx ...`、`PKG`、`MOTOR` 日志确认传感器和出口。
- 如果后续要重构 S2/S4 交接逻辑，先保留当前 20 包 TCP 回归作为 baseline。
