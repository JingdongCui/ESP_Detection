# 以太网 + 分拣系统融合改动说明

本文档记录本次从交接工程 `E:\two_stage_host_handoff_20260705\new_merge_motor_two_stage` 融合到当前视觉工程 `E:\ESP32P4_Detection` 的新增内容、原视觉/UI 链路中的接入点、硬件映射、协议链路和验证状态。

## 1. 融合目标

当前工程原本以视觉检测、UI 展示、相机采集、PPA 预览和模型推理为主；交接工程在此基础上增加了以太网、分拣调度、电机、传感器和上位机通信。本次融合目标是把两条开发线合并：

- 保留当前工程已经完成的视觉二阶段检测、UI 阈值调节、检测开关、预览 overlay 开关和显示链路。
- 引入交接工程的 `Sorter_app` 分拣调度核心。
- 引入分拣硬件 BSP，包括电机、传感器、编码器配置。
- 替换/升级 `Ethernet_app`，启用双 TCP 链路：
  - 控制链路：`192.168.10.1:5000`
  - JPEG 图像链路：`192.168.10.1:5001`
- 让真实视觉识别结果提交给 sorter 调度逻辑。
- 让网络图像包携带最新视觉分类快照。

除公司类别映射外，分拣和以太网逻辑以交接工程为准。

## 2. 本次新增文件

### 2.1 新增 `Sorter_app` 组件

新增目录：`components/Sorter_app/`

| 文件 | 作用 |
| --- | --- |
| `components/Sorter_app/CMakeLists.txt` | 注册 sorter 组件，依赖 `bsp`、`freertos`、`esp_timer`、`esp_driver_usb_serial_jtag` 等。 |
| `components/Sorter_app/sorting_sim_control.c` | 分拣调度控制桥接层；处理 TCP/USB 命令、真实传感器输入、视觉分类提交、电机输出、硬件状态查询。 |
| `components/Sorter_app/include/sorting_sim_control.h` | sorter 控制层对外接口。 |
| `components/Sorter_app/sorter_core/sorter_scheduler.c` | 分拣调度核心状态机。 |
| `components/Sorter_app/sorter_core/sorter_protocol.c` | sorter 事件到文本协议的格式化。 |
| `components/Sorter_app/include/sorter_core/sorter_scheduler.h` | 分拣状态机类型、配置、事件和调度接口。 |
| `components/Sorter_app/include/sorter_core/sorter_protocol.h` | sorter 协议格式化接口。 |

关键对外接口：

```c
void sorting_sim_control_reset(void);
void sorting_sim_control_handle_line(const char *line, size_t len,
                                     sorting_sim_send_fn_t send_fn,
                                     void *send_ctx);
void sorting_sim_control_tick(sorting_sim_send_fn_t send_fn, void *send_ctx);
void sorting_sim_debug_start(void);
void sorting_sim_control_get_settings(sorting_debug_settings_t *settings);
void sorting_sim_control_apply_settings(const sorting_debug_settings_t *settings);
void sorting_sim_control_set_mode(sorting_debug_mode_t mode);
void sorting_sim_control_set_motor_output_enabled(bool enabled);
void sorting_sim_control_set_sensor_input_enabled(bool enabled);
void sorting_sim_control_submit_vision_class(sorter_package_class_t cls,
                                             float confidence);
void sorting_sim_control_simulate_class(sorter_package_class_t cls,
                                        sorting_sim_send_fn_t send_fn,
                                        void *send_ctx);
void sorting_sim_control_get_hardware_status(sorting_hardware_status_t *status);
void sorting_sim_control_get_runtime_debug(sorting_runtime_debug_t *status);
void sorting_sim_control_start_motor_test(void);
void sorting_sim_control_clear_encoder_distance(int index);
```

核心类别枚举来自 `sorter_scheduler.h`：

```c
typedef enum {
    SORTER_CLASS_UNKNOWN = 0,
    SORTER_CLASS_1,
    SORTER_CLASS_2,
    SORTER_CLASS_3,
    SORTER_CLASS_FREE,
    SORTER_CLASS_ERROR,
    SORTER_CLASS_VISION_FAILED,
} sorter_package_class_t;
```

### 2.2 新增分拣传感器 BSP 和统一硬件配置

新增文件：

