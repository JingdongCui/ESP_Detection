# History

## 2026-07-08 ESP32P4_Detection snapshot rearm after three misses

- 用户要求修改发图逻辑：“改成 3 次 miss 允许发图，避免同一个包裹重复发”。
- 按项目规则先读取 `PROJECT.md`、`CURRENT.md`、`HISTORY.md`。
- 当前根目录和 ESP 工程均无 `.codegraph/`，跳过 CodeGraph。
- 修改前 ESP 工作区干净。
- 定位：
  - 发图逻辑位于 `components/vision/framework/vision_detect.c`。
  - 旧逻辑优先按 `vision_package_id` 去重：包裹 ID 变化会立即发图；无 ID 时按视觉上升沿发图。
  - 该策略在同一实物包裹窗口内如果 ID 抖动/变化，可能重复发图。
- 修改：
  - `VISION_DISPLAY_MISS_KEEP_COUNT` 从 `2` 改为 `3`。
  - 删除 `s_last_snapshot_package_id` 和按包裹 ID 变化立即发图的 gating。
  - 新增 `s_snapshot_capture_armed`：
    - 启动/检测关闭后允许发图。
    - 第一次有效命中抓图后 disarm。
    - 连续第 3 次 miss 才 rearm。
    - 连续命中、或 1-2 帧短暂 miss，不再重复抓图。
  - `vision_package_id` 仍保留用于 RTT 日志 `BOXED SNAPSHOT captured pkg=...`。
- 提交：
  - ESP 提交：`e82a832 rearm snapshots after three misses`。
- 验证：
  - `git diff --check` 通过。
  - 修改后第一次 `idf.py build` 通过。
  - 提交后再次 `idf.py build` 通过，app version 为 `e82a832`。
  - app size：`0x526c80`；factory 分区剩余：`0xd9380`，约 `14%`。
  - `idf.py -p /dev/serial/by-id/usb-Espressif_USB_JTAG_serial_debug_unit_E8:F6:0A:E1:7A:D1-if00 flash` 成功。
  - 芯片：ESP32-P4 revision `v3.1`。
  - bootloader、app、partition table、storage 全部 `Hash of data verified`。
  - TTY monitor 跑 50 秒，确认 app version `e82a832`，启动到 camera/LVGL/Ethernet image task，未见 panic/reboot。
  - monitor 中无新包裹快照时 image producer 正常输出 `JPEG snapshot skipped: ESP_ERR_NOT_FOUND`。
  - 本轮未用真实包裹实测重复发图抑制效果；需现场放包裹验证连续命中只发一次，连续 3 次 miss 后下一包重新发图。

## 2026-07-08 ESP32P4_Detection Ethernet image and metrics autostart

- 用户要求调工程：上位机自动连接，并能收到图像和性能参数，改完烧录。
- 按项目规则先读取 `PROJECT.md`、`CURRENT.md`、`HISTORY.md`。
- 当前根目录和 ESP 工程均无 `.codegraph/`，跳过 CodeGraph。
- 检查 ESP 子工程状态：
  - 分支：`feat/screen-uvc-stream`
  - 存在未提交现场差异：`components/bsp/include/sorter_debug_config.h` 中 `SORTER_DEFAULT_HANDOFF_DELAY_MS` 从 `500` 改为 `100`。
  - 按规则先提交该现场差异：`9ecbe05 checkpoint current sorter timing config`。
- 定位问题：
  - `main/system_init.c` 中 `system_monitor()` 被注释。
  - `ethernet_app_start()` 分支被 `#if 0` 屏蔽。
  - host 工程检查确认 `HostController` 构造时启动 `HostNetworkWorker` 并调用 `startServer()`；`HostNetworkWorker` 自动监听 TCP `5000/5001`，所以本轮无需改 host 代码。
- 修改：
  - `main/system_init.c` 恢复 `#if SORTER_TCP_LINK_ENABLE` 下的 `ethernet_app_start()` 和 `ethernet_app_wait_ready()`。
  - `system_monitor()` 恢复到 Ethernet ready、分拣硬件启动之后执行，并增加失败日志。
  - 保持 `sorting_sim_debug_start()`、电机输出、真实传感器输入开机启用。
- 提交：
  - ESP 提交：`92c32eb enable ethernet image and metrics autostart`。
- 构建：
  - 第一次 `idf.py build` 通过，dirty app version 为 `9ecbe05-dirty`。
  - 提交后再次 `idf.py build` 通过，app version 为 `92c32eb`。
  - app size：`0x526d80`；factory 分区剩余：`0xd9280`，约 `14%`。
- 烧录：
  - 端口：`/dev/serial/by-id/usb-Espressif_USB_JTAG_serial_debug_unit_E8:F6:0A:E1:7A:D1-if00`
  - `idf.py -p ... flash` 成功。
  - 芯片：ESP32-P4 revision `v3.1`。
  - bootloader、app、partition table、storage 全部 `Hash of data verified`。
- monitor：
  - 普通管道运行 `idf.py monitor` 失败，原因：`Monitor requires standard input to be attached to TTY`。
  - 使用 TTY 跑 `timeout 60s idf.py -p ... monitor`。
  - 看到 app version `92c32eb`，启动到 camera/LVGL/Ethernet 重连阶段，未见 panic/reboot。
  - 因正式 host 未运行，日志中板端持续尝试连接 `192.168.10.1:5000` 和 `192.168.10.1:5001`，返回 `connect failed so_error=104 errno=119`。
- 临时 TCP 接收器验证：
  - 主机 `enp3s0` 为 `192.168.10.1/24`，`5000/5001` 起初无人监听。
  - 自写临时 Python 接收器监听 `5000/5001`。
  - 第一次验证接收 control metrics：收到 1 秒周期 metrics 包，包含 `cpu_usage/free_heap/free_psram/image_sent/image_encoded` 等字段。
  - 对板子执行 `esptool.py ... run` hard reset 后，接收器确认冷启动自动连接：
    - `ACCEPT port=5000 from=192.168.10.2:53499`
    - `ACCEPT port=5001 from=192.168.10.2:53500`
  - 因现场没有触发“识别成功的新包裹”快照，本轮未收到 JPEG payload；已验证 image TCP 通道自动连接成功。

## 2026-07-07 host technical video reference document

- 用户要求编写上位机 host 技术文档，供队友拍摄作品视频和比赛评审展示参考。
- 用户进一步要求“讲更多的技术”，并选择：
  - Markdown 文档。
  - 工程技术主线。
  - 面向专业评审。
- 检查 host 工程：
  - 工程目录：`/home/kazeform/2026esp/esp32_host_no_inference`
  - 无 `.codegraph/`，按项目规则跳过 CodeGraph。
  - `README.md` 确认 host 是 Qt 6 桌面上位机，无本地模型服务页，检测与分拣结果来自板端。
  - `HostNetworkWorker` 确认双 TCP 服务：control `5000/tcp`、image `5001/tcp`。
  - `packetprotocol` 确认 40 字节自定义 header、magic `"ESP2"`、payload 上限 8 MiB、JPEG/RGB888 类型。
  - `HostController` 确认图像保存、遥测 JSONL、包裹历史、类别统计、控制节流和 `CONFIG a_speed/b_speed/c_speed` 下发。
- 新增文档：
  - `docs/host_technical_video_reference.md`
  - 内容包括系统定位、架构、双链路、独立网络线程、自定义 packet、图像处理、遥测、链路健康、控制下发、跨平台、视频拍摄清单、答辩稿、评委追问和术语表。
- 验证说明：
  - 本次仅新增 Markdown 文档和 agent 记录，未修改 ESP 固件或 host 运行代码。
  - 不运行 `idf.py build` / `idf.py flash monitor`，原因是无固件/代码变更。

## 2026-07-07 ESP32P4_Detection failed package class sequence 123123

- 用户要求将上一轮 `12323` 未检出分类序列改回 `123123`，并烧录。
- 修改前按项目规则提交当前现场配置：
  - `f052c71 checkpoint current hardware runtime config`
  - 包含 A 电机默认速度 `60`、`system_monitor()` 注释、`main/system_init.c` 中 Ethernet 分拣链路分支 `#if 0` 等现场状态。
