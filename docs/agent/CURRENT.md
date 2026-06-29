# Current Task

## Goal

继续执行 `plan.md` 中的蓝屏定位计划，直接烧录测试；用户看到蓝屏会按板上按钮。每轮测试必须记录 commit、配置、build/flash/monitor、串口日志、BLUE_MARK 和肉眼现象。

## Current State

- 工程：`bug_project`
- 分支：`test-blue-mark-arbiter-matrix`
- 当前板上固件：`7b8fc2f test c6b get return only`
- 当前日志：
  - B0 natural：`docs/agent/run_logs/2026-06-28-B0-pipeline-full-baseline.log`
  - B0 force：`docs/agent/run_logs/2026-06-28-B0-force-inference.log`
  - B1 RGB565 force：`docs/agent/run_logs/2026-06-28-B1-rgb565-force-inference.log`
  - P0 RGB565 force no-preview：`docs/agent/run_logs/2026-06-28-P0-rgb565-force-no-preview.log`
  - Y0 RGB565 preview no-inference：`docs/agent/run_logs/2026-06-28-Y0-rgb565-preview-no-inference.log`
  - C0 RGB565 camera off：`docs/agent/run_logs/2026-06-28-C0-rgb565-camera-off.log`
  - C1 RGB565 camera half duty：`docs/agent/run_logs/2026-06-28-C1-rgb565-camera-half-duty.log`
  - C1b RGB565 camera duty 800：`docs/agent/run_logs/2026-06-28-C1b-rgb565-camera-duty800.log`
  - C2 RGB565 camera output：`docs/agent/run_logs/2026-06-28-C2-rgb565-camera-output.log`
  - C3 RGB565 camera duty 1600：`docs/agent/run_logs/2026-06-28-C3-rgb565-camera-duty1600.log`
  - C4 camera only no vision task：`docs/agent/run_logs/2026-06-28-C4-camera-only-no-vision-task.log`
  - C5 vision task timeout 2500：`docs/agent/run_logs/2026-06-28-C5-vision-task-timeout2500.log`
  - C4b camera only heartbeat：`docs/agent/run_logs/2026-06-28-C4b-camera-only-heartbeat.log`
  - C6a vision task no get frame：`docs/agent/run_logs/2026-06-28-C6a-vision-task-no-get-frame.log`
  - C6b get return only：`docs/agent/run_logs/2026-06-28-C6b-get-return-only.log`
- 当前公共配置：
  - `DISPLAY_FIRST=y`
  - `PSRAM_READ_FIRST=n`
  - `CAMERA_WRITE_REGULATOR=y`
  - current C6b camera stream enabled, camera output RGB565, `CONFIG_CAM_SENSOR_STREAM_DUTY_PERIOD_MS=1600`
  - current C6b vision task exists, `CONFIG_VISION_APP_GET_RETURN_ONLY=y`
  - current C6b preview image update disabled
  - current C6b inference disabled
  - Ethernet raw image packet disabled
  - DMA2D enabled
  - LCD RGB565

## Preview Chain Clarification

用户现场观察：最近 LCD preview 区域没有稳定显示真实摄像头图像，有时黑、有时白、有时黑白参半。

对照当前代码和日志后，必须把 preview 分成三层记录：

1. preview 控件初始化：
   - `System_Init()` 总是调用 `vision_preview_init(scr_dashboard_cont_live_vedio, 0, 0, 640, 375)`。
   - `vision_preview_init()` 会分配两个 RGB565 PSRAM buffer，并先 `memset(..., 0)`。
   - 所以 `preview initialized: 640x375` 只证明控件和黑色初始 buffer 创建成功，不证明 live camera preview 在更新。

2. preview 性能链路执行：
   - 只有 `CONFIG_VISION_APP_ENABLE_PREVIEW_IMAGE=y` 且达到 `CONFIG_VISION_PREVIEW_IMAGE_EVERY_N_FRAMES` 时，`vision_app.cpp` 才调用 `vision_preview_update()`。
   - B0 natural 和 B1 日志里有 `preview_ok>0`、`preview_ppa>0`、`PPA_TRACE kind=preview src=640x480 dst=640x375 ret=ESP_OK`，说明这些版本的 preview 更新函数确实执行，并产生了 PPA/PSRAM/LVGL source 切换负载。
   - 这只能证明性能链路在跑，不能证明屏幕上显示的是正确摄像头画面。

