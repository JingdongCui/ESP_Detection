# 识别结果投递到 UI 刷新 — 设计

日期：2026-06-28
状态：待评审

## 1. 背景与问题

检测任务 (`vision_detect_task`，core1) 每帧产出检测结果并调用
`vision_draw_save_result()` 存入结果队列，供显示侧在预览图上画框（已复刻 ESP-who）。

但 dashboard 屏幕"仪表盘"标签页下的 `cont_dashboard` 容器里有一组**文本控件**
（识别状态、置信度、帧率、推理耗时、快递公司、各公司占比），目前全是写死的占位值
（"等待中"/"--"/"A:0.00%"），**从不更新**。

ESP-who 的视觉链路只在裸屏上画框，没有任何文本控件——所以"结构化结果 → LVGL 文本控件"
这一层是**本项目特有的 UI 适配层**，不属于 ESP-who 视觉链路复刻范围，可自由设计。

目标：检测任务每产出一帧结果，就把状态/置信度/帧率/耗时等投递到 `cont_dashboard` 的
文本控件并刷新显示。

## 2. 设计决策（已与用户确认）

| 项 | 决策 | 理由 |
| --- | --- | --- |
| 复用机制 | 照搬现有 `EVT_SYSTEM_MONITOR` 事件通道，新增 `EVT_VISION` | system_monitor 已验证的 BSP→UI 投递模式，零新机制 |
| 接哪些字段 | 全部接，含暂无真实数据的占位字段 | 一次接齐，模型接入后只填值，UI/事件结构不再动 |
| 投递频率 | 每帧都发，无节流 | 真实模型推理 100+ms，检测循环天然 <10Hz，不会饿死 LVGL worker |
| 无框时 | 显示"无目标"，仍刷新其余字段 | 区分"识别成功"与"画面无目标"，帧率/耗时持续可见 |
| 对接范围 | 仅 `cont_dashboard` 容器 | 日志追踪界面（result_01~10 十行列表）本期不接 |

## 3. 架构与数据流

```
vision_detect_task (core1)  每帧
  │ ① 推理产出 vision_det_frame_t（已有）
  │ ② 组装 vision_result_event_data_t
  │     - count>0 → status="识别成功"，confidence=最高分框×100
  │     - count==0 → status="无目标"，confidence=0
  │     - fps_x10 已算好；infer_time_ms 由模型接入点的 esp_timer 测得
  │     - company / jt_*/zt_*/yd_* 占位（假框阶段填默认）
  │ ③ BSP_LVGL_Lock()
  │ ④ send_event(get_current_event_table(),
  │              EVT_VISION, EVT_VISION_RESULT_CHANGED, &data, 0)
  │ ⑤ BSP_LVGL_Unlock()
  ▼
sdk/evt.c  send_event() 同步遍历事件表，匹配 EVT_VISION + code
  ▼
sdk/ui.c  ui_vision_result_event_cb(data)   （持锁中同步执行）
  │  按字段 lv_label_set_text 到 cont_dashboard 各控件
  ▼
LVGL 下一帧刷新文本
```

与 system_monitor **完全同构**：生产者持 `BSP_LVGL_Lock` → `send_event` 同步派发 →
回调里写控件 → 解锁。无队列、无异步、无新线程。

## 4. 组件改动（5 文件，0 改动生成代码）

### 4.1 事件定义 `components/UI/sdk/evt.h`

新增事件号、code 枚举、数据结构：

```c
#define EVT_VISION 7

typedef enum {
    EVT_VISION_RESULT_CHANGED = 1,
} vision_evt_enum;

typedef struct {
    char status[24];      // "识别成功" / "无目标" / "等待中"
    int  confidence;      // 最高分框置信度 %(0~100)；无框=0
    int  fps_x10;         // 帧率 ×10（显示时拆成 NN.N）
    int  infer_time_ms;   // 推理耗时 ms
    char company[24];     // 类别名/快递公司（占位）
    int  jt_a, jt_m;      // 极兔 A/M 占比 ×100（占位全 0）
    int  zt_a, zt_m;      // 中通 A/M
    int  yd_a, yd_m;      // 韵达 A/M
} vision_result_event_data_t;
```

### 4.2 UI 业务层 `components/UI/sdk/ui.c`

- 在已有 `extern lv_obj_t *...` 块补声明 13 个控件（generated 全局，非 static）：
  `scr_dashboard_label_runtime_recognition_status` / `_runtime_confidence` /
  `_runtime_recognition_fps` / `_runtime_infer_time` / `_runtime_express_company` /
  `_JT_A` / `_JT_M` / `_ZT_A` / `_ZT_M` / `_YD_A` / `_YD_M`。
