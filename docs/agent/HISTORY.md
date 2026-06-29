# History

## 2026-06-27 Blue-Screen Debug Before Doc Move

- 放大 `BLUE MARK` 按钮，便于肉眼看到蓝屏后快速点击。
- 添加 LCD framebuffer sampled hash 诊断，`BLUE_MARK` 时 dump 最近约 2.4 秒历史。
- 添加 RAW8/RAW10 自动映射，支持 SC2336 `RAW10_640x480_50fps`。
- 当前诊断固件切到 camera `640x480 RGB888`，preview off，duty 200 ms。
- `idf.py build` 通过。
- `idf.py flash monitor` 已执行，启动日志确认：
  - LCD `1024x600 RGB24 dpi=40MHz dma2d=1`
  - LCD active_read 约 `83608 KB/s`
  - camera `RAW10_640x480_50fps`
  - camera active_write 约 `4500 KB/s`
- DMA2D off 真 A/B 后更差，已恢复 DMA2D on。
- 多次 `BLUE_MARK` 前 framebuffer hash 基本稳定，支持 scanout starvation。

## 2026-06-27 Docs Relocation

- 根据用户要求，文档系统移动到根目录 `docs/agent`。
- 归档根目录旧文档：
  - `docs/hardware_changes.md` -> `docs/agent/archive/2026-06-27-sorter-hardware-changes.md`
  - `docs/simulator_changes.md` -> `docs/agent/archive/2026-06-27-sorter-simulator-changes.md`
- 归档原蓝屏长文档：
  - `bug_project/docs/display_blue_screen_debug.md` -> `docs/agent/archive/2026-06-27-blue-screen-debug-plan.md`
- 移动并重写 agent 文档：
  - `docs/agent/PROJECT.md`
  - `docs/agent/CURRENT.md`
  - `docs/agent/HISTORY.md`
- 删除 `bug_project/docs`，避免工程内外两套文档系统分叉。

## Not Run

- 本次文档迁移没有运行 `idf.py build`，因为没有改代码。
- 本次文档迁移没有运行 `idf.py flash monitor`，因为没有改固件。
- 未执行 git commit：根目录 `.git` 不完整，`bug_project` 工作树已有多处既有脏改，避免混合提交。

## 2026-06-27 RGB565 / Arbitration Tests

- 修改前在 `bug_project` 提交诊断 checkpoint：`78ec4d9 checkpoint blue screen diagnostics`。
- 实验实现：
  - `components/bsp/bsp_lcd.c` 支持随 `CONFIG_LV_COLOR_DEPTH` 切换 RGB565/RGB888。
  - `sdkconfig` 和 `sdkconfig.defaults` 切到 `CONFIG_LV_COLOR_DEPTH=16`。
  - ESP-IDF `esp_lcd_panel_dpi.c` 中 LCD/DPI DW-GDMA `chan_priority` 临时从 1 提到 3。
- 第一轮：`RGB565 + LCD priority 3 + CSI priority 0`
  - `idf.py build` 通过。
  - `idf.py flash monitor` 首次 monitor 因非 TTY 失败，随后单独 `idf.py monitor` 跑通。
  - 启动确认 LCD `1024x600 RGB16 dpi=40MHz`，active_read 约 `55738 KB/s`。
  - 用户肉眼观察到蓝屏。
  - 日志没有可靠对应 `BLUE_MARK`/underrun，说明 marker 链路不能覆盖所有可见蓝屏。
  - 后续 CSI 出现 transaction queue full 和 camera frame timeout，CSI priority 0 不可用。
- 第二轮：`RGB565 + LCD priority 3 + CSI priority 1`
  - `idf.py build` 通过。
  - `idf.py -p /dev/ttyACM0 flash monitor` 跑超过 10 分钟。
  - 启动确认 LCD `1024x600 RGB16 dpi=40MHz`，active_read 约 `55738 KB/s`。
  - camera 侧稳定，`timeouts=0`，capture receive max 约 `40ms`。
  - 用户肉眼观察到蓝屏。
  - 日志仍未可靠出现对应 `BLUE_MARK`/underrun；“无 BLUE_MARK”不能再作为“无蓝屏”的判据。
- 当前结论：
  - RGB565 和 LCD priority 3 降低了带宽，但没有消除可见蓝屏。
  - 近期测试必须以肉眼观察为准，日志链路需补强。
  - 下一步回到 camera off 对照链路，重新确认该链路是否真的无可见蓝屏。
- 保留实验代码提交：`6723542 test rgb565 lcd bandwidth reduction`。

## 2026-06-27 Camera-Off Recheck

- 根据用户要求回到关闭摄像头链路，确认过去 camera-off 无蓝屏结论是否真实。
- 操作：
  - 从 `78ec4d9` 恢复 RGB24 基准显示配置。
  - 将 `sdkconfig` 中 `CONFIG_VISION_APP_ENABLE_CAMERA_STREAM=y` 改为 `# CONFIG_VISION_APP_ENABLE_CAMERA_STREAM is not set`。
  - 确认外部 ESP-IDF `esp_lcd_panel_dpi.c` 的 LCD DW-GDMA `chan_priority` 回到默认 1。
- 提交：`fa00639 test camera stream disabled baseline`。
- `idf.py build` 通过。
- `idf.py -p /dev/ttyACM0 flash monitor` 已执行。
- 启动日志确认：
  - app version `fa00639`
  - LCD `1024x600 RGB24 dpi=40MHz`
  - LCD active_read 约 `83608 KB/s`
  - `camera stream disabled for bandwidth A/B test`
- 用户肉眼观察确认此版本无蓝屏。
- monitor 被用户中断后仍有后台进程占用串口，已用 PID 清理。
- 结论：
  - camera-off 对照链路重新确认无可见蓝屏。
  - camera/CSI/ISP stream 仍是必要触发因素。
  - 最近两轮 RGB565/仲裁实验的“无 BLUE_MARK”不是无蓝屏，说明日志/按钮链路不足。

## 2026-06-27 Ethernet-Off / Camera-On Test

- 用户提出新假设：
  - `merge_project` 改过以太网链路后蓝屏有改善。
  - 队友测试停掉以太网后不蓝屏。
  - 用户已在程序里注释掉以太网入口函数。
- 确认用户改动：
  - `main/system_init.c` 中 `ethernet_app_start()` 被注释。
  - 提交：`8800cf5 test ethernet entry disabled`。
- 恢复摄像头：
  - `sdkconfig` 中 `CONFIG_VISION_APP_ENABLE_CAMERA_STREAM=y`。
  - 提交：`109730f test camera on ethernet off`。
- 验证：
  - `idf.py build` 通过。
  - `idf.py -p /dev/ttyACM0 flash` 通过。
  - 按用户要求没有启动 monitor。
- 当前烧录固件：
  - app commit `109730f`
  - RGB24、camera stream on、Ethernet app entry disabled。
- 待用户肉眼观察是否蓝屏。

## 2026-06-27 Full Features Except Ethernet Build

- 用户要求改成完整功能但关闭以太网，先只构建，不烧录。
- 配置：
  - `main/system_init.c` 中 `ethernet_app_start()` 继续注释。
  - `CONFIG_VISION_APP_ENABLE_CAMERA_STREAM=y`
  - `CONFIG_VISION_APP_ENABLE_PREVIEW_IMAGE=y`
  - `CONFIG_VISION_APP_ENABLE_INFERENCE=y`
  - LCD 继续 RGB24。
- 提交：`b2dcd66 test full features ethernet off`。
- `idf.py build` 通过。
- 按用户要求没有烧录。

## 2026-06-27 Initial Version With Ethernet Off Flash

- 用户观察到 `b2dcd66` 版本 preview 白屏，要求取最开始的 git 版本，只单独停掉 Ethernet，然后构建烧录。
- 操作：
  - 从最早提交 `c8ec469 debug lcd underrun blue screen diagnostics` 创建分支 `test-initial-ethernet-off`。
  - 只修改 `main/system_init.c`：注释 `ethernet_app_start()`。
  - 不改 preview/camera/inference/LCD 配置。
- 提交：`697036a test initial version ethernet off`。
- 说明：
  - 最早提交的 preview 初始化路径是 `System_Init()` 中直接调用 `vision_preview_init(...)`，没有后续 `CONFIG_VISION_APP_ENABLE_PREVIEW_IMAGE` 开关路径。
  - 最早提交的 LCD DPI 配置为 `48 MHz`，不是后续诊断基准 `40 MHz`。