3. preview 视觉有效性：
   - 当前没有日志证明 LCD 上的 preview 区域真实显示摄像头图像。
   - 用户观察到黑、白、黑白参半，这不是正常产品 preview。
   - 因此 B0/B1 只能记录为“preview update/load active, visual preview not validated”，不能记录为“功能完整 preview 正常”。

当前 P0：

- `sdkconfig` 中 `# CONFIG_VISION_APP_ENABLE_PREVIEW_IMAGE is not set`。
- P0 日志全程 `preview=0/0 preview_ppa=0 images=0`。
- P0 必须记录为 preview update 没开；它只证明无 preview update 负载时仍可复现蓝屏。

用户后续修正测试标准：

- 先不以 LCD 上 preview 看起来是否正常作为阻塞。
- 只要 `preview_ok>0`、`preview_ppa>0`、`PPA_TRACE kind=preview ret=ESP_OK`，就认为 preview 性能链路对 PSRAM/PPA/LVGL 产生了真实压力，实验有效。
- 但记录时仍必须区分 `preview load active` 和 `preview visual validated`。

## Completed In This Phase

- Phase 1 diagnostics：`8ed68c2`，新增 `[PIPE_DIAG]`、`[PPA_TRACE]`、BLUE_MARK pipeline dump、frame id、Ethernet raw image packet Kconfig。
- B0 natural：`8ed68c2`，RGB24 + preview load active，ROI 一直 skip，`model_runs=0`。
- B0 force：`a28948c`，RGB24 + preview + forced inference，camera/CSI timeout=0，仍有 BLUE_MARK。
- B1 RGB565 force：`6422f39`，LCD `active_read=55738KB/s`，camera/CSI timeout=0，约 170 秒内 3 次 BLUE_MARK。
- P0 RGB565 force no-preview：`ea24f0f`，`preview_ppa=0` 仍 35 秒内 3 次 BLUE_MARK；证明 preview update/PPA 不是必要触发因素。
- C0 RGB565 camera off：`f1f95e4`，camera stream disabled、preview/inference off，约 90 秒 monitor 无 BLUE_MARK、无 CSI timeout、无实际 underrun 日志。
- C1 RGB565 camera half duty：`9ba705c`，camera stream enabled、`active_write=1800KB/s`、preview/inference off，约 35 秒 BLUE_MARK #1，且当时 `camera_timeouts=0 model_runs=0 preview_ppa=0`；约 82 秒后开始 CSI queue full / camera timeout。
- C1b RGB565 camera duty 800：`c990bdf`，camera stream enabled、`active_write=900KB/s`、preview/inference off，约 168 秒内 4 次 BLUE_MARK，所有 BLUE_MARK 均 `camera_timeouts=0 model_runs=0 preview_ppa=0`；停止前 frame=205，仍 `cam_timeout=0/0`，无 CSI queue full、无实际 underrun 日志。
- C2 RGB565 camera output duty 800：`f2ac998`，camera stream enabled、camera output RGB565、`active_write=600KB/s`、preview/inference off，约 32.2 秒 BLUE_MARK #1，且当时 `camera_timeouts=0 model_runs=0 preview_ppa=0`；约 225.8 秒前仍 `cam_timeout=0/0`，约 229.1 秒后开始 CSI queue full / camera timeout。
- C3 RGB565 camera output duty 1600：`7e03fda`，camera stream enabled、camera output RGB565、`period=1600ms duty=1600ms`、preview/inference off，约 128 秒 monitor 内 3 次 BLUE_MARK；所有 BLUE_MARK 均 `model_runs=0 preview_ppa=0 PPA_TRACE recent count=0`，cam_sensor 层 `timeouts=0`，未见 CSI queue full / 实际 underrun。
- C4 camera only no vision task：`037f341`，camera stream enabled、camera output RGB565、`period=1600ms duty=1600ms`、preview/inference off、vision task 不创建；串口确认 `vision task disabled; camera stream only diagnostic mode`，前 5 个 capture `timeouts=0`，之后约 90 秒无新日志、无 BLUE_MARK、无 CSI queue full、无实际 underrun。
- C5 vision task timeout 2500：`b2b1ca5`，恢复 vision task/get_frame，`CONFIG_VISION_APP_GET_FRAME_TIMEOUT_MS=2500` 大于 1600ms duty，camera output RGB565、preview/inference off；约 137 秒 monitor 内 2 次 BLUE_MARK，所有 BLUE_MARK 均 `camera_timeouts=0 model_runs=0 preview_ppa=0 PPA_TRACE recent count=0`，cam_sensor 层 `timeouts=0`，无 CSI queue full、无实际 underrun。
- C4b camera only heartbeat：`8bcdd70`，无 vision task、无 `get_frame`，增加 1s camera heartbeat；约 130 秒 monitor 中 capture count 从 1 持续到 78，说明 C4 没有 `get_frame` 时 camera stream/capture 仍持续；约 6.8 秒出现 BLUE_MARK #1，cam_sensor `count=2 timeouts=0 last_receive=38ms max_receive=38ms`，无 CSI queue full、无实际 underrun。
- C6a vision task no get_frame：`88cebbc`，vision task 存在并每 1s 打 `no_get_frame` heartbeat，但不调用 `cam_sensor_get_frame()`；约 143 秒 monitor 内 2 次 BLUE_MARK，frame 始终为 0，所有 BLUE_MARK 均 `camera_timeouts=0 model_runs=0 preview_ppa=0 PPA_TRACE recent count=0`，cam_sensor 层 `timeouts=0`，无 CSI queue full、无实际 underrun。
- C6b get return only：`7b8fc2f`，vision task 调用 `cam_sensor_get_frame()` 后立即 `cam_sensor_return_frame()`，不读内容、不 post UI、不做 per-frame pipeline record、不做 PPA/YOLO/preview；camera 启动后只成功返回 3 帧，随后约 `11.45s` 开始持续 CSI queue full / capture timeout。BLUE_MARK #1 发生在 camera 初始化前，不计入 camera/get-return 结果；BLUE_MARK #2 发生在 CSI queue full 已持续约 51 秒后，属于 camera/CSI 已失稳后的结果。未见实际 underrun。

