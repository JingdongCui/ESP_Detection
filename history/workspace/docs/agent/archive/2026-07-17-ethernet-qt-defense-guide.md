# 2026-07-17 Ethernet 与 Qt 上位机答辩文档

## 目标

- 基于当前实际代码整理板端 Ethernet 到 Qt/QML 的完整技术路线。
- 聚焦具体实现、整体框架、协议、并发、状态闭环和答辩表达。

## 基线

- 固件：`ESP32P4_Detection`，`feat/screen-uvc-stream`，HEAD `0af4c90`。
- Host：`esp32_host_no_inference`，`master`，HEAD `3668ddb`。
- 输出：`docs/ethernet_qt_link_defense_guide.md`。

## 核对方法与结论

- 先用 CodeGraph 定位固件 `System_Init → ethernet_app_start`、控制任务、图像任务、静态 IP、重连、控制 JSON、稳定帧和 Vision 提交路径。
- Host 没有 CodeGraph 索引，按实际入口 `main.cpp → HostController → HostNetworkWorker` 核对 Qt 线程、双 `QTcpServer`、协议解析、QML 属性和控制调用。
- 当前实际架构是板端 TCP Client、Qt TCP Server，固定 IP `192.168.10.2 → 192.168.10.1`，5000 控制/遥测、5001 JPEG。
- Qt 当前网络路径运行在独立 `HostNetworkWorker/QThread`；`HostController` 内早期单 socket 私有方法未进入启动路径。
- 当前 V2 图像是 1024×600 干净 JPEG，32 B 元数据 + 最多 8 个 16 B 框，由 QML 叠框；Vision 附近个别 640×375/burn-in 注释属于旧描述。
- 稳定帧 3 槽状态机保证图像和检测元数据同帧；编码队列和 JPEG 槽均为深度 2，拥塞时丢图而不阻塞推理和真实分拣。
- `VISION_DISPLAY_MISS_KEEP_COUNT=3` 按当前 `<` 判断是保留 3 个 miss 帧，在第 4 个连续 miss 才清除上一命中状态。
- 当前板端已实现 CONTROL_JSON get/set/action、校验执行、state/error 回传；修正了 PROJECT 中“板端待实现”的过期记录。

## 验证

- `git diff --check` 通过。
- Host `cmake --build build/linux-debug -j2` 通过。
- Host `ctest --test-dir build/linux-debug --output-on-failure`：1/1 通过。
- `appesp32_host_qmllint` 执行成功，存在大量原有 unqualified access 警告；本任务未修改 QML。
