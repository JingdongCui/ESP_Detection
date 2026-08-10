# 2026-07-04 Merge UI Dependency And Docs Archive

## Scope

- 完善 `docs/agent/MERGE_EXPERIENCE.md`，为下一阶段类似合并准备可复用经验。
- 收口 `merge` 当前阶段：
  - 修复硬件调试模式下 UI CPU/内存/运行时间不刷新。
  - 收窄依赖版本，避免无关组件跟随升级。
  - 按旧版 ESP32-P4 板配置 rev 范围和 CPU 频率。
- 整理 `docs/agent/PROJECT.md`、`CURRENT.md`、`HISTORY.md`，归档当前阶段。

## Merge Code Checkpoints

- 修改前 checkpoint：
  - `0b58a89 checkpoint before ui dependency state fix`
- 本阶段完成提交：
  - `84c9c72 fix ui monitor and pin merge dependencies`

## Root Docs Checkpoints

- 前一轮经验文档扩展 checkpoint：
  - `b4201eb checkpoint before expanding merge experience`
- 前一轮经验文档扩展提交：
  - `d221428 Expand merge experience notes`
- 本轮归档前 checkpoint：
  - `df8e78f checkpoint before archiving merge ui dependency stage`

## UI Refresh Root Cause

- `SORTER_HARDWARE_DEBUG_MONITOR=1` 时，`main/system_init.c` 之前会跳过 `system_monitor()`。
- UI CPU/内存/运行时间刷新依赖 `system_monitor()` 后台任务发送 `EVT_SYSTEM_MONITOR_CHANGED`。
- 因此硬件调试模式下屏幕系统状态不刷新，不是 UI 库本身坏了。

## UI Refresh Fix

- `System_Init()` 现在无论是否硬件调试模式都调用 `system_monitor()`。
- `components/system_monitor/system_monitor.c` 增加 `MONITOR_SERIAL_CPU_LOG_ENABLE=0`，关闭周期性 CPU INFO 表。
- `main/system_init.c` 把 `ISP_AWB` 日志降到 ERROR，避免硬件调试时串口被 warning 刷屏。
- 用户现场观察确认 UI 已恢复刷新。

## Dependency State

- 结论：不是本地全局组件缓存坏了，而是 `merge` manifest 约束太松导致无关组件被解析到较新版本。
- 保留 `esp-dl 3.3.6`，因为 `findlogo.espdl` 在 `esp-dl 3.3.2` 上实机加载触发 Load access fault / Guru Meditation。
- 当前 `merge` 关键版本：
  - `esp-dl 3.3.6`
  - `dl_fft 0.4.0`
  - `esp_new_jpeg 1.0.2`
  - `esp_lv_decoder 0.4.3`
  - `esp_cam_sensor 2.2.0`
  - `esp_video 2.2.0`
  - `esp_ipa 2.1.0`
  - `usb_host_uvc 2.5.0`
  - `esp_lcd_ek79007 1.0.4`
  - `esp_lvgl_adapter 0.4.3`
- `idf.py reconfigure` 已重新生成 `dependencies.lock`，版本落在上述清单。

## Board Configuration

- 当前按旧版 ESP32-P4 处理：
  - `CONFIG_ESP32P4_SELECTS_REV_LESS_V3=y`
  - `CONFIG_ESP32P4_REV_MIN_FULL=0`
  - `CONFIG_ESP32P4_REV_MAX_FULL=199`
  - `CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ=360`
- `sdkconfig` 和 `sdkconfig.defaults` 均已同步。
- 实机烧录识别：
  - `Chip is ESP32-P4 (revision v1.0)`
  - boot log: `chip revision: v1.0`
  - boot log: `efuse_init: Min chip rev: v0.0`
  - boot log: `efuse_init: Max chip rev: v1.99`
  - boot log: `cpu freq: 360000000 Hz`

## Validation

- `idf.py reconfigure`：成功，依赖解析到目标版本。
- `idf.py build`：成功。
- `idf.py -p /dev/ttyUSB0 -b 921600 flash`：成功，bootloader/app/partition/storage hash verified。
- `idf.py -p /dev/ttyUSB0 monitor`：
  - 运行期使用 115200 可读。
  - 启动到 `vision started`、`SORTDBG ready`、电机 BSP 初始化。
  - 未见 Guru Meditation / LoadProhibited。
  - 串口不再被 `ISP_AWB` warning 或 sysmon CPU 表刷屏。
- 未在本轮执行 TCP 20 包回归；本轮主要改 UI 数据源、依赖锁和旧板配置。下一阶段合并前建议先用当前基线跑一次 TCP 20 包作为新 baseline。

## Merge Experience Updates

- `docs/agent/MERGE_EXPERIENCE.md` 已补充：
  - 当前 `merge` 基线。
  - 当前钉住的依赖版本清单。
  - `system_monitor()` 作为 UI 数据源与串口刷屏的拆分原则。
  - 旧板 rev v1.0 / 360MHz 验证事实。
  - `managed_components` 被判定 modified 时的处理方式。
- `docs/agent/PROJECT.md` 已补充长期有效事实：
  - 最新 `merge` 提交。
  - 旧板配置和实机验证。
  - 依赖收窄版本清单。

## Next Stage Preparation

- 下一阶段合并前建议从 `merge` 提交 `84c9c72` 起步。
- 先跑空基线：

```bash
cd /home/kazeform/2026esp/merge
idf.py build
idf.py -p /dev/ttyUSB0 -b 921600 flash
idf.py -p /dev/ttyUSB0 monitor
```

- 若需要模拟器验证，再跑 TCP 20 包和 `log_audit`：

```bash
cd /home/kazeform/2026esp
python -m esp32_sorter_sim_py --headless --transport tcp --host 192.168.10.1 --port 5000 --count 20 --class-sequence 1,2,3,2,3,1 --feed-interval 0.25 --max-active 8 --timeout 180 --stall-timeout 18 --b-idle-threshold 3 --motor-a-speed 60 --motor-b-speed 60 --motor-c-speed 60 --log-file esp32_sorter_sim_py/logs/<name>.log
python -m esp32_sorter_sim_py.log_audit esp32_sorter_sim_py/logs/<name>.log
```

