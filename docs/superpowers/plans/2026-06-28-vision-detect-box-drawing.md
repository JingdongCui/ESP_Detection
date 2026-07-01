# 画框链路（纯 C 复刻 esp-who）实施计划

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** 在现有 PPA→RGB888→LCD 直刷链路上，纯 C 复刻 esp-who 裸屏画框链路，用碰壁反弹假框跑通"坐标映射 + 时间戳对齐队列 + 在预览缓冲画矩形框"。

**Architecture:** 检测任务(core1)在原图坐标系造假框→rescale 到预览坐标系→带时间戳 push 结果队列；显示任务(core0)在 LVGL 锁内、draw_bitmap 前，按时间戳对齐取结果，在 s_preview_buf(RGB888) 上画空心矩形。每个部件 1:1 对应 esp-who 部件，C++ 底层调用用等价纯 C 重写。

**Tech Stack:** ESP-IDF (C)、FreeRTOS (mutex)、PPA、esp_lcd 直刷。无 git、无主机测试框架——验证靠编译 + 烧录 + RTT 日志 + 肉眼。

**项目约束（与 skill 默认不同，已调整）：**
- 不是 git 仓库 → **无 commit 步骤**。
- 画框逻辑依赖摄像头/PPA/LCD 硬件、无主机单测 → 用"编译通过 + 烧录运行 + RTT/肉眼确认"替代单元测试。
- 编译命令 `./agentic/idf_build.sh`，**超时设 600000ms（10 分钟）**。

---

## 文件结构

| 文件 | 责任 | 对应 esp-who |
|---|---|---|
| `components/vision/vision_internal.h`（改） | 新增结果结构体、画框模块 C 接口、预览尺寸 getter 声明 | result_t + 接口声明 |
| `components/vision/vision_draw.c`（新增） | 结果队列+mutex、save、时间戳对齐 lcd_disp_cb、纯 C 空心矩形绘制、深度宏 | who_detect_result_handle.cpp 整文件 |
| `components/vision/vision_detect.c`（改） | 假框碰壁反弹（原图坐标系）+ rescale + save | WhoDetect::task + rescale_detect_result |
| `components/vision/vision_app.c`（改） | 暴露预览尺寸 getter；显示任务锁内画框回调 | WhoFrameLCDDisp 触发 lcd_disp_cb |
| `components/vision/CMakeLists.txt`（改） | 注册 vision_draw.c | — |

---

## Task 1: 在 vision_internal.h 定义结果结构体与画框模块接口

**Files:**
- Modify: `components/vision/vision_internal.h`

- [ ] **Step 1: 在 vision_internal.h 的 `vision_detect_task` 声明之前插入结构体与接口声明**

在文件 `void vision_detect_task(void *arg);` 这行之前插入：

```c
// 一个检测框（纯 C 复刻 dl::detect::result_t，目标检测无 keypoint）。
// box 为 [left_up_x, left_up_y, right_down_x, right_down_y]，预览坐标系。
typedef struct {
    int category;   // 类别索引
    float score;    // 置信度
    int box[4];     // 边界框：左上 x,y 右下 x,y
} vision_det_result_t;

// 单帧检测结果集（复刻 WhoDetect::result_t {det_res, timestamp}）。
// timestamp 与产出该结果的帧的 vision_frame_t.timestamp 一致，供显示侧对齐。
#define VISION_MAX_BOXES 8  // 单帧最多框数（假框阶段仅 1，留余量）
typedef struct {
    vision_det_result_t items[VISION_MAX_BOXES];
    int count;
    int64_t timestamp;
} vision_det_frame_t;

// 检测侧 push 一帧结果进队列（复刻 WhoDetectResultLCDDisp::save_detect_result）。
// 内部加 mutex 保护，可跨任务调用。
void vision_draw_save_result(const vision_det_frame_t *frame);

// 显示侧回调：按 disp_timestamp 对齐取最贴合的结果，在 preview_buf(RGB888) 上画框
// （复刻 lcd_disp_cb + draw_detect_results_on_img）。须在持显示锁、draw_bitmap 前调用。
// preview_buf 尺寸为 preview_w × preview_h × 3 字节。
void vision_draw_lcd_disp_cb(uint8_t *preview_buf, int preview_w, int preview_h,
                             int64_t disp_timestamp);

// 取预览区域尺寸（vision_app.c 内 static，检测侧据此做坐标 rescale）。
// vision_start 完成后才有效；之前返回 0。
void vision_get_preview_size(int *w, int *h);
```

