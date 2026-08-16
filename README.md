# ESP32-P4 端侧视觉自动分拣系统

这是运行在 **ESP32-P4** 上的端侧视觉自动分拣系统。摄像头采集后依次进行面单检测和 ROI Logo 分类，再结合包裹状态机控制三段传送带；LVGL 提供板端交互，Qt 6 上位机负责监控与控制。

![ESP32-P4 端侧视觉自动分拣流程](docs/assets/sorting-system-flow.png)

**技术栈：** `ESP32-P4` · `ESP-DL` · `FreeRTOS` · `LVGL` · `Qt 6` · `TCP`

## 实机效果

Qt 6 上位机 Detection 页面：

![Qt 6 上位机图像页](docs/assets/host_page_2_detection.png)

三类包裹的板端实机识别结果：

<table>
  <tr>
    <th>极兔</th>
    <th>韵达</th>
    <th>中通</th>
  </tr>
  <tr>
    <td><img src="docs/assets/board_detection_jt.jpg" alt="极兔包裹识别结果" width="100%"></td>
    <td><img src="docs/assets/board_detection_yd.jpg" alt="韵达包裹识别结果" width="100%"></td>
    <td><img src="docs/assets/board_detection_zt.jpg" alt="中通包裹识别结果" width="100%"></td>
  </tr>
</table>

### 硬件原型 / 实机分拣平台

![ESP32-P4 分拣系统实物](docs/assets/system_front_photo.jpg)

![实际分拣过程](docs/assets/sorting_process_photo.jpg)

<!-- TODO: 补充一段展示包裹进入、识别和电机分拣过程的实机 GIF 或视频链接。 -->

## 核心亮点

- **两级端侧视觉**：面单检测 → ROI Logo 分类，推理和分拣决策不依赖云端。
- **视觉 + 传感器状态融合**：视觉结果与实际包裹轨迹绑定，多帧投票后进入分拣调度。
- **FreeRTOS 实时任务架构**：采集、显示、推理、通信和监控独立运行，通过 Queue / EventGroup / Mutex 协作。
- **自动分拣状态机**：管理三段传送带、传感器交接、占用保护、超时、异常兜底和急停。
- **板端 + Qt 上位机**：LVGL 显示检测结果，Qt Quick 提供图像、历史、参数和设备监控。

## 关键结果

| 端侧两级推理 | 有包裹连续推理 | 识别正确率 | 分拣速度 |
| ---: | ---: | ---: | ---: |
| 约 60–80 ms | 约 6–7 FPS | 超过 95% | 超过 15 件/分钟 |

## 系统架构

```mermaid
flowchart LR
    C[SC2336 MIPI Camera] --> VF[Vision Frame Ring]
    S[S1 S2 Photoelectric Sensors] --> IO[Real IO Observer]
    VF --> V[Waybill Detector]
    V --> R[ROI Crop]
    R --> L[Logo Classifier]
    L --> F[Multi Frame Fusion]
    IO --> CTRL[Sorter Scheduler]
    F --> CTRL
    CTRL --> M[Motors A B C]
    CTRL --> UI[LVGL UI]
    MON[System Monitor] --> UI
    CTRL --> TCP[Ethernet TCP]
    F --> TCP
    TCP -->|control metrics 5000| H[Qt 6 Host]
    TCP -->|JPEG image 5001| H
```

初始化顺序也体现了模块依赖：`LCD → Touch → Camera → Motor → Encoder → LVGL/UI → Monitor → Vision → Ethernet → Sorter`。触摸侧先建立摄像头复用的 I2C 总线，视觉侧随后创建 frame ring、预览任务和推理任务。

## 核心技术实现

### Edge AI Pipeline

整幅传送带画面同时包含包装、背景和面单。当前实现采用两个阶段：

1. `vision_fetch` 从相机 frame ring 获取最新 RGB888 帧；推理前复制到稳定帧槽，避免相机 mmap buffer 在推理期间被回收。
2. `waybill.espdl` 在整图上定位面单，取最高分框并在 PSRAM 中复制 ROI。
3. `logo.espdl` 只对 ROI 推理，输出 Logo 框和三类置信度；Logo 框再加回面单左上角偏移，统一回到原图坐标系。
4. `vision_detect` 将结果裁剪到预览坐标系，生成 LVGL 叠框、日志字段和上传元数据；连续 miss 保持最近一次命中最多 5 帧，避免 UI 抖动。