- 验证：
  - `idf.py build` 通过。
  - `idf.py -p /dev/ttyACM0 flash` 通过。
  - 按用户要求未启动 monitor。
- 当前板上固件：branch `test-initial-ethernet-off`，commit `697036a`。

## 2026-06-27 Ethernet Impact Investigation

- 用户最新肉眼观察更新：
  - 单独停掉 camera 链路：无蓝屏。
  - 停掉 Ethernet 且 preview 是白色的版本：无蓝屏。
  - 停掉 Ethernet 但其他功能全开：有蓝屏。
- 结论修正：
  - Ethernet 不是蓝屏必要触发条件。
  - camera/CSI/ISP 链路仍是最强必要触发因素。
  - preview 白屏版本的无蓝屏不能证明 Ethernet 是根因，因为 preview 异常可能已经降低显示/相机/拷贝压力。
- 代码调查：
  - `ethernet_app_start()` 会创建 EMAC/IP101 driver、esp-netif、默认 event loop、netif glue、静态 IP、ETH/IP event handler，然后 `esp_eth_start()`。
  - `start_tcp_client_task()` 创建 `eth_tcp` task，core 0，priority 4，stack 8192。
  - TCP 连接失败时每 2 秒重试，不会走图像发送，但 EMAC/LwIP/事件/任务/中断仍存在。
  - TCP 连接成功后当前 `bug_project` 每 10 秒调用一次 `send_metrics_packet()` 和 `send_image_packet()`。
  - `send_image_packet()` 会额外 `cam_sensor_get_frame()`，在 PSRAM 中分配/保留 `s_image_copy_buf`，并 `memcpy()` 整帧后通过 socket 发送。
  - 640x480 RGB888 图像约 900 KB；该路径会制造额外 camera frame 竞争和 PSRAM 大块读写突发。
  - ETH DMA 配置为 512B buffer、20 RX、10 TX；更像内部/DMA memory 和中断/DMA 总线压力，不是主要 PSRAM 常驻占用。
  - `CONFIG_LWIP_TCPIP_TASK_PRIO=18`，LwIP task 优先级高，可能抢占应用层任务。
  - `CONFIG_SPIRAM_TRY_ALLOCATE_WIFI_LWIP is not set`，LwIP 小 buffer 默认不主动放 PSRAM。
- `merge_project` 对照：
  - ETH DMA/LwIP 基础配置基本相同。
  - `TCP_SEND_INTERVAL_MS=300000`，而 `bug_project` 当前为 10000。
  - 当前 `merge_project` 还有 socket writable timeout/失败重连相关改动。
  - 因此改善更可能来自降低/避免应用层 TCP 图像发送突发，而不是 EMAC 基础配置变化。
- 本次只调查并更新文档，没有修改固件代码，未运行 build/flash。

## 2026-06-28 Blue Mark Arbiter Matrix

- 用户要求：
  - 阅读现有文档和 `reference.md`，基于带大号 `BLUE MARK` 按钮的版本继续。
  - 已连接串口，要求烧录测试，多测几种方式并记录。
- Git 基线：
  - 进入 `bug_project`。
  - 找到带大号 BLUE MARK 的版本：`78ec4d9 checkpoint blue screen diagnostics`。
  - 从该提交创建/使用分支：`test-blue-mark-arbiter-matrix`。
- 新增诊断和配置代码：
  - `b022de4 add psram arbiter diagnostics`
  - 新增 `components/bsp/bsp_psram_arbiter.c` 和头文件。
  - 新增 Kconfig：`BSP_PSRAM_ARB_TUNING`、`BSP_PSRAM_ARB_DISPLAY_FIRST`、`BSP_PSRAM_ARB_PSRAM_READ_FIRST`、`BSP_PSRAM_ARB_CAMERA_WRITE_REGULATOR` 及 regulator 参数。
  - `System_Init()` 早期调用 `BSP_PSRAM_ArbiterApplyConfigured()`。
  - BLUE_MARK 时调用 `BSP_PSRAM_ArbiterLogSnapshot("blue_mark")`，记录 ICM/PSRAM 寄存器回读和解码。
- A0 baseline：
  - commit：`78ec4d9`
  - log：`docs/agent/run_logs/2026-06-28-A0-baseline.log`
  - `idf.py build` 通过。
  - `idf.py -p /dev/ttyACM0 flash monitor` 通过。
  - 约 60 秒内未见 `underrun happens`；camera `timeouts=0`，`max_receive` 约 58ms。
- A1 display-first：
  - commit：`723e7b4 test arbiter display first`
  - log：`docs/agent/run_logs/2026-06-28-A1-display-first.log`
  - `idf.py build` 通过。
  - `idf.py -p /dev/ttyACM0 flash monitor` 通过。
  - 约 170 秒内多次 BLUE_MARK；camera `timeouts=0`，`max_receive` 约 39ms；未见 `underrun happens`。
  - 回读：`mst=0x400f45a4 slv=0x00038000 arqos=0x400f45b4 awqos=0x40023564 psram_fctrl=0x40000001`。
- A2 display-first + PSRAM read-first：
  - commit：`ad08426 test arbiter display and psram read first`
  - log：`docs/agent/run_logs/2026-06-28-A2-display-read-first.log`
  - `idf.py build` 通过。
  - `idf.py -p /dev/ttyACM0 flash monitor` 通过。
  - 回读：`psram_fctrl=0x4004c601`，read-first 生效。
  - 约 16.6 秒开始连续 `CSI: csi recv API, transaction queue is full`。
  - 约 33.9 秒 BLUE_MARK 时 camera `timeouts=86`，停止前 timeout 增长到 120+。
  - 结论：PSRAM read-first 会导致 camera/CSI 失稳，不作为候选修复。
- A3 display-first + camera write regulator：
  - commit：`6925dc4 test arbiter camera write regulator`
  - log：`docs/agent/run_logs/2026-06-28-A3-display-camera-regulator.log`
  - `idf.py build` 通过。
  - `idf.py -p /dev/ttyACM0 flash monitor` 通过。
  - 约 110 秒内多次 BLUE_MARK；第 4 次时 camera `count=492 timeouts=0 last_receive=38ms max_receive=39ms`。
  - 未见 `CSI queue full`，未见 `underrun happens`。
  - 当前板上固件停留在 `6925dc4`。

## 2026-06-28 User Observation Correction

- 用户指出两点：
  - 本轮几个版本 LCD preview 都是黑的，意味着功能不全。
  - 当前板上 `6925dc4` 蓝屏很多，不能称为稳定。
- 确认：
  - 当前 `sdkconfig` 中 `CONFIG_VISION_APP_ENABLE_PREVIEW_IMAGE` 未开启。
  - `VISION_DIAG preview=0/3` 代表 preview 控件存在，但图像更新被禁用。
  - 因此 preview 黑屏/不更新是本轮带宽诊断配置的预期结果，但这些版本不是完整功能版本。
- 结论修正：
  - A3 只能称为 camera/CSI 指标稳定：`timeouts=0`、无 CSI queue full、无日志中的 LCD underrun。
  - A3 不能称为屏幕稳定或修复候选已通过，因为用户肉眼观察到蓝屏很多。
  - 下一步不能继续用“无 camera timeout/无 underrun 日志”替代“无可见蓝屏”；需要调整 regulator 参数或继续降低显示/相机突发，并恢复 preview/full feature 做独立验证。

## 2026-06-28 Reference Plan Review

- 用户要求：
  - 阅读根目录 `reference.md`，不要完全听信 gptpro 建议。
  - 对照本工程判断哪些值得做、哪些不值得做。
  - 写出根目录 `plan.md`，并清空 `reference.md`。
- 已完成：
  - 创建 `plan.md`。
  - 清空 `reference.md`。
- 对 `reference.md` 的主要取舍：
  - 采纳：完整功能 baseline、preview/inference 真实负载、pipeline 诊断、PPA trace、BLUE_MARK dump、Ethernet raw image 默认关闭、保留 display-first、关闭 psram read-first。
  - 调整后采纳：先测当前 640x480 完整功能 baseline，再回到 1024x600 产品目标压力；先做轻量 PPA 串行化 A/B，不先重构复杂 scheduler；Camera Owner Task 延后到证据指向多 consumer 后。
  - 暂不优先：立即做 ISP crop/small frame/camera RGB565、Ethernet thumbnail/raw 多模式、大规模 Camera Owner Task 重构。
  - 明确不做：继续使用 `PSRAM_READ_FIRST`、CSI priority 0、把 A3 称为稳定版本、用 preview 黑屏版本判断功能完整性、把 Ethernet 当作唯一根因。