- [ ] **Step 2: 编译验证头文件语法**

Run: `./agentic/idf_build.sh`（超时 600000ms）
Expected: 编译通过（此时新接口未实现，但头文件只是声明，`vision_app.c`/`vision_detect.c` 尚未引用，不会链接报错）。若报未实现错误说明有文件已引用——属正常，继续 Task 2。

---

## Task 2: 新建 vision_draw.c —— 结果队列 + 时间戳对齐 + 纯 C 画框

**Files:**
- Create: `components/vision/vision_draw.c`
- Modify: `components/vision/CMakeLists.txt`

- [ ] **Step 1: 创建 vision_draw.c**

```c
/*
 * vision_draw —— 画框链路（纯 C 复刻 esp-who who_detect_result_handle.cpp）。
 *
 * 部件对应：
 *   - 结果队列 m_results        → s_queue 定长环形数组 + s_mutex
 *   - save_detect_result        → vision_draw_save_result（检测侧 push）
 *   - lcd_disp_cb               → vision_draw_lcd_disp_cb（显示侧对齐取结果+画框）
 *   - draw_detect_results_on_img→ draw_results_on_buf（esp-dl draw_hollow_rectangle 的纯 C 版）
 *   - palette                   → 红色 {255,0,0}
 *
 * esp-who 用无界 std::queue；纯 C 用定长环形数组，深度 VISION_RESULT_QUEUE_DEPTH。
 * 时间戳对齐每帧消费结果，正常不积压。
 */
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include "vision_internal.h"

#define RGB888_BYTES_PER_PIXEL 3

// 结果队列深度（单独宏，不沿用取帧 ringbuf 深度——二者语义无关）。
// 复刻 esp-who m_results：容忍检测 FPS > 显示 FPS 时的结果积压。
#define VISION_RESULT_QUEUE_DEPTH 3

// 画框颜色与线宽（复刻 esp-who 默认 palette={{255,0,0}} 与 line_width=2）。
#define BOX_COLOR_R 255
#define BOX_COLOR_G 0
#define BOX_COLOR_B 0
#define BOX_LINE_WIDTH 2

static const char *TAG = "vision_draw";

// ---- 结果队列（复刻 m_results + m_res_mutex）----
static SemaphoreHandle_t s_mutex;                       // 保护队列（独立于 LVGL 锁）
static vision_det_frame_t s_queue[VISION_RESULT_QUEUE_DEPTH];  // 环形数组
static int s_head;                                      // 最旧元素位置（pop 端）
static int s_count;                                     // 当前元素数
static vision_det_frame_t s_current;                    // 最近对齐选中的结果（复刻 m_result）
static bool s_inited;                                   // 懒初始化标志

// 懒初始化 mutex（首次 save/cb 时建，避免新增 init 入口改动 vision_start）。
static bool ensure_init(void)
{
    if (s_inited) {
        return true;
    }
    s_mutex = xSemaphoreCreateMutex();
    if (!s_mutex) {
        ESP_LOGE(TAG, "no memory for result mutex");
        return false;
    }
    s_head = 0;
    s_count = 0;
    memset(&s_current, 0, sizeof(s_current));
    s_inited = true;
    return true;
}

void vision_draw_save_result(const vision_det_frame_t *frame)
{
    if (!frame || !ensure_init()) {
        return;
    }
    xSemaphoreTake(s_mutex, portMAX_DELAY);
    if (s_count == VISION_RESULT_QUEUE_DEPTH) {
        // 满：丢最旧（head 前移），保证 push 不阻塞——复刻"显示慢时丢旧帧结果"。
        s_head = (s_head + 1) % VISION_RESULT_QUEUE_DEPTH;
        s_count--;
    }
    int tail = (s_head + s_count) % VISION_RESULT_QUEUE_DEPTH;
    s_queue[tail] = *frame;
    s_count++;
    xSemaphoreGive(s_mutex);
}

// 在 RGB888 缓冲上画一条水平/垂直实线段（含 clip，防越界）。
static void draw_hline(uint8_t *buf, int w, int h, int x0, int x1, int y, int thickness)
{
    for (int t = 0; t < thickness; t++) {
        int yy = y + t;
        if (yy < 0 || yy >= h) {
            continue;
        }
        for (int x = x0; x <= x1; x++) {
            if (x < 0 || x >= w) {
                continue;
            }
            uint8_t *p = buf + ((size_t)yy * w + x) * RGB888_BYTES_PER_PIXEL;
            p[0] = BOX_COLOR_R;
            p[1] = BOX_COLOR_G;
            p[2] = BOX_COLOR_B;
        }
    }
}

static void draw_vline(uint8_t *buf, int w, int h, int x, int y0, int y1, int thickness)
{
    for (int t = 0; t < thickness; t++) {
        int xx = x + t;
        if (xx < 0 || xx >= w) {
            continue;
        }
        for (int y = y0; y <= y1; y++) {
            if (y < 0 || y >= h) {
                continue;
            }
            uint8_t *p = buf + ((size_t)y * w + xx) * RGB888_BYTES_PER_PIXEL;
            p[0] = BOX_COLOR_R;
            p[1] = BOX_COLOR_G;
            p[2] = BOX_COLOR_B;
        }
    }
}

// 纯 C 复刻 dl::image::draw_hollow_rectangle：上下横线 + 左右竖线。
static void draw_hollow_rect(uint8_t *buf, int w, int h, int x1, int y1, int x2, int y2)
{
    draw_hline(buf, w, h, x1, x2, y1, BOX_LINE_WIDTH);                 // 上边
    draw_hline(buf, w, h, x1, x2, y2 - BOX_LINE_WIDTH + 1, BOX_LINE_WIDTH);  // 下边
    draw_vline(buf, w, h, x1, y1, y2, BOX_LINE_WIDTH);                 // 左边
    draw_vline(buf, w, h, x2 - BOX_LINE_WIDTH + 1, y1, y2, BOX_LINE_WIDTH);  // 右边
}

// 复刻 draw_detect_results_on_img：遍历结果画框（无 keypoint、无文字）。
static void draw_results_on_buf(uint8_t *buf, int w, int h, const vision_det_frame_t *frame)
{
    for (int i = 0; i < frame->count; i++) {
        const int *box = frame->items[i].box;
        draw_hollow_rect(buf, w, h, box[0], box[1], box[2], box[3]);
    }
}

void vision_draw_lcd_disp_cb(uint8_t *preview_buf, int preview_w, int preview_h,
                             int64_t disp_timestamp)
{
    if (!preview_buf || !ensure_init()) {
        return;
    }
    xSemaphoreTake(s_mutex, portMAX_DELAY);
    // 时间戳对齐（复刻 lcd_disp_cb 的 skip-future）：
    // 队首结果 timestamp <= 显示帧 timestamp 就取并 pop，直到结果比帧更新为止。
    // 留下的 s_current 是最贴合当前显示帧的结果。
    while (s_count > 0) {
        vision_det_frame_t *front = &s_queue[s_head];
        if (front->timestamp <= disp_timestamp) {
            s_current = *front;
            s_head = (s_head + 1) % VISION_RESULT_QUEUE_DEPTH;
            s_count--;
        } else {
            break;
        }
    }
    vision_det_frame_t to_draw = s_current;  // 拷出，缩短持锁时间
    xSemaphoreGive(s_mutex);

    draw_results_on_buf(preview_buf, preview_w, preview_h, &to_draw);
}
```

