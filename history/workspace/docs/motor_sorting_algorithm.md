# 电机分拣算法与状态机实现说明

> 适用工程：`ESP32P4_Detection` 第 9 包开发基线（固件提交 `367e0c7`）  
> 编写日期：2026-07-16  
> 说明范围：视觉识别结果进入分拣调度器后，如何结合 S1～S4 传感器、三段传送带和三台电机完成分拣。

## 1. 先回答核心问题：状态机是谁的状态

主状态机是**包裹对象的状态机**，不是传送带状态机。

源码证据如下：

- 状态枚举 `sorter_package_state_t` 的每个值都描述一个包裹当前处于哪个处理阶段，例如“等待视觉结果”“停在 S2”“沿 B 段前往 1 类出口”。
- 每个包裹由 `sorter_package_track_t` 表示，该结构体内部直接保存 `state`、`cls`、`belt`、进入状态的时间和超时时间。
- 全局调度器 `sorter_scheduler_t` 内有 `tracks[8]`，所以最多可以同时跟踪 8 个包裹，每个包裹都有自己的状态。
- 传送带没有单独的 `BELT_STATE_*` 状态机。传送带相关信息由包裹的 `belt` 字段、B/C 段占用者 `b_owner`/`c_owner`，以及最近一次电机命令共同表示。

完整地说，系统有三层状态：

| 层次 | 状态归属 | 关键字段 | 用途 |
|---|---|---|---|
| 包裹状态 | 每一个包裹对象 | `track.state`、`track.cls`、`track.belt` | 表示包裹走到哪个工序，是分拣算法的主状态机 |
| 共享资源状态 | B、C 传送带资源 | `b_owner`、`c_owner`、`b_direction`、`c_direction` | 防止不同包裹同时争用同一段传送带 |
| 调度器全局状态 | 整套分拣系统 | `estop`、`paused_after_estop` | 表示正常运行、急停或急停后暂停 |

因此，比较准确的表述是：

> 本工程采用“每包裹一个有限状态机 + 全局调度器统一仲裁传送带资源”的实现。传送带是状态机使用的共享执行资源，不是主状态机的对象。

相关定义位于：

- `components/Sorter_app/include/sorter_core/sorter_scheduler.h`：状态枚举、包裹跟踪结构和调度器结构。
- `components/Sorter_app/sorter_core/sorter_scheduler.c`：所有状态迁移、资源预约、超时处理和电机命令生成。

## 2. 物理对象与软件编号

系统把机械输送路径抽象为三段传送带和四个传感器：

| 软件对象 | 物理作用 | 调度器电机 ID | BSP 电机下标 |
|---|---|---:|---:|
| A 段 / 电机 A | 入料、经过视觉区、送至 S2 | 1 | 0 |
| B 段 / 电机 B | 从 S2 分流：正向去 1 类出口，反向去 S4 | 2 | 1 |
| C 段 / 电机 C | 从 S4 再分流：反向去 2 类出口，正向去 3 类出口 | 3 | 2 |

传感器职责：

| 传感器 | 算法中的作用 |
|---|---|
| S1 | 检测新包裹进入，创建包裹 ID，并打开该包裹的视觉识别窗口 |
| S2 | 确认包裹到达 A/B 交接点；先停 A，预约 B，待包裹离开 S2 后启动 B |
| S3 | 确认 1 类包裹到达出口，停止 B 并结束该包裹 |
| S4 | 确认 2/3 类包裹到达 B/C 交接点；先停 B，预约 C，待包裹离开 S4 后启动 C |

注意调度层使用 1～3 编号，而 `bsp_motor_set_speed_direction()` 使用 0～2 下标。转换发生在 `apply_motor_event()` 中：`motor_id - 1`。

## 3. 核心数据结构

### 3.1 单个包裹：`sorter_package_track_t`

每个活动包裹占用 `tracks[]` 中的一个槽位，重要字段如下：

| 字段 | 含义 |
|---|---|
| `id` | 包裹唯一编号，由 S1 上升沿创建时递增 |
| `cls` | 视觉分类结果：1、2、3、空闲、错误或视觉失败 |
| `state` | 当前包裹状态，是主状态机变量 |
| `belt` | 当前归属的 A/B/C 段 |
| `pos_mm` | 软件估算位置；当前主要用于 A 段安全间距判断 |
| `state_enter_ms` | 进入当前状态的时间 |
| `state_timeout_ms` | 当前状态对应的超时时间 |
| `handoff_ready_ms` | 包裹离开 S2/S4 后，允许启动下一段的时刻 |
| `c_start_dist_mm` | 包裹进入 C 段时记录的编码器距离基准 |
| `c_busy_until_ms` | C 段最短占用结束时刻 |
| `c_fallback_until_ms` | C 段编码器失效时的兜底结束时刻 |
| `occupied` | 该槽位是否正在跟踪一个包裹 |

