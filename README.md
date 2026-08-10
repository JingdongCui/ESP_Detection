# 嵌入式边缘 AI 智能分拣系统

基于 **ESP32-P4** 的视觉识别与自动分拣系统。设备在边缘端完成图像采集、面单定位、快递公司识别、包裹状态管理和电机调度；Qt 6 上位机负责图像与遥测展示、历史记录、参数配置和远程控制。

系统面向三类快递包裹（极兔、韵达、中通），形成了“视觉感知 → 边缘推理 → 状态融合 → 传送带分流 → 数据上报”的完整闭环。实时识别和安全控制不依赖云端，网络链路用于监控、留存和较长时间尺度的数据分析。

![系统实物](docs/report_assets/system_front_photo.jpg)

## 项目亮点

| 方向 | 实现 |
| --- | --- |
| 边缘 AI | ESP32-P4 上运行 ESP-DL，两级模型级联完成面单定位和 Logo 分类 |
| 实时闭环 | 光电传感器建立包裹窗口，视觉结果与传感器时序绑定，调度器驱动三段传送带 |
| 本地交互 | 1024×600 LCD + LVGL，显示检测结果、置信度、推理耗时、日志和系统资源 |
| 上位机 | Qt 6 接收 JPEG、metrics 和分拣状态，提供历史统计、阈值/速度配置和设备控制 |
| 工程化通信 | 控制/遥测与图像分离传输，图像协议兼容 V1/V2，控制支持 `CONTROL_JSON` |
| 可观测性 | 板端和上位机均提供运行指标、链路状态、图像记录和调试日志 |

## 系统架构

```text
┌──────────────────────────────────────────────────────────────┐
│                        感知与执行层                          │
│  SC2336 MIPI 摄像头   光电传感器 S1/S2   三段传送带/电机 A/B/C │
└──────────────┬──────────────────┬───────────────────────────┘
               │图像/到位信号      │PWM/状态
               ▼                  ▲
┌──────────────────────────────────────────────────────────────┐
│                     ESP32-P4 边缘控制层                      │
│  BSP/采集 → 面单检测 → ROI/Logo 分类 → 包裹状态机 → 电机调度  │
│        LVGL 本地 UI、系统监控、JPEG 快照、Ethernet TCP        │
└──────────────────────────┬───────────────────────────────────┘
                           │192.168.10.2 ↔ 192.168.10.1
                           ▼
┌──────────────────────────────────────────────────────────────┐
│                       Qt 6 上位机                             │
│  实时看板 · 视觉检测 · 历史记录 · 设备控制 · 系统维护         │
└──────────────────────────┬───────────────────────────────────┘
                           │结构化运行上下文
                           ▼
                 数据分析与运行优化（可选）
```

一次包裹的处理流程：

1. S1 检测到包裹进入视觉区，调度器创建包裹对象并打开视觉窗口。
2. SC2336 采集图像，一级模型定位面单区域。
3. 从面单区域提取 ROI，二级模型识别极兔、韵达或中通，并输出置信度和检测框。
4. 视觉结果与 S2 等后续传感器事件关联，调度器计算包裹应进入的分流支路。
5. A/B/C 三段传送带按状态机运行，执行交接延时、超时和占用保护。
6. 结果在 LCD 上显示，并通过 control/metrics 与 image 两条 TCP 链路发送至上位机。

## 核心功能

### 1. 两级边缘视觉

系统没有直接对整幅传送带画面做粗粒度分类，而是采用“面单检测 → ROI 分类”的级联路径：

- 一级检测：定位包裹面单，抑制背景、包装外观和传送带纹理干扰。
- 二级分类：在面单 ROI 内识别快递公司 Logo，输出类别、置信度和框坐标。
- 结果稳定：S1 视觉窗口使用多帧置信度加权投票；窗口关闭或 S2 到达时，以已有票数决定最终类别。
- 快照策略：每个新包裹只发送一次 JPEG，连续 miss 达到保持阈值后才重新打开下一次抓拍。

### 2. 传感器与电机分拣闭环

分拣调度器维护包裹编号、类别、传感器事件、传送带占用和超时状态，将识别结果转换为可执行的物理动作。