- [ ] **Step 2: 在 CMakeLists.txt 注册 vision_draw.c**

把 `components/vision/CMakeLists.txt` 的 SRCS 改为：

```cmake
idf_component_register(
    SRCS
        "vision_app.c"
        "vision_detect.c"
        "vision_draw.c"
    INCLUDE_DIRS
        "include"
    REQUIRES
        bsp
        UI
        esp_driver_ppa
        esp_lcd
        system_monitor
)
```

- [ ] **Step 3: 编译验证**

Run: `./agentic/idf_build.sh`（超时 600000ms）
Expected: 编译通过。vision_draw.c 的接口此时无人调用（detect/app 尚未接），但函数已定义、被链接保留，不报错。

---

## Task 3: vision_app.c 暴露预览尺寸 getter

**Files:**
- Modify: `components/vision/vision_app.c`

- [ ] **Step 1: 实现 vision_get_preview_size**

在 `vision_app.c` 的 `vision_start` 函数定义之前（例如紧接 `vision_frame_peek_latest` 之后）插入：

```c
// 取预览区域尺寸（检测侧据此把框从原图坐标系 rescale 到预览坐标系）。
// vision_start 设好 s_preview_w/h 后才有效；之前为 0。
void vision_get_preview_size(int *w, int *h)
{
    if (w) {
        *w = s_preview_w;
    }
    if (h) {
        *h = s_preview_h;
    }
}
```

