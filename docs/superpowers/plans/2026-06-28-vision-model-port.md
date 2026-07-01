# 移植旧识别模块到新零拷贝架构（接入真实模型）实施计划

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** 把旧项目 `E:\test1_black\components\vision_app` 的真实识别内核（ROI 预定位 + ESP-DL 推理 + 后处理）移植到新项目 `components/vision/`，替换假框桩，跑通快递 logo（极兔/中通/韵达）三类检测。

**Architecture:** 旧 C++ 推理核（AppYOLO/yolo_decode）+ 纯 C 的 ROI（roi_algorithm/roi_tuning）原样搬入 `components/vision/`；新建 C 封装层 `vision_model.cpp/.h` 把 C++ AppYOLO 包成 C 接口；纯 C 的 `vision_detect.c` 把假框块换成 `vision_model_run` 调用，其余 peek→rescale→save→UI 链路不动。分区表重排腾出 11M storage 打包全部 19 个模型，detect 任务栈扩到 32KB 放 PSRAM。

**Tech Stack:** ESP-IDF (P4)、esp-dl (managed_components/espressif__esp-dl)、SPIFFS、PPA、FreeRTOS、JTAG/RTT 验证。

---

## 重要约定（嵌入式适配）

本项目**非 git 仓库**、**无单元测试框架**，固件跑在 ESP32-P4 实物上。因此本计划的验证手段不是
`pytest`/`git commit`，而是：

- **编译**：`./agentic/idf_build.sh`（超时设 600000ms / 10 分钟）
- **烧录**：`python3 agentic/esp_target.py flash-and-run build/ --app-only`（改分区表的任务用全擦+全量烧录）
- **看日志**：后台跑 `rtt_reader.py` 写 `agentic/.esp-agent/rtt.log`，`tail` 读取
- 每个任务以**「编译通过」**为最小检查点；涉及运行行为的任务额外加**「烧录+RTT 验证」**

每个任务末尾的"检查点"等价于 TDD 里的"跑测试"——必须达成才算完成，未过不进下一任务。

---

## 文件结构

| 文件 | 责任 |
| --- | --- |
| `partitions.csv`（改） | factory 4.5M + storage 11M 重排 |
| `sdkconfig`（改） | `CONFIG_SPIFFS_OBJ_NAME_LEN` 32→64（容纳超 32 字节的中文模型名） |
| `CMakeLists.txt`（顶层，改） | 取消注释 `spiffs_create_partition_image` |
| `components/vision/CMakeLists.txt`（改） | 加 6 个搬入源 + vision_model.cpp，加 esp-dl 等 REQUIRES，加 -O3 |
| `components/vision/vision_app.c`（改） | detect 任务栈 32KB + PSRAM；vision_start 内调 vision_model_init |
| `components/vision/vision_detect.c`（改） | 假框块换 vision_model_run；UI 填类别/耗时/三类概率 |
| `components/vision/app_yolo.cpp` + `include/app_yolo.hpp`（搬入） | C++ 推理核 |
| `components/vision/yolo_decode.cpp` + `include/yolo_decode.hpp`（搬入） | ESPDet-Pico 解码 |
| `components/vision/roi_algorithm.c` + `include/roi_algorithm.h`（搬入） | 纯 C ROI 预定位 |
| `components/vision/roi_tuning.cpp` + `include/roi_tuning.h`（搬入） | CAL 阈值校准（暂不接线） |
| `components/vision/vision_model.cpp` + `include/vision_model.h`（新建） | C 封装层 |

---

### Task 1：搬入旧文件（不改 CMake，先就位）

**Files:**
- Copy: `E:\test1_black\components\vision_app\app_yolo.cpp` → `components/vision/app_yolo.cpp`
- Copy: `app_yolo.hpp` → `components/vision/include/app_yolo.hpp`
- Copy: `yolo_decode.cpp` → `components/vision/yolo_decode.cpp`
- Copy: `yolo_decode.hpp` → `components/vision/include/yolo_decode.hpp`
- Copy: `roi_algorithm.c` → `components/vision/roi_algorithm.c`
- Copy: `roi_algorithm.h` → `components/vision/include/roi_algorithm.h`
- Copy: `roi_tuning.cpp` → `components/vision/roi_tuning.cpp`
- Copy: `roi_tuning.h` → `components/vision/include/roi_tuning.h`

