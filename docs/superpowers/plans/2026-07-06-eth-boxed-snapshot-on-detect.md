# 以太网「识别成功边沿·带框帧」实施计划

> **For agentic workers:** REQUIRED SUB-SKILL: 用 superpowers:executing-plans 逐任务实施。步骤用 `- [ ]` 复选框跟踪。
> **本项目无 pytest**：验证靠 `./agentic/idf_build.sh` 编译 + `flash-and-run` 烧录 + RTT 日志 + 主机端 5001 端口抓包（见 CLAUDE.md）。TDD 的「写失败测试」步骤相应替换为硬件在环验证。

**Goal:** 把以太网图像通道从「每 5 秒抓一帧干净原始帧」改为「每个包裹识别成功（面单+logo）的上升沿发且仅发一帧带框 JPEG」，框直接 burn-in 进图像，协议与主机端零改动。

**Architecture:** 复用 `vision_detect.c` 已有的 miss 保持状态机做包裹级上升沿检测（`s_display_has_last_hit` 由 false→true 即新包裹首次命中）。命中的那一帧在 vision 侧同帧完成「PPA 缩放到 640×375 + burn-in 画框」生成一块带框 RGB888 快照（保证图框同步），经二值信号量交给以太网任务，后者只做 JPEG 编码 + 走原有 slot 队列/协议发送。取消原 5 秒定时抓帧。

**Tech Stack:** ESP-IDF、FreeRTOS（binary semaphore + mutex）、PPA 硬件缩放、硬件 JPEG 编码器、现有 vision 帧总线与 Ethernet_app TCP 图像通道。

---

## 关键约束（实施前必读）

1. **图框必须同帧生成**：检测框 `dets[]` 是原图坐标（`fb.width×fb.height`），以太网发送的是 640×375。绝不能让以太网任务「收到信号后再抓最新帧画框」——那样包裹已移动，框图错位。正确做法是在 `vision_detect_task` 命中的那一刻，用当前 `fb.buf` 完成缩放 + 画框。
2. **坐标映射**：快照缩放比 `sx = 640/src_w, sy = 375/src_h`，逐框映射后 clip 到 `[0,639]×[0,374]`。
3. **class_id 映射**：detect 侧 category `0=极兔 1=韵达 2=中通`（见 `vision_detect.c:193`），以太网协议 `class_id` 取值 `1~3`，故传 `category+1`；`confidence` 取 logo 置信度 `result.ev.logo_confidence`（0~100）。
4. **尺寸单一真相源**：vision 侧 `BOXED_SNAPSHOT_W/H` 必须与 `Ethernet_app` 的 `SNAPSHOT_WIDTH/HEIGHT`（640×375，`ethernet_app.c:59-60`）一致，二者任一改动需同步。
5. **内存**：新增一块 640×375×3 ≈ 720KB 的 PSRAM buffer（`s_boxed_buf`）。以太网侧 take 目标复用已存在的 `s_snapshot_rgb`（干净帧路径取消后空闲），不额外增内部 SRAM。

---

## File Structure

- **Modify** `components/vision/framework/vision_internal.h` — 声明画框暴露接口 + 边沿快照 capture 接口（detect 内部调）。
- **Modify** `components/vision/framework/vision.h` — 声明 `vision_boxed_snapshot_take`（对外，以太网调）。
- **Modify** `components/vision/framework/vision_draw.c` — 暴露 `vision_draw_boxes_rgb888`（包装现有 `draw_results_on_buf`）。
- **Modify** `components/vision/framework/vision_app.c` — 边沿快照基础设施：buffer/信号量初始化、`vision_boxed_snapshot_capture`（PPA 缩放+映射+画框）、`vision_boxed_snapshot_take`。
- **Modify** `components/vision/framework/vision_detect.c` — 命中分支加上升沿检测并触发 capture。
- **Modify** `components/Ethernet_app/ethernet_app.c` — `produce_jpeg_snapshot` 换源为边沿快照；`image_producer_task` 去掉 5 秒定时改为边沿驱动。

---

## Task 1: vision_draw 暴露画框接口

