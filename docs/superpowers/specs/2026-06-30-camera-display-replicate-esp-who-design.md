# 摄像头回显复刻 ESP-who（A2 + none 单缓冲）设计

日期：2026-06-30
状态：已与用户对齐，待实现

## 背景与动机

ESP32P4_Detection 的整条视觉链路（采集 / 零拷贝 peek 帧总线 / 画框 / 推理）是 1:1 复刻
esp-who（E:\esp-who-master）。唯独**摄像头回显**这一段是自研的：当前显示任务用
PPA 缩放后走 `lv_canvas` 乒乓双缓冲交 LVGL 软合成（`VISION_RENDER_LVGL_CANVAS=1` 新方案），
另保留一套双写物理 framebuffer 的旧方案 A2（`=0`）。

用户要求：**删除自研回显方案，改为全硬件链路**——摄像头原始画面经硬件 PPA 缩放后，
由 PPA/DMA 直接搬到 LCD framebuffer 的预览容器子矩形，CPU 不参与像素绘制。

### 关键架构事实

- **esp-who 的显示哲学**：摄像头帧尺寸 = 显示尺寸、全屏，所以显示侧零拷贝零缩放
  （`lv_canvas_set_buffer(canvas, fb->buf, ...)` 或裸屏 `esp_lcd_panel_draw_bitmap`）。
  缩放只发生在帧链的 `WhoPPAResizeNode`（供检测用），不在显示阶段。
- **本项目的约束**：视频嵌在 dashboard 子容器 `cont_live_vedio`（640×375，非全屏），
  摄像头出 RGB888 大帧。「帧尺寸 = 显示尺寸 = 全屏」前提不成立，必须缩放。
- **LVGL canvas 路径必经 CPU**：只要终点是挂在容器里的 `lv_canvas`，LVGL 一定用 CPU
  把内容合成进 framebuffer。要做到「CPU 不碰像素」，唯一路径是 esp-who 裸屏
  `draw_bitmap` 的「子矩形版」：PPA 缩到 640×375，再 PPA/DMA 搬到 panel fb 的预览子矩形。
  这正是旧方案 A2 的做法。
- **`none 模式`** = `ESP_LV_ADAPTER_TEAR_AVOID_MODE_NONE` = **单 framebuffer**
  （`esp_lv_adapter_get_required_frame_buffer_count(NONE, ROTATE_0)` 返回 1）。
  LVGL 工作在 partial render（draw buffer height=50，只 flush 脏区到 panel fb）。
  Panel：MIPI DSI EK79007，1024×600，RGB888（24bit/px），`use_dma2d=1`。

## 目标链路

```
fetch peek 最新 RGB888 帧（零拷贝帧总线，已复刻 esp-who，不动）
  → PPA SRM 缩放到 640×375 中转缓冲 s_preview_buf          (硬件)
  → vision_draw_lcd_disp_cb 在中转缓冲上叠检测框            (CPU，仅画框，复刻 esp-who lcd_disp_cb，不动)
  → PPA(scale=1.0) 把中转缓冲 DMA 搬到唯一 fb 的预览子矩形  (硬件 DMA)
  → DPI 持续扫描该 fb 上屏
```

全程 PPA/DMA 搬运像素；CPU 仅在中转缓冲上画检测框（这步 esp-who 本身也在 buf 上画，属复刻范围）。

## 改动范围

集中在 3 个文件。`vision_draw.c`（画框）和零拷贝帧总线（fetch / ringbuf / 订阅）**完全不动**。

### 1. `components/vision/framework/vision_app.c`

**删除（自研 canvas 新方案）：**
- 编译开关宏 `VISION_RENDER_LVGL_CANVAS`（及所有 `#if/#else/#endif` 分支结构，固定走 A2）。
- canvas 静态变量：`s_canvas`、`s_canvas_buf[2]`、`s_canvas_idx`。
- `vision_display_task` 内的 canvas 分支：`lv_canvas_create` 提交、`lv_canvas_set_buffer`、
  `lv_obj_invalidate`、idx 翻转，以及那段一次性诊断埋点（`dbg_n` / `SEGGER_RTT_printf`）。
- `vision_start` 内的 canvas 分支：content 尺寸取法、两块 canvas_buf 的分配与回收、
  `lv_canvas_create` 容器内建画布。

