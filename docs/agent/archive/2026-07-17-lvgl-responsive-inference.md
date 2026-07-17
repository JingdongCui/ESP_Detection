# LVGL 响应与 100 ms 推理阶段

## 目标

- 撤回导致 LVGL 变卡的任务设置。
- 保持 waybill 推理低于 100 ms，避免重新出现约 500 ms 回退。
- 用户允许生产固件不启动 UVC。

## 根因与决策

- `c26dba8` 为压低推理尾延迟，把 LVGL 主任务从优先级 5 降到了 3，直接降低了触摸、timer 和页面事件的响应能力。
- 旧预览任务在持有 LVGL recursive mutex 时，对两个 framebuffer 做阻塞 PPA。高优先级 LVGL 等锁时会通过 FreeRTOS priority inheritance 抬高 `vision_disp`，让它压过优先级 4 的 ESP-DL worker。
- 最终恢复 `lvgl=5`、`swdraw=4`，保留 `vision_disp=2` 和检测/ESP-DL=4；只在短临界区创建 LVGL async 回调，阻塞 framebuffer blit 不再发生在外部持锁任务中。
- 推理阶段用 atomic active 标志保护；回调看到模型正在运行就丢弃该次纯视觉预览，避免抢占双核模型阶段。
- 预览刷新上限改为 4 FPS。整帧双 framebuffer blit 的累计 max 约 141.753 ms，限流后在相邻刷新间留下约 100 ms 以上的 LVGL 空闲窗口。
- 尝试过 24 行条带 PPA，将单条 max 降到约 33 ms；但检测几乎连续 active，每个预览只完成 0～48 行就被取消，造成预览无法形成完整帧。实验提交 `0ecec31` 已由 `edc26cc` 回退。

## 代码与版本

- 分支：`fix/lvgl-responsive-inference`
- 修改前标签：`backup/before-lvgl-responsive-inference-20260717`
- `177dbd4 perf: keep lvgl responsive during inference`
- `0ecec31 perf: slice preview blits for lvgl responsiveness`（失败实验）
- `edc26cc Revert "perf: slice preview blits for lvgl responsiveness"`
- `5e3d46e perf: leave lvgl idle windows between previews`
- `9bbd224 test: record lvgl inference latency evidence`

## 验证

- `idf.py build`：通过；最终 `sample_project.bin` 为 `0x526f20`，应用分区剩余约 14%。
- `idf.py flash monitor`：通过；ESP32-P4 v1.0 正常启动，无 panic/死锁。
- 最终任务快照：`lvgl=5`、`swdraw=4`、`vision_disp=2`、`vision_det=4`、`vision_fetch=4`、`dl_mc0/1=4`。
- 日志确认 `screen UVC disabled in production inference profile`。
- 异步原型 1×60：P50 63.414 ms、P95 73.731 ms、max 74.280 ms、0 个 >=100 ms、0 个 >=500 ms。
- 最终 4 FPS 版 1×30：P50 60.842 ms、P95 71.402 ms、max 80.548 ms、0 个 >=100 ms、0 个 >=500 ms。
- 证据：`ESP32P4_Detection/test_results/inference_latency_lvgl_async_1x60.json`、`ESP32P4_Detection/test_results/inference_latency_lvgl_final_1x30.json`。

## 当前能力与限制

- 能恢复 LVGL 高优先级事件处理，避免预览 mutex priority inheritance 干扰模型，并保持本轮全部推理样本低于 100 ms。
- 能保留 LCD 摄像头预览和框叠加，但预览最多 4 FPS；UVC 默认关闭。
- 不能用自动日志证明主观触摸手感，仍需操作者连续切页、点击按钮和拖动滑块做最终肉眼确认。
- 不能把 1×30/1×60 hard reset 结果等同于物理断电、多小时稳定性或真实六类包裹级联验收。