- 修改：
  - `components/Sorter_app/sorter_core/sorter_scheduler.c` 的 `next_failed_class()` 序列改为 `CLASS1,CLASS2,CLASS3,CLASS1,CLASS2,CLASS3`。
  - `components/Sorter_app/sorting_sim_control.c` 的 `failed_class_from_cursor()` 同步改为 `CLASS1,CLASS2,CLASS3,CLASS1,CLASS2,CLASS3`。
- 提交：
  - ESP 提交：`a84dbb5 use 123123 failed package class sequence`。
- 验证：
  - `idf.py build` 通过。
  - app version：`a84dbb5`。
  - app 大小：`0x4fa790`，factory 分区剩余 `0x105870`，约 `17%`。
  - `idf.py -p /dev/serial/by-id/usb-Espressif_USB_JTAG_serial_debug_unit_E8:F6:0A:E1:7A:D1-if00 flash` 成功。
  - 芯片：ESP32-P4 revision `v3.1`。
  - bootloader、app、partition table、storage 全部 `Hash of data verified`。
  - `timeout 45s idf.py -p ... monitor` 执行，确认 app version `a84dbb5`，启动到 camera/LVGL/分拣运行日志，未见 panic/reboot。
  - monitor 中看到未检出调度开头包裹为 `class1`、下一包为 `class2`。
  - `ISP_AWB` warning 高频刷屏，输出被截断；本轮未完整跑满六个未检出包裹验证 `123123` 全序列。

## 2026-07-07 ESP32P4_Detection failed package class sequence

- 用户要求修改未检出包裹分类：
  - 初始要求从固定韵达改为 `1,2,3` 轮流。
  - 之后纠正为 `1,2,3,2,3` 序列循环。
- 定位：
  - `components/Sorter_app/sorter_core/sorter_scheduler.c` 的 `next_failed_class()` 是未检出/视觉失败时实际进入调度的分类来源，旧逻辑固定返回 `SORTER_CLASS_3`（韵达）。
  - `components/Sorter_app/sorting_sim_control.c` 的 `failed_class_from_cursor()` 只用于调试状态 `next_failed_class` 显示，旧逻辑也固定韵达。
- 提交：
  - 修改前基线提交：`7a02852 checkpoint before rotating failed classes`。
  - 中间提交：`ca118fe rotate failed package classes`，实现 `1,2,3` 循环，随后按用户纠正继续修改。
  - 最终分类提交：`cad27e5 use 12323 failed package class sequence`。
- 当前工作区仍有未提交现场差异，未在本任务中回退：
  - `components/bsp/include/sorter_debug_config.h`: A 电机默认速度为 `60`。
  - `main/system_init.c`: `system_monitor()` 被注释，`SORTER_TCP_LINK_ENABLE` 分支被 `#if 0` 关闭。
  - 因这些差异存在，最终构建和烧录版本为 `cad27e5-dirty`。
- 验证：
  - `idf.py build` 通过。
  - app version：`cad27e5-dirty`。
  - app 大小：`0x4fa790`，factory 分区剩余 `0x105870`，约 `17%`。
  - `idf.py -p /dev/serial/by-id/usb-Espressif_USB_JTAG_serial_debug_unit_E8:F6:0A:E1:7A:D1-if00 flash` 成功。
  - 芯片：ESP32-P4 revision `v3.1`。
  - bootloader、app、partition table、storage 全部 `Hash of data verified`。
  - `timeout 45s idf.py -p ... monitor` 执行，确认 app version `cad27e5-dirty`，启动到 camera/LVGL 初始化，未见 panic/reboot。
  - monitor 被 `ISP_AWB` warning 高频刷屏，输出被截断；本轮未用真实未检出包裹闭环验证 `12323` 出料序列。

## 2026-07-07 ESP32P4_Detection one image per package

- 用户要求修改发图逻辑：每个包裹发送一次图片，并烧录。
- 定位：
  - 旧逻辑在 `components/vision/framework/vision_detect.c` 中用 `!s_display_has_last_hit` 的视觉上升沿触发 `vision_boxed_snapshot_capture()`。
  - 连续包裹如果视觉一直有面单+logo 命中，可能因为没有等到 miss 清空 `s_display_has_last_hit` 而不触发下一张图。
- 修改：
  - 新增 `s_last_snapshot_package_id`。
  - 视觉成功命中时读取 `sorting_sim_control_get_runtime_debug()` 的 `vision_package_id`。
  - `vision_package_id > 0` 时按包裹 ID 去重：同一 ID 只 capture 一次，新 ID 立即 capture。
  - `vision_package_id <= 0` 时保留原视觉上升沿兜底，避免没有真实 S1 包裹窗口时完全不发图。
  - RTT 日志改为输出 `BOXED SNAPSHOT captured pkg=... cls=... conf=... boxes=...`。
- 验证：
  - `idf.py build` 通过，最终 app version 为 `e2f5afc`。
  - app 大小 `0x526d20`，factory 分区剩余 `0xd92e0`，约 `14%`。
  - `idf.py -p /dev/serial/by-id/usb-Espressif_USB_JTAG_serial_debug_unit_E8:F6:0A:E1:7A:D1-if00 flash` 成功。
  - bootloader/app/partition/storage 均 hash verified。
  - monitor 手动复位确认 app version `e2f5afc`，启动到 camera/LVGL/Ethernet 重连阶段。
  - monitor 中上位机 control/image 连接返回 `connect failed so_error=104 errno=119`，本轮未闭环验证上位机实际收到图片。
- 提交：
  - ESP 提交：`e2f5afc capture one snapshot per package`。

## 2026-07-07 ESP32P4_Detection miss hold from 3 to 2

- 用户打断上一轮 monitor/flash 验证流程，要求把 miss 次数调到 `2`。
- 修改：
  - `components/vision/framework/vision_detect.c` 中 `VISION_DISPLAY_MISS_KEEP_COUNT` 从 `3` 改为 `2`。
- 验证：
  - `idf.py build` 通过，生成 `build/sample_project.bin`。
  - app 大小 `0x526a20`，factory 分区剩余 `0xd95e0`，约 `14%`。
- 提交：
  - ESP 提交：`12f5c41 set vision miss hold to two`。
- 说明：
  - 该提交尚未烧录；上一轮已烧录并 monitor 到的是 `35bbca5`。

## 2026-07-07 ESP32P4_Detection restore boot Ethernet init and shorten miss hold

- 用户要求小幅度改动：
  - 上电开启 Ethernet。
  - `main/system_init.c` 初始化恢复成 git 备份中能连接的内容。
  - miss 次数从 `5` 改成 `3`。
  - 用户当前没有板子，无法烧录。
- 修改前按项目规则提交当前工作区：
  - ESP 提交：`947b8b5 checkpoint before restoring ethernet init`。
  - 该提交保存了当时临时关闭完整 Ethernet/UVC/分拣启动、关闭 LVGL perf monitor 的状态，仅作为恢复前备份点。
- 实施：
  - 从 `4946a30 restore lvgl perf monitor overlay` 恢复 `main/system_init.c` 和 `sdkconfig`。
  - 恢复后 `System_Init()` 上电启动 `screen_uvc_start()`，随后启动 `ethernet_app_start()` 并等待 `ethernet_app_wait_ready(5000)`，再启用分拣调试入口、电机输出和真实传感器输入。
  - `sdkconfig` 恢复 `CONFIG_LV_USE_SYSMON=y`、`CONFIG_LV_USE_PERF_MONITOR=y`、`CONFIG_LV_PERF_MONITOR_ALIGN_BOTTOM_RIGHT=y`。
  - `components/vision/framework/vision_detect.c` 中 `VISION_DISPLAY_MISS_KEEP_COUNT` 从 `5` 改为 `3`。
- 验证：
  - 最终 ESP 提交：`35bbca5 restore ethernet init and shorten vision miss hold`。
  - `idf.py build` 通过，最终 app version 为 `35bbca5`，生成 `build/sample_project.bin`。
  - app 大小 `0x526a20`，factory 分区剩余 `0xd95e0`，约 `14%`。
  - 本轮未执行 `idf.py flash monitor`，原因是用户说明当前没有板子。

## 2026-07-07 ESP32P4_Detection Ethernet and sorter autostart coexistence

- 用户确认旧基线可连接上位机 Ethernet，并要求对比最新版导致连接失败的原因；用户判断可能是电机上电默认启动导致，并要求修改为电机启动与 Ethernet 连接同时存在。
- 按项目规则，修改前先提交用户已有修改：
  - `77bd22a checkpoint before ethernet link fix`
  - 包含现场速度/延时配置和视觉投票相关未提交改动。
