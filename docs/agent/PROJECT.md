# Project Notes

## Workspace

- 根目录：`/home/kazeform/2026esp`
- 当前主任务工程：`ESP32P4_Detection`
- 当前上位机工程：`esp32_host_no_inference`
- 历史工程归档目录：`archive_project/`
- 根目录 git 仓库已初始化，用于跟踪根目录 agent 文档；`ESP32P4_Detection` 和 `esp32_host_no_inference` 各自有独立 git 仓库。

## Report Preparation

- 比赛报告资料源：
  - `hardware.md`：已知硬件信息整理。
  - `report_requset.docx`：用户提供的报告内容要求。
  - `docs/report_system_information.md`：作品、硬件、软件、上位机和性能信息总览。
  - `docs/report_unknown_information.md`：正式报告前待补充、待确认、待实测清单。
- 当前报告要求包括：作品名称、摘要、作品概述、系统组成及功能说明、完成情况及性能参数、总结、参考文献。
- 报告中不能出现学校名称、指导老师等内容。
- 当前原始性能记录为：光线条件良好时正确率 95% 以上，分拣速度每分钟 20 件以上；正式写入前应补充样本数、统计方法和实测证据。
- 当前最新固件状态曾停用硬件分拣启动；正式演示前需确认最终使用固件是否恢复真实电机输出和传感器输入。

## Dataset Notes

- 2026-07-04 已整理 `MyAlbums` 为后续训练数据集：
  - 目录只保留 `16_9/`、`4_3/`、`half/` 三类和 `DATASET_MANIFEST.csv`。
  - 顶层原目录名包含 `16_9` 的归入 `16_9`，包含 `half` 的归入 `half`，未标注的归入 `4_3`；`zt1higher` 按未标注规则归入 `4_3`。
  - 图片统一命名为 `jt_001.jpg`、`yd_001.jpg`、`zt_001.jpg` 形式，每个分类目录内按品牌独立编号。
  - 数量：`16_9=430`，`4_3=368`，`half=180`，总计 `978` 张 jpg。
  - `half` 当前只有 `jt=94` 和 `yd=86`，原始数据里没有 half 标注的 `zt` 目录。
  - `MyAlbums` 子目录内已初始化独立 git，原始基线提交 `830536a`，整理后提交 `8a51bc4`。
- 给队友的数据集压缩包：
  - 已将原 `/home/kazeform/2026esp/MyAlbums_dataset_20260704.zip` 大包删除，改为 1G 以内分包：
    - `/home/kazeform/2026esp/datasets_001.zip`
    - `/home/kazeform/2026esp/datasets_002.zip`
    - `/home/kazeform/2026esp/datasets_003.zip`
    - `/home/kazeform/2026esp/datasets_004.zip`
  - 四个分包 `zip -T` 校验通过，合计 jpg 数量为 `978`。
  - `MyAlbums/.git` 已按要求删除，避免额外占用空间。

## Hardware

- 目标芯片：ESP32-P4
- 显示：1024x600 MIPI DSI/DPI LCD
- 当前显示诊断状态：常用基准为 RGB24、DPI 40 MHz、3 framebuffer、DMA2D on；最近实验另测过 RGB565
- 相机：SC2336 MIPI CSI
- 当前相机诊断状态：sensor `RAW10_640x480_50fps`，ISP 输出 `640x480 RGB888`
- PSRAM：启动日志显示 32 MB，200 MHz
- 串口通常为 `/dev/ttyACM0`
- 当前 ESP32-P4 分拣板串口也可能为 `/dev/ttyUSB0`；2026-07-02 电机分拣迁移烧录使用 `/dev/ttyUSB0`。

## Commands

`bug_project` 构建：

```bash
cd /home/kazeform/2026esp/bug_project
idf.py build
```

`merge_project` 构建：

```bash
cd /home/kazeform/2026esp/merge_project
idf.py build
```

`merge` 构建：

```bash
cd /home/kazeform/2026esp/merge
idf.py build
```

烧录和实机监控：

```bash
cd /home/kazeform/2026esp/bug_project
idf.py flash monitor
```

`merge_project` 烧录和实机监控：

```bash
cd /home/kazeform/2026esp/merge_project
idf.py flash monitor
```

`merge` 烧录和实机监控：

```bash
cd /home/kazeform/2026esp/merge
idf.py -p /dev/ttyUSB0 flash monitor
```

`new_merge` 构建、烧录和运行期监控：

```bash
cd /home/kazeform/2026esp/new_merge
idf.py build
idf.py -p /dev/ttyUSB0 -b 921600 flash
idf.py -p /dev/ttyUSB0 monitor
```

- `new_merge` flash 可用 921600。
- `new_merge` 运行期 monitor 当前应使用默认 115200；`-b 921600 monitor` 会乱码。

`ESP32P4_Detection` 构建、烧录和运行期监控：

```bash
cd /home/kazeform/2026esp/ESP32P4_Detection
idf.py build
idf.py -p /dev/serial/by-id/usb-Silicon_Labs_CP2102N_USB_to_UART_Bridge_Controller_7ee2f3966ac3ee11be78b90f9e1b1c54-if00-port0 flash
idf.py -p /dev/serial/by-id/usb-Silicon_Labs_CP2102N_USB_to_UART_Bridge_Controller_7ee2f3966ac3ee11be78b90f9e1b1c54-if00-port0 monitor
```

- 2026-07-07 当前实测板子也会枚举为 USB Serial/JTAG：
  - `/dev/serial/by-id/usb-Espressif_USB_JTAG_serial_debug_unit_E8:F6:0A:E1:7A:D1-if00`
  - 芯片 revision：ESP32-P4 `v3.1`
- `ESP32P4_Detection` 当前 app version 由最新提交决定；2026-07-08 最新已烧录版本为 `e361f65`，端口为 USB Serial/JTAG 板子。
- `ESP32P4_Detection` 当前默认分拣调度配置集中在 `components/bsp/include/sorter_debug_config.h`：
  - 最新提交基线默认速度：A=`60%`、B=`100%`、C=`100%`。
  - 默认交接延时：`SORTER_DEFAULT_HANDOFF_DELAY_MS=100`。
  - 默认皮带超时：A=`4500ms`、B=`900ms`、C=`1300ms`。
  - 默认 lost timeout：min=`3000ms`、max=`6000ms`。
  - 引脚、传感器 active level、编码器参数也在同一个文件。
  - `components/Sorter_app/sorter_core/sorter_scheduler.c` 的 `sorter_config_default()` 从这些宏读取默认值。
