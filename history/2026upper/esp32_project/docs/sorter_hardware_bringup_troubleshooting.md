# 分拣链路真实硬件调试故障预案

本文档用于接入真实电机、光电传感器和编码器后的现场调试。目标是先确认硬件方向、极性和信号质量，再进入连续分拣，避免把接线问题误判为调度算法问题。

## 当前工程状态

当前 `components/bsp/include/sorter_debug_config.h` 中电机 PWM 引脚仍为 `-1`，S1/S2/S3/S4 来自 `sdkconfig` 且当前也是 `-1`。这表示真实电机和传感器尚未接入，固件会安全地不驱动这些未配置硬件。

接硬件前必须先完成：

- 配置 `SORTER_MOTOR0_PWM_A_GPIO`、`SORTER_MOTOR0_PWM_B_GPIO`
- 配置 `SORTER_MOTOR1_PWM_A_GPIO`、`SORTER_MOTOR1_PWM_B_GPIO`
- 配置 `SORTER_MOTOR2_PWM_A_GPIO`、`SORTER_MOTOR2_PWM_B_GPIO`
- 配置 `CONFIG_SORT_SENSOR_S1_GPIO`、`CONFIG_SORT_SENSOR_S2_GPIO`、`CONFIG_SORT_SENSOR_S3_GPIO`、`CONFIG_SORT_SENSOR_S4_GPIO`
- 按实际编码器配置 `SORTER_ENCODER*_GPIO_A/B`

## 推荐调试顺序

1. 先不放物料，上电后串口发送 `HW_STATUS`，确认未配置项显示为 invalid，已配置项能正常读数。
2. 配好电机引脚后，关闭连续进件，只运行 `MOTOR_TEST`，确认 M1、M2、M3 能独立短转。
3. 逐个遮挡 S1/S2/S3/S4，反复发送 `HW_STATUS`，确认每个传感器只影响自己的状态。
4. 手动转动每个编码器，观察 `HW_STATUS` 中对应 `enc_*` 距离是否变化。
5. 使用串口或上位机跑单包 `test1`、`test2`、`test3`。
6. 单包稳定后，再跑连续小批量，最后再增加速度和进件密度。

## 急停恢复问题

现象：上位机或串口打开急停后，M1/M2/M3 brake；关闭急停后，状态变为 running，但 B 带或 C 带不恢复。

当前先不处理急停恢复问题。正常情况下释放急停后，仍在执行的链路最终应该重新出现对应电机命令，例如：

- B 带一类：`MOTOR,id=2,dir=forward,speed=35`
- B 带去 S4：`MOTOR,id=2,dir=reverse,speed=35`
- C 带出口：`MOTOR,id=3,dir=forward|reverse,speed=35`

如果真实硬件仍不恢复：

- 先看串口是否收到对应 `MOTOR` 行。没有收到，优先查固件状态和上位机日志。
- 收到 `MOTOR` 但电机不动，优先查电机驱动使能、电源、PWM 引脚、方向引脚和共地。
- 如果只恢复 M1，不恢复 M2/M3，先记录串口日志和上位机日志；当前版本暂不把它作为主要调试目标。

## 电机方向反了

现象：`MOTOR_TEST` 时某个电机转动方向与传送带期望方向相反，或分拣时包裹被送回错误方向。

处理：

- M1 方向反：修改 `SORTER_MOTOR0_REVERSE`
- M2 方向反：修改 `SORTER_MOTOR1_REVERSE`
- M3 方向反：修改 `SORTER_MOTOR2_REVERSE`

修改后重新 `idf.py build`、`idf.py -p /dev/ttyACM0 flash`，再单独跑 `MOTOR_TEST`。

注意：不要先通过调度逻辑改方向。方向极性属于 BSP 硬件适配层，应在 `sorter_debug_config.h` 固定。

## 电机不转

优先按这个顺序查：

1. `HW_STATUS` 是否能正常输出，确认板端程序运行。
2. `MOTOR_TEST` 后串口是否有 `MOTOR,id=...` 输出。
3. 电机 PWM GPIO 是否仍为 `-1`。
4. 电机驱动板 VM 电源、逻辑电源、GND 是否共地。
5. 驱动使能脚是否需要额外拉高。
6. PWM A/B 接线是否接反或接到不可用 GPIO。
7. 电机速度是否太低导致带负载无法启动，可先把 A/B/C 速度调到 50% 到 70% 测试。