`allocate_package()` 找到空槽后清零结构体、写入 `id` 并将 `occupied` 置为真；`release_package()` 在完成或出错后释放资源并清零整个槽位。

### 3.2 全局调度器：`sorter_scheduler_t`

调度器保存：

- `tracks[8]`：最多 8 个并行包裹状态机；
- `b_owner`、`c_owner`：B/C 段当前预约给哪个包裹，0 表示空闲；
- `s2_held_package_id`、`s4_held_package_id`：因为下游忙而被暂扣在交接点的包裹；
- `last_commands[3]`：三台电机最近一次命令，用于去重，也用于判断某段是否实际处于运行状态；
- `failed_class_cursor`：视觉失败时按 1、2、3 循环分配的游标；
- `estop`、`paused_after_estop`：全局急停状态。

这就是为什么状态机不能理解为“传送带 A 状态机”：A 段上可能同时有多个包裹，它们各有自己的 `state`，调度器再根据间距与下游占用情况决定 A 是否运行。

## 4. 包裹状态定义和转换

### 4.1 状态含义

| 状态 | 含义 | 所在传送带 |
|---|---|---|
| `EMPTY` | 空槽位，不代表实际包裹 | 无 |
| `WAITING_VISION` | 已由 S1 创建，等待视觉分类 | A |
| `WAITING_AB` | 已取得分类，继续前往 S2 | A |
| `HOLDING_AT_S2` | 已到 S2，等待或正在进行 A→B 交接 | A |
| `ON_B_TO_CLASS1` | B 正转，包裹前往 1 类出口/S3 | B |
| `ON_B_TO_S4` | B 反转，包裹前往 S4 | B |
| `HOLDING_AT_S4` | 已到 S4，等待或正在进行 B→C 交接 | B |
| `ON_C_EXIT` | C 正转或反转，包裹前往 2/3 类出口 | C |
| `DONE` | 分拣完成；发出事件后立即释放跟踪槽位 | 结束 |
| `ERROR` | 不可处理的分类错误；发出事件后立即释放槽位 | 结束 |

所有正常状态修改都通过 `transition_state()` 完成。该函数不仅写 `p->state`，还会重置进入时间，并调用 `timeout_for_state()` 为新状态装载对应超时值。

### 4.2 状态转换总图

```text
S1 激活
  │
  ▼
WAITING_VISION ──视觉成功/视觉失败轮转──► WAITING_AB
  │                                              │
  │ 分类为 ERROR                                 │ S2 激活或 A 超时
  ▼                                              ▼
ERROR                                      HOLDING_AT_S2
                                                 │
                            S2 释放 + 交接延时到期
                  ┌──────────────────────────────┴─────────────────────────────┐
                  │ 类1 / FREE                    类2 / 类3                    │
                  ▼                                                           ▼
          ON_B_TO_CLASS1                                                ON_B_TO_S4
                  │ S3 激活或 B 超时                                          │ S4 激活或 B 超时
                  ▼                                                           ▼
                DONE                                                   HOLDING_AT_S4
                                                                                │
                                                           S4 释放 + 交接延时到期
                                                                                ▼
                                                                         ON_C_EXIT
                                                                                │
                                                         编码器距离/状态超时/兜底
                                                                                ▼
                                                                              DONE
```

这里的“正向/反向”是软件逻辑方向；若某个电机机械安装方向相反，可通过 `SORTER_MOTORx_REVERSE` 在 BSP 层翻转，不需要改调度算法。

## 5. 从开机到真实电机动作的完整调用链

### 5.1 系统启动

入口是 `main/system_init.c` 的 `System_Init()`：

1. `bsp_motor_init()` 初始化三台电机的 MCPWM；
2. `bsp_encoder_init()` 初始化编码器；
3. `vision_start()` 启动视觉采集与推理；
4. `sorting_sim_debug_start()` 启动串口调试任务；
5. `sorting_sim_control_set_motor_output_enabled(true)` 允许真实 PWM 输出；
6. `sorting_sim_control_set_sensor_input_enabled(true)` 允许真实传感器输入。