- 初始最新版失败日志：
  - `E event: create event loop queue failed`
  - `Ethernet sorter link start failed: ESP_ERR_NO_MEM`
  - 说明 Ethernet 初始化太晚，内部 RAM 已被 LCD/camera/LVGL/vision/分拣任务占用。
- 第一次修复：
  - 新增 `ethernet_app_early_init()`，提前创建默认 event loop 和 Ethernet event group。
  - 提交：`e032f76 checkpoint ethernet early init attempt`。
- 用户要求回退到文件夹最开始版本测试 Ethernet：
  - 使用 `23bac51 baseline recovered teammate project` 作为 zip 导入/精简基线。
  - 该基线电机开机默认未启动。
  - 因当前板子为 ESP32-P4 revision v3.1，临时调整 `sdkconfig` revision 后 build/flash。
  - 用户确认该版本连接成功。
- 回到最新版 `e032f76` 后继续修复：
  - `System_Init()` 中将完整 `ethernet_app_start()` 移到分拣硬件启动前。
  - 新增 `ethernet_app_wait_ready(5000)`，最多等待 5 秒，优先让 control/image TCP 连接建立。
  - 之后再调用 `sorting_sim_debug_start()`、`sorting_sim_control_set_motor_output_enabled(true)`、`sorting_sim_control_set_sensor_input_enabled(true)`。
  - 保持 `system_monitor()` 在分拣硬件启动后再启动，减少早期内部 RAM 压力。
- 用户打断要求关闭 `sdkconfig` 的 LVGL perf monitor bottom right：
  - `CONFIG_LV_USE_PERF_MONITOR=y` 改为未启用。
  - bottom right 对齐选项随 perf monitor 关闭不再启用。
- 第一轮验证：
  - `idf.py build` 通过。
  - `idf.py flash` 通过。
  - 上位机端 `ss` 看到 `5000/5001` 两路均 ESTABLISHED。
  - monitor 显示 Ethernet ready 后电机启动，但真实 IO task 报 `create real IO task failed`。
- 第二轮修复：
  - 将 `sorting_sim_control.c` 中 `sort_real_io` 任务改为 `xTaskCreatePinnedToCoreWithCaps()`。
  - 任务栈 4096 字节放到 `MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT`。
  - 失败日志补充 `free_internal` 和 `free_psram`，便于后续定位内存问题。
- 最终验证：
  - `idf.py build` 通过。
  - `idf.py -p /dev/serial/by-id/usb-Espressif_USB_JTAG_serial_debug_unit_E8:F6:0A:E1:7A:D1-if00 flash` 成功。
  - monitor 关键日志显示：
    - `Ethernet Started`
    - `Ethernet Link Up`
    - `ETHIP:192.168.10.2`
    - `control connected to host`
    - `image connected to host`
    - `Ethernet sorter link ready before sorter hardware start`
    - `SORT 电机M1: 正转 65%`
    - `SORT 真实硬件链路已启用`
  - 主机端 `ss` 确认 `192.168.10.1:5000` 和 `192.168.10.1:5001` 均与 `192.168.10.2` 保持 ESTABLISHED。
  - 未再出现 Ethernet `ESP_ERR_NO_MEM` 或 `create real IO task failed`。
- 用户随后澄清：
  - 目标是关闭 LVGL 自带右下角 perf overlay，但保留工程 UI 里的性能/系统监视显示。
  - 复核 `sdkconfig` 后确认当前正确配置为 `CONFIG_LV_USE_SYSMON=y` 且 `CONFIG_LV_USE_PERF_MONITOR` 未启用。
  - `idf.py build` 再次通过，`build/config/sdkconfig.h` 只定义 `CONFIG_LV_USE_SYSMON 1`，未定义 `CONFIG_LV_USE_PERF_MONITOR`。
- 用户现场确认 UI 上性能显示仍未出现，要求用 git 恢复之前有性能显示的版本，即使右下角恢复也可以：
  - 从 git 历史确认 `cf81a8e` 及更早版本均为 `CONFIG_LV_USE_PERF_MONITOR=y`、`CONFIG_LV_PERF_MONITOR_ALIGN_BOTTOM_RIGHT=y`。
  - 仅恢复 `sdkconfig` 中 LVGL perf monitor 相关配置，不改 Ethernet/电机逻辑。
  - `idf.py build` 通过，`build/config/sdkconfig.h` 已定义 `CONFIG_LV_USE_PERF_MONITOR 1` 和 `CONFIG_LV_PERF_MONITOR_ALIGN_BOTTOM_RIGHT 1`。
  - `idf.py -p /dev/serial/by-id/usb-Espressif_USB_JTAG_serial_debug_unit_E8:F6:0A:E1:7A:D1-if00 flash` 成功。
  - ESP 提交：`4946a30 restore lvgl perf monitor overlay`。

## 2026-07-06 ESP32P4_Detection sorter autostart and default speed config

- 用户询问 `ESP32_Detection`/`ESP32P4_Detection` 中电机调试默认是否开启、是否开机自动启动电机。
- 检查结论：
  - 根目录无 `.codegraph/`，跳过 CodeGraph。
  - `main/system_init.c` 中 `SORTER_HARDWARE_DEBUG_MONITOR=1`。
  - 但 `bsp_motor_init()`、`sorting_sim_debug_start()`、`sorting_sim_control_set_motor_output_enabled(true)`、`sorting_sim_control_set_sensor_input_enabled(true)` 原本均未在开机路径实际调用。
  - `sorting_sim_control.c` 内部默认 `s_motor_output_enabled=true`、`s_sensor_input_enabled=true`，收到上位机 `CONFIG`/`MOTOR_TEST` 或真实链路触发时可初始化并输出电机。
- 用户要求设置为自动启动并烧录：
  - 修改前提交 `cf81a8e checkpoint before motor autostart`，其中包含此前未提交的 `sdkconfig` rev v3.1 相关变化。
  - 修改 `main/system_init.c`，开机调用：
    - `sorting_sim_debug_start()`
    - `sorting_sim_control_set_motor_output_enabled(true)`
    - `sorting_sim_control_set_sensor_input_enabled(true)`
  - 提交：`3af4d57 enable sorter autostart`。
  - `idf.py build` 通过，app size `0x526180`，factory 分区剩余约 14%。
  - 使用 `/dev/serial/by-id/usb-Espressif_USB_JTAG_serial_debug_unit_E8:F6:0A:E1:7A:D1-if00` flash 成功。
  - 芯片识别为 ESP32-P4 revision `v3.1`，bootloader/app/partition/storage 均 hash verified。
  - monitor 已看到 app version `cf81a8e-dirty`、min/max chip rev `v3.1/v3.99`、PSRAM 32 MB、camera/LVGL 初始化；因 `ISP_AWB` warning 高频刷屏，用户中断，未形成完整 120 秒运行期结论。
- 用户随后要求：
  - 默认速度全调到 `100`。
  - 告知默认延时/默认速度在哪里调。
  - 给一个方便修改的位置，最好和引脚在同一个地方。
  - 改完编译，不用烧录。
- 已修改：
  - `components/bsp/include/sorter_debug_config.h` 新增默认调度宏：
    - `SORTER_DEFAULT_MOTOR_A_SPEED_PERCENT=100`
    - `SORTER_DEFAULT_MOTOR_B_SPEED_PERCENT=100`
    - `SORTER_DEFAULT_MOTOR_C_SPEED_PERCENT=100`
    - `SORTER_DEFAULT_HANDOFF_DELAY_MS=1000`
    - `SORTER_DEFAULT_BELT_A_TIMEOUT_MS=4500`
    - `SORTER_DEFAULT_BELT_B_TIMEOUT_MS=2000`
    - `SORTER_DEFAULT_BELT_C_TIMEOUT_MS=2000`
    - `SORTER_DEFAULT_LOST_TIMEOUT_MIN_MS=3000`
    - `SORTER_DEFAULT_LOST_TIMEOUT_MAX_MS=6000`
  - `components/Sorter_app/sorter_core/sorter_scheduler.c` 增加 `#include "sorter_debug_config.h"`，`sorter_config_default()` 改为从上述宏读取默认速度、交接延时、皮带超时和 lost timeout。
- 验证：
  - `idf.py build` 通过。
  - app version：`a82793b`。
  - app size：`0x526180`，factory 分区剩余 `0xd9e80`，约 14%。
  - 按用户要求，本次未烧录。