- `ESP32P4_Detection` 当前启动路径在 UI 建好后立即启动 `system_monitor()`，早于 vision/UVC/Ethernet，用于 UI 和 TCP metrics 性能参数；随后启动 vision、UVC 和 Ethernet 上位机链路。当前硬件分拣启动已停用，不调用 `sorting_sim_debug_start()`，不启用电机输出，也不启用真实传感器输入。
- `ESP32P4_Detection` 当前 `main/system_init.c` 初始化内容恢复自 `4946a30 restore lvgl perf monitor overlay`，这是上一轮实机验证 Ethernet/control/image 可连接的基线。
- `ESP32P4_Detection` 当前视觉检测 miss 保持/发图 rearm 计数为 `VISION_DISPLAY_MISS_KEEP_COUNT=3`，位于 `components/vision/framework/vision_detect.c`。
- `ESP32P4_Detection` 当前发图触发逻辑位于 `components/vision/framework/vision_detect.c`：启动/检测关闭后允许发图；第一次有效命中 capture 后关闭发图闸门；必须连续 miss 3 次后才重新允许下一次 capture，避免同一个包裹因检测抖动或包裹 ID 变化重复发图。`vision_package_id` 仅用于 RTT 日志，不再决定是否发图。
- `ESP32P4_Detection` 当前未检出/视觉失败包裹分类规则：
  - 实际调度入口：`components/Sorter_app/sorter_core/sorter_scheduler.c` 的 `next_failed_class()`。
  - 调试状态显示：`components/Sorter_app/sorting_sim_control.c` 的 `failed_class_from_cursor()`。
  - 当前规则为 `CLASS1,CLASS2,CLASS3,CLASS1,CLASS2,CLASS3` 循环，即用户口径 `123123`。
- Ethernet 默认静态链路：
  - 板端：`192.168.10.2`
  - 上位机：`192.168.10.1`
  - control：TCP `5000`
  - image：TCP `5001`
  - 2026-07-08 已验证 host 监听时板端冷启动自动连接 `5000/5001`；control 通道可收到 1 秒周期 metrics 包。
  - 图像 JPEG payload 由“识别成功的新包裹”快照触发；无包裹时 image TCP 可连接但不会产生图像包。
- `sort_real_io` 任务栈放在 PSRAM，避免 Ethernet ready 后内部 RAM 紧张导致真实 IO task 创建失败。
- `sdkconfig` 中 `CONFIG_LV_USE_SYSMON=y` 保留；2026-07-07 用户要求恢复旧版性能显示，当前 `CONFIG_LV_USE_PERF_MONITOR=y` 且 `CONFIG_LV_PERF_MONITOR_ALIGN_BOTTOM_RIGHT=y`。
- 串口优先使用 `/dev/serial/by-id/` 稳定路径；当前现场可能是 CP2102N 或 Espressif USB Serial/JTAG，按实际枚举选择。monitor 使用默认 `115200`。

## 2026-07-06 Workspace Cleanup

- 当前活跃工程只保留：
  - `ESP32P4_Detection`
  - `esp32_host_no_inference`
  - `MyAlbums` 数据集
  - `docs/agent`
- 历史工程已移入 `archive_project/`，只用于参考旧代码。
- `build/`、`managed_components/`、`.codegraph/`、`.cache/`、`.qtcreator/` 等均视为可再生成内容，不需要长期保留。
- `ESP32P4_Detection(8).zip` 原包不完整：
  - `unzip` 无法读取 central directory。
  - `7z` 可恢复大部分源码，但报 `Unexpected end of archive`。
  - 当前工程是 zip 恢复内容加 `.git` 补齐的 `model/`、`sdkconfig`、`partitions.csv`。
  - 后续如果队友重发完整 zip，优先替换当前恢复包或直接同步 git。

## New Merge Motor Migration

- 2026-07-04 已将 `lasttime_merge` 电机分拣链路移植到 `new_merge`，并保留 `new_merge` 原 UI 显示内容。
- `new_merge` 是独立 git 仓库，当前关键分支：
  - `base-before-motor`: 迁移前基线。
  - `motor-two-stage`: two-stage 模型路径 + 电机分拣链路。
  - `motor-roi`: 传统 ROI 路径 + 电机分拣链路。
- 当前 `new_merge` 两个电机分支都包含：
  - `components/Sorter_app`
  - BSP 电机、编码器、分拣传感器
  - vision 分类到 sorter class 的提交链路
  - 真实硬件调试启动路径
- UI 边界：
  - 不从 `lasttime_merge` 迁移 UI 显示差异。
  - 修改前后应保持 `git diff -- components/UI components/UI/generated components/UI/sdk` 为空。
- `new_merge` 当前硬件引脚边界：
  - 2026-07-04 新板 revision 配置后，GT911 touch INT 使用 GPIO21。
  - 电机 2 PWM A 使用 GPIO32；不要再把 GT911 INT 放回 GPIO32。
  - ESP32-P4 console UART 已恢复到 UART0、115200；运行期 monitor 使用默认 115200。
  - sorter S1/S2/S3/S4 当前使用 GPIO53/GPIO23/-1/GPIO22；只停用原占用串口 GPIO38 的 S3，不把全部传感器置 `-1`。
  - GPIO8 是 touch I2C SCL，camera SCCB 复用同一条 GPIO8/7 I2C bus；不要把 sorter sensor 放到 GPIO8。
  - 当前 `sdkconfig` 支持 ESP32-P4 low revision 路径：min v0.0、max v1.99；`ESP32P4_SELECTS_REV_LESS_V3=y`。
- `new_merge` 当前验证状态：
  - `motor-two-stage` build 通过。
  - `motor-roi` build 通过。
  - `motor-roi` 已在 `/dev/ttyUSB0` flash 成功，ESP32-P4 revision v1.0，app/partition/storage hash verified。
  - `motor-roi` 运行到 `vision started`、`SORTDBG ready`、`System initialization done`，90 秒 monitor 窗口未见 panic/reboot。
- 2026-07-04 新板 revision + GPIO20/23/47/22 + GT911 INT GPIO21 验证：
  - `idf.py build` 通过。
  - `/dev/ttyACM0`、921600 flash 成功，芯片 `ESP32-P4 revision v3.1`，bootloader/partition/app/storage hash verified。
  - `idf.py -p /dev/ttyACM0 monitor` 90 秒窗口启动成功，日志显示 `Min chip rev: v3.1`、`Max chip rev: v3.99`、`System initialization done`。
  - sensor 初始化日志显示 S1=GPIO20、S2=GPIO23、S3=GPIO47、S4=GPIO22。
  - 启动时 S3 初始 `raw=1 active=1` 触发一次 `sensor_without_package`，约 6.7 秒变回 inactive；未见 panic/reboot。
- 2026-07-04 因 S1/S3 默认 ON，将 S1/S3 改到 GPIO53/GPIO45：
  - GPIO45/GPIO53 均为 ESP32-P4 input/output GPIO，工程内未见其它模块占用；GPIO53 另有 ADC2 channel 能力，不影响普通 GPIO 输入。
  - 修改前提交 20/47 基线：`d4dca6c update new board pins and revision`。
  - `idf.py build` 通过。
  - `/dev/ttyACM0`、921600 flash 成功，芯片 `ESP32-P4 revision v3.1`，bootloader/app/partition/storage hash verified。
  - 用户要求本次不运行 monitor；S1=GPIO53/S3=GPIO45 的默认电平和遮挡响应待现场观察。
