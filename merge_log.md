/plan 现在给 merge_project 做调试适配。先检查现有的编码器 电机 光电传感器的api.然后要专门在一个文件里面调整调试开关，使用宏定义来调，调整各个硬件的接口，所有传感器的极性正反，每个电机的正转与反转。和一些编码器的设置。然后加一些变量，在ui上面能够控制一些调试参数，能够开关电机控制，调整每个电机的速度，调整安全延时。要有按钮调整超时时间，就是当传感器丢失信号的时候 超时时间的设定，有加减功能，每个传送带对应一个超时时间，默认为2s 0.75s 0.75s。模拟分类各类包裹的按钮，按下之后 电机模拟识别到了某类包裹，然后做对应的电机控制。对于以太网调试链路，在ui上加一个开关功能，开的时候接收上位机的模拟传感器和识别 关闭的时候使用板子上的硬件。ui上要加关闭传感器或开启传感器的切换，关闭后全部用定时功能，以设定的超时时间为准。这样做 有个三模式切换 以太网链路，真实传感器链路，纯计时+电机控制链路。对于超时时间 和安全延时的逻辑，你处理好。不清楚的地方 提前问好。改动前提交git,在merge_project文件夹下建立git仓库 然后提交第一版。你能读到串口，改完烧录。

现在的任务：审查电机分拣链路，实际硬件调试链路。现在我没有接电机和传感器等。你需要尽力确保我接上硬件后能够顺利调试  不出
  问题，为此你要尽可能多地做提前测试。比如测试驱动电机，然后尝试读引脚电平；检查各种可能出现的问题。你可以跑上位机进行测
  试，从而获得更多的信息 确保更多的功能正确。不确定的提前问清楚。



# Merge Log: Sorter Motor Control

Date: 2026-06-23

## Goal

Move the sorter motor scheduling logic from `ESP32P4_Detection` into `merge_project`, then verify the target project with the Python headless upper-computer simulation for 20 packages.

## Files Merged

- Copied sorter core headers into `components/Ethernet_app/include/sorter_core/`:
  - `SorterTypes.hpp`
  - `SorterScheduler.hpp`
  - `ReservationManager.hpp`
  - `CollisionGuard.hpp`
- Copied sorter core implementation:
  - `components/Ethernet_app/sorter_core/SorterScheduler.cpp`
- Copied text protocol adapter:
  - `components/Ethernet_app/include/sorting_sim_control.h`
  - `components/Ethernet_app/sorting_sim_control.cpp`
- Merged Ethernet simulation transport support into:
  - `components/Ethernet_app/ethernet_app.c`
  - `components/Ethernet_app/CMakeLists.txt`

## Integration Notes

- `ethernet_app.c` now supports `ESP_HOST_TYPE_SIM_LINE = 0x12`.
- Incoming `SIM_LINE` packets are passed to `sorting_sim_control_handle_line(...)`.
- The TCP task calls `sorting_sim_control_tick(...)` periodically.
- Scheduler output lines such as `MOTOR`, `STATUS`, `PKG`, and `FAULT` are sent back to the host as `SIM_LINE` packets.
- Image sending is disabled for the sorter simulation build by `SORTING_SIM_TCP_SEND_IMAGES=0`, so the TCP link is used mainly for metrics and sorter protocol lines.
- `components/Ethernet_app/CMakeLists.txt` now builds the C++ scheduler and enables C++17.

## Conflicts And Boundaries

- `merge_project` already had Ethernet, camera, metrics, and time-sync code. The source project's sorter-enabled `ethernet_app.c` was based on the same structure, so the merge was a direct mechanical sync.
- `merge_project/components/bsp/bsp_motor.c` still has motor GPIO macros set to `-1`. Real motor PWM output is not validated in this run.
- The simulation validation depends on `MOTOR,...` text output consumed by the upper-computer simulator. Real hardware direction/speed calibration remains a separate BSP task.
- The current sorter recovery behavior still depends on explicit `PACKAGE_LOST,id=...` for B-belt loss handling. This baseline run did not exercise loss recovery.

## Build And Flash

Build command:

```bash
cd /home/kazeform/2026esp/merge_project
idf.py build
```

Result:

```text
Project build complete.
sample_project.bin binary size 0x318050 bytes.
Smallest app partition is 0x600000 bytes. 0x2e7fb0 bytes (48%) free.
```

Flash command:

```bash
cd /home/kazeform/2026esp/merge_project
idf.py -p /dev/ttyACM0 flash
```

Result:

```text
Hash of data verified.
Leaving...
Hard resetting via RTS pin...
Done
```

## Simulation Validation

Command:

```bash
cd /home/kazeform/2026esp
python -m esp32_sorter_sim_py --headless --transport tcp --host 192.168.10.1 --port 5000 --count 20 --class-sequence 1,2,3,2,3,1 --feed-interval 0.25 --max-active 8 --timeout 180 --stall-timeout 18 --b-idle-threshold 3
```

New log:

```text
esp32_sorter_sim_py/logs/sorter_sim_20260623_204420.log
RESULT ok completed=20/20
```

Anomaly scan command:

```bash
rg -n "POSE_ASSERT|DESYNC|FAULT|RESULT fail|\bstall\b|legacy_tx|unknown_command|board_absent|reason=timeout|DESYNC: board advanced" \
  esp32_sorter_sim_py/logs/sorter_sim_20260623_204420.log
```

Result: no matches.

## Baseline Comparison

Baseline log:

```text
esp32_sorter_sim_py/logs/sorter_sim_20260623_195320.log
```

Compared with new log:

| Metric | Baseline | New merge run |
| --- | ---: | ---: |
| `RESULT ok completed=20/20` | 1 | 1 |
| `PKG state=done` | 20 | 20 |
| `reason=class1_done` | 7 | 7 |
| `reason=class2_done` | 7 | 7 |
| `reason=class3_done` | 6 | 6 |
| `reason=s4_released` | 13 | 13 |
| `RX MOTOR` | 115 | 116 |
| `reason=a_to_b` | 152 | 163 |
| `reason=b_to_c` | 108 | 112 |

The completion distribution and required safety scans match the known-good baseline. Minor command/status count differences are expected from runtime timing and repeated tick snapshots.

## Final Status

The sorter scheduling logic has been merged into `merge_project`, builds successfully, flashes successfully, and passes the 20-package upper-computer simulation validation.