- 新计划核心顺序：
  - Phase 1：补 `[PIPE_DIAG]`、`[PPA_TRACE]`、BLUE_MARK dump、frame id、Ethernet raw image 开关。
  - Phase 2：B0 当前 640x480 完整功能 baseline，preview on、inference real run、Ethernet raw off、LCD RGB24。
  - Phase 3：B1 只改 LCD RGB565。
  - Phase 4-9：preview fps、YOLO fps、PPA 串行化、camera 降载、Ethernet 回归、长时间 soak。
- 本次只整理计划和文档，没有修改 `bug_project` 固件代码，未运行 build/flash。

## 2026-06-28 Phase 1 Diagnostics And B0/B1 Tests

- 用户要求：
  - 执行 `plan.md`，继续测试并记录。
  - 直接烧录，用户看到蓝屏会按板上按钮。
- 当前工程：
  - `bug_project`
  - branch `test-blue-mark-arbiter-matrix`

### Phase 1 Diagnostic Implementation

- commit：`8ed68c2 test b0 pipeline diagnostics`
- 新增文件：
  - `components/bsp/include/bsp_pipeline_diag.h`
  - `components/bsp/bsp_pipeline_diag.c`

## 2026-06-28 Y0 Preview-Load No-Inference Test

- 用户修正测试标准：
  - 先不管 LCD 上 preview 视觉链路是否真正正常。
  - 只要性能上走了 preview 链路、对 PSRAM/PPA/LVGL 有真实压力，实验就有效。
  - 记录仍必须区分 `preview load active` 和 `preview visual validated`。
- commit：`3e87962 test y0 preview load no inference`
- 日志：`docs/agent/run_logs/2026-06-28-Y0-rgb565-preview-no-inference.log`
- 配置：
  - LCD RGB565，`active_read=55738KB/s`。
  - camera stream enabled，`640x480 RGB888`。
  - `CONFIG_VISION_APP_ENABLE_PREVIEW_IMAGE=y`，每 3 帧 preview update。
  - `CONFIG_VISION_APP_ENABLE_INFERENCE` disabled。
  - Ethernet raw image packet disabled。
- 验证：
  - `idf.py build` 通过。
  - `idf.py -p /dev/ttyACM0 flash monitor` 通过，monitor 已停止。
- 实测：
  - 启动日志确认 `YOLO inference disabled for bandwidth A/B test`。
  - preview 性能链路 active：常规 `[PIPE_DIAG] preview=2/2 preview_ppa=2`，PPA trace 为 `kind=preview src=640x480 dst=640x375 ret=ESP_OK`。
  - BLUE_MARK #1：`time_us=6698435 frame=9 camera_timeouts=0 model_runs=0 preview_ok=3/3 yolo_ppa=0 preview_ppa=3`。
  - BLUE_MARK #2：`time_us=71903443 frame=334 camera_timeouts=0 model_runs=0 preview_ok=111/111 yolo_ppa=0 preview_ppa=111`。
  - BLUE_MARK #3：`time_us=91193433 frame=428 camera_timeouts=0 model_runs=0 preview_ok=142/142 yolo_ppa=0 preview_ppa=142`。
  - 约 `93.107s` 后开始出现 `CSI: csi recv API, transaction queue is full` 和 capture timeout。
  - BLUE_MARK #4：`time_us=111214420 frame=429 camera_timeouts=19 model_runs=0 preview_ok=143/143 yolo_ppa=0 preview_ppa=143`。
  - 停止前 capture timeout count 增长到 180，持续 `camera frame timeout` 和 CSI queue full。
  - 未见实际 LCD `underrun happens` 日志；日志里出现的 `underrun happens` 是 `LCD_DIAG` 提示文本。
- 结论：
  - Y0 是 `preview load active, visual preview not validated` 测试。
  - 无 YOLO/model、无 YOLO PPA 时仍出现多次 BLUE_MARK，YOLO/model 不是蓝屏必要触发因素。
  - Y0 的 #1/#2/#3 发生时 camera timeout 仍为 0；#4 已经是 camera/CSI 失稳后的蓝屏，需要和前三次分开看。
  - 下一步转向 Phase C：camera stream/write 降载。

## 2026-06-28 C0 RGB565 Camera-Off Test

- commit：`f1f95e4 test c0 rgb565 camera off`
- 日志：`docs/agent/run_logs/2026-06-28-C0-rgb565-camera-off.log`
- 配置：
  - LCD RGB565，`active_read=55738KB/s`。
  - `CONFIG_VISION_APP_ENABLE_CAMERA_STREAM` disabled。
  - `CONFIG_VISION_APP_ENABLE_PREVIEW_IMAGE` disabled。
  - `CONFIG_VISION_APP_ENABLE_INFERENCE` disabled。
  - Ethernet raw image packet disabled。
- 验证：
  - `idf.py build` 通过，app size `0x1fcfd0`。
  - `idf.py -p /dev/ttyACM0 flash` 通过。
  - `idf.py -p /dev/ttyACM0 flash monitor` 中 monitor 因非 TTY 失败，随后用 `script -q -f -a ... -c "idf.py -p /dev/ttyACM0 monitor"` 成功进入 monitor。
- 实测：
  - 启动确认 app version `f1f95e4`。
  - LCD：`1024x600 RGB16 dpi=40MHz refresh=46.449Hz fbs=3 fb_size=1200KB dma2d=1`。
  - 启动日志：`camera stream disabled for bandwidth A/B test`。
  - 约 90 秒 monitor 内无新日志输出，无 BLUE_MARK、无 CSI queue full、无 camera timeout、无实际 LCD `underrun happens`。
- 结论：
  - C0 再次支持：无 camera/CSI/ISP 写 PSRAM 时，当前 RGB565/UI 基线没有复现蓝屏。
  - 下一步 C1：开启 camera stream，但把 `CONFIG_CAM_SENSOR_STREAM_DUTY_PERIOD_MS` 从 200ms 拉到 400ms，preview/inference 继续关闭。

## 2026-06-28 C1 RGB565 Camera Half-Duty Test

- commit：`9ba705c test c1 rgb565 camera half duty`
- 日志：`docs/agent/run_logs/2026-06-28-C1-rgb565-camera-half-duty.log`
- 配置：
  - LCD RGB565，`active_read=55738KB/s`。
  - camera stream enabled，`640x480 RGB888`。
  - `CONFIG_CAM_SENSOR_STREAM_DUTY_PERIOD_MS=400`。
  - `CONFIG_VISION_APP_ENABLE_PREVIEW_IMAGE` disabled。
  - `CONFIG_VISION_APP_ENABLE_INFERENCE` disabled。
  - Ethernet raw image packet disabled。
- 验证：
  - `idf.py build` 通过，app size `0x205c50`。
  - `script -q -f ... -c "idf.py -p /dev/ttyACM0 flash monitor"` 完成 flash + monitor。
- 实测：
  - 启动确认 app version `9ba705c`。
  - LCD：`1024x600 RGB16 dpi=40MHz refresh=46.449Hz fbs=3 fb_size=1200KB dma2d=1`。
  - camera 诊断：`active_write=1800KB/s frame=900KB fps=50 write_fps=2 period=400ms duty=400ms buffers=3`。
  - preview/inference 确认关闭：常规 `[PIPE_DIAG] model_run=0 yolo_ppa=0 preview=0/0 preview_ppa=0`。
  - BLUE_MARK #1：`time_us=34926425 frame=75 total_frames=75 camera_timeouts=0 model_runs=0 preview_ok=0/0 yolo_ppa=0 preview_ppa=0`。
  - BLUE_MARK #1 时 camera 诊断：`active_write=1800KB/s count=76 timeouts=0 last_receive=38ms max_receive=38ms age=167ms`。
  - BLUE_MARK #1 的 PPA trace：`recent count=0`。
  - 未见实际 LCD `underrun happens`；日志中的 `underrun happens` 是 `LCD_DIAG` 提示文本。
  - 约 `82.283s` 后开始 `CSI: csi recv API, transaction queue is full` 和 capture timeout。
  - 停止前 capture timeout count 到 120，并持续 `vision_app: camera frame timeout`。
- 结论：
  - C1 证明：即使 average camera write 降到 `1800KB/s`，且没有 preview update、没有 YOLO/model、没有 PPA，仍能复现用户可见蓝屏。
  - 第一次蓝屏发生在 camera/CSI timeout=0 阶段，应和后段 CSI queue full 失稳分开看。
  - 仅降低平均 camera write rate 不足以消除蓝屏；后续要继续降低 camera 写入/输出压力，或确认单帧 900KB RGB888 写入突发是否为关键因素。

