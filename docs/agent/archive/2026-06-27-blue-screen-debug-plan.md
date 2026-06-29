# Display Blue-Screen Flash Debug

## 当前结论

当前已经从“怀疑 LCD underrun”推进到“降低 LCD scanout 对 PSRAM 实时依赖”的修复阶段。

最可信判断：短暂蓝屏来自 LCD DSI/DPI scanout starvation。LCD 持续从 PSRAM 读取 framebuffer 时，被 camera/CSI/ISP 写入图像帧的突发访问打断，导致显示链路短时间取不到数据。

这个判断分两层：

- 现象层：LCD DPI/DSI underrun 或同类显示取数饿死，可信度很高。ESP-IDF 5.5.4 的 DSI DPI 驱动在 underrun 分支会打印 `can't fetch data from external memory fast enough, underrun happens`，并说明屏幕可能已经变蓝。现象与当前 100-300 ms 蓝屏、无重启、自动恢复一致。
- 根因层：camera/CSI/ISP 写 PSRAM 的帧突发与 LCD framebuffer 读冲突，可信度也很高。已验证 camera stream 关闭后明显稳定；恢复 camera 写 PSRAM 后，即使 preview 关闭、YOLO 未真正 run，仍然能复现。

最新关键证据是 framebuffer sampled hash 稳定。蓝屏前 LCD framebuffer 采样 hash 没有明显突变，说明屏幕蓝色大概率不是应用层画出来的，也不是 LVGL、dashboard、preview 或按钮把 framebuffer 写成蓝色。问题更像显示扫描取数失败，而不是 framebuffer 内容被写坏。

## 当前固件状态

当前仓库处于诊断固件状态，不是最终业务配置：

- LCD：`1024x600 RGB24`
- LCD DPI clock：`40 MHz`
- LCD DMA2D：启用，`CONFIG_BSP_LCD_USE_DMA2D=y`
- Camera sensor mode：SC2336 `RAW10_640x480_50fps`
- Camera output buffer：`640x480 RGB888`
- Camera capture/duty period：`200 ms`
- Preview image：关闭
- Inference task：启用，但近期测试日志里 `run=0ms`，当前场景 ROI 早退，没有形成完整 YOLO 推理压力
- Blue marker：已放大，点击后打印 LCD、camera、dashboard 和 framebuffer hash 诊断

启动日志确认的关键带宽：

- LCD active read：约 `83608 KB/s`
- Camera active write：约 `4500 KB/s`
- Camera frame size：约 `900 KB/frame`

之前 `1024x600 RGB888` 相机帧约 `1800 KB/frame`，在 `200 ms` duty 下 active write 约 `9000 KB/s`。切到 `640x480 RAW10` 后，ISP 后写入 PSRAM 的输出仍是 RGB888，但单帧从约 `1800 KB` 降到约 `900 KB`。这降低了 camera 侧压力，但还没有证明问题被根治。

## 现象

- `bug_project`：屏幕会短暂闪蓝，之前可达到每分钟多次。
- 单次蓝屏持续约 100-300 ms。
- 板子不重启，日志继续输出，UI 会恢复。
- `merge_project` 也有类似现象，但频率更低，通常几分钟一次。

## 已实现诊断

### BLUE MARK

第一屏提供 `BLUE MARK` 按钮。看到蓝屏后尽快点击，日志会打印：

- `[BLUE_MARK] #N`：事件编号、`esp_timer` 时间、tick、core、heap
- `[LCD_DIAG]`：LCD 分辨率、色深、DPI clock、framebuffer 地址、active read、DMA2D 状态
- `[CAM_DIAG]`：camera stream 状态、帧尺寸、active write、capture count、timeout、最近 receive 时间
- `[BLUE_MARK] fb_hash ...`：最近约 2.4 秒的 LCD framebuffer sampled hash
- dashboard 当前业务状态

`BLUE MARK` 是当前最可靠的人工事件锚点，因为 ESP-IDF 没有公开的 DSI underrun callback，且 `underrun happens` 日志并不总能在可见蓝屏附近出现。

### Framebuffer Hash

LCD 诊断任务每 100 ms 对 3 个 framebuffer 采样固定块并计算 hash，保留 24 条历史。点击 `BLUE MARK` 时 dump 最近历史。

判定方式：

- 如果蓝屏发生前 hash 稳定，说明 framebuffer 内容没有明显突变，更支持 display scanout starvation。
- 如果 hash 先突变再蓝屏，应转向排查 framebuffer overwrite、cache sync、LVGL/DMA2D 写入路径或地址管理错误。

