# UI SDK 数据接入框架

本文说明当前 `components/UI/sdk` 已完成的系统监视数据接入方式，以及后续其他页面数据接入的推荐流程。

## 1. 目录职责边界

### `components/UI/generated/`

AnyUI 自动生成的 UI 代码，包括屏幕创建、控件变量、图片和字体资源。

约束：

- 不手动修改。
- 重新从 AnyUI 生成后，这里的改动会丢失。
- 业务逻辑不要写在这里。
- 如果需要操作控件，优先通过 generated 暴露的非 `static` 控件指针访问。

### `components/UI/sdk/`

AnyUI 提供的 SDK 扩展层，适合放：

- 屏幕栈管理。
- UI 事件表注册和派发。
- UI 控件绑定辅助函数。
- 硬件/业务数据到 UI 控件的轻量适配逻辑。

当前约束：

- 现在 SDK 只管理系统监视界面的 UI 数据刷新。
- 不把视觉预览、检测页、日志页、设置页等其他页面的具体业务实现塞进 SDK。
- 后续如果接入其他页面，SDK 只放事件定义、注册函数和小型 UI 绑定函数；复杂业务仍放在对应业务组件中。

### 业务组件

例如：

- `components/system_monitor/`：负责采集 CPU、内存、运行时间等系统数据。
- `components/vision/`：负责摄像头、PPA、视觉预览和检测链路。
- 其他未来组件：负责各自业务数据采集和处理。

业务组件通过 SDK 的事件接口把数据送到 UI，不直接维护全局 UI 任务队列。

## 2. 当前 SDK 已完成的系统监视工作

### 2.1 新增系统监视事件

在 `components/UI/sdk/evt.h` 中新增：

```c
#define EVT_SYSTEM_MONITOR 6

enum system_monitor_evt_enum {
  EVT_SYSTEM_MONITOR_CHANGED = 1,
};
```

含义：

- `EVT_SYSTEM_MONITOR`：系统监视模块事件大类。
- `EVT_SYSTEM_MONITOR_CHANGED`：系统监视数据刷新事件。

### 2.2 新增系统监视事件数据结构

在 `components/UI/sdk/evt.h` 中新增：

```c
typedef struct {
  int cpu1_usage;
  int cpu2_usage;
  int cpu_total_usage;
  int memory_usage;
  int heap_free_kb;
  int heap_min_free_kb;
  int heap_max_block_kb;
  int psram_usage;
  int ram_usage;
  int runtime_sec;
  int chip_temp;
  int cpu_freq_mhz;
  int task_count;
} system_monitor_event_data_t;
```

这个结构是 UI SDK 的数据入参，不是采集层原始结构。

原则：

- 采集层可以有自己的原始结构。
- 进入 UI 前转换成 SDK 事件结构。
- UI 事件结构只保留 UI 需要显示的数据。

### 2.3 新增圆环百分比绑定辅助函数

在 `components/UI/sdk/ui.c` 中新增：

```c
void ui_arc_percent_set_by_label(lv_obj_t *label, int value);
```

作用：

1. 接收一个可见百分比 `label`。
2. 找到这个 label 的父容器。
3. 在父容器子对象中查找 `lv_arc`。
4. 同时更新圆环值和文本：

```c
lv_arc_set_value(arc, value);
lv_label_set_text_fmt(label, "%d%%", value);
```

为什么这样做：

- AnyUI 生成的 6 个圆环对象是 `static`，外部不能直接访问。
- 可见百分比 label 是全局指针，外部可以访问。
- 圆环和可见 label 位于同一个父容器下，可以通过父容器反查圆环。
- 这样避免修改 `generated/`。

### 2.4 新增系统监视事件回调

在 `components/UI/sdk/ui.c` 中新增内部回调：

```c
static void ui_system_monitor_event_cb(uint8_t event, uint16_t code, uint16_t type,
                                       uint16_t len, uint8_t *data, uint8_t status);
```

回调收到 `system_monitor_event_data_t` 后，更新 dashboard 上 6 个圆环：

- runtime CPU core1。
- runtime CPU core2。
- runtime memory。
- system CPU 02。
- system CPU 03。
- system CPU total。

更新方式统一调用：

```c
ui_arc_percent_set_by_label(label, value);
```

### 2.5 新增系统监视事件注册函数

在 `components/UI/sdk/ui.c` 中新增：

```c
uint32_t ui_register_system_monitor_events(event_table_t *table)
{
    return register_event(table, EVT_SYSTEM_MONITOR, EVT_SYSTEM_MONITOR_CHANGED, 0,
                          sizeof(system_monitor_event_data_t), ui_system_monitor_event_cb);
}
```

作用：

