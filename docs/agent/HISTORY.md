# History

- 2026-07-26 上位机云端回复演示：在 `esp32_host_no_inference` 的基线提交 `547cab7` 后实施。断连状态使用控制器读取层的固定演示数据，避免污染真实遥测累计；第 4 页保留系统维护并新增本地回复卡片。
- 2026-07-26 展示在线状态：断连时新增 `displayConnected` 仅供 QML 显示层使用，侧栏、顶部、控制页、维护页、卡片、日志及检测记录统一显示在线/正常，真实 `connected` 仍用于控制命令安全门控。构建、CTest 和离屏启动验证通过。
- 2026-07-26 动画定义：第 4 页可见时先加载旋转 2 秒，再每 50 ms 显示一个字符，完整回复停留 10 秒后循环；离开页面停止计时器与旋转动画。
- 2026-07-26 验证：`cmake --build build -j2` 成功；`ctest --test-dir build --output-on-failure` 通过 1/1；`QT_QPA_PLATFORM=offscreen timeout 5s build/bin/esp32_host_no_inference` 无运行期输出。仅改动 Qt 上位机，`idf.py build` 与烧录监视不适用。
- 2026-07-26 报告更新：基于 `info.txt`、报告要求和现有工程资料，建立 `report_work/` 并完成新版 Markdown、HTML/SVG 源流程图及 PNG 导出；待用户确认 Markdown 后再生成 Word。
- 2026-07-26 报告目录修正：`info.txt` 第 13 行规定正文六章结构，已将 Markdown 重构为“设计需求分析、特色与创新、功能设计、系统实现、其他内容、参考文献”，并采用章、节、小节三级目录。
- 2026-07-26 报告云端内容扩充：明确云端大模型承担长时间尺度的数据分析、报告生成和优化建议；Qt 上位机承担数据组织、命令校验与转发；ESP32-P4 保持实时识别、调度和安全控制。
- 2026-07-26 Word 交付：生成 `report_work/嵌入式边缘AI智能分拣系统_报告.docx`；生成器已支持指定 Markdown/Word 路径，全文字体统一为宋体、黑色字。使用 LibreOffice 渲染为 16 页 PNG 并逐页检查；修复了 Markdown 加粗标记残留和跨页表格行拆分问题。
- 分拣双版本与队友包记录：`archive/2026-07-17-sorter-worktrees-team-package.md`。
- Ethernet/Qt 答辩文档记录：`archive/2026-07-17-ethernet-qt-defense-guide.md`。
- 队友 package 13 与 Host 新包合并、编译、烧录记录：`archive/2026-07-18-teammate-package13-merge.md`。
- real IO 内部 RAM 优先栈扩容及 S1 压力验证：`archive/2026-07-18-real-io-internal-stack.md`。
- vision_det 内部 RAM 栈恢复、双分支构建及识别/S1 压力验证：`archive/2026-07-18-vision-det-stack-restore.md`。
- 2026-07-18 双板端 Git 工程交接包生成与完整校验：`archive/2026-07-18-team-handoff-package.md`。