## 2026-07-06 teammate zip import, cleanup, flash, sorter timeout tuning

- 用户要求：
  - 解压两个队友刚整理的 zip：`ESP32P4_Detection(8).zip` 和 `esp32_host_no_inference.zip`。
  - 整理根目录中过多历史工程，创建 `archive_project/`，历史工程只保留参考价值。
  - 删除 `managed_components` 等无用生成内容。
  - 烧录解压后的 ESP 工程。
  - 将电机分拣超时时间从 `6s/3s/3s` 改成 `4.5s/2s/2s`。
- 修改前根目录创建 checkpoint：`d9e2aef checkpoint before project cleanup`。
- 解压/恢复：
  - `esp32_host_no_inference.zip` 可正常解压，已排除 `build/`、`.qtcreator/`、`.claude/`。
  - `ESP32P4_Detection(8).zip` 损坏：`unzip` 报 central directory 缺失，`7z` 报 `Unexpected end of archive`。
  - 7z 仍恢复出大部分源码和 `.git`。
  - 用恢复出的 `.git` 补回缺失的 `model/`、`sdkconfig`、`sdkconfig.defaults`、`partitions.csv` 等构建必需文件。
- 目录整理：
  - 活跃 ESP 工程放到 `/home/kazeform/2026esp/ESP32P4_Detection`。
  - 活跃上位机工程放到 `/home/kazeform/2026esp/esp32_host_no_inference`。
  - 历史工程移动到 `archive_project/`：
    - `new_merge_before_zip_20260706`
    - `esp32_host_no_inference_before_zip_20260706`
    - `lasttime_merge_before_zip_20260706`
    - `lasttime_my_before_zip_20260706`
    - `lasttime_teammate_before_zip_20260706`
    - `teammate_project_before_zip_20260706`
    - `ignore_before_zip_20260706`
    - `esp32_sorter_sim_py_before_zip_20260706`
  - 清理了 `build/`、`managed_components/`、`.cache/`、`.codegraph/`、`.qtcreator/`、`__pycache__/` 等可再生成目录。
- Git：
  - ESP 工程分支：`feat/screen-uvc-stream`。
  - ESP zip 导入/精简基线提交：`23bac51 baseline recovered teammate project`。
  - 上位机工程初始化 git，提交：`9a5e5f2 baseline teammate host project`。
- 超时修改：
  - 文件：`ESP32P4_Detection/components/Sorter_app/sorter_core/sorter_scheduler.c`
  - `belt_a_timeout_ms`: `6000` -> `4500`
  - `belt_b_timeout_ms`: `3000` -> `2000`
  - `belt_c_timeout_ms`: `3000` -> `2000`
  - ESP 提交：`71e63c6 tune sorter belt timeouts`。
- 构建：
  - `idf.py build` 通过。
  - app version：`71e63c6`。
  - app size：`0x50fee0`。
  - factory 分区剩余：`0xf0120`，约 16%。
- 烧录：
  - 端口：`/dev/serial/by-id/usb-Silicon_Labs_CP2102N_USB_to_UART_Bridge_Controller_7ee2f3966ac3ee11be78b90f9e1b1c54-if00-port0`。
  - `idf.py -p <by-id-port> flash` 成功。
  - 芯片：ESP32-P4 revision `v1.0`。
  - bootloader/app/partition/storage hash verified。
- 监控：
  - `idf.py flash monitor` 的 monitor 阶段在非 TTY 下失败：`Monitor requires standard input to be attached to TTY`。
  - 改用 TTY 执行 `timeout 120s idf.py -p <by-id-port> monitor`。
  - monitor 确认 app version `71e63c6`、min/max chip rev `v0.0/v1.99`、PSRAM 32 MB 200 MHz。
  - SC2336 相机检测成功，`1024x600 RGB888`。
  - UVC 启动：`screen UVC stream started: default 1024x600 MJPEG q90`。
  - Ethernet 启动：`Ethernet Started`。
  - 系统启动到 `System initialization done`，120 秒窗口未见 panic/reboot。
  - logo 模型自检出现一次 `compare_elements` 不一致错误，但模型继续加载、fixed image tests 继续执行，vision 和系统初始化完成，当前按非致命现象记录。

## 2026-07-05 two-stage host handoff package

- 用户要求切到两阶段模型并跟进最新，基于“未修改 TCP 链路版本”写差异报告，打包上位机和 ESP 工程给队友接手。
- 用户确认：
  - ESP 差异基线使用 `motor-two-stage` 的 `7a18be7 disable sorter sensors on console uart pins`。
  - 直接推进 `motor-two-stage` 分支。
  - 交接包只包含 latest two-stage ESP 工程 + host，不包含 ROI 或 teammate_project。
- `motor-two-stage` 已同步：
  - cherry-pick `2dcfd7f enable host tcp by default without sim output` -> `f0ad05e`。
  - cherry-pick `58a5143 attach category metadata to jpeg packets` -> `2930984`。
  - 发现 two-stage 仍为 S1 disabled，而 ROI 最新硬件语义为 S1=53、S2=23、S3=-1、S4=22；补最小提交 `a390dc3 sync two-stage sorter s1 pin`。
- 验证：
  - ESP `idf.py build` 通过，app size `0x4ecd30`，factory 分区剩余约 18%。
  - 完整 `idf.py flash` 通过，ESP32-P4 revision `v1.0`，bootloader/app/partition/storage hash verified。
  - S1 同步后 `idf.py app-flash` 通过，app hash verified。
  - monitor 确认 app version `a390dc3`，min/max chip rev `v0.0/v1.99`，UART0 115200 正常。
  - monitor 确认 `sort sensor S1 configured on GPIO 53`、S2=23、S3 disabled、S4=22，启动到 `System initialization done`。
  - monitor 时未启动 host，因此 control/image connect failed 为预期重连日志。
  - host `cmake --build --preset debug` 通过。
- 交接包：
  - `/home/kazeform/2026esp/two_stage_host_handoff_20260705.tar.gz`
  - `/home/kazeform/2026esp/two_stage_host_handoff_20260705.tar.gz.sha256`
  - 大小约 140 MB。
  - SHA256: `af66e056a34f3aac8d1e09d0a97e95eaf16ca622fed8da84db960d3d04088fd6`。
  - 包内包含 `docs/README.md`、`docs/ESP_MIGRATION_GUIDE.md`、`docs/HOST_HANDOFF.md`、`docs/DIFF_REPORT.md`，以及 ESP/host 两份完整 patch。
  - 已排除 `.git`、`build`、`.codegraph`、`.qtcreator` 和本地调试帧文件。

## 2026-07-05 host package-focused data display polish

- 用户要求继续优化上位机数据显示：
  - 第一个页面不要写队列深度、已编码 JPEG、丢弃、无帧、最新大小，改成总包裹数和三类快递数量。
  - 第二个页面不要显示图片分辨率，不要显示检测目标数量；默认每张图片有一个目标，是三类之一。
  - 第四个页面链路端口卡片仍拥挤，需要调整布局。
- 已修改：
  - `HostController` 新增 `courierStatsCards` 和 `imageHealthCards`，把首页业务统计与维护页链路健康拆开。
  - 首页底部改为 `总包裹数/极兔/中通/韵达` 四张卡。
  - 首页顶部把“图像帧数/检测目标”改为“总包裹数/最新类别”。
  - `latestFrameInfo` 不再拼接图片宽高和 JPEG 格式，改为包裹/类别/置信度。
  - 视觉页主图底栏移除“检测目标 N 个”，历史记录移除分辨率和目标数量文案。
  - 系统维护页链路端口区改为控制/图像端口并排，板端地址和上位机地址各自独占整行。
  - 系统维护页图片链路健康改用 `imageHealthCards`，继续保留 JPEG 发送、链路队列、链路跳过、快照状态、最新体积等诊断信息。
- 验证：
  - `cmake --build --preset debug` 通过。
  - `git diff --check` 通过。
  - `QT_QPA_PLATFORM=offscreen timeout 8s ./build/debug/bin/esp32_host_no_inference` 可启动到 timeout，无 QML 输出。

## 2026-07-05 host image category display and courier stats

- 用户要求上位机显示图片时带快递类别：极兔/中通/韵达之一；视觉检测页体现该功能；快递统计作为 feature 替换图片链路中的失败统计，不再显示发送失败。
- 用户确认：
  - 允许同时改板端和上位机。
  - 无法分类时按极兔显示和统计。
  - 快递统计按收到的 JPEG 图片记录计数。
