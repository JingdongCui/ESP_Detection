# Host 队友交接压缩包（2026-07-17）

## 交付物

- 压缩包：`/home/kazeform/2026esp/esp32_host_no_inference_control_20260717.zip`
- 校验文件：`/home/kazeform/2026esp/esp32_host_no_inference_control_20260717.zip.sha256`
- 大小：57,205 bytes
- SHA256：`776516a304d8269532bda80694b676f9eb379ab9ec2d3343450dbfd458bd3f18`
- Host 提交：`3668ddb90829d12fc92513d86a968edb1abe9fc3`
- 配套固件：`c26dba8`（`backup/final-production-candidate-20260717`）

## 包内容与排除项

压缩包由 Host Git HEAD 通过 `git archive` 生成，共 31 个目录项、226,399 bytes 未压缩内容。包含完整 C++/QML 源码、CMake 文件、协议测试、README 和 `TEAMMATE_HANDOFF.md`。

不包含 `.git`、`build/`、Qt Creator 缓存、本机运行日志、遥测、接收图片或平台运行库。Host README 中原先关于设备控件仅为本地状态和使用 `type=0x12` 设置的过期说明已修正为当前 `type=0x11 CONTROL_JSON` 闭环。

## 验证

1. 原工作树执行 `cmake --build build/linux-debug -j` 通过。
2. 原工作树执行 CTest，`packetprotocol_tests` 1/1 通过。
3. `unzip -t` 检查压缩包全部文件，无错误。
4. `sha256sum -c` 通过。
5. 将压缩包解压到全新 `/tmp` 目录后重新 CMake configure、Release build 和 CTest，1/1 通过，证明包内源码完整且不依赖原仓库构建目录。

## Git

- 修改前 Host 工作树干净，功能基线为 `6bcee3b`。
- 新增队友交接文档并修正 README，提交为 `3668ddb docs: add teammate host handoff guide`。
- 归档后 Host 工作树干净。
