# 2026-07-16 第 9 包工作区整理

## 目标

整理工作区，以 `ESP32P4_Detection(9).zip` 为新的唯一活跃固件工程，其余既有内容归为参考资料，并准备构建、烧录和实机启动验证。

## 已完成

- 根目录无 CodeGraph；任务开始时已读取 `PROJECT.md`、`CURRENT.md`、`HISTORY.md`。
- 根 Git 整理前 checkpoint 为 `21b121d`。
- 旧 `ESP32P4_Detection` 的 `main/system_init.c` 存在未提交改动，先提交为 `b79edb4 chore: checkpoint before workspace reorganization`。
- `unzip -t ESP32P4_Detection(9).zip` 通过，共 17,796 项且无 CRC 错误。
- 原包保存在 `reference/source_archives/ESP32P4_Detection_9_original.zip`。
- 旧主工程移动到 `reference/firmware/ESP32P4_Detection_before_9_20260716/`。
- 建立 `reference/` 分类目录并移动旧固件、上位机、数据集、历史工程、报告源文件、笔记和原始压缩包。
- 第 9 包解压为新的 `ESP32P4_Detection/`，活跃分支为 `feat/screen-uvc-stream`。
- 第 9 包原 Git HEAD 为 `cd9e591`；包内已有 `ui.c`、`system_init.c`、`sdkconfig` 改动。
- 将 CRLF 统一为 LF，并建立导入基线提交 `367e0c7 chore: establish package 9 development baseline`。
- 按用户要求删除新工程内的 `AGENTS.md`、`CLAUDE.md`、`.claude/`、`Skills/`、`agentic/`、`esp-agent.sh`、`backtest_run.py`、`test-full-toolchain.sh`。
- 保留新工程 `.codegraph/` 作为本地索引，并在工程 `.gitignore` 中忽略 CodeGraph 和编辑器本地配置。
- 校名、校徽和演示脚本移动到 `reference/package9_extras/`，原压缩包仍可完整恢复所有内容。
- 根工作区整理结果最终提交为 `e8616ec chore: checkpoint package 9 workspace reorganization`。

## 当时环境

- 已连接 CP2102N 串口，稳定路径指向 `/dev/ttyUSB0`。
- ESP-IDF 版本为 v5.5.4。

## 未执行项

- 原计划继续执行 `idf.py build`、`idf.py flash` 和 `idf.py monitor`。
- 用户随后切换到电机分拣算法文档任务，因此上述构建、烧录和实机验证未在该阶段执行；后续如需发布或验证第 9 包固件，应继续完成这三项。