- 修改前 checkpoint：
  - 上位机：`5adbb4f checkpoint before image category display`。
  - 板端：`a09b055 checkpoint before jpeg category metadata`。
- 已修改：
  - 板端 `vision` 增加最新分类快照接口，读取视觉结果里的 `company/confidence`，未知/无目标映射为极兔、置信度 0。
  - 板端 JPEG 包头复用保留字段：`reserved=class_id`，`reserved2 & 0xff=confidence_pct`。
  - 上位机 `PacketHeader` 解析 `reserved/reserved2`，图片历史记录写入 `categoryLabel/categoryConfidence`。
  - 上位机按收到的图片记录累计极兔/中通/韵达数量。
  - 视觉检测页主图增加类别徽标，历史列表显示类别和置信度。
  - `imageLinkCards()` 中“失败”卡片替换为“快递统计”卡片。
  - 修正 `DetectionPage.qml` 的 `hasSelectedFrame` bool 绑定，避免 undefined bool 警告。
- 验证：
  - 上位机 `cmake --build --preset debug` 通过。
  - 上位机 offscreen 启动 8 秒无输出。
  - 本机模拟发送 3 张 JPEG，分别带 `class_id=1/2/3`，生成 `frame_920001.jpg`、`frame_920002.jpg`、`frame_920003.jpg`。
  - `rg` 检查无 `发送失败` 或 UI `title: "失败"` 残留；正常上传计数 `JPEG 发送` 保留。
  - 板端 `idf.py build` 通过，app 大小 `0x4eee40`，factory 分区剩余约 18%。
  - 板端 `idf.py flash` 成功，ESP32-P4 revision `v1.0`，各分区 hash verified。
  - monitor 120 秒窗口启动到 `System initialization done`，未见 panic/reboot。
  - `ss` 轮询确认 `5000/5001` 均保持 `ESTABLISHED`，`latest_preview.jpg` 持续每 5 秒更新。
  - Python listener 解析真实板端 JPEG 包头：`class_id=1 confidence=0`，说明元数据随图像包发送。

## 2026-07-05 host maintenance layout and TCP default split

- 用户反馈系统维护页链路端口卡片错位，并指出“默认关闭 TCP 模拟分拣链路”不应关闭板端和上位机的 TCP 通信。
- 定位：
  - `ReservePage.qml` 右侧链路端口区域固定高 `276`，但复用 4 张 `MetricCard`，两行卡片加标题/边距超过面板高度，导致错位/溢出。
  - `new_merge/main/system_init.c` 中 `SORTER_TCP_LINK_ENABLE` 默认值为 `0`，重烧录后 `ethernet_app_start()` 不执行，板端不会连接上位机。
- 修改前 checkpoint：
  - 上位机：`34338dd checkpoint before maintenance layout fix`。
  - 板端：`5e507e6 checkpoint before tcp default split`。
- 已修改：
  - 上位机 `ReservePage.qml` 增加紧凑 `PortCard`，链路端口四卡片从通用 `MetricCard` 改为专用端口卡，面板高度改为 `242`。
  - 板端 `SORTER_TCP_LINK_ENABLE` 默认改回 `1`，默认启动 control `5000` 和 image `5001` 上位机链路。
  - 板端新增 `SORTER_TCP_SIM_LINE_OUTPUT_ENABLE`，默认 `0`；默认 tick 使用 `sorting_sim_control_tick(NULL, NULL)`，保持本地调度 tick，但不把 tick 产生的模拟状态线输出到 TCP。
  - 上位机发来的 `CONFIG`、`HW_STATUS` 等 SIM line 命令仍走 `process_rx_packet()`，可处理并按需回复。
- 验证：
  - 上位机 `cmake --build --preset debug` 通过。
  - 上位机 `QT_QPA_PLATFORM=offscreen timeout 8s ./build/debug/bin/esp32_host_no_inference` 可启动到 timeout；验证进程监听 `192.168.10.1:5000/5001`。
  - 板端 `idf.py build` 通过，app 大小 `0x4eecc0`，factory 分区剩余约 18%。
  - 板端 `idf.py -p /dev/serial/by-id/usb-Silicon_Labs_CP2102N_USB_to_UART_Bridge_Controller_7ee2f3966ac3ee11be78b90f9e1b1c54-if00-port0 flash` 成功。
  - monitor 120 秒窗口启动到 `System initialization done`，chip revision `v1.0`，app min/max `v0.0/v1.99`，S1/S2/S4 为 GPIO53/23/22，S3 disabled，未见 panic/reboot。
  - `ss` 轮询确认 `192.168.10.1:5000` 和 `192.168.10.1:5001` 均保持 `ESTABLISHED`。
  - `~/Documents/ESP32Host/images/latest_preview.jpg` 更新时间为 `2026-07-05 19:39:56 +0800`，证明 JPEG 图像链路实际接收。

## 2026-07-05 host delivery polish

- 用户要求把上位机从 demo/占位感打磨为可交付作品：
  - 首页不要显示实时事件流。
  - 第二页不要出现画质增强说明。
  - 图片标签从“图片帧”改为“包裹#N”。
  - 预留页填充为实际可用页面。
- 用户确认：
  - 预留页方向为“系统维护”。
  - 首页底部整行直接删除。
  - 完全关闭演示模式；无设备时不生成假数据。
- 修改前上位机提交：`ef188a1 checkpoint before delivery polish`。
- 已修改：
  - 从 CMake 移除并删除 `demodatasource.cpp/.h`，`HostController` 不再连接离线假 metrics/detection。
  - `DashboardPage.qml` 删除底部实时事件流。
  - `DetectionPage.qml` 改为“包裹图像记录”，历史记录显示 `packageLabel`，无检测结果时显示“预览已接收，等待检测结果”。
  - `HostController::addImageHistoryRecord()` 为图片记录生成本地 `包裹#N` 标签；内部 TCP seq 仍保留用于选择/查询。
  - `ReservePage.qml` 重做为系统维护页，包含监听状态、最后遥测、累计接收、保存目录、重新监听、同步时间、运行日志、端口和图片链路健康。
  - Header / 控制页 / README 文案改为交付口径，不再显示演示、预留接口、画质增强等调试词。
- 验证：
  - `cmake --preset debug` 通过。
  - `cmake --build --preset debug` 通过。
  - `QT_QPA_PLATFORM=offscreen timeout 8s ./build/debug/bin/esp32_host_no_inference` 可启动到 timeout，无 QML 加载错误输出。
  - `rg` 检查无 `演示`、`demo`、`图片帧`、`画质增强`、`预留接口`、`实时事件流` 等交付前文案残留。

## 2026-07-05 new_merge TCP packet filtering and default off

- 用户要求检查 TCP 通信数据包是否有合适过滤、是否刷屏，并默认停掉以太网模拟分拣链路。
- 初始分支：`new_merge/motor-two-stage`。
- 修改前创建 checkpoint：`7060f44 checkpoint before tcp packet filtering review`。
- TCP 包审计：
  - packet header 为 40 字节，magic `ESP2`，type `0x01` image、`0x02` metrics、`0x10` time sync、`0x12` SIM line。
  - metrics 发送周期为 1000ms，读取 `system_monitor_get_metrics()` 缓存。
  - image 发送周期为 5000ms，队列深度 2，stale 15s，payload 按 8KB chunk 发送。
  - control task 每 20ms tick；`sorter_scheduler_tick()` 原本每 tick 都会发 `STATUS,reason=tick`，有活动包裹时还会每 tick 发带 `pos` 的 `PKG`，日志层过滤不等于 TCP 发包过滤，存在明显刷屏风险。
- 已修改：
  - `main/system_init.c`: `SORTER_TCP_LINK_ENABLE` 默认从 `1` 改为 `0`，仍允许编译期 `-DSORTER_TCP_LINK_ENABLE=1` 覆盖。
  - `components/Ethernet_app/ethernet_app.c`: 在 `send_sim_line_packet()` 前增加 SIM line 过滤。
  - 丢弃 `STATUS` reason=`tick`/`sensor1`/`package_new`/`vision`。
  - 对 `PKG` 按 `id/belt/state/class` 生成签名；签名变化立即发送，未变化时最多 1 秒发一次心跳，避免 20ms position 刷屏。
  - JPEG `queued`/`sent` 周期日志从 INFO 降到 DEBUG。
