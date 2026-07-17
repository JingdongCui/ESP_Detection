# UI 整体架构

> 适用范围：`components/UI/` 当前实现。本文按实际运行代码整理，用于后续定位、修改和扩展 UI。

## 1. 一句话总览

当前 UI 是一个 **单 LVGL Screen、整棵控件树常驻、通过隐藏/显示容器实现页面切换** 的 dashboard：

- `generated/` 负责控件树、样式、图片、字体和设计器生成事件；
- `sdk/ui.c` 负责业务数据绑定与手写交互绑定；
- 外部任务通过 `evt.c` 的同步事件分发更新 UI；
- UI 操作通过 `ui_dashboard_handlers_t` 回调到 `main` 注入的 BSP/vision 实现；
- 网络设置目前是例外，`sdk/ui.c` 直接调用 `Ethernet_app` 接口；
- 所有跨任务 LVGL 访问都必须由调用方持有 `BSP_LVGL_Lock()`。

```text
main/System_Init
    │
    ├─ BSP_LVGL_AdapterInit()          创建 LVGL worker、显示和触摸
    │
    ├─ [BSP_LVGL_Lock]
    │    ├─ setupUi()                  创建并加载唯一 screen
    │    └─ ui_bind_dashboard()        注册数据事件 + 挂载业务交互
    │  [BSP_LVGL_Unlock]
    │
    ├─ vision_start()                  启动采集/显示/推理任务
    ├─ system_monitor()                启动系统监视任务
    └─ ethernet_app_start()            启动以太网事件及 TCP 任务
```

关键入口：

- 固件启动：`main/system_init.c:47`
- UI 创建：`generated/setup_ui.c:17`
- dashboard 创建：`generated/setup_scr_dashboard.c:11275`
- 业务统一绑定：`sdk/ui.c:773`

---

## 2. 目录职责

```text
components/UI/
├─ CMakeLists.txt
├─ generated/                  AnyUI 生成区
│  ├─ setup_ui.c/.h            UI 总入口、资源声明
│  ├─ setup_scr_dashboard.c    全部控件对象、样式、设计器事件
│  ├─ animations.c/.h          生成动画
│  ├─ images/                  图片转成的 lv_img_dsc_t C 资源
│  ├─ fonts/                   字体转成的 LVGL C 资源
│  └─ *.mk / SConscript        AnyUI/模拟器构建清单
├─ sdk/                        UI 运行时与业务适配层
│  ├─ ui.c/.h                  LVGL 工具、数据刷新、业务交互绑定
│  ├─ evt.c/.h                 同步事件表/事件分发
│  ├─ cmd.c/.h                 通用命令兼容层，当前 dashboard 未使用
│  ├─ scr_list.c/.h            多 screen 栈，当前只有一个 screen
│  ├─ mem.h                    LVGL 8/9 内存 API 兼容
│  ├─ comm.h / sdk.h           聚合头文件
│  └─ *.mk / SConscript        SDK 构建清单
└─ simulator/                  SDL 桌面模拟器
   ├─ main.c                   LVGL + SDL 启动入口
   ├─ dummy.c                  BT/Wi-Fi/电池等占位命令和定时数据
   ├─ lv_conf.h                模拟器 LVGL 配置
   └─ Makefile/runtime.mk      模拟器构建配置
```

### 2.1 生成代码与手写代码边界

| 区域 | 主要职责 | 修改建议 |
|---|---|---|
| `generated/setup_scr_dashboard.c` | 控件定义、布局、样式、设计器配置的点击事件 | 优先回 AnyUI 工程修改后重新生成；直接手改容易被覆盖 |
| `generated/setup_ui.c/.h` | 创建 screen、加载 screen、声明资源 | 视为生成文件，不放业务逻辑 |
| `generated/images/`、`generated/fonts/` | 二进制资源的 C 表示 | 由资源生成流程维护，不手改数组内容 |
| `sdk/ui.c/.h` | 项目业务绑定、事件到 UI、UI 到业务 | 当前主要手写扩展点 |
| `sdk/evt.c/.h` | 事件类型和同步分发基础设施 | 新增数据事件时扩展 `evt.h`，谨慎改分发语义 |
| `main/system_init.c` | 注入业务 handler、控制初始化顺序 | 新增 UI→业务能力时通常需要补一项 handler |

