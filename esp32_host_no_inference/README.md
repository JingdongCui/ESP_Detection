# ESP32 Host

Qt 6 desktop host for receiving ESP32-P4 Ethernet frames and telemetry.

This copy is the no upper-computer inference variant. It keeps Ethernet
receive, preview, telemetry, and device controls, but does not call the local
YOLO service and does not require `ml/logo_inference_service.py`.

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
cd /home/kazeform/2026upper/esp32_host_no_inference
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
```

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
