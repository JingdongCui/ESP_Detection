# Git bundle backups（2026-07-17）

三个 bundle 均已执行 `git bundle verify`，结果为完整历史且校验通过。

| 文件 | 内容 | SHA-256 |
|---|---|---|
| `workspace-docs.bundle` | 根目录文档与测试证据仓库，HEAD `10905e5` | `ba775e6af005995ca0beb78c7cc47bd847449a55386d546ea689fb4da58deba6` |
| `firmware.bundle` | 固件全部分支和 27 个 refs，HEAD `c26dba8` | `ff67c222bd6e9f726d457780fd9f698a096df9da3626aefd3713b0c634176b06` |
| `host.bundle` | Host 全历史，HEAD `6bcee3b` | `922845ae796f9ac551445de3a880093b08e62826d1c2907c005d2ac085545046` |

恢复示例：

```bash
git clone firmware.bundle restored-firmware
git -C restored-firmware switch goal/inference-and-device-control
```

Bundle 文件由根仓库 `.gitignore` 排除，不会被再次打入根仓库历史；本 README 保留校验信息。