**Files:**
- Modify: `components/vision/framework/vision_internal.h`（在 `vision_draw_lcd_disp_cb` 声明附近，约 :81 之后）
- Modify: `components/vision/framework/vision_draw.c`（现有 `draw_results_on_buf` 在 :209-225）

- [ ] **Step 1: 在 vision_internal.h 声明画框接口**

在 `vision_internal.h` 中 `vision_draw_lcd_disp_cb(...)` 声明（约 :81-82）之后插入：

```c
// 在任意 RGB888 缓冲上画检测框（面单绿框打底、logo 分类色框覆盖），复用显示侧同一画框逻辑。
// buf 尺寸为 w×h×3；frame->items[].box 必须已是 buf 坐标系（调用方负责缩放/clip）。
void vision_draw_boxes_rgb888(uint8_t *buf, int w, int h, const vision_det_frame_t *frame);
```

- [ ] **Step 2: 在 vision_draw.c 实现（包装现有 static）**

在 `vision_draw.c` 的 `draw_results_on_buf`（:209-225）之后、`vision_draw_lcd_disp_cb`（:227）之前插入：

```c
void vision_draw_boxes_rgb888(uint8_t *buf, int w, int h, const vision_det_frame_t *frame)
{
    if (!buf || !frame || w <= 0 || h <= 0) {
        return;
    }
    draw_results_on_buf(buf, w, h, frame);
}
```

- [ ] **Step 3: 编译验证接口可用**

Run: `./agentic/idf_build.sh`（timeout 600000）
Expected: 编译通过（此步仅新增未被调用的函数，应无警告）。

---

## Task 2: vision_app 边沿带框快照基础设施

**Files:**
- Modify: `components/vision/framework/vision.h`（对外接口声明）
- Modify: `components/vision/framework/vision_internal.h`（capture 内部接口声明）
- Modify: `components/vision/framework/vision_app.c`（include、全局、初始化、两个函数实现）

- [ ] **Step 1: vision.h 声明对外 take 接口**

在 `vision.h` 末尾（`#ifdef __cplusplus extern "C"` 块内、`#endif` 之前）插入：

```c
// 阻塞等待一张「识别成功边沿·带框」RGB888 快照并拷贝到 dst（640×375×3）。
// 该快照由 vision 检测侧在新包裹首次命中时生成（面单+logo 框已 burn-in）。
// class_id_out 取值 1~3（极兔/韵达/中通），conf_out 为 logo 置信度 0~100。
// 返回 true 表示 dst 已填入一帧；false 表示 timeout_ms 内无新包裹。
bool vision_boxed_snapshot_take(uint8_t *dst, size_t dst_capacity,
                                uint16_t *class_id_out, uint8_t *conf_out,
                                uint32_t timeout_ms);
```

- [ ] **Step 2: vision_internal.h 声明 capture 接口**

在 `vision_internal.h` 中 Task 1 新增的 `vision_draw_boxes_rgb888` 声明之后插入：

```c
// 检测侧在「新包裹识别成功」上升沿调用：用当前原图帧 src(src_w×src_h RGB888)
// 硬件缩放到 640×375，按同比例映射 dets[] 原图坐标框并 burn-in，生成带框快照，
// 经信号量交给以太网侧消费。忙于上一张消费时本次静默跳过（不阻塞检测任务）。
// dets 为原图坐标检测框数组（vision_model_det_t，含 category/score/stage）。
void vision_boxed_snapshot_capture(const uint8_t *src, int src_w, int src_h,
                                   const vision_model_det_t *dets, int det_count,
                                   uint16_t class_id, uint8_t confidence_pct);
```

> 注：`vision_internal.h` 顶部已 `#include "vision_model.h"`（见其 :20），`vision_model_det_t` 可用。

- [ ] **Step 3: vision_app.c 增加 include 与全局状态**

确认 `vision_app.c` 顶部已含 `#include "freertos/semphr.h"`；若无则加。在文件的 static 全局区（`s_snapshot_ppa` 声明附近）插入：

