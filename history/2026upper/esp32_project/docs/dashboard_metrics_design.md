# ESP32-P4 边缘 AI 快递识别 Dashboard 指标设计

## 1. 文档目标

本文档用于定义本工程 UI Dashboard 应展示哪些参数、每个参数的数据来源、可靠性口径、刷新建议，以及哪些参数当前不应作为真实数据展示。

本工程是基于 ESP32-P4 的边缘 AI 快递识别系统，当前已有以下基础能力：

- MIPI CSI 摄像头实时采集
- LVGL Dashboard 显示
- ESP-DL / ESPDet 模型推理
- 目标检测框与 ROI 框叠加显示
- FreeRTOS / heap / PSRAM 系统监控
- Ethernet 状态和 TCP metrics 上报
- RTT 日志输出

Dashboard 设计应区分两类用户：

1. **操作员**：只关心系统是否正常、当前识别结果是否可信。
2. **工程师**：需要定位 AI、摄像头、内存、PSRAM、网络和任务调度问题。

因此，UI 应拆分为：

- **首屏监控页**：展示业务闭环和系统健康摘要。
- **工程诊断页**：展示 AI pipeline、资源占用、任务、PSRAM、摄像头和网络细节。

---

## 2. 设计原则

### 2.1 首屏只展示业务闭环指标

首屏只回答以下问题：

- 当前画面是否正常？
- 当前是否检测到目标？
- 当前识别成什么快递类型？
- 当前识别置信度是否足够？
- 当前系统是否实时运行？
- CPU、内存、网络是否健康？

不应在首屏堆叠调试指标，例如 ROI 失败码、每任务栈水位、PSRAM per-task bandwidth 等。

### 2.2 工程诊断页展示性能拆解

工程诊断页用于定位问题，可以展示：

- AI 各阶段耗时
- ROI 状态
- raw class score
- heap / PSRAM 状态
- 任务 CPU 占比
- 任务栈水位
- 摄像头 timeout
- Ethernet / TCP 状态

### 2.3 不展示无法可靠获取的参数

当前工程中有些 UI 占位字段尚无可靠实时数据源，例如：

- 设备温度
- 真实电机 RPM
- 真实业务成功率
- 真实累计分拣件数
- 网络实时带宽
- 真实包裹物理尺寸

这些字段不能直接显示为真实值，否则会误导使用者。

### 2.4 所有百分比必须有明确分母

例如：

- 置信度：最终检测结果的 `Detection::confidence`
- PSRAM 使用率：`(total_psram - free_psram) / total_psram`
- CPU 使用率：基于 FreeRTOS runtime stats 的 busy time
- 成功率：必须基于业务事件统计，不能直接基于帧级 detection

---

## 3. 当前工程数据来源

### 3.1 识别结果数据源

关键文件：

- `components/vision_app/include/app_yolo.hpp`
- `components/vision_app/vision_app.cpp`
- `components/vision_app/app_yolo.cpp`
- `components/vision_app/yolo_decode.cpp`

核心结构：

```cpp
struct Detection {
    float x1, y1, x2, y2;
    float confidence;
    int class_id;
};
```

可获取参数：

| 参数 | 来源 | 说明 |
|---|---|---|
| 检测框 | `Detection::x1/y1/x2/y2` | 原始图像坐标系中的 bbox |
| 置信度 | `Detection::confidence` | 最终保留检测结果的置信度 |
| 类别 ID | `Detection::class_id` | 对应快递类型索引 |
| 检测数量 | `detections.size()` | 当前帧检测到的目标数量 |

当前类别名定义在 `components/vision_app/vision_preview.cpp`：

```cpp
极兔 / 中通 / 韵达
```

### 3.2 AI 性能数据源

关键文件：

- `components/vision_app/include/app_yolo.hpp`
- `components/vision_app/app_yolo.cpp`
- `components/vision_app/vision_app.cpp`

核心结构：