## 2026-06-28 C1b RGB565 Camera Duty 800 Test

- 用户要求继续测试，且 preview 视觉正确性不作为阻塞；只要性能链路或 PSRAM 压力真实存在，就记录为有效压力测试。
- commit：`c990bdf test c1b rgb565 camera duty 800`
- 日志：`docs/agent/run_logs/2026-06-28-C1b-rgb565-camera-duty800.log`
- 变更：
  - 只把 `CONFIG_CAM_SENSOR_STREAM_DUTY_PERIOD_MS` 从 400ms 改到 800ms。
  - LCD RGB565 保持不变。
  - camera stream enabled，640x480 RGB888。
  - inference disabled。
  - preview update disabled。
  - Ethernet raw image disabled。
- 验证：
  - `idf.py build` 通过，app version `c990bdf`，binary size `0x205c50`。
  - `script -q -f ../docs/agent/run_logs/2026-06-28-C1b-rgb565-camera-duty800.log -c "idf.py -p /dev/ttyACM0 flash monitor"` 成功。
- 关键日志：
  - LCD：`1024x600 RGB16 dpi=40MHz refresh=46.449Hz fbs=3 fb_size=1200KB dma2d=1`。
  - LCD bandwidth：`active_read=55738KB/s`。
  - camera：`active_write=900KB/s frame=900KB fps=50 write_fps=1 period=800ms duty=800ms buffers=3`。
  - inference/preview/PPA 关闭：常规 `[PIPE_DIAG] model_run=0 yolo_ppa=0 preview=0/0 preview_ppa=0`，BLUE_MARK 时 `[PPA_TRACE] recent count=0`。
  - BLUE_MARK #1：`time_us=6090434 frame=2 camera_timeouts=0 model_runs=0 preview_ok=0/0 yolo_ppa=0 preview_ppa=0`。
  - BLUE_MARK #2：`time_us=71072430 frame=83 camera_timeouts=0 model_runs=0 preview_ok=0/0 yolo_ppa=0 preview_ppa=0`。
  - BLUE_MARK #3：`time_us=90953425 frame=108 camera_timeouts=0 model_runs=0 preview_ok=0/0 yolo_ppa=0 preview_ppa=0`。
  - BLUE_MARK #4：`time_us=110897429 frame=133 camera_timeouts=0 model_runs=0 preview_ok=0/0 yolo_ppa=0 preview_ppa=0`。
  - 停止前约 168 秒：`frame=205 cam_timeout=0/0 max_cam_wait=798ms model_run=0 yolo_ppa=0 preview=0/0 preview_ppa=0 blue=4`。
  - 全程未见 CSI queue full / camera frame timeout。
  - 全程未见实际 LCD `underrun happens`；只有启动和 BLUE_MARK 诊断提示文本。
- 结论：
  - 把 average camera write 从 C1 的 `1800KB/s` 再降到 `900KB/s`，仍在 camera 稳定阶段快速复现多次用户可见蓝屏。
  - 继续只拉长 capture duty 的收益有限；下一步 C2 应降低 camera 单帧输出带宽或分辨率。
- 早前诊断代码修改清单：
  - `components/bsp/CMakeLists.txt`
  - `components/bsp/bsp_lcd.c`
  - `components/vision_app/vision_app.cpp`
  - `components/vision_app/vision_preview.cpp`
  - `components/vision_app/vision_preview.hpp`
  - `components/Ethernet_app/Kconfig`
  - `components/Ethernet_app/ethernet_app.c`
  - `sdkconfig`
- 功能：
  - `[PIPE_DIAG]` 每秒输出 camera、ROI、model、preview、PPA、BLUE_MARK 汇总。
  - `[PPA_TRACE]` 记录 YOLO preprocess 和 preview PPA 的 frame id、尺寸、耗时、结果。
  - BLUE_MARK 时 dump 最近 pipeline/PPA 状态。
  - Ethernet metrics 保留，raw image packet 默认关闭。
- `idf.py build` 通过。

### B0 Natural Full-Pipeline Baseline

- commit：`8ed68c2`
- log：`docs/agent/run_logs/2026-06-28-B0-pipeline-full-baseline.log`
- 配置：
  - A3 arbiter 保留。
  - preview image update enabled。
  - inference enabled。
  - Ethernet raw image disabled。
  - LCD RGB24。
- 结果：
  - LCD：`1024x600 RGB24 dpi=40MHz`，`fb_size=1800KB`，`active_read=83608KB/s`。
  - preview 正常更新：BLUE_MARK #3 时 `preview_ok=146/146`，`preview_ppa=146`。
  - ROI 一直 skip，模型没有真实运行：`model_runs=0`，`yolo_ppa=0`，`run=0`。
  - camera 稳定：`timeouts=0`，未见 CSI queue full。
  - 未见 `underrun happens`。
- 结论：
  - 该版本证明 preview-on 诊断链路正常。
  - 但不是完整 inference 负载 baseline，因为模型没有实际运行。

### B0 Force-Inference Baseline

- commit：`a28948c test b0 force inference load`
- log：`docs/agent/run_logs/2026-06-28-B0-force-inference.log`
- 修改：
  - 新增 `CONFIG_VISION_APP_FORCE_INFERENCE_ON_ROI_SKIP`。
  - ROI skip 时强制用 full frame 跑 preprocess/model。
- `idf.py build` 通过。
- `idf.py -p /dev/ttyACM0 flash monitor` 已执行。
- 结果：
  - LCD：RGB24，`fb_size=1800KB`，`active_read=83608KB/s`。
  - 启动后第一次 multi-core model run 耗时 `1264ms`，随后 fallback 到 single-core runtime。
  - 后续约 `1.5-1.6 fps`。
  - 每秒约 `model_run=2`，`yolo_ppa=2`，preview 每 3 帧更新一次。
  - BLUE_MARK #1：`time_us=7892273 frame=2 model_runs=1 preview_ok=0/0 camera_timeouts=0`。
  - BLUE_MARK #3：`time_us=151948274 frame=222 camera_timeouts=0 roi_ok=0 roi_skip=221 model_runs=221 preview_ok=73/73 yolo_ppa=221 preview_ppa=73`。
  - camera 稳定：`timeouts=0`，`max_receive=58ms`。
  - 未见 CSI queue full，未见 `underrun happens`。
- 结论：
  - 这是 RGB24 下 preview + full forced inference 的有效基线。
  - 仍有用户可见蓝屏/BLUE_MARK，因此不是显示稳定版本。

### B1 RGB565 Force-Inference Test

- commit：`6422f39 test b1 rgb565 force inference`
- log：`docs/agent/run_logs/2026-06-28-B1-rgb565-force-inference.log`
- 修改：
  - `bsp_lcd.c` 根据 `CONFIG_LV_COLOR_DEPTH` 配置 DPI `bits_per_pixel` 和 `in_color_format`。
  - `sdkconfig` / `sdkconfig.defaults` 切到 `CONFIG_LV_COLOR_DEPTH=16`。
- `idf.py build` 通过。
- `idf.py -p /dev/ttyACM0 flash monitor` 首次 flash 成功，但 monitor 因 stdin 非 TTY 失败。
- 随后单独运行 `idf.py -p /dev/ttyACM0 monitor` 并保存日志，monitor 正常，最后用 Ctrl-] 退出。
- 结果：
  - LCD：`1024x600 RGB16 dpi=40MHz refresh=46.449Hz fbs=3 fb_size=1200KB dma2d=1`。
  - LCD `active_read=55738KB/s`，相对 RGB24 的 `83608KB/s` 明确下降约 1/3。
  - B1 没有触发 B0-force 的 multi-core fallback，运行约 `5 fps`。
  - 常规 `run_avg` 约 `48-55ms`，`pre_avg` 约 `22-26ms`，preview PPA 通常约 `7ms`。
  - BLUE_MARK #1：`time_us=7334803 frame=7 camera_timeouts=0 model_runs=7 preview_ok=2/2 yolo_ppa=7 preview_ppa=2`。
  - BLUE_MARK #2：`time_us=129822775 frame=618 camera_timeouts=0 model_runs=618 preview_ok=206/206 yolo_ppa=618 preview_ppa=206`。
  - BLUE_MARK #3：`time_us=169440710 frame=813 camera_timeouts=0 model_runs=813 preview_ok=271/271 yolo_ppa=813 preview_ppa=271`。
  - camera 稳定：`timeouts=0`，`max_receive=40ms`。
  - 未见 CSI queue full，未见 `underrun happens`。
