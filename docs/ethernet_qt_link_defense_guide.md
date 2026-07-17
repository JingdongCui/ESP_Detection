# ESP32-P4 以太网链路与 Qt 上位机技术详解

> 用途：理解当前工程中“板端视觉结果如何通过以太网到达 Qt，上位机如何显示、控制并反馈状态”的完整实现，便于系统讲解、代码走读和答辩。
>
> 本文以 2026-07-17 当前源码为准：板端 `ESP32P4_Detection` 分支 `feat/screen-uvc-stream`、提交 `0af4c90`；上位机 `esp32_host_no_inference` 分支 `master`、提交 `3668ddb`。

## 1. 一句话概括技术路线

本系统采用“ESP32-P4 边缘推理 + 有线以太网双 TCP 通道 + Qt/QML 可视化与远程控制”的架构：ESP32-P4 完成图像采集、模型推理、分拣决策和 JPEG 编码，主动连接 PC 上的两个 TCP 服务；Qt 上位机只负责协议解析、结果展示、运行监控和参数控制，不在 PC 上重复推理。

答辩时可以先讲下面四点：

1. 板端和 PC 使用固定 IP 直连，板端是 TCP Client，Qt 是 TCP Server。
2. 5000 端口传控制、状态、遥测和时间同步；5001 端口只传 JPEG 图像事务。
3. 每个业务包都有固定 40 字节公共头，TCP 粘包和拆包由 `payload_len` 驱动的流式状态机解决。
4. 图像、检测框、类别、置信度和推理耗时来自同一个稳定帧槽，Qt 只做显示和框坐标映射，因此不会把上一帧的框画到下一帧图像上。

## 2. 整体框架

```text
ESP32-P4 板端
┌──────────────────────────────────────────────────────────────┐
│ SC2336 相机 → 稳定帧槽(3个) → ESP-DL 推理 → 分拣分类提交     │
│                              │                               │
│                              └→ 图像上报触发                 │
│                                  ↓                           │
│                        非阻塞编码队列(深度2)                 │
│                                  ↓                           │
│                        JPEG生产任务 → 就绪槽(2个)            │
│                                           ↓                  │
│ 系统监控 → JSON遥测 → 控制任务 → TCP 5000   图像任务 → 5001 │
└───────────────────────────────┬──────────────────────────────┘
                                │ 有线以太网，TCP
┌───────────────────────────────▼──────────────────────────────┐
│ Qt 6 上位机                                                  │
│ HostNetworkWorker（独立 QThread）                            │
│   ├─ QTcpServer :5000 → 控制流缓存 → 协议分包                │
│   └─ QTcpServer :5001 → 图像流缓存 → V1/V2 校验与解析         │
│                  ↓ Qt queued signals                         │
│ HostController（GUI 主线程）                                 │
│   ├─ 指标、控制状态、图像历史、文件保存                      │
│   └─ Q_PROPERTY / Q_INVOKABLE                                │
│                  ↓                                           │
│ QML：概览页 / 视觉页 / 设备控制页 / 系统维护页               │
└──────────────────────────────────────────────────────────────┘
```

这条路线的核心边界是：

- 实时采集、推理和真实分拣属于板端。
- 网络层传输“结果和状态”，不传原始连续视频流。
- Qt 不依赖模型运行环境，部署复杂度和 PC 算力要求更低。
- 控制命令采用请求—执行—完整状态回传，Qt 最终以板端真实状态为准。

## 3. 物理网络和连接角色

### 3.1 板端以太网硬件

板端使用 ESP32-P4 内部 EMAC，通过 RMII 连接板载 IP101 PHY。当前配置为：

| 项目 | 当前值 |
| --- | --- |
| PHY | IP101 |
| 接口 | RMII |
| MDC | GPIO31 |
| MDIO | GPIO52 |
| PHY Reset | GPIO51 |
| PHY Address | 1 |

底层入口是 `components/bsp/ethernet_init.c` 的 `example_eth_init()`：创建 ESP32 EMAC、创建 IP101 PHY，然后安装 ESP-IDF Ethernet Driver。上层 `ethernet_app_start()` 再将驱动接入 `esp-netif` 和默认事件循环。

### 3.2 固定 IP

| 设备 | 地址 |
| --- | --- |
| Qt 上位机网卡 | `192.168.10.1/24` |
| ESP32-P4 | `192.168.10.2/24` |
| 子网掩码 | `255.255.255.0` |
| 板端配置的网关 | `192.168.10.1` |

