# MIPI 屏幕刷新与 PSRAM 读写优先级方案

日期：2026-07-03

## 目标

在 ESP32-P4 上稳定驱动 1024×600 RGB888 MIPI DSI 屏幕，优先保证屏幕持续刷新，降低 CSI 摄像头、PPA、LVGL、esp-dl 推理、CPU/XIP 访问对 PSRAM 带宽的竞争影响。

核心原则：

1. **DSI framebuffer 读优先级最高**：MIPI DSI DPI 屏需要主控持续从 framebuffer 输出像素流，读不到数据会直接表现为闪屏、蓝闪或撕裂。
2. **显示优先于推理**：推理可以降帧、跳帧或延迟，屏幕刷新不能被推理抢占。
3. **限制显示链路带宽，而不是盲目提高任务优先级**：瓶颈主要在 PSRAM 总线，不是单纯 CPU 调度。
4. **先稳定，再优化带宽**：第一阶段保留双 framebuffer 写入，待屏幕稳定后再考虑只写 inactive framebuffer 等复杂优化。

## 官方资料依据

乐鑫 ESP-IDF MIPI DSI LCD 文档说明：

- MIPI LCD 通常没有 GRAM，高分辨率屏需要主控维护 LCD framebuffer，并通过 MIPI DSI DPI 接口持续刷新到屏幕。
- `esp_lcd_dpi_panel_config_t::dpi_clock_freq_mhz` 决定 DPI 像素时钟；更高像素时钟会提高刷新率，但如果 DMA 带宽不足，可能导致闪烁。
- DPI panel framebuffer 可通过 `esp_lcd_dpi_panel_get_frame_buffer()` 获取。
- `esp_lcd_dpi_panel_register_event_callbacks()` 可注册刷新完成回调，后续可用于更精确的 framebuffer 同步。

相关官方文档：

- ESP-IDF MIPI DSI LCD：`https://docs.espressif.com/projects/esp-idf/en/stable/esp32p4/api-reference/peripherals/lcd/dsi_lcd.html`
- ESP-IDF LCD 总览：`https://docs.espressif.com/projects/esp-idf/en/stable/esp32p4/api-reference/peripherals/lcd/index.html`
- ESP-IDF External RAM：`https://docs.espressif.com/projects/esp-idf/en/stable/esp32p4/api-guides/external-ram.html`
- ESP-IDF FreeRTOS SMP：`https://docs.espressif.com/projects/esp-idf/en/stable/esp32p4/api-reference/system/freertos_idf.html`

## 现有代码链路

### LCD / MIPI DSI 初始化

文件：`components/bsp/bsp_lcd.c`

当前 LCD 初始化流程：

1. 初始化背光 PWM。
2. 打开 MIPI DSI PHY 2.5V LDO。
3. 创建 DSI bus。
4. 创建 DBI IO，用于面板寄存器控制。
5. 创建 EK79007 DPI panel。
6. 获取 DOUBLE_DIRECT 模式需要的 framebuffer 数量。
7. 设置 RGB888 framebuffer。
8. 将 DPI clock 从 52MHz 降到 26MHz。
9. 启用 `use_dma2d`。
10. 初始化面板。
11. 调用 `bsp_psram_qos_tune()` 调整 AXI ICM QoS。

关键代码：

- `components/bsp/bsp_lcd.c:42`：`bsp_psram_qos_tune()`
- `components/bsp/bsp_lcd.c:108`：`EK79007_1024_600_PANEL_60HZ_CONFIG(LCD_COLOR_PIXEL_FORMAT_RGB888)`
- `components/bsp/bsp_lcd.c:113`：`dpi_cfg.dpi_clock_freq_mhz = 26`
- `components/bsp/bsp_lcd.c:114`：`dpi_cfg.num_fbs = num_fbs`
- `components/bsp/bsp_lcd.c:115`：`dpi_cfg.flags.use_dma2d = 1`
- `components/bsp/bsp_lcd.c:169`：`BSP_LCD_GetFrameBuffers()`

当前 26MHz 配置对应约 30Hz：

