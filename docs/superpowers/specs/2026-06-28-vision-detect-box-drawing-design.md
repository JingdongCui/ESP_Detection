# 纯 C 复刻 esp-who 裸屏画框链路 — 设计

> 项目：ESP32P4_Detection ｜ 日期：2026-06-28
> 目标：在现有"采集 + PPA 缩放 + LCD 直刷"链路上，复刻 esp-who 的检测框绘制链路。
> 本阶段范围：只搭画框管线（假框占位验证），不接检测模型。

---

## 一、范围与边界

**做**：坐标映射 + 跨任务结果传递（时间戳对齐队列）+ 在 RGB888 预览缓冲上画矩形框。用假框（碰壁反弹）跑通整条链路。

**不做**：
- 不接 esp-dl 检测模型（`model->run`/预处理/NMS 留到下一步迭代）。
- 不画类别/置信度文字（esp-who 裸屏路径本就不画文字；esp-dl 也无文字绘制原语）。
- 不画关键点（本阶段假框无 keypoint；目标检测模型也无）。

**复刻原则**：每个 esp-who 部件 1:1 对应一个纯 C 部件，逻辑照搬。esp-who/esp-dl 是 C++，底层 C++ 调用（`draw_hollow_rectangle`、`result_t`/`std::list`）用等价纯 C 重写——逻辑一致、功能一致，不算自研。

---

## 二、对照表：esp-who 部件 → 纯 C 复刻落点

| esp-who 部件 | 源文件 | 纯 C 复刻落点 |
|---|---|---|
| `dl::detect::result_t`（含 keypoint，std::vector） | dl_detect_define.hpp:9 | C struct `vision_det_result_t { int category; float score; int box[4]; }`（不含 keypoint） |
| `WhoDetectResultLCDDisp::m_results`（std::queue 无界） | who_detect_result_handle.cpp | 定长环形数组 + mutex，深度宏 `VISION_RESULT_QUEUE_DEPTH` |
| `save_detect_result`（push 队列，mutex 保护） | :143 | C 函数，检测任务出框后调用 |
| `lcd_disp_cb`（时间戳对齐，skip future result） | :150 | C 函数，显示任务 PPA 后、draw_bitmap 前调用 |
| `draw_detect_results_on_img` → `draw_hollow_rectangle` | :9 / dl_image_draw.hpp | C 函数：在 s_preview_buf(RGB888) 逐像素画空心矩形 |
| `rescale_detect_result` / `set_rescale_params` | who_detect.cpp:32,98 | C 函数：box × (预览/原图) 比例 + clip 到预览边界 |
| palette 颜色表 | :116 | RGB888 红色 `{255,0,0}`（esp-who 默认 palette 即红） |

---

## 三、数据流（每帧）

```
检测任务 (core1, vision_detect.c)              显示任务 (core0, vision_app.c)
─────────────────────────────                 ─────────────────────────────
peek 最新帧 (原图坐标系)                         等 NEW_FRAME → peek 最新帧
造假框 result[] (原图坐标系)                      PPA 缩放转 RGB888 → s_preview_buf
rescale: box × (preview/原图) + clip            ┌─ BSP_LVGL_Lock()
  → 预览坐标系                                   ├─ 锁内复查 HIDDEN
save_detect_result(result[], timestamp)         ├─ vision_draw_lcd_disp_cb(&fb):
        │                                        │     时间戳对齐取结果
        └──── push 结果队列 ───────────────►     │     在 s_preview_buf 画空心矩形
                                                 ├─ esp_lcd_panel_draw_bitmap 直刷
                                                 └─ BSP_LVGL_Unlock()
```

**与 esp-who 的唯一架构差异**：esp-who 把框画在摄像头帧 fb 本身（fb 既是显示源又是画布）；本项目显示源是 PPA 缩放后的 `s_preview_buf`（预览坐标系），故框画在 `s_preview_buf` 上，box 在检测侧就 rescale 到预览坐标系。本质仍是"在显示画布上画框"，画布换成 preview_buf 是直刷架构的必然结果。

---

## 四、关键设计点