控制层默认模式是 `SORTING_DEBUG_MODE_REAL_SENSOR`。`apply_settings_locked()` 会调用 `set_real_io_enabled_locked()`，进而执行 `ensure_hardware_observer_locked()`：初始化编码器和分拣传感器，并由 `start_real_io_task_locked()` 创建 `real_io_task`。

### 5.2 S1 创建包裹

`real_io_task()` 每 10 ms 轮询一次 S1～S4：

1. `bsp_sort_sensor_get_level()` 读取 GPIO，并按每个传感器的 active level 转成布尔值；
2. 原始电平保持 20 ms 后，才被接受为稳定变化；
3. `process_real_sensor_event_locked(S1, true)` 收到 S1 激活；
4. `update_vision_s1_locked()` 只在 S1 的非激活→激活边沿调用 `open_vision_window_locked()`，避免一个持续遮挡信号重复建包；
5. `open_vision_window_locked()` 分配递增 ID，并调用 `sorter_scheduler_package_new()`；
6. `sorter_scheduler_package_new()` 分配一个 `track`，设置 `belt=A`，转入 `WAITING_VISION`，然后调用 `schedule()`。

`schedule()` 会决定 A 电机是否运行。默认在没有 S2 阻塞时让 A 正转；有多个 A 段包裹时，还会通过 `can_move_a_package()` 检查前后距离是否大于“包裹长度 + 安全间距”。

### 5.3 视觉结果绑定到当前包裹

视觉任务 `vision_detect_task()` 对每帧执行模型推理，找到最高分的 logo 后调用：

```c
sorting_sim_control_submit_vision_category(category, score);
```

控制层通过 `s_vision_package_id` 把结果绑定到 S1 最近创建、且尚未分类的包裹。具体链路是：

```text
vision_detect_task
  → sorting_sim_control_submit_vision_category
  → vision_category_to_class
  → classify_current_vision_window_locked
  → sorter_scheduler_vision_result
  → transition_state(WAITING_AB)
  → schedule
```

模型类别与分拣类别不是简单的 `category + 1`，当前映射为：

| 模型 category | 公司 | 调度类别 |
|---:|---|---|
| 0 | 极兔 | `SORTER_CLASS_1` |
| 1 | 韵达 | `SORTER_CLASS_3` |
| 2 | 中通 | `SORTER_CLASS_2` |

一个视觉窗口只接受第一次成功分类。`s_vision_classified` 置真后，后续帧即使继续看到同一个包裹也不会重复提交。

如果包裹到达 S2 时仍没有成功视觉结果，`fail_current_vision_window()` 会提交 `SORTER_CLASS_VISION_FAILED`。`normalize_vision_class()` 再调用 `next_failed_class()`，按 1→2→3→1 的顺序轮转分配，保证未识别包裹不会全部堆到同一个出口。

### 5.4 S2 完成 A→B 交接和第一次分流

S2 激活后，`process_real_sensor_event_locked()` 调用 `sorter_scheduler_sensor(..., 2, true, 0)`。真实传感器没有直接携带包裹 ID，所以调度器选择 ID 最小、状态匹配的包裹。

`sorter_scheduler_sensor()` 的 S2 分支执行：

1. 包裹从 `WAITING_AB` 转成 `HOLDING_AT_S2`；
2. 停止 A 电机；
3. `release_s2_to_b()` 尝试用该包裹 ID 预约 B 段；
4. 若 B 已被其他包裹占用，则记录 `s2_held_package_id`，包裹继续停在 S2；
5. 若预约成功，A 被强制正转、B 保持停止，使包裹先跨过 S2 交接区域。

当 S2 从激活变为不激活，说明包裹尾部已经离开 S2。此时设置：

```text
handoff_ready_ms = 当前时间 + handoff_delay_ms
```

`service_pending_transfers()` 在延时到达后调用 `complete_s2_handoff()`：

- 1 类、FREE：`belt=B`，状态转为 `ON_B_TO_CLASS1`，B 正转；
- 2 类、3 类：`belt=B`，状态转为 `ON_B_TO_S4`，B 反转。

### 5.5 1 类出口

1 类包裹在 `ON_B_TO_CLASS1` 状态下等待 S3：

