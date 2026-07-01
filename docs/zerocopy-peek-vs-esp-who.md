# 零拷贝 peek 帧总线：本次实现与 ESP-who 的对比

> 适用范围：`components/vision` 复刻 ESP-who 的"fetch ringbuf + 零拷贝 peek"帧来源机制
> 编写日期：2026-06-27
> 配套文档：`docs/零拷贝peek竞态风险.md`（概率保证的竞态风险说明）

---

## 一、本文档在说什么

我们把推理帧来源从原先的「PSRAM 双缓冲 + memcpy 副本」改成了 ESP-who 原生的
「ringbuf 持有 mmap 帧 + 零拷贝 peek」。本文逐项说明：

- ESP-who 的机制长什么样（C++ pipeline 节点）；
- 我们用 C 怎么复刻的，哪些 1:1 照搬、哪些按需简化；
- 两个刻意保留的差异（推理读原图、显示走 LCD 直刷）及原因。

---

## 二、ESP-who 的原始框架

ESP-who 的采集链路是一条 **pipeline**，每个节点是独立 FreeRTOS 任务，节点间用
长度 1 的覆盖队列（`xQueueOverwrite`）串联。核心基类是 `WhoFrameCapNode`
（`components/who_frame_cap/who_frame_cap_node.cpp`），派生出三类节点：

```
WhoFetchNode      ── cam_fb_get(DQBUF) 取原始帧 ──┐
                                                  ├─ 各自持有一个 ringbuf
WhoDecodeNode     ── JPEG 解码（本项目用不到）     │
WhoPPAResizeNode  ── PPA 缩放出推理用小图 ─────────┘
```

下游的 `WhoDetect`（推理）作为**订阅者**挂到某个节点上，等 `NEW_FRAME` 事件，
然后 `cam_fb_peek()` 取该节点 ringbuf 里最新那帧的指针直接跑模型。

### 2.1 三个关键函数（我们复刻的就是这三处）

**① ringbuf 持有 + 满才归还**（`WhoFetchNode::update_ringbuf`，167-174 行）：

```cpp
void WhoFetchNode::update_ringbuf(cam_fb_t *fb) {
    if (m_cam_fbs.full()) {
        auto fb_prev = m_cam_fbs.pop();
        m_cam->cam_fb_return(fb_prev);   // 满了才把最旧帧 QBUF 还驱动
    }
    m_cam_fbs.push(fb);
}
```

**② 满才发 NEW_FRAME**（`WhoFrameCapNode::task`，131-141 行）：

```cpp
xSemaphoreTake(m_mutex, portMAX_DELAY);
update_ringbuf(out_fb);
bool full_ringbuf = m_cam_fbs.full();
xSemaphoreGive(m_mutex);
if (full_ringbuf) {                       // 只有 ringbuf 填满才通知订阅者
    for (const auto &task : m_tasks) {
        if (task->is_active())
            xEventGroupSetBits(task->get_event_group(), NEW_FRAME);
    }
}
```

**③ 零拷贝 peek 最新帧**（`WhoFrameCapNode::cam_fb_peek`，53-76 行）：

```cpp
cam_fb_t *WhoFrameCapNode::cam_fb_peek(int index) {
    xSemaphoreTake(m_mutex, portMAX_DELAY);
    if (index == -1) index = m_cam_fbs.size() - 1;   // -1 = 最新那帧
    cam_fb_t *ret = m_cam_fbs[index];
    xSemaphoreGive(m_mutex);                          // 取到指针立即放锁
    return ret;                                       // 之后读图全程无锁
}
```

订阅者登记用 `add_new_frame_signal_subscriber`（78-81 行）。

### 2.2 防竞态原理

`m_cam_fbs` 是深度 `ringbuf_len` 的环形缓冲。被它持有的帧（DQBUF 出队但未 QBUF
归还）驱动不会重填。推理 peek 的是"最新"帧，要被连续挤进 `ringbuf_len` 个更新的帧
之后才会沦为最旧帧被 pop 还给驱动。这段时间就是推理读完的安全窗口——**靠深度，不靠锁**。