- [ ] **Step 1: 复制 8 个文件到位**

```bash
SRC=E:/test1_black/components/vision_app
DST=E:/ESP32P4_Detection/components/vision
cp "$SRC/app_yolo.cpp"      "$DST/app_yolo.cpp"
cp "$SRC/yolo_decode.cpp"   "$DST/yolo_decode.cpp"
cp "$SRC/roi_algorithm.c"   "$DST/roi_algorithm.c"
cp "$SRC/roi_tuning.cpp"    "$DST/roi_tuning.cpp"
cp "$SRC/include/app_yolo.hpp"     "$DST/include/app_yolo.hpp"
cp "$SRC/include/yolo_decode.hpp"  "$DST/include/yolo_decode.hpp"
cp "$SRC/include/roi_algorithm.h"  "$DST/include/roi_algorithm.h"
cp "$SRC/include/roi_tuning.h"     "$DST/include/roi_tuning.h"
```

- [ ] **Step 2: 检查点 — 确认 8 文件就位**

```bash
ls -la E:/ESP32P4_Detection/components/vision/*.{c,cpp} E:/ESP32P4_Detection/components/vision/include/*.h*
```
Expected: 列出 app_yolo.cpp/yolo_decode.cpp/roi_algorithm.c/roi_tuning.cpp 及 4 个头文件，连同原有
vision_app.c/vision_detect.c/vision_draw.c/vision.h/vision_internal.h。此任务**不编译**（CMake 未引用，
编译不会纳入；vision_model 尚未建）。

---

### Task 2：新建 C 封装层 `vision_model.h` / `vision_model.cpp`

**Files:**
- Create: `components/vision/include/vision_model.h`
- Create: `components/vision/vision_model.cpp`

- [ ] **Step 1: 写 `vision_model.h`（纯 C 头）**

```c
/*
 * vision_model —— C 封装层：把 C++ AppYOLO 包成纯 C 接口，供 vision_detect.c(纯 C) 调用。
 * 持 static AppYOLO*，挂载 SPIFFS /storage，加载默认 .espdl 模型。
 */
#pragma once
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// 单框检测结果（原图坐标系）。
typedef struct {
    int   category;   // 类别索引：0=极兔 1=中通 2=韵达
    float score;      // 置信度 0~1
    int   box[4];     // 左上x,左上y,右下x,右下y
} vision_model_det_t;

// 挂载 SPIFFS(/storage) + new AppYOLO(224x224,3类,conf0.25) + load_model(默认模型)。
// 必须在 vision_start 单线程阶段、detect 任务创建前调用。返回 false=失败。
bool vision_model_init(void);

// 对原图 RGB888 跑一次完整检测（内部 ROI→letterbox→量化→model->run→decode）。
// 输出原图坐标系框到 out[0..max_out)，返回框数(0~max_out)；-1=推理失败。
int  vision_model_run(const uint8_t *rgb888, int w, int h,
                      vision_model_det_t *out, int max_out);

// 上次 vision_model_run 的推理耗时(ms)，供 UI infer_time_ms。
int  vision_model_last_infer_ms(void);

// 上次三类概率×100（极兔/中通/韵达），供 UI jt/zt/yd 字段；无结果填 0。
void vision_model_get_class_probs(int *jt, int *zt, int *yd);

#ifdef __cplusplus
}
#endif
```

- [ ] **Step 2: 写 `vision_model.cpp`（C++ 实现，extern "C" 暴露）**