板端在 `configure_static_ip()` 中停止 DHCP Client，然后设置静态地址。PC 网卡也必须手动配置为 `192.168.10.1/24`。

### 3.3 为什么是“Qt 监听、板端主动连接”

Qt 使用 `QTcpServer` 监听 5000 和 5001，ESP32-P4 使用 BSD Socket 主动连接 `192.168.10.1`。这样设计有三个实际好处：

- PC 的地址和端口固定，板端启动后可以自动重连，不需要用户在板端输入目标地址。
- Qt 能在板端上电前先启动，板端获得 IP 后自然建立两条连接。
- 板端掉线或重启时，Qt 保持监听；板端恢复后重新连接即可。

连接不是一条，而是两条独立 TCP 流：

| 端口 | 方向 | 主要内容 | 设计目的 |
| --- | --- | --- | --- |
| 5000 | 双向 | 遥测、控制 JSON、状态、时间同步、兼容 SIM line | 小包、低延迟、需要双向交互 |
| 5001 | 板端到 Qt | JPEG 图像 V1/V2 | 隔离大包，避免图像发送阻塞控制和遥测 |

这解决了单连接常见的队头阻塞问题：即使一张 JPEG 正在分段发送，控制命令仍可在 5000 上独立往返。

## 4. 板端启动与任务模型

### 4.1 启动路径

`main/system_init.c` 的当前顺序是：NVS、LCD、触摸、相机、电机、编码器、LVGL/UI、系统监控、Ethernet、Vision，最后启用真实电机输出和传感器输入。

Ethernet 在 Vision 前启动有意为之：`start_tcp_tasks()` 会先建立图像提交回调，随后 Vision 启动后产生的有效快照可以直接进入网络流水线。

`ethernet_app_start()` 的主要步骤是：

1. 创建 Ethernet EventGroup。
2. 初始化 EMAC 和 IP101 PHY。
3. 初始化 `esp-netif` 与默认事件循环。
4. 建立 Ethernet netif 并绑定驱动。
5. 停止 DHCP，设置静态 IP。
6. 注册链路事件和 Got-IP 事件。
7. 启动 Ethernet Driver。
8. 创建控制、图像生产、图像发送三个 FreeRTOS 任务。

只有收到 `IP_EVENT_ETH_GOT_IP` 后，事件组才置位 `TCP_CONNECT_READY_BIT`，TCP 任务才尝试连接 PC。链路断开时会清除连接位、清理待编码/待发送图像，并更新板端 UI 状态。

### 4.2 三个网络任务

| 任务 | 核 | 优先级 | 栈 | 职责 |
| --- | ---: | ---: | ---: | --- |
| `eth_control` | 0 | 4 | 8192 B | 5000 收包、控制处理、时间同步、1 秒遥测 |
| `eth_img_prod` | 0 | 3 | 8192 B | RGB 稳定帧转 JPEG，组装 V2 元数据和检测框 |
| `eth_img_send` | 0 | 2 | 6144 B | 连接 5001、发送就绪 JPEG、清理过期帧 |

三个任务的栈通过带 capability 的创建接口放在 PSRAM，减少内部 RAM 压力。优先级上控制任务最高、编码其次、图像发送最低，体现“控制可用性优先于大数据吞吐”的原则。

### 4.3 重连机制

板端两个连接任务都运行永久循环：

- 等待 Got-IP 事件位。
- 创建 socket，使用非阻塞 `connect()`。
- 最多等待 5 秒连接完成。
- 成功后恢复阻塞模式，开启 `TCP_NODELAY`，设置 1 秒发送超时。
- 连接失败等待 2 秒再试；连接建立后异常断开，关闭 socket，等待 1 秒再进入下一轮。

图像通道空闲时也用 `recv(MSG_PEEK | MSG_DONTWAIT)` 检查对端是否已经关闭，避免“没有图片可发时永远发现不了断线”。

## 5. 应用层公共协议

TCP 只保证有序字节流，不保留应用消息边界。因此工程在 TCP 上增加固定公共头。

### 5.1 40 字节公共头

所有多字节字段使用小端序。Magic 为 `0x32505345`，按小端字节观察是 `45 53 50 32`，即 ASCII `ESP2`。