虽然 `sdk/ui.c/.h` 文件头仍标记为 AnyUI SDK 文件，但当前项目已经在其中加入大量业务适配。重新生成 UI 时，应确认生成器是否会覆盖 `sdk/`，并在生成前保留差异。

---

## 3. 初始化与对象生命周期

### 3.1 固件初始化顺序

`System_Init()` 的 UI 相关顺序位于 `main/system_init.c:47`：

1. 初始化 LCD、触摸、摄像头、马达和编码器；
2. `BSP_LVGL_AdapterInit()` 创建 LVGL worker，并注册 1024×600 显示与触摸；
3. 获取 `BSP_LVGL_Lock()`；
4. 调用 `setupUi()`；
5. 调用 `ui_bind_dashboard()` 注入业务函数并完成双向绑定；
6. 释放 LVGL 锁；
7. UI 控件就绪后再启动 vision、system monitor、Ethernet 等生产者任务。

这个顺序保证：后台任务第一次发送事件前，控件指针和事件表已经完成初始化。

### 3.2 唯一 screen 的创建

`setupUi()` 位于 `generated/setup_ui.c:17`：

```text
setupUi()
  ├─ setup_scr_dashboard()
  │   ├─ create_ui()             第一次调用时创建全部 534 个全局控件指针
  │   ├─ register_ui_events()    挂载设计器生成事件
  │   ├─ register_sys_events()   当前为空
  │   └─ init_states()
  │       └─ set_current_event_table(&scr_dashboard_event_table)
  ├─ lv_scr_load(scr_dashboard)
  └─ push_scr(setup_scr_dashboard)
```

`setup_scr_dashboard()` 有单例判断：`scr_dashboard != NULL` 时不重建控件树，只重新设置当前事件表并返回原对象。因此当前 dashboard 的对象生命周期基本等于应用生命周期。

### 3.3 全局控件指针

`generated/setup_scr_dashboard.c` 将所有控件定义为全局 `lv_obj_t *`。`sdk/ui.c` 和 vision 显示代码通过 `extern` 直接访问这些对象。

优点：绑定简单、没有查找成本。  
代价：模块之间依赖具体控件名；AnyUI 重命名或删除控件时，手写代码会直接编译失败，需要同步修改 `sdk/ui.c` 和使用该控件的外部组件。

---

## 4. 页面与屏幕切换

## 4.1 当前不是多 screen 架构

当前只有一个真正的 LVGL screen：`scr_dashboard`。顶部导航和设置页子导航都没有调用 `lv_scr_load()`，而是在同一对象树中修改 `LV_OBJ_FLAG_HIDDEN`。

因此这里应区分：

- **screen 切换**：`lv_scr_load()` / `lv_scr_load_anim()`，当前只发生在启动；
- **页面切换**：同一 screen 内显示一个容器、隐藏其他容器，这是当前实际导航方式。

### 4.2 一级导航

一级容器都挂在 `scr_dashboard_cont_background2` 下：

| 顶部按钮 | 显示容器 | 隐藏的同级容器 |
|---|---|---|
| `imgbtn_dash` | `cont_dashboard` | `cont_system`、`cont_log`、`cont_set` |
| `imgbtn_sys` | `cont_system` | `cont_dashboard`、`cont_log`、`cont_set` |
| `imgbtn_log` | `cont_log` | `cont_dashboard`、`cont_system`、`cont_set` |
| `imgbtn_set` | `cont_set` | `cont_dashboard`、`cont_system`、`cont_log` |

对应生成事件位于 `generated/setup_scr_dashboard.c:611-702`。事件除了隐藏/显示容器，还同步切换顶部 image button 的图片源和 `LV_STATE_CHECKED`。

默认创建状态：

- `cont_dashboard` 可见；
- `cont_system`、`cont_log`、`cont_set` 初始隐藏。

### 4.3 设置页二级导航

设置页的五个子容器都挂在 `scr_dashboard_cont_set` 下：

- `scr_dashboard_cont_display`
- `scr_dashboard_cont_detect`
- `scr_dashboard_cont_control`
- `scr_dashboard_cont_net`
- `scr_dashboard_cont_about`

点击对应侧边按钮时，生成事件会显示目标容器、隐藏另外四个，并切换按钮图片/checked 状态。对应实现位于 `generated/setup_scr_dashboard.c:797-926`。

