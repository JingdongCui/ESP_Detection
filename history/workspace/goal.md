# Goal：恢复 70 ms 推理并完成上位机设备控制闭环

## 目标

在保留队友新增的相机 ISP/UI 功能、以太网通信和真实分拣 IO 的前提下，先定位并消除推理调度回退，再完成上位机第三页到板端的设备控制链路。

核心验收指标：

- 单个模型阶段的 `infer_diag` 延时稳定恢复到约 70 ms；验收看分布和长稳结果，不以少量快样本代替。
- 预热后 waybill/logo 单阶段分别满足：P50=`60～85 ms`、P95=`<=100 ms`、最大值=`<=150 ms`；至少 300 个有效样本中不得出现 `>=500 ms`。
- 级联总延时按实际命中路径另行统计；不要把 `wb + crop + logo` 的级联总时间误当成单模型 70 ms 指标。
- Ethernet 必须能启动、Link Up、取得静态地址 `192.168.10.2`，并能连接队友上位机。
- 所有必需任务必须创建成功；栈地址、最低剩余水位、内部 SRAM/PSRAM归属和堆连续块必须有实测证据。
- 上位机控件必须与板端真实函数逐项对应，设置后由板端 state 回读校正，不能只验证 UI 数字变化。
- 必须完成构建、协议、实机连接、断线重连、长稳、视觉效果和截图/照片证据验收。
- 不允许以崩溃、看门狗、丢失真实传感器事件或明显卡 UI 为代价换取延时。

## 执行顺序

必须按以下阶段顺序推进，不要把性能实验、控制协议和 UVC 内存问题混在同一个提交里：

1. **保护现场并建立多份 Git 备份。** 识别并保存当前另一个 agent 的未提交实验。
2. **恢复单阶段推理延时。** 完成启动矩阵和最小修复，先取得稳定的约 70 ms 基线。
3. **完成板端设备控制协议。** 上位机已经完成；这里只补板端接收、业务调用、状态回传和联调。
4. **最后处理 UVC。** 只有前两项目标稳定后才允许继续 JPEG DMA 内存问题。
5. **完整回归并维护文档。** 任何阶段失败都回到该阶段最近的 Git checkpoint，不跨阶段叠加猜测性改动。

## 已完成内容：不要重复实现

上位机工程 `esp32_host_no_inference` 已完成第三页和 `CONTROL_JSON` 主机侧实现：

- checkpoint：`c85829d checkpoint host UI refresh before device controls`
- 功能提交：`6bcee3b feat: implement remote device control dashboard`
- 上位机工作树当前应保持干净。
- 已通过 CMake build、CTest 和 8 秒 `QT_QPA_PLATFORM=offscreen` 启动检查。
- 已实现 `type=0x11` 控制 JSON 构造/解析、连接后状态查询、显示/ISP/检测/三路电机/网络上报/重启 UI，以及协议测试。

除非板端联调证明协议字段或解析存在明确错误，否则不要重做或大改上位机 UI。若确需修改，必须先在上位机独立仓库建立新 checkpoint，并单独提交。

## 尚未完成内容

### A. 板端设备控制协议

当前上位机发送 `ESP_HOST_TYPE_CONTROL = 0x11`，板端 `process_rx_packet()` 尚未处理该类型，因此第三页控制暂时不会改变硬件或业务状态。

需要实现以下 wire format：

```json
{"op":"get"}
{"op":"set","key":"camera.brightness","value":128}
{"op":"action","key":"device.restart"}
{"op":"state","ok":true,"settings":{},"capabilities":{}}
{"op":"error","key":"...","message":"..."}
```

固定控制键及实际业务函数：

| 控制键 | 板端调用 |
|---|---|
| `display.screen_brightness` | `BSP_LCD_SetBrightness()`，必须遵守其 UI/LVGL 单任务约束 |
| `camera.brightness` | `cam_sensor_isp_set_brightness()` |
| `camera.contrast` | `cam_sensor_isp_set_contrast()` |
| `camera.contrast_auto` | `cam_sensor_isp_set_contrast_auto()` |
| `camera.saturation` | `cam_sensor_isp_set_saturation()` |
| `camera.saturation_auto` | `cam_sensor_isp_set_saturation_auto()` |
| `camera.hue` | `cam_sensor_isp_set_hue()` |
| `vision.detection_enabled` | `vision_set_detection_enabled()` |
| `vision.preview_overlay_enabled` | `vision_set_preview_overlay_enabled()` |
| `vision.waybill_threshold` | `vision_model_set_waybill_score_threshold_percent()` |
| `vision.logo_threshold` | `vision_model_set_logo_score_threshold_percent()` |
| `sorter.motor_a_speed` | `sorting_sim_control_get/apply_settings()` 的 A 路速度 |
| `sorter.motor_b_speed` | `sorting_sim_control_get/apply_settings()` 的 B 路速度 |
| `sorter.motor_c_speed` | `sorting_sim_control_get/apply_settings()` 的 C 路速度 |
| `report.image_enabled` | `ethernet_app_set_report_image_enabled()` |
| `report.metrics_enabled` | `ethernet_app_set_report_metrics_enabled()` |
| `device.restart` | `esp_restart()` |