```cpp
struct YOLOPerfStats {
    int64_t roi_detect_us;
    int64_t ppa_resize_us;
    int64_t quantize_us;
    int64_t preprocess_us;
    int64_t input_copy_us;
    int64_t pre_us;
    int64_t run_us;
    int64_t decode_us;
    int64_t total_us;
    float target_confidence;
    uint32_t detect_count;
    bool runtime_multi_core;
    bool fallback_triggered;
    bool ppa_preprocess;
    bool roi_success;
    bool roi_skipped;
    int roi_fail_reason;
    int roi_x, roi_y, roi_w, roi_h;
    float class_prob[3];
    float class_prob_avg[3];
    float class_prob_max[3];
};
```

可获取参数：

| 参数 | 来源 | 建议用途 |
|---|---|---|
| ROI 检测耗时 | `roi_detect_us` | 工程诊断页 |
| PPA resize 耗时 | `ppa_resize_us` | 工程诊断页 |
| 量化耗时 | `quantize_us` | 工程诊断页 |
| 预处理耗时 | `preprocess_us` | 工程诊断页 |
| 模型 run 耗时 | `run_us` | 首屏或工程页 |
| decode 耗时 | `decode_us` | 工程诊断页 |
| AI 总耗时 | `total_us` | 首屏推荐 |
| raw 最佳分数 | `target_confidence` | 工程诊断页 |
| ROI 状态 | `roi_success / roi_skipped / roi_fail_reason` | 工程诊断页 |
| ROI 坐标 | `roi_x/y/w/h` | 工程诊断页或调试叠加 |
| runtime 状态 | `runtime_multi_core / fallback_triggered / ppa_preprocess` | 工程诊断页 |
| 类别 raw score | `class_prob / avg / max` | 工程诊断页 |

### 3.3 摄像头与预览数据源

关键文件：

- `components/bsp/bsp_cam_sensor.c`
- `components/vision_app/vision_preview.cpp`
- `components/vision_app/vision_app.cpp`

可获取参数：

| 参数 | 来源 | 说明 |
|---|---|---|
| 摄像头分辨率 | `frame_w / frame_h` | 当前视觉任务请求 1024x600 |
| 帧大小 | `frame_size` | RGB888，每像素 3 字节 |
| 摄像头 timeout | `timeout_count` | camera BSP 内已有统计 |
| capture receive 耗时 | `receive_us` | camera BSP 内已有统计 |
| 预览转换耗时 | `FrameTimings` | `vision_preview.cpp` 内已有 |
| LVGL lock 等待 | `FrameTimings` | 用于定位 UI 卡顿 |
| AI FPS | `fps_current` | 当前已绑定 UI |

### 3.4 系统资源数据源

关键文件：

- `main/system_monitor.c`
- `components/Ethernet_app/ethernet_app.c`

可获取参数：

| 参数 | API / 来源 | 说明 |
|---|---|---|
| free heap | `esp_get_free_heap_size()` | 当前剩余 heap |
| min free heap | `esp_get_minimum_free_heap_size()` | 历史最低 heap |
| internal free | `heap_caps_get_free_size(MALLOC_CAP_INTERNAL)` | 内部 RAM 剩余 |
| PSRAM free | `heap_caps_get_free_size(MALLOC_CAP_SPIRAM)` | PSRAM 剩余 |
| PSRAM total | `heap_caps_get_total_size(MALLOC_CAP_SPIRAM)` | PSRAM 总量 |
| largest free block | `heap_caps_get_largest_free_block(MALLOC_CAP_DEFAULT)` | 最大连续空闲块 |
| CPU 使用率 | `uxTaskGetSystemState()` | `ethernet_app.c` 已有计算逻辑 |
| 任务 CPU 占比 | `uxTaskGetSystemState()` | `system_monitor.c` 已打印 |
| 任务栈水位 | `TaskStatus_t::usStackHighWaterMark` | 工程诊断页 |
| PSRAM 带宽估算 | L2 cache counter + DMA 估算 | `system_monitor.c` 已实现 |

### 3.5 网络数据源

关键文件：

- `components/Ethernet_app/ethernet_app.c`
- `components/bsp/ethernet_init.c`

可获取参数：