- 结论：
  - RGB565 已经真实降低 LCD scanout 带宽。
  - RGB565 单独不足以消除可见蓝屏；本轮约 170 秒内仍有至少 3 次 BLUE_MARK。
  - B1 与 B0-force 不是纯 LCD-only 运行时对照，因为 B0-force fallback 到 single-core、B1 保持 multi-core，负载形态不同。

### Current Board State

- 当前板上烧录版本：`6422f39 test b1 rgb565 force inference`。
- 当前结论：
  - camera/CSI 在 B0-force 和 B1 中都稳定。
  - 显示仍不稳定。
  - 下一步应继续 `plan.md` Phase 4-6，优先测试 preview 更新频率、YOLO 频率、PPA 串行化。

## 2026-06-28 P0 No-Preview Test And Preview Chain Clarification

- 用户要求继续按 `plan.md` 测试，随后打断并要求先确认 preview 链路：
  - 用户现场观察到最近 LCD preview 没有真正稳定显示摄像头图像。
  - 现象包括黑、白、黑白参半。
  - 要求确认当前状态能否说明 preview 链路真的在走；如果没有真正 preview，必须记录为没开，并停止继续测试，整理下一阶段计划。

### P0 RGB565 Force-Inference No-Preview Test

- commit：`ea24f0f test p0 rgb565 no preview`
- log：`docs/agent/run_logs/2026-06-28-P0-rgb565-force-no-preview.log`
- 配置：
  - 基于 B1。
  - `# CONFIG_VISION_APP_ENABLE_PREVIEW_IMAGE is not set`
  - LCD RGB565 保持。
  - force inference 保持。
  - Ethernet raw image disabled 保持。
- `idf.py build` 通过。
- `idf.py -p /dev/ttyACM0 flash monitor` 通过，monitor 已用 Ctrl-] 停止。
- 结果：
  - LCD：`1024x600 RGB16 dpi=40MHz`，`fb_size=1200KB`，`active_read=55738KB/s`。
  - preview update 没开：全程 `preview=0/0`、`preview_ppa=0`、`images=0`。
  - BLUE_MARK #1：`time_us=9101816 frame=17 camera_timeouts=0 model_runs=16 preview_ok=0/0 yolo_ppa=16 preview_ppa=0`。
  - BLUE_MARK #2：`time_us=13922915 frame=40 camera_timeouts=0 model_runs=39 preview_ok=0/0 yolo_ppa=39 preview_ppa=0`。
  - BLUE_MARK #3：`time_us=35338756 frame=144 camera_timeouts=0 model_runs=143 preview_ok=0/0 yolo_ppa=143 preview_ppa=0`。
  - camera 稳定：BLUE_MARK #3 时 `count=151 timeouts=0 last_receive=38ms max_receive=38ms`，停止前 capture #240 仍 `timeouts=0`。
  - 未见 CSI queue full，未见 `underrun happens`。
  - #2 后出现一次 fallback：`Multi-core run took 713ms; falling back to single-core runtime`。
- 结论：
  - P0 是 preview update disabled 测试，不能说 preview 链路在走。
  - 即使 `preview_ppa=0`，仍然 35 秒内 3 次 BLUE_MARK，因此 preview update/PPA 不是蓝屏必要触发因素。
  - 当前板上烧录版本已变为 `ea24f0f`。

### Preview Chain Clarification

- 无残留 `idf.py` / `idf_monitor` / `tee run_logs` / `esptool` 进程。
- 当前必须把 preview 分成三层记录：
  - preview 控件初始化。
  - preview 性能链路执行。
  - LCD 上视觉 preview 是否真实有效。
- 代码确认：
  - `System_Init()` 调用 `vision_preview_init(scr_dashboard_cont_live_vedio, 0, 0, 640, 375)`。
  - `vision_preview_init()` 分配两个 RGB565 PSRAM buffer，并先清零；所以首帧前黑屏是初始化行为。
  - `vision_preview_update()` 才执行 RGB888 camera frame -> RGB565 preview buffer 的 PPA 转换、cache flush、`lv_img_set_src()` 和 `lv_obj_invalidate()`。
  - `vision_preview_update()` 只有在 `CONFIG_VISION_APP_ENABLE_PREVIEW_IMAGE=y` 且达到 `CONFIG_VISION_PREVIEW_IMAGE_EVERY_N_FRAMES` 时才会调用。
  - preview image 初始化后调用 `lv_obj_move_background()`，放到父容器底层；存在“性能链路更新了，但视觉上被 UI 背景/容器遮挡或显示不正确”的风险。
- 日志确认：
  - B0 natural 和 B1 日志里有 `preview_ok>0`、`preview_ppa>0`、`PPA_TRACE kind=preview src=640x480 dst=640x375 ret=ESP_OK`。
  - 因此 B0/B1 的 preview 性能链路确实在跑，产生 PPA/PSRAM/LVGL source 切换负载。
  - 但这些日志不能证明 LCD 上显示的是正确摄像头画面。
- 结论修正：
  - B0/B1 记录为 `preview update/load active, visual preview not validated`。
  - P0 记录为 `preview update disabled`。
  - 用户现场观察的黑/白/黑白参半不是正常产品 preview，下一阶段必须先做 preview 视觉链路确认，不再把 B0/B1 称为完整功能 preview 正常。

## 2026-06-28 C2 RGB565 Camera Output Test

- commit：`f2ac998 test c2 rgb565 camera output`
- log：`docs/agent/run_logs/2026-06-28-C2-rgb565-camera-output.log`
- 配置：
  - LCD RGB565。
  - camera stream enabled。
  - camera sensor 仍为 `MIPI_2lane_24Minput_RAW10_640x480_50fps`。
  - camera output 改为 RGB565，`CONFIG_CAM_SENSOR_OUTPUT_RGB565=y`。
  - `CONFIG_CAM_SENSOR_STREAM_DUTY_PERIOD_MS=800`。
  - inference off。
  - preview update off。
- `idf.py build` 通过。
- `script -q -f ../docs/agent/run_logs/2026-06-28-C2-rgb565-camera-output.log -c "idf.py -p /dev/ttyACM0 flash monitor"` 已执行，monitor 用 Ctrl-] 停止。
- 结果：
  - App version：`f2ac998`。
  - LCD：`1024x600 RGB16 dpi=40MHz refresh=46.449Hz fbs=3 fb_size=1200KB dma2d=1`。
  - LCD `active_read=55738KB/s`。
  - camera：`cam_sensor_init OK [640x480 RGB565, 600 KB/frame]`。
  - camera active write：`active_write=600KB/s frame=600KB fps=50 write_fps=1 period=800ms duty=800ms buffers=3`。
  - preview/inference/PPA off：常规日志和 BLUE_MARK dump 均 `model_run=0 yolo_ppa=0 preview=0/0 preview_ppa=0`，`PPA_TRACE recent count=0`。
  - BLUE_MARK #1：`time_us=32220423 frame=34 total_frames=34 camera_timeouts=0 model_runs=0 preview_ok=0/0 yolo_ppa=0 preview_ppa=0`。
  - BLUE_MARK #1 时 camera：`active_write=600KB/s count=35 timeouts=0 last_receive=38ms max_receive=38ms age=289ms`。
  - 约 `225.846s` 前最后一次稳定诊断仍为 `frame=277 cam_timeout=0/0 max_cam_wait=800ms model_run=0 preview=0/0 preview_ppa=0 blue=1`。
  - 约 `229.069s` 开始 `CSI: csi recv API, transaction queue is full` 和 `capture timeout count=1`，停止前 timeout count 到 120。
  - 未见实际 LCD `underrun happens` 日志；只见诊断提示文本中的 `underrun happens`。
- 判定：
  - 将 camera 单帧 PSRAM 写入从 RGB888 `900KB/frame` 降到 RGB565 `600KB/frame`，且平均 `active_write=600KB/s`，仍出现 BLUE_MARK。
  - C2 的 BLUE_MARK #1 发生时 camera/CSI 尚未 timeout，且 preview/YOLO/PPA 均未运行。
  - C2 后段 CSI queue full 属于另一个后期 camera 接收异常，应和 #1 蓝屏分开记录。

## 2026-06-28 C3 RGB565 Camera Output Duty 1600 Start