状态查询必须返回：

- 上述所有可调值；
- ISP brightness/contrast/saturation/hue 的 `supported/min/max/step`；
- `network.local_ip`、`network.host_ip`、`model.info`；
- 曝光、增益、红/蓝白平衡增益通过现有 metrics JSON 上报；即使关闭指标上报，控制状态和错误包仍必须可返回。

处理要求：

- 使用 IDF `json/cJSON` 或等价的严格解析，不用脆弱的字符串截取代替 JSON 校验。
- 严格验证 `op/key/value` 类型、数值范围和 ISP step；失败不改变状态并返回 `op=error`。
- 上位机采用乐观更新，不等待逐条回包；板端仍要在 `get` 和成功 `set` 后异步回传最新 state，用于纠正实际值。
- 远程修改后同步板端现有设置控件，但不要修改 AnyUI 生成文件；只允许在 `components/UI/sdk/ui.c/.h` 手写绑定层实现。
- 屏幕亮度不能直接从 Ethernet 任务违反 `BSP_LCD_SetBrightness()` 的单任务约束，应通过 UI/LVGL 上下文调度。
- 重启按已确认需求单击直接执行，不增加确认弹窗。

### B. 明确不做的设备控制项

- PID 闭环控制及 20/30/40/50 cm/s 按钮。
- 数据/指标发送频率调节。
- 面单/Logo 模型 1/2/3 切换；板端当前只有按钮状态，没有模型热切换后端。
- 恢复出厂设置；板端当前没有实际存储恢复函数。
- 新增电机总启停、传感器输入开关；本轮分拣控制只做 A/B/C 三路开环速度。

### C. 板端控制联调与测试

- 上位机连接后自动 `get`，第三页显示真实设置和 ISP 能力范围。
- 逐项验证屏幕亮度、四项 ISP、两个自动开关、检测、叠框、两个阈值、A/B/C 速度、图像上报、指标上报和重启。
- 验证未知 key、错误 JSON、错误类型、越界值、ISP 不支持项均返回错误且不改变状态。
- 验证关闭 metrics 后仍能重新打开 metrics，控制通道不被关闭。
- 验证重启后控制/图像两个 TCP 端口自动重连并重新同步状态。

## 当前实测问题（2026-07-16）

当前烧录配置为 Ethernet DMA `RX=10 / TX=6`，队友上位机已连接时，推理稳定为：

```text
wb=552680~618680 us
logo=514924~594034 us
crop=23~54 ms
wall=1.10~1.25 s
current-task cpu=196~315 ms
wait=0.90~0.96 s
```

代表日志：

```text
[infer_diag] path=cascade ... wb=552680us crop=35514us logo=544532us wall=1132726us cpu=219947us wait=912779us
[infer_diag] path=cascade ... wb=607164us crop=44004us logo=528761us wall=1179929us cpu=219684us wait=960245us
```

结论：现在不是“级联总延时 500 ms”，而是 waybill 和 logo **每个阶段各约 500～600 ms**，级联总 wall time 约 1.1～1.25 s。大量时间不计在 `vision_det` 当前任务 CPU 中，可能发生在 ESP-DL 双核 worker、同优先级调度等待或共享总线/缓存等待中。

## 队友提供的任务栈/RTT观测快照

以下内容由队友通过 JTAG 扫描和 `agentic/.esp-agent/rtt.log` 获取，作为后续 goal 的输入证据。**采样所用固件 SHA、启动选项及其与当前工作树的对应关系尚未核对**；因此先记录为观测快照，不能直接用来证明当前未提交的 PSRAM 栈修改已经生效或未生效。

ESP32-P4 当前配置中 `StackType_t` 为 1 byte；下表“最低剩余”是任务运行以来的历史最低剩余栈，并非瞬时空闲量。JTAG 扫描报告的 19 个栈地址均为 `0x4FFxxxxx`（内部 SRAM），没有栈位于外部 PSRAM `0x48000000–0x4BFFFFFF`。合计分配约 87.48 KiB，历史最低剩余合计约 56.73 KiB。