这五个子容器在创建时都被设置为隐藏。因此进入设置页后，需要由二级导航点击来选择具体面板；如果设计上希望进入设置页立即显示默认子页，应在 AnyUI 初始状态或 `ui_bind_dashboard()` 中明确设置默认面板。

### 4.4 日志页内部导航

日志页有两套切换：

1. `sdk/ui.c:693` 的 `ui_show_log_page()` 控制四个分页容器 `cont_log_page1..4`；
2. `generated/setup_scr_dashboard.c` 中 20 个日志条目事件控制 `cont_detail_01..20` 的显示，并维护条目的 checked 状态。

前后翻页按钮由 `sdk/ui.c:724` 手工挂载；页码限制为 `1..4`，不会循环。

### 4.5 预留但当前未使用的多 screen 设施

`sdk/scr_list.c` 实现了一个以 `setup_func_t` 为元素的链表栈，`sdk/cmd.c` 的 `CMD_NAVIGATE_BACK` 可以 pop 后重建/加载上一 screen；`sdk/ui.c:182` 也提供了动画加载包装。

但当前实际情况是：

- `setupUi()` 只 push `setup_scr_dashboard`；
- 项目中没有第二个 `setup_scr_*`；
- `send_command()` 没有调用者；
- 栈只剩一个 screen 时，返回命令会直接拒绝 pop；
- `ui_scr_load_with_anim()` 当前无调用者。

所以这些属于 AnyUI SDK 的预留能力，不是当前导航主链路。

---

## 5. UI 数据更新：业务数据 → UI

### 5.1 事件类型

`sdk/evt.h` 定义当前 dashboard 使用的三类业务事件：

| 事件 | 数据结构 | 主要显示内容 |
|---|---|---|
| `EVT_SYSTEM_MONITOR_CHANGED` | `system_monitor_event_data_t` | CPU、内存、PSRAM、运行时间、温度、频率、任务数 |
| `EVT_VISION_RESULT_CHANGED` | `vision_result_event_data_t` | 识别状态、面单/Logo 置信度、FPS、推理耗时、公司、分类概率 |
| `EVT_ETHERNET_STATUS_CHANGED` | `ethernet_event_data_t` | 以太网连接/断开图标 |

旧的 BT、Wi-Fi、电池、抽屉、时间事件仍保留在 SDK 中，但当前 dashboard 的 `ui_register_all_events()` 没有注册这些事件。

### 5.2 事件表结构

每个生成 screen 可以拥有一个 `event_table_t`。当前 dashboard 在 `generated/setup_scr_dashboard.c:553` 定义 `scr_dashboard_event_table`，并在 `init_states()` 中设为全局 `current_event_table`。

`ui_bind_dashboard()` 调用 `ui_register_all_events()`，把三类业务回调注册到当前表中：

```text
scr_dashboard_event_table
  ├─ EVT_SYSTEM_MONITOR_CHANGED → ui_system_monitor_event_cb
  ├─ EVT_VISION_RESULT_CHANGED → ui_vision_result_event_cb
  └─ EVT_ETHERNET_STATUS_CHANGED → ui_ethernet_event_cb
```

注册节点由 LVGL 内存分配器创建，插入链表头。相同的 `(event, code, callback)` 会被拒绝重复注册。

### 5.3 `send_event()` 是同步回调，不是消息队列

`sdk/evt.c:31` 的 `send_event()` 只是遍历链表，并在调用者当前任务中直接执行匹配回调：

```text
producer task
  └─ send_event(table, event, code, &stack_data, status)
       └─ callback(..., data, ...)
            └─ 立即修改 LVGL 控件
```

重要语义：

- 没有 FreeRTOS queue；
- 没有数据复制；
- 没有 `lv_async_call()`；
- 回调执行结束后 `send_event()` 才返回；
- 生产者可以安全传栈上结构体，但回调绝不能保存 `data` 指针；
- 回调耗时会直接阻塞生产者任务；
- 事件表本身没有互斥，依赖“启动阶段注册完成，运行期只读”的约束。

### 5.4 三条实际数据链路

#### A. 系统监控