当前观测：多次 mark 前 hash 基本稳定，少数最后一条 hash 改变出现在按钮点击导致 UI 重绘之后。

## 已完成实验

| 实验 | 配置 | 结果 | 判断 |
| --- | --- | --- | --- |
| 满负载 baseline | LCD 48 MHz，camera `1024x600 RGB888 30fps`，preview on，inference on | 约 30 秒内多次 `[BLUE_MARK]` | 满负载稳定复现 |
| Camera off | LCD/LVGL/UI/按钮/日志继续运行，camera stream disabled | 约 195 秒无 `[BLUE_MARK]` | camera path 是必要触发因素 |
| 降 LCD clock | LCD 40 MHz，camera continuous 30fps | 蓝屏频率下降但仍复现 | 降低 LCD 读压力有效但不够 |
| LCD 36 MHz | camera continuous 30fps | 仍复现，且接近显示时序边界 | 继续降 clock 不是理想修复 |
| Preview off | LCD 40 MHz，duty 100 ms，preview image off | `images=0`，仍复现 | preview 不是必要触发因素 |
| App-side capture throttle | 应用少取帧，camera stream 仍连续 | 仍复现 | 只降应用取帧不等于硬件少写 PSRAM |
| Hardware stream duty | duty 100 ms / 200 ms | 有改善但仍复现 | 降平均带宽不足，单帧突发仍可触发 |
| RAW10 640x480 | SC2336 `RAW10_640x480_50fps`，ISP 输出 `640x480 RGB888`，duty 200 ms | active write 降到约 `4500 KB/s`，mark 前 hash 稳定 | 降低 camera 写压力有帮助，但未证明根治 |
| DMA2D off | 修正 fallback 后设置 `CONFIG_BSP_LCD_USE_DMA2D=n` | 复现更差 | DMA2D off 不是有效缓解，已恢复 DMA2D on |

## 已排除或降级的假设

| 假设 | 当前判断 |
| --- | --- |
| 应用层主动画成蓝色 | 不像。camera off 时 UI/LVGL 仍运行但明显稳定；hash 也不支持 framebuffer 先变蓝。 |
| YOLO 推理导致 | 不是必要条件。近期日志 `run=0ms`，模型加载但 ROI 早退，仍能复现。 |
| Preview image 上传导致 | 不是必要条件。preview off、`images=0` 时仍复现。 |
| 纯 LCD 时序错误 | 不像主因。camera off 时稳定；但 LCD clock 和时序会影响裕量。 |
| 电源、重启或 crash | 不像。无重启，日志连续，系统恢复。 |
| DMA2D 是主因 | 不支持。DMA2D off A/B 更差。 |

## 当前分析

LCD 40 MHz、RGB24、1024x600 下，LCD framebuffer active read 约 `83 MB/s`。这是持续读压力。

camera 当前已降到 `640x480 RGB888`，每帧约 `900 KB`。虽然 `200 ms` duty 把平均写带宽降到约 `4.5 MB/s`，每次 capture 仍然会形成一次较大的 PSRAM 写入突发。之前 `1024x600 RGB888` 时单帧约 `1.8 MB`，复现更明显。

因此重点不只是平均带宽，而是最坏瞬时延迟：

- LCD scanout 需要连续、低延迟地读取 framebuffer；
- camera/CSI/ISP 写入一帧时产生突发访问；
- 如果突发访问与 LCD active scanout 的关键时刻碰撞，LCD FIFO 可能饿死；
- DSI/DPI 驱动可能打印 underrun，但当前没有公开 callback，日志也未必每次都能作为事件锚点。

DMA2D off 更差也有信息量：它基本排除了“DMA2D 是主因”的方向。更合理的解释是 DMA2D on 至少让 framebuffer copy 更短、更规整；off 后走 CPU memcpy/cache sync 或 fallback 路径，反而增加 CPU cache、PSRAM copy 或 cache msync 干扰。因此 DMA2D 保持 on，不再作为主修复方向。

当前真正脆弱点是 `1024x600 RGB24` LCD framebuffer 以约 `83 MB/s` 持续从 PSRAM 被 DSI/DPI/GDMA 扫描读取。camera 写突发是把这个实时链路撞到 underrun 的触发因素。

## 日志判定规则

看到蓝屏后尽快点击 `BLUE MARK`。分析日志时按以下优先级判断：

