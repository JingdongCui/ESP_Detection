# Current Task

## Goal

将 `new_merge` 切到两阶段模型分支并跟进最新 TCP/JPEG/分类/硬件配置，生成给队友接手上位机和移植 ESP 改动的交接压缩包。

## Current State

- `new_merge` 当前分支：`motor-two-stage`。
- ESP 差异基线：`7a18be7 disable sorter sensors on console uart pins`。
- ESP 最新提交：`a390dc3 sync two-stage sorter s1 pin`。
- 上位机分支：`esp32_host_no_inference/tcp-board-align`。
- 上位机最新提交：`9728df5 polish package data display`。
- 已生成交接包：
  - `/home/kazeform/2026esp/two_stage_host_handoff_20260705.tar.gz`
  - `/home/kazeform/2026esp/two_stage_host_handoff_20260705.tar.gz.sha256`
- 包内容：
  - 最新 two-stage ESP 工程。
  - 最新上位机工程。
  - `docs/README.md`
  - `docs/ESP_MIGRATION_GUIDE.md`
  - `docs/HOST_HANDOFF.md`
  - `docs/DIFF_REPORT.md`
  - ESP 和 host 两份完整 patch。

## Verification

- ESP：
  - `idf.py build` 通过，app size `0x4ecd30`，factory 分区剩余约 18%。
  - `idf.py -p <stable-port> -b 921600 flash` 完整烧录通过，bootloader/app/partition/storage hash verified。
  - 同步 S1 后 `idf.py -p <stable-port> -b 921600 app-flash` 通过，app hash verified。
  - monitor 确认 ESP32-P4 revision `v1.0`，app min/max `v0.0/v1.99`，UART0 115200 正常。
  - monitor 确认 S1=GPIO53、S2=GPIO23、S3 disabled、S4=GPIO22，启动到 `System initialization done`，未见 panic/reboot。
- 上位机：
  - `cmake --build --preset debug` 通过。
- 打包：
  - tar.gz 生成成功，大小约 140 MB。
  - tar 内容确认包含 docs 和 patches，排除 `.git`、`build`、`.codegraph`、`.qtcreator` 和本地调试帧。
  - SHA256: `af66e056a34f3aac8d1e09d0a97e95eaf16ca622fed8da84db960d3d04088fd6`。

## Notes

- monitor 时未启动上位机，所以 control/image connect failed 日志是预期重连行为，不代表 TCP 默认关闭。
- 交接包不包含 ROI 分支、ROI 专属开发者页面或 ROI 模型改动。
- ESP patch 基于 two-stage `7a18be7..a390dc3`。
- Host patch 基于 `f9ae6d9..9728df5`。

## Next Step

- 将交接包发给队友，队友优先阅读 `docs/README.md` 和 `docs/ESP_MIGRATION_GUIDE.md`。

## Blockers

- 未阻塞。