| 任务 | 核 | 分配 | JTAG最低剩余 | 历史最大使用 | JTAG内存归属 |
|---|---:|---:|---:|---:|---|
| swdraw | 0 | 8052 B | 6708 B | 1344 B | 内部 SRAM |
| vision_det | 1 | 12144 B | 10272 B | 1872 B | 内部 SRAM |
| vision_disp | 0 | 3956 B | 1492 B | 2464 B | 内部 SRAM |
| lvgl | 1 | 16244 B | 9444 B | 6800 B | 内部 SRAM |
| isp_task | 0 | 3948 B | 1404 B | 2544 B | 内部 SRAM |
| vision_fetch | 0 | 4092 B | 3244 B | 848 B | 内部 SRAM |
| esp_timer | 0 | 3952 B | 3472 B | 480 B | 内部 SRAM |
| Tmr Svc | 任意 | 2040 B | 1624 B | 416 B | 内部 SRAM |
| ipc1 | 1 | 1016 B | 616 B | 400 B | 内部 SRAM |
| ipc0 | 0 | 1008 B | 472 B | 536 B | 内部 SRAM |
| dl_mc1 | 1 | 2008 B | 1152 B | 856 B | 内部 SRAM |
| dl_mc0 | 0 | 1784 B | 928 B | 856 B | 内部 SRAM |
| sort_real_io | 0 | 3948 B | 1984 B | 1964 B | 内部 SRAM |
| sort_dbg | 0 | 4084 B | 3176 B | 908 B | 内部 SRAM |
| cam_isp | 1 | 3948 B | 3244 B | 704 B | 内部 SRAM |
| sysmon | 0 | 3948 B | 1824 B | 2124 B | 内部 SRAM |
| main | 0 | 10360 B | 4664 B | 5696 B | 内部 SRAM |
| IDLE1 | 1 | 1520 B | 1184 B | 336 B | 内部 SRAM |
| IDLE0 | 0 | 1528 B | 1192 B | 336 B | 内部 SRAM |

RTT 的 `system_monitor` 表中 `STK` 同样表示历史最低剩余栈，最近一张完整记录为 `[SYSMON #5020]`、uptime 约 1260 s：

| 任务 | 核 | RTT CPU | RTT STK最低剩余 |
|---|---:|---:|---:|
| IDLE0 | c0 | 52% | 1192 B |
| dl_mc0 | c0 | 38% | 944 B |
| dl_mc1 | c1 | 37% | 944 B |
| IDLE1 | c1 | 36% | 1184 B |
| vision_det | c1 | 22% | 9308 B |
| swdraw | c0 | 3% | 6500 B |
| lvgl | c1 | 3% | 9444 B |
| sysmon | c0 | 1% | 1836 B |
| isp_task | c0 | 1% | 1144 B |
| esp_timer | c0 | 1% | 3472 B |
| sort_dbg | c0 | 0% | 3176 B |
| cam_isp | c1 | 0% | 3180 B |
| main | c0 | 0% | 4664 B |
| sort_real_io | c0 | 0% | 1984 B |
| ipc0 | c0 | 0% | 472 B |
| vision_fetch | c0 | 0% | 3244 B |
| vision_disp | c0 | 0% | 1516 B |
| Tmr Svc | 任意 | 0% | 1624 B |
| ipc1 | c1 | 0% | 616 B |

对 goal 的直接意义：

- 栈容量明显过量的首批候选是 `vision_det`、`swdraw`、`vision_fetch`、`sort_dbg`，但正式缩减前仍需覆盖最坏业务路径并留足安全余量。
- `vision_disp`、`isp_task`、`sort_real_io`、`sysmon` 的已用量约 2.0～2.5 KiB，不能直接从 4 KiB砍到 2 KiB；优先考虑 3～3.5 KiB并重新跑高水位。
- `ipc0/ipc1` 和 `dl_mc0/dl_mc1` 的绝对剩余较小，暂不缩减。
- RTT 显示 `dl_mc0`/`dl_mc1` 分别约占 38%/37% CPU，说明必须把 ESP-DL worker 的实际运行时间纳入 500 ms根因分析，不能仅依赖 `vision_det` 的 current-task CPU。
- “所有栈均在内部 SRAM”与当前代码声称 Ethernet/ISP 栈使用 PSRAM存在冲突。下一次测试必须同时记录固件 ELF SHA、任务句柄栈起始地址及 `heap_caps_check_integrity_all()` 结果，确认是旧固件快照、创建 API未生效，还是监控表遗漏了 Ethernet 任务。

## 已确认现状