- [ ] **Step 2: 编译验证**

Run: `./agentic/idf_build.sh`（超时 600000ms）
Expected: 编译通过。

---

## Task 4: vision_detect.c 假框碰壁反弹 + rescale + save

**Files:**
- Modify: `components/vision/vision_detect.c`

- [ ] **Step 1: 替换 vision_detect.c 全文**

```c
/*
 * vision_detect —— 推理任务（假框阶段，纯 C 复刻 WhoDetect::task 链路）。
 *
 * 复刻部件：
 *   - peek 最新帧                → vision_frame_peek_latest（原图坐标系）
 *   - 造结果                     → 假框碰壁反弹（替代 model->run，下一步换真模型）
 *   - rescale_detect_result      → rescale_box（原图坐标系 → 预览坐标系 + clip）
 *   - m_result_cb(save)          → vision_draw_save_result
 *
 * 假框在原图坐标系移动、碰壁反弹，经 rescale 落到预览坐标系，验证整条映射链。
 */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_timer.h"
#include "SEGGER_RTT.h"
#include "vision_internal.h"

// 假框尺寸（原图坐标系，固定宽高，仅平移）。
#define FAKE_BOX_W 120
#define FAKE_BOX_H 120
// 每帧位移（原图像素），数值越大移动越快。
#define FAKE_BOX_STEP 6

void vision_detect_task(void *arg)
{
    EventGroupHandle_t eg = (EventGroupHandle_t)arg;
    uint32_t frame_seq = 0;
    int64_t last_us = 0;

    // 假框状态（原图坐标系）：左上角位置 + 速度方向。首帧拿到原图尺寸后再初始化。
    int box_x = 0, box_y = 0;     // 左上角
    int vx = FAKE_BOX_STEP;       // x 方向速度
    int vy = FAKE_BOX_STEP;       // y 方向速度
    bool box_inited = false;

    while (true) {
        xEventGroupWaitBits(eg, VISION_NEW_FRAME, pdTRUE, pdFALSE, portMAX_DELAY);

        vision_frame_t fb;
        if (!vision_frame_peek_latest(&fb)) {
            continue;
        }

        frame_seq++;
        int64_t now_us = esp_timer_get_time();
        int fps10 = 0;
        if (last_us != 0) {
            int64_t dt = now_us - last_us;
            if (dt > 0) {
                fps10 = (int)(10000000LL / dt);
            }
        }
        last_us = now_us;

        // 首帧用原图尺寸把假框放中心。
        if (!box_inited) {
            box_x = (fb.width - FAKE_BOX_W) / 2;
            box_y = (fb.height - FAKE_BOX_H) / 2;
            if (box_x < 0) box_x = 0;
            if (box_y < 0) box_y = 0;
            box_inited = true;
        }

        // 碰壁反弹：移动后若越界则夹回边界并反向。
        box_x += vx;
        box_y += vy;
        if (box_x < 0) { box_x = 0; vx = -vx; }
        if (box_y < 0) { box_y = 0; vy = -vy; }
        if (box_x + FAKE_BOX_W > fb.width)  { box_x = fb.width - FAKE_BOX_W;  vx = -vx; }
        if (box_y + FAKE_BOX_H > fb.height) { box_y = fb.height - FAKE_BOX_H; vy = -vy; }

        // 造一帧结果（原图坐标系），随后 rescale 到预览坐标系。
        vision_det_frame_t result = {0};
        result.count = 1;
        result.timestamp = fb.timestamp;  // 与帧对齐，供显示侧时间戳对齐
        result.items[0].category = 0;
        result.items[0].score = 1.0f;
        // 原图坐标系 box：左上 (box_x,box_y) 右下 (box_x+W, box_y+H)
        int ox1 = box_x, oy1 = box_y;
        int ox2 = box_x + FAKE_BOX_W, oy2 = box_y + FAKE_BOX_H;

        // rescale（复刻 rescale_detect_result）：原图 → 预览，比例 = 预览/原图，再 clip。
        int pw = 0, ph = 0;
        vision_get_preview_size(&pw, &ph);
        if (pw > 0 && ph > 0 && fb.width > 0 && fb.height > 0) {
            float sx = (float)pw / fb.width;
            float sy = (float)ph / fb.height;
            int x1 = (int)(ox1 * sx);
            int y1 = (int)(oy1 * sy);
            int x2 = (int)(ox2 * sx);
            int y2 = (int)(oy2 * sy);
            // clip 到预览边界（复刻 limit_box）。
            if (x1 < 0) x1 = 0; if (x1 > pw - 1) x1 = pw - 1;
            if (y1 < 0) y1 = 0; if (y1 > ph - 1) y1 = ph - 1;
            if (x2 < 0) x2 = 0; if (x2 > pw - 1) x2 = pw - 1;
            if (y2 < 0) y2 = 0; if (y2 > ph - 1) y2 = ph - 1;
            result.items[0].box[0] = x1;
            result.items[0].box[1] = y1;
            result.items[0].box[2] = x2;
            result.items[0].box[3] = y2;
            vision_draw_save_result(&result);
        }

        SEGGER_RTT_printf(0, "[vision_det] frame #%u %dx%d box[%d,%d,%d,%d] infer %d.%d fps\n",
                          (unsigned)frame_seq, fb.width, fb.height,
                          result.items[0].box[0], result.items[0].box[1],
                          result.items[0].box[2], result.items[0].box[3],
                          fps10 / 10, fps10 % 10);
    }
}
```