模型由 ESP-DL 的 `dl::Model + ImagePreprocessor + ESPDetPostProcessor` 组成，模型文件从 SPIFFS 加载，当前默认使用 INT8 模型运行路径。图像帧、ROI 和预览中转缓冲按用途分别使用 PSRAM / DMA-capable memory；PPA 用于预览缩放和 framebuffer 搬运。阈值可从板端 UI/上位机下发，模型选择状态持久化到 NVS。

### FreeRTOS 任务与数据流

| Task / 模块 | 职责 | 主要同步方式 |
| --- | --- | --- |
| `cam_isp` / camera driver | SC2336 采集与 ISP 输出 | task notification |
| `vision_fetch` | 获取帧并写入 frame ring | EventGroup + ring Mutex |
| `vision_disp` | PPA 缩放、画框、更新 LCD 预览 | EventGroup + LVGL lock |
| `vision_det` | 稳定帧、两级推理、结果融合与上传触发 | EventGroup + stable-frame Mutex |
| `eth_control` | 控制 JSON、metrics、分拣状态和重连 | TCP receive buffer + EventGroup |
| `eth_img_prod` / `eth_img_send` | JPEG 编码、队列排队和分块发送 | Queue + image Mutex |
| `sysmon` | CPU、任务栈、heap、FPS 和链路指标 | snapshot Mutex |

推理 task 不持有 LVGL 锁；预览缩放和画框尽量在锁外完成，最后只在写 framebuffer 时进入 UI 临界区。目的是避免显示搬运阻塞 ESP-DL worker。

### 分拣状态机

`sorter_scheduler.c` 保存最多 8 个包裹轨迹，每个轨迹包含包裹编号、类别、所在皮带、状态进入时间、交接时间和 C 段距离。调度器收到三类输入：

- **视觉事件**：将 `waybill → ROI logo` 的最终类别写入对应包裹；
- **传感器事件**：S1 建包，S2 触发 A→B，后续传感器/编码器用于释放与出料；
- **时间/占用事件**：皮带被占用时保持包裹，超时后进入 fallback 或结束轨迹。

电机事件通过 BSP 转换为方向、速度和停止/刹车命令。识别失败不让流水线永久等待，而是使用 `CLASS1 → CLASS2 → CLASS3` 循环兜底；`estop` 会对 A/B/C 三路发出 brake，并在解除后重新调度。

#### Package Lifecycle / 包裹状态机

```mermaid
stateDiagram-v2
    [*] --> WAITING_VISION: S1 rising edge / package_new
    WAITING_VISION --> WAITING_AB: vision result
    WAITING_VISION --> WAITING_AB: vision timeout / fallback class
    WAITING_AB --> HOLDING_AT_S2: S2 active or timeout
    HOLDING_AT_S2 --> ON_B_TO_CLASS1: reserve B / class1 route
    HOLDING_AT_S2 --> ON_B_TO_S4: reserve B / class2 or class3
    ON_B_TO_CLASS1 --> DONE: S3 event or timeout fallback
    ON_B_TO_S4 --> HOLDING_AT_S4: S4 active or timeout
    HOLDING_AT_S4 --> ON_C_EXIT: reserve C / handoff delay
    ON_C_EXIT --> DONE: encoder distance or fallback timeout
    WAITING_AB --> ERROR: invalid result / fault
    HOLDING_AT_S2 --> HOLDING_AT_S2: B occupied / hold
    HOLDING_AT_S4 --> HOLDING_AT_S4: C occupied / hold
```

当前硬件配置启用 S1=`GPIO22`、S2=`GPIO23`；S3/S4 在配置中保留为 `-1`，因此后两级可由调度器/超时策略接管，但不应在没有对应接线时描述为已完成四传感器实测。传感器实时 I/O 以 10 ms 轮询、20 ms debounce，调度器以 100 ms 周期 tick，并在状态转换时检查占用、交接延时和 timeout。

### TCP 与 Qt 上位机

板端固定链路为 `192.168.10.2 ↔ 192.168.10.1`：

| Channel | Port | 内容 |
| --- | ---: | --- |
| control / metrics | 5000 | `CONTROL_JSON`、设备状态、metrics、包裹/电机事件、time sync |
| image | 5001 | 新包裹 JPEG 快照及检测元数据 |