```text
system_monitor 的 sysmon 任务
  └─ post_system_ui()                    system_monitor.c:59
      ├─ 构造栈上 system_monitor_event_data_t
      ├─ BSP_LVGL_Lock()
      ├─ send_event(EVT_SYSTEM_MONITOR_CHANGED)
      │   └─ ui_system_monitor_event_cb  sdk/ui.c:320
      │       ├─ 更新 CPU arc/百分比
      │       ├─ 更新内存和 PSRAM slider/label
      │       └─ 更新运行时间、温度、频率、任务数、关于页信息
      └─ BSP_LVGL_Unlock()
```

#### B. 视觉结果

视觉链路为了保证“显示帧、检测框、文字结果”时间对齐，不由推理任务直接发 UI 事件：

```text
vision_det(core1)
  ├─ 推理并构造 vision_det_frame_t
  ├─ 把 UI 文本放入 frame.ev
  └─ vision_draw_save_result()           写入带 mutex 的结果环形队列

vision_disp(core0)
  └─ vision_draw_lcd_disp_cb()           vision_draw.c:235
      ├─ 按 timestamp 选择最贴近当前显示帧的结果
      ├─ 在预览缓冲画框
      └─ 有新结果时：
          ├─ BSP_LVGL_Lock()
          ├─ send_event(EVT_VISION_RESULT_CHANGED)
          │   └─ ui_vision_result_event_cb  sdk/ui.c:371
          └─ BSP_LVGL_Unlock()
```

注意：`vision_draw_lcd_disp_cb()` 自己获取一次 LVGL 锁来发文字事件，随后显示任务再单独获取锁把视频区域搬到 framebuffer，避免锁嵌套和长时间持锁。

#### C. 以太网状态

```text
ESP Ethernet/IP event loop
  └─ post_eth_status_ui()                ethernet_app.c:931
      ├─ 构造栈上 ethernet_event_data_t
      ├─ BSP_LVGL_Lock()
      ├─ send_event(EVT_ETHERNET_STATUS_CHANGED)
      │   └─ ui_ethernet_event_cb        sdk/ui.c:414
      └─ BSP_LVGL_Unlock()
```

以太网状态控件是 image button。回调使用 `lv_imagebutton_set_state()`，不是只增删 `LV_STATE_CHECKED`，因为两态差异来自图片源而非普通样式；前者会强制刷新图片。

### 5.5 减少无效重绘

`sdk/ui.c:30` 的 `ui_label_set_text_safe()` 会先比较 label 当前文本，只有内容变化时才调用 `lv_label_set_text()`，避免重复分配、布局和 invalidate。新增高频数据字段时应继续使用：

- `ui_label_set_text_safe()`
- `ui_label_set_text_fmt_safe()`
- `ui_arc_percent_set_by_label()`

不要在高频事件回调里无条件反复 `lv_label_set_text_fmt()`。

---

## 6. 用户交互与指令下发：UI → 业务

### 6.1 统一 handler 注入

`sdk/ui.h:100` 定义 `ui_dashboard_handlers_t`。`System_Init()` 在 `main/system_init.c:71` 注入实际实现，`ui_bind_dashboard()` 将结构体拷贝到静态 `s_handlers`。

当前映射：

| UI 操作 | `ui_dashboard_handlers_t` 字段 | 实际业务函数 |
|---|---|---|
| 亮度滑块 | `brightness` | `BSP_LCD_SetBrightness` |
| 左上 LOGO 点击 | `calibration` | 当前实际为 `vision_frame_dump_request` |
| 检测开关 | `detection_enabled` | `vision_set_detection_enabled` |
| 预览叠框开关 | `preview_overlay_enabled` | `vision_set_preview_overlay_enabled` |
| 面单阈值读/写 | `waybill_score_threshold_get/set` | `vision_model_get/set_waybill_score_threshold_percent` |
| Logo 阈值读/写 | `logo_score_threshold_get/set` | `vision_model_get/set_logo_score_threshold_percent` |
| 模型信息 | `model_info_get` | `vision_model_get_model_info_string` |

其中：

- 检测和叠框开关使用 C11 atomic，跨任务读取安全；
- 模型阈值使用 mutex 更新默认值和模型运行时阈值；
- 模型信息返回常驻静态字符串，UI 不释放；
- LOGO handler 名称/注释仍写“校准”，但当前注入的是诊断抓帧请求，不是 ROI 校准，修改时要以 `system_init.c` 的实际绑定为准。

### 6.2 绑定时会主动同步默认值

