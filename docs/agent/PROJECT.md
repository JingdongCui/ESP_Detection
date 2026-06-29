# Project Notes

## Workspace

- 根目录：`/home/kazeform/2026esp`
- 当前主任务工程：`merge_project`
- 相关对照工程：`bug_project`
- 根目录 git 仓库已初始化，用于跟踪根目录 agent 文档；`bug_project` 和 `merge_project` 各自仍有独立 git 仓库。

## Hardware

- 目标芯片：ESP32-P4
- 显示：1024x600 MIPI DSI/DPI LCD
- 当前显示诊断状态：常用基准为 RGB24、DPI 40 MHz、3 framebuffer、DMA2D on；最近实验另测过 RGB565
- 相机：SC2336 MIPI CSI
- 当前相机诊断状态：sensor `RAW10_640x480_50fps`，ISP 输出 `640x480 RGB888`
- PSRAM：启动日志显示 32 MB，200 MHz
- 串口通常为 `/dev/ttyACM0`

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
- UI 调试面板显示 S1-S4 四个状态块：绿色填充表示 ON，深色/无明显颜色表示 OFF，`--` 表示当前未有效读取。

## Merge Project Sorter Control Link

- 真实硬件链路和以太网模拟链路的共同核心是：
  - `components/Sorter_app/sorting_sim_control.c`
  - `components/Sorter_app/sorter_core/sorter_scheduler.c`
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