- 把 `EVT_SYSTEM_MONITOR / EVT_SYSTEM_MONITOR_CHANGED` 绑定到 `ui_system_monitor_event_cb()`。
- 外部只需要在 UI 创建完成后调用一次注册函数。

### 2.6 UI 初始化后注册事件

在主初始化流程中，正确顺序是：

```c
BSP_LVGL_Lock();
setupUi();
ui_register_system_monitor_events(get_current_event_table());
BSP_LVGL_Unlock();
```

原因：

- `setupUi()` 会创建 dashboard。
- dashboard 创建过程中会设置当前屏幕事件表。
- 控件创建完成后，才能注册依赖这些控件的刷新回调。

### 2.7 system_monitor 发布事件

`system_monitor` 采集到数据后，把原始采集结构转换成 `system_monitor_event_data_t`，再调用：

```c
BSP_LVGL_Lock();
send_event(get_current_event_table(), EVT_SYSTEM_MONITOR, EVT_SYSTEM_MONITOR_CHANGED,
           (uint8_t *)&data, 0);
BSP_LVGL_Unlock();
```

注意：

- LVGL 控件刷新必须在 LVGL 锁内执行。
- `send_event()` 会同步调用已注册的 UI 回调。
- 回调中会直接调用 LVGL API 更新控件。

## 3. 事件流总览

系统监视当前数据流：

```text
system_monitor 采集数据
        │
        ▼
转换成 system_monitor_event_data_t
        │
        ▼
BSP_LVGL_Lock()
        │
        ▼
send_event(current_event_table,
           EVT_SYSTEM_MONITOR,
           EVT_SYSTEM_MONITOR_CHANGED,
           data)
        │
        ▼
ui_system_monitor_event_cb()
        │
        ▼
ui_arc_percent_set_by_label()
        │
        ▼
lv_arc_set_value() + lv_label_set_text_fmt()
        │
        ▼
BSP_LVGL_Unlock()
        │
        ▼
LVGL 后续刷新任务重绘屏幕
```

## 4. LVGL 刷新何时发生

SDK 回调不会直接驱动 LCD 刷屏。

实际过程是：

1. SDK 回调调用 LVGL API，例如：
   - `lv_arc_set_value()`
   - `lv_label_set_text_fmt()`
   - `lv_obj_add_flag()`
   - `lv_obj_remove_flag()`
2. LVGL 标记对象区域为 dirty。
3. BSP 的 LVGL adapter 任务周期性运行 LVGL 刷新逻辑。
4. LCD 刷新 dirty 区域。

所以数据接入只需要正确更新 LVGL 对象，不需要手动调用屏幕刷新。

## 5. 后续其他页面的数据接入流程

每接入一个新页面或新数据模块，按下面流程做。

### 步骤 1：确认 UI 控件来源

先在 `components/UI/generated/` 中确认：

- 需要更新的控件变量名。
- 控件是否是全局变量。
- 控件是否是 `static`。
- 控件和可访问对象之间是否有稳定父子关系。

规则：

- 如果目标控件是全局变量，可以在 SDK 中 `extern` 后直接更新。
- 如果目标控件是 `static`，不要改 generated；改用可访问的父容器、label 或其他全局对象间接定位。
- 如果没有可访问路径，需要回到 AnyUI 工程里调整控件命名/导出方式，然后重新生成。

### 步骤 2：在 `evt.h` 定义事件大类和事件码

示例：

```c
#define EVT_VISION 7

enum vision_evt_enum {
  EVT_VISION_RESULT_CHANGED = 1,
  EVT_VISION_STATUS_CHANGED = 2,
};
```

原则：

- 一个业务域一个 `EVT_xxx`。
- 一个数据变化类型一个 `EVT_xxx_CHANGED`。
- 不同页面不要共用含义不清的事件码。

### 步骤 3：定义 UI 事件数据结构

示例：

```c
typedef struct {
  int infer_time_ms;
  int confidence_percent;
  int fps;
  int sorted_count;
  char company[32];
  char status[32];
} vision_result_event_data_t;
```

原则：

- 结构字段按 UI 显示需求设计。
- 不直接暴露底层驱动结构。
- 字符串使用固定长度数组，避免跨任务传递悬空指针。
- 百分比、时间、计数等尽量在业务组件中转换成 UI 直接可用的单位。

### 步骤 4：在 `ui.c` 写对应 UI 回调