```c
// ---- 边沿触发带框快照（识别成功新包裹时生成，供以太网 burn-in 发送）----
// 尺寸必须与 Ethernet_app 的 SNAPSHOT_WIDTH/HEIGHT 一致（当前 640×375）。
#define BOXED_SNAPSHOT_W 640
#define BOXED_SNAPSHOT_H 375
#define BOXED_SNAPSHOT_BYTES ((size_t)BOXED_SNAPSHOT_W * BOXED_SNAPSHOT_H * 3)
static uint8_t         *s_boxed_buf;        // 640×375×3 RGB888，PSRAM，cache 对齐
static SemaphoreHandle_t s_boxed_ready;     // 二值：capture give，take 取
static SemaphoreHandle_t s_boxed_mutex;     // 保护 buffer 填充与拷出不重叠
static uint16_t         s_boxed_class_id;   // 随快照的类别 1~3
static uint8_t          s_boxed_conf;       // 随快照的 logo 置信度 0~100
```

- [ ] **Step 4: vision_app.c 在 vision_start 初始化资源**

用 Grep 定位 `vision_start` 内 `s_snapshot_ppa` 的创建处（`ppa_register_client`）。在其成功之后、创建各任务（`xTaskCreate*`）之前插入：

```c
    s_boxed_buf = heap_caps_aligned_alloc(64, BOXED_SNAPSHOT_BYTES,
                                          MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    s_boxed_ready = xSemaphoreCreateBinary();
    s_boxed_mutex = xSemaphoreCreateMutex();
    if (!s_boxed_buf || !s_boxed_ready || !s_boxed_mutex) {
        ESP_LOGE(TAG, "boxed snapshot init failed (buf=%p ready=%p mtx=%p)",
                 s_boxed_buf, s_boxed_ready, s_boxed_mutex);
        // 快照失败不阻断视觉主链路：capture/take 内部有 NULL 保护，直接放行继续。
    }
```

> 若 `vision_app.c` 未定义 `TAG`，改用该文件已有的日志宏（Grep `ESP_LOGE(` 确认现有 tag 名）。

- [ ] **Step 5: vision_app.c 实现 capture**

在 `vision_copy_latest_frame_scaled_rgb888`（:165-234）之后插入。注意此函数在检测任务上下文调用，PPA 用与该函数相同的 `s_snapshot_ppa`；两处均为 BLOCKING 模式、串行使用，PPA 事务本身互斥：

