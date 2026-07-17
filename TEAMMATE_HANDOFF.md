# 队友交接说明（2026-07-17）

## 配套版本

- Host 功能基线：`6bcee3b feat: implement remote device control dashboard`。
- 推荐配套固件：`c26dba8`，标签 `backup/final-production-candidate-20260717`。
- 生产固件默认关闭 UVC：`CONFIG_SCREEN_UVC_ENABLE=n`。

本压缩包是源码包，不含 `.git`、构建目录、Qt Creator 缓存、运行日志和本机接收图片。解压后目录可直接作为 CMake 源码目录。

## Linux 构建与测试

依赖 Qt 6.5+（Quick、Network）、CMake 3.16+ 和 Ninja。

```bash
cmake -S . -B build/linux-release -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build/linux-release -j
./build/linux-release/bin/esp32_host_no_inference
```

协议测试：

```bash
cmake -S . -B build/linux-debug -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build/linux-debug -j
ctest --test-dir build/linux-debug --output-on-failure
```

打包前已重新执行 build 与 CTest，结果为 1/1 通过。

## 网络配置

- 上位机网卡：`192.168.10.1/24`
- ESP32-P4：`192.168.10.2`
- TCP 5000：控制、状态、遥测、时间同步
- TCP 5001：V1/V2 JPEG 图像

防火墙需要允许 ESP32-P4 访问上位机的 TCP 5000/5001。运行数据保存在当前用户的 `Documents/ESP32Host/`。

## 当前功能

- 四页 Qt/QML 界面：概览、视觉、设备控制、系统维护。
- V1/V2 图像协议，V2 支持面单框和三类 Logo 框叠加。
- `type=0x11 CONTROL_JSON` 完整状态同步：显示亮度、ISP、检测/叠框、阈值、A/B/C 速度、图像/指标上报、重启。
- control/image 双端口自动监听、重连和连接后自动 `get`。
- 协议单元测试覆盖公共头、V2 合法/畸形 payload 和框坐标归一化。

## 已知边界

- 这是源码交接包，不包含 Qt 运行库或 Windows DLL；目标电脑需安装 Qt，或自行执行部署流程。
- 生产固件关闭 UVC 不影响 Ethernet 图像链路。
- 当前软件闭环已联调，但 LCD/ISP 画面、真实包裹、传感器和三路电机仍需在整机现场观察。
- 若视觉页没有图像，先确认真实包裹触发了板端图像上报，再检查 TCP 5001；无包裹时保持空页面是正常行为。
