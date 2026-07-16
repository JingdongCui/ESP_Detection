# 上位机打包与 Dolphin 修复

## 上位机交付包

- 从 Host 提交 `dd22a29` 使用 `git archive` 生成 `esp32_host_no_inference_v2_20260716.zip`。
- 压缩包不含 `.git`、`build`、遥测或 `.qtcreator`，并生成同名 `.sha256` 文件。
- `unzip -t` 完整性校验通过。
- 从压缩包全新解压后，CMake Debug 配置和构建通过，CTest `packetprotocol_tests` 1/1 通过。

## Dolphin 打开方式修复

- 按桌面 `Agent经验库/00-索引.md` 路由，只读取 `系统维护防踩坑.md` 的 Dolphin 维护经验。
- 实时状态确认终端为 `LC_ALL=C.UTF-8`，同时存在旧 Dolphin 进程和不同 locale 的 KService 缓存；默认文本、JPEG、PDF 文件关联均存在。
- 关闭两个旧 Dolphin 进程后，以 `env -u LC_ALL LANG=en_US.UTF-8 kbuildsycoca6 --noincremental` 全量重建服务缓存。
- 正确 locale 的缓存已更新，随后重新打开 Dolphin 到交付包目录；新进程和 KIO worker 正常运行，未见启动错误。
- “打开方式”候选菜单仍需用户右键文件做最终 GUI 目视确认。