- 固件工程：`ESP32P4_Detection`
- 当前合入队友代码后的提交：`5f5be51 merge teammate camera UI and stable image pipeline`
- 合入前可对照标签：`backup/before-teammate-merge-20260716`（`ba3ef25`）
- `idf.py build` 已通过。
- 完整 `idf.py flash monitor` 已完成，bootloader/app/partition/storage 均通过 hash 校验。
- 相机：`1024x600 RGB888, 3 buffers, 30 fps`。
- Ethernet：RX10/TX6 下成功 Started / Link Up / Got IP，队友上位机控制与图像端口均连接成功。
- UVC 仍失败：`jpeg_new_encoder_engine: no memory for jpeg encoder rxlink`。这是独立的内部 DMA 小块碎片问题，不是本 goal 的首要指标。
- Ethernet 应用层三个任务栈本来就已通过 `xTaskCreatePinnedToCoreWithCaps` 放在 PSRAM：
  - `eth_control` 8192 B
  - `eth_img_prod` 8192 B
  - `eth_img_send` 6144 B
- ISP 任务栈已改到 PSRAM；当前实验配置还把 ISP 降为优先级 2并固定 core0。
- 推理任务 12 KB 栈必须继续保留在内部 SRAM。此前已实测：推理栈放 PSRAM 会在中断保存现场与 DMA/缓存竞争时造成双核死锁。

## 尚未提交的工作树实验

开始 goal 前先检查：

```bash
git -C ESP32P4_Detection status --short
git -C ESP32P4_Detection diff
```

当前预期包含：

- 摄像头帧缓冲数改为 Kconfig，当前值 3。
- ISP worker 使用 PSRAM 栈、优先级 2、core0。
- Ethernet DMA 队列改为 RX10/TX6。
- `ethernet_init.c` 中 EMAC RX 被实验性改为优先级 3并固定 core0。

其中 EMAC RX 降优先级/绑核已经实测 **对 500 ms 问题无改善**，不应当把它当作修复；若没有新的证据，先撤销这一项，避免降低收包可靠性。不要丢弃其他未提交改动。

## 已排除或基本排除

1. **TCP 重连风暴不是主因。** 无上位机时慢；队友上位机稳定连接后仍慢。
2. **EMAC RX 默认优先级 15不是主因。** 临时降到优先级 3并固定 core0后仍为 1.1 s 左右。
3. **单纯减少 Ethernet DMA 队列不能修复延时。** RX20/TX10、RX16/TX8、RX12/TX6、RX10/TX6 均观察到或无法消除稳态回退；缩队列主要解决初始化内部 RAM 不足。
4. **仅把 ISP 栈放 PSRAM不能解决稳态回退。** 它释放了内部 RAM；ISP 降优先级后，完整初始化前的一次推理可到约 216～310 ms，但系统全部任务启动后仍回退到约 1.1 s。

## 最优先调查方向

### 1. 用启动矩阵确定真正的分界点

日志时间线显示，快推理发生在 `sorting_sim_control_start()` 之前；慢推理发生在真实分拣 IO 启动之后。Ethernet 恰好位于两者之间，不能仅凭时间相关性认定是网卡。

做可回退的 A/B 实验，每组至少记录 20 次 `infer_diag`：

1. 完整系统，仅临时跳过 `sorting_sim_control_start()`。
2. 保留分拣，临时跳过 `ethernet_app_start()`。
3. 保留 Ethernet 与分拣，临时跳过 `screen_uvc_start()`。
4. 完整系统作为对照。

每次同时记录：`wb/crop/logo/wall/cpu/wait`、任务运行时间、内部 RAM、最大连续块和是否发生重启。

### 2. 重点检查同优先级任务竞争

当前关键任务：

- `vision_det`：core1，priority 4。
- ESP-DL `dl_mc0/dl_mc1`：分别 core0/core1；每次推理继承调用者 priority 4。
- `vision_fetch`：core0，priority 4。
- `vision_disp`：core0，priority 4。
- `sort_real_io`：core0，priority 4，每 10 ms 轮询一次。
- LVGL worker：priority 5。
- ISP worker：当前实验为 core0，priority 2。

高概率问题是 `dl_mc0` 与 `sort_real_io`、fetch/display 同为 priority 4，导致双核算子在同步点等待 core0 worker。必须用 FreeRTOS runtime/task snapshot 或在 ESP-DL worker 两端加入低开销计时验证，不要只看 `vision_det` 的 CPU 时间。

候选修复按风险从低到高验证：

1. 优化 `sort_real_io`：事件驱动/任务通知代替固定 10 ms 重轮询，减少每次唤醒工作量。
2. 将非实时显示/后台任务降级，但真实传感器采样不能长期饿死。
3. 推理期间临时提升 `vision_det` 及 ESP-DL worker 优先级，完成后恢复；验证 UI 和传感器最大阻塞时间。
4. 若必须保留 10 ms 真实 IO，评估单核推理或更合理的 core 分工，禁止盲目把真实 IO 栈搬到 PSRAM来“解决”调度问题。