| 文件 | 作用 |
| --- | --- |
| `components/bsp/bsp_sort_sensor.c` | S1/S2/S3/S4 分拣传感器 GPIO 初始化和读取。 |
| `components/bsp/include/bsp_sort_sensor.h` | 分拣传感器 BSP 对外接口。 |
| `components/bsp/include/sorter_debug_config.h` | 分拣电机、传感器、编码器 GPIO 的统一配置源。 |

传感器接口：

```c
typedef enum {
    BSP_SORT_SENSOR_S1 = 1,
    BSP_SORT_SENSOR_S2 = 2,
    BSP_SORT_SENSOR_S3 = 3,
    BSP_SORT_SENSOR_S4 = 4,
} bsp_sort_sensor_id_t;

esp_err_t bsp_sort_sensor_init(void);
esp_err_t bsp_sort_sensor_get_level(bsp_sort_sensor_id_t sensor_id,
                                    int *level,
                                    bool *active);
esp_err_t bsp_sort_sensor_get_state(bsp_sort_sensor_id_t sensor_id,
                                    bool *active);
```

### 2.3 新增模型文件

当前工作区新增：

| 文件 | 作用 |
| --- | --- |
| `model/waybill_try.espdl` | 当前 `vision_model.cpp` 指向的面单检测模型文件。 |

对应代码变更在 `components/vision/detector/vision_model.cpp`：

```c
#define VISION_WAYBILL_MODEL_FILE  "waybill_try.espdl"
```

## 3. 本次修改/融合文件总览

| 文件 | 修改类型 | 说明 |
| --- | --- | --- |
| `agentic/board.md` | 修改 | 更新融合后的板级硬件描述，明确 GPIO53 是 S1，补充分拣电机、传感器、编码器和以太网接口。 |
| `components/Ethernet_app/CMakeLists.txt` | 修改 | 增加 `Sorter_app`、`system_monitor`、`vision`、`esp_new_jpeg` 等依赖。 |
| `components/Ethernet_app/ethernet_app.c` | 替换/升级 | 使用交接工程双 TCP 链路版本，接入 sorter 命令、metrics、JPEG snapshot。 |
| `components/bsp/CMakeLists.txt` | 修改 | 加入 `bsp_sort_sensor.c`。 |
| `components/bsp/bsp_encoder.c` | 替换/融合 | 使用交接版编码器逻辑，编码器 GPIO 全部按 `sorter_debug_config.h` 为 `-1`。 |
| `components/bsp/bsp_motor.c` | 替换/融合 | 使用交接版 MCPWM 电机输出逻辑，GPIO 来自 `sorter_debug_config.h`。 |
| `components/bsp/include/bsp_motor.h` | 修改 | 增加 `bsp_motor_set_output_enabled(bool enabled)` 等交接版接口。 |
| `components/vision/CMakeLists.txt` | 修改 | 增加 `Sorter_app` 依赖，使视觉检测任务可 include `sorting_sim_control.h`。 |
| `components/vision/detector/vision_model.cpp` | 修改 | 面单模型文件名改为 `waybill_try.espdl`。 |
| `components/vision/framework/vision.h` | 修改 | 保留原检测/overlay 接口，新增 classification 和 snapshot API。 |
| `components/vision/framework/vision_app.c` | 修改 | 新增独立 snapshot PPA client 和最新帧缩放复制接口。 |
| `components/vision/framework/vision_detect.c` | 修改 | 真实 logo 识别成功后提交 sorter，加入公司类别映射。 |
| `components/vision/framework/vision_draw.c` | 修改 | 维护最近一次分类快照，供以太网 JPEG header 使用。 |
| `components/vision/framework/vision_internal.h` | 修改 | 声明 draw 层分类快照读取接口。 |
| `main/CMakeLists.txt` | 修改 | 增加 `Sorter_app` 依赖。 |
| `main/system_init.c` | 修改 | 合并启动链路：日志过滤、sorter 启动、电机/传感器使能、以太网启动。 |
| `docs/superpowers/specs/2026-07-05-ethernet-sorter-fusion-design.md` | 新增 | 本次融合设计规格。 |

另外工作区中 `.claude/settings.local.json` 也有本地配置变更，它不是固件融合逻辑的一部分。

## 4. 硬件映射