```cpp
/*
 * vision_model —— C 封装层实现。挂 SPIFFS、建 AppYOLO、跑 detect、把 std::vector<Detection>
 * 转成 C 数组吐出。C++ 类型不泄漏到 vision_detect.c。
 */
#include "vision_model.h"

#include <vector>
#include "app_yolo.hpp"
#include "esp_log.h"
#include "esp_spiffs.h"
#include "esp_timer.h"

static const char *TAG = "vision_model";

// 默认参数（沿用旧项目 vision_app.cpp 的值）。
#define VISION_MODEL_PATH    "/storage/train-11-zt-explore.espdl"
#define VISION_MODEL_INPUT_W 224
#define VISION_MODEL_INPUT_H 224
#define VISION_MODEL_CLASSES 3
#define VISION_MODEL_CONF    0.25f

static AppYOLO *s_yolo = nullptr;
static int      s_last_infer_ms = 0;
static int      s_prob_x100[3]  = {0, 0, 0};  // jt/zt/yd ×100

static esp_err_t mount_storage(void)
{
    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/storage",
        .partition_label = "storage",
        .max_files = 4,
        .format_if_mount_failed = false,
    };
    esp_err_t ret = esp_vfs_spiffs_register(&conf);
    if (ret == ESP_ERR_INVALID_STATE) {
        ESP_LOGW(TAG, "SPIFFS already mounted");
        return ESP_OK;
    }
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "SPIFFS mount failed: %s", esp_err_to_name(ret));
        return ret;
    }
    size_t total = 0, used = 0;
    if (esp_spiffs_info(conf.partition_label, &total, &used) == ESP_OK) {
        ESP_LOGI(TAG, "SPIFFS mounted: %u/%u KB",
                 (unsigned)(used / 1024), (unsigned)(total / 1024));
    }
    return ESP_OK;
}

extern "C" bool vision_model_init(void)
{
    if (mount_storage() != ESP_OK) {
        return false;
    }
    YOLOConfig cfg;
    cfg.input_w = VISION_MODEL_INPUT_W;
    cfg.input_h = VISION_MODEL_INPUT_H;
    cfg.num_classes = VISION_MODEL_CLASSES;
    cfg.conf_threshold = VISION_MODEL_CONF;

    s_yolo = new AppYOLO(cfg);
    if (!s_yolo) {
        ESP_LOGE(TAG, "AppYOLO alloc failed");
        return false;
    }
    if (!s_yolo->load_model(VISION_MODEL_PATH)) {
        ESP_LOGE(TAG, "load_model %s failed", VISION_MODEL_PATH);
        delete s_yolo;
        s_yolo = nullptr;
        return false;
    }
    ESP_LOGI(TAG, "model loaded: %s (%dx%d, %d cls, conf %.2f)",
             VISION_MODEL_PATH, cfg.input_w, cfg.input_h,
             cfg.num_classes, cfg.conf_threshold);
    return true;
}

extern "C" int vision_model_run(const uint8_t *rgb888, int w, int h,
                                vision_model_det_t *out, int max_out)
{
    if (!s_yolo || !rgb888 || !out || max_out <= 0) {
        return -1;
    }
    std::vector<Detection> dets;
    dets.reserve(max_out);
    int64_t t0 = esp_timer_get_time();
    bool ok = s_yolo->detect(rgb888, w, h, dets);
    s_last_infer_ms = (int)((esp_timer_get_time() - t0) / 1000);
    if (!ok) {
        return -1;
    }
    // 取三类概率（×100）供 UI；用 perf 的 class_prob。
    YOLOPerfStats perf = s_yolo->get_last_perf_stats();
    for (int i = 0; i < 3 && i < YOLOPerfStats::kClassProbCount; i++) {
        s_prob_x100[i] = (int)(perf.class_prob[i] * 100.0f);
    }
    int n = 0;
    for (size_t i = 0; i < dets.size() && n < max_out; i++) {
        out[n].category = dets[i].class_id;
        out[n].score    = dets[i].confidence;
        out[n].box[0]   = (int)dets[i].x1;
        out[n].box[1]   = (int)dets[i].y1;
        out[n].box[2]   = (int)dets[i].x2;
        out[n].box[3]   = (int)dets[i].y2;
        n++;
    }
    return n;
}

extern "C" int vision_model_last_infer_ms(void)
{
    return s_last_infer_ms;
}

extern "C" void vision_model_get_class_probs(int *jt, int *zt, int *yd)
{
    if (jt) *jt = s_prob_x100[0];
    if (zt) *zt = s_prob_x100[1];
    if (yd) *yd = s_prob_x100[2];
}
```