- S1/S2 用于包裹进入、交接和离开状态确认。
- 三路电机通过双 PWM 控制启停、方向和速度。
- 默认速度、交接延时、皮带超时和 lost timeout 集中在 `components/bsp/include/sorter_debug_config.h` 配置。
- B 支路忙时可对主带 A 做运行时限速，避免包裹在分流口堆积。
- 识别失败包裹按 `CLASS1 → CLASS2 → CLASS3` 循环策略进入兜底分流，避免阻塞流水线。

![实际分拣过程](docs/report_assets/sorting_process_photo.jpg)

### 3. 板端 UI 与系统监控

LVGL 界面覆盖实时看板、参数设置、日志追踪和系统资源页面，支持：

- 实时画面、类别、置信度、推理耗时和当前包裹状态。
- 屏幕/ISP、检测与叠框、面单/Logo 阈值等参数查看与调整。
- CPU、任务、堆内存、帧率和链路状态监控。
- 触摸操作和人工分类弹窗；弹窗期间暂停预览直刷，但不停止采集与推理。

| 板端看板 | 板端设置 | 板端日志 | 板端资源 |
| --- | --- | --- | --- |
| ![](docs/report_assets/board_ui_dashboard.jpg) | ![](docs/report_assets/board_ui_settings.jpg) | ![](docs/report_assets/board_ui_log.jpg) | ![](docs/report_assets/board_ui_system.jpg) |

### 4. Qt 6 上位机

上位机工程位于 [`esp32_host_no_inference/`](esp32_host_no_inference/)，采用 Qt Quick/QML + C++ 实现：

- 性能总览：设备在线状态、吞吐、延迟、类别统计和趋势。
- 视觉检测：接收 JPEG，叠加面单框和 Logo 框，记录检测历史。
- 设备控制：屏幕/ISP、检测阈值、A/B/C 速度、图像/指标上报和重启。
- 系统维护：监听状态、端口、保存目录、运行日志和时间同步。
- 未连接设备时支持展示模式；控制命令仍以真实连接状态为安全门控。

| 性能总览 | 视觉检测 | 设备控制 | 系统维护 |
| --- | --- | --- | --- |
| ![](docs/report_assets/host_page_1_dashboard.png) | ![](docs/report_assets/host_page_2_detection.png) | ![](docs/report_assets/host_page_3_control.png) | ![](docs/report_assets/host_page_4_system.png) |

## 通信与数据协议

板端默认使用静态链路：板端 `192.168.10.2`，上位机 `192.168.10.1`。

| 链路 | 端口 | 内容 |
| --- | ---: | --- |
| control/metrics | 5000 | 控制命令、设备状态、metrics、分拣状态、时间同步 |
| image | 5001 | 包裹 JPEG 快照，packet type `0x01` |

公共头固定 40 字节，所有多字节字段使用小端序。图像链路兼容两种版本：

- **V1**：payload 为纯 JPEG，类别和百分比置信度放在保留字段中，用于旧固件回退。
- **V2**：payload 为图像元数据、最多 8 个检测框和干净 JPEG；包含 frame id、采集时间、推理耗时、主类别、千分制置信度及源图尺寸。

设备控制使用 `type=0x11 CONTROL_JSON`，支持 `get/set/action`、状态回传、能力声明和错误响应。上位机连接后自动查询状态，端侧校验参数范围并回传执行后的完整 state。

## 硬件组成

| 模块 | 规格/用途 |
| --- | --- |
| 主控 | ESP32-P4，32 MB PSRAM，200 MHz |
| 显示与触摸 | 1024×600 MIPI DSI/DPI LCD，GT911 |
| 摄像头 | SC2336 MIPI-CSI，sensor RAW10，ISP 输出 RGB888 |
| 传感器 | E18-D80NK 光电传感器，检测包裹到位 |
| 执行机构 | 三段传送带，三路 12 V 直流减速电机与 PWM 驱动 |
| 机械结构 | 4080 铝型材机架；主带约 100×40 cm，分流带各约 40×40 cm |

![传送带布局](docs/report_assets/conveyor_layout_photo.jpg)

![摄像头安装](docs/report_assets/camera_mount_photo.jpg)

![电源与电机驱动](docs/report_assets/hardware_power_driver_board.jpg)

## 关键实现

### 板端软件分层