`ui_attach_all_widgets()` 不只是挂事件，还会初始化控件和业务状态：

- 亮度设为 80%，主动发送 `LV_EVENT_VALUE_CHANGED`，同步标签和背光；
- 检测、预览叠框开关设为开启，并主动下发到 vision；
- 两个阈值先通过 getter 读取运行时默认值，再设 slider 并主动调用 setter；
- 日志页切到第 1 页；
- 关于页填充模型信息；
- 网络控件填 IP，并把默认上报配置下发到 Ethernet。

因此 `ui_bind_dashboard()` 应只调用一次。重复调用虽然事件表能拒绝相同业务事件，但 LVGL 控件事件会被重复挂载，导致一次操作触发多次业务调用。

### 6.3 网络设置的直接依赖

网络设置没有走 `ui_dashboard_handlers_t`，而是由 `sdk/ui.c` 直接调用：

- `ethernet_app_get_local_ip()`
- `ethernet_app_get_host_ip()`
- `ethernet_app_set_metrics_interval_ms()`
- `ethernet_app_set_report_image_enabled()`
- `ethernet_app_set_report_metrics_enabled()`

这也是 `components/UI/CMakeLists.txt` 显式 `REQUIRES Ethernet_app` 的原因。

如后续希望 UI 完全与业务解耦，应把这些接口也迁移到 `ui_dashboard_handlers_t`；在迁移前，新增网络控件可保持现有直接调用风格以减少不一致。

### 6.4 当前“控制页”并没有真正下发控制指令

生成代码中的 PID 速度 50/40/30/20、PID 模式、开环模式按钮，目前只做：

- checked 状态互斥；
- PID/开环子容器隐藏与显示。

它们没有调用 `send_command()`，也没有调用 sorter/motor 业务接口。因此当前控制页是显示层状态，不会改变实际分拣或电机行为。

同样，`sdk/cmd.c` 的 `send_command()` 当前在项目中没有调用者。后续接入控制功能时，不要误以为现有按钮已经完成指令下发。

推荐接入方式：给 `ui_dashboard_handlers_t` 增加明确的业务 handler，例如模式、目标速度或启停回调，再在 `sdk/ui.c` 手工挂载控件事件；避免直接把分拣业务代码写进生成文件。

---

## 7. 线程与锁模型

### 7.1 核心规则

LVGL 不是线程安全的。除 LVGL worker 自身回调外，任意任务访问 `lv_obj_t` 或 framebuffer 相关共享显示资源时，都必须持有：

```c
BSP_LVGL_Lock();
/* LVGL / 显示共享资源操作 */
BSP_LVGL_Unlock();
```

锁实现位于 `components/bsp/bsp_lvgl_adapter_init.c:72`，底层调用 `esp_lv_adapter_lock(-1)`。

### 7.2 当前各上下文

| 上下文 | 是否直接操作 UI | 当前保护方式 |
|---|---|---|
| LVGL worker 中的按钮/slider 回调 | 是 | worker 已在 LVGL 上下文，无需再次加锁 |
| `System_Init()` 创建和绑定 UI | 是 | 外层 `BSP_LVGL_Lock()` |
| `sysmon` 后台任务 | 是，经同步事件回调 | `post_system_ui()` 外层加锁 |
| ESP Ethernet 事件循环 | 是，经同步事件回调 | `post_eth_status_ui()` 外层加锁 |
| `vision_disp` 任务 | 是，经同步事件及 framebuffer 搬运 | 分段加锁，重计算/PPA/画框尽量放锁外 |
| `vision_det` 任务 | 否，先写 vision 自有结果队列 | 使用结果队列 mutex，不获取 LVGL 锁 |

### 7.3 新增事件生产者时的固定模板

```c
my_ui_event_data_t data = { /* 填充完整快照 */ };

BSP_LVGL_Lock();
send_event(get_current_event_table(), EVT_MY_MODULE, EVT_MY_MODULE_CHANGED,
           (uint8_t *)&data, 0);
BSP_LVGL_Unlock();
```

事件回调应只做快速控件更新，禁止在持 LVGL 锁期间执行：

- 网络阻塞收发；
- 长耗时推理；
- 大块图片缩放/编码；
- 等待其他任务完成；
- 可能反向等待 LVGL 的锁操作。