- 仿 `ui_system_monitor_event_cb` 新增 `ui_vision_result_event_cb`：
  ```c
  static void ui_vision_result_event_cb(event_table_t *t, void *d) {
      const vision_result_event_data_t *v = d;
      lv_label_set_text(scr_dashboard_label_runtime_recognition_status, v->status);
      lv_label_set_text_fmt(scr_dashboard_label_runtime_confidence, "%d%%", v->confidence);
      lv_label_set_text_fmt(scr_dashboard_label_runtime_recognition_fps, "%d.%d",
                            v->fps_x10/10, v->fps_x10%10);
      lv_label_set_text_fmt(scr_dashboard_label_runtime_infer_time, "%dms", v->infer_time_ms);
      lv_label_set_text(scr_dashboard_label_runtime_express_company, v->company);
      lv_label_set_text_fmt(scr_dashboard_label_JT_A, "A:%d.%02d%%", v->jt_a/100, v->jt_a%100);
      lv_label_set_text_fmt(scr_dashboard_label_JT_M, "M:%d.%02d%%", v->jt_m/100, v->jt_m%100);
      // ZT / YD 同理
  }
  ```
  - **不碰** `scr_dashboard_label_runtime_confidence_threshole`（用户阈值滑块配置，非检测输出）。
- 仿 `ui_register_system_monitor_events` 新增：
  ```c
  uint32_t ui_register_vision_events(event_table_t *table) {
      return register_event(table, EVT_VISION, EVT_VISION_RESULT_CHANGED, 0,
                            sizeof(vision_result_event_data_t), ui_vision_result_event_cb);
  }
  ```

### 4.3 UI 头 `components/UI/sdk/ui.h`

```c
uint32_t ui_register_vision_events(event_table_t *table);
```

### 4.4 初始化 `main/system_init.c`

在 `ui_register_system_monitor_events(...)` 之后、同一 LVGL 锁内补一行：

```c
ui_register_vision_events(get_current_event_table());
```

（`vision_start()` 在其后，确保事件表先注册好回调再启检测任务。）

### 4.5 检测任务 `components/vision/vision_detect.c`

每帧 `vision_draw_save_result()` 之后，组装并投递：

```c
vision_result_event_data_t ev = {0};
if (result.count > 0) {
    strcpy(ev.status, "识别成功");
    int best = 0;
    for (int i = 1; i < result.count; i++)
        if (result.items[i].score > result.items[best].score) best = i;
    ev.confidence = (int)(result.items[best].score * 100);
    strcpy(ev.company, "—");   // 占位，模型接入后填类别名
} else {
    strcpy(ev.status, "无目标");
}
ev.fps_x10 = fps10;               // 已有
ev.infer_time_ms = infer_ms;      // 模型接入点 esp_timer 测得；假框阶段 0
// jt_*/zt_*/yd_* 占位保持 0

BSP_LVGL_Lock();
send_event(get_current_event_table(), EVT_VISION, EVT_VISION_RESULT_CHANGED, (uint8_t*)&ev, 0);
BSP_LVGL_Unlock();
```

模型接入点（vision_detect.c 现有 "模型接入点" 标记处）补 `esp_timer_get_time()`
前后两点算 `infer_ms`，并把真实类别名/占比填进 `company`/`jt_*` 等——
事件结构与 UI 回调无需再改。

## 5. 占位策略

假框阶段无真实快递公司/占比数据，相关字段填默认（company="—"，占比 0，显示
"A:0.00%"）。模型接入后只在 4.5 的组装处填真值，**evt.h 结构与 ui.c 回调保持不变**——
这是"全部字段都接含占位"决策的收益。

## 6. 风险与边界

- **每帧发的锁竞争**：依赖"真实推理 100+ms"前提。若未来模型极快（<30ms）导致 >30Hz，
  需回到节流方案；本期按用户判断不加节流。
- **线程安全**：投递全程持 `BSP_LVGL_Lock`，与 system_monitor 一致，跨核安全。
- **不改 generated**：所有控件经 `extern` 引用，不触碰 `components/UI/generated/`。
- **范围边界**：日志追踪 10 行列表 (`result_01~10`/`status_01~10`) 本期不接。

## 7. 验证

1. 编译通过（`./agentic/idf_build.sh`）。
2. 烧录运行，RTT 确认检测任务在跑、send_event 无报错。
3. 屏幕 dashboard 仪表盘页：有假框时显示"识别成功"+ 置信度/帧率/耗时实时刷新；
   构造无框时显示"无目标"。