| 偏移 | 大小 | 字段 | 含义 |
| ---: | ---: | --- | --- |
| 0 | 4 | `magic` | `0x32505345`，帧同步标记 |
| 4 | 2 | `version` | 普通包为 1，V2 图像为 2 |
| 6 | 2 | `type` | 业务类型 |
| 8 | 4 | `header_size` | 固定 40 |
| 12 | 4 | `seq` | 发送序号 |
| 16 | 8 | `timestamp_ms` | 发送端 Unix 毫秒时间 |
| 24 | 4 | `payload_len` | 紧随其后的负载字节数 |
| 28 | 2 | `width` | 图像宽度，非图像为 0 |
| 30 | 2 | `height` | 图像高度，非图像为 0 |
| 32 | 2 | `pixel_format` | JPEG 为 2 |
| 34 | 2 | `reserved` | V1 兼容字段 |
| 36 | 4 | `reserved2` | V1 兼容字段 |

板端结构使用 `packed` 并用 `_Static_assert` 保证公共头恰好 40 字节；Qt 不直接强转 C 结构，而是用 `qFromLittleEndian()` 按偏移解析，避免编译器对齐和主机字节序造成歧义。

### 5.2 业务类型

| Type | 名称 | 当前用途 |
| ---: | --- | --- |
| `0x01` | IMAGE_RESULT | 5001 图像结果，当前板端发送 V2 |
| `0x02` | METRICS_JSON | 5000 板端运行指标 |
| `0x03` | DETECTION_JSON | Qt 保留兼容解析；当前板端主路径不发送 |
| `0x10` | TIME_SYNC_JSON | Qt 向板端校时 |
| `0x11` | CONTROL_JSON | 双向设备控制和状态同步 |
| `0x12` | SIM_LINE | 兼容调试文本；当前板端输出宏默认关闭，不是正式控制协议 |

### 5.3 Qt 如何解决粘包、拆包和错位

`HostNetworkWorker` 为控制和图像通道各维护一个 `QByteArray` 流缓存。`readyRead` 到来时执行：

1. `readAll()` 追加到对应通道缓存。
2. 缓存少于 40 字节时等待下一次数据。
3. 从头解析 Magic、版本和头长度；无效则丢 1 字节继续寻找下一个合法头。
4. 检查 `payload_len` 不超过 8 MiB。
5. 缓存尚未达到 `40 + payload_len` 时继续等待。
6. 收齐后切出一个完整 payload，交给业务分发，然后继续解析缓存中剩余包。

因此一次 `readyRead` 收到半包、一个包或多个包都不会影响业务层。

板端接收控制包采用相同思想，但限制更严格：接收缓存 1024 字节，单个 Host→Board payload 最大 512 字节。当前时间同步和控制命令都远小于该上限。

## 6. 控制通道 5000 的完整闭环

### 6.1 连接时序

```text
Qt 启动
  └─ HostNetworkWorker 在独立 QThread 监听 5000/5001

ESP32-P4 Got IP
  ├─ eth_control 连接 Qt:5000
  └─ eth_img_send 连接 Qt:5001

Qt 接受 5000
  ├─ 立即发送 TIME_SYNC_JSON
  └─ 通知 HostController “控制已连接”
       └─ HostController 自动发送 CONTROL_JSON {"op":"get"}

板端收到 get
  └─ 回传 {"op":"state", "settings":..., "capabilities":...}

Qt 应用板端状态
  └─ 更新 Q_PROPERTY → QML 控件自动刷新
```

Qt 的“connected”状态以控制 socket 为准。图像 socket 可以独立连接或断开，因此“设备在线”不等同于“5001 一定正在收图”。

### 6.2 时间同步

Qt 发送示例：

```json
{"unix_ms":"1784300000000","tz_offset_min":480}
```

板端用 `settimeofday()` 设置系统时间，再设置时区。公共头中的 `timestamp_ms` 随后具有可读的 Unix 时间意义。这里同时保留板端 `esp_timer` 单调时间用于超时和耗时计算，避免系统校时引起业务超时跳变。

### 6.3 CONTROL_JSON 请求

读取完整状态：

```json
{"op":"get"}
```

设置参数：

```json
{"op":"set","key":"sorter.motor_a_speed","value":65}
```

执行动作：

```json
{"op":"action","key":"device.restart"}
```

当前可写控制项：

