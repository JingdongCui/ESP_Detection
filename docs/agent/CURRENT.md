# Current Task

## 当前目标

- 等待硬件后验证两个已交付分拣版本。

## 当前现状

- 队友包：`deliverables/ESP32P4_Detection_team_handoff_20260717_with_git.zip`。
- SHA256：`caee4a62169d7c8129909b78730d3a5bf4a1af5ad411c63e467c90904d7c727e`。
- 包内两个工作目录均带完整 `.git` 和三个本地分支，且已完成解压、清单、Git 对象及 HEAD 校验。
- 两分支均已通过 `idf.py build`。
- 已完成答辩资料 `docs/ethernet_qt_link_defense_guide.md`，按当前固件 `0af4c90` 和 Host `3668ddb` 核对双 TCP、协议、线程/任务、图像和控制闭环。

## 下一步

- 将压缩包和同名 `.sha256` 发给队友。
- 有板子后分别执行完整 `idf.py flash monitor`，对比 S1 投票、S2 交接及 B 忙时 A=60% 的日志和机械行为。

## 阻塞点

- 当前没有 `/dev/serial/by-id/`，无法进行实机烧录与 monitor。
