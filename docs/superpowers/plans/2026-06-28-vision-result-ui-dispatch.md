# 识别结果投递到 UI 刷新 Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** 检测任务每帧把识别状态/置信度/帧率/推理耗时投递到 dashboard 仪表盘页的文本控件并实时刷新。

**Architecture:** 照搬现有 `EVT_SYSTEM_MONITOR` 事件通道，新增 `EVT_VISION`。生产者（`vision_detect_task`，core1）每帧组装 `vision_result_event_data_t`，持 `BSP_LVGL_Lock` 调 `send_event` 同步派发到 `ui.c` 的回调，回调里 `lv_label_set_text*` 写 `cont_dashboard` 控件。无队列、无异步、无新线程。

**Tech Stack:** ESP-IDF, LVGL 9.x, anyui SDK 事件机制 (`register_event`/`send_event`/`event_table_t`), FreeRTOS, SEGGER RTT 日志。

**注意：** 本工作区非 git 仓库，无 host 单元测试框架。每个任务的"验证"= 编译通过 + 烧录 + RTT 观察 + 屏幕目视，而非单元测试。各任务故意做成可独立编译通过的小步，最后一个任务统一上板验证。

---

### Task 1: 事件定义 — evt.h 新增 EVT_VISION

**Files:**
- Modify: `components/UI/sdk/evt.h:48-70`（在 `EVT_SYSTEM_MONITOR` 块之后插入）

- [ ] **Step 1: 在 evt.h 的 `system_monitor_event_data_t` 结构体闭合 `}` 之后、`event_cb_func_t` typedef 之前，插入 EVT_VISION 定义**

在第 70 行（`} system_monitor_event_data_t;`）之后插入：

```c

#define EVT_VISION 7
enum vision_evt_enum {
  EVT_VISION_RESULT_CHANGED = 1,
};

typedef struct {
  char status[24];      // "识别成功" / "无目标" / "等待中"
  int  confidence;      // 最高分框置信度 %(0~100)；无框=0
  int  fps_x10;         // 帧率 ×10（显示拆成 NN.N）
  int  infer_time_ms;   // 推理耗时 ms（假框阶段=0）
  char company[24];     // 类别名/快递公司（占位）
  int  jt_a, jt_m;      // 极兔 A/M 占比 ×100（占位全 0）
  int  zt_a, zt_m;      // 中通 A/M
  int  yd_a, yd_m;      // 韵达 A/M
} vision_result_event_data_t;
```

- [ ] **Step 2: 编译验证（仅头改动，全量不必，增量即可）**

Run: `./agentic/idf_build.sh`（超时设 600000）
Expected: 编译通过（此时无人用新结构体，只校验语法）。若报错，检查结构体语法与分号。

---

### Task 2: UI 业务层 — ui.c 新增 vision 回调与注册函数

**Files:**
- Modify: `components/UI/sdk/ui.c:173`（在 `extern` 控件声明块末尾追加）
- Modify: `components/UI/sdk/ui.c:268`（在 `ui_register_system_monitor_events` 之后追加回调+注册）

- [ ] **Step 1: 在 `extern lv_obj_t *scr_dashboard_label_runtime_bright__data;`（第 173 行）之后追加 11 个 vision 控件的 extern 声明**

```c
extern lv_obj_t *scr_dashboard_label_runtime_recognition_status;
extern lv_obj_t *scr_dashboard_label_runtime_confidence;
extern lv_obj_t *scr_dashboard_label_runtime_recognition_fps;
extern lv_obj_t *scr_dashboard_label_runtime_infer_time;
extern lv_obj_t *scr_dashboard_label_runtime_express_company;
extern lv_obj_t *scr_dashboard_label_JT_A;
extern lv_obj_t *scr_dashboard_label_JT_M;
extern lv_obj_t *scr_dashboard_label_ZT_A;
extern lv_obj_t *scr_dashboard_label_ZT_M;
extern lv_obj_t *scr_dashboard_label_YD_A;
extern lv_obj_t *scr_dashboard_label_YD_M;
```

- [ ] **Step 2: 在 `ui_register_system_monitor_events` 函数闭合 `}`（第 268 行）之后插入 vision 回调与注册函数**

回调签名必须与 `event_cb_func_t` 一致（6 参数），并校验 `len`：