硬件映射统一放在：`components/bsp/include/sorter_debug_config.h`。

### 4.1 电机 GPIO

| 电机 | PWM A | PWM B | 说明 |
| --- | --- | --- | --- |
| Motor0 / A 段 | GPIO2 | GPIO3 | MCPWM 双路方向控制。 |
| Motor1 / B 段 | GPIO32 | GPIO36 | MCPWM 双路方向控制。 |
| Motor2 / C 段 | GPIO4 | GPIO5 | MCPWM 双路方向控制。 |

对应宏：

```c
#define SORTER_MOTOR_COUNT 3
#define SORTER_MOTOR1_PWM_A_GPIO 32
#define SORTER_MOTOR1_PWM_B_GPIO 36
#define SORTER_MOTOR0_PWM_A_GPIO 2
#define SORTER_MOTOR0_PWM_B_GPIO 3
#define SORTER_MOTOR2_PWM_A_GPIO 4
#define SORTER_MOTOR2_PWM_B_GPIO 5

#define SORTER_MOTOR0_REVERSE 0
#define SORTER_MOTOR1_REVERSE 0
#define SORTER_MOTOR2_REVERSE 0
```

### 4.2 分拣传感器 GPIO

| 传感器 | GPIO | 有效电平 | 当前状态/用途 |
| --- | --- | --- | --- |
| S1 | GPIO53 | 高电平 | 新包裹入口 / 打开视觉窗口。GPIO53 不再作为音频 PA_EN 使用。 |
| S2 | GPIO23 | 高电平 | A 到 B 交接。 |
| S3 | -1 | 高电平 | 当前未接，按交接工程配置保留为 disabled。 |
| S4 | GPIO22 | 高电平 | B 到 C 交接。 |

对应宏：

```c
#define SORTER_SENSOR_S1_GPIO 53
#define SORTER_SENSOR_S2_GPIO 23
#define SORTER_SENSOR_S3_GPIO -1
#define SORTER_SENSOR_S4_GPIO 22

#define SORTER_SENSOR_S1_ACTIVE_LEVEL 1
#define SORTER_SENSOR_S2_ACTIVE_LEVEL 1
#define SORTER_SENSOR_S3_ACTIVE_LEVEL 1
#define SORTER_SENSOR_S4_ACTIVE_LEVEL 1
```

### 4.3 编码器 GPIO

当前编码器全部未接，按交接工程配置为虚拟/disabled：

```c
#define SORTER_ENCODER_COUNT 3
#define SORTER_ENCODER0_GPIO_A -1
#define SORTER_ENCODER0_GPIO_B -1
#define SORTER_ENCODER1_GPIO_A -1
#define SORTER_ENCODER1_GPIO_B -1
#define SORTER_ENCODER2_GPIO_A -1
#define SORTER_ENCODER2_GPIO_B -1
```

## 5. 公司类别映射

这是本次融合中唯一不完全照搬交接工程的边界，按用户确认执行。

### 5.1 视觉模型 category

视觉模型输出 category：

| category | 公司 |
| --- | --- |
| 0 | 极兔 |
| 1 | 韵达 |
| 2 | 中通 |

UI 文本保留：

```c
static const char *kClassName[3] = {"极兔", "韵达", "中通"};
```

### 5.2 sorter class 语义

分拣系统 class 语义：

| sorter class | 公司 |
| --- | --- |
| `SORTER_CLASS_1` | 极兔 |
| `SORTER_CLASS_2` | 中通 |
| `SORTER_CLASS_3` | 韵达 |

### 5.3 视觉到 sorter 的最终映射

实现位置：`components/vision/framework/vision_detect.c`

```c
static sorter_package_class_t sorter_class_from_logo_category(int category)
{
    switch (category) {
    case 0:
        return SORTER_CLASS_1;
    case 1:
        return SORTER_CLASS_3;
    case 2:
        return SORTER_CLASS_2;
    default:
        return SORTER_CLASS_UNKNOWN;
    }
}
```

最终映射：

| 视觉 category | UI 公司名 | sorter class |
| --- | --- | --- |
| 0 | 极兔 | `SORTER_CLASS_1` |
| 1 | 韵达 | `SORTER_CLASS_3` |
| 2 | 中通 | `SORTER_CLASS_2` |

