# Current Task

## Goal

打断 TCP 上位机对齐任务，先处理 `new_merge` 板端 revision/串口/传感器占用：
- 降低 ESP32-P4 revision 支持，兼容低 revision 板。
- 恢复 UART console，方便烧录后 monitor。
- 只停用占用串口 GPIO37/38 中当前实际占用的传感器 GPIO38，不把所有传感器置 `-1`。

## Current State

- `new_merge` 分支：`motor-roi`。
- 修改前已在 `new_merge` 提交现场 checkpoint：
  - `9c4be17 checkpoint before revision and serial config`
- 已实现：
  - `sdkconfig` / `sdkconfig.defaults` 改为 ESP32-P4 low revision 路径：
    - `CONFIG_ESP32P4_SELECTS_REV_LESS_V3=y`
    - `CONFIG_ESP32P4_REV_MIN_0=y`
    - `CONFIG_ESP32P4_REV_MIN_FULL=0`
    - `CONFIG_ESP_REV_MIN_FULL=0`
    - `CONFIG_ESP32P4_REV_MAX_FULL=199`
    - `CONFIG_ESP_REV_MAX_FULL=199`
  - UART console 恢复为 UART0、115200，并保留 secondary USB Serial/JTAG。
  - sorter sensor 当前配置：
    - S1=GPIO53
    - S2=GPIO23
    - S3=-1
    - S4=GPIO22
- 注意：低 revision Kconfig 会联动默认 CPU 频率到 360MHz。

## Verification

- `cd /home/kazeform/2026esp/new_merge && idf.py build`：通过。
- 低 revision build 使用 `<3.0` ROM linker scripts，生成 app：`build/sample_project.bin`，大小 `0x4ec5e0`，factory 分区剩余约 18%。
- USB 枚举能看到 `10c4:ea60 Silicon Labs CP210x UART Bridge`，但当前没有 `/dev/ttyUSB*`、`/dev/ttyACM*` 或 `/dev/serial/by-id` 节点。
- `dmesg` 需要权限，当前无法读取；`cp210x/usbserial` 未显示已加载。
- 因无 tty 节点，暂无法执行 `idf.py flash monitor`。

## Immediate Next Step

- 处理系统串口枚举后执行：
  - `idf.py -p <port> -b 921600 flash`
  - `idf.py -p <port> monitor`
  - 确认 boot log 中 min/max chip revision 和 UART console 输出。

## Blockers

- 板子 USB 已枚举为 CP210x，但系统未生成 tty 节点，无法烧录/监控。
