# History

## 2026-07-17 板端工程队友交接包

- 读取 `PROJECT.md`、`CURRENT.md`、`HISTORY.md` 并盘点根目录与独立 Git 仓库。
- 确认交接范围仅为 `ESP32P4_Detection/`，不包含上位机、数据集和根仓库历史。
- 发现现有 `sdkconfig` 未提交变化：目标 revision 切换到 v3.1，CPU 切换到 400 MHz。
- 按修改前提交规则建立检查点：`f51b420 chore: checkpoint ESP32-P4 v3.1 build config`。
- 已执行 `git fsck --full --no-dangling`，仓库对象检查无报错。
- 新增队友交接文档 `ESP32P4_Detection/HANDOFF.md`。

## 2026-07-17 分拣参数、双 worktree 与视觉投票

- 从 2026-07-07 稳定参考 `4946a30` 恢复速度和皮带超时，并按用户要求覆盖交接延时与传感器：
  - S1=GPIO22、S2=GPIO23、S3/S4=-1。
  - A/B/C=65/100/100%，handoff=100ms，A/B/C timeout=4500/900/1300ms。
- 主分支提交 `07c71c9 fix: restore sorter pins and timing defaults`。
- 找到 2026-07-16 原始限速修复 `0efa82a fix: restore direct stop and cap A speed while B is busy`，只移植其中 `a_run_speed()` 最小逻辑。
- 创建 worktree `/home/kazeform/2026esp/ESP32P4_Detection_b_busy_60` 和分支 `feat/sorter-b-busy-speed-cap`。
- 恢复旧版 5 次置信度加权视觉投票，主分支提交 `0af4c90 feat: restore confidence-weighted vision voting`。
- 限速分支重放到投票提交之上，提交 `d903e7a feat: cap belt A speed while belt B is busy`；相对主分支只差调度器限速帮助函数、两个调用点和 60% 宏。
- 两分支 `git diff --check` 通过，`idf.py build` 通过；app 大小分别约 `0x524870`、`0x5248f0`，分区剩余约 14%。
- 当前没有 `/dev/serial/by-id/`，未执行 `idf.py flash monitor`。