```c

static void ui_vision_result_event_cb(uint8_t event, uint16_t code, uint16_t type,
                                       uint16_t len, uint8_t *data, uint8_t status)
{
    LV_UNUSED(event);
    LV_UNUSED(code);
    LV_UNUSED(type);
    LV_UNUSED(status);

    if (!data || len != sizeof(vision_result_event_data_t)) {
        return;
    }

    vision_result_event_data_t *v = (vision_result_event_data_t *)data;

    lv_label_set_text(scr_dashboard_label_runtime_recognition_status, v->status);
    lv_label_set_text_fmt(scr_dashboard_label_runtime_confidence, "%d%%", v->confidence);
    lv_label_set_text_fmt(scr_dashboard_label_runtime_recognition_fps, "%d.%d",
                          v->fps_x10 / 10, v->fps_x10 % 10);
    lv_label_set_text_fmt(scr_dashboard_label_runtime_infer_time, "%dms", v->infer_time_ms);
    lv_label_set_text(scr_dashboard_label_runtime_express_company, v->company);
    lv_label_set_text_fmt(scr_dashboard_label_JT_A, "A:%d.%02d%%", v->jt_a / 100, v->jt_a % 100);
    lv_label_set_text_fmt(scr_dashboard_label_JT_M, "M:%d.%02d%%", v->jt_m / 100, v->jt_m % 100);
    lv_label_set_text_fmt(scr_dashboard_label_ZT_A, "A:%d.%02d%%", v->zt_a / 100, v->zt_a % 100);
    lv_label_set_text_fmt(scr_dashboard_label_ZT_M, "M:%d.%02d%%", v->zt_m / 100, v->zt_m % 100);
    lv_label_set_text_fmt(scr_dashboard_label_YD_A, "A:%d.%02d%%", v->yd_a / 100, v->yd_a % 100);
    lv_label_set_text_fmt(scr_dashboard_label_YD_M, "M:%d.%02d%%", v->yd_m / 100, v->yd_m % 100);
}

uint32_t ui_register_vision_events(event_table_t *table)
{
    return register_event(table, EVT_VISION, EVT_VISION_RESULT_CHANGED, 0,
                          sizeof(vision_result_event_data_t), ui_vision_result_event_cb);
}
```

注意：**不要**触碰 `scr_dashboard_label_runtime_confidence_threshole`（用户阈值滑块配置，非检测输出）。

- [ ] **Step 3: 编译验证**

Run: `./agentic/idf_build.sh`
Expected: 编译通过。常见错误：控件名拼写（对照 `setup_scr_dashboard.c` 中的 `NULL` 声明行）；`ui_register_vision_events` 尚未在头声明会产生隐式声明警告——下一任务修复。

---

### Task 3: UI 头 — ui.h 声明 ui_register_vision_events

**Files:**
- Modify: `components/UI/sdk/ui.h:72`（在 `ui_register_system_monitor_events` 声明之后）

- [ ] **Step 1: 在 `uint32_t ui_register_system_monitor_events(event_table_t *table);`（第 72 行）之后追加声明**

```c
uint32_t ui_register_vision_events(event_table_t *table);
```

- [ ] **Step 2: 编译验证**

Run: `./agentic/idf_build.sh`
Expected: 编译通过，Task 2 的隐式声明警告消失。

---

### Task 4: 初始化 — system_init.c 注册 vision 事件回调

**Files:**
- Modify: `main/system_init.c:39`（在 `ui_register_system_monitor_events(...)` 之后、同一 LVGL 锁内）

- [ ] **Step 1: 在第 39 行 `ui_register_system_monitor_events(get_current_event_table());` 之后追加一行**

```c
    ui_register_vision_events(get_current_event_table());
```

必须在 `BSP_LVGL_Unlock();`（第 56 行）之前、`vision_start();`（第 59 行）之前——确保检测任务启动前回调已注册。`ui.h` 已在第 10 行 include，无需新增头。

- [ ] **Step 2: 编译验证**

Run: `./agentic/idf_build.sh`
Expected: 编译通过。

---

### Task 5: 检测任务 — vision_detect.c 每帧投递结果

**Files:**
- Modify: `components/vision/vision_detect.c:22-27`（include 区）
- Modify: `components/vision/vision_detect.c:122-129`（save 之后、RTT 打印之前）

- [ ] **Step 1: 在 include 区追加两个头**

在第 27 行 `#include "vision_internal.h"` 之后追加：