```text
refresh = 26e6 / 1354 / 636 ≈ 30.2Hz
```

这会把 DSI 持续读 framebuffer 的带宽从约 110 MB/s 降到约 55 MB/s，是正确的第一层降压措施。

### LVGL 适配器

文件：`components/bsp/bsp_lvgl_adapter_init.c`

关键配置：

- `components/bsp/bsp_lvgl_adapter_init.c:21`：LVGL worker 栈 16KB。
- `components/bsp/bsp_lvgl_adapter_init.c:22`：LVGL worker 优先级 5。
- `components/bsp/bsp_lvgl_adapter_init.c:23`：LVGL worker 固定到 core1。
- `components/bsp/bsp_lvgl_adapter_init.c:38`：LVGL draw buffer 使用 PSRAM。
- `components/bsp/bsp_lvgl_adapter_init.c:43`：使用 `ESP_LV_ADAPTER_TEAR_AVOID_MODE_DOUBLE_DIRECT`。

当前设计含义：

- LVGL worker 负责 UI 刷新，core1，优先级 5。
- LVGL draw buffer 在 PSRAM，节省内部 SRAM。
- DOUBLE_DIRECT 有两块物理 framebuffer，避免 tearing，但会增加 framebuffer 一致性维护成本。

### 视觉显示任务

文件：`components/vision/framework/vision_app.c`

任务结构：

- `vision_fetch_task`：core0，生产者，取 V4L2 frame。
- `vision_display_task`：core0，显示任务，PPA 缩放 + 画框 + blit 到 framebuffer。
- `vision_detect_task`：core1，推理任务，读取同一最新帧。

当前优先级：

- `components/vision/framework/vision_app.c:43`：`VISION_FETCH_PRIORITY = 5`
- `components/vision/framework/vision_app.c:47`：`VISION_DISP_PRIORITY = 4`
- `components/vision/framework/vision_app.c:56`：`VISION_DET_PRIORITY = 4`

显示任务流程：

1. 等待 `VISION_NEW_FRAME`。
2. `vision_frame_peek_latest()` 获取最新相机帧。
3. dashboard 隐藏时跳过。
4. PPA 将原图缩放到 `s_preview_buf`。
5. `vision_draw_lcd_disp_cb()` 按时间戳取检测结果并画框。
6. 获取 LVGL 锁。
7. 对两块 framebuffer 分别调用 `blit_preview_to_fb()`。
8. 释放 LVGL 锁。

关键代码：

- `components/vision/framework/vision_app.c:267`：`vision_display_task()`
- `components/vision/framework/vision_app.c:287`：PPA 原图缩放到预览缓冲。
- `components/vision/framework/vision_app.c:323`：画框。
- `components/vision/framework/vision_app.c:328`：获取 LVGL 锁。
- `components/vision/framework/vision_app.c:330`：遍历 framebuffer。
- `components/vision/framework/vision_app.c:331`：blit 到 framebuffer。

`blit_preview_to_fb()` 当前在每次 PPA blit 前对视频横带执行 C2M writeback，避免 PPA invalidate 丢失 LVGL 已写但未落 PSRAM 的 cache 内容：

- `components/vision/framework/vision_app.c:224`：`blit_preview_to_fb()`
- `components/vision/framework/vision_app.c:230`：`esp_cache_msync(... ESP_CACHE_MSYNC_FLAG_DIR_C2M ...)`
- `components/vision/framework/vision_app.c:236`：PPA scale=1.0 搬运到 framebuffer 视频区域。

该处理对当前 DOUBLE_DIRECT + LVGL CPU flush + PPA 写 fb 的组合是必要的。

### 推理任务

文件：`components/vision/framework/vision_detect.c`

推理任务在 core1 运行，读取最新原始 RGB888 帧：

- `components/vision/framework/vision_detect.c:62`：`vision_detect_task()`
- `components/vision/framework/vision_detect.c:76`：读取最新帧引用。
- `components/vision/framework/vision_detect.c:137`：`vision_model_run()`。
- `components/vision/framework/vision_detect.c:216`：保存检测结果。