1. 确认级：`[BLUE_MARK]` 前后约 0-1500 ms 内出现 `can't fetch data from external memory fast enough, underrun happens`，判定为 LCD DSI/DPI underrun。
2. 高概率级：没有 underrun 日志，但 mark 前后 camera stream active，且 framebuffer hash 稳定，同时 camera off 组稳定，判定为 camera 写突发诱发显示链路 starvation。
3. 反证级：mark 前 framebuffer hash 先异常突变，或 camera buffer 与 LCD framebuffer 地址重叠，转向查 memory overwrite、cache sync 或地址生命周期。

需要重点保留的日志：

```text
[BLUE_MARK] ...
[BLUE_MARK] fb_hash ...
[LCD_DIAG] ...
[CAM_DIAG] ...
[VISION_DIAG] ...
can't fetch data from external memory fast enough, underrun happens
```

## 相关配置开关

BSP 侧：

- `CONFIG_BSP_LCD_DIAG_ENABLE`：打印 LCD 诊断。
- `CONFIG_BSP_LCD_DPI_CLOCK_MHZ`：DPI pixel clock。降低可减少 LCD framebuffer 持续读压力，但太低会接近面板时序边界。
- `CONFIG_BSP_LCD_USE_DMA2D`：LCD framebuffer copy 是否使用 DMA2D。已测试 off 更差，当前保持 on。
- `CONFIG_CAM_SENSOR_DIAG_LOG`：打印 camera capture timing 和估算写带宽。
- `CONFIG_CAM_SENSOR_STREAM_DUTY_PERIOD_MS`：硬件 stream duty 周期。比应用层少取帧更有效，但不能消除单帧突发。

Vision App 侧：

- `CONFIG_VISION_CAM_WIDTH` / `CONFIG_VISION_CAM_HEIGHT`：请求 camera capture 尺寸。
- `CONFIG_VISION_APP_ENABLE_CAMERA_STREAM`：是否启用 camera stream。
- `CONFIG_VISION_APP_ENABLE_PREVIEW_IMAGE`：是否更新 preview image。
- `CONFIG_VISION_PREVIEW_IMAGE_EVERY_N_FRAMES`：preview image 更新间隔。
- `CONFIG_VISION_APP_ENABLE_INFERENCE`：是否启用 vision/inference task。
- `CONFIG_VISION_APP_PERF_LOG`：打印 vision timing summary。

Sensor 侧：

- 当前已支持按 sensor picked format 自动选择 RAW8/RAW10 的 CSI/ISP input。
- SC2336 当前使用 `RAW10_640x480_50fps`。

## 执行计划

### 1. 加 DSI pattern 快速确认实验

在做 RGB565 之前，先加一个 DSI pattern mode + camera on 的短实验。这个实验不是修复，但能把方向钉死。

测试目标：把“从 PSRAM 读 framebuffer scanout”路径基本拿掉，只保留 DSI host/PHY/panel 输出 pattern。若 pattern 模式下 camera 压力仍不蓝，而恢复 framebuffer scanout 就蓝，后续就可以专注 PSRAM framebuffer scanout，而不是应用层绘制、LVGL、preview 或面板本身。

测试配置：

- LCD 40 MHz
- DSI pattern 输出
- camera `640x480 RGB888`，先保持 duty 200 ms；必要时提高 camera 压力
- 运行 5-10 分钟

判定：

| 结果 | 解释 |
| --- | --- |
| Pattern + camera stress 10 分钟无蓝屏、无 underrun | 强确认：问题在 PSRAM framebuffer scanout，不是 DSI PHY/面板本身 |
| Pattern + camera stress 仍蓝 | 重新考虑 DSI PHY、panel power、MIPI 干扰、电源或更底层 DSI host 问题 |

注意：pattern 会覆盖 UI，屏幕上的 `BLUE MARK` 按钮可能不可用。这个测试最好加 UART 命令、GPIO 按钮或固定计时日志来标记肉眼蓝屏。

### 2. 实现完整 LCD RGB565 路径

RGB565 是当前最值得押注的修复方向，因为它直接降低 LCD framebuffer active read。当前 RGB24 active read 约 `83608 KB/s`；RGB565 后理论约 `55739 KB/s`，下降约 33%。

不要只改一个配置项，需要确认整条显示路径真正变成 16 bpp：