| 分类 | Key | 类型/范围 | 板端执行对象 |
| --- | --- | --- | --- |
| 显示 | `display.screen_brightness` | 整数 0–100 | LCD 背光 |
| ISP | `camera.brightness` | capability 给出的 min/max/step | 摄像头 ISP |
| ISP | `camera.contrast` | capability 给出的 min/max/step | 摄像头 ISP |
| ISP | `camera.contrast_auto` | bool | ISP 自动对比度 |
| ISP | `camera.saturation` | capability 给出的 min/max/step | 摄像头 ISP |
| ISP | `camera.saturation_auto` | bool | ISP 自动饱和度 |
| ISP | `camera.hue` | capability 给出的 min/max/step | 摄像头 ISP |
| 视觉 | `vision.detection_enabled` | bool | 推理总开关 |
| 视觉 | `vision.preview_overlay_enabled` | bool | 板端预览叠框开关 |
| 视觉 | `vision.waybill_threshold` | 整数 0–100 | 面单阈值 |
| 视觉 | `vision.logo_threshold` | 整数 0–100 | Logo 阈值 |
| 分拣 | `sorter.motor_a_speed` | 整数 0–100 | A 带设定速度 |
| 分拣 | `sorter.motor_b_speed` | 整数 0–100 | B 带设定速度 |
| 分拣 | `sorter.motor_c_speed` | 整数 0–100 | C 带设定速度 |
| 上报 | `report.image_enabled` | bool | 图像上报总开关 |
| 上报 | `report.metrics_enabled` | bool | 指标上报总开关 |
| 动作 | `device.restart` | action | 板端重启 |

对于 B 忙限速固件，Qt 设置和状态中显示的是 A 带“设定速度”；调度器在 B 被预留或占用时，实际输出会再限制为 `min(设定速度, 60%)`。上位机目前不显示这个瞬时限速值。

### 6.4 为什么不是“发完命令就相信 UI”

板端收到 `set` 后会：

1. 校验 JSON、`op`、`key` 和 value 类型。
2. 校验数值范围；ISP 还校验硬件 capability 的 min/max/step。
3. 调用真实业务接口执行设置。
4. 用 LVGL 锁同步板端界面。
5. 回传完整 `state`，不是只回一个简单 ACK。

若失败，板端返回：

```json
{"op":"error","key":"...","message":"..."}
```

Qt 收到 error 后记录原因，并主动重新请求完整状态。这形成“Qt 请求—板端验证执行—板端真实状态回传—Qt 校正界面”的闭环，避免 UI 显示值和设备实际值长期不一致。

滑块连续拖动时，`HostController` 会按 key 合并重复待发值，并以 100 ms 间隔逐条发送，防止鼠标事件产生大量控制包。每个成功设置仍由板端完整状态回传完成最终校正。

### 6.5 遥测

板端控制任务默认每 1 秒读取一次 `system_monitor` 快照并发送 JSON，主要字段包括：

- CPU 总占用、CPU0、CPU1。
- free heap、历史最小 heap、内部 RAM、PSRAM、最大连续块。
- 运行时间。
- 图像队列深度、编码数、发送数。
- 背压丢弃、过期丢弃、编码失败、发送失败、无帧、无连接次数。
- 最近一张图的 frame id、框数量、JPEG 大小、编码耗时、发送耗时。

Qt 解析后更新概览卡片和趋势数据，并把原始 JSON 追加保存为：

```text
用户 Documents/ESP32Host/telemetry.jsonl
```

指标关闭或间隔设为 0 时不会补发历史积压；恢复后只发送新的快照。

## 7. 图像通道 5001 的完整链路

### 7.1 图像不是连续视频

当前 Ethernet 图像链路发送的是“包裹识别成功快照”，不是持续视频。当前触发条件是同一推理帧同时存在有效面单结果和 Logo 结果，并且从“无有效包裹”进入“首次有效命中”的上升沿。

同一包裹连续被模型命中时不会重复发送。当前代码允许保留 3 个 miss 帧，并在第 4 个连续 miss 时清除上一次命中状态；此后下一次有效命中才会再发一张。换句话说，常量 `VISION_DISPLAY_MISS_KEEP_COUNT=3` 表示“保留 3 帧”，不是“第 3 个 miss 当场清零”。因此：