- 基于 C2。
- 只把 `CONFIG_CAM_SENSOR_STREAM_DUTY_PERIOD_MS` 从 800 改到 1600。
- 目标 active write：约 `300KB/s`。
- preview/inference 继续关闭；LCD RGB565、camera output RGB565 保持。
- 第一次 `idf.py build` 暴露 Kconfig 限制：`CAM_SENSOR_STREAM_DUTY_PERIOD_MS` 原 range 为 `[0, 1000]`，1600 被忽略并回退默认值；该 build 不作为有效 C3。
- 第一次无效 build 后 `sdkconfig` 被写回 `CONFIG_CAM_SENSOR_STREAM_DUTY_PERIOD_MS=0`；随后一次快速 build 仍不是有效 C3。
- 已把 `components/bsp/Kconfig` 中 `CAM_SENSOR_STREAM_DUTY_PERIOD_MS` range 扩到 `0 2000`，并把 `sdkconfig` 恢复为 1600；需要重新 build/flash。
- 有效 C3 `idf.py build` 已通过，未再出现 1600 超范围警告，binary size `0x205c50`。
- commit：`7e03fda test c3 rgb565 camera duty 1600`
- log：`docs/agent/run_logs/2026-06-28-C3-rgb565-camera-duty1600.log`
- `script -q -f ../docs/agent/run_logs/2026-06-28-C3-rgb565-camera-duty1600.log -c "idf.py -p /dev/ttyACM0 flash monitor"` 已执行，monitor 用 Ctrl-] 停止。
- 结果：
  - App version：`7e03fda`。
  - LCD：`active_read=55738KB/s`。
  - camera：`cam_sensor_init OK [640x480 RGB565, 600 KB/frame]`。
  - 启动日志确认 `period=1600ms duty=1600ms`，`Streaming started ... duty=1600ms`。
  - 诊断打印仍显示 `active_write=600KB/s frame=600KB write_fps=1 period=1600ms duty=1600ms`；但 PIPE/VISION fps 约 `0.62`，说明有效帧节奏约 1600ms，`active_write` 字段在该低频下不应直接当作精确平均写带宽。
  - C3 周期性出现 `vision_app: camera frame timeout`，因为 vision 取帧等待短于 1600ms duty；这不是 cam_sensor/CSI 层 timeout。
  - BLUE_MARK #1：`time_us=62689422 frame=37 total_frames=37 camera_timeouts=36 model_runs=0 preview_ok=0/0 yolo_ppa=0 preview_ppa=0`；cam_sensor at mark：`count=37 timeouts=0 last_receive=38ms max_receive=38ms age=356ms`；`PPA_TRACE recent count=0`。
  - BLUE_MARK #2：`time_us=82517418 frame=49 total_frames=49 camera_timeouts=48 model_runs=0 preview_ok=0/0 yolo_ppa=0 preview_ppa=0`；cam_sensor at mark：`count=49 timeouts=0 last_receive=38ms max_receive=38ms age=969ms`；`PPA_TRACE recent count=0`。
  - BLUE_MARK #3：`time_us=102294417 frame=61 total_frames=61 camera_timeouts=61 model_runs=0 preview_ok=0/0 yolo_ppa=0 preview_ppa=0`；cam_sensor at mark：`count=61 timeouts=0 last_receive=38ms max_receive=38ms age=1548ms`；`PPA_TRACE recent count=0`。
  - 停止前：`uptime=128245ms frame=78 ... blue=3`。
  - 未见 `CSI: csi recv API, transaction queue is full`。
  - 未见实际 LCD `underrun happens` 日志；只见诊断提示文本中的 `underrun happens`。
- 判定：
  - C3 仍可复现蓝屏。
  - C3 的 BLUE_MARK 发生时 preview/YOLO/PPA 均关闭，cam_sensor/CSI 层没有 timeout。
  - 需要把 vision_app 周期性取帧 timeout 作为 C3 副作用单独记录，不能和 cam_sensor/CSI queue full 混淆。

## 2026-06-28 C4 Camera Stream Only No Vision Task Start

- 基于 C3。
- 新增 `CONFIG_VISION_APP_CAMERA_ONLY_NO_TASK`。
- 启用后 `vision_app_start()` 仍执行 `init_camera()` 和 `cam_sensor_start()`，但不创建 `vision_task`，因此不调用 `cam_sensor_get_frame()`。
- 当前 C4 配置：
  - LCD RGB565。
  - camera stream enabled。
  - camera output RGB565。
  - `CONFIG_CAM_SENSOR_STREAM_DUTY_PERIOD_MS=1600`。
  - inference off。
  - preview update off。
  - `CONFIG_VISION_APP_CAMERA_ONLY_NO_TASK=y`。
- 目的：分离 camera/CSI 硬件写入和应用层 vision 取帧/timeout 循环。
- `idf.py build` 已通过，binary size `0x202fb0`。
- commit：`037f341 test c4 camera stream without vision task`
- log：`docs/agent/run_logs/2026-06-28-C4-camera-only-no-vision-task.log`
- `script -q -f ../docs/agent/run_logs/2026-06-28-C4-camera-only-no-vision-task.log -c "idf.py -p /dev/ttyACM0 flash monitor"` 已执行，monitor 用 Ctrl-] 停止。
- 结果：
  - App version：`037f341`。
  - LCD：`active_read=55738KB/s`。
  - camera：`active_write=600KB/s frame=600KB fps=50 write_fps=1 period=1600ms duty=1600ms buffers=3`。
  - camera：`cam_sensor_init OK [640x480 RGB565, 600 KB/frame]`。
  - camera：`Streaming started (capture task priority=10 core=0 duty=1600ms)`。
  - C4 模式确认：`vision task disabled; camera stream only diagnostic mode`。
  - 前 5 个 capture：
    - `capture #1 receive=0ms max=0ms timeouts=0`
    - `capture #2 receive=38ms max=38ms timeouts=0`
    - `capture #3 receive=38ms max=38ms timeouts=0`
    - `capture #4 receive=38ms max=38ms timeouts=0`
    - `capture #5 receive=38ms max=38ms timeouts=0`
  - 因没有 vision task，日志没有周期性 `[PIPE_DIAG]` / `[VISION_DIAG]`。
  - capture #5 后，monitor 继续观察约 90 秒无新串口输出。
  - 本轮 C4 monitor 期间未记录到 BLUE_MARK。
  - 本轮 C4 monitor 期间未记录到 `CSI: csi recv API, transaction queue is full`。
  - 本轮 C4 monitor 期间未记录到 `camera frame timeout`。
  - 本轮 C4 monitor 期间未记录到实际 LCD `underrun happens`；只见诊断提示文本中的 `underrun happens`。
- 判定：
  - C4 在本轮观察窗口内未复现蓝屏。
  - C4 与 C3 的关键差异是没有 vision task，因此没有应用层 `cam_sensor_get_frame()` 循环，也没有 C3 的周期性 vision timeout。
  - C4 仍有 camera capture task 和 camera/CSI/ISP stream；但由于缺少周期性诊断日志，后续应补 heartbeat 或做 C5 调整 get_frame timeout 后再验证。

## 2026-06-28 C5 Vision Task Long Frame Timeout

- 目的：保留 vision task 和 `cam_sensor_get_frame()`/`cam_sensor_return_frame()` 应用层取帧循环，但把取帧 timeout 调到大于 C3 的 `1600ms` duty，去掉 C3 的周期性 `vision_app: camera frame timeout` 干扰。
- 代码/配置：
  - `components/vision_app/Kconfig` 新增 `CONFIG_VISION_APP_GET_FRAME_TIMEOUT_MS`，默认 1000，C5 配置为 2500。
  - `components/vision_app/vision_app.cpp` 将 `cam_sensor_get_frame(..., 1000)` 改为使用 `CONFIG_VISION_APP_GET_FRAME_TIMEOUT_MS`。
  - `sdkconfig` 关闭 `CONFIG_VISION_APP_CAMERA_ONLY_NO_TASK`，设置 `CONFIG_VISION_APP_GET_FRAME_TIMEOUT_MS=2500`。
  - 保持 `CONFIG_CAM_SENSOR_OUTPUT_RGB565=y`、`CONFIG_CAM_SENSOR_STREAM_DUTY_PERIOD_MS=1600`、preview/inference off。
- commit：`b2b1ca5 test c5 vision task long frame timeout`
- build：
  - `idf.py build` 通过。
  - 提交后再次 `idf.py build` 通过，App version 为 `b2b1ca5`，binary size `0x205c50`。
