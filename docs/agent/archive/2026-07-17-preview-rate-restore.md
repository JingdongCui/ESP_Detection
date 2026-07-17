# 预览刷新率恢复阶段

## 目标

- 提高 LCD 预览刷新率，恢复到 goal 开始前的连续刷新观感。
- 保留上一阶段的 LVGL/推理解耦，避免推理重新升高到 100～500 ms。
- UVC 允许不启动，本阶段继续关闭。

## 对照结论

- goal 开始标签 `backup/goal-start-20260717` 的第一父提交为 `ba3ef25`，第二父提交为 `6fd12b4`。
- goal 前 `vision_display_task` 没有固定帧率限流，拿到新帧即执行预览准备和双 framebuffer 搬运。
- 上一阶段提交 `5e3d46e` 新增 `VISION_PREVIEW_MIN_INTERVAL_US=250000`，把预览主动限制为 4 FPS。
- 不能直接恢复 goal 前“显示任务持 LVGL mutex 做两次阻塞 PPA”的实现，否则会重新引入 mutex 优先级继承和推理长尾。

## 实现

- 固件提交：`683ce8f perf: restore continuous preview refresh`。
- 删除 250 ms 人工限流和 `last_preview_us` 判定；每个新帧都可尝试预览。
- 保留安全隔离：
  - `vision_disp` 优先级 2；
  - framebuffer 搬运仍通过 `lv_async_call` 在 LVGL 线程执行；
  - 推理 active 时丢弃视觉帧，不抢占双核 ESP-DL；
  - LVGL=5、swdraw=4；
  - UVC 关闭。
- 修改前备份标签：`backup/before-preview-rate-restore-20260717`。

## 验证

- `idf.py build`：通过，固件大小 `0x526ea0`，app 分区余量 `0xd9160`（14%）。
- `idf.py flash monitor`：通过，ESP32-P4 v1.0 正常启动；日志确认 `screen UVC disabled in production inference profile`。
- 连续预览实板日志：vision 在 7.637 s 启动，54.058 s 报告 `preview async blit count=300 infer_skip=412 max=127138us`。
  - 300 个完成帧 / 46.421 s = 约 6.46 FPS；
  - 相比上一版固定 4 FPS 提升约 61.5%；
  - 刷新节奏重新由双 framebuffer PPA 实际吞吐决定，与 goal 前“无人工限速”一致。
- 推理 1×60 hard-reset 报告：`test_results/inference_latency_preview_continuous_1x60.json`。
  - P50 67.188 ms；
  - P95 75.037 ms；
  - max 79.087 ms；
  - 0 个 >=100 ms，0 个 >=150 ms，0 个 >=500 ms。

## 边界

- 自动日志证明实际完成刷新约 6.46 FPS，但最终“肉眼观感与 goal 前完全一致”仍需操作者观察实屏确认。
- 当前双 framebuffer RGB888 整帧搬运单次最大约 127 ms，是预览继续提高的主要上限；若要明显超过约 6～7 FPS，需要改变显示缓冲/像素格式/局部更新架构，而不是继续调整任务优先级。
- 推理期间仍会主动跳过预览帧，这是保证推理低于 100 ms 的必要取舍。

