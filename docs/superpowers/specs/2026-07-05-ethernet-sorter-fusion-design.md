# 以太网分拣融合设计

日期：2026-07-05

## 背景

当前工作是融合两个并行开发结果：本项目主线继续开发视觉二阶段检测，队友在 `E:\two_stage_host_handoff_20260705\new_merge_motor_two_stage` 上开发以太网、分拣调度、电机和传感器控制。融合时除快递公司类别映射外，以交接工程实现为准。

快递 logo 类别固定为：`0=极兔`、`1=韵达`、`2=中通`。物理分拣出口保持旧机器语义：极兔到 `CLASS1`，中通到 `CLASS2`，韵达到 `CLASS3`。因此视觉到 sorter 的提交映射为 `cat0->SORTER_CLASS_1`、`cat1->SORTER_CLASS_3`、`cat2->SORTER_CLASS_2`。

## 硬件接口

`agentic/board.md` 是融合后的硬件事实来源。分拣接口按交接工程接入：Motor0/1/2 分别使用 GPIO2/3、GPIO32/36、GPIO4/5；S1/S2/S4 使用 GPIO53/GPIO23/GPIO22，高电平有效；S3 和三路编码器暂未接。GPIO53 当前用于 S1，不再作为音频 PA_EN 使用。

以太网使用 ESP32-P4 内部 EMAC + IP101 PHY：MDC=GPIO31、MDIO=GPIO52、PHY_RST=GPIO51、PHY_ADDR=1。板端静态 IP 为 `192.168.10.2`，上位机/网关为 `192.168.10.1`。控制 TCP 端口为 5000，JPEG 图像端口为 5001。

## 组件结构

新增 `components/Sorter_app`，直接移植交接工程的调度核心、文本协议和控制桥接层：

- `sorter_core/sorter_scheduler.c/h`：维护包裹状态、皮带占用、超时、交接、完成和故障。
- `sorter_core/sorter_protocol.c/h`：把 typed event 格式化为外部 TCP/debug 文本协议。
- `sorting_sim_control.c/h`：解析 TCP/串口命令，桥接真实传感器、电机输出、编码器距离和 scheduler。

扩展 `components/bsp`：新增 `bsp_sort_sensor.c/h` 和 `sorter_debug_config.h`，并按交接工程 GPIO 表更新电机、传感器和编码器配置。当前主线里编码器0的 GPIO5/6 配置被交接工程覆盖，GPIO5 用于 Motor2 PWM_B，编码器全为未接。

替换 `components/Ethernet_app/ethernet_app.c` 为交接工程新版。新版分成控制链路和图像链路：控制链路连接 `192.168.10.1:5000`，处理 SIM_LINE、metrics 和 sorter tick；图像链路连接 `192.168.10.1:5001`，低频抓取最新视觉帧并编码 JPEG 发送。

## 视觉接入

视觉主链路保持现有零拷贝采集、PPA 显示和二阶段推理结构。融合只增加两个旁路能力：

1. logo 成功识别后调用 `sorting_sim_control_submit_vision_class()`，把分类结果提交给 sorter。
2. 为 Ethernet JPEG 链路导出低频快照接口：`vision_copy_latest_frame_scaled_rgb888()` 和 `vision_get_latest_classification()`。

`vision_detect.c` 需要包含 `sorting_sim_control.h`，并新增类别转换函数，严格使用 `0=极兔`、`1=韵达`、`2=中通` 的映射。`vision.h` 暴露 `vision_classification_t`、分类快照和帧缩放复制接口。`vision_app.c` 使用 PPA 从最新 ringbuf 帧复制缩放快照，不改变显示/检测任务对帧的所有权。`vision_draw.c` 维护最近分类快照供上位机图片标签使用。

`components/vision/CMakeLists.txt` 需要依赖 `Sorter_app`。

## 启动顺序

`System_Init()` 按交接工程顺序融合：LCD、触摸、摄像头、LVGL adapter、`setupUi()`、`ui_bind_dashboard()`、`vision_start()`、`sorting_sim_debug_start()`、启用电机输出和传感器输入、`system_monitor()`、`ethernet_app_start()`。

以太网启动失败只记录 warning，不阻断视觉和本地分拣链路。Sorter debug 任务先于以太网上线，使真实传感器/视觉提交和 TCP 模拟共用同一调度核心。

## 协议与运行模式

控制链路保留交接工程协议。常用命令包括 `CONFIG`、`RESET`、`ESTOP`、`MOTOR_TEST`、`HW_STATUS`、`ENC_CLEAR`、`PACKAGE_NEW`、`VISION_RESULT`、`VISION_FRAME`、`SENSOR`、`DISTANCE`。

`CONFIG` 支持 `mode=ethernet|real|timed`、A/B/C 速度、handoff delay、A/B/C timeout、lost timeout、`motor_output`、`sensor_input` 和兼容字段 `real_io`。默认电机速度为 60%。

## 验证

第一阶段验证为构建通过：`./agentic/idf_build.sh`。第二阶段烧录运行后检查 RTT：SPIFFS/模型加载成功、Ethernet 获取 `192.168.10.2`、Sorter_app 初始化成功、无 Guru Meditation。第三阶段使用交接工程的 TCP 20 包回归，目标为 `completed=20/20`，且 warnings、drops、pose_asserts、desyncs、faults 全为 0。第四阶段用真实视觉画面确认 UI 公司名、JPEG header class、sorter 提交 class 三者一致。

## 已确认边界

- 交接工程是以当前工程为基础开发的队友分支；融合时以交接工程为准。
- 快递公司映射由用户确认：`0=极兔`、`1=韵达`、`2=中通`。
- GPIO53 由用户确认接 S1，因此不再按音频 PA_EN 使用。
- 当前未知/未接硬件按交接工程配置为 `-1`，不自行猜测新 GPIO。