- [ ] **Step 2: 编译验证**

Run: `./agentic/idf_build.sh`（超时 600000ms）
Expected: 编译通过。

---

## Task 5: vision_app.c 显示任务接入画框回调

**Files:**
- Modify: `components/vision/vision_app.c:256-265`

- [ ] **Step 1: 在锁内 draw_bitmap 之前插入画框回调**

把 `vision_display_task` 中这段（锁内 HIDDEN 复查通过后的直刷块）：

```c
        BSP_LVGL_Lock();
        if (!lv_obj_has_flag(scr_dashboard_cont_dashboard, LV_OBJ_FLAG_HIDDEN)) {
            esp_lcd_panel_draw_bitmap(BSP_LCD_GetPanelHandle(),
                                      s_preview_x,
                                      s_preview_y,
                                      s_preview_x + s_preview_w,
                                      s_preview_y + s_preview_h,
                                      s_preview_buf);
        }
        BSP_LVGL_Unlock();
```

替换为（在 draw_bitmap 前插入画框，复刻 esp-who：lcd_disp_cb 在显示同一帧时把框画进显示画布）：

```c
        BSP_LVGL_Lock();
        if (!lv_obj_has_flag(scr_dashboard_cont_dashboard, LV_OBJ_FLAG_HIDDEN)) {
            // 画框：按帧时间戳对齐取检测结果，画进 s_preview_buf（复刻 lcd_disp_cb）。
            // 放在 draw_bitmap 之前——先在缓冲上叠框，再整块直刷。纯内存操作，不碰 panel。
            vision_draw_lcd_disp_cb((uint8_t *)s_preview_buf, s_preview_w, s_preview_h, fb.timestamp);
            esp_lcd_panel_draw_bitmap(BSP_LCD_GetPanelHandle(),
                                      s_preview_x,
                                      s_preview_y,
                                      s_preview_x + s_preview_w,
                                      s_preview_y + s_preview_h,
                                      s_preview_buf);
        }
        BSP_LVGL_Unlock();
```