- flash/monitor：
  - `script -q -f ../docs/agent/run_logs/2026-06-28-C5-vision-task-timeout2500.log -c "idf.py -p /dev/ttyACM0 flash monitor"`
  - monitor 用 Ctrl-] 停止。
- 启动确认：
  - App version：`b2b1ca5`。
  - LCD：`1024x600 RGB16`，`active_read=55738KB/s`。
  - camera：`active_write=600KB/s frame=600KB fps=50 write_fps=1 period=1600ms duty=1600ms buffers=3`。
  - camera output：`cam_sensor_init OK [640x480 RGB565, 600 KB/frame]`。
  - vision task 已创建并启动：`vision task created priority=3 core=1 stack=32768B`，`vision task started core=1 priority=3`。
- BLUE_MARK #1：
  - `time_us=6530420 frame=1 total_frames=1 camera_timeouts=0 model_runs=0 preview_ok=0/0 yolo_ppa=0 preview_ppa=0`。
  - `PPA_TRACE recent count=0`。
  - cam_sensor：`count=2 timeouts=0 last_receive=38ms max_receive=38ms age=196ms`。
- BLUE_MARK #2：
  - `time_us=129435423 frame=78 total_frames=78 camera_timeouts=0 model_runs=0 preview_ok=0/0 yolo_ppa=0 preview_ppa=0`。
  - `PPA_TRACE recent count=0`。
  - cam_sensor：`count=78 timeouts=0 last_receive=38ms max_receive=58ms age=1497ms`。
- 停止前：
  - `uptime=136236ms frame=83 ... cam_timeout=0/0 ... blue=2`。
  - vision diag 稳定约 `0.62fps`，`cam_wait` 约 `1525-1527ms`。
  - 未记录到 `vision_app: camera frame timeout`。
  - 未记录到 `CSI: csi recv API, transaction queue is full`。
  - 未记录到实际 LCD `underrun happens`；只见诊断提示文本中的 `underrun happens`。
- 结论：
  - C5 去掉了 C3 的应用层 timeout 干扰，但仍复现蓝屏。
  - C4 无 vision task 未复现，C5 恢复 vision task/get_frame/return 后快速复现；下一步重点拆应用层取帧/还帧循环、pipeline 记录和 UI post。

## 2026-06-28 C4b Camera Only Heartbeat

- 目的：复测 C4，补上 1s camera heartbeat，确认没有 `cam_sensor_get_frame()` 时 capture count 是否持续增长。
- 基于：`b2b1ca5 test c5 vision task long frame timeout`。
- 代码/配置：
  - `components/vision_app/Kconfig` 新增 `CONFIG_VISION_APP_CAMERA_ONLY_HEARTBEAT_MS`。
  - `components/vision_app/vision_app.cpp` 在 `CONFIG_VISION_APP_CAMERA_ONLY_NO_TASK=y` 时启动 `camera_only_heartbeat_task`，每 1000ms 调用 `cam_sensor_log_diagnostics("cam_heartbeat")`。
  - 为 no-task 配置补充 `CONFIG_VISION_APP_GET_FRAME_TIMEOUT_MS` 默认宏，修复 no-task 配置下 Kconfig symbol 不生成导致的编译错误。
  - `sdkconfig` 设置：
    - `CONFIG_VISION_APP_CAMERA_ONLY_NO_TASK=y`
    - `CONFIG_VISION_APP_CAMERA_ONLY_HEARTBEAT_MS=1000`
    - `CONFIG_VISION_APP_ENABLE_CAMERA_STREAM=y`
    - `CONFIG_CAM_SENSOR_OUTPUT_RGB565=y`
    - `CONFIG_CAM_SENSOR_STREAM_DUTY_PERIOD_MS=1600`
    - preview/inference off
- commit：`8bcdd70 test c4b camera only heartbeat`
- build：
  - 第一次 build 失败：no-task 配置下 `CONFIG_VISION_APP_GET_FRAME_TIMEOUT_MS` 未定义。
  - 加默认宏后 `idf.py build` 通过。
  - 提交后再次 `idf.py build` 通过，App version 为 `8bcdd70`，binary size `0x203080`。
- flash/monitor：
  - log：`docs/agent/run_logs/2026-06-28-C4b-camera-only-heartbeat.log`
  - 命令：`script -q -f ../docs/agent/run_logs/2026-06-28-C4b-camera-only-heartbeat.log -c "idf.py -p /dev/ttyACM0 flash monitor"`
  - monitor 用 Ctrl-] 停止。
- 启动确认：
  - App version：`8bcdd70`。
  - LCD：`1024x600 RGB16 dpi=40MHz refresh=46.449Hz fbs=3 fb_size=1200KB dma2d=1`。
  - LCD active_read：`55738KB/s`。
  - PSRAM arbiter：`display_first=1 psram_read_first=0 camera_write_regulator=1`。
  - camera：`Format: MIPI_2lane_24Minput_RAW10_640x480_50fps 640x480 50fps`。
  - camera diag：`active_write=600KB/s frame=600KB fps=50 write_fps=1 period=1600ms duty=1600ms buffers=3`。
  - camera output：`cam_sensor_init OK [640x480 RGB565, 600 KB/frame]`。
  - C4b 模式确认：`vision task disabled; camera stream only diagnostic mode`。
  - heartbeat 确认：`camera-only heartbeat started interval=1000ms`。
- capture count：
  - 约 `7468ms`：`count=1`
  - 约 `30767ms`：`count=16`
  - 约 `67019ms`：`count=38`
  - 约 `120390ms`：`count=72`
  - 约 `130460ms`：`count=78`
  - 结论：无 vision task、无 `get_frame` 时，camera capture count 仍持续增长。
- BLUE_MARK：
  - #1：`time_us=6837656 tick=6647 core=1 psram_free=23863KB internal_free=94KB`。
  - BLUE_MARK 时 pipeline dump：`blue=1 frame=0 total_frames=0 camera_timeouts=0 roi_ok=0 roi_skip=0 model_runs=0 preview_ok=0/0 yolo_ppa=0 preview_ppa=0`。
  - `PPA_TRACE recent count=0`。
  - BLUE_MARK 时 camera：`streaming=1 frame=640x480 fps=50 active_write=600KB/s count=2 timeouts=0 last_receive=38ms max_receive=38ms age=492ms latest=0 app=-1 psram_free=23863KB`。
  - dashboard unavailable 是预期现象：无 vision result。
- 负面信号：
  - 未见 `CSI: csi recv API, transaction queue is full`。
  - 未见 `camera frame timeout`。
  - 未见实际 LCD `underrun happens`；只见 `[LCD_DIAG] if blue flash appears...` 诊断提示文本。
- 判定：
  - C4 的“无蓝”不能解释为 camera stream/capture 停止；C4b 已确认无 `get_frame` 时 capture count 持续增长。
  - C4b 在无 vision task、无 `get_frame`、无 preview/inference/PPA 时仍出现 BLUE_MARK，因此 camera/CSI/ISP stream/write 本身仍是强相关触发方向。
  - C6a/C6b 仍继续执行，但目标变为比较 vision task 和 get/return 对 BLUE_MARK 频率/形态的影响，而不是证明它们是否必要。

## 2026-06-28 C6a Vision Task No Get Frame

- 目的：保留 camera stream 和 vision task，但 vision task 不调用 `cam_sensor_get_frame()`，排除 vision task/heartbeat/周期日志本身。
- 代码/配置：
  - `components/vision_app/Kconfig` 新增 `CONFIG_VISION_APP_TASK_NO_GET_FRAME` 和 `CONFIG_VISION_APP_TASK_NO_GET_FRAME_HEARTBEAT_MS`。
  - `components/vision_app/vision_app.cpp` 在 `vision_task` 启动后增加 no-get-frame 诊断分支；每 1000ms 打 `BSP_PipelineDiag_LogPeriodic()`、`[VISION_DIAG] no_get_frame=1`、`cam_sensor_log_diagnostics("vision_no_get_frame")`。
  - 该分支不调用 `cam_sensor_get_frame()`，不读 frame 内容，不 post UI，不做 per-frame pipeline record，不做 PPA/YOLO/preview。
  - `sdkconfig` 设置：
    - `# CONFIG_VISION_APP_CAMERA_ONLY_NO_TASK is not set`
    - `CONFIG_VISION_APP_TASK_NO_GET_FRAME=y`
    - `CONFIG_VISION_APP_TASK_NO_GET_FRAME_HEARTBEAT_MS=1000`
    - `CONFIG_VISION_APP_GET_FRAME_TIMEOUT_MS=2500`
    - `CONFIG_CAM_SENSOR_OUTPUT_RGB565=y`
    - `CONFIG_CAM_SENSOR_STREAM_DUTY_PERIOD_MS=1600`
    - preview/inference off