- 5001 已连接但没有真实包裹时，无图是正常现象。
- 图像上报开关必须开启。
- 检测必须开启并同时得到面单和 Logo 有效结果。

当前板端 LVGL 网络页在初始化时默认把“图像上报”开关设为关闭，指标上报默认开启。可以通过板端 UI 或 Qt 设备控制页开启图像上报。

### 7.2 同帧一致性如何保证

Vision 维护 3 个稳定帧槽，每个槽约束为以下状态机：

```text
FREE → COPYING → INFERENCING → ENCODE_PENDING → ENCODING → FREE
```

具体过程：

1. 从最新相机帧获取引用。
2. 用 PPA 把整帧复制到独立的 1024×600 RGB888 稳定槽。
3. 在这个稳定槽上执行推理。
4. 把同一次推理得到的 frame id、采集时间、主类别、置信度、耗时和最多 8 个框写回该槽的 upload 描述。
5. 槽进入 `ENCODE_PENDING` 后提交给 Ethernet。
6. JPEG 生产任务将槽切到 `ENCODING`，编码完成后才释放回 `FREE`。

因为像素和检测元数据共享一个槽及同一个生命周期，编码阶段不会读到被相机或下一轮推理覆盖的数据。这是“图像和框同帧”的关键，不是靠时间上大概对齐。

### 7.3 非阻塞和背压

推理任务向 Ethernet 提交时不等待网络发送：

- 提交门锁采用立即尝试。
- 编码输入队列深度为 2，使用非阻塞 `xQueueSend(..., 0)`。
- 没有 5001 连接、上报关闭或队列满时，当前快照直接拒绝并释放稳定槽。
- JPEG 就绪槽也是 2 个；没有空槽时记为 backpressure 丢弃。
- 已编码但超过 15 秒仍未发送的图像记为 stale 并丢弃。

这里选择“丢旧结果，不阻塞推理和真实分拣”。因为本系统更关心当前包裹和控制实时性，而不是保证每一张诊断图都传完。

### 7.4 JPEG 编码

当前参数：

| 项目 | 值 |
| --- | --- |
| 输入 | 1024×600 RGB888 |
| 编码 | ESP JPEG Encoder |
| Quality | 70 |
| Chroma subsampling | 4:4:4 |
| 输出槽 | 2 个，每个最大 1 MiB，位于 PSRAM |
| 发送分块 | 8 KiB |

JPEG 编码与网络发送分成两个任务。这样慢 socket 不会占用 JPEG 生产任务，也不会直接阻塞 Vision 推理任务。

### 7.5 V2 图像事务

V2 图像包的结构是：

```text
40 B 公共头
32 B ImageMetaV2
box_count × 16 B ImageBoxV2
jpeg_len B 干净 JPEG
```

32 字节元数据：

| 偏移 | 类型 | 字段 |
| ---: | --- | --- |
| 0 | u32 | `frame_id` |
| 4 | u64 | `capture_timestamp_us` |
| 12 | u32 | `jpeg_len` |
| 16 | u16 | `box_count`，最大 8 |
| 18 | u16 | `primary_class_id` |
| 20 | u16 | `primary_confidence_x1000` |
| 22 | u16 | `infer_time_ms` |
| 24 | u16 | `source_width` |
| 26 | u16 | `source_height` |
| 28 | u16 | `meta_size=32` |
| 30 | u16 | `box_size=16` |

每个 16 字节框：

| 字段 | 类型 | 含义 |
| --- | --- | --- |
| `stage` | u8 | 0=面单，1=Logo |
| `category` | u8 | Logo 类别 0=极兔、1=韵达、2=中通 |
| `score_x1000` | u16 | 0–1000 置信度 |
| `x1,y1,x2,y2` | u16×4 | 基于 JPEG 原图的闭区间坐标 |
| `flags` | u16 | 当前为 0，预留扩展 |
| `reserved` | u16 | 当前为 0 |

当前 V2 JPEG 不烧入检测框；框作为结构化元数据发送，由 Qt/QML 叠加。这样同一张干净图可以自由开关框、改变颜色和显示标签，也避免板端绘图再次修改 JPEG 像素。

Qt 同时保留 V1 图像兼容：V1 payload 是纯 JPEG，主类别和百分比置信度放在公共头保留字段中，没有检测框，推理耗时记为 0。

### 7.6 Qt 收图后的处理

网络线程先完成 V1/V2 结构校验。V2 至少检查：