- [ ] **Step 2: 编译验证**

Run: `./agentic/idf_build.sh`（超时 600000ms）
Expected: 编译通过、链接成功（vision_draw 接口此时被 detect/app 引用）。

---

## Task 6: 烧录并验证画框链路

**Files:** 无（验证任务）

- [ ] **Step 1: 烧录并运行**

Run: `python3 agentic/esp_target.py flash-and-run build/ --app-only`
Expected: 烧录成功，固件启动。

- [ ] **Step 2: 启动 RTT 读取器（后台）**

Run（run_in_background）: `python3 agentic/rtt_reader.py --elf build/<project>.elf --output agentic/.esp-agent/rtt.log --kill-existing`
（`<project>` 名从 build/ 下 .elf 实际文件名确定）

- [ ] **Step 3: 读 RTT 日志确认检测侧出框**

Run: `tail -20 agentic/.esp-agent/rtt.log`
Expected: 持续打印 `[vision_det] frame #N WxH box[x1,y1,x2,y2] infer F.F fps`，box 坐标在每帧间小幅连续变化（碰壁后某一维反向）。

- [ ] **Step 4: 肉眼验证（dashboard 页）**

确认 LCD 预览区域出现**一个红色空心矩形**，在预览区内缓慢移动、碰壁反弹、不越出预览区。

- [ ] **Step 5: 肉眼验证（切页隐藏）**

切到 set/log/sys 页（dashboard 隐藏）：预览画面与框一起消失，不残留、不盖住新页面 UI（锁内 HIDDEN 复查生效）。

---

## Self-Review（写计划后自查）

**Spec 覆盖：**
- 结果结构体 → Task 1 ✓
- 结果队列 + save + 时间戳对齐 + 画框 → Task 2 ✓
- 预览尺寸暴露给检测侧 rescale → Task 3 ✓
- 假框碰壁反弹 + rescale → Task 4 ✓
- 显示任务锁内画框回调 → Task 5 ✓
- 编译 + 烧录 + RTT + 肉眼（含切页隐藏）验证 → Task 6 ✓
- 结果队列深度宏 VISION_RESULT_QUEUE_DEPTH 定义在 vision_draw.c → Task 2 ✓
- 画框颜色红、线宽 2、无文字无 keypoint → Task 2 ✓

**类型一致性：**
- `vision_det_result_t.box[4]`、`vision_det_frame_t{items,count,timestamp}` 在 Task 1 定义，Task 2/4 一致引用 ✓
- 接口签名 `vision_draw_save_result(const vision_det_frame_t*)`、`vision_draw_lcd_disp_cb(uint8_t*,int,int,int64_t)`、`vision_get_preview_size(int*,int*)` 在 Task 1 声明，Task 2/3/4/5 调用一致 ✓
- `VISION_MAX_BOXES`(Task1) 与 Task2 `items` 数组、Task4 `count=1` 一致 ✓

**占位符扫描：** 无 TBD/TODO；每个代码步骤给出完整可编译代码 ✓