## 6. 原视觉链路中的融合内容

### 6.1 `vision.h` 新增网络快照与分类 API

文件：`components/vision/framework/vision.h`

保留原有 UI/视觉控制接口：

```c
void vision_set_detection_enabled(bool enabled);
bool vision_is_detection_enabled(void);
void vision_set_preview_overlay_enabled(bool enabled);
bool vision_is_preview_overlay_enabled(void);
```

新增分类快照类型和接口：

```c
typedef struct {
    int class_id;
    int confidence_pct;
    bool valid;
} vision_classification_t;

bool vision_get_latest_classification(vision_classification_t *out);
```

新增网络图像链路使用的帧缩放复制接口：

```c
esp_err_t vision_copy_latest_frame_scaled_rgb888(uint8_t *dst,
                                                 int dst_w,
                                                 int dst_h,
                                                 size_t dst_capacity,
                                                 int *src_w,
                                                 int *src_h,
                                                 size_t *out_len,
                                                 int64_t *timestamp_us);
```

### 6.2 `vision_app.c` 新增 snapshot PPA client

文件：`components/vision/framework/vision_app.c`

新增静态 PPA client：

```c
static ppa_client_handle_t s_snapshot_ppa;
```

在 `vision_start()` 内，原显示 PPA client `s_ppa` 注册成功后，额外注册 `s_snapshot_ppa`：

```c
ppa_client_config_t ppa_cfg = { .oper_type = PPA_OPERATION_SRM };
esp_err_t ret = ppa_register_client(&ppa_cfg, &s_ppa);
...
ret = ppa_register_client(&ppa_cfg, &s_snapshot_ppa);
```

这样 JPEG snapshot 不复用显示任务的 PPA client，避免和 LCD 预览缩放路径混用。

### 6.3 `vision_app.c` 新增最新帧缩放复制接口

实现：`vision_copy_latest_frame_scaled_rgb888()`

功能：

1. 从视觉零拷贝 ringbuf 中取最新帧引用。
2. 使用 `s_snapshot_ppa` 将原始 RGB888 帧缩放到调用方提供的 `dst_w × dst_h` RGB888 buffer。
3. 填出原图宽高、输出长度、时间戳。
4. PPA 写完后对目标 buffer 做 cache sync。

调用者：`components/Ethernet_app/ethernet_app.c`

用途：为 5001 图像 TCP 链路提供 JPEG 编码前的缩放 RGB 图。

### 6.4 `vision_draw.c` 新增最近分类快照

文件：`components/vision/framework/vision_draw.c`

新增状态：

```c
static vision_classification_t s_latest_classification;
```

每次 `vision_draw_save_result()` 保存检测结果时，同步更新分类快照：

```c
s_latest_classification = classification_from_event(&frame->ev);
```

新增转换逻辑：

```c
if (strcmp(ev->company, "中通") == 0) {
    cls.class_id = 2;
    cls.valid = true;
} else if (strcmp(ev->company, "韵达") == 0) {
    cls.class_id = 3;
    cls.valid = true;
} else if (strcmp(ev->company, "极兔") == 0) {
    cls.class_id = 1;
    cls.valid = true;
}
```

这里的 `class_id` 是上位机/JPEG header 使用的 class id：

| 公司 | JPEG / 上位机 class_id |
| --- | --- |
| 极兔 | 1 |
| 中通 | 2 |
| 韵达 | 3 |

新增读取接口：

```c
bool vision_draw_get_latest_classification(vision_classification_t *out);
```

### 6.5 `vision_internal.h` 新增 draw 分类快照声明

文件：`components/vision/framework/vision_internal.h`

新增：

```c
#include "vision.h"

bool vision_draw_get_latest_classification(vision_classification_t *out);
```

### 6.6 `vision_detect.c` 接入 sorter 提交

文件：`components/vision/framework/vision_detect.c`

新增 include：

```c
#include "sorting_sim_control.h"
```

在本帧真实 logo 成功分支中，保留 UI 公司名写入，同时提交 sorter：

```c
int cat = result.items[best_logo].category;
if (cat >= 0 && cat < 3) {
    strcpy(result.ev.company, kClassName[cat]);
    sorting_sim_control_submit_vision_class(
        sorter_class_from_logo_category(cat), result.items[best_logo].score);
} else {
    strcpy(result.ev.company, "--");
}
```