- commit：`88cebbc test c6a vision task no get frame`
- build：
  - `idf.py build` 通过，binary size `0x203770`。
  - 提交后再次 `idf.py build` 通过，App version 为 `88cebbc`。
- flash/monitor：
  - log：`docs/agent/run_logs/2026-06-28-C6a-vision-task-no-get-frame.log`
  - 命令：`script -q -f ../docs/agent/run_logs/2026-06-28-C6a-vision-task-no-get-frame.log -c "idf.py -p /dev/ttyACM0 flash monitor"`
  - monitor 用 Ctrl-] 停止。
- 启动确认：
  - App version：`88cebbc`。
  - LCD：`1024x600 RGB16 dpi=40MHz refresh=46.449Hz fbs=3 fb_size=1200KB dma2d=1`。
  - LCD active_read：`55738KB/s`。
  - PSRAM arbiter：`display_first=1 psram_read_first=0 camera_write_regulator=1`。
  - camera diag：`active_write=600KB/s frame=600KB fps=50 write_fps=1 period=1600ms duty=1600ms buffers=3`。
  - vision task 已创建：`vision task created priority=3 core=1 stack=32768B`。
  - C6a 模式确认：`vision task no-get-frame diagnostic mode interval=1000ms`。
- no-get-frame 确认：
  - 周期 `[VISION_DIAG] no_get_frame=1 core=1 prio=3 ...`。
  - 周期 `[PIPE_DIAG] frame=0 fps=0 ... cam_timeout=0/0 ... model_run=0 yolo_ppa=0 preview=0/0 preview_ppa=0`。
  - camera count 仍持续增长：约 `7521ms count=1`，约 `30750ms count=16`，约 `120519ms count=72`，停止前约 `144981ms count=87`。
- BLUE_MARK：
  - #1：`time_us=6369661`，blue_dump `frame=0 total_frames=0 camera_timeouts=0 roi_ok=0 roi_skip=0 model_runs=0 preview_ok=0/0 yolo_ppa=0 preview_ppa=0`。
  - #1 camera：`count=2 timeouts=0 last_receive=38ms max_receive=38ms age=35ms`。
  - #2：`time_us=129422655`，blue_dump `frame=0 total_frames=0 camera_timeouts=0 roi_ok=0 roi_skip=0 model_runs=0 preview_ok=0/0 yolo_ppa=0 preview_ppa=0`。
  - #2 camera：`count=78 timeouts=0 last_receive=38ms max_receive=58ms age=1484ms`。
  - 两次 BLUE_MARK 均 `PPA_TRACE recent count=0`。
- 负面信号：
  - 未见 `CSI: csi recv API, transaction queue is full`。
  - 未见 `camera frame timeout`。
  - 未见实际 LCD `underrun happens`；只见 `[LCD_DIAG] if blue flash appears...` 诊断提示文本。
- 判定：
  - C6a 在 vision task 存在、但完全不 `get_frame` 时仍复现 BLUE_MARK。
  - `cam_sensor_get_frame()` / `cam_sensor_return_frame()` 不是蓝屏必要条件。
  - C6b 仍继续做，用于比较 get/return 是否提高 BLUE_MARK 频率或改变形态。

## 2026-06-28 C6b Get Return Only

- 目的：vision task 调用 `cam_sensor_get_frame()` 后立即 `cam_sensor_return_frame()`，不读内容、不 post UI、不做 per-frame pipeline record、不做 PPA/YOLO/preview，用于观察 get/return 本身的影响。
- 代码/配置：
  - `components/vision_app/Kconfig` 新增 `CONFIG_VISION_APP_GET_RETURN_ONLY`。
  - `components/vision_app/vision_app.cpp` 在成功 `cam_sensor_get_frame()` 后增加 get-return-only 分支：
    - 立即 `cam_sensor_return_frame(frame)`。
    - 不调用 `BSP_PipelineDiag_RecordCameraFrame()`。
    - 不读 frame 内容。
    - 不 post UI。
    - 不做 PPA/YOLO/preview。
    - 约每 1s 打 `[VISION_DIAG] get_return_only=1 ... returned_size=... returned_wh=...` 和 `cam_sensor_log_diagnostics("vision_get_return")`。
  - `sdkconfig` 设置：
    - `# CONFIG_VISION_APP_CAMERA_ONLY_NO_TASK is not set`
    - `# CONFIG_VISION_APP_TASK_NO_GET_FRAME is not set`
    - `CONFIG_VISION_APP_GET_RETURN_ONLY=y`
    - `CONFIG_VISION_APP_GET_FRAME_TIMEOUT_MS=2500`
    - `CONFIG_CAM_SENSOR_OUTPUT_RGB565=y`
    - `CONFIG_CAM_SENSOR_STREAM_DUTY_PERIOD_MS=1600`
    - preview/inference off
- build：
  - 第一次 build 失败：`now` 变量在 get-return-only 分支中与后续 pipeline 分支重复定义。
  - 修复：将 get-return-only 分支变量改名为 `get_return_now`。
  - `idf.py build` 通过。
- commit：`7b8fc2f test c6b get return only`
- 提交后验证：
  - `idf.py build` 通过，App version 为 `7b8fc2f`。
- flash/monitor：
  - log：`docs/agent/run_logs/2026-06-28-C6b-get-return-only.log`
  - 命令：`script -q -f ../docs/agent/run_logs/2026-06-28-C6b-get-return-only.log -c "idf.py -p /dev/ttyACM0 flash monitor"`
  - monitor 用 Ctrl-] 停止。
- 启动确认：
  - App version：`7b8fc2f`。
  - LCD：`1024x600 RGB16 dpi=40MHz refresh=46.449Hz fbs=3 fb_size=1200KB dma2d=1`。
  - LCD active_read：`55738KB/s`。
  - PSRAM arbiter：`display_first=1 psram_read_first=0 camera_write_regulator=1`。
  - camera output：`cam_sensor_init OK [640x480 RGB565, 600 KB/frame]`。
  - camera diag：`active_write=600KB/s frame=600KB fps=50 write_fps=1 period=1600ms duty=1600ms buffers=3`。
  - C6b 模式确认：
    - `get_return_only=1 frames=2 fps=1.23 cam_wait=1609ms returned_size=614400 returned_wh=640x480`
    - `get_return_only=1 frames=1 fps=0.63 cam_wait=1513ms returned_size=614400 returned_wh=640x480`
- BLUE_MARK：
  - #1：`time_us=2410660`，发生在 camera 初始化前；`cam_sensor: [CAM_DIAG] source=blue_mark unavailable: ESP_ERR_INVALID_STATE`。该标记不能计入 camera/get-return 结果。
  - #2：`time_us=62951656`，发生在 CSI queue full / camera timeout 已持续约 51 秒之后；blue_dump `blue=2 frame=0 total_frames=0 camera_timeouts=21 roi_ok=0 roi_skip=0 model_runs=0 preview_ok=0/0 yolo_ppa=0 preview_ppa=0`。
  - #2 camera：`count=3 timeouts=265 last_receive=23ms max_receive=38ms age=55030ms`。
  - 后续还有 #3/#4 按键标记，均发生在持续 CSI queue full / camera timeout 状态下。
- camera/CSI：
  - camera 启动后只成功 capture/get-return 3 帧。
  - 约 `11452ms` 开始 `CSI: csi recv API, transaction queue is full`。
  - 随后持续 `vision_app: camera frame timeout`。
  - 停止前 `capture timeout count` 约 690。
- 负面信号：
  - 未见实际 LCD `underrun happens`；只见 `[LCD_DIAG] if blue flash appears...` 诊断提示文本。
  - `PPA_TRACE recent count=0`，无 PPA/preview/YOLO 活动。
- 判定：
  - C6b 不是干净的“get/return 触发蓝屏”实验，因为可计入的 BLUE_MARK 发生在 camera/CSI 已失稳之后。
  - C6b 是明确的“get-return-only 在 640x480 RGB565 下会快速触发 CSI queue full / camera timeout”结果。
  - 继续按用户计划进入 S1/S2/S3 小帧测试；先在 C6b get-return-only 基线上降低 frame size，判断是否能避免 CSI queue full，并在 camera/CSI 稳定前提下观察 BLUE_MARK。