推理任务会大量读 PSRAM 中的相机帧，也会触发 esp-dl 模型、rodata、cache 访问。它应让位于屏幕刷新。

## PSRAM 竞争源分析

当前 PSRAM 读写竞争主要来自：

| 来源 | 访问类型 | 影响 |
|---|---|---|
| MIPI DSI DPI | 持续读 framebuffer | 最敏感，读不到会闪屏 |
| CSI / V4L2 | 写相机帧 | 影响采集稳定性 |
| PPA 原图缩放 | 读相机帧、写预览缓冲 | 每显示帧一次 |
| PPA blit | 读预览缓冲、写 framebuffer | 当前每帧写两块 fb |
| LVGL worker | 写 framebuffer / draw buffer | UI 刷新 |
| esp-dl 推理 | 读相机帧、模型、rodata | 可降帧 |
| CPU / cache / XIP | 指令与数据访问 | 可让路 |
| 诊断抓帧 | 大块 memcpy + cache writeback | 仅按键触发，不能常态化 |

`sdkconfig` 当前 PSRAM 相关配置：

```text
CONFIG_SPIRAM=y
CONFIG_SPIRAM_MODE_HEX=y
CONFIG_SPIRAM_SPEED_200M=y
CONFIG_SPIRAM_FETCH_INSTRUCTIONS=y
CONFIG_SPIRAM_RODATA=y
CONFIG_SPIRAM_XIP_FROM_PSRAM=y
CONFIG_SPIRAM_FLASH_LOAD_TO_PSRAM=y
CONFIG_SPIRAM_USE_MALLOC=y
CONFIG_SPIRAM_MALLOC_ALWAYSINTERNAL=16384
CONFIG_SPIRAM_MALLOC_RESERVE_INTERNAL=32768
CONFIG_CACHE_L2_CACHE_256KB=y
CONFIG_CACHE_L2_CACHE_LINE_128B=y
CONFIG_FREERTOS_TASK_CREATE_ALLOW_EXT_MEM=y
```

注意：官方 External RAM 文档说明，ESP32-P4 上 XIP from PSRAM 可能提升性能，也可能增加 PSRAM bus 负载，具体取决于应用访问模式。因此 XIP 配置应作为后续 A/B 测试项，而不是第一优先级直接修改项。

## 推荐方案

### P0：硬件总线优先级优先保屏

保留并强化当前 `bsp_psram_qos_tune()` 思路：

```c
axi_icm_ll_set_dw_gdma_qos_arbiter_prio(0, 8, 15);
axi_icm_ll_set_dw_gdma_qos_arbiter_prio(1, 8, 15);
axi_icm_ll_set_cache_qos_arbiter_prio(4, 4);
axi_icm_ll_set_cpu_qos_arbiter_prio(2, 2);
```

建议语义：

| 主体 | 读优先级 | 写优先级 | 说明 |
|---|---:|---:|---|
| DSI framebuffer DMA | 15 | - | 屏幕刷新最高优先级 |
| CSI / GDMA | - | 8~10 | 摄像头采集可排队但不能长期饿死 |
| PPA / DMA2D | 15 或与 GDMA 同级 | 8~10 | 允许显示链路硬件搬运优先于 CPU |
| Cache / XIP | 4 | 4 | 推理和代码访问可变慢 |
| CPU 普通访问 | 2 | 2 | 普通任务让路 |

风险：需要进一步确认 DW_GDMA0/1 与 DSI、CSI、PPA 的实际绑定关系。当前两个 DW_GDMA 都拉高是保守方案。

验证方式：

1. RTT 中确认打印：`[QoS] AXI ICM tuned...`
2. 如需读 AXI ICM 寄存器，必须先 halt。P4 上 running 态读取部分 HP 外设寄存器可能返回 0。

### P1：软件任务优先级调整

建议改为：

```c
#define VISION_FETCH_PRIORITY   6
#define VISION_DISP_PRIORITY    5
#define VISION_DET_PRIORITY     3
```