- 验证：
  - `git diff --check` 通过。
  - `motor-two-stage` 生成提交：`32dcd9f filter tcp sim packets and disable link by default`。
  - `motor-two-stage` `idf.py build` 通过，app 大小 `0x4a9380`，factory 分区剩余 `0x156c80`，约 22%。
  - cherry-pick 到 `motor-roi` 生成提交：`c7ccbc0 filter tcp sim packets and disable link by default`。
  - `motor-roi` `idf.py build` 通过，app 大小 `0x4ab490`，factory 分区剩余 `0x154b70`，约 22%。
  - 本轮未执行 flash/monitor。

## 2026-07-05 host image history and preview enhancement

- 用户反馈上位机“接收图片历史”看不到，并要求对接收到的图片做画质优化，减少类似椒盐噪声的视觉问题。
- 问题定位：
  - `DetectionPage.qml` 右侧历史使用 `host.frameHistory`。
  - `HostController::applyDetectionFrame()` 才会向 `m_frameHistory` 插入记录。
  - 单纯收到 `5001` JPEG 只更新 `latest_preview.jpg` / `latestImageUrl`，不会生成历史记录，因此只有图片流、没有 detection JSON 时历史为空。
- 已修改：
  - `saveLatestPreviewImage()` 将接收图解码为 `QImage` 后做展示增强。
  - 增强算法：3x3 中值滤波压制孤立椒盐/彩色噪点，随后轻微反锐化保留边缘；重新编码为质量 88 的 JPEG。
  - 每帧保存独立 `frame_%06u.jpg`，同时覆盖 `latest_preview.jpg`。
  - `addImageHistoryRecord()` 在每张图片保存成功后插入历史项，历史上限 48 条。
  - `DetectionPage.qml` 右侧列表标题改为接收图片历史，并显示真实缩略图；无检测 JSON 的图片帧显示“仅图片帧 画质增强后展示”。
- 验证：
  - `cmake --build --preset debug` 通过。
  - `QT_QPA_PLATFORM=offscreen timeout 8s ./build/debug/bin/esp32_host_no_inference` 可启动到 timeout，无 QML 加载错误输出。
  - 测试时启动上位机并用本机 `~/Documents/ESP32Host/images/latest_preview.jpg` 按协议模拟发送到 `192.168.10.1:5001`，确认生成 `frame_910001.jpg` 和更新 `latest_preview.jpg`。
  - 测试期间真实板端也继续写入 `frame_005781.jpg`、`frame_006004.jpg`，说明实时图片历史路径可用。

## 2026-07-05 host UI parity with board normal dashboard

- 用户要求重新规划：上位机参考板端普通 UI 内容，不把板端 DEV/debug 页面内容作为上位机显示/控制数据；DEV 相关只需要电机调速。
- 读取 `new_merge/components/UI/generated/setup_scr_dashboard.c`、`components/UI/sdk/ui.c`、`components/UI/sdk/ui.h` 后整理 `docs/agent/BOARD_UI_INVENTORY.md`：
  - 普通 dashboard 包含系统监控、JPEG/视觉预览、检测/历史、普通 settings、about/system 信息。
  - settings 中存在屏幕亮度、置信度阈值、检测开关、预览叠加、控制模式/速度等普通项。
  - 当前 UI SDK 只看到亮度和 logo calibration 的业务绑定；置信度/检测/叠加等普通项在上位机先做本地 UI 状态。
  - 明确不镜像 DEV/debug 页：S1-S4、编码器、MTEST、包裹注入、`ENC_CLEAR`、专用 `HW_STATUS` 面板。
- 上位机修改：
  - 删除 `ModelWorkspacePage.qml`，从 `Main.qml` 和 `CMakeLists.txt` 移除模型工作台页面。
  - 移除 `HostController` / `HostNetworkWorker` 中本地模型服务、推理开关、HTTP 请求、推理结果回发等逻辑。
  - `HostNetworkWorker::handleImage()` 改为收到每张 `5001` JPEG 都更新预览，不再按 10 帧节流。
  - `sendInitialControls()` 只做 time sync，不再主动发送 `HW_STATUS`。
  - Dashboard 增加板端 image link counters，显示 encoded/sent、队列、drop/fail、latest JPEG size、encode/send time。
  - Detection 页面保留原图片预览/历史结构，默认显示最新板端 JPEG，叠加框受本地 preview overlay 控制。
  - Control 页面改为普通设置：亮度、置信度阈值、检测开关、预览叠加、电机速度；只有电机速度向板端发送 `CONFIG a_speed=<v> b_speed=<v> c_speed=<v>`。
  - README 改为板端视觉链路说明，写明 `5000/5001`、PC/board IP、DEV 项不镜像。
- 验证：
  - `rg` 检查后，代码/QML 中不再有本地推理页面、YOLO 服务控制、MTEST/encoder/S1-S4 控制残留；剩余 `esp32_host_no_inference` 仅为工程/可执行文件名。
  - `cmake --preset debug` 通过。
  - `cmake --build --preset debug` 通过。
  - `QT_QPA_PLATFORM=offscreen timeout 8s ./build/debug/bin/esp32_host_no_inference` 可启动到 timeout，无 QML 加载错误输出。

## 2026-07-05 new_merge host TCP/JPEG low revision validation

- 读取 `docs/agent/PROJECT.md`、`CURRENT.md`、`HISTORY.md`，确认 `new_merge` 当前分支为 `motor-roi`，提交为 `ceae9b5`。
- 按规则为 `esp32_host_no_inference` 初始化独立 git：
  - `f9ae6d9 baseline host no inference`
  - 分支 `tcp-board-align`
- 上位机协议对齐提交：`2ad49e8 align host tcp protocol with board`。
  - `packetprotocol.h` 将 `0x12` 改名为 `kTypeSimLine`，不再称为 inference result。
  - `HostNetworkWorker` 对 `0x12` 直接按 UTF-8 SIM line 写日志。
  - 初始控制只发送 `TimeSyncJson(0x10)` 和 `SIM_LINE("HW_STATUS")`。
  - 不再发送板端不解析的 `ControlJson(0x11)` / `upload_format`。
  - `motor_speed` 映射为 `CONFIG a_speed=<v> b_speed=<v> c_speed=<v>`。
  - `conveyor` / `auto_run` 映射为板端 `CONFIG mode=... motor_output=... sensor_input=...`。
  - `brightness`、`fill_light`、`danger_threshold` 只保留本地 UI 状态。
  - README 更新 PC/board IP、5000/5001、防火墙、JPEG 固定端口和 no-inference 说明。
- 上位机验证：
  - `cmake --preset debug` 通过。
  - `cmake --build --preset debug` 通过。
  - offscreen 运行后监听 `192.168.10.1:5000` 和 `192.168.10.1:5001`。
- 板端首次 monitor 验证 `ceae9b5`：
  - ESP32-P4 revision `v1.0`，app min/max 为 `v0.0` / `v1.99`。
  - UART0 115200 console 正常。
  - S1=GPIO53，S2=GPIO23，S3 disabled，S4=GPIO22。
  - `System initialization done`。
  - 发现三条 TCP task 创建失败：`create TCP control/image producer/image sender task failed`。
- 板端修正提交：
  - `c88a38a checkpoint before tcp task stack fix`
  - `50f132a allocate tcp task stacks in psram`
    - 使用 `xTaskCreatePinnedToCoreWithCaps(..., MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT)`。
    - 失败日志补充 internal/PSRAM 空闲量。
  - `12ec1cf fix jpeg snapshot cache sync`
    - snapshot RGB buffer 改为 64 字节对齐。
    - PPA 后 M2C cache sync 移除 `ESP_CACHE_MSYNC_FLAG_UNALIGNED`。
  - `14ff3a2 move jpeg producer off busy core`
    - image producer 从 core1 移到 core0，优先级 3；sender 保持 core0 优先级 2。