### 3. 对比合入前后的模型和相机链路

使用 `git diff ba3ef25..5f5be51` 和独立 worktree 对比，重点检查：

- 相机由 15 fps 改到 30 fps后的 PSRAM/AXI 带宽影响。
- 新 ISP pipeline、稳定帧复制和 PPA 操作频率。
- 模型文件、ESP-DL 配置、编译优化、cache/PSRAM 配置是否变化。
- `infer_diag` 的 CPU 统计是否遗漏两个 ESP-DL worker 的运行时间。

必要时只把相机临时恢复 15 fps做 A/B；若能接近 70 ms，再评估 30 fps预览与推理采样解耦。

## PSRAM 使用原则

适合放 PSRAM：

- Ethernet 应用层控制/图像发送任务栈（已经完成）。
- ISP 设置与状态采样这种低优先级、非中断关键路径任务栈（已经完成）。
- JPEG 大缓冲、稳定帧和非 DMA 控制数据。

暂时不要放 PSRAM：

- `vision_det` 栈和 ESP-DL worker 栈。
- EMAC 底层 RX 栈。
- 中断、高频实时 IO、持有关键锁或 cache-disable 阶段会运行的任务栈。

PSRAM 只能解决内部 SRAM 容量问题，不能自动解决任务优先级、core 竞争或 PSRAM/AXI 带宽问题。

## UVC 后续问题

推理恢复后再处理。当前失败在 JPEG 引擎的内部 DMA 对齐描述符分配：

```text
jpeg_new_encoder_engine(93): no memory for jpeg encoder rxlink
```

优先方案是早期只预留 JPEG engine 的少量内部 DMA 资源，UVC 大缓冲和 USB 初始化仍最后执行；不要简单把整个 UVC 提前，从而再次挤掉 Ethernet DMA。

## 验收方案与完成标准

只有以下所有项目都有可复核证据时，goal 才能标记完成。单次启动成功、少量日志、纯 offscreen 启动或只看上位机 UI 均不能作为最终验收。

### 1. 固件与上位机静态检查

板端每次正式修改后执行：

```bash
cd /home/kazeform/2026esp/ESP32P4_Detection
source /home/kazeform/ProgramFiles/esp_idf_5.5.4/esp-idf-v5.5.4/export.sh
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor
```

串口应优先使用实际存在的 `/dev/serial/by-id/...` 稳定路径；若使用 `/dev/ttyUSB0`，验收记录必须同时写入 USB VID/PID/序列号，避免换板后误烧。

上位机至少执行：

```bash
cd /home/kazeform/2026esp/esp32_host_no_inference
cmake --build build/linux-debug -j
ctest --test-dir build/linux-debug --output-on-failure
```

若工程提供 `qmllint`、格式检查或协议测试目标，也必须运行并保存结果。最终还必须在真实图形会话启动一次；`QT_QPA_PLATFORM=offscreen` 只用于冒烟，不能替代视觉验收。

通过条件：

- 板端 `idf.py build` 零错误；警告须逐条说明是否为既有生成代码警告。
- `flash` 的 bootloader、partition、app、storage全部 hash verified。
- Host build和全部 CTest通过，无 QML运行时 error、binding loop或资源加载失败。
- 验收报告记录根仓库、板端、上位机三个 Git SHA以及固件 ELF SHA256。

### 2. 70 ms推理性能验收

统计对象必须分开：

- `wb`：waybill模型阶段。
- `logo`：logo模型阶段。
- `crop`：ROI裁剪/复制阶段，单独统计，不混入70 ms模型指标。
- `wall`：完整级联端到端时间，用于系统体验验收。

采样规则：

1. 每次冷启动完成全部模块初始化后，丢弃前 10 次推理作为预热。
2. 至少完成 5 次冷启动；每次启动至少收集 60 个包含 waybill 和 logo 的有效级联样本，总样本数不少于 300。
3. 样本必须覆盖：Host未启动、Host已连接、metrics开启、图像上传、控制页连续操作、真实 IO启用六种负载；不得只在关闭业务模块的实验配置上验收。
4. 使用脚本从原始 RTT/UART日志生成 CSV，至少包含 `boot_id/sample_id/wb/crop/logo/wall/cpu/wait/host_connected/image_send/real_io`。
5. 报告每个阶段的 count、min、P50、P90、P95、P99、max、mean和标准差，并保留原始日志。

硬性通过门槛：