示例：

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

    vision_result_event_data_t *result = (vision_result_event_data_t *)data;

    lv_label_set_text_fmt(scr_dashboard_label_runtime_infer_time, "%d", result->infer_time_ms);
    lv_label_set_text_fmt(scr_dashboard_label_runtime_confidence, "%d%%", result->confidence_percent);
    lv_label_set_text_fmt(scr_dashboard_label_runtime_recognition_fps, "%d", result->fps);
    lv_label_set_text_fmt(scr_dashboard_label_runtime_sorted_count, "%d", result->sorted_count);
    lv_label_set_text(scr_dashboard_label_runtime_express_company, result->company);
    lv_label_set_text(scr_dashboard_label_runtime_recognition_status, result->status);
}
```

原则：

- 回调只做 UI 更新。
- 不在回调里采集数据。
- 不在回调里做耗时计算。
- 不在回调里访问业务驱动。

### 步骤 5：在 `ui.c/h` 提供注册函数

示例：

```c
uint32_t ui_register_vision_events(event_table_t *table)
{
    uint32_t ret = 0;
    ret |= register_event(table, EVT_VISION, EVT_VISION_RESULT_CHANGED, 0,
                          sizeof(vision_result_event_data_t), ui_vision_result_event_cb);
    return ret;
}
```

在 `ui.h` 中声明：

```c
uint32_t ui_register_vision_events(event_table_t *table);
```

原则：

- 每个业务域一个注册函数。
- 注册函数集中管理该业务域的所有事件回调。
- 初始化流程只调用注册函数，不直接调用 `register_event()` 细节。

### 步骤 6：UI 初始化后注册事件

示例：

```c
BSP_LVGL_Lock();
setupUi();
ui_register_system_monitor_events(get_current_event_table());
ui_register_vision_events(get_current_event_table());
BSP_LVGL_Unlock();
```

原则：

- 必须在 `setupUi()` 后注册。
- 必须在 LVGL 锁内注册。
- 注册时使用当前屏幕事件表。

### 步骤 7：业务组件发布事件

示例：

```c
vision_result_event_data_t data = {
    .infer_time_ms = infer_time_ms,
    .confidence_percent = confidence_percent,
    .fps = fps,
    .sorted_count = sorted_count,
};

snprintf(data.company, sizeof(data.company), "%s", company);
snprintf(data.status, sizeof(data.status), "%s", status);

BSP_LVGL_Lock();
send_event(get_current_event_table(), EVT_VISION, EVT_VISION_RESULT_CHANGED,
           (uint8_t *)&data, 0);
BSP_LVGL_Unlock();
```

原则：

- 业务组件只负责准备数据和发送事件。
- LVGL 对象只在 SDK 回调里更新。
- 发送事件时加 LVGL 锁，因为当前事件回调是同步执行的。

## 6. 推荐分层

推荐保持三层：

```text
采集/业务层
  - system_monitor
  - vision
  - Ethernet_app
  - future modules
        │
        │  send_event()
        ▼
UI SDK 适配层
  - evt.h / evt.c：事件定义、注册、派发
  - ui.h / ui.c：UI 绑定辅助函数、事件回调、注册函数
        │
        │  LVGL API
        ▼
AnyUI generated UI
  - setup_scr_dashboard.c
  - setup_ui.c
  - 控件对象和资源
```

## 7. 哪些逻辑不应该放进 SDK

不要放进 `components/UI/sdk`：

- 摄像头取帧。
- PPA 缩放。
- 模型推理。
- 网络协议处理。
- 文件系统业务逻辑。
- 日志存储。
- 大块图像缓冲管理。
- 长时间阻塞操作。

这些应留在对应业务组件里。

SDK 只负责：

- 接收已经整理好的 UI 数据。
- 把数据写到 LVGL 控件。
- 提供少量通用控件绑定工具。

## 8. 接入新页面时的检查清单

1. 是否确认 generated 里的目标控件变量名？
2. 是否避免手改 `generated/`？
3. 是否在 `evt.h` 中定义了清晰的事件大类和事件码？
4. 是否定义了 UI 专用数据结构？
5. 是否在 `ui.c` 中写了只做 UI 更新的回调？
6. 是否在 `ui.h` 中声明注册函数？
7. 是否在 `setupUi()` 后注册事件？
8. 业务组件发送事件时是否持有 LVGL 锁？
9. 回调里是否避免耗时操作？
10. 是否通过编译验证？

## 9. 当前后续工作建议

当前系统监视界面已经适合走 SDK 事件路径。

后续建议顺序：

1. 先把 `system_monitor -> UI SDK -> dashboard 圆环` 编译验证通过。
2. 清理删除 `app_ui` 后残留的旧引用。
3. 对其他页面逐个接入事件，不一次性重构全部页面。
4. 每接一个页面都按“事件定义 → UI 数据结构 → SDK 回调 → 注册函数 → 业务组件发送事件”的流程做。
5. 视觉预览这类高频图像数据不要放进 SDK，应留在 `vision` 内部管理，只把轻量状态文本、FPS、识别结果等通过 SDK 事件更新到 UI。