- 2026-07-04 用户继续要求 S3 改到 GPIO37，串口不用：
  - 修改前提交 53/45 基线：`bad060d move sorter sensors to gpio53 and gpio45`。
  - GPIO37 原为 console UART0 TX；已将 `sdkconfig` 改为 `ESP_CONSOLE_NONE`/`CONSOLE_UART_NONE`，释放 GPIO37 给 S3。
  - 当前 sorter S1/S2/S3/S4 为 GPIO53/GPIO23/GPIO37/GPIO22。
  - `idf.py build` 通过。
  - `/dev/ttyACM0`、921600 flash 成功，芯片 `ESP32-P4 revision v3.1`，bootloader/app/partition/storage hash verified。
  - 用户要求本次不运行 monitor；S1=GPIO53/S3=GPIO37 的默认电平和遮挡响应待现场观察。
- 2026-07-05 用户打断要求恢复串口并降低 revision 支持：
  - 修改前提交 checkpoint：`9c4be17 checkpoint before revision and serial config`。
  - 只停用占用串口的 S3 GPIO38：S1/S2/S3/S4 为 GPIO53/GPIO23/-1/GPIO22。
  - `sdkconfig` / `sdkconfig.defaults` 当前为 ESP32-P4 min v0.0、max v1.99。
  - UART console 恢复为 UART0、115200，secondary USB Serial/JTAG enabled。
  - `idf.py build` 通过；app 大小 `0x4ec5e0`，factory 分区剩余约 18%。
  - 当前 USB 能看到 `10c4:ea60 CP210x`，但没有 `/dev/ttyUSB*`/`/dev/ttyACM*` 节点；未执行 flash/monitor。
- 当前真实四传感器链路未完整验证；S1/S3 实际 GPIO 需要硬件重新指定后再打开。
- 本轮按用户要求未运行 TCP 模拟测试。
- 2026-07-04 two-stage 给队友压缩包：
  - `/home/kazeform/2026esp/new_merge_motor_two_stage_20260704.tar.gz`
  - 排除 `build`、`.git`、`.codegraph`、`.cache`；保留源码、`components`、`managed_components`。
- `motor-roi` 临时开发者硬件调试页：
  - 备份点：`f792882 backup roi before temporary developer debug ui`。
  - 当前实现提交：`8da7918 hide preview while developer debug panel is open`。
  - 入口为 dashboard 右上方小 `DEV` 按钮，点击弹出硬件调试覆盖层。
  - 参考 `lasttime_my` 硬件调试页，功能包括模式/电机/传感器开关、速度/延时/超时调节、S1-S4、编码器、MTEST、CLASS 注入和活动包裹列表。
  - DEV 调试页打开时会隐藏 `scr_dashboard_cont_dashboard`，让 camera preview 绘制逻辑停止刷新 dashboard 区域，避免 preview 遮挡调试页；关闭 DEV 后恢复原隐藏状态。
  - 这是临时调试功能，不应长期混进生成 UI。清理边界：
    - 删除 `components/UI/sdk/developer_debug_ui.c`
    - 删除 `components/UI/sdk/developer_debug_ui.h`
    - 从 `components/UI/sdk/ui.c` 移除 `#include "developer_debug_ui.h"` 和 `ui_developer_debug_attach()` 调用
    - 从 `components/UI/CMakeLists.txt` 移除 `sdk/developer_debug_ui.c`
    - 如果 UI 不再调用 sorter API，则从 UI 组件依赖中移除 `Sorter_app`
- `motor-roi` ROI 画框边界：
  - 当前最多显示 1 个传统 ROI 面单框 + 1 个 logo 框。
  - logo 模型候选仍保留最多 4 个用于比较，但只将最高置信度候选写入最终检测结果，画框层因此只画最高分 logo。
  - 该限制位于 `components/vision/detector/vision_model.cpp` 的 `vision_model_run()`，不是 UI 生成内容或 `vision_draw.c` 的开关。

## New Merge TCP Image/Metrics Link

- 2026-07-05 已将 `new_merge` TCP 链路改为降低 CPU 峰值的双通道设计：
  - control/metrics/sorter protocol：TCP `192.168.10.1:5000`。
  - image：TCP `192.168.10.1:5001`。
- 2026-07-05 后续默认策略：
  - `SORTER_TCP_LINK_ENABLE` 默认值为 `1`，板端默认启动与上位机的 Ethernet TCP 通信链路。
  - `SORTER_TCP_SIM_LINE_OUTPUT_ENABLE` 默认值为 `0`，板端默认不把 20ms tick 产生的 Ethernet 模拟分拣状态线输出到上位机。
  - 默认仍接收上位机 `CONFIG`、`HW_STATUS`、time sync 等控制包；control `5000`、image `5001`、metrics/JPEG 链路保持可用。
  - 若现场需要把调度器 tick 输出的 SIM line 也推给上位机，可编译期显式启用 `SORTER_TCP_SIM_LINE_OUTPUT_ENABLE=1`。
  - TCP SIM line 发送层过滤掉 `STATUS,reason=tick/sensor1/package_new/vision`。
  - `PKG` SIM line 按 `id/belt/state/class` 去重；状态不变时最多 1 秒心跳一次，避免 20ms tick 刷屏。
- metrics：
  - 1 秒周期。
  - 读取 `system_monitor_get_metrics()` 缓存，不在 Ethernet task 内调用 `uxTaskGetSystemState()`。
  - JSON 里包含 image queue/drop/encode/send 统计，便于现场估算图片链路压力。
- image：
  - 测试阶段 5 秒一张；后续业务目标是每个包裹触发一次。
  - 默认快照尺寸 `640x375`。
  - 使用 `esp_new_jpeg` 编码 JPEG，quality 60，subsample 4:4:4。
  - image queue 深度 2；host 慢时跳过新图，避免无限排队。
  - image payload 按 8KB chunk 发送并 yield。
  - JPEG 包头 `reserved` 携带快递类别：`1=极兔`、`2=中通`、`3=韵达`。
  - JPEG 包头 `reserved2` 低 8 bit 携带类别置信度百分比 `0..100`。
  - 未识别或无目标时按 `极兔` 兼容显示，置信度为 `0`。
- vision/Ethernet 边界：
  - Ethernet 不直接调用 `cam_sensor_get_frame()`。
  - 通过 `vision_copy_latest_frame_scaled_rgb888()` 从 vision ring 最新帧生成 PPA 缩放快照。
  - 快照接口在 PPA 写完后做 M2C cache invalidate，确保 JPEG 编码读取的是新图。
  - 通过 `vision_get_latest_classification()` 读取最新分类快照，不在 Ethernet task 内跑推理。
- 已知验证状态：
  - 2026-07-05 低 revision ESP32-P4 v1.0 实机验证通过。
  - 当前已验证提交：`14ff3a2 move jpeg producer off busy core`。
  - `idf.py build` 通过，app 大小约 `0x4ec6c0`，factory 分区剩余约 18%。
  - `idf.py flash` 全量烧录 `12ec1cf` 成功，`idf.py app-flash` 烧录 `14ff3a2` 成功。
  - monitor 确认 app min/max chip rev 为 `v0.0` / `v1.99`，芯片为 `v1.0`，UART0 115200 正常。
  - sensors：S1=GPIO53、S2=GPIO23、S3 disabled、S4=GPIO22。
  - TCP control/image 均连接上位机 `192.168.10.1:5000/5001`。
  - telemetry 每秒更新；实测 `image_encoded=15`、`image_sent=15`、drop/fail 均 0，JPEG 约 39 KB，encode 约 332-367 ms，send 约 8-19 ms。
  - 上位机最新 preview 文件：`~/Documents/ESP32Host/images/latest_preview.jpg`。