### 4.1 坐标映射（rescale）
检测在原图坐标系（`fb.width × fb.height`）造框，显示在预览坐标系（`s_preview_w × s_preview_h`）。映射：
```
box_preview_x = box_orig_x * s_preview_w / fb.width
box_preview_y = box_orig_y * s_preview_h / fb.height
```
再 clip 到 `[0, s_preview_w-1]` / `[0, s_preview_h-1]`（复刻 `limit_box`）。检测侧需读到预览尺寸 → 经 `vision_internal.h` 暴露的接口获取。

### 4.2 时间戳对齐（复刻 skip-future 逻辑）
显示任务取当前显示帧 `fb.timestamp`，遍历结果队列：帧 timestamp ≥ 结果 timestamp 则取该结果并 pop，继续直到结果比帧更新为止——保留最贴合当前显示帧的那份结果。`vision_frame_t` 已带 `timestamp`，检测/显示 peek 同一帧拿到同一时间戳，天然可对齐。

### 4.3 画框算法（纯 C 复刻 draw_hollow_rectangle）
RGB888 缓冲，每像素 3 字节。空心矩形 = 上下两条横线 + 左右两条竖线，线宽 2。逐像素写入：
```
offset = (y * s_preview_w + x) * 3;  buf[offset]=R; buf[offset+1]=G; buf[offset+2]=B;
```
边界 clip 防越界。颜色红 `{255, 0, 0}`。

### 4.4 假框形态：碰壁反弹
一个矩形框，在预览区域内缓慢移动、碰边反弹。检测任务每帧更新位置（原图坐标系），经 rescale 落到预览。既验证画框，又能肉眼确认坐标映射正确（框始终贴预览区内、不越界、运动连续）。

### 4.5 结果队列深度（单独宏，不沿用取帧深度）
```c
#define VISION_RESULT_QUEUE_DEPTH 3   // 容忍检测快于显示时的积压，复刻 esp-who m_results
```
定义在 `vision_draw.c` 顶部。**不沿用取帧 ringbuf 深度**（那是运行时 `fb_count-2`，绑定摄像头硬件缓冲数，语义无关）——两者来源独立，避免改 fb_count 时意外带动结果队列。esp-who 用无界 queue，纯 C 定长取 3 足够（时间戳对齐每帧消费，正常不积压超 1-2）。

### 4.6 锁与时序
- 画框（写 `s_preview_buf`）放在现有 `BSP_LVGL_Lock` 内、`draw_bitmap` 之前。画进缓冲是纯内存操作、不碰 panel，安全。
- 结果队列用自己的 mutex（复刻 `m_res_mutex`），与 LVGL 锁解耦。
- 锁内复查 HIDDEN 的现有逻辑保持不变，画框插在复查通过之后、draw_bitmap 之前。

---

## 五、文件改动

| 文件 | 改动 |
|---|---|
| `vision_draw.c`（**新增**） | 结果结构体队列 + mutex；`vision_draw_save_result`（push）；`vision_draw_lcd_disp_cb`（对齐+画框）；纯 C 空心矩形绘制；深度宏。对应 who_detect_result_handle.cpp 整文件 |
| `vision_internal.h` | 声明 `vision_det_result_t`、save/cb 接口；暴露预览尺寸给检测侧 rescale |
| `vision_detect.c` | 假框生成（碰壁反弹，原图坐标系）+ rescale + 调 `vision_draw_save_result` |
| `vision_app.c` | 显示任务锁内、draw_bitmap 前插 `vision_draw_lcd_disp_cb(&fb)` |
| `vision/CMakeLists.txt` | 注册 `vision_draw.c` |

---

## 六、验证方式
1. 编译通过（`./agentic/idf_build.sh`，10 分钟超时）。
2. 烧录运行，肉眼确认预览区出现一个红色空心矩形，缓慢移动、碰壁反弹、不越界。
3. RTT 日志确认检测任务正常 save、显示任务正常对齐取结果。
4. 切到其他页面（dashboard 隐藏）时框随预览一起消失（锁内 HIDDEN 复查生效）。

---

## 七、下一步迭代（不在本阶段）
- 接 esp-dl 检测模型：`model->run` 是 C++，纯 C 调用需 C++ 薄封装暴露 C 接口。
- 真实类别名出现后，再决定是否加文字（自写位图字体 / LVGL overlay）。