```c
#include "sdk.h"                       // send_event / get_current_event_table / EVT_VISION / vision_result_event_data_t
#include "bsp_lvgl_adapter_init.h"     // BSP_LVGL_Lock / BSP_LVGL_Unlock
```

（vision 组件 CMakeLists 已 REQUIRES `UI` 与 `bsp`，无需改依赖。）

- [ ] **Step 2: 在 `vision_draw_save_result(&result);`（第 122 行）之后、`SEGGER_RTT_printf`（第 125 行）之前，组装并投递事件**

```c
            // ===== 投递结果到 UI（dashboard 仪表盘页文本控件）=====
            vision_result_event_data_t ev = {0};
            if (result.count > 0) {
                strcpy(ev.status, "识别成功");
                int best = 0;
                for (int i = 1; i < result.count; i++) {
                    if (result.items[i].score > result.items[best].score) {
                        best = i;
                    }
                }
                ev.confidence = (int)(result.items[best].score * 100.0f);
                strcpy(ev.company, "—");   // 占位，模型接入后填类别名
            } else {
                strcpy(ev.status, "无目标");
                ev.confidence = 0;
            }
            ev.fps_x10 = fps10;
            ev.infer_time_ms = 0;          // 占位，模型接入点用 esp_timer 测真值
            // jt_*/zt_*/yd_* 占位保持 0（{0} 已清零）

            BSP_LVGL_Lock();
            send_event(get_current_event_table(), EVT_VISION, EVT_VISION_RESULT_CHANGED,
                       (uint8_t *)&ev, 0);
            BSP_LVGL_Unlock();
```

注意缩进：此代码块在 `if (pw > 0 && ph > 0 && ...)` 块内（第 102-123 行的 `vision_draw_save_result` 同级）。假框阶段 `result.count` 恒为 1，故实际只会走"识别成功"分支——"无目标"分支为模型接入后真实零框时生效，现在先写好。

- [ ] **Step 3: 编译验证**

Run: `./agentic/idf_build.sh`
Expected: 编译通过。常见错误：`strcpy` 需 `<string.h>`——vision_detect.c 当前未 include，若报隐式声明，在 include 区追加 `#include <string.h>`。

---

### Task 6: 上板集成验证

**Files:** 无改动，纯验证。

- [ ] **Step 1: 全量编译**

Run: `./agentic/idf_build.sh`（超时 600000）
Expected: 编译通过，生成 `build/sample_project.elf`。

- [ ] **Step 2: 烧录并运行**

Run: `python3 agentic/esp_target.py flash-and-run build/ --app-only`
Expected: 烧录成功，固件启动。

- [ ] **Step 3: 启动 RTT 读取器（后台）**

Run（后台）: `python3 agentic/rtt_reader.py --elf build/sample_project.elf --output agentic/.esp-agent/rtt.log --kill-existing`
等待 3 秒。

- [ ] **Step 4: 读 RTT 确认检测任务在跑、无崩溃**

Run: `tail -30 agentic/.esp-agent/rtt.log`
Expected: 持续出现 `[vision_det] frame #N ... fps` 行；无 panic/backtrace/`Guru Meditation`。若崩溃，按 CLAUDE.md "调试崩溃或挂起" 流程 halt + cpu-regs + GDB bt 定位（重点查 send_event 空指针或控件 NULL）。

- [ ] **Step 5: 目视屏幕 dashboard 仪表盘页**

Expected:
- 识别状态显示"识别成功"（假框恒有框）。
- 置信度显示"100%"（假框 score=1.0）。
- 帧率显示随实际刷新跳动的 "NN.N"。
- 推理耗时显示 "0ms"（占位）。
- 快递公司显示 "—"，各公司 A/M 显示 "A:0.00%"/"M:0.00%"（占位）。

若文本不刷新：确认 Task 4 注册行在 `vision_start` 之前；确认控件名与 generated 一致；RTT 加临时打印确认回调被调用。

---

## 模型接入后的收尾（本计划范围外，记录备查）

`vision_detect.c` 第 66-97 行"模型接入点"替换为真实推理时：
- 在 model->run 前后加 `esp_timer_get_time()` 两点，差值填 `ev.infer_time_ms`。
- 真实类别名填 `ev.company`，真实占比 ×100 填 `ev.jt_a` 等。
- **evt.h 结构体与 ui.c 回调无需改动**——这是"全部字段含占位都接"决策的收益。