- 版本、类型、JPEG pixel format 和尺寸。
- `meta_size=32`、`box_size=16`、框数不超过 8。
- 公共头尺寸与元数据尺寸一致。
- payload 总长度严格等于 `32 + 框数组 + jpeg_len`。
- score 不超过 1000、框坐标有序且不越界。
- JPEG 以 `FF D8` 开始。

通过后，网络线程发出 Qt signal，把 JPEG、frame id、类别、置信度、耗时和框列表送到 GUI 主线程的 `HostController`。跨线程连接由 Qt queued connection 处理，网络 socket 和流缓存不会在 QML GUI 线程中运行。

`HostController` 随后：

1. 用 `QImage` 解码 JPEG。
2. 按当前板端色序兼容逻辑执行红蓝通道交换。
3. 执行轻量中值降噪与锐化。
4. 以质量 88 重新编码。
5. 使用 `QSaveFile` 原子保存 `frame_XXXXXX.jpg` 和 `latest_preview.jpg`。
6. 建立图像历史、类别统计和检测框 QVariant 数据。
7. 发出 property change signals，驱动 QML 更新。

运行数据目录是：

```text
用户 Documents/ESP32Host/images/
```

因此上位机最终展示和保存的是经过色序兼容及轻量增强后的图像，不是逐字节原样保存的板端 JPEG。

### 7.7 QML 如何正确叠框

Qt C++ 先把闭区间像素坐标归一化：

```text
x = x1 / image_width
y = y1 / image_height
w = (x2 - x1 + 1) / image_width
h = (y2 - y1 + 1) / image_height
```

`DetectionStage.qml` 使用 `Image.PreserveAspectFit`。图像可能在控件内产生黑边，所以不能直接用整个控件宽高映射。QML 使用：

- `Image.paintedWidth` / `paintedHeight`：实际图像绘制尺寸。
- `(viewport - paintedSize) / 2`：图像在视口中的偏移。
- 归一化框 × painted size：最终屏幕坐标。

框颜色为：面单绿色；Logo 类别 0/1/2 分别为极兔红、韵达黄、中通蓝。关闭叠框只隐藏 QML Repeater，不改变 JPEG 文件。

## 8. Qt 上位机的软件分层

### 8.1 启动和对象关系

`main.cpp` 创建一个 `HostController`，通过 `QQmlContext` 暴露为全局 `hostController`。QML 只访问它的 `Q_PROPERTY` 和 `Q_INVOKABLE`，不直接持有 socket。

`HostController` 构造时：

1. 创建 `QThread`。
2. 创建 `HostNetworkWorker` 并 `moveToThread()`。
3. 连接网络 worker 的状态、字节数、图像、遥测、控制和日志 signals。
4. 启动网络线程和两个监听端口。
5. 创建控制命令节流定时器。

实际在运行的网络路径是 `HostNetworkWorker`。`HostController` 中仍保留一套早期单 socket 的 `QTcpServer/QTcpSocket` 私有方法，但当前 `startServer()` 已经转发给 worker，这套旧方法没有进入启动链路。代码走读和答辩应以 worker 双端口路径为准。

### 8.2 分层职责

| 层 | 文件 | 职责 |
| --- | --- | --- |
| 协议层 | `packetprotocol.h/.cpp` | 小端读写、公共头、V2 图像、控制 JSON 构造/校验 |
| 网络层 | `hostnetworkworker.h/.cpp` | 双 `QTcpServer`、连接管理、流缓存、分包、跨线程 signal |
| 业务/VM 层 | `hostcontroller.h/.cpp` | 状态模型、文件保存、指标计算、控制节流、Q_PROPERTY |
| 表现层 | `*.qml` | 概览、视觉、设备控制、维护页面和交互 |

这种结构使 QML 不需要理解 TCP，也使协议单元测试不依赖 GUI。

## 9. 可靠性与实时性设计

### 9.1 已实现的防护

- TCP 提供有序、可靠传输和重传。
- 固定 Magic、版本、头尺寸和 payload 长度解决消息边界问题。
- Qt 对坏头逐字节重同步，对长度、框和 JPEG 做多层校验。
- 控制和图像分通道，避免大 JPEG 阻塞命令。
- 板端控制优先级高于图像生产和发送。
- 图像队列非阻塞，拥塞时丢图而不是拖慢推理和分拣。
- 三槽稳定帧生命周期保证图像和检测元数据同帧。
- socket 断开后自动重连，并清除旧图队列。
- Qt 网络线程和 GUI 线程分离，避免网络大包卡住界面事件循环。
- 控制命令有类型/范围/capability 校验，并以完整状态回传收敛。
- Qt 协议测试覆盖 V1/V2 头、畸形 V2 payload、越界坐标、长度不一致、损坏 JPEG、控制请求和响应。

