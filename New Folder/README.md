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
- `5001/tcp`：JPEG 图像包，packet type `0x01`，pixel format `2`

主界面显示系统遥测、图像链路、包裹图像记录、置信度阈值和电机调速。电机速度通过 packet type `0x12`
发送 `CONFIG a_speed=<v> b_speed=<v> c_speed=<v>`。亮度、置信度阈值、检测开关和预览叠加框当前为上位机本地状态。
系统维护页提供监听状态、端口、保存目录、运行日志、重新监听和同步时间。板端 DEV 页调试项不在上位机中镜像。

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
