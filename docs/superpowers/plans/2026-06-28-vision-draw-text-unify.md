# 画框+文本统一走显示侧时间戳对齐 实施计划

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** 把 UI 文本投递从 detect(core1) 持 LVGL 锁的路径移到 display(core0) 的时间戳对齐路径，让文本与画框完全同步，并消除「假框不死、真推理死」的跨核锁竞争。

**Architecture:** detect 只算推理 + 把整份 `vision_result_event_data_t ev` 内嵌进结果结构后 save_result，完全不碰 LVGL 锁；display 在 `vision_draw_lcd_disp_cb` 时间戳对齐选出 `s_current` 后，既画框（无锁写 s_preview_buf）又持一次 LVGL 锁 `send_event(&s_current.ev)`，零推导。

**Tech Stack:** ESP-IDF / FreeRTOS 双核 / anyui evt 事件系统 / esp_lv_adapter 递归锁 / JTAG+RTT 验证。

参照 spec：`docs/superpowers/specs/2026-06-28-vision-draw-text-unify-design.md`

---

## 文件结构

| 文件 | 责任 | 改动 |
| --- | --- | --- |
| `components/vision/vision_internal.h` | 组件内共享结构/接口 | `vision_det_frame_t` 内嵌 `ev`，include evt.h |
| `components/vision/vision_detect.c` | 推理任务(core1) | 去掉持锁投递，改填 `result.ev` 进队列 |
| `components/vision/vision_draw.c` | 结果队列+画框+文本投递 | 显示侧对齐后画框 + 持锁 send_event |

---

### Task 1: 结构内嵌 ev 字段

**Files:**
- Modify: `components/vision/vision_internal.h:57-61`（`vision_det_frame_t`）
- Modify: `components/vision/vision_internal.h:12-21`（include 区）

- [ ] **Step 1: 在 include 区加 evt.h**

`vision_internal.h` 现有 include 区（第 14-17 行附近）末尾追加：

```c
#include "evt.h"           // vision_result_event_data_t（UI 文本投递结构）
```

- [ ] **Step 2: 给 vision_det_frame_t 内嵌 ev**

把现有定义（第 57-61 行）：

```c
typedef struct {
    vision_det_result_t items[VISION_MAX_BOXES];
    int count;
    int64_t timestamp;
} vision_det_frame_t;
```

改为：

```c
typedef struct {
    vision_det_result_t items[VISION_MAX_BOXES];  // 画框坐标（预览坐标系）
    int count;
    int64_t timestamp;                            // 显示侧对齐用
    vision_result_event_data_t ev;                // 投文本用，detect 一次性填好整份
} vision_det_frame_t;
```

---

### Task 2: detect 去掉持锁投递，改填 result.ev

**Files:**
- Modify: `components/vision/vision_detect.c:22-31`（include 区）
- Modify: `components/vision/vision_detect.c:66-145`（`#if 1` 包裹 + 投递块）

- [ ] **Step 1: 删除不再需要的 include**

删除第 29-30 行（detect 不再 send_event/持锁）：

```c
#include "sdk.h"                       // send_event / get_current_event_table / EVT_VISION / vision_result_event_data_t
#include "bsp_lvgl_adapter_init.h"     // BSP_LVGL_Lock / BSP_LVGL_Unlock
```

保留 `vision_internal.h`（已含 evt.h → `vision_result_event_data_t` 仍可见）、
`vision_model.h`、`SEGGER_RTT.h`、esp_timer.h、FreeRTOS 头。

- [ ] **Step 2: 把文本组装写进 result.ev，删除持锁投递动作**

当前第 66-145 行结构为：`#if 1` → 推理+rescale+save_result → 组装局部 `ev` →
`BSP_LVGL_Lock/send_event/BSP_LVGL_Unlock` → `#endif`。

改为去掉 `#if 1/#endif` 包裹；把局部 `vision_result_event_data_t ev = {0};`
改成直接操作 `result.ev`；删除 `save_result` 之后的整段投递动作中的锁与 send_event，
只保留组装。**注意：组装顺序要在 save_result 之前**（先填好 ev 再 push）。

把第 66-145 行整段替换为：