| 参数 | 来源 | 说明 |
|---|---|---|
| Ethernet link | Ethernet event | link up/down |
| IP 地址 | `IP_EVENT_ETH_GOT_IP` | 当前静态 IP 为 192.168.10.2 |
| MAC 地址 | `ETH_CMD_G_MAC_ADDR` | 当前只在日志中打印 |
| TCP host 状态 | `connect_to_host()` | 需要缓存状态给 UI |
| metrics 序号 | `s_tx_seq` | TCP metrics 使用 |
| 发送失败次数 | send 返回值 | 需要新增统计 |
| 上次发送时间 | TCP task | 需要新增统计 |

---

## 4. 首屏推荐展示参数

首屏建议展示以下参数。

### 4.1 摄像头实时预览

| 项目 | 内容 |
|---|---|
| UI 位置 | 主视觉区域 |
| 数据来源 | `vision_preview_update()` |
| 当前状态 | 已实现 |
| 展示建议 | 保持最大面积显示 |

说明：

摄像头画面是现场判断系统状态的第一入口，应放在首屏最显眼位置。

### 4.2 检测框

| 项目 | 内容 |
|---|---|
| UI 位置 | 叠加在预览画面上 |
| 数据来源 | `Detection::x1/y1/x2/y2` |
| 当前状态 | 已绘制绿色框 |
| 展示建议 | 首屏保留 |

说明：

检测框比坐标数字更直观，首屏应显示框，不应显示 bbox 数值。

### 4.3 当前识别状态

推荐状态：

- `等待画面`
- `未检测到目标`
- `识别成功`
- `低置信度`
- `摄像头异常`
- `网络离线`

| 项目 | 内容 |
|---|---|
| 数据来源 | `detect_ok`、`detections.empty()`、camera/network 状态 |
| 当前状态 | 已有“识别成功 / 未检测到目标” |
| 展示建议 | 扩展为多状态 |

### 4.4 当前快递类型

| 项目 | 内容 |
|---|---|
| UI 名称 | 当前类型 |
| 数据来源 | `Detection::class_id` |
| 当前类别 | 极兔 / 中通 / 韵达 |
| 无目标时 | 显示 `--` |

说明：

首屏只展示最终识别类型，不展示所有 raw class score。

### 4.5 当前置信度

| 项目 | 内容 |
|---|---|
| UI 名称 | 置信度 |
| 数据来源 | 推荐使用最高 `Detection::confidence` |
| 单位 | `%` |
| 展示形式 | 数字 + 进度条 |

重要口径：

- 首屏推荐使用最终 detection 的 `confidence`。
- `YOLOPerfStats::target_confidence` 更适合工程诊断页，因为它表示解码阶段看到的 raw best score，不一定等价于最终业务结果。

推荐颜色规则：

| 置信度 | 颜色 | 状态 |
|---|---|---|
| `>= 80%` | 绿色 | 高可信 |
| `50% ~ 80%` | 黄色 | 需关注 |
| `< 50%` | 红色 | 低置信 |

### 4.6 AI FPS

| 项目 | 内容 |
|---|---|
| UI 名称 | AI FPS |
| 数据来源 | `vision_app.cpp` 中的 `fps_current` |
| 当前状态 | 已绑定 `scr_dashboard_label_runtime_fps` |
| 刷新周期 | 1 秒 |

说明：

FPS 是判断系统实时性的关键指标，适合首屏展示。

### 4.7 AI 耗时

| 项目 | 内容 |
|---|---|
| UI 名称 | AI 耗时 |
| 推荐来源 | `YOLOPerfStats::total_us / 1000` |
| 单位 | ms |
| 刷新周期 | 逐帧或 1 秒平滑 |

口径说明：

- 如果 UI 文案叫“推理耗时”，应使用 `YOLOPerfStats::run_us`。
- 如果希望展示一帧 AI pipeline 成本，应使用 `YOLOPerfStats::total_us`，文案应叫“AI 耗时”。

推荐首屏使用：

```text
AI 耗时 = total_us / 1000 ms
```

工程诊断页再拆分 `preprocess / run / decode`。

### 4.8 CPU 使用率

| 项目 | 内容 |
|---|---|
| UI 名称 | CPU |
| 数据来源 | FreeRTOS runtime stats |
| 当前实现 | `ethernet_app.c::calculate_cpu_usage_percent()` |
| 展示形式 | 百分比 / 圆环 |
| 刷新周期 | 1 秒 |

实现注意：

