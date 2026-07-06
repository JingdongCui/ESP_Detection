# Current Task

## Goal

解压队友 2026-07-06 发来的 ESP 和上位机 zip，整理根目录工程，归档旧工程，烧录队友 ESP 工程，并将电机分拣默认超时时间从 `6s/3s/3s` 改为 `4.5s/2s/2s`。

## Current State

- 当前活跃 ESP 工程：`/home/kazeform/2026esp/ESP32P4_Detection`
  - 分支：`feat/screen-uvc-stream`
  - zip 导入/精简基线：`23bac51 baseline recovered teammate project`
  - 超时修改提交：`71e63c6 tune sorter belt timeouts`
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

- `idf.py build` 通过。
  - app version：`71e63c6`
  - app size：`0x50fee0`
  - factory 分区剩余：`0xf0120`，约 16%。
- `idf.py -p /dev/serial/by-id/usb-Silicon_Labs_CP2102N_USB_to_UART_Bridge_Controller_7ee2f3966ac3ee11be78b90f9e1b1c54-if00-port0 flash` 成功。
  - ESP32-P4 revision：`v1.0`
  - bootloader/app/partition/storage 均 hash verified。
- `idf.py monitor` 120 秒窗口验证：
  - app version：`71e63c6`
  - min/max chip rev：`v0.0/v1.99`
  - PSRAM：32 MB，200 MHz
  - camera：SC2336 detected，`1024x600 RGB888`
  - UVC：`screen UVC stream started: default 1024x600 MJPEG q90`
  - Ethernet：`Ethernet Started`
  - 启动到 `System initialization done`
  - 未见 panic/reboot。
- monitor 中 logo 模型自检出现一次 `compare_elements` 不一致错误，但模型继续加载、fixed image tests 继续执行，vision 和系统初始化完成；当前记录为非致命现象。

## Next Step

- 如需继续现场验证，重点看新 UVC 预览、Ethernet 图像链路和真实分拣节拍是否符合 `4.5s/2s/2s`。

## Blockers

- 原始 ESP zip 文件不完整；当前工程已可构建烧录，但仍建议让队友后续重新发一份完整 zip 或直接同步 git 仓库。