公共头为 40 字节，接收端先校验 magic/version/header size/payload length，再等待完整 packet，因此 TCP 半包与粘包不会直接被当成一条消息。Image V2 在 JPEG 前携带 32 字节 metadata 和最多 8 个 16 字节检测框，Qt 上位机据此在 QML 中叠加面单框/Logo 框。Qt host 源码当前作为独立工程维护。

上位机采用 `QML Page → HostController / HostNetworkWorker → PacketProtocol → TCP` 分层：板端断开时仍可查看已有界面；只有连接建立后，界面上的控制操作才会发送到板端。

## 性能与测试结果

测试条件：自然光照，各角度摆放包裹；部分 Logo 被黑色墨痕遮挡。

| 指标 | 已有结果 | 条件 / 说明 |
| --- | ---: | --- |
| 三类识别置信度 | 极兔、韵达、中通均超过 80% | 三张板端实机识别图 |
| 单轮两级端侧推理 | 约 60–80 ms | 已有实测 P50 67.2 ms、P95 75.0 ms、最大 79.1 ms |
| 有包裹连续推理 | 约 6–7 FPS | 进入面单检测和 ROI Logo 分类两级推理 |
| 无包裹连续推理 | 约 13 FPS | 未检测到包裹时跳过二级 Logo 推理 |
| 分拣速度 | 超过 15 件/分钟 | 实机分拣记录 |
| 识别正确率 | 超过 95% | 真实包裹测试记录 |

## 硬件与软件环境

| 项目 | 当前配置 |
| --- | --- |
| MCU | ESP32-P4，CPU 360 MHz，启动日志显示约 32 MB PSRAM，PSRAM 200 MHz |
| Camera | SC2336 MIPI CSI，RAW8 1024×600@30；ISP 输出 RGB888 |
| Display / Touch | 1024×600 MIPI DSI/DPI LCD，GT911 touch |
| Actuator | A/B/C 三路直流电机 PWM；编码器接口已预留 |
| Firmware | ESP-IDF 5.5.4、FreeRTOS、LVGL 9.5、ESP-DL、PPA |
| Host | Qt 6 / Qt Quick / QML，C++，CMake + Ninja |

## 项目结构

```text
ESP32P4_Detection/
├── main/                         # 启动与初始化编排
├── components/
│   ├── bsp/                      # LCD、Touch、Camera、电机、编码器、传感器
│   ├── vision/                   # frame ring、模型、ROI、后处理、上传触发
│   ├── Sorter_app/               # 包裹状态机、传感器融合、电机调度
│   ├── Ethernet_app/             # TCP control/metrics/image
│   ├── UI/                       # LVGL 页面与事件绑定
│   ├── system_monitor/           # CPU、任务、heap、FPS、链路指标
│   └── screen_uvc/               # 可选 UVC profile，默认未启用
├── model/                        # waybill.espdl / logo.espdl 等模型资源
├── docs/assets/                  # README 展示图片
├── partitions.csv                # Flash 分区
└── sdkconfig.defaults            # ESP-IDF 默认配置
```

## 构建与运行

### ESP32-P4 firmware

```bash
cd ESP32P4_Detection
idf.py set-target esp32p4
idf.py build
idf.py -p <串口> flash monitor
```

烧录前确认 `components/bsp/include/sorter_debug_config.h` 中的 GPIO、传感器有效电平和电机默认输出与实际接线一致。模型分区资源由 `model/` 和 `partitions.csv` 配合使用。

### Qt host

Qt host 为独立工程，使用 Qt 6 / QML / CMake 实现；本仓库保留板端协议、数据字段和链路说明。复现上位机时，将主机地址设为 `192.168.10.1`，监听 TCP `5000/5001`。

## Roadmap / 后续计划

- 当前展示固件默认以 S1/S2 实体输入为主，S3/S4 及编码器需要按最终机械接线补齐实机验证。
- UVC 屏幕流代码已保留，但生产启动路径默认关闭；需要单独 profile 验证 JPEG DMA 内存与视觉延迟。
- 现有准确率/吞吐数据仍是特定样本和光照条件下的记录，后续应补充按类别、光照和包裹间距分组的 benchmark。
- 可继续完善异常包裹的机械回收、传感器故障诊断和更细的电机闭环控制。
