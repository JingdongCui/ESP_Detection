# Current Task

## Goal

按 `goal.md` 完成 ESP32-P4 70 ms 推理、Host 设备控制闭环、重连、长稳与 UVC 调查，并保留多份可回退版本。

## Current State

- 推荐固件：分支 `goal/inference-and-device-control`，提交 `c26dba8`，标签 `backup/final-production-candidate-20260717`；Host 为 `6bcee3b`。
- 生产 profile 默认 `CONFIG_SCREEN_UVC_ENABLE=n`，保留相机/LCD、真实 S1/S2/S4 与三路电机、Ethernet、CONTROL_JSON、双 TCP、ISP 和强化 ESP-DL guard。
- 原 500～600 ms 回退根因为 `sort_dbg` USB Serial/JTAG monitor 干扰 ESP-DL；生产配置关闭该调试任务，真实分拣 IO 保持启用。
- 剩余 UI 尾延迟机制为 LVGL mutex 优先级继承：preview 在锁内阻塞 PPA blit 时曾被 priority-5 LVGL 提升到 5。`60c9f8a` 将 lvgl/swdraw/vision_disp=3/3/2，fetch/detect/dl_mc0/1=4。
- 5×60 共 300 样本严格通过：P50/P95/max=67.312/71.393/132.003 ms，零 `>=150/500 ms`。
- `60c9f8a` 的 3660.179 s 长稳完成：boot 1、fatal 0、guard 0、双端口失败 0、末尾 24/24 task 和 heap integrity=ok。1048 样本 P50/P95/max=67.308/74.273/217.760 ms，8 个 `>=150 ms`、0 个 `>=500 ms`；连续性通过，但长稳 max 仍不满足 150 ms。
- `b08a1a3` 的 guard 由只看 vtable 增强为校验实际 `forward_args` target 可执行性和补码，并由 worker 重解析后直接调用；覆盖无效跳转 0x10 的直接路径，未证明上游对象损坏根因消除。
- 板端 CONTROL_JSON 全控制面、能力/错误校验、state 回读、restart、5000/5001 重连均已实板通过；Host build、CTest 和四页 UI/状态回显截图已完成。
- UVC DMA 根因已解决：`7a42b1f` 提前预留 JPEG descriptors 后，板端成功打印 `UVC Device Start` 和 `screen UVC stream started`。
- UVC 与推理当前冲突：即使 TinyUSB/UVC task 降到 3/3，空闲 1×60 为 P50/P95/max=71.552/394.671/486.401 ms，12 个 `>=150 ms`。实验版保留在 `backup/uvc-starts-latency-regression-20260717`，不用于生产。
- 最终生产 profile 已完成精确全量 `idf.py flash monitor`，所有镜像 Hash verified；启动 24 tasks、双 TCP 和真实 sorter 正常。最终 1×60 回归 P50/P95/max=67.718/71.507/72.325 ms，严格通过。

## Backups

- 最终生产候选：`backup/final-production-candidate-20260717` (`c26dba8`)。
- UI 隔离长稳基线：`backup/ui-inference-isolation-61min-pass-candidate-20260717` (`60c9f8a`)。
- UVC 可启动但性能回退版：`backup/uvc-starts-latency-regression-20260717` (`b0e7a02`)。
- 强化 target guard：`backup/dl-target-guard-candidate-20260717` (`b08a1a3`)。
- Guard 长稳回退版：`backup/dl-guard-61min-pass-candidate-20260717` (`56a53fd`)。
- 完整控制回退版：`backup/control-json-isp-reconnect-candidate-20260717` (`b1dfef5`)。
- 最小 70 ms 修复：`backup/inference-70ms-candidate-20260717` (`6ad4fd5`)。
- 失败/诊断标签和完整矩阵见 `docs/goal-delivery-2026-07-17.md`。

## Next Step

1. 用户现场做五次物理断电冷启动、LCD 触摸/亮度/ISP 画面与真实包裹/电机照片录像。
2. 若继续收敛长稳 max，增加 core-specific ESP-DL worker dispatch/start/end 诊断，关联 priority-11 ISP task 和高优先级中断。
3. 接好 ESP32-P4 USB OTG 后启用 `CONFIG_SCREEN_UVC_ENABLE`，验证枚举、MJPEG 画质/帧率/长稳；再决定是否能降低 USB 中断干扰或仅保留专用 profile。

## Blockers

- 当前环境无法代替用户执行真正断电、LCD 触摸、真实包裹/传感器/电机观察和照片录像。
- 当前电脑未接入或未枚举 ESP32-P4 USB OTG，无法完成主机 UVC 拉流验证。
- 300 样本严格窗口通过，但 1048 样本长稳仍有 8 个 150～218 ms wait 尖峰；加上物理验收未完成，不能创建 stable 标签。