```c
void vision_boxed_snapshot_capture(const uint8_t *src, int src_w, int src_h,
                                   const vision_model_det_t *dets, int det_count,
                                   uint16_t class_id, uint8_t confidence_pct)
{
    if (!s_boxed_buf || !s_boxed_mutex || !s_boxed_ready ||
        !src || src_w <= 0 || src_h <= 0 || !s_snapshot_ppa) {
        return;
    }
    // 忙于上一张消费（take 持锁 memcpy 中）则本次跳过，绝不阻塞检测任务。
    if (xSemaphoreTake(s_boxed_mutex, 0) != pdTRUE) {
        return;
    }

    // 1) PPA 硬件缩放 src(src_w×src_h) → s_boxed_buf(640×375)，RGB888。
    ppa_srm_oper_config_t srm = {
        .in = {
            .buffer = (void *)src,
            .pic_w = src_w,
            .pic_h = src_h,
            .block_w = src_w,
            .block_h = src_h,
            .block_offset_x = 0,
            .block_offset_y = 0,
            .srm_cm = PPA_SRM_COLOR_MODE_RGB888,
        },
        .out = {
            .buffer = s_boxed_buf,
            .buffer_size = BOXED_SNAPSHOT_BYTES,
            .pic_w = BOXED_SNAPSHOT_W,
            .pic_h = BOXED_SNAPSHOT_H,
            .block_offset_x = 0,
            .block_offset_y = 0,
            .srm_cm = PPA_SRM_COLOR_MODE_RGB888,
        },
        .rotation_angle = PPA_SRM_ROTATION_ANGLE_0,
        .scale_x = (float)BOXED_SNAPSHOT_W / src_w,
        .scale_y = (float)BOXED_SNAPSHOT_H / src_h,
        .mode = PPA_TRANS_MODE_BLOCKING,
    };
    if (ppa_do_scale_rotate_mirror(s_snapshot_ppa, &srm) != ESP_OK) {
        xSemaphoreGive(s_boxed_mutex);
        return;
    }
    // PPA 输出对 CPU 画框可见：M2C 使 CPU 读到 PPA 写入的像素。
    esp_cache_msync(s_boxed_buf, BOXED_SNAPSHOT_BYTES, ESP_CACHE_MSYNC_FLAG_DIR_M2C);

    // 2) 原图坐标框 → 640×375 坐标，clip，stage/category 透传，构造临时帧。
    vision_det_frame_t tmp = {0};
    float sx = (float)BOXED_SNAPSHOT_W / src_w;
    float sy = (float)BOXED_SNAPSHOT_H / src_h;
    int kept = 0;
    for (int i = 0; i < det_count && kept < VISION_MAX_BOXES; i++) {
        int x1 = (int)(dets[i].box[0] * sx);
        int y1 = (int)(dets[i].box[1] * sy);
        int x2 = (int)(dets[i].box[2] * sx);
        int y2 = (int)(dets[i].box[3] * sy);
        if (x1 < 0) x1 = 0; if (x1 > BOXED_SNAPSHOT_W - 1) x1 = BOXED_SNAPSHOT_W - 1;
        if (y1 < 0) y1 = 0; if (y1 > BOXED_SNAPSHOT_H - 1) y1 = BOXED_SNAPSHOT_H - 1;
        if (x2 < 0) x2 = 0; if (x2 > BOXED_SNAPSHOT_W - 1) x2 = BOXED_SNAPSHOT_W - 1;
        if (y2 < 0) y2 = 0; if (y2 > BOXED_SNAPSHOT_H - 1) y2 = BOXED_SNAPSHOT_H - 1;
        tmp.items[kept].category = dets[i].category;
        tmp.items[kept].score    = dets[i].score;
        tmp.items[kept].box[0]   = x1;
        tmp.items[kept].box[1]   = y1;
        tmp.items[kept].box[2]   = x2;
        tmp.items[kept].box[3]   = y2;
        tmp.items[kept].stage    = dets[i].stage;
        kept++;
    }
    tmp.count = kept;

    // 3) burn-in 画框（面单绿框打底、logo 分类色框覆盖）。
    vision_draw_boxes_rgb888(s_boxed_buf, BOXED_SNAPSHOT_W, BOXED_SNAPSHOT_H, &tmp);
    // 画框是 CPU 写：C2M writeback，保证后续 JPEG 编码器（DMA）读到最新像素。
    esp_cache_msync(s_boxed_buf, BOXED_SNAPSHOT_BYTES,
                    ESP_CACHE_MSYNC_FLAG_DIR_C2M | ESP_CACHE_MSYNC_FLAG_UNALIGNED);

    s_boxed_class_id = class_id;
    s_boxed_conf = confidence_pct;
    xSemaphoreGive(s_boxed_mutex);
    // 二值信号量：未被消费时再次 give 保持 signaled（只留最新一张，符合边沿语义）。
    xSemaphoreGive(s_boxed_ready);
}
```

- [ ] **Step 6: vision_app.c 实现 take**

紧接 capture 之后插入：

```c
bool vision_boxed_snapshot_take(uint8_t *dst, size_t dst_capacity,
                                uint16_t *class_id_out, uint8_t *conf_out,
                                uint32_t timeout_ms)
{
    if (!s_boxed_ready || !s_boxed_mutex || !s_boxed_buf ||
        !dst || dst_capacity < BOXED_SNAPSHOT_BYTES) {
        return false;
    }
    if (xSemaphoreTake(s_boxed_ready, pdMS_TO_TICKS(timeout_ms)) != pdTRUE) {
        return false;  // 超时内无新包裹
    }
    // 持锁只做一次 memcpy 拷出，尽快释放，缩短与 capture 的竞争窗口。
    xSemaphoreTake(s_boxed_mutex, portMAX_DELAY);
    memcpy(dst, s_boxed_buf, BOXED_SNAPSHOT_BYTES);
    if (class_id_out) *class_id_out = s_boxed_class_id;
    if (conf_out)     *conf_out = s_boxed_conf;
    xSemaphoreGive(s_boxed_mutex);
    return true;
}
```

- [ ] **Step 7: 编译验证**