### 9.2 当前边界和改进方向

答辩时应主动、准确说明边界：

1. 当前是可信专用局域网方案，没有 TLS、身份认证或应用层签名。TCP 有传输校验，但不能防止同网段恶意控制。产品化可增加 TLS、设备证书、命令权限和重放保护。
2. 网络使用固定 IP，部署简单但不适合任意办公网络；产品化可增加 DHCP、设备发现和可配置地址。
3. Qt 每个端口只保留一个当前板端连接，新连接会替换旧连接；当前定位是单机单板。
4. Qt 的在线状态以 5000 控制连接为准，图像 5001 需要结合图像计数和遥测中的发送/失败指标单独判断。
5. Host→Board 控制 payload 上限为 512 字节，适合当前小 JSON，不适合大配置文件。
6. 当前图像策略是关键事件快照，不保证每次推理或每个包裹都留下多帧视频证据。
7. 当前源码中 Vision 附近仍有旧注释提到“640×375、burn-in”；实际执行常量、V2 协议和 Qt 展示路径是 1024×600 干净 JPEG + QML 叠框，应以执行代码为准。

## 10. 现场排查顺序

### 10.1 先判断是哪一层

```text
物理灯不亮
  → 网线、PHY、供电、RMII/PHY 初始化

物理链路亮但无连接
  → PC IP、防火墙、Qt 是否监听、板端是否 Got IP

5000 已连接但无指标
  → metrics 开关/间隔、公共头解析、板端 control task

5000 正常但无图
  → 5001、image report、detection、真实包裹触发、队列/编码统计

有图但框错位
  → V2 source size、坐标闭区间、paintedWidth/Height 映射

控制滑块回弹
  → 板端校验拒绝或 capability 不支持，查看 CONTROL error 和状态回传
```

### 10.2 PC 常用检查

```bash
ip address
ss -ltnp | grep -E ':5000|:5001'
ping 192.168.10.2
sudo tcpdump -ni <网卡名> host 192.168.10.2 and '(tcp port 5000 or tcp port 5001)'
```

若 Linux 启用 UFW，需要允许板端访问：

```bash
sudo ufw allow in on <网卡名> from 192.168.10.2 to any port 5000 proto tcp
sudo ufw allow in on <网卡名> from 192.168.10.2 to any port 5001 proto tcp
```

重点观察板端日志：`Ethernet Link Up`、`Ethernet Got IP Address`、`control connected to host`、`image connected to host`、`sent image v2`、`metrics send failed`。

重点观察 Qt：监听地址、控制/图像连接日志、图像解析拒绝原因、控制 error，以及概览页的 image queue、encoded、sent、backpressure、stale、send_fail 指标。

## 11. 高频答辩问题与建议回答

### Q1：为什么选择 TCP，不使用 UDP？

控制命令、设备状态和 JPEG 都要求完整、有序。TCP 已提供重传、顺序和拥塞控制，能降低应用层复杂度。图像不是高帧率实时视频，而是事件快照，因此可以接受 TCP 的可靠性优先策略。为了避免 JPEG 影响控制时延，又进一步拆成了 5000 和 5001 两条 TCP 流。

### Q2：TCP 会粘包，系统如何判断一包结束？

每包都有固定 40 字节头，其中 `payload_len` 给出负载长度。接收端维护字节缓存，先收完整头，再等待完整 payload；一次 read 收到半包或多包都能循环解析。Magic、版本和头长度还能用于错误后的重新同步。

### Q3：为什么不把模型放在 Qt 上位机？

目标是边缘智能分拣。模型在 ESP32-P4 上运行，断开 PC 后板端仍具备识别和真实分拣能力；Qt 只承担展示、管理和诊断。这样减少原始图像持续上传带宽，也避免 PC 性能差异改变分拣结果。

### Q4：怎么保证图像和框是同一帧？