### Y0 RGB565 Preview-Load No-Inference Test

- commit：`3e87962 test y0 preview load no inference`
- 日志：`docs/agent/run_logs/2026-06-28-Y0-rgb565-preview-no-inference.log`
- 配置差异：
  - 基于 P0。
  - 开启 `CONFIG_VISION_APP_ENABLE_PREVIEW_IMAGE=y`。
  - 关闭 `CONFIG_VISION_APP_ENABLE_INFERENCE`。
  - 保留 camera stream、LCD RGB565、Ethernet raw image disabled。
- 验证：
  - `idf.py build` 通过。
  - `idf.py -p /dev/ttyACM0 flash monitor` 通过，monitor 已停止。
- 结果：
  - LCD：`1024x600 RGB16 dpi=40MHz`，`fb_size=1200KB`，`active_read=55738KB/s`。
  - inference 确认关闭：启动日志 `YOLO inference disabled for bandwidth A/B test`，所有 BLUE_MARK dump 均 `model_runs=0 yolo_ppa=0`。
  - preview 性能链路确认开启：常规 `[PIPE_DIAG] preview=2/2 preview_ppa=2`，BLUE_MARK dump 中 `preview_ok>0 preview_ppa>0`，PPA trace 全为 `kind=preview ... ret=ESP_OK`。
  - BLUE_MARK #1：`time_us=6698435 frame=9 camera_timeouts=0 model_runs=0 preview_ok=3/3 yolo_ppa=0 preview_ppa=3`。
  - BLUE_MARK #2：`time_us=71903443 frame=334 camera_timeouts=0 model_runs=0 preview_ok=111/111 yolo_ppa=0 preview_ppa=111`。
  - BLUE_MARK #3：`time_us=91193433 frame=428 camera_timeouts=0 model_runs=0 preview_ok=142/142 yolo_ppa=0 preview_ppa=142`。
  - 约 `93.107s` 后开始出现 `CSI: csi recv API, transaction queue is full` 和 capture timeout。
  - BLUE_MARK #4：`time_us=111214420 frame=429 camera_timeouts=19 model_runs=0 preview_ok=143/143 yolo_ppa=0 preview_ppa=143`。
  - 停止前 capture timeout count 增长到 180，持续 `camera frame timeout` 和 CSI queue full。
  - 未见实际 LCD `underrun happens` 日志；只见诊断提示文本中的 `underrun happens`。
- 判定：
  - Y0 是 preview load active、visual preview not validated 测试。
  - 无 YOLO/model、无 YOLO PPA 时仍出现多次 BLUE_MARK，因此 YOLO/model 不是蓝屏必要触发因素。
  - Y0 后段出现 camera/CSI 失稳，和 B1/P0 的 `timeouts=0` 不同；后续测试要区分“前 91 秒内蓝屏但 camera timeout=0”和“后段 camera queue full 后的蓝屏”。