---

## 三、我们的 C 复刻

ESP-who 是 C++ 模板 pipeline，节点可任意级联、ringbuf 是模板容器、节点间用队列。
本项目只需要「采集 → 显示 + 推理」一条最短链路，故用 C 做了**等价但扁平的**实现：
单一原图 ringbuf + 事件组订阅表，不引入 pipeline/队列级联。

文件：
- `components/bsp/bsp_cam_sensor.c/.h` —— V4L2 取帧层（等价 ESP-who 的 `WhoP4Cam`）
- `components/vision/vision_internal.h` —— 帧总线接口
- `components/vision/vision_app.c` —— ringbuf + fetch 任务 + 显示任务
- `components/vision/vision_detect.c` —— 推理任务（订阅者，当前为桩）

### 3.1 逐项对照表

| ESP-who（C++） | 本项目（C） | 关系 |
|---|---|---|
| `cam_fb_get()` / `cam_fb_return()`（WhoP4Cam，封装 V4L2） | `cam_sensor_get_frame()`（DQBUF）/ `cam_sensor_return_frame()`（QBUF） | **结构相同**，本来就都是 V4L2 MMAP |
| `m_cam_fbs`（环形缓冲模板，深度 `ringbuf_len`） | `s_ring[]`（`vision_frame_t` 数组）+ `s_ring_head/count/cap` | **简化**：模板容器 → 定长数组手写环形索引 |
| `WhoFetchNode::update_ringbuf`（满才 pop+return） | `ring_update()`（满才 pop+`cam_sensor_return_frame`） | **1:1 照搬**逻辑 |
| `WhoFrameCapNode::task`（满才发 NEW_FRAME） | `vision_fetch_task`（`ring_is_full()` 才发） | **1:1 照搬**逻辑 |
| `cam_fb_peek(-1)`（锁内取最新指针，立即放锁） | `vision_frame_peek_latest()`（同样锁内取、立即放） | **1:1 照搬**逻辑 |
| `add_new_frame_signal_subscriber` + 各任务自带 `m_event_group` | `vision_frame_subscribe()` 返回事件组 + `s_subs[]` 订阅表 | **等价**：C++ 任务对象自带事件组 → C 显式建组并登记 |
| `m_mutex` 保护 ringbuf | `s_ring_mutex` | 相同 |
| ringbuf 深度 = 构造参数 `ringbuf_len` | `s_ring_cap = cam_sensor_get_fb_count()-2` | **等价**：深度集中由 `CAM_FB_COUNT` 宏推导 |
| pipeline 多节点级联（Fetch→Decode→PPAResize），队列串联 | 单一原图 ringbuf，无级联无队列 | **简化**：本项目不需要解码/多级缩放节点 |

### 3.2 缓冲深度的单一来源

ESP-who 深度是构造时传入的 `ringbuf_len`。我们把它锚定到摄像头缓冲数：

```c
// bsp_cam_sensor.c
#define CAM_FB_COUNT  5            // V4L2 缓冲总数（唯一定义处）
int cam_sensor_get_fb_count(void) { return CAM_FB_COUNT; }

// vision_app.c → ring_init()
s_ring_cap = cam_sensor_get_fb_count() - 2;   // 深度 3：留 2 给驱动（1 写 +1 排队）
```

改 `CAM_FB_COUNT` 一处即同时改变 V4L2 缓冲数与 ringbuf 深度，二者天然一致，
不会出现"申请了 5 个缓冲但 ringbuf 只持有 3 个"之外的错配。深度→竞态时间窗口的
换算见 `零拷贝peek竞态风险.md`。

### 3.3 启动竞态的处理（一处 C 特有的细节）