相机帧先复制到独立稳定槽，推理、框元数据和 JPEG 编码都引用这个槽。槽具有严格状态机，编码完成前不会释放给下一帧复用。frame id、采集时间、框和像素因此来自同一个对象生命周期。

### Q5：网络慢会不会拖慢推理和分拣？

不会直接阻塞。Vision 使用非阻塞队列提交，JPEG 编码和 socket 发送分别由独立低优先级任务处理。队列满或连接不可用时选择丢弃上报图，而不是等待网络；真实分拣仍在板端调度器中运行。

### Q6：控制命令如何确保真的生效？

Qt 发送带 key/value 的控制 JSON；板端校验类型、范围和硬件 capability，调用真实模块接口执行，再回传完整设备状态。Qt 用回传状态刷新控件。失败时板端返回 error，Qt 记录错误并重新读取状态，不把本地 UI 值当作最终真值。

### Q7：为什么用 Qt/QML？

Qt Network 提供跨平台异步 TCP，QThread 方便隔离网络和 GUI；C++ 层负责协议和状态模型，QML 通过属性绑定快速构建设备控制、趋势卡片和图像叠框。Linux 和 Windows 可复用同一套业务代码。

### Q8：怎么保证界面不卡？

socket、流缓存和协议初步解析都在 `HostNetworkWorker` 的独立 QThread 中。只有解析后的结构化数据通过 queued signal 进入 GUI 主线程。板端也把推理、JPEG 编码和网络发送拆成不同任务，并让控制任务优先级高于图像任务。

### Q9：检测框缩放后为什么不会错位？

协议传原图闭区间坐标，C++ 先归一化。QML 不按控件总尺寸缩放，而是按 `PreserveAspectFit` 后真正的 `paintedWidth/paintedHeight` 和居中偏移映射，所以即使视口比例不同、有黑边，框仍对应实际图像区域。

### Q10：系统安全性如何？

当前比赛原型运行在板端与 PC 直连的可信有线网段，重点是稳定性和实时性，因此尚未加入 TLS 和认证。产品化时会增加双向认证、加密、命令权限、审计和防重放；这是已知工程边界，不应把 TCP 本身误说成安全机制。

## 12. 源码导航

板端：

- `main/system_init.c`：系统启动和 Ethernet/Vision 顺序。
- `components/bsp/ethernet_init.c`：ESP32-P4 EMAC + IP101 PHY 初始化。
- `components/Ethernet_app/ethernet_app.c`：静态 IP、双 socket、任务、协议、遥测、控制、JPEG 队列与重连。
- `components/Ethernet_app/include/image_result_protocol.h`：40 B 公共头、32 B V2 元数据、16 B 检测框。
- `components/vision/framework/vision_detect.c`：推理、首次命中图像触发、分类提交。
- `components/vision/framework/vision_stable_frame.c`：三槽稳定帧生命周期和 Ethernet 提交回调。
- `components/vision/framework/vision_upload.h`：Vision→Ethernet 内部 upload 数据结构。
- `components/system_monitor/system_monitor.c`：板端指标采集。
- `components/UI/sdk/ui.c`：板端网络开关、上报间隔和远程控制状态同步。

Qt 上位机：

- `main.cpp`：创建 `HostController` 并注入 QML。
- `hostnetworkworker.cpp`：当前实际双端口网络线程。
- `packetprotocol.cpp`：公共头、V2 图像和控制 JSON协议。
- `hostcontroller.cpp`：业务状态、控制节流、图像保存、遥测和 QML 属性。
- `DetectionStage.qml`：图像显示和检测框映射。
- `DetectionPage.qml`：最新图像和历史包裹记录。
- `ControlPage.qml`：远程控制界面。
- `tests/packetprotocol_tests.cpp`：协议单元测试。

## 13. 最后记忆框架

答辩时可以按“端—链—议—流—控—稳”六个字展开：

- **端**：板端推理分拣，Qt 展示管理。
- **链**：静态 IP、有线 Ethernet、板端 Client、Qt Server、双 TCP。
- **议**：40 字节公共头，小端，V1/V2，JSON 控制，结构化图像元数据。
- **流**：稳定帧→推理→非阻塞队列→JPEG→5001→Qt Worker→Controller→QML。
- **控**：Qt set/get/action→板端校验执行→完整 state/error 回传。
- **稳**：任务/线程隔离、重连、粘包处理、背压丢图、同帧槽、协议校验。
