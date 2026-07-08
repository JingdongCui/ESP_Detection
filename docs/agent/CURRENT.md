# Current Task

## Goal

完成比赛报告定稿：删除采购/淘宝类硬件资料图，只保留实拍图；调整系统正面图位置；生成可提交的 Markdown 和 Word 文档。

## Current State

- 根目录 `.codegraph/` 不存在，跳过 CodeGraph。
- 正式 Markdown 已从 `docs/competition_report_draft.md` 更名为：
  - `docs/competition_report_final.md`
- 已生成 Word 文档：
  - `docs/competition_report_final.docx`
- 已新增 DOCX 构建脚本：
  - `tools/build_competition_report_docx.py`
- 定稿已提交：
  - `dcd63c4 docs: finalize competition report`

## Verification

- `docs/competition_report_final.md` 未检出“初稿”“建议配图”“学校”“指导老师”“定稿前”“缺失图片”“主要硬件资料图”等不适合最终正文的字样。
- Markdown 中所有图片路径均存在。
- 已删除正文中的采购/淘宝类参数截图和机械尺寸截图，仅保留实拍图、上位机截图、板端 UI 和流程内容。
- Word 文档已用 LibreOffice 渲染为 PDF，再转为 21 页 PNG 检查，未见明显重叠、裁切、表格溢出或图片异常。
- DOCX 正文 XML 中 `word/document.xml` 的文字颜色仅为 `000000`。
- DOCX 正文中包含 `SimHei`、`SimSun`、`Times New Roman` 字体设置。
- `git diff --check` 通过。
- 本轮只修改文档和生成报告，不修改 ESP 固件或上位机代码；未执行 `idf.py build` / `idf.py flash monitor`。

## Next Step

- 无；等待用户后续审阅或提出封面/格式模板调整。

## Blockers

- 无。
