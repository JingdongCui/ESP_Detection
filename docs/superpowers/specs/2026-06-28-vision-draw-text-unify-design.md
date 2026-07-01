# 画框+文本统一走显示侧时间戳对齐 — 设计文档

日期：2026-06-28
状态：已批准设计，待写实施计划

## 背景与问题

T6 把 `vision_detect.c` 的假框换成真实推理后，T7 实物验证暴露三个问题：

1. **根本没画框（看不见框）**：detect 链路实际是激活的（`#if 1`），推理、
   `vision_draw_save_result`、文本投递都在跑。但用户看不见框——根因是下方
   问题 3 的死锁让管线在画框前冻结，或框每帧被空帧结果清掉、闪得看不见。
2. **文本被空帧覆盖**：detect 每帧都持锁 `send_event` 投递文本（含空帧的
   "无目标"），检测成功后下一帧 ~22ms 的空推理立即覆盖，用户看不见。
3. **死机嫌疑**：detect(core1) 持 `BSP_LVGL_Lock()` 做 `send_event` 更新 UI
   文本（vision_detect.c:141-144），与 display(core0) 持锁、LVGL worker(core1)
   持锁做 flush 三方抢同一把递归锁。这偏离 esp-who 架构——esp-who 的 detect
   从不碰 LVGL 锁。「假框阶段不死、真实推理一上就死」，唯一新增变量就是 detect
   里跑了推理 + 持锁投文本的耗时/时序，把死锁窗口放大。

## esp-who 参照（已核对源码）

- `WhoDetect::task`（who_detect.cpp:59）：每帧 peek → run → rescale →
  `m_result_cb`，**全程不持 LVGL 锁**。空结果同样调 cb。
- `WhoDetectResultLCDDisp::save_detect_result`（:143）：detect 侧只 push 进
  `m_results` 队列，仅持轻量 `m_res_mutex`。
- `WhoDetectResultLCDDisp::lcd_disp_cb`（:150）：**显示侧**按帧时间戳对齐，
  skip-future-result，选出 `m_result` 后画框。
- esp-who **没有** UI 文本投递（识别成功/置信度/公司名）——这是本项目独有需求，
  无 esp-who 参照，需自行设计。

本项目的 `vision_draw.c` 已把队列 + 时间戳对齐 + 画 hollow rect 复刻到位，
逻辑齐全；缺的是 detect 链路启动，以及文本投递的位置与同步策略。

## 设计决策（用户已确认）

1. **文本停留策略**：和画框完全同步——目标在就显示文本与框，目标走（空帧对齐）
   就一起清空。同一个对齐结果 `s_current` 同时驱动框和文本。
2. **文本投递位置**：移到显示侧。detect 完全脱离 LVGL 锁，只算推理 + save_result；
   display 在时间戳对齐拿到结果后，既画框（无锁，写 s_preview_buf）又持 LVGL 锁
   `send_event` 投文本。全系统只剩 display 一个持锁者，消除跨核锁竞争。

## 数据流

```
detect(core1)：peek帧 → 推理 → rescale → 填结果(含文本字段) → save_result
              不碰 LVGL 锁、不 send_event

display(core0)：peek帧 → PPA缩放 → vision_draw_lcd_disp_cb(对齐取 s_current) →
                ├─ 画框(写 s_preview_buf，无锁)
                └─ 投文本(持 LVGL 锁 send_event，由 s_current 推导)
                → blit 到两块 fb
```

## 结构扩展

UI 文本字段不必新拆——`vision_result_event_data_t`（evt.h:77-86）这个结构体
早已把 status/confidence/fps_x10/infer_time_ms/company/三类概率(a/m) 全定义齐了，
原先被注释的 detect 代码本就在组装它。因此 `vision_det_frame_t`（vision_internal.h）
直接**内嵌整个 `ev`**，使显示侧不必读 detect 的全局态（那是会被新推理覆盖的），
也不必重新推导任何字段：