- 板端验证：
  - `idf.py build` 通过，app 大小 `0x4ec6c0`，factory 剩余约 18%。
  - `idf.py flash` 全量烧录 `12ec1cf` 成功，bootloader/app/partition/storage hash verified。
  - `idf.py app-flash` 烧录 `14ff3a2` 成功，app hash verified。
  - monitor 验证 `14ff3a2` 启动到 `System initialization done`，无 panic/reboot。
  - `ss` 显示 control `5000` 与 image `5001` 均 ESTABLISHED。
  - telemetry 显示 metrics 每秒更新，示例：`image_encoded=15`、`image_sent=15`、drop/fail 均 0，最近 JPEG 约 `39184` bytes，encode 约 `332ms`，send 约 `12ms`。
  - `~/Documents/ESP32Host/images/latest_preview.jpg` 更新时间为 `2026-07-05 18:11:06 +0800`，大小 `39184` bytes。
- 串口经验：
  - 显式 `-b 921600/460800/115200` 多次遇到 stub 或 app 写入中断。
  - 直接 `idf.py flash` 可成功，失败后重试也可成功；app-only 变更优先用 `idf.py app-flash` 降低串口暴露时间。
- 同步到 `motor-two-stage`：
  - `git switch motor-two-stage`。
  - cherry-pick `9e4b23d ceae9b5 50f132a 12ec1cf 14ff3a2` 无冲突完成。
  - 生成提交：`97d0c96`、`c57a05e`、`3651afa`、`950ca0f`、`321b759`。
  - `idf.py build` 通过，app 大小 `0x4ea5c0`，factory 分区剩余约 18%。
  - 未对 `motor-two-stage` 进行实机 flash/monitor；当前实机验证对象仍是 `motor-roi` 的 `14ff3a2`。

## 2026-07-05 new_merge low revision UART restore

- 用户打断 TCP 上位机对齐任务，要求先处理 revision 支持和串口占用。
- 初始要求“把传感器设置为 -1”后用户明确修正：不要全部置 `-1`，只改 GPIO37/38 中占用串口的传感器。
- 修改前在 `new_merge` 提交现场 checkpoint：`9c4be17 checkpoint before revision and serial config`。
- 当前 `new_merge` 分支：`motor-roi`。
- 修改内容：
  - `components/bsp/include/sorter_debug_config.h`: 只将 `SORTER_SENSOR_S3_GPIO` 从 `38` 改为 `-1`。
  - S1/S2/S4 保持 `53/23/22`。
  - `sdkconfig` / `sdkconfig.defaults`: 启用 ESP32-P4 low revision 路径：
    - `CONFIG_ESP32P4_SELECTS_REV_LESS_V3=y`
    - `CONFIG_ESP32P4_REV_MIN_0=y`
    - `CONFIG_ESP32P4_REV_MIN_FULL=0`
    - `CONFIG_ESP_REV_MIN_FULL=0`
    - `CONFIG_ESP32P4_REV_MAX_FULL=199`
    - `CONFIG_ESP_REV_MAX_FULL=199`
  - `sdkconfig`: 恢复 UART console 到 UART0、115200，secondary USB Serial/JTAG enabled。
- 过程记录：
  - 第一次仅修改 revision full 值后 build 仍选择 `ESP32P4_REV_MIN_301`，说明 Kconfig 选择项未切换。
  - 查阅 ESP-IDF `components/esp_hw_support/port/esp32p4/Kconfig.hw_support` 后，补上 `ESP32P4_SELECTS_REV_LESS_V3=y` 和 `ESP32P4_REV_MIN_0=y`。
  - 第二次 build 日志使用 `<3.0` ROM linker scripts，说明低 revision 选择生效。
  - `idf.py build` 通过；app 大小 `0x4ec5e0`，factory 分区剩余约 18%。
  - `sdkconfig` 被 build 重新生成为 LF，已恢复成仓库原 CRLF，避免全文件换行 diff。
- 串口状态：
  - `find /dev -maxdepth 1 \( -name 'ttyACM*' -o -name 'ttyUSB*' \) -print` 无输出。
  - `lsusb` 能看到 `10c4:ea60 Silicon Labs CP210x UART Bridge`。
  - `/dev/serial/by-id`、`/dev/serial/by-path` 不存在。
  - `dmesg` 读取失败：`Operation not permitted`。
  - `lsmod | rg 'cp210x|usbserial'` 无输出。
  - 因无 tty 节点，未执行 `idf.py flash monitor`。

## 2026-07-05 new_merge TCP CPU optimization

- 用户目标：降低 `new_merge` TCP 链路 CPU 占用；业务为实时 CPU/内存 metrics，图片允许延迟；允许双通道和压缩图片。
- 只读分析：
  - 当前 `new_merge` 实际默认未启动 TCP：`SORTER_HARDWARE_DEBUG_MONITOR=1` 时跳过 `ethernet_app_start()`。
  - 旧图片路径由 `SORTING_SIM_TCP_SEND_IMAGES` 控制，默认 0。
  - 若打开旧图片路径，会在 Ethernet task 内直接 `cam_sensor_get_frame()`，整帧 `memcpy` 到 PSRAM 后 raw TCP 发送。
  - 当前相机默认 SC2336 MIPI RAW8 1024x600，ISP 输出 RGB888；raw 单帧约 `1024*600*3 = 1.84MB`。
  - 旧路径会同时带来 V4L2 buffer 竞争、PSRAM 大块拷贝、LwIP/TCP 分片发送和 control/metrics 队头阻塞。
  - `/home/kazeform/2026upper/esp32_project` 的改进方向包括 `esp_new_jpeg`、control/image 双端口、JPEG payload、独立图片 socket。
- 用户确认：
  - 图片默认使用预览尺寸 `640x375`。
  - 测试阶段先做 5 秒一张；真实业务后续按每个包裹触发。
  - 图片队列最多 2 张。
- 修改前在 `new_merge` 提交现场状态：`0559655 checkpoint before tcp cpu optimization`。
- 已实现：
  - `components/vision/framework/vision.h` / `vision_app.c` 新增 `vision_copy_latest_frame_scaled_rgb888()`。
  - 该接口从 vision ring 最新帧生成 PPA 缩放 RGB888 快照；持 ring mutex 保证 V4L2 buffer 生命周期，PPA 后做 M2C cache invalidate。
  - `components/Ethernet_app/ethernet_app.c` 改为 control/image 双通道：
    - `5000`：metrics、time sync、SIM_LINE、sorter tick。
    - `5001`：JPEG 图片发送。
  - `eth_control` 每 1 秒发送 metrics，读取 `system_monitor_get_metrics()` 缓存，不再枚举 FreeRTOS 任务。
  - `eth_img_prod` 每 5 秒尝试生成 `640x375` JPEG；队列满或 image socket 未连接时跳过。
  - `eth_img_send` 低优先级发送 JPEG，payload 按 8KB chunk 发送并 yield。
  - JPEG encoder 使用 `esp_new_jpeg` C API，quality 60，subsample 4:4:4。
  - metrics JSON 增加 image queue/drop/encode/send 统计字段。
  - `main/system_init.c` 新增 `SORTER_TCP_LINK_ENABLE=1`，并将 `system_monitor()` 放在 Ethernet 启动前。
  - `components/Ethernet_app/CMakeLists.txt` 增加 `system_monitor`、`vision`、`esp_new_jpeg` 依赖，移除失效的 `SORTING_SIM_TCP_SEND_IMAGES=0` 编译定义。
- 静态检查：
  - `rg` 确认 `components/Ethernet_app` 内无 `cam_sensor_get_frame`。
  - `rg` 确认 `components/Ethernet_app` 内无 `uxTaskGetSystemState`。
- 构建验证：
  - `cd /home/kazeform/2026esp/new_merge && idf.py build` 通过。
  - app 大小 `0x4fbed0`，factory 分区剩余约 17%。
- 实机验证：
  - `find /dev -maxdepth 1 \( -name 'ttyACM*' -o -name 'ttyUSB*' \) -print` 无输出。
  - 当前无板子/串口设备，未执行 flash/monitor。

## 2026-07-04 MyAlbums split packages

- 按用户要求删除 `MyAlbums/.git`，`MyAlbums` 从约 7.9G 降到约 2.9G。
- 删除旧大包 `/home/kazeform/2026esp/MyAlbums_dataset_20260704.zip`。
- 重新生成 1G 以内分包：
  - `datasets_001.zip`: 942533582 bytes，339 张 jpg。
  - `datasets_002.zip`: 942011668 bytes，285 张 jpg。
  - `datasets_003.zip`: 943555639 bytes，282 张 jpg，包含 `DATASET_MANIFEST.csv`。
  - `datasets_004.zip`: 232218906 bytes，72 张 jpg。
