# Current Task

## Goal

将 `lasttime_merge` 的电机分拣算法移植到 `new_merge`，保留 `new_merge` 现有 UI 显示内容，并在 `new_merge` 内提供 two-stage 与传统 ROI 两个分支。

## Current State

- `new_merge` 已初始化独立 git，并保留基线分支：
  - `base-before-motor`: `8b53744 checkpoint before motor migration`
- two-stage 分支：
  - `motor-two-stage`: `7a18be7 disable sorter sensors on console uart pins`
- 传统 ROI 分支：
  - `motor-roi`: `3e58b04 add temporary developer debug panel`
- 已移植内容：
  - `components/Sorter_app`
  - BSP 电机、编码器、分拣传感器和 sorter debug config
  - vision 分类到 sorter class 的提交链路
  - 真实硬件调试启动路径，TCP 模拟链路按本轮要求未运行
- UI 边界：
  - two-stage 打包前未改 UI 显示内容。
  - ROI 分支按用户新要求新增临时 `DEV` 开发者入口和硬件调试弹窗，来源参考 `lasttime_my` 硬件调试页。
  - 临时 UI 改动集中在 `components/UI/sdk/developer_debug_ui.*`、`components/UI/sdk/ui.c` 一行挂载、`components/UI/CMakeLists.txt`。
- 硬件引脚调整：
  - GT911 touch INT 从 GPIO32 改为 GPIO24，避免与电机 PWM GPIO32 冲突。
  - 分拣传感器 S1/S3 原占用 GPIO37/GPIO38，与 ESP32-P4 console UART 冲突，已置为 `-1`；S2/S4 保持 GPIO23/GPIO22。
- 当前主 worktree 停在 `motor-roi`。
- 用户删除的模型文件已在 ROI 备份提交中记录：
  - `model/jt30829206.bin`
  - `model/logo.espdl`
  - `model/yd29041906.bin`
  - `model/zt31241902.bin`
- 给队友的 two-stage 压缩包：
  - `/home/kazeform/2026esp/new_merge_motor_two_stage_20260704.tar.gz`
  - 142M，排除了 `build`、`.git`、`.codegraph`、`.cache`，保留 `components` 和 `managed_components`。
- ROI 备份提交：
  - `f792882 backup roi before temporary developer debug ui`
- ROI 临时开发者调试页提交：
  - `3e58b04 add temporary developer debug panel`

## Verification

- `motor-two-stage`: `idf.py build` 通过。
- `motor-roi`: `idf.py build` 通过。
- `motor-roi`: `idf.py -p /dev/ttyUSB0 -b 921600 flash` 通过，app/partition/storage hash verified。
- `motor-roi`: `idf.py -p /dev/ttyUSB0 monitor` 使用 115200 运行期波特率可读。
- 实机启动日志确认：
  - ESP32-P4 revision v1.0。
  - GT911 不再阻塞启动。
  - `vision started`。
  - `SORTDBG ready`。
  - S1/S3 `GPIO is not configured`，S2/S4 正常配置。
  - `System initialization done`。
  - 90 秒 monitor 窗口内无新增 panic/reboot。
- 临时开发者调试页加入后：
  - `idf.py build` 通过。
  - `idf.py -p /dev/ttyUSB0 -b 921600 flash` 通过。
  - `idf.py -p /dev/ttyUSB0 monitor` 到 `System initialization done`，90 秒窗口未见 panic/reboot。
  - 串口有模型自检 `Test output box0 does not match` 错误，但继续启动；该现象不由调试 UI 引入。

## Immediate Next Step

- 若继续实物分拣联调，需要确认 S1/S3 后续实际可用 GPIO；当前为了保住串口监控，S1/S3 已禁用。
- 若需要清理临时开发者入口，删除 `developer_debug_ui.*`，从 `ui.c` 移除 `ui_developer_debug_attach()`，并从 `components/UI/CMakeLists.txt` 移除该源文件和不再需要的 `Sorter_app` 依赖。
- 若需要验证 two-stage 实机启动，需要切到 `motor-two-stage` 后单独 flash/monitor。

## Blockers

- 无代码阻塞。
- 真实 S1/S3 传感器线位未定，当前不能验证完整四传感器链路。