注意：提交发生在 miss-keep 保持逻辑之前，基于当前帧真实 `best_logo`，不会因为显示保持帧而重复提交旧分类。

### 6.7 `components/vision/CMakeLists.txt` 增加 sorter 依赖

文件：`components/vision/CMakeLists.txt`

`REQUIRES` 中新增：

```cmake
Sorter_app
```

原因：`vision_detect.c` 直接 include `sorting_sim_control.h` 并调用 sorter 提交接口。

## 7. UI 链路中的保留与融合

文件：`main/system_init.c`

本次没有覆盖当前 UI 绑定逻辑，而是保留当前视觉工程完整 dashboard handler。

保留内容：

```c
ui_bind_dashboard(&(ui_dashboard_handlers_t){
    .brightness = BSP_LCD_SetBrightness,
    .calibration = vision_frame_dump_request,
    .detection_enabled = vision_set_detection_enabled,
    .preview_overlay_enabled = vision_set_preview_overlay_enabled,
    .waybill_score_threshold_get = vision_model_get_waybill_score_threshold_percent,
    .waybill_score_threshold_set = vision_model_set_waybill_score_threshold_percent,
    .logo_score_threshold_get = vision_model_get_logo_score_threshold_percent,
    .logo_score_threshold_set = vision_model_set_logo_score_threshold_percent,
});
```

也就是说，以下现有 UI 功能没有被交接工程简化版覆盖：

- 背光调节。
- LOGO 键触发抓帧/校准入口。
- 检测开关。
- 预览 overlay 开关。
- 面单分数阈值读取/设置。
- LOGO 分数阈值读取/设置。

## 8. `System_Init` 启动链路融合

文件：`main/system_init.c`

### 8.1 新增 include

```c
#include "esp_err.h"
#include "sorting_sim_control.h"
```

### 8.2 新增日志过滤配置

新增宏：

```c
#define SORTER_HARDWARE_DEBUG_MONITOR 1
#ifndef SORTER_TCP_LINK_ENABLE
#define SORTER_TCP_LINK_ENABLE 0
#endif
```

新增函数：

```c
static void configure_monitor_logging(void)
```

作用：降低默认日志噪声，只保留融合调试重点 tag：

- `system`
- `sorting_sim`
- `bsp_motor`
- `bsp_sort_sensor`
- `bsp_encoder`
- `vision`
- `vision_model`
- `ISP_AWB`

### 8.3 初始化顺序

当前启动顺序：

1. `configure_monitor_logging()`
2. `BSP_LCD_Init()`
3. `BSP_Touch_Init()`
4. `cam_sensor_init()`
5. `BSP_LVGL_AdapterInit()`
6. LVGL lock 内：
   - `setupUi()`
   - `ui_bind_dashboard(...)`
7. `vision_start()`
8. sorter 自动启动和真实 IO 使能当前被注释，作为硬件安全默认：
   - `//sorting_sim_debug_start()`
   - `//sorting_sim_control_set_motor_output_enabled(true)`
   - `//sorting_sim_control_set_sensor_input_enabled(true)`
9. `system_monitor()`
10. `ethernet_app_start()` 受 `SORTER_TCP_LINK_ENABLE` 控制；当前默认 `0`，即上位机 TCP 链路关闭。

对应新增片段：

```c
vision_start();

//sorting_sim_debug_start();
//sorting_sim_control_set_motor_output_enabled(true);
//sorting_sim_control_set_sensor_input_enabled(true);

system_monitor();

#if SORTER_TCP_LINK_ENABLE
esp_err_t eth_ret = ethernet_app_start();
if (eth_ret != ESP_OK) {
    ESP_LOGW(TAG, "Ethernet sorter link start failed: %s", esp_err_to_name(eth_ret));
}
#elif SORTER_HARDWARE_DEBUG_MONITOR
ESP_LOGI(TAG, "硬件分拣调试: 上位机TCP链路已关闭");
#endif
```

### 8.4 main 组件依赖

文件：`main/CMakeLists.txt`

新增 `Sorter_app`：

```cmake
idf_component_register(SRCS "main.c" "system_init.c"
                    INCLUDE_DIRS "."
                    REQUIRES bsp UI Ethernet_app Sorter_app system_monitor vision)
```