Run: `./agentic/idf_build.sh`（timeout 600000）
Expected: 编译通过。若报 `s_snapshot_ppa` / `ppa_srm_oper_config_t` 未定义，确认插入位置在 `vision_app.c`（该文件已用这些符号，见 :202-227）。

---

## Task 3: vision_detect 命中上升沿触发 capture

**Files:**
- Modify: `components/vision/framework/vision_detect.c`（命中分支 :220-223）

- [ ] **Step 1: 在命中分支加上升沿检测并触发**

将 `vision_detect.c` 的命中分支（当前 :220-223）：

```c
    } else if (best_waybill >= 0 && best_logo >= 0) {
        s_display_last_hit = result;
        s_display_has_last_hit = true;
        s_display_miss_count = 0;
    } else if (s_display_has_last_hit && s_display_miss_count < VISION_DISPLAY_MISS_KEEP_COUNT) {
```

替换为：

```c
    } else if (best_waybill >= 0 && best_logo >= 0) {
        // 上升沿：上一个包裹已离开（miss 超阈值 has_last_hit 归零）或从无 → 新包裹首次识别成功。
        // 仅此刻发一帧带框图；同一包裹后续持续命中 has_last_hit 已为真，不重发。
        bool rising_edge = !s_display_has_last_hit;
        s_display_last_hit = result;
        s_display_has_last_hit = true;
        s_display_miss_count = 0;
        if (rising_edge) {
            // category 0/1/2 → 协议 class_id 1/2/3；conf 取 logo 置信度。
            // 传原图坐标 dets[]+n，capture 内映射到 640×375 并 burn-in（图框同帧同步）。
            uint16_t cls = (uint16_t)(result.items[best_logo].category + 1);
            uint8_t conf = (uint8_t)result.ev.logo_confidence;
            vision_boxed_snapshot_capture(fb.buf, fb.width, fb.height, dets, n, cls, conf);
            SEGGER_RTT_printf(0, "[vision_det] BOXED SNAPSHOT captured cls=%u conf=%u boxes=%d\n",
                              (unsigned)cls, (unsigned)conf, n);
        }
    } else if (s_display_has_last_hit && s_display_miss_count < VISION_DISPLAY_MISS_KEEP_COUNT) {
```

> `dets`、`n`、`fb`、`best_logo`、`result.ev.logo_confidence` 均在同函数作用域已定义（见 :127-131、:139、:179-183、:186）。`vision_internal.h` 已被本文件 include（:32），capture 声明可见。

- [ ] **Step 2: 编译验证**

Run: `./agentic/idf_build.sh`（timeout 600000）
Expected: 编译通过。

---

## Task 4: 以太网改为边沿驱动消费

**Files:**
- Modify: `components/Ethernet_app/ethernet_app.c`
  - `produce_jpeg_snapshot`（:390-447）换源
  - `image_producer_task`（:747-814）去 5 秒定时
  - include vision 对外头（确认 `vision.h` 已被引用）

- [ ] **Step 1: produce_jpeg_snapshot 换源为边沿带框快照**

将 `ethernet_app.c` 的 `produce_jpeg_snapshot`（:390-447）整体替换为：

```c
static esp_err_t produce_jpeg_snapshot(image_slot_t *slot)
{
    int64_t start_ms = monotonic_ms();
    uint16_t class_id = 1;
    uint8_t conf = 0;

    // 阻塞等一张「识别成功新包裹」带框快照（vision 侧同帧缩放+画框已完成）。
    // 1s 超时：无新包裹则返回 NOT_FOUND，由 producer 记 no_frame 并继续等。
    if (!vision_boxed_snapshot_take(s_snapshot_rgb, SNAPSHOT_RGB_BYTES,
                                    &class_id, &conf, 1000)) {
        return ESP_ERR_NOT_FOUND;
    }

    int encoded = 0;
    jpeg_enc_set_quality(s_jpeg_enc, JPEG_QUALITY);
    jpeg_error_t err = jpeg_enc_process(s_jpeg_enc,
                                        s_snapshot_rgb,
                                        (int)SNAPSHOT_RGB_BYTES,
                                        slot->jpeg,
                                        JPEG_OUTBUF_BYTES,
                                        &encoded);
    if (err != JPEG_ERR_OK || encoded <= 0) {
        ESP_LOGW(TAG, "jpeg encode failed err=%d encoded=%d", (int)err, encoded);
        return ESP_FAIL;
    }

    slot->jpeg_len = (size_t)encoded;
    slot->created_ms = monotonic_ms();
    slot->encode_ms = (int)(slot->created_ms - start_ms);
    slot->src_w = SNAPSHOT_WIDTH;
    slot->src_h = SNAPSHOT_HEIGHT;
    if (class_id < 1 || class_id > 3) {
        class_id = 1;
    }
    if (conf > 100) {
        conf = 100;
    }
    slot->class_id = class_id;
    slot->confidence_pct = conf;
    return ESP_OK;
}
```