- 当前函数是 `static`，仅供 Ethernet metrics 使用。
- 后续应抽成共享系统指标快照。
- 不要在 LVGL 锁内调用 `uxTaskGetSystemState()`。

### 4.9 内存 / PSRAM 使用率

| 项目 | 内容 |
|---|---|
| UI 名称 | MEM / PSRAM |
| 推荐来源 | `heap_caps_get_total_size/free_size(MALLOC_CAP_SPIRAM)` |
| 展示形式 | 百分比 + free MB |
| 刷新周期 | 1 秒 |

推荐口径：

```text
PSRAM used % = 100 * (total_psram - free_psram) / total_psram
```

说明：

本项目图像帧、AI 输入、预览缓冲大量依赖 PSRAM，因此首屏 memory 建议优先展示 PSRAM 使用情况，而不是混合 heap。

### 4.10 Edge Online 状态

| 项目 | 内容 |
|---|---|
| UI 名称 | Edge Online |
| 数据来源 | Ethernet link/IP/TCP 状态 |
| 当前状态 | 多数只在日志中体现 |
| 展示形式 | Online / Offline / IP Ready / Link Down |

推荐状态分层：

| 状态 | 含义 |
|---|---|
| `Link Down` | 网线或 PHY 未连接 |
| `IP Ready` | 板端已有 IP，但未确认上位机连接 |
| `Host Connected` | TCP 已连接上位机 |
| `Online` | TCP 连接可用且最近发送成功 |

注意：

不能只用 Ethernet link up 判断 `Online`。

---

## 5. 工程诊断页推荐展示参数

### 5.1 AI pipeline 耗时拆解

| 参数 | 来源 | 单位 |
|---|---|---|
| 摄像头等待 | `cam_wait_us` | ms |
| ROI 检测 | `roi_detect_us` | ms |
| 预处理总耗时 | `preprocess_us` | ms |
| PPA resize | `ppa_resize_us` | ms |
| 量化 | `quantize_us` | ms |
| input copy | `input_copy_us` | ms |
| 模型 run | `run_us` | ms |
| decode | `decode_us` | ms |
| preview 渲染 | `preview_us` | ms |
| loop 总耗时 | `loop_us` | ms |
| 最大 run 耗时 | `max_run_us` | ms |
| 最大 loop 耗时 | `max_loop_us` | ms |

推荐显示形式：

```text
AI Pipeline
Camera wait     5 ms
ROI detect      3 ms
Preprocess      8 ms
Inference       72 ms
Decode          2 ms
Preview         12 ms
Total loop      103 ms
```

### 5.2 ROI 诊断

| 参数 | 来源 | 说明 |
|---|---|---|
| ROI success | `roi_success` | 是否找到包裹区域 |
| ROI skipped | `roi_skipped` | 是否跳过 ROI |
| ROI fail reason | `roi_fail_reason` | 失败原因 |
| ROI x/y/w/h | `roi_x/y/w/h` | ROI 坐标和尺寸 |

ROI 失败原因适合工程页，不建议首屏展示。

### 5.3 类别 raw score 诊断

| 参数 | 来源 | 说明 |
|---|---|---|
| 极兔 raw score | `class_prob[0]` | 当前帧 raw score |
| 中通 raw score | `class_prob[1]` | 当前帧 raw score |
| 韵达 raw score | `class_prob[2]` | 当前帧 raw score |
| avg score | `class_prob_avg[]` | 短窗口平均 |
| max score | `class_prob_max[]` | 短窗口最大 |

说明：

raw score 仅用于调试模型输出稳定性，不应作为首屏最终置信度。

### 5.4 系统内存诊断

| 参数 | 来源 |
|---|---|
| Free heap | `esp_get_free_heap_size()` |
| Min free heap | `esp_get_minimum_free_heap_size()` |
| Free internal | `heap_caps_get_free_size(MALLOC_CAP_INTERNAL)` |
| Free PSRAM | `heap_caps_get_free_size(MALLOC_CAP_SPIRAM)` |
| Total PSRAM | `heap_caps_get_total_size(MALLOC_CAP_SPIRAM)` |
| Largest free block | `heap_caps_get_largest_free_block(MALLOC_CAP_DEFAULT)` |

