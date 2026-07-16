# History

## 2026-07-16 workspace reorganization and package 9 import

- 用户要求整理杂乱工作区，以 `ESP32P4_Detection(9).zip` 为后续开发基础，原有内容改为参考资料，并完成解压、烧录和测试。
- 根目录无 CodeGraph；读取 `PROJECT.md`、`CURRENT.md`、`HISTORY.md` 后开始任务。
- 根 Git 修改前工作区干净，checkpoint 为 `21b121d`。
- 旧 `ESP32P4_Detection` 存在 `main/system_init.c` 未提交改动；先提交为 `b79edb4 chore: checkpoint before workspace reorganization`，避免归档丢失现场状态。
- `unzip -t ESP32P4_Detection(9).zip` 通过；压缩包共 17,796 项，唯一顶层目录为 `ESP32P4_Detection/`。
- 建立 `reference/` 分类目录并移动旧固件、上位机、数据集、历史工程、报告源文件、笔记和原始压缩包。
- 解压第 9 包为新的 `ESP32P4_Detection/`。
- 第 9 包内 Git 为 `feat/screen-uvc-stream` 分支，原 HEAD `cd9e591`；包内已有 `ui.c`、`system_init.c`、`sdkconfig` 改动。
- 将 CRLF 统一为 LF，并建立导入基线提交 `367e0c7 chore: establish package 9 development baseline`。
- 用户追加要求删除新工程 Agent/Skills；已删除 `AGENTS.md`、`CLAUDE.md`、`.claude/`、`Skills/`、`agentic/`、`esp-agent.sh`、`backtest_run.py`、`test-full-toolchain.sh`。
- 保留新工程 `.codegraph/` 作为本地索引，并在工程 `.gitignore` 中忽略 CodeGraph 和编辑器本地配置。
- 校名/校徽和演示脚本移到 `reference/package9_extras/`，原压缩包仍可完整恢复所有内容。
- 上一阶段 2026-07-08 报告任务历史移入 `docs/agent/archive/2026-07-08-report-finalization.md`。