**保留并启用（A2 路径作为唯一回显路径）：**
- 静态变量 `s_preview_buf`、`s_fbs[3]`、`s_fb_count`。
- `vision_display_task` 的 A2 分支：PPA 缩放到 `s_preview_buf` → `vision_draw_lcd_disp_cb`
  画框 → 持 LVGL 锁 + 复查 HIDDEN + blit 到 fb。
- `vision_start` 的 A2 分支：绝对坐标/整块尺寸取法（`lv_obj_get_coords` →
  `s_preview_x/y/w/h`）、`s_preview_buf` 分配、`BSP_LCD_GetFrameBuffers`。

**适配 none 单缓冲：**
- `s_fb_count` 校验从「`< 2` 报错」改为「`!= 1` 报错」（none 模式恒为 1 块）。
- blit 双写循环 `for (i < s_fb_count)` 自然退化为只写 1 块（count=1），逻辑无需特判。

**删除 cache 横带对齐逻辑（前提已消失）：**
- `blit_preview_to_fb` 开头那段「blit 前对整条视频横带做 C2M writeback」
  （`esp_cache_msync(band, band_len, C2M | UNALIGNED)`）。
  该逻辑专为 DOUBLE_DIRECT 两块 fb 的 arc dirty cache 不一致（翻转乱跳）设计；
  none 单缓冲只有一块 fb，不存在两块不一致，前提消失。
- 保留 `blit_preview_to_fb` 的 PPA(scale=1.0) 搬运本体（PPA 内部已处理 output cache）。

### 2. `components/bsp/bsp_lcd.c`

- `BSP_LCD_GetFrameBuffers`：当前写死 `esp_lcd_dpi_panel_get_frame_buffer(panel, 2, fb0, fb1)`
  取 2 块（注释还称「DOUBLE_DIRECT num_fbs=2」）。none 模式 panel 只有 1 块，取 2 块返回
  `ESP_ERR_INVALID_ARG`。**改为取 1 块**：`esp_lcd_dpi_panel_get_frame_buffer(panel, 1, fb0)`，
  `fb1`/`fb2` 若传入则置 NULL，更新注释为 none 单缓冲。
  （这是 A2 此前未启用所以未暴露的现存 bug，启用前必修。）

### 3. `components/vision/framework/vision.h`

- 仅在 `vision_start` 注释里若有提及双路径/canvas 的描述，同步更新为单一 A2 路径。无接口变更。

## 风险与取舍（none 单缓冲）

- **撕裂**：单缓冲下 DPI 边扫描边被 PPA 写，视频区理论上可能撕裂。这是 none 模式固有代价，
  用户已接受。P4 PPA 搬 640×375 RGB888 为亚毫秒级，撕裂窗口小，预期实测不明显。
  若实测明显撕裂，届时再议（TE 同步或回双缓冲），**不在本次范围**。
- **LVGL 覆盖视频区**：partial render 只 flush 脏区。视频区删掉 canvas 后无 LVGL 对象，
  PPA 写入不会被常规重绘覆盖。两种会盖的情况及对策：
  - 切页（dashboard 隐藏）：LVGL 重绘整屏 → A2 已有「锁内复查 HIDDEN，隐藏则不 blit」处理，保留。
  - 容器背景样式：`cont_live_vedio` 自带背景，首帧前会露背景；视频每帧 blit 覆盖其上即可。
    背景样式是否调整由用户定，默认保持现状。

## 验证方式

1. `./agentic/idf_build.sh`（10 分钟超时）编译通过，无残留 `VISION_RENDER_LVGL_CANVAS` 引用。
2. `flash-and-run --app-only` 烧录运行。
3. RTT 日志确认 `vision started` 且无 `expect ... framebuffers` 报错（单缓冲校验通过）。
4. 肉眼确认 dashboard 预览容器内显示实时摄像头画面 + 检测框，切到其他页再切回正常。
5. 观察是否有明显撕裂/花屏（none 模式取舍点）。

## 不做的事（YAGNI）

- 不引入 esp-who 的 `WhoPPAResizeNode` 帧链节点（那是「保留子容器、缩放挪进帧链」的方案 B，
  用户未选）。缩放仍在显示任务内做。
- 不动画框 `vision_draw.c`、不动零拷贝帧总线、不动推理任务。
- 不加 TE 同步、不回双/三缓冲（除非实测撕裂不可接受）。