- 2026-07-05 默认 TCP 策略修正验证：
  - `motor-roi` flash 新固件成功，ESP32-P4 revision `v1.0`。
  - monitor 120 秒窗口启动到 `System initialization done`，未见 panic/reboot。
  - 上位机 offscreen 监听 `192.168.10.1:5000/5001` 后，`ss` 确认 control/image 两条连接均 `ESTABLISHED`。
  - `~/Documents/ESP32Host/images/latest_preview.jpg` 更新时间到 `2026-07-05 19:39:56 +0800`，JPEG 接收正常。
- 2026-07-05 低 revision 实机修正：
  - 普通 `xTaskCreatePinnedToCore` 分配 TCP task stack 会因内部 RAM 紧张失败；TCP task stack 改用 `xTaskCreatePinnedToCoreWithCaps(..., MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT)`。
  - ESP-IDF 5.5 的 M2C cache sync 不允许 `ESP_CACHE_MSYNC_FLAG_UNALIGNED`；snapshot buffer 需 cache-line 对齐后用纯 `ESP_CACHE_MSYNC_FLAG_DIR_M2C`。
  - CPU1 长期接近 100%；JPEG producer 不能 pin 到 core1，已移到 core0、priority 3。
- 2026-07-05 已同步到 `motor-two-stage`：
  - cherry-pick `9e4b23d ceae9b5 50f132a 12ec1cf 14ff3a2` 无冲突。
  - 后续继续同步 `2dcfd7f 58a5143` 并补 S1=GPIO53，当前最新提交：`a390dc3 sync two-stage sorter s1 pin`。
  - 当前 two-stage 已包含默认开启上位机 TCP、默认关闭 SIM tick 输出、JPEG 分类元数据、S1=53/S2=23/S3=-1/S4=22。
  - `idf.py build` 通过，app 大小 `0x4ecd30`，factory 分区剩余约 18%。
  - `/dev/serial/by-id/usb-Silicon_Labs_CP2102N_USB_to_UART_Bridge_Controller_7ee2f3966ac3ee11be78b90f9e1b1c54-if00-port0` 完整 flash 成功，ESP32-P4 revision `v1.0`。
  - S1 同步后 `app-flash` 成功；monitor 确认 app version `a390dc3`，min/max chip rev `v0.0/v1.99`，UART0 115200，S1=GPIO53、S2=GPIO23、S3 disabled、S4=GPIO22，启动到 `System initialization done`。
  - monitor 时未启动上位机，因此 control/image connect failed 为预期重连日志。
- 2026-07-05 two-stage + 上位机交接包：
  - `/home/kazeform/2026esp/two_stage_host_handoff_20260705.tar.gz`
  - `/home/kazeform/2026esp/two_stage_host_handoff_20260705.tar.gz.sha256`
  - SHA256: `af66e056a34f3aac8d1e09d0a97e95eaf16ca622fed8da84db960d3d04088fd6`。
  - 包含最新 `new_merge_motor_two_stage`、`esp32_host_no_inference`、差异报告、ESP 移植指南、host 交接说明和两份完整 patch；排除 `.git`、`build`、`.codegraph`、`.qtcreator` 和本地调试帧。

## ESP32 Host No Inference

- `/home/kazeform/2026esp/esp32_host_no_inference` 已初始化独立 git 仓库。
- 当前分支：`tcp-board-align`。
- 提交：
  - `f9ae6d9 baseline host no inference`
  - `2ad49e8 align host tcp protocol with board`
- 协议边界：
  - `0x10`：TimeSync JSON。
  - `0x12`：SIM line，不再作为 inference result JSON。
  - 不发送板端不解析的 `0x11 ControlJson/upload_format`。
  - image socket 固定 `5001`，packet type `0x01`，pixel format `2` JPEG。
  - JPEG 包头 `reserved/reserved2` 解析为快递类别和置信度；旧固件或未知值按 `极兔`、`0%` 处理。
- 2026-07-05 上位机 UI 对齐边界：
  - 参考 `new_merge` 板端普通 dashboard/settings 页面，不镜像临时 DEV/debug 页面。
  - 保留臃肿信息面板风格，删除本地模型服务/推理工作台页面。
  - 不再注入离线假数据；无设备时显示待连接/空状态，监听服务仍自动启动。
  - 首页不显示实时事件流；运行日志归入系统维护页。
  - 系统维护页替代原预留页，展示监听、端口、保存目录、遥测时间、运行日志和图片链路健康。
  - 首页使用业务统计卡：总包裹数、极兔、中通、韵达；队列、丢弃、无帧、最新体积等链路诊断项只放在系统维护页。
  - 系统维护页链路端口布局为控制/图像端口并排，板端地址和上位机地址独占整行。
  - 图片预览页默认显示最新板端 JPEG；取消 10 帧节流，收到 `5001` 图片即更新 `latest_preview.jpg`。
  - 接收图片历史不依赖 detection JSON；每张图片保存为独立 `frame_%06u.jpg` 并插入历史列表，同时覆盖 `latest_preview.jpg`。
  - 上位机显示标签使用本地接收顺序 `包裹#N`，不要在 UI 显示“图片帧/画质增强”等实现细节。
  - 视觉页主图和历史记录显示快递类别与置信度，不显示图片分辨率和检测目标数量；默认每张 JPEG 预览对应一个包裹。
  - 快递统计按收到的 JPEG 图片记录计数，不按真实包裹 ID；无法分类按极兔统计。
  - 图片链路统计中不展示“发送失败”卡片，改为“快递统计”。
  - host 展示图会做本地画质增强：3x3 中值去椒盐/彩色孤立点 + 轻微锐化，再以 JPEG quality 88 保存。
  - 控制页保留屏幕亮度、置信度阈值、检测开关、预览叠加开关为本地 UI 状态。
  - 新增电机调速，发送 `CONFIG a_speed=<v> b_speed=<v> c_speed=<v>` 到板端。
  - 不显示或控制板端 DEV 页的 S1-S4、编码器、MTEST、包裹注入、`ENC_CLEAR`、专用 `HW_STATUS` 面板。
  - metrics 固定 `0x02`。
- 控制映射：
  - `motor_speed` -> `CONFIG a_speed=<v> b_speed=<v> c_speed=<v>`。
  - `conveyor=true` -> `CONFIG mode=real motor_output=1 sensor_input=1`。
  - `conveyor=false` -> `CONFIG motor_output=0 sensor_input=0`。
  - `auto_run=true` -> `CONFIG mode=real sensor_input=1 motor_output=1`。
  - `auto_run=false` -> `CONFIG mode=timed sensor_input=0 motor_output=0`。
  - `brightness`、`fill_light`、`danger_threshold` 仅本地 UI 状态。

## Merge Sorter Migration Baseline