原因：`system_init.c` 直接 include `sorting_sim_control.h`。

## 9. 以太网链路融合

文件：`components/Ethernet_app/ethernet_app.c`

### 9.1 网络常量

```c
#define HOST_IP                    "192.168.10.1"
#define HOST_CONTROL_PORT          5000
#define HOST_IMAGE_PORT            5001
#define ESP_HOST_TYPE_SIM_LINE     0x12u
```

含义：

| 常量 | 值 | 说明 |
| --- | --- | --- |
| `HOST_IP` | `192.168.10.1` | 上位机 IP。 |
| `HOST_CONTROL_PORT` | `5000` | 控制 TCP 链路。 |
| `HOST_IMAGE_PORT` | `5001` | JPEG 图像 TCP 链路。 |
| `ESP_HOST_TYPE_SIM_LINE` | `0x12` | sorter 文本命令/状态包类型。 |

### 9.2 控制链路 5000

控制链路职责：

- 连接上位机 `192.168.10.1:5000`。
- 接收 `ESP_HOST_TYPE_SIM_LINE` 包。
- 将 payload 交给：

```c
sorting_sim_control_handle_line((const char *)payload,
                                header->payload_len,
                                send_sim_line_packet,
                                (void *)(intptr_t)sock);
```

- sorter 输出的状态/事件通过 `send_sim_line_packet()` 发回上位机。
- 处理 time sync 包。
- 发送 metrics 包，metrics 来源优先使用：

```c
system_monitor_get_metrics(&m)
```

### 9.3 图像链路 5001

图像链路职责：

- 连接上位机 `192.168.10.1:5001`。
- 从视觉链路取最新帧：

```c
vision_copy_latest_frame_scaled_rgb888(...)
```

- 编码 JPEG。
- 读取最新分类快照：

```c
vision_get_latest_classification(&cls)
```

- 将 JPEG 与分类/置信度等 header 信息发给上位机。

### 9.4 Ethernet_app 组件依赖

文件：`components/Ethernet_app/CMakeLists.txt`

```cmake
idf_component_register(
    SRCS "ethernet_app.c"
    INCLUDE_DIRS "include"
    REQUIRES bsp Sorter_app system_monitor vision esp_eth esp_netif esp_event log freertos esp_timer esp_new_jpeg
)
```

新增依赖原因：

| 依赖 | 原因 |
| --- | --- |
| `Sorter_app` | 控制链路转发 sorter 命令/状态。 |
| `system_monitor` | metrics 数据来源。 |
| `vision` | JPEG snapshot 和分类快照。 |
| `esp_new_jpeg` | RGB888 编码为 JPEG。 |

## 10. BSP 融合

### 10.1 `components/bsp/CMakeLists.txt`

新增源文件：

```cmake
"bsp_sort_sensor.c"
```

### 10.2 电机 BSP

文件：

- `components/bsp/bsp_motor.c`
- `components/bsp/include/bsp_motor.h`

融合内容：

- 电机 GPIO 不再硬编码在 motor 文件中，而是来自 `sorter_debug_config.h`。
- 支持三路电机。
- 对外提供：

```c
esp_err_t bsp_motor_init(void);
esp_err_t bsp_motor_set_output_enabled(bool enabled);
esp_err_t bsp_motor_set_compare(uint8_t motor_id,
                                uint32_t pwm_a_compare,
                                uint32_t pwm_b_compare);
esp_err_t bsp_motor_set_speed_direction(uint8_t motor_id,
                                        uint32_t speed,
                                        uint8_t direction);
```

### 10.3 编码器 BSP

文件：

- `components/bsp/bsp_encoder.c`
- `components/bsp/include/bsp_encoder.h`

融合内容：

- 编码器 GPIO 来自 `sorter_debug_config.h`。
- 当前三路编码器 GPIO 全为 `-1`，因此按未接/虚拟处理。
- 保留对外速度/距离相关接口，供 sorter 硬件状态和调度兼容使用。

### 10.4 传感器 BSP

文件：

- `components/bsp/bsp_sort_sensor.c`
- `components/bsp/include/bsp_sort_sensor.h`

融合内容：