### 5.5 FreeRTOS 任务诊断

| 参数 | 来源 |
|---|---|
| 任务名 | `TaskStatus_t::pcTaskName` |
| 优先级 | `TaskStatus_t::uxCurrentPriority` |
| CPU 占比 | `TaskStatus_t::ulRunTimeCounter` 差分 |
| 栈水位 | `TaskStatus_t::usStackHighWaterMark` |
| 当前核心 | `xPortGetCoreID()` |

建议重点关注：

- `vision_task`
- `cam_cap`
- `sys_mon`
- LVGL task
- TCP task
- IDLE task

### 5.6 PSRAM 带宽诊断

| 参数 | 来源 | 说明 |
|---|---|---|
| PSRAM 总带宽 | `s_rpt_total_bw` | 估算值 |
| CPU PSRAM 带宽 | L2 cache miss 差分 | 估算值 |
| LCD DMA 带宽 | 固定公式 | 1024x600x3x60 |
| Camera DMA 带宽 | 固定公式 | 1024x600x3x30 |
| 每任务 miss | `s_rpt_tasks[].misses` | 工程诊断 |
| 每任务 KB/s | `s_rpt_tasks[].bw` | 工程诊断 |

说明：

PSRAM 带宽为估算值，不应作为精密测量值。

### 5.7 摄像头诊断

| 参数 | 来源 |
|---|---|
| 摄像头型号 | camera BSP 初始化 |
| 分辨率 | `frame_width / frame_height` |
| 帧格式 | RGB888 |
| 帧大小 | `frame_size` |
| 帧缓冲数量 | `CONFIG_CAM_SENSOR_FB_COUNT` |
| capture timeout | `timeout_count` |
| receive 最大耗时 | `max_receive_us` |
| capture task 栈水位 | `uxTaskGetStackHighWaterMark()` |

### 5.8 网络诊断

| 参数 | 来源 |
|---|---|
| Link 状态 | Ethernet event |
| IP 地址 | IP event |
| MAC 地址 | Ethernet ioctl |
| TCP host | `192.168.10.1:5000` |
| TCP connected | `connect_to_host()` 结果 |
| metrics seq | `s_tx_seq` |
| 发送失败次数 | 需新增统计 |
| 上次发送时间 | 需新增统计 |

---

## 6. 当前不建议展示或不建议首屏展示的参数

### 6.1 不建议首屏展示，但可放工程页

| 参数 | 原因 |
|---|---|
| ROI fail reason | 操作者难以理解，适合工程定位 |
| ROI 坐标数值 | 对业务结果无直接意义 |
| PPA resize / quantize / input copy 耗时 | AI 内部细节，适合工程页 |
| runtime multi-core | 推理策略，不是业务状态 |
| fallback_triggered | 可作为告警，不适合作为主指标 |
| class_prob_avg/max | raw score，不等于最终识别置信度 |
| PSRAM per-task bandwidth | 工程性能指标 |
| 每任务栈水位 | 仅工程诊断有意义 |

### 6.2 当前不建议展示为真实值

| 参数 | 当前问题 | 建议 |
|---|---|---|
| 设备温度 | 当前工程未见可靠温度读取逻辑 | 暂不显示或显示 `N/A` |
| 电机 RPM | encoder BSP 提供 `cm/s`，不是 RPM | 改名为传送速度，或新增 RPM 换算 |
| 累计分拣件数 | 当前没有业务事件去重统计 | 新增事件状态机后再显示 |
| 成功率 | 当前没有明确业务分母 | 新增事件统计后再显示 |
| 网络带宽 | 当前没有收发字节速率统计 | 新增统计后再显示 |
| 真实包裹尺寸 | bbox 不是物理尺寸 | 不作为真实尺寸展示 |

---

## 7. 需要新增统计逻辑的参数

### 7.1 识别事件状态机

当前 `detections` 是帧级结果，一件包裹会在连续多帧中重复出现。

不能这样统计：

```text
累计件数 += detections.size()
```

推荐新增件级事件状态机：