## 电机抖动、异响或发热

可能原因：

- 电源电流不足，启动瞬间掉压。
- 驱动 PWM 输入悬空或 GPIO 配错。
- 电机 A/B 控制逻辑与驱动板模式不匹配。
- 机械阻力过大或皮带过紧。
- 急停 brake 模式对当前驱动等效为短刹，频繁触发会明显发热。

处理：

- 先空载测试，再挂皮带测试。
- 用万用表或示波器看驱动供电是否掉压。
- 若 brake 导致发热，调试阶段减少频繁急停切换，必要时把实际驱动模式改为更适合的 coast/stop 语义。

## S1 与视觉结果错包

S1 是必要传感器。真实硬件模式下，ESP32 直接读取 S1 GPIO：S1 上升沿代表新包裹进入视觉窗口，之后收到的视觉分类结果绑定到这个窗口。

如果出现“视觉结果像是前一个包裹的”：

- 先发 `HW_STATUS`，确认遮挡 S1 时 `s1=1`、放开时 `s1=0`，且 `s1_valid=1`。
- 如果 S1 状态反了，修改 `SORTER_SENSOR_S1_ACTIVE_LEVEL`。
- 如果 S1 一直不触发，检查 `CONFIG_SORT_SENSOR_S1_GPIO`、供电、共地和传感器输出类型。
- 如果 S1 正常但分类仍错包，检查视觉模块是否在 S1 触发后才发送分类，且没有缓存上一帧结果。
- 如果两个包裹间距太近，先增大进件间隔，确保每个包裹都有清晰的 S1 释放再触发。

## 传感器一直 ON 或一直 OFF

可能原因：

- S1/S2/S3/S4 GPIO 未配置或接错。
- 光电传感器输出是低电平有效，但当前 `SORTER_SENSOR_S*_ACTIVE_LEVEL` 为 `1`。
- NPN/PNP 输出与板端输入上拉/下拉方式不匹配。
- 传感器供电电压不对，或输出没有与 ESP32 共地。

处理：

- 遮挡和放开传感器时反复发 `HW_STATUS`。
- 如果遮挡时反而从 `ON` 变 `OFF`，修改对应 `SORTER_SENSOR_S*_ACTIVE_LEVEL`。
- 如果状态完全不变，用万用表直接测 GPIO 电平是否变化。
- 如果 GPIO 电平变化但 `HW_STATUS` 不变，检查配置的 GPIO 编号和实际接线。

## 传感器触发顺序不正常

预期顺序：

- S1：视觉检测点，新包裹窗口开始。
- S2：A 带到 B 带交接点。
- S3：一类出口。
- S4：B 带到 C 带交接点。

常见问题：

- S1 接错或未触发，会导致视觉分类无法可靠绑定到当前包裹。
- S2/S3/S4 接线互换，会出现一类被误判为到 S4，或二/三类提前完成。
- S2 安装位置太靠前，会导致包裹未真正到交接点就停止 A 带。
- S4 安装位置太靠后，会导致 B 带反向超时或包裹撞机械端。

处理：

- 单包低速测试，日志中确认 `HW_STATUS` 的 `s1/s2/s3/s4` 与实际遮挡一致。
- 先固定 S2，再调 S3/S4；不要同时移动多个传感器。

## 编码器距离不变化

可能原因：

- A/B 相 GPIO 未配置、接错或没有共地。
- 编码器输出电压不适配 ESP32 输入。
- 编码器实际安装在未启用的那一路。
- `SORTER_ENCODER_PHASE_PULSES_PER_REV` 或轮径配置不匹配。

处理：

- 手动转动编码器，发送 `HW_STATUS` 看 `enc_a/enc_b/enc_c`。
- 使用 `ENC_CLEAR index=0|1|2` 清零后再转动确认。
- 如果距离方向为负或方向不符合实际，修改 `SORTER_ENCODER*_REVERSE`。
- 如果距离变化比例不对，校准 `SORTER_ENCODER_PHASE_PULSES_PER_REV` 和 `SORTER_ENCODER_WHEEL_DIAMETER_MM`。

