# Current Task

## Goal

完善 `docs/agent/MERGE_EXPERIENCE.md`，把本轮 merge/old/teammatesproject_old 排查和前几轮合并踩坑沉淀成后续类似合并可复用的经验文档。

## Current State

- 修改前已提交 checkpoint：
  - `b4201eb checkpoint before expanding merge experience`
- 已将 `MERGE_EXPERIENCE.md` 从开头骨架扩展为完整交接文档。
- 当前内容覆盖：
  - 合并总原则、checkpoint 和三工程对照方法。
  - ESP-IDF 依赖锁、managed_components、组件级 manifest 的排查经验。
  - `esp-dl 3.3.6` 必要升级证据与无关库连带升级风险。
  - ESP32-P4 旧/新 revision、CPU 频率和 `sdkconfig` 注意点。
  - build 目录路径污染、微信临时目录缓存误导。
  - findlogo/ROI、RGB/BGR、类别到物理出口映射。
  - UI `system_monitor()` 被关闭导致 CPU/状态不刷新的经验。
  - 电机分拣、TCP 20 包 audit、烧录/monitor 波特率、常见失败定位顺序。
  - 面向 `teammatesproject_old` 的推荐移植顺序。

## Immediate Next Step

- 如用户认可，提交本次文档完善。

## Blockers

- 无。