- 四个分包均通过 `zip -T`。
- 分包合计 jpg 数量 978，分类统计仍为 `16_9=430`、`4_3=368`、`half=180`。

## 2026-07-04 new_merge pin revision check

- 读取 `docs/agent/PROJECT.md`、`CURRENT.md`、`HISTORY.md`。
- 确认 `/home/kazeform/2026esp/new_merge` 为独立 git 仓库，分支 `motor-roi`，修改前工作区干净。
- 按仓库规则创建修改前空提交：`ed31845 checkpoint before pin revision update`。
- `new_merge` 无 `.codegraph/`，跳过 CodeGraph。
- 检查命令：
  - `rg -n "GPIO...|SCCB|I2C_SCL|TOUCH_INT|SORTER_SENSOR|REV_MIN|REV_MAX" components main sdkconfig sdkconfig.defaults sdkconfig.old ...`
  - `nl -ba components/bsp/include/sorter_debug_config.h`
  - `nl -ba components/bsp/bsp_touch.c`
  - `nl -ba components/bsp/bsp_cam_sensor.c`
  - `nl -ba sdkconfig sdkconfig.defaults`
- 结果：
  - `components/bsp/include/sorter_debug_config.h` 当前 S1=-1、S2=23、S3=-1、S4=22。
  - `components/bsp/bsp_touch.c` 当前 `TOUCH_I2C_SCL_GPIO=8`、`TOUCH_I2C_SDA_GPIO=7`、`TOUCH_INT_GPIO=24`。
  - `components/bsp/bsp_cam_sensor.c` 明确复用 touch 已创建的 I2C bus 作为 SCCB，总线为 GPIO8/7。
  - GPIO21 未见现有硬件占用；GPIO47 未见现有硬件占用；GPIO22/23 已为目标 S4/S2。
  - GPIO8 与 touch/camera 控制总线冲突，不能直接作为 sorter S1 输入。
  - `sdkconfig.defaults` 为 ESP32-P4 rev >=3.0 配置，当前 `sdkconfig` 仍为 rev <3.0 配置。
- 用户确认 S1 改用 GPIO20。
- 补查 GPIO20：未发现工程内硬件 GPIO 占用。
- 已修改：
  - `new_merge/components/bsp/include/sorter_debug_config.h`: S1=20, S2=23, S3=47, S4=22。
  - `new_merge/components/bsp/bsp_touch.c`: `TOUCH_INT_GPIO=21`。
  - `new_merge/sdkconfig`: `CONFIG_ESP32P4_REV_MIN_301=y`, `CONFIG_ESP_REV_MIN_FULL=301`, `CONFIG_ESP32P4_REV_MAX_FULL=399`, `CONFIG_ESP_REV_MAX_FULL=399`。
- 当前可用串口：`/dev/ttyACM0`。
- `idf.py build` 成功；生成 `build/sample_project.bin`，app 大小 `0x4bed40`，factory 分区剩余约 21%。
- `idf.py -p /dev/ttyACM0 -b 921600 flash` 成功：
  - 识别芯片 `ESP32-P4 revision v3.1`。
  - bootloader、app、partition table、storage 全部 `Hash of data verified`。
- 首次 `idf.py -p /dev/ttyACM0 monitor` 因非 TTY 失败；改用带 PTY 执行 `timeout 90s idf.py -p /dev/ttyACM0 monitor`。
- monitor 结果：
  - boot log: `chip revision: v3.1`。
  - app log: `Min chip rev: v3.1`, `Max chip rev: v3.99`, `Chip rev: v3.1`。
  - `vision started: preview 640x375 at 19,118, ring depth 3`。
  - `SORTDBG ready`。
  - `sort sensor S1 configured on GPIO 20`。
  - `sort sensor S2 configured on GPIO 23`。
  - `sort sensor S3 configured on GPIO 47`。
  - `sort sensor S4 configured on GPIO 22`。
  - `System initialization done`。
  - 启动时 S3 初始 `raw=1 active=1` 触发一次 `sensor_without_package`，约 6.7 秒日志显示 S3 稳定变为 `raw=0 active=0`。
  - 90 秒 monitor 窗口未见 panic/reboot。
- 用户后续反馈：无遮挡时 S2/S4 默认 OFF 正常，S1/S3 默认 ON，且不只是反相问题。
- 只读检查：
  - `new_merge` 无 `.codegraph/`，继续跳过 CodeGraph。
  - `components/bsp/bsp_sort_sensor.c` 对 S1-S4 统一配置为 input、`GPIO_PULLUP_DISABLE`、`GPIO_PULLDOWN_ENABLE`、`GPIO_INTR_DISABLE`。
  - `components/bsp/include/sorter_debug_config.h` 中四个 active level 都是 1，没有 S1/S3 特殊反相。
  - ESP-IDF `soc/gpio_num.h` 标注 GPIO20/GPIO47 都是 input/output。
  - ESP-IDF `soc_caps.h` 中 ESP32-P4 有效 GPIO mask 覆盖 GPIO0-54，有效 digital IO pad 为 GPIO16-54；GPIO20/GPIO47 都在范围内。
  - 工程内显式 `*_GPIO` 占用：motor 2/3/4/5/32/36，touch/camera I2C 7/8，touch reset/int 33/21，LCD backlight/reset 26/27，Ethernet 31/52/51，sensors 20/23/47/22。
  - 未查到 GPIO20/GPIO47 被其它工程模块复用。
- 串口命令：
  - 第一次 pyserial 打开 `/dev/ttyACM0` 触发板子复位，命令发送过早，只读到启动日志。
  - 第二次等待 `System initialization done` 后发送 `HW_STATUS`，返回：`SIMOUT HW_STATUS,mtest=0,s1=1,s2=0,s3=1,s4=0,s1_valid=1,s2_valid=1,s3_valid=1,s4_valid=1,...`。
  - 因 S1 默认 active，调度器已创建一个 package 并进入 `holding_at_s2`，会干扰实物联调。
- 结论：未查到软件资源冲突；S1/S3 默认 ON 更像外部电路/接线/管脚电平问题，或内部下拉不足以压住输入。

- 用户确认：S1 插 GPIO53，S3 插 NC 标注的 GPIO45。
- 检查 GPIO45/GPIO53：
  - ESP-IDF `soc/gpio_num.h` 标注二者均为 input/output。
  - 工程内未查到 GPIO45/GPIO53 被其它模块占用。
  - GPIO53 另有 ADC2 channel 能力，不影响普通 GPIO 输入。
- 按规则在继续改 S1/S3 前提交当前已验证引脚/revision 基线：`d4dca6c update new board pins and revision`。
- 已修改 `new_merge/components/bsp/include/sorter_debug_config.h`: S1=53, S2=23, S3=45, S4=22。
- `idf.py build` 成功，app 大小仍为 `0x4bed40`，factory 分区剩余约 21%。
- `idf.py -p /dev/ttyACM0 -b 921600 flash` 成功：芯片 `ESP32-P4 revision v3.1`，bootloader、app、partition table、storage 全部 `Hash of data verified`。
- 按用户要求，本次未运行 monitor。
- 用户继续要求 S3 改到 GPIO37，且串口不用。
- 按规则在继续改 S3 前提交当前 53/45 基线：`bad060d move sorter sensors to gpio53 and gpio45`。
- 检查发现 GPIO37 是当前 ESP32-P4 console UART0 TX；为释放 GPIO37 给 S3，修改 `sdkconfig` 关闭 console UART：
  - `CONFIG_ESP_CONSOLE_NONE=y`
  - `CONFIG_CONSOLE_UART_NONE=y`
  - `CONFIG_ESP_CONSOLE_UART_NONE=y`
  - UART console num 改为 `-1`
- 已修改 `new_merge/components/bsp/include/sorter_debug_config.h`: S1=53, S2=23, S3=37, S4=22。
- `idf.py build` 成功，app 大小 `0x4bac20`，factory 分区剩余约 21%。
- `idf.py -p /dev/ttyACM0 -b 921600 flash` 成功：芯片 `ESP32-P4 revision v3.1`，bootloader、app、partition table、storage 全部 `Hash of data verified`。
- 按用户要求，本次不运行 monitor。

## Archived

- `docs/agent/archive/2026-07-04-myalbums-dataset-organization.md`
- `docs/agent/archive/2026-07-04-merge-ui-dependency-docs.md`
- `docs/agent/archive/2026-07-03-findlogo-merge.md`
