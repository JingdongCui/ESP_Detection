# Git bundle backups（2026-07-17）

三个 bundle 均已执行 `git bundle verify`，结果为完整历史且校验通过。

| 文件 | 内容 | SHA-256 |
|---|---|---|
| `workspace-docs.bundle` | 根目录文档、阶段归档与最终验收证据索引，HEAD `351931a` | `57de969cdc5e581cc259d28c5fb2a3a30f42e83682cd91eb3c346c14e9a11d97` |
| `firmware.bundle` | 固件全部分支和标签（含生产、core timing、HS/FS UVC 实验 refs），HEAD `c26dba8` | `a19ab61a315f30f3ee3402272b8855d413ac423179c9693fe5ed38d6fd6f87bf` |
| `host.bundle` | Host 全历史，HEAD `6bcee3b` | `922845ae796f9ac551445de3a880093b08e62826d1c2907c005d2ac085545046` |

恢复示例：

```bash
git clone firmware.bundle restored-firmware
git -C restored-firmware switch goal/inference-and-device-control
```

Bundle 文件由根仓库 `.gitignore` 排除，不会被再次打入根仓库历史；本 README 保留校验信息。
