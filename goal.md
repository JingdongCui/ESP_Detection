# Goal：恢复 ESP32-P4 推理延时到约 70 ms

## 目标

在保留队友新增的相机 ISP/UI 功能、以太网通信和真实分拣 IO 的前提下，定位并消除推理调度回退。

核心验收指标：

- 单个模型阶段的 `infer_diag` 延时恢复到约 70 ms。
- 建议硬门槛：`wb <= 100 ms`、`logo <= 100 ms`，连续至少 30 次无持续回退。
- 级联总延时按实际命中路径另行统计；不要把 `wb + crop + logo` 的级联总时间误当成单模型 70 ms 指标。
- Ethernet 必须能启动、Link Up、取得静态地址 `192.168.10.2`，并能连接队友上位机。
- 不允许以崩溃、看门狗、丢失真实传感器事件或明显卡 UI 为代价换取延时。

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

## 操作约束

- 保留用户自己的根目录 `AGENTS.md`、`.agents`、`.codex` 和归档中的 Agent/Skill 内容；只清理队友压缩包自带的相关文件。
- 不要破坏标签 `backup/before-teammate-merge-20260716`。
- 不要覆盖或丢弃当前未提交实验；先逐项辨认、记录，再选择保留或撤销。
- 上位机继续直接使用队友工程 `esp32_host_no_inference`，本 goal 主要修改板端。
