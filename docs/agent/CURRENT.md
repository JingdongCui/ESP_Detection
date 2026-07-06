# Current Task

## Goal

在 `ESP32P4_Detection` 中启用开机分拣链路，并把电机默认速度/默认延时集中到与引脚相同的配置文件，方便现场修改。

## Current State

- 当前活跃 ESP 工程：`/home/kazeform/2026esp/ESP32P4_Detection`
  - 分支：`feat/screen-uvc-stream`
  - zip 导入/精简基线：`23bac51 baseline recovered teammate project`
  - 超时修改提交：`71e63c6 tune sorter belt timeouts`
  - 开机自动启用分拣链路提交：`3af4d57 enable sorter autostart`
  - 当前默认分拣配置集中在 `components/bsp/include/sorter_debug_config.h`
    - 默认速度：A/B/C = `100%`
    - 默认交接延时：`1000ms`
    - 默认皮带超时：A=`4500ms`、B=`2000ms`、C=`2000ms`
    - 默认 lost timeout：min=`3000ms`、max=`6000ms`
  - `components/Sorter_app/sorter_core/sorter_scheduler.c` 的 `sorter_config_default()` 已改为读取上述宏。
- 当前活跃上位机工程：`/home/kazeform/2026esp/esp32_host_no_inference`
  - 分支：`master`
  - zip 导入基线：`9a5e5f2 baseline teammate host project`
- 历史工程已移动到 `archive_project/`：
  - `new_merge_before_zip_20260706`
  - `esp32_host_no_inference_before_zip_20260706`
  - `lasttime_merge_before_zip_20260706`
  - `lasttime_my_before_zip_20260706`
  - `lasttime_teammate_before_zip_20260706`
  - `teammate_project_before_zip_20260706`
  - `ignore_before_zip_20260706`
  - `esp32_sorter_sim_py_before_zip_20260706`
- 已删除/排除的可再生成或本地缓存：
  - 各工程 `build/`
  - 各工程 `managed_components/`
  - `.codegraph/`
  - `.cache/`
  - `.qtcreator/`
  - `__pycache__/`
- `ESP32P4_Detection(8).zip` 不是完整标准 zip，`unzip` 报缺少 central directory，`7z` 报 `Unexpected end of archive`。
  - 已用 7z 尽量恢复源码。
  - 使用恢复出的 `.git` 从 HEAD 补回构建必需的 `model/`、`sdkconfig`、`sdkconfig.defaults`、`partitions.csv`。
  - 因此当前 `ESP32P4_Detection` 是“zip 恢复内容 + git 补齐构建文件 + 精简无关工具目录”的可构建工程。

## Verification

- 2026-07-06 开机自动启用分拣链路后：
  - `idf.py build` 通过。
  - `/dev/serial/by-id/usb-Espressif_USB_JTAG_serial_debug_unit_E8:F6:0A:E1:7A:D1-if00` flash 成功。
  - 芯片：ESP32-P4 revision `v3.1`。
  - bootloader、app、partition、storage 均 hash verified。
  - monitor 已看到 app version `cf81a8e-dirty`、min/max chip rev `v3.1/v3.99`、PSRAM 32 MB、camera/LVGL 初始化；后续被 `ISP_AWB` warning 刷屏，用户中断，不作为完整运行期验证。
- 2026-07-06 默认速度/默认延时集中配置后：
  - `idf.py build` 通过。
  - app version：`a82793b`
  - app size：`0x526180`
  - factory 分区剩余：`0xd9e80`，约 14%。
  - 用户要求本次不烧录。

## Next Step

- 如需现场修改速度/延时/引脚，优先改 `ESP32P4_Detection/components/bsp/include/sorter_debug_config.h`。
- 下次需要实机时再烧录，并重点验证 A/B/C 速度 100% 下的节拍和稳定性。

## Blockers

- 原始 ESP zip 文件不完整；当前工程已可构建烧录，但仍建议让队友后续重新发一份完整 zip 或直接同步 git 仓库。