- [ ] **Step 3: 检查点 — 仅语法自查（此任务不单独编译，Task 4 改 CMake 后统一编译）**

确认 `vision_model.h` 无 C++ 关键字泄漏（纯 C 可包含）；`vision_model.cpp` 所有导出函数带
`extern "C"`；`#include "app_yolo.hpp"` 路径在 `include/` 下。本任务无法独立编译——CMake 尚未引用，
留待 Task 4 全量编译验证。

---

### Task 3：改分区表 + SPIFFS 名长 + 顶层打包

**Files:**
- Modify: `partitions.csv`
- Modify: `sdkconfig`（`CONFIG_SPIFFS_OBJ_NAME_LEN`）
- Modify: `CMakeLists.txt:10`（顶层）

- [ ] **Step 1: 重排 `partitions.csv`**

把 factory/storage 两行改为（其余行不动）：

```
factory,  app,  factory, 0x10000,  4608K,
storage,  data, spiffs,  0x490000, 11264K,
```
说明：factory 4.5M=4608K，storage 起于 0x490000(=0x10000+4608K)，11M=11264K，止于 0xf90000，
不越 16M 边界。

- [ ] **Step 2: 提高 SPIFFS 对象名长度上限（容纳 37 字节中文模型名）**

`sdkconfig` 内把 `CONFIG_SPIFFS_OBJ_NAME_LEN=32` 改为：
```
CONFIG_SPIFFS_OBJ_NAME_LEN=64
```
（最长模型名 `图片总和_roi_ESP32_light_ok.espdl` = 37 字节，64 留足余量。）

- [ ] **Step 3: 取消顶层打包注释**

`CMakeLists.txt` 第 10 行去掉前导 `# `：
```cmake
spiffs_create_partition_image(storage model FLASH_IN_PROJECT)
```

- [ ] **Step 4: 检查点 — 确认改动**

```bash
grep -nE "factory|storage" E:/ESP32P4_Detection/partitions.csv
grep -n "SPIFFS_OBJ_NAME_LEN" E:/ESP32P4_Detection/sdkconfig
grep -n "spiffs_create_partition_image" E:/ESP32P4_Detection/CMakeLists.txt
```
Expected: partitions.csv 显示 4608K/0x490000/11264K；OBJ_NAME_LEN=64；打包行无前导 `#`。
此任务不编译（Task 4 统一编）。

---

### Task 4：改 vision 组件 CMake + 全量编译

**Files:**
- Modify: `components/vision/CMakeLists.txt`

- [ ] **Step 1: 重写 `components/vision/CMakeLists.txt`**

```cmake
idf_component_register(
    SRCS
        "vision_app.c"
        "vision_detect.c"
        "vision_draw.c"
        "app_yolo.cpp"
        "yolo_decode.cpp"
        "roi_algorithm.c"
        "roi_tuning.cpp"
        "vision_model.cpp"
    INCLUDE_DIRS
        "include"
    REQUIRES
        bsp
        UI
        esp_driver_ppa
        esp_lcd
        system_monitor
        esp-dl
        esp_timer
        spiffs
        esp_psram
        esp_mm
)

target_compile_options(${COMPONENT_LIB} PRIVATE -ffast-math -O3)
```

- [ ] **Step 2: 检查点 — 全量编译（首次接 esp-dl，3~10 分钟，超时 600000ms）**

Run:
```bash
./agentic/idf_build.sh
```
Expected: 编译成功，无报错。重点排查：
- esp-dl 头/符号找到（REQUIRES esp-dl 生效）
- app_yolo/yolo_decode/roi_* 编译通过（旧代码原样，应无问题）
- vision_model.cpp C/C++ 边界无链接错误
- `vision_detect.c` 此时仍是假框（未改），但已能 include vision_model.h（仅声明，未调用，编译 OK）

若 esp-dl 相关报 "undefined reference" 或头找不到，先确认 `managed_components/espressif__esp-dl`
存在且 `main/idf_component.yml` 已声明依赖（探索阶段已确认存在）。