- 初始化 S1/S2/S3/S4。
- 对 `-1` 的 S3 做 disabled 处理。
- 为 `sorting_sim_control` 提供真实传感器状态。

## 11. 分拣控制模式和命令链路

`sorting_sim_control_handle_line()` 支持的主要文本命令包括：

| 命令 | 说明 |
| --- | --- |
| `HELP` | 返回支持的命令说明。 |
| `CONFIG ...` | 配置运行模式、电机速度、超时时间、motor/sensor enable 等。 |
| `RESET` | 重置 sorter 状态。 |
| `ESTOP` | 急停控制。 |
| `PACKAGE_NEW id` | 外部模拟新包裹。 |
| `VISION_RESULT id/free/class` | 外部模拟视觉结果。 |
| `VISION_FRAME s1/free/class` | 外部模拟视觉帧/S1/分类。 |
| `SENSOR id/state` | 外部模拟传感器。 |
| `DISTANCE motor/dist` | 外部模拟距离。 |
| `MOTOR_TEST` | 启动电机测试。 |
| `HW_STATUS` | 查询硬件状态，包括 S1/S2/S3/S4、编码器、电机测试状态。 |
| `ENC_CLEAR index` | 清编码器距离。 |

当前 `System_Init()` 中已经预留真实 IO 使能代码，但处于注释状态：

```c
//sorting_sim_control_set_motor_output_enabled(true);
//sorting_sim_control_set_sensor_input_enabled(true);
```

这表示当前固件默认不会在启动阶段主动打开真实电机输出和真实传感器输入；需要硬件联调时，再确认空载或机械隔离后打开。

## 12. 板级文档同步

文件：`agentic/board.md`

已同步内容：

- GPIO53 是 S1，不再作为音频 PA_EN 使用。
- S2=GPIO23。
- S3 未接，配置为 `-1`。
- S4=GPIO22。
- Motor0/1/2 的 PWM A/B GPIO。
- 编码器全未接。
- 以太网静态 IP、上位机 IP 和 5000/5001 端口。

## 13. 当前验证状态

### 13.1 已完成的软件验证

已运行：

```bash
./agentic/idf_build.sh
```

结果：构建通过，生成：

```text
build/sample_project.bin
build/sample_project.elf
```

构建中仍有既有 warning：

```text
unknown kconfig symbol 'LV_USE_XML'
unknown kconfig symbol 'LV_USE_DEMO_SCROLL'
```

这两个 warning 来自 `sdkconfig.defaults`，不是本次融合链路的阻塞错误。

### 13.2 已完成的静态核对

已核对：

- `sorter_debug_config.h`
  - S1 = GPIO53
  - S2 = GPIO23
  - S3 = -1
  - S4 = GPIO22
  - Encoder0/1/2 A/B 全为 -1
- `vision_detect.c`
  - `case 0 -> SORTER_CLASS_1`
  - `case 1 -> SORTER_CLASS_3`
  - `case 2 -> SORTER_CLASS_2`
  - UI 文本仍为 `{"极兔", "韵达", "中通"}`
- `system_init.c`
  - 保留完整 UI handler。
  - 已预留 sorter debug 启动，但当前注释关闭。
  - 已预留 motor/sensor 使能，但当前注释关闭。
  - 已接入 `ethernet_app_start()` 条件编译启动；当前 `SORTER_TCP_LINK_ENABLE=0`，默认关闭 TCP 链路。

### 13.3 尚未完成的硬件验证

尚未执行实机烧录和端到端联调。当前代码为硬件安全默认：sorter 自动启动、motor/sensor 使能和 TCP 链路默认关闭；硬件联调前需要按现场条件打开对应开关。

待验证项：

1. 烧录运行：
   ```bash
   python3 agentic/esp_target.py flash-and-run build/ --app-only
   ```
2. RTT 控制块扫描：
   ```bash
   python3 agentic/rtt_reader.py --elf build/sample_project.elf --scan-only
   ```
3. 启动 RTT reader：
   ```bash
   python3 agentic/rtt_reader.py --elf build/sample_project.elf --output agentic/.esp-agent/rtt.log --kill-existing
   ```