| 项目 | 应确认 |
| --- | --- |
| DPI framebuffer input format | `esp_lcd_dpi_panel_config_t.in_color_format = LCD_COLOR_FMT_RGB565` |
| DPI output format | 若项目显式配置 `out_color_format`，设为 RGB565；或确认默认等于 input |
| panel/controller bits | `esp_lcd_panel_dev_config_t.bits_per_pixel = 16`，并确认 panel init 的像素格式命令匹配 |
| LVGL | `LV_COLOR_DEPTH=16`，draw buffer、image buffer 按 2 bytes/pixel |
| LCD_DIAG | 打印 `bpp=16`，fb size 约 `1024*600*2 = 1228800 bytes` |
| active_read | 从约 `83608 KB/s` 降到约 `55739 KB/s` |

如果 RGB565 后 `[LCD_DIAG] active_read` 没有接近 2/3，说明改法没有真正打到 framebuffer scanout 带宽。

### 3. RGB565 第一组验证

第一轮不要恢复 preview 和完整 YOLO，先验证 LCD read 降低后是否能承受当前 camera 写突发。

配置：

- LCD RGB565
- LCD 40 MHz
- DMA2D on
- camera `RAW10_640x480_50fps`
- ISP output `640x480 RGB888`
- duty 200 ms
- preview off
- inference task on，但允许 ROI 早退

判定：

| 结果 | 结论 |
| --- | --- |
| 10 分钟 0 `BLUE_MARK`，0 `underrun` | RGB565 LCD 是强候选修复 |
| 仍蓝，但频率明显下降 | 方向正确，但 camera 写突发仍需继续降 |
| 频率几乎不变 | 转向 DSI pattern、DMA/GDMA 优先级、PSRAM/cache/PHY 方向 |

### 4. RGB565 稳定后逐步恢复业务

恢复顺序固定，不一次性全开：

1. RGB565 + 640x480 camera + duty 200 ms + preview off，10 分钟。
2. 恢复 preview image，但保持低频，例如 every N frames，10 分钟。
3. 恢复真实目标场景下 YOLO run，确认 `run>0ms`，10 分钟。
4. duty 从 200 ms 调到 100 ms，10 分钟。
5. 最后再评估是否需要 continuous stream。

preview 和 YOLO 不是必要触发条件，但仍可能是放大器。它们不应该在根因判断阶段占主线，但必须在候选修复验收阶段逐步加回来。

### 5. 如果 RGB565 后仍复现

继续降低 camera 单次突发和 DMA/PSRAM 最坏延迟，优先顺序如下：

1. camera/ISP 输出改 RGB565 或更小业务帧。
   - `640x480 RGB888`：约 `900 KB/frame`
   - `640x480 RGB565`：约 `600 KB/frame`
   - `320x240 RGB888`：约 `225 KB/frame`
   - `320x240 RGB565`：约 `150 KB/frame`
   - `YUV422 640x480`：约 `600 KB/frame`
   - `GRAY 640x480`：约 `300 KB/frame`
2. 若还没做，必须做 DSI pattern 测试。pattern 稳定就继续围绕 PSRAM scanout；pattern 也蓝，就查 DSI PHY 2.5 V、lane rate、panel init、排线/连接、电源瞬态。
3. 查 PPA/DMA/GDMA burst 和仲裁。
   - PPA 是否被 LVGL/rotation/scale 使用
   - PPA/DMA2D burst length 是否可调
   - DSI GDMA channel priority 是否可提升
   - camera/CSI/ISP 写 PSRAM burst 是否可缩短
   - camera DMA priority 是否可降低
4. 再测 PSRAM/cache 性能配置。
   - `CONFIG_SPIRAM_XIP_FROM_PSRAM=y`
   - `CONFIG_CACHE_L2_CACHE_256KB=y`
   - `CONFIG_CACHE_L2_CACHE_LINE_128B=y`
   - `COMPILER_OPTIMIZATION_PERF=y`
5. 用新 ESP-IDF 分支做最小复现对比，判断是硬件极限还是 IDF 5.5.4 驱动/仲裁配置问题。

DMA2D 保持 on，不再把 DMA2D off 作为主修复方向。

## 修复判定标准

`bug_project` 候选修复至少需要满足：

- 完整目标业务功能开启，包括 camera、UI、preview 或替代显示路径，以及有目标场景下的推理流程。
- 真实目标场景下 YOLO `run>0ms`。
- 连续 10 分钟无可见蓝屏、无 `[BLUE_MARK]`。
- 无 `underrun happens`。
- framebuffer hash 无异常突变。
- camera timeout 为 0 或符合预期范围。
- UI、touch、camera、vision task、Ethernet 均正常启动和运行。
- 无自动重启，无持续 heap 下降。

`merge_project` 因为原始复现频率更低，候选修复除了 10 分钟功能验收，还需要 30-60 分钟 soak test。