1. 连续 N 帧检测到同类且置信度超过阈值，进入 tracking。
2. 稳定达到 M 帧后确认一次包裹识别事件。
3. 目标消失 K 帧后结束事件，允许下一件包裹计数。
4. 每个事件记录：
   - 时间戳
   - 类别
   - 最高置信度
   - 平均置信度
   - AI 耗时
   - 状态：成功 / 低置信度 / 异常

用于生成：

- 累计识别件数
- 成功识别件数
- 低置信度次数
- 最近事件日志
- 业务成功率

### 7.2 成功率统计

成功率必须明确分母和分子。

推荐定义：

```text
业务成功率 = confirmed_success_events / total_package_events
```

工程页可额外显示：

```text
ROI 成功率 = roi_success_frames / total_frames
检测帧命中率 = detection_frames / roi_success_frames
```

### 7.3 CPU / MEM 快照 API

当前 CPU 使用率计算在 `ethernet_app.c` 内部，`system_monitor.c` 主要通过 RTT 打印。

建议后续新增统一快照：

```c
typedef struct {
    uint32_t cpu_usage_pct;
    uint32_t free_heap;
    uint32_t min_free_heap;
    uint32_t free_internal;
    uint32_t free_psram;
    uint32_t total_psram;
    uint32_t largest_free_block;
    uint32_t psram_used_pct;
} system_metrics_snapshot_t;
```

UI 只读取快照，不在 LVGL 锁内计算。

### 7.4 Ethernet 状态快照

建议新增：

```c
typedef struct {
    bool link_up;
    bool ip_ready;
    bool host_connected;
    uint32_t tx_seq;
    uint32_t send_fail_count;
    int64_t last_tx_ms;
    char ip[16];
    char mac[18];
} ethernet_status_snapshot_t;
```

用于首屏 Online 状态和工程网络诊断。

### 7.5 传送速度 / RPM 口径

当前 encoder BSP 更适合提供传送速度：

```text
speed_cm_s
```

如果 UI 必须显示 RPM，需要补充：

- 编码器线数
- 减速比
- 轮径
- count 到 RPM 的换算公式

在此之前，不建议显示 RPM。

### 7.6 温度数据

当前工程未见可靠温度来源。

如后续需要展示，需要先确定：

- ESP32-P4 内部温度是否适用
- 是否有外部温度传感器
- 是否需要校准
- 告警阈值如何定义

---

## 8. UI 字段映射建议

### 8.1 可以直接沿用的字段

| 当前字段 | 推荐真实含义 |
|---|---|
| 检测结果 | 当前识别状态 |
| 实时帧率 | AI FPS |
| 推理耗时 | 若使用 `run_us`，保持“推理耗时” |
| 当前识别类型 | 当前最高置信度 detection 的类别 |
| CONFIDENCE | 当前最高 detection confidence |
| CPU | CPU busy percent |
| MEMORY | PSRAM used percent 或 memory health |

### 8.2 建议改名的字段

| 当前字段 | 建议改名 | 原因 |
|---|---|---|
| 推理耗时 | AI 耗时 | 如果展示 `total_us`，语义更准确 |
| 电机转速 | 传送速度 | 当前有 cm/s，不是 RPM |
| 成功率 | 当前置信度 | 当前没有业务成功率统计闭环 |
| 设备温度 | 系统状态 / 预留 | 当前没有温度数据源 |

### 8.3 建议隐藏或移到工程页的字段

| 字段 | 原因 |
|---|---|
| ROI 框 | 默认首屏可能干扰业务预览，可放工程模式开关 |
| 各类别 avg/max score | 工程调试数据 |
| PSRAM bandwidth | 工程性能数据 |
| task stack / per-task CPU | 工程诊断数据 |

---

## 9. 指标刷新周期建议

| 指标 | 推荐刷新周期 | 说明 |
|---|---|---|
| 摄像头预览 | 每帧 | 当前已有 |
| 检测框 | 每帧 | 当前已有 |
| 识别状态 | 每帧或状态变化时 | 可加 0.5~1 秒保持，避免闪烁 |
| 当前类别 | 每帧或状态稳定后 | 建议结合事件状态机 |
| 当前置信度 | 每帧或 200ms | 可平滑显示 |
| AI FPS | 1 秒 | 当前已有 1 秒窗口 |
| AI 耗时 | 每帧或 1 秒平均 | 首屏建议显示平滑值 |
| CPU 使用率 | 1 秒 | 避免频繁调用任务统计 |
| 内存 / PSRAM | 1 秒 | 开销低 |
| Ethernet 状态 | 事件驱动 + 1 秒兜底 | link/IP/TCP 状态变化时更新 |
| 工程诊断表 | 1~2 秒 | 避免影响 UI |