```c
        // ===== 真实推理：原图 RGB888 → ROI/前处理/model->run/后处理 → 原图坐标框 =====
        vision_model_det_t dets[VISION_MAX_BOXES];
        int n = vision_model_run(fb.buf, fb.width, fb.height, dets, VISION_MAX_BOXES);
        if (n < 0) {
            n = 0;  // 推理失败按无目标处理
        }

        // rescale 原图坐标 → 预览坐标，逐框 clip（复刻 rescale_detect_result + limit_box）。
        vision_det_frame_t result = {0};
        result.timestamp = fb.timestamp;  // 与帧对齐，供显示侧时间戳对齐
        int pw = 0, ph = 0;
        vision_get_preview_size(&pw, &ph);
        int kept = 0;
        if (pw > 0 && ph > 0 && fb.width > 0 && fb.height > 0) {
            float sx = (float)pw / fb.width;
            float sy = (float)ph / fb.height;
            for (int i = 0; i < n && kept < VISION_MAX_BOXES; i++) {
                int x1 = (int)(dets[i].box[0] * sx);
                int y1 = (int)(dets[i].box[1] * sy);
                int x2 = (int)(dets[i].box[2] * sx);
                int y2 = (int)(dets[i].box[3] * sy);
                if (x1 < 0) x1 = 0;
                if (x1 > pw - 1) x1 = pw - 1;
                if (y1 < 0) y1 = 0;
                if (y1 > ph - 1) y1 = ph - 1;
                if (x2 < 0) x2 = 0;
                if (x2 > pw - 1) x2 = pw - 1;
                if (y2 < 0) y2 = 0;
                if (y2 > ph - 1) y2 = ph - 1;
                result.items[kept].category = dets[i].category;
                result.items[kept].score    = dets[i].score;
                result.items[kept].box[0]   = x1;
                result.items[kept].box[1]   = y1;
                result.items[kept].box[2]   = x2;
                result.items[kept].box[3]   = y2;
                kept++;
            }
        }
        result.count = kept;

        // ===== 组装 UI 文本数据进 result.ev（不投递，随结果进队列，由显示侧对齐后 send）=====
        // 空帧同样填好"无目标"那份，保证文本与画框完全同步。
        if (result.count > 0) {
            strcpy(result.ev.status, "识别成功");
            int best = 0;
            for (int i = 1; i < result.count; i++) {
                if (result.items[i].score > result.items[best].score) {
                    best = i;
                }
            }
            result.ev.confidence = (int)(result.items[best].score * 100.0f);
            // 类别名映射：0=极兔 1=中通 2=韵达（默认，待实测核对）
            static const char *kClassName[3] = {"极兔", "中通", "韵达"};
            int cat = result.items[best].category;
            if (cat >= 0 && cat < 3) {
                strcpy(result.ev.company, kClassName[cat]);
            } else {
                strcpy(result.ev.company, "--");
            }
        } else {
            strcpy(result.ev.status, "无目标");
            result.ev.confidence = 0;
            strcpy(result.ev.company, "--");
        }
        result.ev.fps_x10 = fps10;
        result.ev.infer_time_ms = vision_model_last_infer_ms();
        // 三类概率×100（极兔/中通/韵达），A/M 暂同填概率值（占位，后续可分平均/峰值）
        int jt = 0, zt = 0, yd = 0;
        vision_model_get_class_probs(&jt, &zt, &yd);
        result.ev.jt_a = jt; result.ev.jt_m = jt;
        result.ev.zt_a = zt; result.ev.zt_m = zt;
        result.ev.yd_a = yd; result.ev.yd_m = yd;

        // push 进结果队列（detect 全程只持轻量 s_mutex，不碰 LVGL 锁）。
        vision_draw_save_result(&result);

        SEGGER_RTT_printf(0, "[vision_det] frame #%u %dx%d boxes=%d infer=%dms %d.%d fps\n",
                          (unsigned)frame_seq, fb.width, fb.height,
                          result.count, vision_model_last_infer_ms(),
                          fps10 / 10, fps10 % 10);
```

注意：RTT 打印从 `#endif` 之后移到块内（紧跟 save_result），因为 `result` 变量
作用域在 while 循环体内，去掉 `#if` 包裹后位置不影响，但要确保 `result` 已声明。

---

### Task 3: vision_draw.c 显示侧画框后投递文本

**Files:**
- Modify: `components/vision/vision_draw.c:14-18`（include 区）
- Modify: `components/vision/vision_draw.c:135-159`（`vision_draw_lcd_disp_cb`）

- [ ] **Step 1: 加 include**

`vision_draw.c` 现有 include 区（第 14-18 行）末尾追加：

```c
#include "sdk.h"                       // send_event / get_current_event_table / EVT_VISION
#include "bsp_lvgl_adapter_init.h"     // BSP_LVGL_Lock / BSP_LVGL_Unlock
```

（`vision_internal.h` 已含 evt.h → `vision_result_event_data_t`/`EVT_VISION`/
`EVT_VISION_RESULT_CHANGED` 可见。）

- [ ] **Step 2: 在画框后投递文本**

当前 `vision_draw_lcd_disp_cb`（第 135-159 行）末尾是：

```c
    vision_det_frame_t to_draw = s_current;  // 拷出，缩短持锁时间
    xSemaphoreGive(s_mutex);

    draw_results_on_buf(preview_buf, preview_w, preview_h, &to_draw);
}
```

改为（画框后，用 to_draw.ev 持 LVGL 锁投递一次）：