```text
main/system_init.c
  ├─ BSP：LCD / Touch / Camera / Motor / Encoder / Sensor
  ├─ UI：LVGL 页面、触摸事件、预览与状态显示
  ├─ Vision：采集、预处理、ESP-DL 推理、投票与快照触发
  ├─ Sorter：包裹状态机、传感器去抖、三路电机调度
  ├─ Ethernet：control/metrics 与 image 双链路
  ├─ System Monitor：任务、CPU、heap、帧率和链路指标
  └─ Screen UVC：按 profile 可选启用的屏幕流输出
```

系统初始化顺序按依赖关系组织为：LCD → Touch → Camera → Motor → Encoder → LVGL/UI → System Monitor → Vision → Ethernet → Sorter → UVC。Touch 先建立可复用的 I2C 总线，再交给摄像头侧复用。

### 上位机软件分层

```text
QML 页面层
  ├─ Dashboard：指标、趋势、在线状态
  ├─ Detection：JPEG、检测框、历史记录
  ├─ Control：参数、速度、动作和能力状态
  └─ Reserve/System：维护、日志、监听与数据目录
        ↓
HostController / HostNetworkWorker
        ↓
PacketProtocol：V1/V2 头解析、CONTROL_JSON、坐标归一化
        ↓
TCP control:5000 / image:5001
```

## 性能记录

以下结果来自已有实测记录，测试条件为真实包裹和室内良好光照：

| 指标 | 结果 |
| --- | ---: |
| 三类包裹识别准确率（300 件） | 96.7%（290/300） |
| 极兔 / 韵达 / 中通准确率 | 98.0% / 93.0% / 99.0% |
| 端侧推理延迟 | 约 72–75 ms，单阶段测试 P95 约 71.4 ms |
| 分拣速度 | 20 件/分钟以上 |
| 安全交接延时 | 100 ms |
| 图像接收成功率 | 99% 以上 |
| 图像上传策略 | 每个包裹对象一次 JPEG |

![极兔识别结果](docs/report_assets/board_detection_jt.jpg)

![韵达识别结果](docs/report_assets/board_detection_yd.jpg)

![中通识别结果](docs/report_assets/board_detection_zt.jpg)

## 目录结构

```text
.
├── main/                         # 系统启动与初始化编排
├── components/
│   ├── bsp/                      # LCD、摄像头、触摸、电机、传感器
│   ├── vision/                   # 视觉采集、模型、后处理与结果发布
│   ├── Sorter_app/               # 包裹状态机与电机分拣调度
│   ├── Ethernet_app/             # TCP 控制、遥测和图像链路
│   ├── UI/                       # LVGL 页面与生成资源
│   ├── system_monitor/           # 任务、内存、性能监控
│   └── screen_uvc/               # 可选屏幕 UVC 输出
├── model/                        # ESP-DL 模型与模型分区资源
├── esp32_host_no_inference/      # Qt 6 上位机
├── docs/report_assets/           # 系统照片、板端 UI、上位机截图
├── partitions.csv               # factory + storage 分区
└── sdkconfig.defaults            # ESP-IDF 默认配置
```

## 构建与运行

### ESP32-P4 固件

环境要求：ESP-IDF 5.5.x、ESP32-P4 工具链、已配置的摄像头/LCD/电机硬件。

```bash
cd ESP32P4_Detection
idf.py set-target esp32p4
idf.py build
idf.py -p <串口> flash monitor
```

模型文件和分区配置位于 `model/`、`partitions.csv`。首次运行前请确认板级引脚、传感器有效电平和电机默认输出与实际接线一致。

### Qt 6 上位机

```bash
cd esp32_host_no_inference
cmake -S . -B build/linux-release -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build/linux-release
./build/linux-release/bin/esp32_host_no_inference
```

主机网卡建议配置为 `192.168.10.1/24`，并放行 TCP `5000/5001`。上位机协议测试：

```bash
cmake -S . -B build/linux-debug -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build/linux-debug
ctest --test-dir build/linux-debug --output-on-failure
```

## 文档与验证记录

- [系统信息与架构资料](docs/report_system_information.md)
- [Ethernet 与 Qt 链路说明](docs/ethernet_qt_link_defense_guide.md)
- [完整项目报告（Markdown）](docs/competition_report_final.md)
- [新版系统报告](report_work/嵌入式边缘AI智能分拣系统_报告.md)
- [上位机协议与构建说明](esp32_host_no_inference/README.md)

## License

项目中的业务代码、模型和硬件资料用于本项目研究与展示。第三方组件遵循其各自许可证，详见对应组件目录和依赖声明。