保持 LVGL worker：

```c
adapter_cfg.task_priority = 5;
adapter_cfg.task_core_id  = 1;
```

推荐任务表：

| 任务 | Core | 建议优先级 | 说明 |
|---|---:|---:|---|
| `vision_fetch` | 0 | 6 | 短任务，确保 DQBUF 及时返回，不长期占 CPU |
| `vision_disp` | 0 | 5 | 显示业务高于普通业务 |
| LVGL worker | 1 | 5 | 保持 UI worker 不被推理压制 |
| `vision_det` | 1 | 3 | 推理让位，允许掉帧 |
| system monitor | 0/任意 | 1~2 | 后台诊断，不应抢显示 |
| sorter / ethernet 普通任务 | 0/任意 | 3~4 | 不高于显示 |

理由：

- core0 上 fetch 是短任务，高优先级只保证“就绪即快速处理”，不会长期饿死显示。
- display 固定 core0，避免和 LVGL worker / 推理在 core1 上堆叠。
- detect 降到 3，保证 core1 上 LVGL worker 优先。
- 推理延迟只影响识别 FPS，不应影响屏幕持续刷新。

### P2：显示任务限流到屏幕刷新率

当前 `vision_display_task()` 每个 `VISION_NEW_FRAME` 都执行 PPA 缩放和双 framebuffer blit。如果摄像头帧率高于屏幕刷新率，会产生无意义的 PSRAM 压力。

建议在 `vision_display_task()` 中增加节拍门控：

```text
默认最多 30Hz：间隔 33333us
若仍闪屏，降到 20Hz：间隔 50000us
隐藏 dashboard 时继续完全跳过
```

策略：

```text
vision_fetch：尽量采集最新帧
vision_disp：最多按屏幕刷新率显示最新帧，中间帧直接丢弃
vision_det：按推理速度处理最新帧，必要时进一步降帧
```

推荐第一阶段使用 30Hz 限流，因为当前 DPI clock 已是约 30.2Hz。

预期收益：

- 减少 PPA 读相机帧。
- 减少 PPA 写预览缓冲。
- 减少 PPA 写两块 framebuffer。
- 减少 `esp_cache_msync()` 横带 writeback。
- 降低 PSRAM 总线峰值压力。

### P3：保留双写 framebuffer，先不优化为单写

当前显示任务对两块 framebuffer 都写入视频区域：

```c
for (int i = 0; i < s_fb_count; i++) {
    if (blit_preview_to_fb(s_fbs[i]) != ESP_OK) {
        break;
    }
}
```

优点：

- 无论 LVGL/DSI 翻到哪块 fb，都有最新视频。
- 避免预览区域拖尾。
- 实现简单，稳定性高。

缺点：

- framebuffer 写带宽翻倍。

建议：第一阶段保留双写。只有在屏幕稳定后，再考虑以下优化：

1. 通过 `on_refresh_done` 追踪当前 scan/next framebuffer。
2. 只写 inactive framebuffer。
3. 或者将视频区域拆成更独立的 overlay/plane（若驱动与硬件路径支持）。

不建议第一阶段直接做单写，因为 framebuffer 同步错误会引入撕裂、拖尾或局部旧帧。

### P4：推理与诊断链路降压

#### 推理让位

推理任务应接受降帧：

- `VISION_DET_PRIORITY` 降到 3。
- 如仍有闪屏，可增加推理节拍门控，例如每 2 帧或每 100ms 推理一次。
- 保持推理栈在 SRAM，不能回 PSRAM 栈。

背景：项目已有经验表明，PSRAM 栈在中断上下文可能造成双核死锁，因此 `vision_det` 栈必须继续留在内部 SRAM。

#### 诊断抓帧仅按需触发

`vision_detect.c` 中的诊断抓帧会执行整帧 `memcpy()` 和 `esp_cache_msync()`：

- `components/vision/framework/vision_detect.c:114`：检测抓帧请求。
- `components/vision/framework/vision_detect.c:122`：整帧 memcpy。
- `components/vision/framework/vision_detect.c:123`：cache writeback。

