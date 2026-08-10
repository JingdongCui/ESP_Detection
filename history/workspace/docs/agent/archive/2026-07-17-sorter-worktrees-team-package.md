# 2026-07-17 分拣双版本与队友交付包

## 目标

- 从历史稳定版本恢复真实分拣参数和视觉投票。
- 保留基础版与“B 忙时 A 限速 60%”两个可对照版本。
- 生成带完整 Git 内容、可直接发给队友的压缩包。

## 固件结果

- 主 worktree：`ESP32P4_Detection/`。
  - 分支：`feat/screen-uvc-stream`。
  - HEAD：`0af4c90c73fcaef87c82a69cfb1c5d478968b3b8`。
- 限速 worktree：`ESP32P4_Detection_b_busy_60/`。
  - 分支：`feat/sorter-b-busy-speed-cap`。
  - HEAD：`d903e7a23c98505605d29c97ed59575c7c1ee485`。
- 公共配置：S1=GPIO22、S2=GPIO23、S3/S4=-1；A/B/C=65/100/100%；handoff=100ms；A/B/C timeout=4500/900/1300ms。
- 两分支均恢复每个 S1 窗口最多 5 次的置信度加权投票。
- 限速分支相对基础分支只修改调度器和配置头：B 空闲时 A 使用设定速度，B 被预留或占用时 A 最大 60%。
- 串口分拣调试任务已删除，真实 IO 与调度路径保留。

## 关键提交与来源

- `f17536e fix: remove crashing serial sorter debug task`。
- `07c71c9 fix: restore sorter pins and timing defaults`。
- `0af4c90 feat: restore confidence-weighted vision voting`。
- `d903e7a feat: cap belt A speed while belt B is busy`。
- 参数和投票参考 2026-07-07 稳定提交 `4946a30`。
- B 忙限速语义参考 2026-07-16 提交 `0efa82a`，只移植最小限速逻辑。

## 验证

- 两分支 `git diff --check` 通过。
- 两分支 `idf.py build` 通过；app 大小约 `0x524870`、`0x5248f0`，分区剩余约 14%。
- 当前没有 `/dev/serial/by-id/`，未执行本轮 `idf.py flash monitor`。

## 队友包

- 路径：`deliverables/ESP32P4_Detection_team_handoff_20260717_with_git.zip`。
- 大小：33,997,510 bytes（约 33 MB）。
- SHA256：`caee4a62169d7c8129909b78730d3a5bf4a1af5ad411c63e467c90904d7c727e`。
- 包内目录：
  - `ESP32P4_Detection_standard/` 默认检出 `feat/screen-uvc-stream`。
  - `ESP32P4_Detection_b_busy_60/` 默认检出 `feat/sorter-b-busy-speed-cap`。
- 两个目录均为独立、可移动的 Git 仓库，均包含 `feat/screen-uvc-stream`、`feat/sorter-b-busy-speed-cap`、`main` 三个本地分支，不含指向打包机器的 remote 或 worktree 绝对路径。
- 未包含 `build/`、`managed_components/`、`.codegraph/` 和编辑器缓存。
- 已完成：`zip -T`、完整解压、包内 `MANIFEST_SHA256.txt` 校验、两个仓库 `git fsck --full --no-dangling`、工作区干净状态与两个目标分支 HEAD 校验。

## 工作区整理

- 新交付物统一放入 `deliverables/`。
- 打包临时目录与解压验证目录已删除。
- 根目录旧的孤立校验文件已移动到 `backups/2026-07-17/obsolete_ESP32P4_Detection_handoff_20260717_with_git.zip.sha256`，未删除。