- waybill：P50=`60～85 ms`、P95=`<=100 ms`、max=`<=150 ms`。
- logo：P50=`60～85 ms`、P95=`<=100 ms`、max=`<=150 ms`。
- 至少 300 个验收样本中，任何单模型阶段出现 `>=500 ms`，本项直接失败并继续定位，不能按异常值删除。
- 两模型均执行的级联路径建议 P95=`<=220 ms`、max=`<=300 ms`；若模型固有耗时证明无法满足，必须给出拆分数据和用户确认，不能自行放宽。
- 连接Host、发送图像或操作控制页前后，单阶段P95劣化不得超过10%；不允许从70 ms重新稳定升到500 ms。
- 日志中不得出现模型任务创建失败、推理超时、framebuffer竞态、panic、watchdog或异常重启。

### 3. 任务启动、栈水位与内存验收

每次冷启动必须枚举所有任务，而不是只枚举当前能被RTT表看到的19个。至少包括：

- 系统：`main`、`ipc0/1`、`IDLE0/1`、`Tmr Svc`、`esp_timer`。
- UI/视觉：`lvgl`、`swdraw`、`vision_fetch`、`vision_disp`、`vision_det`、`dl_mc0/1`。
- 相机/ISP：驱动 `isp_task`、应用 `cam_isp`。
- 分拣：`sort_real_io`、`sort_dbg`，以及测试时临时创建的任务。
- 网络：`emac_rx`、`tcpip_thread`、`eth_control`、`eth_img_prod`、`eth_img_send`。
- UVC启用后：USB/UVC/JPEG相关任务和句柄。

每个任务至少记录：任务名、handle、core/affinity、priority、分配栈字节、历史最低剩余字节、最大使用字节、栈起始地址、内部 SRAM/PSRAM归属、创建返回值。

通过条件：

- 所有必需任务和驱动句柄均创建成功，无 `ESP_ERR_NO_MEM`、NULL handle或静默跳过。
- 连续运行60分钟后，应用自建任务的最低剩余栈必须同时满足 `>=512 B` 且 `>=分配栈的20%`；中断/复杂库调用任务建议保留至少1 KiB。
- 可调应用任务若长期最低剩余 `>60%`，应缩栈或迁入PSRAM，或者在验收报告中说明保留原因；目标水位区间为剩余约25%～50%，不是把栈压到溢出边缘。
- `ipc0/1`、`dl_mc0/1` 等IDF/ESP-DL静态任务不为节省少量内存盲目缩减；除非有专门压力测试和上游实现依据。
- 适合PSRAM的 Ethernet应用任务、低优先级ISP状态任务必须以实际栈地址证明位于PSRAM；不能只以调用了 `WithCaps` API作为证据。
- `vision_det`、ESP-DL worker、EMAC RX和中断关键路径继续使用内部 SRAM，除非新的实机证据证明迁移安全。
- 完整初始化后记录 `MALLOC_CAP_INTERNAL`、`MALLOC_CAP_DMA|INTERNAL`、`MALLOC_CAP_SPIRAM` 的 free/min/largest block；前后对比必须证明没有持续泄漏，且能满足 Ethernet/JPEG DMA所需对齐连续块。
- 运行前后执行堆完整性检查；不得出现heap corruption、stack overflow hook、任务看门狗或双核死锁。
- “充分利用内存”定义为：安全水位满足、适合外置的栈实际进入PSRAM、内部 DMA连续块足够、无明显超配且无泄漏；不以单纯追求最低剩余字节为目标。

建议压力路径：同时保持相机30 fps、LCD刷新、级联推理、Ethernet metrics、图像JPEG发送、真实 IO轮询和Host控制操作，持续60分钟后再读取最终水位。

### 4. 上位机—板端控制逻辑闭环验收

每个控制项必须验证“Host发包 → 板端严格解析 → 调用真实业务函数 → 板端回传state/error → Host按回传值显示”完整闭环：

| 控制项 | 实机动作/观测 | 回读验收 |
|---|---|---|
| 屏幕亮度 | 板端LCD亮度实际改变 | `display.screen_brightness`与板端当前值一致 |
| 相机亮度/对比度/饱和度/色相 | 板端预览画面产生对应变化 | state返回实际对齐后的值及supported/min/max/step |
| 对比度/饱和度自动 | 自动与手动状态实际切换；禁用项正确灰化 | 布尔值与板端状态一致 |
| 检测开关 | 推理日志和检测结果实际停止/恢复 | `vision.detection_enabled`一致 |
| 叠框开关 | Host/板端约定的框显示实际隐藏/恢复 | `vision.preview_overlay_enabled`一致 |
| waybill/logo阈值 | 边界样本检出行为或板端阈值实际改变 | 两个threshold回读一致 |
| A/B/C速度 | 三路PWM/电机命令分别改变，不串路 | 三路设置值分别回读一致 |
| 图像上报 | image TCP payload停止/恢复 | `report.image_enabled`一致 |
| 指标上报 | metrics停止后仍可用control重新开启 | `report.metrics_enabled`一致 |
| 重启 | 板端真实重启并重新连接两个端口 | Host恢复连接并自动重新`get`状态 |