---

### Task 5：detect 任务栈扩容 + 调 vision_model_init

**Files:**
- Modify: `components/vision/vision_app.c:46-47`（栈宏）
- Modify: `components/vision/vision_app.c:430` 附近（vision_draw_init 后加 vision_model_init）
- Modify: `components/vision/vision_app.c:462-464`（detect 任务创建改 WithCaps）

- [ ] **Step 1: 栈宏 4096→32768**

`vision_app.c` 把：
```c
#define VISION_DET_STACK_SIZE 4096
```
改为：
```c
#define VISION_DET_STACK_SIZE 32768
```

- [ ] **Step 2: 顶部加 include**

`vision_app.c` 的 include 区（`#include "vision_internal.h"` 后）加：
```c
#include "vision_model.h"           // C 封装层：模型初始化与推理
#include "freertos/idf_additions.h" // xTaskCreatePinnedToCoreWithCaps
#include "esp_heap_caps.h"          // 已有则不重复
```
（注意 `esp_heap_caps.h` 文件已 include，勿重复添加。）

- [ ] **Step 3: vision_draw_init 成功后加模型初始化**

在 `vision_start` 内、`vision_draw_init()` 成功分支之后、`vision_frame_subscribe()` 之前插入：
```c
    // 初始化模型（挂 SPIFFS + 建 AppYOLO + 加载默认 .espdl）。须在 detect 任务创建前的
    // 单线程阶段做——detect 任务首帧即调 vision_model_run，模型须已就绪。
    if (!vision_model_init()) {
        ESP_LOGE(TAG, "vision_model_init failed");
        return ESP_ERR_INVALID_STATE;
    }
```

- [ ] **Step 4: detect 任务创建改 WithCaps（栈放 PSRAM）**

把原 detect 任务创建：
```c
    ok = xTaskCreatePinnedToCore(
        vision_detect_task, "vision_det",
        VISION_DET_STACK_SIZE, det_eg, VISION_DET_PRIORITY, NULL, 1);
```
改为：
```c
    ok = xTaskCreatePinnedToCoreWithCaps(
        vision_detect_task, "vision_det",
        VISION_DET_STACK_SIZE, det_eg, VISION_DET_PRIORITY, NULL, 1,
        MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
```
（fetch/display 任务不动，仍用普通 xTaskCreatePinnedToCore。）

- [ ] **Step 5: 检查点 — 增量编译**

Run:
```bash
./agentic/idf_build.sh
```
Expected: 编译成功。`xTaskCreatePinnedToCoreWithCaps` 来自 `freertos/idf_additions.h`，
返回 pdPASS/pdFAIL 同 xTaskCreate，原 `if (ok != pdPASS)` 判断不变。

---

### Task 6：vision_detect.c 假框块换真实推理

**Files:**
- Modify: `components/vision/vision_detect.c:77-108`（假框块）
- Modify: `components/vision/vision_detect.c:99-161`（结果填充与 UI 投递改多框）
- Modify: `components/vision/vision_detect.c` 顶部（去假框宏，加 include）

- [ ] **Step 1: 顶部 include 加 vision_model.h，删假框宏**

`vision_detect.c` 的 include 区加：
```c
#include "vision_model.h"   // C 封装层：vision_model_run / 耗时 / 三类概率
```
删除假框相关宏（`FAKE_BOX_W/H/STEP`）。

- [ ] **Step 2: 删假框状态变量**

删 `vision_detect_task` 内假框状态（`box_x/box_y/vx/vy/box_inited` 及其首帧初始化、碰壁反弹整段）。
保留 `frame_seq`、fps 窗口平均相关变量（`fps_win_start_us`/`fps_win_frames`/`fps10`）不动。

- [ ] **Step 3: 假框块（行 77-108 之间）整体替换为真实推理 + 多框 rescale**

把从 `// ===== 模型接入点` 到 `vision_draw_save_result(&result);` 之间的逻辑替换为：