1. S3 激活；
2. `sorter_scheduler_sensor()` 找到最早的 `ON_B_TO_CLASS1` 包裹；
3. 停止 B；
4. `release_reservations()` 释放 B/C 预约；
5. `release_package(..., DONE)` 发出完成事件并清空跟踪槽位；
6. `service_pending_transfers()` 尝试释放之前停在 S2/S4 的下一个包裹；
7. `schedule()` 重新计算 A 段是否可以继续进料。

### 5.6 S4 完成 B→C 交接和第二次分流

2/3 类包裹沿 B 反向运动到 S4。S4 激活后的处理与 S2 类似：

1. `ON_B_TO_S4` → `HOLDING_AT_S4`；
2. 停止 B；
3. `release_s4_to_c()` 尝试预约 C；
4. C 忙则写入 `s4_held_package_id` 并继续等待；
5. C 空闲则让 B 反转、C 暂停，先把包裹送过交接区；
6. S4 变为不激活后开始 `handoff_delay_ms` 延时；
7. `complete_s4_handoff()` 将包裹归属改为 C，状态改成 `ON_C_EXIT` 并启动 C：
   - 2 类：C 反转；
   - 3 类：C 正转。

### 5.7 C 段结束条件

`real_io_task()` 对三路编码器积分得到累计距离。第三路编码器更新时调用：

```c
sorter_scheduler_distance(&s_scheduler, 3, distance_mm);
```

`sorter_scheduler_distance()` 首次记录 C 段距离基准，之后同时满足以下条件才调用 `complete_c_package()`：

- 增量距离达到 `c_exit_distance_mm`，默认 360 mm；
- 当前时间达到 `c_busy_until_ms`，默认至少占用 2500 ms。

另外，`service_state_timeouts()` 提供定时兜底。包裹结束时会停止 C、释放预约、发出 `DONE`，然后继续调度等待中的包裹。

## 6. 电机命令如何真正落到 PWM

调度核心本身不直接操作 GPIO。它通过事件把“意图”和“硬件实现”分开：

```text
调度函数
  → emit_motor(motor_id, direction, speed)
  → dispatch_event
  → scheduler_event
  → apply_motor_event
  → bsp_motor_set_speed_direction(motor_id - 1, speed, direction)
  → bsp_motor_set_compare
  → ESP-IDF MCPWM comparator
  → 两路 PWM GPIO
```

关键函数职责：

| 函数 | 职责 |
|---|---|
| `emit_motor()` | 生成电机事件；与 `last_commands[]` 比较，过滤重复命令 |
| `scheduler_event()` | 接收调度器事件；记录日志，并把电机事件交给硬件适配层 |
| `apply_motor_event()` | 校验开关、ID 和速度，把 STOP/BRAKE 转成速度 0，把正反转转成 BSP 参数 |
| `bsp_motor_set_speed_direction()` | 根据方向选择两路 PWM 中哪一路输出占空比，并处理机械反向配置 |
| `bsp_motor_set_compare()` | 写 MCPWM A/B 比较器，真正改变 GPIO 波形 |

调度器的电机 ID 从 1 开始，BSP 下标从 0 开始。`apply_motor_event()` 中的减 1 是必须保留的边界转换。

## 7. 多包裹并行与资源仲裁

系统不是“完成一个包裹后才能创建下一个”。最多 8 个包裹可以同时存在，但下游传送带必须独占。

### 7.1 B/C 段预约

- `reserve_b()`：只有 `b_owner == 0` 或已经是当前包裹，才能预约 B；
- `reserve_c()`：只有 `c_owner == 0` 或已经是当前包裹，才能预约 C；
- `release_reservations()`：包裹完成或出错时释放它持有的 B/C 资源。

这使不同方向命令不会同时作用到同一台电机。例如一个 1 类包裹要求 B 正转时，后续 2 类包裹不能同时要求 B 反转，只能停在 S2。

### 7.2 暂扣与恢复

- B 忙：包裹停在 S2，写入 `s2_held_package_id`；
- C 忙：包裹停在 S4，写入 `s4_held_package_id`；
- `service_pending_transfers()` 每次传感器事件、状态 tick 或包裹完成后重新尝试释放；
- `first_filtered()` 和 `first_in_state()` 优先选择较小的包裹 ID，形成近似先到先服务顺序。

### 7.3 A 段节拍控制

`schedule()` 综合三个条件控制 A：