ESP-who 里订阅关系在 pipeline 搭建时就连好，任务对象自带事件组，不存在"任务起来了
但还没人订阅"。我们是 C，事件组要显式建。为避免 fetch 任务已经开始发 `NEW_FRAME`
而订阅者事件组尚未创建：

```c
// vision_start()：先订阅，后建任务
EventGroupHandle_t disp_eg = vision_frame_subscribe();
EventGroupHandle_t det_eg  = vision_frame_subscribe();
... 然后才 xTaskCreatePinnedToCore(三个任务)
```

`s_subs[]`/`s_sub_count` 只在 `vision_start` 单线程里写、fetch 任务里只读，故无需加锁。

---

## 四、两个刻意保留的差异

这是与 ESP-who 唯二的功能差异，均为本项目需求决定，非复刻偏差：

### 差异 1：推理读**原始全尺寸图**，而非 PPA 缩放后的小图

- ESP-who：推理订阅的是 `WhoPPAResizeNode`，peek 到的是已缩放成模型输入尺寸的小图。
- 本项目：推理直接 peek **原图 ringbuf**，拿到的是摄像头 RGB565 全尺寸帧
  （`vision_detect.c` 里 `fb.buf` = 1024×600 裸 mmap 指针）。
- 原因：按需求，预处理/缩放留到模型接入阶段在推理侧做，采集层只管供原图。

### 差异 2：显示走**现有 LCD 直刷**，而非 LVGL canvas

- ESP-who：缩放结果回灌 LVGL canvas 控件显示。
- 本项目：显示任务 `vision_display_task` 用 PPA 把原图缩放转 RGB888 后，
  直接 `esp_lcd_panel_draw_bitmap` 刷到 LCD 视频区域（沿用项目既有预览机制）。
- 原因：复用本项目已验证的 LCD 直刷链路，不引入 canvas。

> 注意：显示侧的 PPA 缩放是显示**自己的**输出（写进 `s_preview_buf`），与推理无关。
> 推理那一路从 DQBUF 到读图全程是同一个裸 mmap 指针，零次图像拷贝。

---

## 五、与被替换的旧实现对比（为什么换）

| | 旧实现（双缓冲副本） | 新实现（零拷贝 peek） |
|---|---|---|
| 推理帧来源 | memcpy 一份原图到 PSRAM ping-pong 双缓冲 | 直接 peek ringbuf 里的 mmap 裸指针 |
| 每帧开销 | 一次全图 memcpy（约 1.2MB@1024×600） | 0（只拷几十字节的 `vision_frame_t` 引用） |
| 防竞态 | 双缓冲 + 读写索引错开 | ringbuf 深度撑出的时间窗口（概率保证） |
| 与 ESP-who | 思路对齐但非原生做法 | 1:1 原生做法 |
| 代价 | 安全、但每帧多一次大拷贝 | 零拷贝、但有概率竞态风险（见风险文档） |

换的动机：1:1 复刻 ESP-who 原生机制、消除每帧大拷贝。代价是引入概率竞态风险，
已在 `零拷贝peek竞态风险.md` 中评估并接受（30fps 下推理裸读窗口 ~100ms，足够）。

---

## 六、数据流总览

```
摄像头 DMA ──→ V4L2 mmap 缓冲[5]
                  │ DQBUF（零拷贝，给指针）
                  ▼
            vision_fetch_task
                  │ ring_update：push 进 s_ring（拷的是指针不是图）
                  │ 满 → pop 最旧帧 QBUF 还驱动
                  │ 满 → 向所有订阅者发 NEW_FRAME
       ┌──────────┴───────────┐
       ▼                      ▼
  vision_display_task    vision_detect_task
   peek 最新帧            peek 最新帧（同一裸指针）
   PPA 缩放→RGB888        （模型接入处）
   LCD 直刷               读 fb.buf 直接跑模型
```

全链路图像数据只有摄像头 DMA 写入那一次"产生"，之后采集→显示→推理传的都是指针。
显示侧 PPA 那次缩放是显示自身的输出，不在推理路径上。