4. 确认启动日志无 Guru Meditation。
5. 确认看到视觉、sorter、传感器、以太网相关日志。
6. 上位机设置为 `192.168.10.1/24`。
7. 验证控制链路 `5000`。
8. 验证图像链路 `5001`。
9. 验证 JPEG 包可解码。
10. 验证 JPEG header 中 class/confidence 与视觉 UI 结果一致。
11. 验证 20 包 TCP 回归。
12. 真实传感器/电机空载安全验证。

## 14. 融合后的数据流

### 14.1 视觉到 UI

```text
Camera RGB888 frame
  -> vision_fetch_task
  -> zero-copy ringbuf
  -> vision_detect_task
  -> vision_model_run
  -> result.ev.company / confidence / fps / infer time
  -> vision_draw_save_result
  -> UI event dispatch / overlay draw
```

原 UI 显示和阈值调节链路保留。

### 14.2 视觉到 sorter

```text
vision_detect_task
  -> best_logo >= 0
  -> category 0/1/2
  -> sorter_class_from_logo_category()
  -> sorting_sim_control_submit_vision_class()
  -> sorter_scheduler_vision_result()
  -> motor/sensor scheduling
```

### 14.3 视觉到 JPEG 图像链路

```text
Ethernet image task
  -> vision_copy_latest_frame_scaled_rgb888()
  -> esp_new_jpeg encode
  -> vision_get_latest_classification()
  -> send JPEG packet to 192.168.10.1:5001
```

### 14.4 上位机控制到 sorter

```text
Host 192.168.10.1:5000
  -> TCP control packet
  -> ESP_HOST_TYPE_SIM_LINE
  -> sorting_sim_control_handle_line()
  -> sorter_scheduler / hardware control
  -> send_sim_line_packet()
  -> host receives STATUS/HW_STATUS/etc.
```

## 15. 风险点和注意事项

1. **真实电机输出接口已接入，但当前默认关闭**
   - `System_Init()` 中预留了 `sorting_sim_control_set_motor_output_enabled(true)`，当前处于注释状态。
   - 打开前必须确认空载或机械隔离。

2. **S3 当前未接**
   - `SORTER_SENSOR_S3_GPIO = -1` 是交接工程配置，不是漏接代码。
   - `HW_STATUS` 中应看到 S3 invalid/未接。

3. **GPIO53 已改为 S1**
   - 不再作为音频 PA_EN 使用。
   - 后续不要把 GPIO53 恢复到音频用途。

4. **编码器全未接**
   - 距离相关逻辑目前依赖虚拟/超时/传感器路径。
   - 若后续接入编码器，需要只改 `sorter_debug_config.h` 和必要方向参数，不要改视觉类别映射。

5. **类别映射不能反**
   - 模型 category：`0=极兔, 1=韵达, 2=中通`。
   - sorter class：`CLASS1=极兔, CLASS2=中通, CLASS3=韵达`。
   - 因此 `cat1` 必须映射到 `CLASS3`，`cat2` 必须映射到 `CLASS2`。

6. **JPEG snapshot 使用独立 PPA client**
   - 不要改成复用显示 PPA client。
   - 显示和 JPEG 是两条不同消费路径。

7. **以太网启动失败不阻断系统**
   - `ethernet_app_start()` 失败只 warning。
   - 视觉和本地 sorter 仍可继续运行。

## 16. 后续建议验证顺序

建议按以下顺序验证，降低电机误动作风险：

1. 不接执行机构或机械隔离，烧录运行。
2. 启动 RTT，确认无崩溃。
3. 用 `HW_STATUS` 验证 S1/S2/S4 有效，S3 invalid。
4. 上位机只连 5000 控制链路，先发 `HELP` / `HW_STATUS`。
5. 再启用 5001 图像链路，确认 JPEG 可解码。
6. 用静态/手动 logo 验证 UI 公司名、JPEG class、sorter class 三者一致。
7. 执行 `MOTOR_TEST` 单电机/短时测试方向。
8. 空载跑 20 包 TCP 回归。
9. 最后再带真实包裹测试 S1 -> 视觉窗口 -> S2/S4 -> 调度完成。

## 17. 当前结论

本次代码层面的以太网 + 分拣 + 视觉/UI 融合已经完成，并已通过 ESP-IDF 构建。当前还未完成的是实机硬件端到端验证，尤其是电机输出、传感器输入、TCP 5000/5001 和 JPEG packet 的现场联调。