1. S2 是否有无法交接的包裹；
2. A 段是否有可移动包裹；
3. 包裹间距是否达到 `package_length_mm + safe_gap_mm`。

当前默认包裹长度 120 mm、安全间距 80 mm，即软件希望相邻包裹至少相隔 200 mm。

需要注意：当前 `pos_mm` 没有根据 A 编码器持续积分，只在 S2 到达/超时时写入一个固定位置。因此安全间距逻辑已经存在，但真实运行中的连续位置估计还不完整；当前可靠的节拍边界主要依靠 S2 暂扣和 B/C 资源预约。

## 8. 超时与故障兜底

`real_io_task()` 每 100 ms 对活动调度器调用一次 `sorter_scheduler_tick()`。tick 内部先运行 `service_state_timeouts()`，再处理等待交接，最后调用 `schedule()`。

默认参数来自 `components/bsp/include/sorter_debug_config.h`：

| 参数 | 当前值 |
|---|---:|
| A/B/C 电机速度 | 100% / 100% / 100% |
| 交接延时 | 1000 ms |
| A 状态超时 | 4500 ms |
| B 状态超时 | 2000 ms |
| C 状态超时 | 2000 ms |
| lost timeout 范围 | 3000～6000 ms |

主要超时行为：

- `WAITING_VISION` 超时：按 1、2、3 轮转分配类别，然后继续；
- `WAITING_AB` 超时：按已经到达 S2 处理并尝试交接；
- `ON_B_TO_CLASS1` 超时：按已经到达 1 类出口处理并结束；
- `ON_B_TO_S4` 超时：按已经到达 S4 处理并尝试交接；
- `ON_C_EXIT` 超时：停止 C 并结束包裹。

超时计时只在相关电机确实运行、并且该包裹持有对应资源时推进。若电机因为下游阻塞而停止，代码会把 `state_enter_ms` 刷新到当前时刻，避免“等待资源的时间”被误判成“运输丢件时间”。

### 当前 C 段参数的实现注意点

源码为 C 段设置了 2500 ms 的最短占用时间和 8000 ms 的 fallback，但当前 `belt_c_timeout_ms` 是 2000 ms。通用状态超时会在 2000 ms 结束 `ON_C_EXIT`，早于 2500 ms 的编码器最短占用门槛。

因此按当前默认配置，C 编码器的“360 mm 且至少 2500 ms”完成路径通常来不及触发，包裹会先被 2000 ms 状态超时结束。若希望以编码器距离作为主要结束依据，应把 C 状态超时调大到高于 `c_min_busy_ms`，并结合机械实测重新确定 fallback；这是当前实现需要校准的地方，不应把三个时间参数理解成同时有效的独立保护层。

## 9. 当前硬件配置对算法的实际影响

当前 `sorter_debug_config.h` 中：

- S1=GPIO53、S2=GPIO23、S4=GPIO22；
- S3 GPIO 为 `-1`，即 1 类出口传感器未启用；
- 三路编码器 GPIO 都是 `-1`，即编码器完成条件当前没有真实输入；
- 三台电机 PWM GPIO 已配置，输出默认开启；
- S1～S4 的有效电平均配置为高电平。

由此得到当前实机行为：

- S1、S2、S4 可以走真实传感器路径；
- 1 类包裹无法由 S3 实际触发完成，依赖 B 段 2000 ms 超时兜底；
- 2/3 类包裹无法由 C 编码器距离触发完成，依赖 C 段状态超时兜底；
- 这不是状态机缺少相应分支，而是相应 GPIO 当前被显式配置为未连接。

## 10. 关键函数索引

### 启动与控制层

| 函数 | 文件 | 作用 |
|---|---|---|
| `System_Init()` | `main/system_init.c` | 初始化电机、编码器、视觉并打开分拣输入输出 |
| `ensure_initialized()` | `sorting_sim_control.c` | 装载默认参数并初始化调度器 |
| `apply_settings_locked()` | `sorting_sim_control.c` | 应用模式、速度、超时和硬件开关 |
| `real_io_task()` | `sorting_sim_control.c` | 轮询传感器、积分编码器、周期 tick |
| `process_real_sensor_event_locked()` | `sorting_sim_control.c` | 将真实 S1～S4 信号接入包裹窗口和调度器 |
| `sorting_sim_control_submit_vision_category()` | `sorting_sim_control.c` | 把视觉类别绑定到当前 S1 包裹 |

### 调度核心

