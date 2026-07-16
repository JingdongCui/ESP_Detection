# Current Task

## Goal

整理工作区，以 `ESP32P4_Detection(9).zip` 为新的唯一活跃固件工程；其余既有内容归为参考资料；完成构建、烧录和实机启动验证。

## Current State

- 根目录无 `.codegraph/`；新主工程自带 `.codegraph/`，保留为本地可再生成索引。
- 目标包已通过 `unzip -t`：17,796 项，无 CRC 错误。
- 原包保留为 `reference/source_archives/ESP32P4_Detection_9_original.zip`。
- 旧主工程已提交整理前状态 `b79edb4`，并移动到 `reference/firmware/ESP32P4_Detection_before_9_20260716/`。
- 新主工程已解压到 `ESP32P4_Detection/`，当前分支 `feat/screen-uvc-stream`。
- 包内 3 个已跟踪改动已整理为新基线提交 `367e0c7`。
- 按用户要求，已删除新工程内的 Agent/Skills 配套：`AGENTS.md`、`CLAUDE.md`、`.claude/`、`Skills/`、`agentic/` 及相关脚本。
- 包内校名/校徽和演示脚本已移到 `reference/package9_extras/`。
- 原上位机、数据集、历史工程、报告源文件均已分类到 `reference/`。
- 已连接串口：CP2102N 稳定路径，指向 `/dev/ttyUSB0`。
- ESP-IDF：v5.5.4。

## Next Step

1. 运行 `idf.py build`。
2. 使用 CP2102N 稳定路径执行 `idf.py flash`。
3. 执行 `idf.py monitor` 并记录启动、外设、视觉和分拣链路状态。
4. 更新验证记录并提交根工作区整理结果。

## Blockers

- 暂无；实机运行结果待验证。