```c
        // ===== 真实推理：原图 RGB888 → ROI/前处理/model->run/后处理 → 原图坐标框 =====
        vision_model_det_t dets[VISION_MAX_BOXES];
        int n = vision_model_run(fb.buf, fb.width, fb.height, dets, VISION_MAX_BOXES);
        if (n < 0) {
            n = 0;  // 推理失败按无目标处理
        }

        // rescale 原图坐标 → 预览坐标，逐框 clip（复刻 rescale_detect_result + limit_box）。
        vision_det_frame_t result = {0};
        result.timestamp = fb.timestamp;
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
                if (x1 < 0) x1 = 0; if (x1 > pw - 1) x1 = pw - 1;
                if (y1 < 0) y1 = 0; if (y1 > ph - 1) y1 = ph - 1;
                if (x2 < 0) x2 = 0; if (x2 > pw - 1) x2 = pw - 1;
                if (y2 < 0) y2 = 0; if (y2 > ph - 1) y2 = ph - 1;
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
        vision_draw_save_result(&result);
```

- [ ] **Step 4: UI 投递块改真实数据（类别名/耗时/三类概率）**

把原 `#if 1 ... #endif` 的 UI 投递块替换为：

```c
#if 1   /* DEBUG: 置 0 可临时禁用 UI 投递以测基线 CPU */
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
            // 类别名映射：0=极兔 1=中通 2=韵达（默认，待实测核对）
            static const char *kClassName[3] = {"极兔", "中通", "韵达"};
            int cat = result.items[best].category;
            if (cat >= 0 && cat < 3) {
                strcpy(ev.company, kClassName[cat]);
            } else {
                strcpy(ev.company, "--");
            }
        } else {
            strcpy(ev.status, "无目标");
            ev.confidence = 0;
            strcpy(ev.company, "--");
        }
        ev.fps_x10 = fps10;
        ev.infer_time_ms = vision_model_last_infer_ms();
        // 三类概率×100（极兔/中通/韵达），A/M 暂同填概率值（占位，后续可分平均/峰值）
        int jt = 0, zt = 0, yd = 0;
        vision_model_get_class_probs(&jt, &zt, &yd);
        ev.jt_a = jt; ev.jt_m = jt;
        ev.zt_a = zt; ev.zt_m = zt;
        ev.yd_a = yd; ev.yd_m = yd;

        BSP_LVGL_Lock();
        send_event(get_current_event_table(), EVT_VISION, EVT_VISION_RESULT_CHANGED,
                   (uint8_t *)&ev, 0);
        BSP_LVGL_Unlock();
#endif
```

注意：原假框版的 UI 投递在 `if (pw>0...)` 块内。改后 UI 投递应**移到该块外**（无论 rescale 是否成功
都投递，无目标也要刷新 UI）。把 UI 块放在 `vision_draw_save_result(&result);` 之后、与 rescale 块同级。

- [ ] **Step 5: 改 RTT 日志行（多框安全）**

把原引用 `result.items[0].box[...]` 的 SEGGER_RTT_printf 改为：
```c
        SEGGER_RTT_printf(0, "[vision_det] frame #%u %dx%d boxes=%d infer=%dms %d.%d fps\n",
                          (unsigned)frame_seq, fb.width, fb.height,
                          result.count, vision_model_last_infer_ms(),
                          fps10 / 10, fps10 % 10);
```
（原代码无条件读 `items[0]`，count=0 时是脏读；改为打印 count + 耗时更安全。）

- [ ] **Step 6: 检查点 — 增量编译**

Run:
```bash
./agentic/idf_build.sh
```
Expected: 编译成功。确认 `VISION_MAX_BOXES`（vision_internal.h 定义=8）足够；`ev.jt_a` 等字段名与
`components/UI/sdk/evt.h` 一致（探索阶段已确认存在 jt_a/jt_m/zt_a/zt_m/yd_a/yd_m）。

---

### Task 7：全擦烧录 + 实物 RTT 验证

**Files:** 无（纯烧录/验证）

- [ ] **Step 1: 全擦 flash（分区表变了，必须全擦）**

Run:
```bash
python3 agentic/esp_target.py erase
```
Expected: 擦除成功。改了分区表布局，不能只烧 app。