| 函数 | 作用 |
|---|---|
| `sorter_scheduler_package_new()` | 创建包裹 track，进入 `WAITING_VISION` |
| `sorter_scheduler_vision_result()` | 保存分类，进入 `WAITING_AB` |
| `sorter_scheduler_sensor()` | 处理 S2/S3/S4 到达和离开事件 |
| `sorter_scheduler_distance()` | 用 C 编码器增量判断 2/3 类包裹完成 |
| `transition_state()` | 统一修改包裹状态并装载状态超时 |
| `schedule()` | 计算 A 段是否应该继续运行 |
| `reserve_b()` / `reserve_c()` | 预约下游传送带 |
| `release_s2_to_b()` / `complete_s2_handoff()` | A→B 两阶段交接 |
| `release_s4_to_c()` / `complete_s4_handoff()` | B→C 两阶段交接 |
| `service_pending_transfers()` | 重试被暂扣的包裹和延时到期的交接 |
| `service_state_timeouts()` | 处理视觉、运输、出口传感器失效等超时 |
| `release_package()` | 发出最终状态、释放预约并清空 track |
| `sorter_scheduler_estop()` | 三台电机刹停，并控制全局急停状态 |

### 硬件输出

| 函数 | 文件 | 作用 |
|---|---|---|
| `emit_motor()` | `sorter_scheduler.c` | 生成去重后的电机命令事件 |
| `scheduler_event()` | `sorting_sim_control.c` | 将调度事件接入日志和硬件输出 |
| `apply_motor_event()` | `sorting_sim_control.c` | 将调度器方向/速度转为 BSP 参数 |
| `bsp_motor_set_speed_direction()` | `components/bsp/bsp_motor.c` | 选择正反转 PWM 通道 |
| `bsp_motor_set_compare()` | `components/bsp/bsp_motor.c` | 写入 MCPWM 比较值 |
| `bsp_sort_sensor_get_level()` | `components/bsp/bsp_sort_sensor.c` | 读取 GPIO 并转成 active 状态 |

## 11. 以一个 2 类包裹为例

假设当前没有其他包裹占用 B/C：

1. S1 激活：创建包裹 `#7`，状态为 `WAITING_VISION`，A 正转；
2. 视觉识别到中通：模型 category=2 映射为 `CLASS_2`，状态变为 `WAITING_AB`；
3. S2 激活：包裹变为 `HOLDING_AT_S2`，A 停止，包裹 #7 预约 B；
4. 调度器让 A 再次正转，把包裹送过 S2，而 B 暂停；
5. S2 释放并经过 1000 ms：包裹归属改为 B，状态变为 `ON_B_TO_S4`，B 反转；
6. S4 激活：状态变为 `HOLDING_AT_S4`，B 停止，包裹 #7 预约 C；
7. 调度器让 B 继续反转，把包裹送过 S4，而 C 暂停；
8. S4 释放并经过 1000 ms：包裹归属改为 C，状态变为 `ON_C_EXIT`，C 反转；
9. C 编码器满足退出距离，或 C 状态超时：停止 C，包裹变为 `DONE`；
10. 清空 #7 的跟踪槽位并释放 B/C，调度器尝试放行在 S2/S4 等待的下一个包裹。

整个过程中，`#7` 的状态连续变化；A/B/C 只根据它和其他包裹的状态收到启停/方向命令。这再次说明主状态属于包裹，而电机和传送带是被调度的资源。

## 12. 调试时建议观察的日志

建议按以下顺序验证：

1. `SORT 传感器S1 稳定变化`：是否建立包裹；
2. `SORT 视觉识别: 包裹#N`：视觉结果是否绑定到正确 ID；
3. `package_new`、`vision`：是否进入 `WAITING_VISION`/`WAITING_AB`；
4. `a_to_b`、`s2_clear_delay`、`s2_released`：A→B 交接是否完整；
5. `class1_done` 或 `b_to_c`：第一次分流是否正确；
6. `s4_clear_delay`、`s4_released`：B→C 交接是否完整；
7. `class2_done`/`class3_done` 或 `timeout_c_exit`：C 段如何结束；
8. `sensor_without_package`、`sensor_missed`、`duplicate_package`、`queue_full`：是否存在传感器顺序、包裹绑定或队列问题。

定位问题时应同时记录“包裹 ID + 包裹 state + b_owner/c_owner + 电机命令”，只看某一台电机是否转动不足以判断调度算法是否正确。