实现约束：

- LVGL 对象更新必须加锁。
- 系统指标采集不要放在 LVGL 锁内。
- 任务统计不要每帧调用。

---

## 10. 推荐实施顺序

### 第一阶段：接入现有可靠指标

目标：先把 UI 占位值替换成真实数据。

建议接入：

1. 当前识别类型
2. 当前 detection confidence
3. AI FPS
4. AI 耗时
5. CPU %
6. PSRAM used %
7. Edge Online 状态

### 第二阶段：新增业务事件统计

目标：让累计件数、成功率、事件日志变成真实业务指标。

建议新增：

1. 包裹事件状态机
2. 稳定识别阈值
3. 事件环形缓存
4. 成功 / 低置信 / 异常统计
5. 最近 6 条事件记录

### 第三阶段：工程诊断页

目标：把已有性能统计结构化展示。

建议接入：

1. AI pipeline 耗时表
2. ROI 状态与 fail reason
3. 类别 raw score
4. heap / PSRAM / largest block
5. 任务 CPU / 栈水位
6. PSRAM bandwidth
7. 摄像头 timeout / receive 耗时
8. Ethernet 连接与发送状态

### 第四阶段：补齐硬件指标

目标：只有在硬件来源和口径明确后再展示。

包括：

1. 温度
2. RPM
3. 网络带宽
4. 真实包裹尺寸

---

## 11. 风险与验证清单

### 11.1 数据口径风险

| 风险 | 避免方式 |
|---|---|
| 把帧级 detection 当作件级计数 | 新增事件状态机 |
| 把 raw score 当作最终置信度 | 首屏使用 `Detection::confidence` |
| 成功率分母不清 | 明确事件级分母 |
| MEM 百分比含义不清 | 标明 PSRAM / internal / heap |
| Online 语义过度乐观 | 区分 link、IP、TCP、recent TX |

### 11.2 性能开销风险

| 风险 | 避免方式 |
|---|---|
| 每帧调用 `uxTaskGetSystemState()` | 1 秒采样并缓存 |
| LVGL 锁内做复杂计算 | 锁外采集，锁内只更新对象 |
| 工程页刷新过快 | 1~2 秒刷新 |
| 日志过多影响性能 | 工程模式开关或限频 |

### 11.3 UI 线程安全风险

- LVGL 对象必须在持有 LVGL lock 时更新。
- 系统指标采集不应依赖 UI 锁。
- 网络、视觉、系统监控任务之间应通过快照或队列传递数据。

### 11.4 验证清单

实施 UI 绑定后建议验证：

1. 无目标时：类型显示 `--`，置信度不误报。
2. 有目标时：类型、置信度、检测框一致。
3. 低置信度时：状态颜色变为黄色或红色。
4. 摄像头断流时：状态能显示异常。
5. Ethernet 断开时：Online 状态能正确下降。
6. TCP host 未连接时：不能显示完全 Online。
7. CPU/MEM 刷新不会造成 UI 卡顿。
8. 累计件数不会按帧重复增加。
9. 工程诊断页刷新不会明显降低 AI FPS。
10. RTT 日志与 UI 指标趋势一致。

---

## 12. 最终推荐首屏参数

最终建议首屏只保留以下核心参数：

```text
1. 摄像头实时画面
2. 检测框
3. 当前识别状态
4. 当前快递类型
5. 当前最终检测置信度
6. AI FPS
7. AI 耗时 ms
8. CPU 使用率
9. PSRAM / 内存使用率
10. Edge Online 状态
```

在未实现事件状态机之前，以下参数不要作为真实业务值展示：

```text
1. 累计分拣件数
2. 业务成功率
3. 低置信事件次数
4. 最近事件日志
```

这些应在第二阶段业务事件统计完成后再接入 Dashboard。
