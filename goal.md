# Goal：恢复 70 ms 推理并完成上位机设备控制闭环

## 目标

在保留队友新增的相机 ISP/UI 功能、以太网通信和真实分拣 IO 的前提下，先定位并消除推理调度回退，再完成上位机第三页到板端的设备控制链路。

核心验收指标：

- 单个模型阶段的 `infer_diag` 延时恢复到约 70 ms。
- 建议硬门槛：`wb <= 100 ms`、`logo <= 100 ms`，连续至少 30 次无持续回退。
- 级联总延时按实际命中路径另行统计；不要把 `wb + crop + logo` 的级联总时间误当成单模型 70 ms 指标。
- Ethernet 必须能启动、Link Up、取得静态地址 `192.168.10.2`，并能连接队友上位机。
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

## 验证与完成标准

每次正式代码修改后执行：

```bash
cd /home/kazeform/2026esp/ESP32P4_Detection
source /home/kazeform/ProgramFiles/esp_idf_5.5.4/esp-idf-v5.5.4/export.sh
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor
```

完成必须同时满足：

- 连续 30 次单阶段模型延时接近 70 ms，且硬门槛不持续超过 100 ms。
- Ethernet 和队友上位机控制/图像链路正常。
- 真实分拣 IO 不丢事件，电机和传感器响应满足业务要求。
- UI 无不可接受的长时间卡顿。
- 至少运行 30 分钟无 panic、watchdog、POWERON 异常重启或双核死锁。
- 更新 `docs/agent/PROJECT.md`、`CURRENT.md`、`HISTORY.md`，并记录所有 A/B 数据与最终取舍。

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
- 只有连续 30 次延时、以太网、真实 IO 和稳定性都通过后，才能给阶段打 stable 标签。

## 操作约束

- 保留用户自己的根目录 `AGENTS.md`、`.agents`、`.codex` 和归档中的 Agent/Skill 内容；只清理队友压缩包自带的相关文件。
- 不要破坏标签 `backup/before-teammate-merge-20260716`。
- 不要覆盖或丢弃当前未提交实验；先逐项辨认、记录，再选择保留或撤销。
- 上位机继续直接使用 `esp32_host_no_inference` 的 `6bcee3b` 功能基线；本 goal 主要修改板端，只有联调发现明确主机端缺陷时才改上位机。
- 未完成推理恢复前，不开始板端控制协议代码；未完成控制联调前，不开始 UVC 内存修复。
- 不允许为了追求 70 ms 永久关闭 Ethernet、真实分拣 IO、检测功能或板端 UI；临时跳过只能作为有独立提交的 A/B 实验。