```c
    vision_det_frame_t to_draw = s_current;  // 拷出，缩短持锁时间
    xSemaphoreGive(s_mutex);

    draw_results_on_buf(preview_buf, preview_w, preview_h, &to_draw);

    // 投递与本显示帧对齐的文本（detect 已填好整份 ev，含空帧的"无目标"）。
    // 持一次 LVGL 锁，独立于显示任务后续的 blit 锁段——避免锁嵌套与持锁时长叠加。
    // send_event 是同步回调派发（见 evt.c），ui_vision_result_event_cb 仅 label set，
    // 非阻塞。全系统持 LVGL 锁的只剩 display 与 LVGL worker，回到 esp-who 式单持锁者。
    BSP_LVGL_Lock();
    send_event(get_current_event_table(), EVT_VISION, EVT_VISION_RESULT_CHANGED,
               (uint8_t *)&to_draw.ev, 0);
    BSP_LVGL_Unlock();
}
```

注意：`vision_draw_lcd_disp_cb` 在 `vision_app.c:321` 被调用，位于显示任务
PPA 缩放之后、`BSP_LVGL_Lock()` blit 段（vision_app.c:333）**之前**，
锁外独立调用——本步的 Lock/Unlock 不会与 blit 段嵌套。

---

### Task 4: 编译

**Files:** 无（验证）

- [ ] **Step 1: 编译**

Run: `./agentic/idf_build.sh`（超时 600000ms）
Expected: 编译通过，无 error。重点确认：
- vision_internal.h include evt.h 后无循环依赖/重定义。
- vision_detect.c 删 include 后 `vision_result_event_data_t` 仍可见
  （经 vision_internal.h → evt.h）。
- vision_draw.c 新 include 的 sdk.h/bsp_lvgl_adapter_init.h 路径正确解析。

若报 `vision_result_event_data_t` 未定义：确认 evt.h 的 include 路径在 vision
组件 CMakeLists 的 REQUIRES/INCLUDE_DIRS 中可达（UI 组件已是 vision 的依赖，
因 detect 原本就 include sdk.h 编译通过，evt.h 同源应可达）。

---

### Task 5: 全擦烧录 + 实物 RTT 验证

**Files:** 无（验证）

- [ ] **Step 1: 烧录并运行**

Run: `python3 agentic/esp_target.py flash-and-run build/ --app-only`
Expected: 烧录成功，固件启动。

- [ ] **Step 2: 启动 RTT 读取器（后台）**

Run（run_in_background）: `python3 agentic/rtt_reader.py --elf build/ESP32P4_Detection.elf --output agentic/.esp-agent/rtt.log --kill-existing`
（elf 名以 build/ 下实际 .elf 为准，必要时先 `ls build/*.elf` 确认。）

- [ ] **Step 3: 读 RTT 验证不死机 + fps 稳定**

等 3 秒，Run: `tail -30 agentic/.esp-agent/rtt.log`
Expected: `[vision_det] frame #N ... fps` 行持续递增输出，SYSMON 行也在刷，
fps 稳定不为 0，无长时间静默（静默=死锁复现）。

- [ ] **Step 4: 实物验证框文同步**

镜头对准目标 → 屏幕出现红框 + 仪表盘文本"识别成功"+公司名+置信度；
移开目标 → 框消失 + 文本回"无目标"，两者同步切换。

- [ ] **Step 5: 长时间运行验证死锁是否消除**

持续运行 ≥2 分钟，反复切页（dashboard/set/log/sys）+ 进出目标，
观察 RTT 是否持续输出、是否复现静默卡死。
Expected: 不复现死机。若仍死机 → 说明根因不止锁竞争，转 spec「风险」节方案
（LVGL 锁各站点加 RTT 探针 + 超时锁，记录持锁者/等待者）。

---

## Self-Review

**Spec coverage:**
- 数据流改造（detect 脱锁 / display 投递）→ Task 2 + Task 3 ✓
- 结构内嵌 ev → Task 1 ✓
- 文本投递时机（画框后、blit 锁段外独立 Lock）→ Task 3 Step 2 ✓
- 测试与验证（编译/烧录/框文同步/长跑）→ Task 4 + Task 5 ✓
- 风险（死锁未根除的后备）→ Task 5 Step 5 ✓

**Placeholder scan:** 无 TBD/TODO；所有代码步骤给出完整代码。

**Type consistency:**
- `vision_det_frame_t.ev`（Task 1）= `vision_result_event_data_t`，
  Task 2 填 `result.ev.*`、Task 3 发 `&to_draw.ev` 一致。
- `to_draw = s_current`（已有），`to_draw.ev` 随结构拷贝带出，类型一致。
- `EVT_VISION`/`EVT_VISION_RESULT_CHANGED`（evt.h:72-75）与 detect 原用法一致。
