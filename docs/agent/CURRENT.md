# Current Task

## 当前目标

- 保留两个可对照烧录的分拣分支，并在两者中恢复旧版视觉投票。

## 当前现状

- 主 worktree：`ESP32P4_Detection/`，分支 `feat/screen-uvc-stream`，HEAD `0af4c90`。
- 限速 worktree：`ESP32P4_Detection_b_busy_60/`，分支 `feat/sorter-b-busy-speed-cap`，HEAD `d903e7a`。
- 两分支共有配置：S1=22、S2=23、S3/S4=-1；速度 65/100/100；交接 100ms；A/B/C 超时 4500/900/1300ms。
- 两分支均恢复每个 S1 窗口最多 5 次的置信度加权投票。
- 限速分支仅额外增加：B 空闲时 A 按设定速度，B 被预留/占用时 A 最大 60%。
- 两个 worktree 均已完成 `idf.py build`。

## 下一步

- 有板子后分别执行完整 `idf.py flash monitor`，对比 S1 投票、S2 交接及 B 忙时 A=60% 的日志和机械行为。

## 阻塞点

- 当前没有 `/dev/serial/by-id/`，无法进行实机烧录与 monitor。