```c
#include "evt.h"   // vision_result_event_data_t

typedef struct {
    vision_det_result_t items[VISION_MAX_BOXES];  // 画框坐标（预览坐标系）
    int count;
    int64_t timestamp;                            // 对齐用
    vision_result_event_data_t ev;                // 投文本用，detect 组装好整份
} vision_det_frame_t;
```

- `items[]`/`count`：显示侧画框用。
- `ev`：detect 一次性填好 status/confidence/company/三类概率/fps/耗时，
  显示侧对齐后直接 `send_event(... &s_current.ev ...)`，零推导。
- 类别名映射表 `kClassName[3]` 留在 detect（本就在那），不挪到显示侧。

## 各文件改动

### vision_internal.h
- `#include "evt.h"`；`vision_det_frame_t` 内嵌 `vision_result_event_data_t ev`。

### vision_detect.c
- 推理链路已激活（`#if 1`），无需取消注释；去掉 `#if 1/#endif` 调试包裹。
- UI 文本组装**保留在 detect**：现成的 `ev`（status/confidence/company/
  三类概率/fps_x10/infer_time_ms）填好后写进 `result.ev`，再 save_result。
  类别名映射表 `kClassName[3]` 留在 detect 不动。
- 删除 detect 里的投递动作整块（vision_detect.c:141-144 的
  `BSP_LVGL_Lock`/`send_event`/`BSP_LVGL_Unlock`）——只组装数据进队列，
  不再持锁 send_event。
- 随之删除不再需要的 include：`sdk.h`、`bsp_lvgl_adapter_init.h`
  （send_event/BSP_LVGL_Lock 改由 vision_draw.c 用）。
- fps 窗口平均逻辑保留在 detect，算出的 `fps10` 填进 `result.ev.fps_x10`。
- 保留 RTT 打印（SEGGER_RTT_printf）。

### vision_draw.c
- `vision_draw_lcd_disp_cb` 在画框后，新增：持 LVGL 锁
  `send_event(EVT_VISION, EVT_VISION_RESULT_CHANGED, &s_current.ev, 0)`，
  **直接发 s_current.ev，零推导**（detect 已填好，空帧时 detect 填的就是
  "无目标"那份）。
- 需 include sdk.h（send_event/get_current_event_table/EVT_VISION）、
  bsp_lvgl_adapter_init.h（BSP_LVGL_Lock/Unlock）。

## 锁与时序

- detect：只持 `s_mutex`（结果队列锁，轻量），不碰 LVGL 锁。
- display：`vision_draw_lcd_disp_cb` 内画框无锁；投文本持 LVGL 锁 send_event
  （send_event 是同步回调派发，非阻塞，见 evt.c）。注意此调用点在显示任务
  现有 `BSP_LVGL_Lock()` blit 段**之外**，避免锁嵌套与持锁时长叠加——
  在 PPA 缩放后、blit 段前独立完成。
- 结果：全系统持 LVGL 锁的只剩 display 与 LVGL worker，回到 esp-who 式
  单持锁者模型。

## 测试与验证

1. 编译通过（`./agentic/idf_build.sh`）。
2. 全擦烧录，RTT 观察 `[vision_det]` 行持续输出、fps 稳定、不死机。
3. 实物：镜头对准目标 → 看到红框 + 仪表盘文本「识别成功」；移开目标 →
   框消失 + 文本回「无目标」，两者同步。
4. 长时间运行观察是否复现死机（验证锁竞争是否消除）。

## 风险

- 死机根因若不止锁竞争（如 vsync ISR 丢失致 worker flush 永久阻塞），本改动
  减少持锁者但不一定根除。若改动后仍死机，需在 LVGL 锁各站点加 RTT 探针 +
  超时锁，下次死机时记录持锁者/等待者。
- 显示侧每帧持 LVGL 锁 send_event：send_event 为非阻塞同步派发，
  `ui_vision_result_event_cb` 仅 label set，已确认不阻塞，风险低。
