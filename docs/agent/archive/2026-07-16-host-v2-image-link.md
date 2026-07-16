# Host V2 图像链路移植

## 目标与边界

- 真正的上位机恢复到 `/home/kazeform/2026esp/esp32_host_no_inference`。
- `New Folder` 仅作为队友 V2 协议/画框参考，未修改。
- 板端升级为 V2 图像事务，Host 保留 V1 回退兼容。

## 实现决策

- V2 image 使用 40B 公共头、32B metadata、`0..8` 个 16B box 和 JPEG，字段均为小端。
- 快照保持干净 RGB/JPEG，不再在板端 burn-in；检测框与图像在同一互斥区内一起发布。
- Ethernet 分段发送 header/meta/boxes/JPEG，不增加整包大缓冲。
- Host V1 使用 header 保留字段中的类别/置信度；V2 使用 payload 元数据。
- QML 使用 `paintedWidth/paintedHeight` 处理等比缩放和 letterbox，叠加开关只控制上位机框层。

## 提交

- 固件：`ba3ef25 feat: send v2 image results with detection boxes`
- Host：`dd22a29 add v2 image result protocol and overlays`
- 实施前根工作区检查点：`f39aee2`

## 验证

- Host Debug/Release 构建通过。
- CTest `packetprotocol_tests`：1/1 通过，覆盖 V1/V2 header、合法/畸形 V2 payload 和闭区间坐标归一化。
- `appesp32_host_qmllint` exit 0，无 error；仍有工程基线的 unqualified access 警告。
- Host offscreen 启动正常。
- 固件 `idf.py build` 通过，app 大小 `0x513ae0`，剩余 `0xec520`。
- `/dev/serial/by-id/usb-Silicon_Labs_CP2102N_USB_to_UART_Bridge_Controller_7ee2f3966ac3ee11be78b90f9e1b1c54-if00-port0` 完整 flash 成功，bootloader/app/partition/storage Hash 全部 verified。
- monitor 确认 ESP32-P4 v1.0 启动到 `System initialization done`，control/image 连接 Host，time sync 成功，未见 panic/reboot。
- `ss` 确认 `192.168.10.1:5000/5001` 与 `192.168.10.2` 同时 ESTABLISHED，telemetry 持续写入。

## 未完成的现场项

- 镜头当前可命中面单但未命中 Logo，因此未触发“面单+Logo”上升沿快照，未对真实 V2 JPEG 和 GUI 框位置做实物目视。
- screen UVC 仍有既知 `jpeg encoder rxlink` 内存不足，本任务未扩大处理。