## Current Conclusions

- 不能再把“camera timeout=0”称为“稳定”；只能称为 camera/CSI 稳定。
- 当前 B0/B1/P0 测试里，camera/CSI 都稳定，且串口没有记录到 LCD underrun；但用户仍能看到蓝屏。
- Y0 中 #1/#2/#3 蓝屏发生时 `camera_timeouts=0`，但约 93 秒后 camera/CSI 开始 queue full/timeout，#4 已经属于 camera 失稳后的蓝屏。
- `PSRAM_READ_FIRST` 和 CSI priority 0 已有明确反证，不继续做。
- RGB565 可降低 LCD read bandwidth，但单独不足以消除蓝屏。
- B0/B1 的 preview 性能链路确实有负载；但 preview 视觉正确性没有证明，不能称为功能完整 preview 正常。
- P0 证明 preview update/PPA 不是必要触发因素；Y0 证明 YOLO/model 不是必要触发因素。
- C1/C1b/C2/C3 证明把 camera write 从 `1800KB/s` 降到 `900KB/s`，再把单帧输出从 RGB888 `900KB/frame` 降到 RGB565 `600KB/frame`，并把 duty 拉到 `1600ms` 后，仍不能消除蓝屏。
- C3 有一个副作用：vision_app 取帧等待时间短于 1600ms duty，导致周期性 `vision_app: camera frame timeout`；但 cam_sensor 层 `timeouts=0`，不是 CSI queue full。
- C4 暂时提示：去掉应用层 vision get_frame 循环后，在本轮观察窗口内没有复现蓝屏；但 C4 缺少周期性 pipe/vision 日志，只能按 monitor 观察窗口记录。
- C5 说明：把 vision_app 取帧 timeout 调到 2500ms 后，C3 的周期性 `camera frame timeout` 消失；但恢复正常 `cam_sensor_get_frame()` / `cam_sensor_return_frame()` 循环后仍快速复现蓝屏。C4/C5 对照把重点缩小到应用层取帧/还帧循环、相关调度或 pipeline 记录，而不是 preview/YOLO/PPA。
- C4b 修正 C4 解释：C4 的无蓝不能解释为 camera 停止，capture count 已确认持续增长；但 C4b 在无 vision task、无 `get_frame` 时仍出现 1 次 BLUE_MARK，因此 camera/CSI/ISP stream/write 本身仍是强相关触发方向。
- C6a 说明：vision task 存在但完全不 `get_frame` 时仍出现 BLUE_MARK，因此 `cam_sensor_get_frame()` / `cam_sensor_return_frame()` 不是蓝屏必要条件。
- C6b 说明：`get_frame` 后立即 return 的 640x480 RGB565 路径会快速触发 CSI queue full / camera timeout；但 C6b 的可计入 BLUE_MARK 发生在 camera/CSI 失稳后，不能作为 get/return 本身足以导致蓝屏的干净证据。
- 当前继续烧录测试，但 preview 判据改为性能链路：
  - `preview_ok>0` / `preview_ppa>0` 即视为 preview PSRAM/PPA 压力存在。
  - 不要求先确认外部看起来的 preview 视觉正确性。
- 下一轮应尽量只改一个变量，优先转向 camera stream/write 降载。

## Immediate Next Step

不继续纠结 preview 外观看起来是否正常；后续只按 performance load 记录 preview。

下一阶段继续 Phase C，转入小帧测试：

- 下一步 S1：在 C6b get-return-only 基线上尝试 512x300 RGB565，先核对启动日志中实际 `cam_sensor_init OK [512x300 RGB565, ...]` 是否成立；若仍是 640x480，则记录小帧配置未生效。
- S1 主要观察 CSI queue full / camera timeout 是否消失；若 camera/CSI 稳定，再看 BLUE_MARK 频率。
- S1 后按同样规则做 S2 320x240 RGB565、S3 224x224 RGB565。

## Verification Standard

- `idf.py build` 必须通过。
- `idf.py -p /dev/ttyACM0 flash monitor` 或等价 flash + monitor 必须执行。
- 串口日志保存到 `docs/agent/run_logs/`。
- 记录 BLUE_MARK 次数、frame、camera timeout、model_runs、preview_ok、yolo_ppa、preview_ppa。
- 记录用户肉眼蓝屏现象优先级高于日志中是否有 underrun。