> 移除了对 `vision_copy_latest_frame_scaled_rgb888` 与 `vision_get_latest_classification` 的调用——class/conf 现随快照来自命中那一刻，避免二次时序错位。

- [ ] **Step 2: image_producer_task 去掉 5 秒定时**

将 `image_producer_task`（:747-785，即到 `produce_jpeg_snapshot` 调用前）中的定时逻辑替换。原 :750、:754-759 的 `next_image_ms` 段删除，函数头部到取 slot 段改为：

```c
static void image_producer_task(void *arg)
{
    (void)arg;

    while (true) {
        xEventGroupWaitBits(s_eth_events, TCP_CONNECT_READY_BIT, pdFALSE, pdTRUE, portMAX_DELAY);

        EventBits_t bits = xEventGroupGetBits(s_eth_events);
        if ((bits & TCP_IMAGE_CONNECTED_BIT) == 0) {
            vTaskDelay(pdMS_TO_TICKS(200));
            continue;
        }
        if (ensure_image_resources() != ESP_OK) {
            if (xSemaphoreTake(s_image_lock, pdMS_TO_TICKS(50)) == pdTRUE) {
                s_image_stats.encode_fail++;
                xSemaphoreGive(s_image_lock);
            }
            vTaskDelay(pdMS_TO_TICKS(1000));
            continue;
        }

        int64_t now_ms = monotonic_ms();
        image_slot_t *slot = NULL;
        if (xSemaphoreTake(s_image_lock, portMAX_DELAY) == pdTRUE) {
            queue_drop_stale_locked(now_ms);
            slot = queue_find_free_locked();
            if (!slot) {
                s_image_stats.dropped_backpressure++;
            }
            xSemaphoreGive(s_image_lock);
        }
        if (!slot) {
            vTaskDelay(pdMS_TO_TICKS(100));   // 队列满，让发送任务腾位
            continue;
        }

        esp_err_t ret = produce_jpeg_snapshot(slot);   // 内部阻塞等边沿快照（1s 超时）
```

其后从 `if (xSemaphoreTake(s_image_lock, portMAX_DELAY) == pdTRUE) {`（原 :788 起）到函数结尾的 slot 标记/统计逻辑**保持不变**。

- [ ] **Step 3: 清理未用的定时宏引用**

`TCP_IMAGE_INTERVAL_MS`（:49）现已无引用。保留宏定义无害，但删除其唯一引用点即可（Step 2 已删）。Grep 确认 `TCP_IMAGE_INTERVAL_MS` 在 `ethernet_app.c` 中已无残留引用：

Run: `grep -n TCP_IMAGE_INTERVAL_MS components/Ethernet_app/ethernet_app.c`（用 Grep 工具）
Expected: 仅剩 :49 的 `#define`，无其他引用。

- [ ] **Step 4: 确认 include**

确认 `ethernet_app.c` 顶部已 `#include "vision.h"`（原代码调用 `vision_copy_latest_frame_scaled_rgb888`/`vision_get_latest_classification` 应已 include）。`vision_boxed_snapshot_take` 声明在 `vision.h`，无需新增 include。若这两个函数原来声明在别处，补 `#include "vision.h"`。

- [ ] **Step 5: 编译验证**

Run: `./agentic/idf_build.sh`（timeout 600000）
Expected: 编译通过，无「隐式声明 vision_boxed_snapshot_take」警告。

---

## Task 5: 硬件在环验证

**Files:** 无（仅烧录 + 观测）

