# Current Task

## Goal

按 `new_merge` 板端普通 UI 调整 `esp32_host_no_inference`，删除本地推理页面，保留图片预览并补齐板端数据/控制映射。

## Current State

- `new_merge` 板端普通 UI 已整理到 `docs/agent/BOARD_UI_INVENTORY.md`。
- 上位机 `esp32_host_no_inference` 当前分支：`tcp-board-align`。
- 已删除上位机本地模型服务/推理工作台页面入口与 QML 文件。
- Dashboard 继续保持大信息量风格，并增加板端 image link counters：队列、encoded/sent、drop/fail、latest JPEG size、encode/send time。
- 图片预览页保留原布局，小改为默认显示最新板端 JPEG，叠加框受本地 preview overlay 开关控制。
- 控制页参考板端普通 settings：
  - 屏幕亮度：本地 UI 状态。
  - 置信度阈值：本地 UI 状态，用于低置信度标记。
  - 检测开关：本地 UI 状态。
  - 预览叠加框：本地 UI 状态。
  - 电机速度：发送 `CONFIG a_speed=<v> b_speed=<v> c_speed=<v>`。
- 临时 DEV/debug 页内容不进入上位机：S1-S4、编码器、MTEST、包裹注入、`ENC_CLEAR`、专用 `HW_STATUS` 面板均未添加。

## Verification

- 上位机：
  - `cmake --preset debug`：通过。
  - `cmake --build --preset debug`：通过。
  - `QT_QPA_PLATFORM=offscreen timeout 8s ./build/debug/bin/esp32_host_no_inference`：应用可启动并保持运行到 timeout，无 QML 加载错误输出。
- 本轮未改板端代码，未重新运行 `idf.py build/flash/monitor`。

## Notes

- 上位机不再主动发送 `HW_STATUS`，避免把 DEV 页面调试项做成常驻控制。
- `brightness`、`danger_threshold`、`detection_enabled`、`preview_overlay` 当前只影响本地 UI；板端当前 TCP SIM line 未定义对应普通 dashboard 控制命令。
- 图片看不到的问题优先看 `5001` image socket 是否连接、`latest_preview.jpg` 是否更新；上位机已改为每张 JPEG 都刷新 preview，不再 10 帧节流。

## Next Step

- 现场联调时先启动上位机，再复位/重启板端；观察 `5000/5001` 连接、Dashboard image counters、预览页最新 JPEG 是否更新。

## Blockers

- 未阻塞。
