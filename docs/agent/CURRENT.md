# Current Task

## Goal

按用户要求继续优化上位机数据显示：第一页改为包裹/快递数量统计，第二页不显示分辨率和目标数量，第四页调整链路端口卡片布局。

## Current State

- 上位机当前分支：`tcp-board-align`。
- 上位机修改前 checkpoint：`5adbb4f checkpoint before image category display`。
- 板端当前分支：`motor-roi`。
- 板端修改前 checkpoint：`a09b055 checkpoint before jpeg category metadata`。
- 已修改：
  - 首页底部改用 `courierStatsCards`：总包裹数、极兔、中通、韵达，不再展示队列深度、已编码、丢弃、无帧、最新大小。
  - 首页顶部将“图像帧数/检测目标”调整为“总包裹数/最新类别”。
  - 视觉检测页主图和历史记录不再显示图片分辨率、检测目标数量；每张图默认按一个包裹和三类之一展示。
  - 系统维护页链路端口卡片改为端口并排、板端地址/上位机地址独占整行，缓解拥挤。
  - 系统维护页仍保留图片链路健康技术项，改由 `imageHealthCards` 独立提供。

## Verification

- 上位机：
  - `cmake --build --preset debug`：通过。
  - `git diff --check`：通过。
  - `QT_QPA_PLATFORM=offscreen timeout 8s ./build/debug/bin/esp32_host_no_inference`：可启动到 timeout，无输出。

## Notes

- 快递统计口径仍按收到的 JPEG 图片记录计数，不等于真实包裹 ID 计数。
- 无法分类按用户确认的策略显示和统计为极兔。
- 图片链路健康技术项仍保留在系统维护页，不放在首页。

## Next Step

- 提交上位机 UI 调整和根目录 agent 文档。

## Blockers

- 未阻塞。