- [ ] **Step 1: 全量/增量编译**

Run: `./agentic/idf_build.sh`（timeout 600000）
Expected: `Project build complete`，生成 `build/*.elf` / `*.bin`。

- [ ] **Step 2: 烧录并运行**

Run: `python3 agentic/esp_target.py flash-and-run build/ --app-only`
Expected: 烧录成功，固件启动。

- [ ] **Step 3: 启动 RTT 读取器（后台）**

Run（run_in_background）: `python3 agentic/rtt_reader.py --elf build/<project>.elf --output agentic/.esp-agent/rtt.log --kill-existing`
（`<project>` 用 `build/*.elf` 实际名）

- [ ] **Step 4: 观测边沿触发日志**

在相机视野内放入一个包裹（面单+logo 可识别），移走，再放入第二个包裹。等 3-5 秒后读日志：

Run: `tail -40 agentic/.esp-agent/rtt.log`
Expected:
- 每个包裹**首次**识别成功时出现一行 `[vision_det] BOXED SNAPSHOT captured cls=.. conf=.. boxes=..`；
- 同一包裹持续在视野内的后续帧**不再**出现该行（上升沿只触发一次）；
- 包裹移走再放回新包裹，再次出现该行。

- [ ] **Step 5: 主机端抓包验证（5001 图像通道）**

在主机（192.168.10.1）用现有图像通道客户端连 5001，Expected：
- 仅在包裹识别成功边沿收到一帧 JPEG（不再有每 5 秒的空帧节奏）；
- 解码 JPEG 后图上有**面单绿框 + logo 分类色框**（极兔红/韵达黄/中通蓝），框与包裹位置对齐（无明显错位）；
- 40 字节包头 `reserved` = class_id（1~3）、`reserved2` = confidence（0~100），与 RTT 打印的 cls/conf 一致；
- 同一包裹只收到一帧，不重复。

- [ ] **Step 6: 背压/断连回归**

- 图像通道未连接时放入包裹：RTT 有 capture 日志但主机无帧，重连后正常（binary sem 只保留最新一张，重连后可能先收到最近一张，符合预期）。
- 连续快速通过多个包裹：观察 `s_image_stats.dropped_backpressure`（通过控制通道 5000 的指标 JSON）是否合理，队列深度 2 溢出时丢弃计数增加，不崩溃。

---

## Self-Review

**Spec 覆盖：**
- 「识别成功(面单+logo)边沿触发」→ Task 3 上升沿检测（复用 `best_waybill>=0 && best_logo>=0` + `!s_display_has_last_hit`）。✓
- 「一个包裹发一次，重复识别不重发」→ `has_last_hit` 真值门控 + miss 阈值归零机制。✓
- 「带框帧 burn-in」→ Task 2 capture 内 PPA 缩放 + `vision_draw_boxes_rgb888`。✓
- 「面单+logo 全部框」→ 复用 `draw_results_on_buf`（面单绿框 + logo 分类色框）。✓
- 「完全替换 5 秒定时」→ Task 4 去 `next_image_ms`，producer 阻塞等边沿。✓
- 「协议/主机端零改动」→ 仍走原 slot 队列、40 字节包头、`reserved`/`reserved2` 复用。✓

**类型一致性：** `vision_boxed_snapshot_capture`（Task 2 声明 / Task 3 调用）签名一致：`(const uint8_t*, int, int, const vision_model_det_t*, int, uint16_t, uint8_t)`。`vision_boxed_snapshot_take`（Task 2 声明 / Task 4 调用）一致：`(uint8_t*, size_t, uint16_t*, uint8_t*, uint32_t)`。`vision_draw_boxes_rgb888`（Task 1 定义 / Task 2 调用）一致。`BOXED_SNAPSHOT_W/H`(640/375) = `SNAPSHOT_WIDTH/HEIGHT`。✓

**Placeholder 扫描：** 无 TBD/TODO；每处代码步骤含完整代码；`<project>` 为运行期 elf 实际名（非占位，Step 3/4 已说明取 `build/*.elf`）；vision_start 插入点用 `s_snapshot_ppa`/`ppa_register_client` 锚点（精确可 Grep 定位）。✓