- [ ] **Step 2: 全量烧录（bootloader+分区表+app+storage 模型镜像）**

Run:
```bash
python3 agentic/esp_target.py flash build/
```
Expected: 烧录成功，storage 分区写入 ~10.7M 模型镜像（耗时较长）。

- [ ] **Step 3: 启动 RTT 读取器（后台）**

Run（run_in_background）:
```bash
python3 agentic/rtt_reader.py --elf build/sample_project.elf --output agentic/.esp-agent/rtt.log --kill-existing
```

- [ ] **Step 4: 检查点 A — 模型加载成功**

等 3 秒后读日志：
```bash
tail -40 agentic/.esp-agent/rtt.log
```
Expected: 出现
- `SPIFFS mounted: <used>/<total> KB`（total ≈ 11M）
- `model loaded: /storage/train-11-zt-explore.espdl (224x224, 3 cls, conf 0.25)`
- 无 `load_model ... failed` / `SPIFFS mount failed`

若 SPIFFS 挂载失败 → 查分区表是否生效、storage 镜像是否烧入；
若 load_model 失败 → 查模型文件名是否因 OBJ_NAME_LEN 被截断（确认 Task 3 Step 2 生效）。

- [ ] **Step 5: 检查点 B — 推理跑帧 + 栈未溢出**

```bash
tail -20 agentic/.esp-agent/rtt.log
```
Expected: 持续出现 `[vision_det] frame #N WxH boxes=K infer=Mms X.X fps`，infer 有合理毫秒值。
**无** `Stack canary` / `stack overflow` / panic 回溯（确认 32KB PSRAM 栈足够）。
若崩 → halt + cpu-regs + GDB bt 查栈溢出或空指针。

- [ ] **Step 6: 检查点 C — 实物画框 + UI 显示**

对准快递 logo（极兔/中通/韵达任一）。
Expected:
- LCD 预览区画出检测框，位置贴合 logo
- dashboard UI：status="识别成功"、company 显示类别中文名、confidence/fps/infer_ms/三类概率有值
- 若框位置整体偏移/镜像 → 查 BGR→RGB swap 或 rescale 比例
- 若类别名错位（如极兔显示成中通）→ 调 vision_detect.c Step 4 的 kClassName 映射 + vision_model 概率索引

- [ ] **Step 7: 检查点 D — 零拷贝竞态观察**

观察是否有花屏/错帧（推理读 fb.buf 期间被 fetch 回收重填）。
Expected: 画面稳定。若花屏 → 在 `vision_model_run` 入口先 `memcpy` 一份 fb.buf 再处理（设计文档已列此风险）。
此步为观察项，无问题则移植完成。

---

## 自查（Self-Review）

**1. Spec 覆盖**：设计文档 §4 文件落位→Task1+2；§5 C 封装接口→Task2；§6 数据流→Task5+6；
§7 类别映射→Task6 Step4；§8 CMake→Task3+4；§9 分区→Task3；§3 两硬约束→Task3(分区)+Task5(栈)；
§10 验证计划→Task7。新增发现（模型名超 32 字节）→Task3 Step2。全覆盖。

**2. Placeholder 扫描**：所有代码步给出完整代码；命令给出预期输出；无 TBD/TODO（vision_model_set_path
运行时选择 UI 属设计文档明确的"后续迭代"，不在本计划范围，非占位）。

**3. 类型一致性**：`vision_model_det_t`（category/score/box[4]）Task2 定义、Task6 使用一致；
`vision_model_run/last_infer_ms/get_class_probs` 签名 Task2↔Task6 一致；`VISION_DET_STACK_SIZE`
Task5 改宏、任务创建引用一致；UI 字段 jt_a/jt_m 等与 evt.h 一致。

**4. 顺序正确性**：文件先就位(T1)→封装层建(T2)→分区/打包配置(T3)→首次全量编译(T4)→栈+init(T5)→
换假框(T6)→烧录验证(T7)。T1~T3 不单独编译（CMake 未就绪），T4 起每任务编译，符合"先让它编过再让它对"。