每项至少测试：最小值、典型值、最大值、重复设置同值、快速连续修改。所有范围必须以板端capabilities为准，不得把Host写死范围当真实能力。

负向协议测试必须覆盖：未知op、未知key、缺字段、错误JSON、错误类型、越界值、错误step、超长payload、截断包、重复包和不支持项。通过条件是板端不改变状态、回传明确error、控制连接不断开且后续合法命令仍可执行。

### 5. Ethernet连接、断线与恢复验收

至少执行：

- 5次板端冷启动，Host先启动；两个TCP端口均应自动连接并完成时间/状态同步。
- 5次板端冷启动，Host后启动；重连不能造成任务泄漏、堆持续下降或推理升到500 ms。
- 10次Host关闭/重开；control和image都必须恢复，旧socket和旧队列正确清理。
- 5次网线拔插或接口down/up；Link/IP/两个TCP通道均自动恢复。
- 5次远程重启；Host必须恢复连接、重新查询状态，UI不得保留错误的“已连接”假状态。
- 发送至少100个控制请求并穿插图像传输；无协议错位、粘包误解析、死连接或无限重试刷屏。

每轮保存时间戳日志，统计连接耗时、重连次数、失败次数、堆变化和推理P95。任何一次无法自动恢复、异常POWERON重启或稳定推理回退均判失败。

### 6. 分拣真实IO与整机长稳验收

- S1上升沿建包、S2/S4状态转换、三路电机命令、编码器采样和人工分类路径必须逐项实测。
- 每个有效传感器至少触发50次，记录输入边沿、package id、调度状态和电机输出；不得漏事件、重复建包或串包。
- 在推理峰值、图像上传和控制操作同时发生时重复测试，真实IO响应不能因追求70 ms而长期饿死。
- 连续运行至少60分钟；建议最终交付前运行2小时。期间保持Host连接、相机、LCD、推理、metrics、周期图像、真实IO和控制操作。
- 长稳期间不得出现panic、watchdog、stack overflow、heap corruption、双核死锁、异常重启、连接永久中断或持续内存下降。

### 7. UI视觉与截图/照片验收

最终必须在真实图形环境检查上位机全部页面，不能只看编译和日志。至少保存以下证据：

1. Host连接页：control/image两个通道均连接、IP/状态正确。
2. 实时图像页：画面比例正确，waybill和三类Logo框位置/颜色正确，无拉伸、错位或旧框残留。
3. 控制页全景：分组、标签、单位、范围、禁用状态和当前值清晰可见。
4. 控制操作前/后对比：至少包含LCD亮度、一个ISP参数、检测/叠框开关和一路电机速度。
5. capabilities/state同步后的页面：Host显示值与板端日志一致。
6. 非法控制请求的错误反馈，以及错误后连接仍正常。
7. 断线、重连、板端重启后的页面状态，不能残留假连接或旧状态。
8. 板端LCD/相机预览实拍：亮度、ISP变化、检测框和页面显示效果；若UVC已恢复，可补充数字截图，但不能用它替代必要的实拍。

截图要求：使用真实窗口尺寸与目标分辨率，包含时间/测试编号；不得裁掉关键状态。逐张人工检查文字截断、重叠、颜色对比、滑块/开关状态、框坐标和高DPI缩放。发现视觉问题须修复并重新截图。

### 8. 验收证据与最终报告

统一保存到 `docs/agent/archive/2026-07-17-goal-acceptance/`（大体积原始日志可放忽略目录，只提交摘要、哈希和路径），至少包含：

- `README.md`：三仓库SHA、硬件/串口、构建与烧录命令、最终结论。
- `inference.csv`和统计摘要：全部有效样本及P50/P95/P99/max。
- `tasks.csv`：完整任务、优先级、core、栈地址、水位和内存归属。
- `memory.csv`：各阶段internal/DMA/PSRAM free/min/largest block。
- `connection.md`：冷启动、Host重启、网线拔插、远程重启结果。
- `controls.md`：每个控制键的正向、边界、负向和回读结果。
- `stability.md`：60分钟/2小时长稳记录、重启计数、最低堆和最低栈。
- `screenshots/`：上述Host截图与板端实拍，带编号和说明。
- 原始UART/RTT/Host日志的文件名、SHA256和保存位置。

最终完成清单：