## 分拣逻辑不正常

现象：包裹分类正确，但走错出口，或状态卡在某个阶段。

排查：

- 串口看 `PKG,id=...,state=...,class=...`，确认板端分类状态是否正确。
- 一类应进入 `on_b_to_class1`，到 S3 后 `class1_done`。
- 二类和三类应先进入 `on_b_to_s4`，再进入 `on_c_exit`，最后 `class2_done` 或 `class3_done`。
- 如果卡在 `holding_at_s2`，查 S2 是否释放、B 带是否被前一个包裹占用。
- 如果卡在 `holding_at_s4`，查 S4 是否释放、C 带是否被前一个包裹占用。
- 如果 C 带完成很早或很晚，查编码器距离、`c_exit_distance_mm`、`c_min_busy_ms` 和 `c_fallback_busy_ms`。

### S2 移交后全停

现象：日志反复出现 `PKG,...state=holding_at_s2`，同时 `POSES` 或 `MOTOR` 显示 M1/M2/M3 都是 stop；上位机 pending 为 `waiting_s2_ack`。

处理：

- 先确认 S2 被遮挡时板端有 `STATUS,...reason=s2_held` 或 `a_to_b`。
- 当 B 带释放后，板端应该输出 `MOTOR,id=1,dir=forward,...`，用 M1 把 S2 处包裹带离传感器。
- 如果 M1 正转后 S2 仍不释放，查 S2 安装位置、传感器有效电平、包裹是否卡在交接点。
- 如果 M1 正转命令后立刻又变成 stop，优先怀疑调度覆盖问题；当前版本已加回归测试防止这个问题复发。
- S2 释放后应出现 `STATUS,...reason=s2_clear_delay`，延迟结束后出现 `s2_released`，随后 M2 根据分类正转或反转。

## 连续进件丢包或堵塞

可能原因：

- 进件间距小于包裹长度加安全间距。
- A 带速度过高，S2 触发后包裹来不及稳定停靠。
- B/C 带速度过低，后级占用时间太长。
- 传感器抖动导致重复触发或提前释放。

处理：

- 先把自动进件间隔调大，确认单包和双包稳定。
- 降低 A 带速度或提高 B/C 带速度。
- 增大 `handoff_delay_ms`，让交接释放更稳。
- 如果真实传感器抖动，先从硬件滤波、遮光安装和固定支架解决，再考虑软件滤波。

## 上位机连接异常

如果串口模式正常，但 TCP 模式日志反复出现连接失败：

- 确认上位机监听地址和板端目标地址一致。
- 当前板端默认尝试连接 `192.168.10.1:5000`。
- 若 PC 没有配置该网段，日志中的连接失败是预期现象，不影响串口调试。

## 串口命令偶发 unknown command

现象：日志出现类似 `IN ISION_FRAME...`，命令首字节丢失，随后板端返回 `unknown_command`。

处理：

- 确认使用最新上位机代码；串口打开后会清空缓冲并在发送行之间加入最小间隔。
- 如果仍出现，降低自动进件速度或增大发送间隔，避免视觉触发和分类结果背靠背写入。
- 串口线过长或接触不良时也可能出现丢字节，先换短线并避免 USB 集线器。
- 连续压测前先跑一次 3 到 6 个包裹的 headless 串口测试，确认没有 `unknown_command`。

## 每次修改后的最小回归

每次改 GPIO、方向、极性或速度后，至少跑：

```bash
idf.py build
idf.py -p /dev/ttyACM0 flash
```

然后按顺序执行：

```text
HW_STATUS
MOTOR_TEST
test1
test2
test3
```

预期结果：

- `HW_STATUS` 中已配置硬件为 valid。
- `MOTOR_TEST` 三个电机都按预期短转。
- `test1` 输出 `class1_done`。
- `test2` 输出 `class2_done`。
- `test3` 输出 `class3_done`。
- 急停打开后 M1/M2/M3 brake。急停释放恢复暂不作为当前版本验收项。