该路径只能作为按键触发诊断，不能常态化运行。

### P5：XIP from PSRAM 做 A/B 测试

如果执行 P0~P4 后仍有蓝闪或刷新不稳，再测试关闭 XIP from PSRAM。

当前相关配置：

```text
CONFIG_SPIRAM_FETCH_INSTRUCTIONS=y
CONFIG_SPIRAM_RODATA=y
CONFIG_SPIRAM_XIP_FROM_PSRAM=y
CONFIG_SPIRAM_FLASH_LOAD_TO_PSRAM=y
```

测试方案：

- A 组：保持当前 XIP from PSRAM。
- B 组：关闭 XIP from PSRAM 和 rodata/instruction fetch to PSRAM。

观察指标：

- 屏幕蓝闪是否减少。
- `vision_model_last_infer_ms()` 是否变差。
- 内部 RAM 是否仍足够。
- 系统启动与 SPIFFS 模型加载是否正常。

不建议第一阶段直接关闭，因为该选项可能对模型/代码访问有收益，必须用实测判断。

## 实施顺序

### 阶段 1：低风险调度调整

1. 调整任务优先级：
   - fetch：5 → 6
   - display：4 → 5
   - detect：4 → 3
2. 保持 LVGL worker prio 5 / core1。
3. 编译并运行。
4. 观察 RTT：任务 CPU 占用、推理耗时、显示是否闪屏。

### 阶段 2：显示任务限流

1. 在 `vision_display_task()` 增加 30Hz 门控。
2. 若仍闪屏，改为 20Hz。
3. 观察是否还出现 DSI FIFO underrun / 蓝闪。

### 阶段 3：验证硬件 QoS

1. 确认 `bsp_psram_qos_tune()` 被调用。
2. 需要时 halt 后读 AXI ICM 寄存器验证配置。
3. 如能区分 DSI/CSI/PPA 所属 GDMA master，再细化 QoS，而不是统一拉高两个 DW_GDMA。

### 阶段 4：XIP from PSRAM A/B 测试

仅当前三阶段仍不稳定时执行。

## 验证指标

建议通过 RTT 和肉眼观察结合验证：

1. 屏幕：
   - 无蓝闪。
   - 无局部撕裂。
   - dashboard 切页后视频区域不拖尾。
2. 帧率：
   - 显示稳定 20~30fps。
   - 推理 FPS 可低于显示 FPS。
3. CPU：
   - LVGL worker 不被推理长期压制。
   - core0 不被 display/PPA 调度造成长时间阻塞。
4. 内存：
   - 内部 SRAM largest block 仍足够创建 `vision_det` 栈。
   - PSRAM free 和 largest block 正常。
5. cache：
   - framebuffer 区域无 LVGL 圆环/仪表盘乱跳。
   - 诊断抓帧只在按键触发时出现大块 writeback。

## 推荐最终配置

```text
硬件总线：
DSI/GDMA 读优先级最高，cache/CPU 降权。

LCD：
1024×600 RGB888，DPI clock 26MHz，约 30Hz。

任务：
vision_fetch  core0 prio 6
vision_disp   core0 prio 5
LVGL worker   core1 prio 5
vision_det    core1 prio 3

显示：
最多 30Hz，必要时降 20Hz。
保留 DOUBLE_DIRECT 双 framebuffer 写入。

推理：
允许掉帧，不抢显示。
栈继续放内部 SRAM。

XIP：
先保持当前配置，仅在仍闪屏时做 A/B 测试。
```

## 结论

当前项目已经完成了最关键的基础工作：MIPI DSI 降到约 30Hz、DOUBLE_DIRECT、PPA 写 framebuffer、AXI ICM QoS。下一步不应优先重构显示链路，而应先做两个小而有效的调整：

1. **任务优先级改为 fetch 6 / display 5 / detect 3。**
2. **给 display 增加 30Hz 节拍门控，必要时降到 20Hz。**

这两项能最大限度降低 PSRAM 峰值竞争，同时保持现有双 framebuffer 稳定性。