- 2026-07-02 已将 `old_project` 电机分拣链路第一轮移植到 `merge`。
- 迁移内容：
  - `components/Sorter_app`：分拣调度器和 `sorting_sim_control`。
  - `components/bsp`：电机、编码器、分拣传感器和 sorter debug config。
  - `components/Ethernet_app`：TCP 模拟链路。
  - `components/vision/framework/vision_detect.c`：LOGO 分类 0/1/2 提交 sorter class1/2/3。
  - `main/system_init.c`：启动 Ethernet sorter link、debug 任务并启用 motor output。
- `merge` 默认 Ethernet 模拟目标：
  - 板端 IP：`192.168.10.2`
  - 上位机/网关：`192.168.10.1`
  - TCP port：`5000`
- TCP 20 件验证命令：

```bash
cd /home/kazeform/2026esp
python -m esp32_sorter_sim_py --headless --transport tcp --host 192.168.10.1 --port 5000 --count 20 --class-sequence 1,2,3,2,3,1 --feed-interval 0.25 --max-active 8 --timeout 180 --stall-timeout 18 --b-idle-threshold 3 --log-file esp32_sorter_sim_py/logs/merge_tcp_migration_20_rev0_20260702.log
python -m esp32_sorter_sim_py.log_audit esp32_sorter_sim_py/logs/merge_tcp_migration_20_rev0_20260702.log
```

- 2026-07-02 迁移验收结果：
  - build 成功。
  - `/dev/ttyUSB0` flash/monitor 启动成功。
  - TCP 20 件 `RESULT ok completed=20/20`。
  - audit: warnings=0, drops=0, desyncs=0, faults=0。
  - done: class1=7, class2=7, class3=6。
- 已知边界：
  - 默认模拟链路会接受 TCP `VISION_FRAME`、`VISION_RESULT`、`SENSOR`、`DISTANCE`；真实 IO 模式需配置切换。
  - 编码器仍沿用 `old_project` 虚拟配置；若真实 C 段需要距离闭环，后续需补实际 encoder GPIO。
  - boot log 提示 GPIO37/GPIO38 用于 console UART，而 sorter S1/S3 默认也使用 GPIO37/GPIO38；真实传感器联调时需确认是否冲突。

## Merge Findlogo Cascade

- 2026-07-03 `merge` 接入视觉级联检测。
- 2026-07-03 后续将第一阶段从 waybill 模型改为传统算法 ROI：
  - 第一阶段：`components/vision/detector/roi_algorithm.c` 整帧定位面单 ROI。
  - 第二阶段：`model/findlogo.espdl` 在面单 ROI 内检测快递 logo。
- `det_pico_224_224_waybill.espdl` 当前保留在 `model/` 便于回滚，但运行时不再加载。
- `findlogo.espdl` 模型来源：
  - SHA256: `3c23a1ae917adba01020e42f9ae4cfaacc6f6999649fd8231f06488f3ec41477`
  - 与 `/home/kazeform/2026upper/esp-detection/model/best/datasets5000_kl_MOSIC_NOINT16.espdl` 一致。
  - 文件名保持 `findlogo.espdl`，因为 SPIFFS 当前文件名长度受 `CONFIG_SPIFFS_OBJ_NAME_LEN=32` 约束。
- logo 训练类别顺序来自 `/home/kazeform/2026upper/esp-detection/cfg/logo.yaml`：
  - `cat0 = jt = 极兔`
  - `cat1 = yd = 韵达`
  - `cat2 = zt = 中通`
- 对外概率接口保持 `vision_model_get_class_probs(int *jt, int *zt, int *yd)` 参数顺序不变；内部归属为 `cat0->jt, cat1->yd, cat2->zt`。
- 物理分拣出口保留旧语义：
  - `JT -> CLASS1`
  - `ZT -> CLASS2`
  - `YD -> CLASS3`
  - 因训练顺序为 `jt,yd,zt`，代码映射为 `cat0->CLASS1, cat1->CLASS3, cat2->CLASS2`。
- RGB/BGR 约定：
  - 模型导出链路期望 RGB。
  - 当前相机帧和 ROI 源按 `DL_IMAGE_PIX_TYPE_BGR888` 声明。
  - ESP-DL `ImagePreprocessor` 负责 BGR 到 RGB 的转换；不要额外手写通道交换，除非后续实测推翻源格式声明。
- 传统 ROI 和 ROI 校准：
  - `roi_algorithm_detect()` 使用亮度、近似饱和度、RGB 通道差和最小通道阈值找最大白色连通区域。
  - `roi_tuning.cpp` 维护运行时阈值，左上角 LOGO 按钮触发 `roi_tuning_request_calibration()`，由下一帧执行校准。
  - 原左上角 LOGO 按钮触发抓帧 dump 的功能已停用；`vision_frame_dump_request()` 代码保留但不绑定该按钮。
- 依赖约束：
  - 原锁定 `esp-dl 3.3.2` 可完成 build，但实机加载 `findlogo.espdl` 时在 `fbs::FbsModel::get_operation_parameter(...)` 触发 Load access fault / Guru Meditation。
  - 当前保留 `esp-dl 3.3.6` 是基于实机失败证据，不是无证据升级。
- 运行日志验收关键点：
  - SPIFFS mounted。
  - findlogo 模型 `inputs=1 outputs=6`。
  - findlogo score 输出三尺度、每尺度 3 类：`1x28x28x3`、`1x14x14x3`、`1x7x7x3`。
  - 无 LoadProhibited / Guru Meditation。
- 2026-07-03 传统 ROI 级联验证：
  - 备份提交：`1f3fb87 checkpoint before traditional roi cascade`。
  - `idf.py build` 成功。
  - `/dev/ttyUSB0` 不存在，改用 `/dev/ttyACM0`。
  - `idf.py -p /dev/ttyACM0 -b 921600 flash` 成功，芯片 `ESP32-P4 revision v3.1`，app/partition/storage hash verified。
  - `idf.py -p /dev/ttyACM0 monitor` 启动到 `vision started`、SORTDBG、电机/传感器初始化，无 Guru Meditation。
  - 当前 monitor 会持续刷 `ISP_AWB: subwindow size ...` warning；这是现有相机 ISP 日志噪声，不是 ROI 接入本身的 panic。
- 队友合并报告：
  - `merge/docs/findlogo_merge_report.md`
- 2026-07-03 TCP 20 包分拣回归：
  - 日志：`esp32_sorter_sim_py/logs/merge_findlogo_tcp_20_20260703.log`
  - `RESULT ok completed=20/20`
  - `audit_status=ok`
  - `warnings=0,drops=0,pose_asserts=0,desyncs=0,faults=0`
  - 完成分布：`class1=7,class2=7,class3=6`
