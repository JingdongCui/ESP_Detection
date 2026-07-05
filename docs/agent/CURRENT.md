# Current Task

## Goal

打磨 `esp32_host_no_inference`，从演示/占位风格调整为可交付的板端视觉分拣上位机。

## Current State

- 上位机当前分支：`tcp-board-align`。
- 修改前 checkpoint：`ef188a1 checkpoint before delivery polish`。
- 已修改：
  - 移除离线假数据源 `DemoDataSource`，无设备时不再注入假 metrics / detection。
  - 首页删除底部“实时事件流”整行。
  - 视觉页历史改为“包裹图像记录”，图片接收记录显示 `包裹#N`，不再显示“图片帧/画质增强”等调试文案。
  - 预留页改为“系统维护”，展示监听状态、最后遥测、累计接收、保存目录、端口、图片链路健康和运行日志。
  - Header / README / 控制页文案改成交付口径：待连接、系统维护、遥测/控制命令等。

## Verification

- 上位机：
  - `cmake --preset debug`：通过。
  - `cmake --build --preset debug`：通过。
  - `QT_QPA_PLATFORM=offscreen timeout 8s ./build/debug/bin/esp32_host_no_inference`：可启动到 timeout，无 QML 加载错误输出。
  - `rg` 检查无 `演示/demo/图片帧/画质增强/预留接口/实时事件流` 等交付前文案残留。
- 本轮未改板端代码，未执行 `idf.py build/flash/monitor`。

## Notes

- 无设备连接时，上位机保持空状态和待连接文案；监听服务仍自动启动。
- `包裹#N` 为上位机本地接收顺序号，每次应用启动从 1 开始。

## Next Step

- 如需现场验证，启动上位机后复位板端，确认维护页端口/日志、视觉页包裹记录和首页指标正常更新。

## Blockers

- 未阻塞。