如果未来事件数据产生频率很高或回调变重，应把当前同步分发改造成有所有权定义的队列/异步刷新机制，而不是直接从生产者任务持续调用 LVGL。

---

## 8. 如何新增功能

### 8.1 新增“业务数据 → UI 显示”

1. 在 AnyUI 中创建或调整控件，重新生成 `generated/`；
2. 在 `sdk/evt.h` 增加事件号、事件 code 和完整快照结构体；
3. 在 `sdk/ui.c` 声明目标控件 `extern`；
4. 编写 `ui_xxx_event_cb()`：校验 `data != NULL` 且 `len == sizeof(...)`；
5. 编写 `ui_register_xxx_events()`；
6. 在 `ui_register_all_events()` 增加注册调用；
7. 业务生产者构造数据，在 `BSP_LVGL_Lock()` 内同步 `send_event()`；
8. 保证生产者在 `ui_bind_dashboard()` 之后启动。

推荐传“完整显示快照”，不要让 UI 回调再跨模块查询多个不一致的瞬时值。

### 8.2 新增“UI 操作 → 业务执行”

1. 在 `sdk/ui.h` 定义清晰的 handler typedef；
2. 给 `ui_dashboard_handlers_t` 增加字段；
3. 在 `sdk/ui.c` 写控件事件回调，读取/校验值后调用 `s_handlers.xxx`；
4. 写 `ui_attach_xxx()` 并加入 `ui_attach_all_widgets()`；
5. 在 `main/system_init.c` 的复合字面量中注入实际业务函数；
6. 业务层用 atomic、mutex 或队列处理与其他任务共享的状态；
7. 不在生成事件中直接加入业务依赖，防止重新生成后丢失。

### 8.3 新增一级/二级页面

当前架构适合继续用“常驻容器 + hidden”方式：

1. 在 AnyUI 中增加目标容器和导航按钮；
2. 同级页面切换时保证只显示一个目标容器；
3. 同步更新按钮图片源和 checked 状态；
4. 若页面包含视频直刷等外部绘制区域，外部任务必须检查页面可见性；
5. 若控件树和内存继续膨胀，再考虑真正拆成多个 screen 并按需创建/销毁。

不要把容器页面误加到 `scr_list`。只有真正由 `lv_scr_load()` 切换的顶级 screen 才应进入 screen 栈。

### 8.4 接通控制页真实命令

控制页当前只有视觉反馈。建议按以下顺序接入：

```text
控制按钮事件
  → sdk/ui.c 读取模式/速度
  → s_handlers.sorter_xxx(...)
  → main/system_init.c 注入 sorting_sim_control 或正式 sorter 接口
  → 业务层自行处理并发与硬件状态
  → 如需状态回显，再新增 EVT_SORTER_STATUS_CHANGED 反向刷新 UI
```

“用户点击后的目标值”和“设备实际执行状态”最好分开：前者通过 handler 下发，后者由业务状态事件回传，避免 UI 仅凭 checked 状态假设硬件已成功执行。

---

## 9. 模拟器能力与限制

`simulator/main.c` 使用 SDL 创建 1024×600 显示，调用 `setupUi()` 后循环执行 `lv_timer_handler()`。它适合验证：

- 控件布局和资源；
- 一级/二级容器导航；
- 生成事件的 checked/hidden 行为；
- 日志条目和分页等纯 UI 交互。

当前限制：

- 模拟器没有调用 `ui_bind_dashboard()`；
- `dummy.c` 只提供旧 BT/Wi-Fi/电池/时间占位逻辑；
- dashboard 当前没有注册这些旧事件；
- BSP、vision、Ethernet 的实际 handler 不存在于模拟器。

因此模拟器目前不能完整验证亮度、视觉阈值、识别数据、以太网状态和业务指令链路。若要让模拟器覆盖这些功能，应增加一套 simulator handler 和业务事件定时注入，并调用 `ui_bind_dashboard()`。

---

## 10. 当前架构中的注意点