- 烧录可用 `idf.py -p /dev/ttyUSB0 -b 921600 flash`；运行期 monitor 可读波特率为 115200，`-b 921600 monitor` 会输出乱码。
- 2026-07-04 当前 `merge` 已按旧板重新收口：
  - 最新阶段提交：`84c9c72 fix ui monitor and pin merge dependencies`。
  - 实机烧录识别芯片 `ESP32-P4 revision v1.0`。
  - 启动日志确认 `efuse_init: Min chip rev: v0.0`、`Max chip rev: v1.99`、`cpu freq: 360000000 Hz`。
  - `idf.py build` 成功。
  - `idf.py -p /dev/ttyUSB0 -b 921600 flash` 成功，app/partition/storage hash verified。
  - `system_monitor()` 在硬件调试模式下也启动，UI CPU/内存/运行时间恢复刷新。
  - `sysmon` 串口 CPU INFO 表关闭，`ISP_AWB` warning 降到 ERROR，monitor 不再被该 warning 刷屏。
- 2026-07-04 `merge` 当前依赖收窄：
  - 保留 `esp-dl 3.3.6`，因为 `findlogo.espdl` 在 `esp-dl 3.3.2` 上实机加载崩溃。
  - 必要新依赖：`dl_fft 0.4.0`、`esp_new_jpeg 1.0.2`、`esp_lv_decoder 0.4.3`。
  - 无关升级收回到基线：`esp_cam_sensor 2.2.0`、`esp_video 2.2.0`、`esp_ipa 2.1.0`、`usb_host_uvc 2.5.0`、`esp_lcd_ek79007 1.0.4`、`esp_lvgl_adapter 0.4.3`。

## Merge Project Sensor Chain

- `merge_project` 分拣传感器硬件映射位于 `components/bsp/include/sorter_debug_config.h`：
  - S1 GPIO37, S2 GPIO23, S3 GPIO38, S4 GPIO22。
  - 当前四路 active level 均为 1。
- 传感器 BSP 初始化后将 S1-S4 配为 GPIO input，下拉开启；未配置 GPIO 为 -1 时视为 disabled。
- `bsp_sort_sensor_get_level()` 返回 GPIO 原始电平和按 active level 换算后的 active 状态；`bsp_sort_sensor_get_state()` 保持兼容，只返回 active。
- `sorting_sim_control.c` 的 `real_io_task` 每 10 ms 轮询 S1-S4，20 ms 防抖，稳定跳变后才送入调度器。
- 实时日志关键字：
  - `sort sensor Sx init raw_level=... active=... valid=...`
  - `sort sensor Sx raw change raw_level=... active=... stable=... valid=...`
  - `sort sensor Sx stable change raw_level=... active=... debounce_ms=20`
- 当前 `merge` 生成 UI 未接入 S1-S4 传感器状态块；S1-S4 仍通过日志、TCP/串口 `HW_STATUS` 或后续 UI 接口查看。

## Merge Project Sorter Control Link

- 真实硬件链路和以太网模拟链路的共同核心是：
  - `components/Sorter_app/sorting_sim_control.c`
  - `components/Sorter_app/sorter_core/sorter_scheduler.c`
- 2026-07-03 电机算法结构优化后：
  - scheduler 内部通过 typed event 表达 `MOTOR/STATUS/PKG/FAULT`。
  - 外部 TCP/debug 文本协议保持兼容，仍由 `sorter_protocol_format_event()` 输出原格式。
  - 本地电机输出直接处理 `SORTER_EVENT_MOTOR`，不再解析本机生成的 `MOTOR,...` 字符串。
  - 已删除未使用的 `SORTER_STATE_WAITING_BC`、`b_center_to_exit_mm`、`transfer_timeout_mm`、`max_packages`。
  - 第一轮优化未改电机速度、超时、交接、路由、传感器语义。
  - 优化报告：`merge/docs/motor_algorithm_review.md`。
  - 优化前后 TCP 20 包均通过：`audit_status=ok`、`completed=20/20`、`class1=7,class2=7,class3=6`。
- 2026-07-03 后续默认速度调整：
  - `sorter_config_default()` 中 A/B/C 三个电机默认速度统一为 `60%`。
  - TCP/串口 `CONFIG` 仍可通过 `a_speed`、`b_speed`、`c_speed` 实时覆盖。
  - 上位机模拟器启动会发送 `CONFIG`；验证 60% 路径时需加 `--motor-a-speed 60 --motor-b-speed 60 --motor-c-speed 60`。
  - 60% TCP 20 包日志：`esp32_sorter_sim_py/logs/merge_motor_speed60_tcp_20_20260703.log`。
  - 60% 回归结果：`audit_status=ok`、`RESULT ok completed=20/20`、`warnings=0,drops=0,pose_asserts=0,desyncs=0,faults=0`、完成分布 `class1=7,class2=7,class3=6`。
- 真实链路：
  - `real_io_task()` 每 10 ms 读取 S1-S4，20 ms 防抖后调用 `sorter_scheduler_sensor()`。
  - S1 触发后打开视觉窗口；`vision_app.cpp` 检测到目标时调用 `sorting_sim_control_submit_vision_class()`。
  - 若无检测结果，S2 到达或视觉状态超时会触发识别失败处理。
- 以太网模拟链路：
  - `VISION_FRAME` 可同时模拟 S1 和分类。
  - `VISION_RESULT` 可按 package id 或当前真实视觉窗口提交分类。
  - `SENSOR` 模拟 S2-S4 事件，`DISTANCE` 模拟编码器距离。
- 对照风险：
  - 上位机模拟若发送 package id，会比真实传感器更理想；真实传感器事件当前以 `package_id=0` 进入调度器，由调度器选择最早候选包裹。
  - 编码器未接时，真实链路不会可靠调用 `sorter_scheduler_distance()`；C 段完成依赖 `c_fallback_busy_ms` 超时。以太网模拟若发送 `DISTANCE`，不能代表该硬件状态。
  - 上位机模拟没有真实 GPIO 电平、active level、线序、防抖和推理延迟风险；实机仍需用 `sort sensor Sx ...`、`PKG`、`MOTOR` 日志验证。
- 识别失败分配策略：
  - `SORTER_CLASS_VISION_FAILED` 和直接提交的 `SORTER_CLASS_UNKNOWN/class=none` 进入调度器时按 class1、class2、class3、class1... 轮番分配。
  - 视觉超时 `timeout_vision` 使用同一个轮转游标。
- UI/调试边界：
  - 当前 `merge` 生成 UI 保留视觉 dashboard、日志、设置、系统页。
  - 已对 UI 公司名和 sorter 提交映射做 findlogo 类别顺序适配：`cat0=极兔/JT`、`cat1=韵达/YD`、`cat2=中通/ZT`，物理出口仍为 `JT->CLASS1`、`ZT->CLASS2`、`YD->CLASS3`。
  - 旧工程的分拣手动控制面板未整体迁移；当前屏幕没有直接 CLASS 注入、`MOTOR_TEST`、编码器清零、S1-S4 状态、活动包裹列表或 per-belt 速度/超时编辑控件。
  - `sorting_sim_control_get_settings()`、`sorting_sim_control_apply_settings()`、`sorting_sim_control_get_runtime_debug()`、`sorting_sim_control_get_hardware_status()` 是后续 UI 接入可用的 C API；当前生成 UI 没有调用这些接口。
  - 当前运行时分拣调参主要通过 TCP 或 USB 串口命令完成。

## Blue-Screen Long-Term Findings

