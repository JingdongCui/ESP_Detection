# Current Task

## Goal

将比赛作品 Markdown 报告从初版扩充为接近可交付版本，补充 PSRAM 蓝屏优化、模型训练心得、现有图片素材和后续缺图清单。

## Current State

- 根目录 `.codegraph/` 不存在，跳过 CodeGraph。
- 本轮修改文件：
  - `docs/competition_report_draft.md`
  - `docs/report_system_information.md`
  - `docs/report_unknown_information.md`
  - `docs/agent/CURRENT.md`
  - `docs/agent/HISTORY.md`
- 报告已去掉“初稿说明”和“建议配图”。
- 报告已加入：
  - PSRAM 带宽竞争导致 LCD 蓝屏闪烁的工程定位。
  - 降低摄像头帧率、降低 DPI clock、zero-copy peek、调整 AXI/PSRAM 仲裁优先级等稳定性优化。
  - 模型训练心得：高质量数据集优先，反光严重数据可能伤害模型。
  - 量化策略：MSE equalization `(10, 0.1)`，无 TQT，无 bias correction。
  - 现有上位机截图、板端识别图、硬件图、机械尺寸图。
  - 定稿前仍需补充图片表。

## Verification

- 本次只修改 Markdown 文档，未修改 ESP 固件或上位机代码。
- 未执行 `idf.py build` / `idf.py flash monitor`，原因是无代码改动。

## Next Step

- 运行文档自检和 `git diff --check`。
- 提交本次报告扩充。

## Blockers

- 当前无板子，无法补新的实机照片或重新验证运行稳定性。
- 缺少用户后续提供的实际分拣图、板子 UI 正面图、系统整体图。