1. **单 screen，不要把容器切换当作 screen 切换。** 绝大部分导航问题应检查 hidden flag 和按钮状态，而不是检查 screen 栈。
2. **`send_event()` 同步执行。** 生产者传栈数据是安全的，但回调不能保存指针，且生产者必须先持 LVGL 锁。
3. **事件注册不是线程安全的。** 保持所有注册在后台生产者启动前完成，运行期不要动态增删当前事件表。
4. **`ui_bind_dashboard()` 只调用一次。** 重复调用会重复挂载 LVGL 控件回调。
5. **生成控件名是手写层 ABI。** AnyUI 重命名控件后，要同步检查 `sdk/ui.c`、vision 等所有 `extern` 使用者。
6. **控制页当前不控制硬件。** PID/开环/速度按钮只改 UI 状态。
7. **LOGO 当前触发抓帧。** handler 字段名叫 `calibration`，但实际绑定为 `vision_frame_dump_request`。
8. **网络设置是 UI 对业务的直接依赖例外。** 其余主要业务通过 handler 注入。
9. **视频预览不是普通 LVGL image。** vision 显示任务把 PPA 结果直接搬入双 framebuffer 的预览区域，并根据 `cont_dashboard` 是否隐藏决定是否绘制。
10. **高频文本更新要走 safe helper。** 避免无变化文本触发重分配和重绘。
11. **大量控件常驻内存。** 当前生成文件有约 534 个全局控件指针，所有页面一次创建；新增大页面时需关注内部 SRAM/堆和首次创建耗时。
12. **图片按钮换图要用正确 API。** 以太网状态这种“状态对应不同图片源”的控件，应使用 image button state API 触发重绘。

---

## 11. 快速定位索引

| 想修改的内容 | 首先查看 |
|---|---|
| UI 启动顺序、业务注入 | `main/system_init.c:47` |
| 唯一 screen 的创建和加载 | `generated/setup_ui.c:17` |
| 控件定义、布局、生成点击事件 | `generated/setup_scr_dashboard.c` |
| dashboard 初始化单例 | `generated/setup_scr_dashboard.c:11275` |
| 顶部一级导航 | `generated/setup_scr_dashboard.c:611` |
| 设置页二级导航 | `generated/setup_scr_dashboard.c:797` |
| 数据事件结构 | `sdk/evt.h:48` |
| 事件注册/同步派发 | `sdk/evt.c:31`、`sdk/evt.c:140` |
| 系统/视觉/以太网数据刷新 | `sdk/ui.c:320`、`:371`、`:414` |
| UI→业务 handler 定义 | `sdk/ui.h:74` |
| UI→业务控件绑定 | `sdk/ui.c:451` |
| dashboard 统一绑定入口 | `sdk/ui.c:773` |
| 系统监控事件生产者 | `components/system_monitor/system_monitor.c:59` |
| 视觉结果对齐及事件生产者 | `components/vision/framework/vision_draw.c:235` |
| 以太网状态事件生产者 | `components/Ethernet_app/ethernet_app.c:931` |
| LVGL 全局锁 | `components/bsp/bsp_lvgl_adapter_init.c:72` |
| 多 screen 预留栈 | `sdk/scr_list.c`、`sdk/cmd.c` |
| 桌面模拟器入口 | `simulator/main.c:77` |

---

## 12. 维护时的推荐检查清单

### 修改 UI 布局后

- [ ] 重新生成后目标控件名是否变化；
- [ ] `sdk/ui.c` 和外部组件中的 `extern` 是否仍存在；
- [ ] 初始 hidden/checked 状态是否正确；
- [ ] 顶部和设置页导航是否保持互斥；
- [ ] `components/UI/CMakeLists.txt` 是否包含新增图片/字体 C 文件；
- [ ] 固件编译和 SDL 模拟器是否都能通过。

### 新增数据刷新后

- [ ] 事件结构体在生产者和 UI 两边是否一致；
- [ ] 回调是否校验 `len`；
- [ ] 生产者是否在 `BSP_LVGL_Lock()` 内调用 `send_event()`；
- [ ] 回调是否只做快速 UI 更新；
- [ ] 是否避免保存生产者传入的数据指针；
- [ ] 高频文本是否使用 safe helper。

### 新增业务控制后

- [ ] 是否优先走 `ui_dashboard_handlers_t`；
- [ ] 是否在 `System_Init()` 中完成注入；
- [ ] 共享业务状态是否由 atomic/mutex/queue 保护；
- [ ] 是否区分“命令已点击”和“硬件已执行”；
- [ ] 是否需要反向状态事件更新 UI；
- [ ] 是否避免把业务代码写进 `generated/`。