- 蓝屏问题当前最像 LCD DSI/DPI scanout starvation，而不是应用层画蓝或 framebuffer 被写坏。
- framebuffer sampled hash 在多次 `BLUE MARK` 前保持稳定，是当前最关键的反证：屏幕蓝色不是 framebuffer 内容先变蓝。
- camera/CSI/ISP 写 PSRAM 是必要触发因素；preview 和 YOLO 不是必要条件，但可能放大问题。
- 只让应用少取帧不等于硬件少写 PSRAM；必须确认 camera stream/CSI/ISP 实际写入降低。
- 只降低平均 duty/FPS 不一定解决单帧突发导致的最坏延迟。
- DMA2D off 已测更差，当前保持 DMA2D on，不再作为主修复方向。
- 当前最值得押注的修复方向是降低 LCD framebuffer scanout 带宽，优先 LCD RGB565；其次降低 camera/ISP 单帧写入突发。
- 2026-06-27 最近两轮 RGB565/仲裁实验均有用户肉眼观察到的蓝屏，但日志未可靠出现对应 `BLUE_MARK`/underrun；因此“没有 `BLUE_MARK`”不能判定“没有蓝屏”。
- RGB565 确认能把 LCD active_read 从约 `83608 KB/s` 降到约 `55738 KB/s`，但最近实机观察显示它没有单独消除可见蓝屏。
- LCD DW-GDMA priority 提到 3 后，配合 RGB565 仍出现可见蓝屏；CSI priority 降到 0 会导致 CSI queue full 和 camera frame timeout，不适合作为候选修复。
- 2026-06-28 在带大号 `BLUE_MARK` 的 `78ec4d9` 基础上增加 PSRAM/ICM 仲裁诊断：
  - `display-first` 回读稳定，约 170 秒内 camera `timeouts=0`，未见 `underrun happens`。
  - `display-first + PSRAM read-first` 会在约 16 秒后触发连续 CSI queue full，camera timeout 快速增长，不适合作为候选修复。
  - `display-first + camera write regulator` 约 110 秒内 camera `timeouts=0`、`max_receive=39ms`，未见 CSI queue full 或 underrun，但用户肉眼观察当前板上仍蓝屏很多；因此它不是屏幕稳定修复，只说明 camera/CSI 没有被该配置弄坏。
- 本轮 arbiter 矩阵关闭了 `CONFIG_VISION_APP_ENABLE_PREVIEW_IMAGE`，preview 黑屏/不更新是预期现象；这些版本只能用于带宽/仲裁诊断，不能当作完整功能验证。
- 2026-06-28 Phase 1 之后，已补充 `[PIPE_DIAG]`、`[PPA_TRACE]`、frame id 和 BLUE_MARK pipeline dump；后续实机蓝屏测试应优先使用这些版本或后续派生版本。
- B0 natural (`8ed68c2`) 打开 preview 后，preview 正常更新，但 ROI 一直 skip，`model_runs=0`，不能代表完整 inference 负载。
- B0 force (`a28948c`) 强制 ROI skip 时也跑 full-frame inference，建立了 RGB24 + preview + forced inference 基线；camera/CSI 稳定、未见 underrun，但仍有用户可见蓝屏/BLUE_MARK。
- B1 RGB565 force (`6422f39`) 确认整条 LCD 路径进入 RGB565：`fb_size=1200KB`，`active_read=55738KB/s`，相对 RGB24 `83608KB/s` 下降约 1/3；但约 170 秒内仍有 3 次用户可见蓝屏/BLUE_MARK，因此 RGB565 不是单独充分修复。
- B1 与 B0-force 不是纯 LCD-only 运行时对照：B0-force 首次 multi-core model run `1264ms` 后 fallback 到 single-core、约 `1.5-1.6 fps`；B1 保持 multi-core、约 `5 fps`。这个差异必须在后续分析中作为事实记录，不能忽略。
- B0-force 和 B1 都显示 camera 稳定：`timeouts=0`，无 CSI queue full。当前更像 display/LCD 可见稳定性问题仍未解决，而不是 camera receive 已失稳。
- Preview 需要分三层记录：控件初始化、preview update/PPA 性能链路执行、LCD 上视觉 preview 是否真实有效。`preview initialized` 只证明控件和清零 buffer 创建成功；`preview_ok>0`/`preview_ppa>0` 只证明性能链路在跑，不证明屏幕上是真实摄像头画面。
- 用户现场观察到 preview 区域黑、白、黑白参半；这不是正常产品 preview。B0/B1 只能记录为 `preview update/load active, visual preview not validated`，不能称为完整功能 preview 正常。
- P0 no-preview (`ea24f0f`) 明确关闭 `CONFIG_VISION_APP_ENABLE_PREVIEW_IMAGE`，全程 `preview=0/0 preview_ppa=0 images=0`，但 35 秒内仍有 3 次 BLUE_MARK；因此 preview update/PPA 不是蓝屏必要触发因素。
- Y0 preview-load no-inference (`3e87962`) 明确关闭 YOLO/model，保留 camera + preview update/PPA + LCD RGB565；#1/#2/#3 BLUE_MARK 时 `model_runs=0 yolo_ppa=0 camera_timeouts=0 preview_ok>0 preview_ppa>0`，因此 YOLO/model 不是蓝屏必要触发因素。
- Y0 约 93 秒后开始出现 CSI queue full 和 camera timeout，#4 已经属于 camera/CSI 失稳后的蓝屏；分析时要把 Y0 前三次和后段失稳分开记录。
- C0 RGB565 camera-off (`f1f95e4`) 关闭 camera stream、preview、inference 后，约 90 秒 monitor 内无 BLUE_MARK、无 CSI queue full、无 camera timeout、无实际 LCD `underrun happens`。
- C1 RGB565 camera half-duty (`9ba705c`) 仅开启 camera stream，preview/inference/PPA 均关闭，`active_write=1800KB/s`；约 35 秒出现 BLUE_MARK #1，且当时 `camera_timeouts=0 model_runs=0 preview_ppa=0`。约 82 秒后才开始 CSI queue full / camera timeout。
- C0/C1 对照支持 camera/CSI/ISP 写 PSRAM 是当前最小必要触发方向；C1 同时说明仅降低平均 camera write 到 `1800KB/s` 仍不足，后续继续测试更低 duty 或降低单帧输出带宽。
- C1b RGB565 camera duty 800 (`c990bdf`) 仅开启 camera stream，preview/inference/PPA 均关闭，`active_write=900KB/s`；约 168 秒内 4 次 BLUE_MARK，所有 BLUE_MARK 均 `camera_timeouts=0 model_runs=0 preview_ppa=0`，停止前仍无 CSI queue full / camera timeout / 实际 underrun。因此继续只拉长 camera duty 收益有限，下一步优先降低 camera 单帧输出带宽或分辨率。
- C2 RGB565 camera output duty 800 (`f2ac998`) 仅开启 camera stream，preview/inference/PPA 均关闭，camera output 改为 RGB565，`active_write=600KB/s`、`600KB/frame`；约 32.2 秒 BLUE_MARK #1，且当时 `camera_timeouts=0 model_runs=0 preview_ppa=0`。约 229.1 秒后才开始 CSI queue full / camera timeout。因此把 camera 单帧写入从 `900KB/frame` 降到 `600KB/frame` 仍不足以消除蓝屏。
- C3 RGB565 camera output duty 1600 (`7e03fda`) 仅开启 camera stream，preview/inference/PPA 均关闭，camera output RGB565，`period=1600ms duty=1600ms`；约 128 秒内 3 次 BLUE_MARK。所有 BLUE_MARK 时 cam_sensor 层 `timeouts=0`，无 CSI queue full，PPA recent count=0；但 vision_app 因取帧等待短于 duty 产生周期性 `camera frame timeout`。因此继续降低 duty 仍不能消除蓝屏，且要区分 vision_app 取帧 timeout 与 cam_sensor/CSI timeout。
- C4 camera stream only no vision task (`037f341`) 保留 camera stream、camera output RGB565、`period=1600ms duty=1600ms`，但不创建 vision task、不调用 `cam_sensor_get_frame()`。启动后确认前 5 个 capture `timeouts=0`，随后约 90 秒 monitor 窗口无 BLUE_MARK、无 CSI queue full、无实际 underrun。该结果提示应用层取帧循环/等待/调度可能参与触发，但 C4 缺少周期 heartbeat，需要后续补充验证。
- C5 vision task timeout 2500 (`b2b1ca5`) 恢复 vision task 和 `cam_sensor_get_frame()`/`cam_sensor_return_frame()` 循环，将取帧 timeout 提高到 2500ms，大于 `1600ms` duty。约 137 秒 monitor 内 2 次 BLUE_MARK，且全程 `camera_timeouts=0`、cam_sensor `timeouts=0`、preview/inference/PPA 均关闭、无 CSI queue full、无实际 underrun。C5 消除了 C3 的应用层 timeout 副作用但仍复现，说明 C4/C5 的关键差异更可能在应用层取帧/还帧循环或相关调度/记录。
- C4b camera stream only heartbeat (`8bcdd70`) 无 vision task、无 `get_frame`，但 1s camera heartbeat 确认 capture count 从 1 持续增长到 78；约 130 秒内出现 1 次 BLUE_MARK，且当时 cam_sensor `timeouts=0`、无 CSI queue full、无 preview/inference/PPA、无实际 underrun。因此 C4 的“无蓝”不能解释为 camera 停止，camera/CSI/ISP stream/write 本身仍是强相关触发方向。
- C6a vision task no get_frame (`88cebbc`) 保留 vision task，但完全不调用 `cam_sensor_get_frame()`；约 143 秒内出现 2 次 BLUE_MARK，frame 始终为 0，BLUE_MARK 时 `camera_timeouts=0`、cam_sensor `timeouts=0`、无 preview/inference/PPA、无 CSI queue full、无实际 underrun。因此 `cam_sensor_get_frame()` / `cam_sensor_return_frame()` 不是蓝屏必要条件。
- C6b get return only (`7b8fc2f`) 调用 `cam_sensor_get_frame()` 后立即 `cam_sensor_return_frame()`，不读内容、不 post UI、不做 per-frame pipeline record、不做 PPA/YOLO/preview；camera 启动后只成功返回 3 帧，约 11.45 秒后开始持续 CSI queue full / camera timeout。可计入的 BLUE_MARK 发生在 camera/CSI 已失稳之后，因此不能作为 get/return 本身足以触发蓝屏的干净证据，但证明 640x480 RGB565 get-return-only 路径会快速破坏 camera/CSI。