- 推理300+样本满足70 ms分布要求，零个500 ms回退。
- 所有必需任务/句柄启动，栈水位安全，PSRAM归属真实生效，内部DMA内存充足且无泄漏。
- Host与板端所有约定控制项逐项闭环，正向/边界/错误测试通过。
- Ethernet启动、断线、重连、远程重启和图像/指标链路通过。
- 真实分拣IO和长稳通过，UI与截图人工验收通过。
- `docs/agent/PROJECT.md`、`CURRENT.md`、`HISTORY.md`及阶段归档已更新。
- 三个仓库工作树干净，所有最终提交和stable标签均可定位；不存在未解释的实验残留。

## 强制 Git 备份制度

本 goal 要求“多 Git 备份”，不是只在最终完成时提交一次。根目录、上位机和板端是三个独立仓库，必须分别操作，严禁在错误目录提交。

### 1. 开始前保护现场

先记录三个仓库：

```bash
git -C /home/kazeform/2026esp status --short --branch
git -C /home/kazeform/2026esp/esp32_host_no_inference status --short --branch
git -C /home/kazeform/2026esp/ESP32P4_Detection status --short --branch
```

板端当前预期有另一个 agent 的五个未提交文件：

```text
components/bsp/bsp_cam_sensor.c
components/bsp/ethernet_init.c
components/bsp/include/bsp_cam_sensor.h
sdkconfig
sdkconfig.defaults
```

这些改动包含 3 framebuffer、ISP PSRAM worker、RX10/TX6 和 EMAC RX 优先级实验。不要 `reset`、`checkout`、`clean` 或覆盖。优先等待原 agent 提交；若开始 goal 时仍未提交，先完整检查 diff，再建立独立 checkpoint 提交并明确写“保留队友未完成实验”，不得把它伪装成已验证修复。

### 2. 分支、标签与提交节奏

在板端当前分支最新状态上建立 goal 专用分支；不要移动或删除已有标签：

```bash
git -C /home/kazeform/2026esp/ESP32P4_Detection switch -c goal/inference-and-device-control
git -C /home/kazeform/2026esp/ESP32P4_Detection tag backup/goal-start-20260716
```

若分支或标签已存在，先检查其指向，禁止强制覆盖。至少保留以下独立提交/标签节点：

1. `checkpoint: preserve teammate runtime experiments`
2. `test: record startup matrix baseline`
3. 每个 A/B 实验各自一个临时结果提交；失败实验用 `git revert` 生成可追溯回退提交。
4. `fix: restore inference scheduling`
5. 标签 `backup/inference-70ms-stable-20260716`
6. `feat: handle host device control json`
7. `test: verify ethernet device controls`
8. 标签 `backup/device-controls-stable-20260716`
9. 根仓库单独提交最终 `goal.md` 和 `docs/agent` 记录。

提交规则：

- 每次风险改动前先提交当前可工作的状态；一次提交只包含一个实验假设或一个功能阶段。
- 不使用 `git commit --amend`、交互式 rebase、force push 或历史改写；在整个 goal 完成前不 squash。
- 回退失败实验优先使用 `git revert <commit>`，不使用 `git reset --hard` 或 `git checkout -- <file>`。
- 每次 build/flash/monitor 后，把命令、固件提交哈希、串口、关键日志和结论写入 `docs/agent/HISTORY.md`，然后单独提交文档。
- 上位机若因联调需要修改，必须在 `esp32_host_no_inference` 独立提交，不能混入根仓库或板端提交。
- 每次提交后执行 `git status --short --branch`，确保没有意外文件和未解释的残留改动。

### 3. 实验备份原则

- 推荐用独立 commit/revert 保存 A/B 实验，不要依赖 stash 作为唯一备份。
- 长日志可以归档到 `docs/agent/archive/`，不要把整个 `build/` 或大体积串口原始文件提交进 Git。
- 任何“看起来有效”的修复必须先保存前后两份日志和两个可定位提交，再进入下一实验。
- 只有完成本文件规定的5次冷启动、300+推理样本、连接/控制/真实IO和长稳验收后，才能给阶段打stable标签。

## 操作约束

- 保留用户自己的根目录 `AGENTS.md`、`.agents`、`.codex` 和归档中的 Agent/Skill 内容；只清理队友压缩包自带的相关文件。
- 不要破坏标签 `backup/before-teammate-merge-20260716`。
- 不要覆盖或丢弃当前未提交实验；先逐项辨认、记录，再选择保留或撤销。
- 上位机继续直接使用 `esp32_host_no_inference` 的 `6bcee3b` 功能基线；本 goal 主要修改板端，只有联调发现明确主机端缺陷时才改上位机。
- 未完成推理恢复前，不开始板端控制协议代码；未完成控制联调前，不开始 UVC 内存修复。
- 不允许为了追求 70 ms 永久关闭 Ethernet、真实分拣 IO、检测功能或板端 UI；临时跳过只能作为有独立提交的 A/B 实验。
