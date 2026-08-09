# ESP32 Host

Qt 6 桌面上位机，用于接收 ESP32-P4 以太网遥测、JPEG 图像和分拣状态，并提供基础板端控制。

当前版本定位为现场交付工具：无本地模型服务页，无离线假数据注入；检测与分拣结果来自板端视觉/分拣链路。

## Requirements

- Qt 6.5 or newer with `Quick` and `Network`
- CMake 3.16 or newer
- Ninja or another CMake generator

## Linux

```bash
cmake -S . -B build/linux-release -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build/linux-release
cmake --install build/linux-release --prefix dist/linux
./dist/linux/bin/esp32_host_no_inference
```

Run directly from the build tree:

```bash
cd /home/kazeform/2026esp/esp32_host_no_inference
cmake -S . -B build/linux-release -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build/linux-release
./build/linux-release/bin/esp32_host_no_inference
```

The Linux install layout intentionally relies on Qt from the target distribution.
For a fully self-contained Linux bundle, package the installed tree with the
distribution's packaging tools or an AppImage workflow.

If UFW is enabled, allow the board to connect:

```bash
sudo ufw allow in on enp3s0 from 192.168.10.2 to any port 5000 proto tcp
sudo ufw allow in on enp3s0 from 192.168.10.2 to any port 5001 proto tcp
```

The Ethernet adapter should use:

- PC: `192.168.10.1/24`
- Board: `192.168.10.2`

运行端口：

- `5000/tcp`：控制、遥测、时间同步、分拣 SIM line 协议
- `5001/tcp`：JPEG 图像事务，packet type `0x01`，pixel format `2`

## 图像协议

所有多字节字段均为小端。公共头固定 40 字节，依次包含 `magic/version/type/header_size/seq/timestamp_ms/payload_len/width/height/pixel_format/reserved/reserved2`。

- V1 图像：`version=1`，payload 为纯 JPEG；`reserved` 为主类别，`reserved2` 低 8 位为百分比置信度。Host 保留该路径用于旧固件回退，检测框为空，推理耗时记为 0。
- V2 图像：`version=2`，payload 为 32 字节元数据、`box_count` 个 16 字节框、纯 JPEG。metrics、检测 JSON、控制和时间同步仍使用 V1。

V2 元数据布局：

| 偏移 | 类型 | 字段 |
| ---: | --- | --- |
| 0 | `u32` | `frame_id` |
| 4 | `u64` | `capture_timestamp_us` |
| 12 | `u32` | `jpeg_len` |
| 16 | `u16` | `box_count`，最大 8 |
| 18 | `u16` | `primary_class_id` |
| 20 | `u16` | `primary_confidence_x1000` |
| 22 | `u16` | `infer_time_ms` |
| 24/26 | `u16/u16` | `source_width/source_height` |
| 28/30 | `u16/u16` | `meta_size=32` / `box_size=16` |

每个框依次为 `stage:u8, category:u8, score_x1000:u16, x1/y1/x2/y2:u16, flags:u16, reserved:u16`。坐标基于 JPEG 尺寸且为闭区间；Host 会按 `Image.paintedWidth/paintedHeight` 映射到 `PreserveAspectFit` 的实际绘制区域。`stage=0` 面单框为绿色；`stage=1` Logo 按 `category=0/1/2` 使用极兔红、韵达黄、中通蓝。关闭“叠加框”只隐藏 QML 图层，V2 JPEG 文件本身始终不烧入框。

主界面显示系统遥测、图像链路、包裹图像记录、置信度阈值和电机调速。设备控制使用公共头 `type=0x11 CONTROL_JSON`：Host 连接后自动发送 `{"op":"get"}`，并用板端回传的 `state/capabilities/error` 校正界面状态。支持屏幕亮度、相机 ISP、检测/叠框、面单与 Logo 阈值、A/B/C 三路速度、图像/指标上报和设备重启。

旧 `type=0x12` 仍用于接收板端 SIM line，不再用于设备设置。系统维护页提供监听状态、端口、保存目录、运行日志、重新监听和同步时间。板端 DEV 页调试项不在上位机中镜像。

## Windows

Run from a Qt-enabled terminal, such as "Qt 6.x for Desktop" or a Developer PowerShell where Qt is on `PATH`.

```powershell
cmake -S . -B build\windows-release -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build\windows-release
cmake --install build\windows-release --prefix dist\windows
.\dist\windows\bin\esp32_host.exe
```

The Windows install step runs Qt's deploy logic and copies the required Qt DLLs,
plugins, and QML imports into `dist\windows`.

If Windows Firewall asks, allow private network access. The Ethernet adapter should use:

- PC: `192.168.10.1/24`
- Board: `192.168.10.2`

Runtime data is saved under the user's Documents directory in `ESP32Host`.

## Tests

```bash
cmake -S . -B build/linux-debug -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build/linux-debug
ctest --test-dir build/linux-debug --output-on-failure
cmake --build build/linux-debug --target appesp32_host_qmllint
```

协议测试覆盖 V1/V2 公共头分派、合法 V2 事务、截断/超量框/越界坐标/长度不一致/损坏 JPEG，以及闭区间框坐标归一化。