## Blue-Screen Diagnostic Signals

关键日志：

```text
[BLUE_MARK] ...
[BLUE_MARK] fb_hash ...
[LCD_DIAG] ...
[CAM_DIAG] ...
[VISION_DIAG] ...
can't fetch data from external memory fast enough, underrun happens
```

判定规则：

- 确认级：`BLUE_MARK` 前后约 0-1500 ms 内出现 `underrun happens`，判定为 LCD DSI/DPI underrun。
- 高概率级：没有 underrun 日志，但 mark 前后 camera stream active，且 framebuffer hash 稳定，同时 camera off 组稳定，判定为 camera 写突发诱发显示链路 starvation。
- 反证级：mark 前 framebuffer hash 先异常突变，或 camera buffer 与 LCD framebuffer 地址重叠，转向查 memory overwrite、cache sync 或地址生命周期。
- 日志链路风险：用户肉眼看到蓝屏但没有对应 `BLUE_MARK`/underrun 时，以肉眼现象为准；必须补充独立的可见状态记录或重新验证按钮/日志链路，不能只用 marker 缺失作为稳定结论。
- Ethernet 不是蓝屏的必要触发条件：用户已观察到“关闭以太网但 camera/preview/inference 全开”仍会蓝屏。
- Ethernet 仍可能是放大因素：应用层 TCP 图像发送会额外 `cam_sensor_get_frame()`，再把整帧复制到 PSRAM 后发送；这会制造 camera frame 竞争和 PSRAM 大块读写突发。
- Ethernet driver 本身主要消耗 EMAC DMA buffer、esp-netif/LwIP 任务和中断负载；当前配置未启用 `SPIRAM_TRY_ALLOCATE_WIFI_LWIP`，因此 LwIP/EMAC 小 buffer 不应是主要 PSRAM 占用来源。
- `merge_project` 的 Ethernet 改善线索更像应用层节流：图像/metrics 发送周期从 10 秒变为 300 秒，而不是 ETH DMA/LwIP 基础配置变化。

## RGB565 Pitfalls

RGB565 必须确认整条显示路径都变成 16 bpp：

- `esp_lcd_dpi_panel_config_t.in_color_format = LCD_COLOR_FMT_RGB565`
- 若显式配置 `out_color_format`，也确认 RGB565 或默认等于 input
- `esp_lcd_panel_dev_config_t.bits_per_pixel = 16`
- panel init 的像素格式命令与 16 bpp 匹配
- LVGL `LV_COLOR_DEPTH=16`
- draw buffer、image buffer 按 2 bytes/pixel
- `[LCD_DIAG]` 打印 bpp=16，fb size 约 `1024*600*2 = 1228800 bytes`
- active_read 从当前约 `83608 KB/s` 降到约 `55739 KB/s`

如果 RGB565 后 `[LCD_DIAG] active_read` 没有接近 2/3，说明没有真正降低 framebuffer scanout 带宽。

## Archived Docs

- 旧 sorter hardware 文档已归档到 `docs/agent/archive/2026-06-27-sorter-hardware-changes.md`
- 旧 simulator 文档已归档到 `docs/agent/archive/2026-06-27-sorter-simulator-changes.md`
- 原蓝屏长文档已归档到 `docs/agent/archive/2026-06-27-blue-screen-debug-plan.md`
- 2026-06-28 蓝屏/相机链路诊断阶段已归档到 `docs/agent/archive/2026-06-29-blue-screen-camera-diagnostics.md`
- 2026-06-29 至 2026-07-02 `merge_project` 分拣传感器、失败分类和运行态 debug 显示阶段已归档到 `docs/agent/archive/2026-07-02-merge-runtime-debug-stage.md`
