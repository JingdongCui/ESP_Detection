# 全屏 UVC 视频流（DSI 合成屏 → PC 实时预览）实施计划

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** 把 ESP32-P4 MIPI 屏完整合成画面（1024×600 LVGL UI + 检测框）以 USB UVC 摄像头方式实时推给 PC，PC 用任意通用摄像头软件即可查看。

**Architecture:** 每当 PC（UVC 主机）请求一帧时，在 UVC 任务上下文里：① `BSP_LCD_GetFrameBuffers` 取 DSI 合成帧缓冲（RGB888、内存字节序 B,G,R）；② 用 PPA SRM 硬件搬运并置 `rgb_swap=true` 把 B,G,R 翻成 R,G,B、输出到对齐缓冲（零 CPU 修红蓝，顺带得到一份原子快照）；③ 用 ESP32-P4 硬件 JPEG 编码器（RGB888 / YUV444 / 质量 80）压缩；④ 通过 `usb_device_uvc` 回调把 JPEG 帧交给 TinyUSB，走独立的 USB 2.0 Type-C（OTG HS）口上传。全链路 PPA+JPEG+USB DMA 硬件完成，CPU 仅编排。

**Tech Stack:** ESP-IDF v5.5.4；`esp_driver_ppa`（`ppa_do_scale_rotate_mirror` + `rgb_swap`）；`esp_driver_jpeg`（硬件 JPEG，`jpeg_new_encoder_engine`/`jpeg_encoder_process`）；`espressif/usb_device_uvc`（基于 TinyUSB 的 UVC device）；现有 `bsp`（`BSP_LCD_GetFrameBuffers`）。

**关于测试策略（重要）：** 本特性是 USB/PPA/JPEG 硬件胶水层，无法用纯软件单元测试做 TDD。验证方式为**硬件在环**：编译通过 → 烧录 → RTT 日志断言初始化与每帧编码 → PC 端识别为摄像头 → 通用软件打开 → 目视核对画面内容与颜色。每个任务末尾给出可执行的构建/烧录/RTT/目视检查，而非 pytest 断言。

---

### Task 1: 添加 usb_device_uvc 依赖并确认组件拉取

**Files:**
- Modify: `main/idf_component.yml`（在 dependencies 末尾追加一行）

- [ ] **Step 1: 追加依赖声明**

在 `main/idf_component.yml` 的 `dependencies:` 块末尾（`espressif/esp_sccb_intf: '==0.0.8'` 之后）追加：

```yaml
  espressif/usb_device_uvc: '^1.0.0'
```

- [ ] **Step 2: 触发依赖解析（增量构建即可）**

Run: `./agentic/idf_build.sh reconfigure`（timeout 600000）
Expected: component manager 打印下载 `espressif/usb_device_uvc` 及其依赖（`espressif/esp_tinyusb` / `tinyusb`），reconfigure 成功无报错。

- [ ] **Step 3: 确认组件落地并记录 Kconfig/示例配置**

Run: `ls managed_components/ | grep -i uvc`
Expected: 出现 `espressif__usb_device_uvc`。

再检查该组件的 Kconfig 与示例默认配置，供 Task 2 使用（不要凭空猜符号名，以实际文件为准）：

Run: `grep -rn "FRAMESIZE\|FORMAT\|RESOLUTION\|WIDTH\|HEIGHT\|UVC_" managed_components/espressif__usb_device_uvc/Kconfig`
把打印出的实际 Kconfig 符号名记录下来（例如 `UVC_CAM1_FRAMESIZE_WIDTH` 之类），Task 2 按记录到的真实符号名设置。

- [ ] **Step 4: 提交**

```bash
git add main/idf_component.yml main/idf_component.lock
git commit -m "build: add usb_device_uvc dependency for screen UVC stream"
```

---

### Task 2: 配置 TinyUSB / UVC 描述符（1024×600 JPEG，HS 口）

**Files:**
- Modify: `sdkconfig`（经 menuconfig 写入；同时把关键项写入 `sdkconfig.defaults` 便于重建）

**背景：** ESP32-P4 的 UVC 走 USB 2.0 OTG **High-Speed** 控制器（DP=GPIO20/DM=GPIO19，专用 PHY）。烧录/调试走独立的 USB-Serial-JTAG 控制器，二者互不影响，可同时使用。

**已探明的组件行为（Task1 读源码得出）：** `usb_device_uvc@1.3.1` 的 ISOC MJPEG 描述符**恒定广播 4 个分辨率**（`UVC_FRAMES_INFO[cam][0..3]`：`[0]`=CAM1 默认，`[1..3]`=三个 `UVC_MULTI_FRAME_*` 尺寸，库默认 640×480/480×320/320×240），`bDefaultFrameIndex=1` 指向 `[0]`。该宏无 MULTI 开关保护，无法裁成单分辨率。**因此策略调整为：把 `[0]` 设成 1024×600 作默认，固件按主机协商到的分辨率（start_cb 回调参数）用 PPA 缩放输出**——4 种尺寸都能正确工作。真实 Kconfig 符号名（注意 HEIGT 拼写）：`CONFIG_UVC_CAM1_FRAMESIZE_WIDTH` / `CONFIG_UVC_CAM1_FRAMESIZE_HEIGT` / `CONFIG_UVC_CAM1_FRAMERATE` / `CONFIG_TINYUSB_RHPORT_HS`（P4 默认已 HS）/ `CONFIG_FORMAT_MJPEG_CAM1` / `CONFIG_UVC_MODE_ISOC_CAM1`。

- [ ] **Step 1: 把关键项写入 sdkconfig.defaults**

在 `sdkconfig.defaults` 追加（符号名已按 Task1 实测组件 Kconfig 确认）：

```
CONFIG_TINYUSB_RHPORT_HS=y
CONFIG_FORMAT_MJPEG_CAM1=y
CONFIG_UVC_MODE_ISOC_CAM1=y
CONFIG_UVC_CAM1_FRAMESIZE_WIDTH=1024
CONFIG_UVC_CAM1_FRAMESIZE_HEIGT=600
CONFIG_UVC_CAM1_FRAMERATE=15
```

- [ ] **Step 2: reconfigure 确认配置生效**

Run: `./agentic/idf_build.sh reconfigure`（timeout 600000）
Expected: 无 Kconfig 报错。
Run: `grep -i "framesize_width\|framesize_heigt\|rhport_hs\|mjpeg_cam1\|framerate" sdkconfig`
Expected: `CONFIG_UVC_CAM1_FRAMESIZE_WIDTH=1024`、`CONFIG_UVC_CAM1_FRAMESIZE_HEIGT=600`、HS/MJPEG 均在。

- [ ] **Step 3: 提交**

```bash
git add sdkconfig sdkconfig.defaults
git commit -m "build: configure TinyUSB HS + UVC JPEG default 1024x600 (adaptive)"
```

---

### Task 3: 新建 screen_uvc 组件骨架（头文件 + 构建脚本 + 空实现）

**Files:**
- Create: `components/screen_uvc/include/screen_uvc.h`
- Create: `components/screen_uvc/screen_uvc.c`
- Create: `components/screen_uvc/CMakeLists.txt`
- Create: `components/screen_uvc/idf_component.yml`

- [ ] **Step 1: 写公共头 `components/screen_uvc/include/screen_uvc.h`**

```c
#pragma once

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

// 启动全屏 UVC 视频流：分配缓冲、初始化 PPA/硬件 JPEG/UVC device。
// 成功后主机(PC)可将本设备识别为 USB 摄像头并拉取 1024x600 MJPEG。
// 需在 BSP_LCD_Init 与 setupUi 之后调用（依赖 DSI framebuffer 已建立）。
esp_err_t screen_uvc_start(void);

#ifdef __cplusplus
}
#endif
```

- [ ] **Step 2: 写构建脚本 `components/screen_uvc/CMakeLists.txt`**

```cmake
idf_component_register(SRCS "screen_uvc.c"
                       INCLUDE_DIRS "include"
                       REQUIRES bsp esp_driver_ppa esp_driver_jpeg esp_mm esp_timer)
```

（`usb_device_uvc` 由下面的 `idf_component.yml` 作为托管依赖引入，无需写进 REQUIRES。）

- [ ] **Step 3: 写组件依赖 `components/screen_uvc/idf_component.yml`**

```yaml
dependencies:
  espressif/usb_device_uvc: '^1.0.0'
```

- [ ] **Step 4: 写空实现 `components/screen_uvc/screen_uvc.c`（先只保证能编译）**

```c
#include "screen_uvc.h"
#include "esp_log.h"

static const char *TAG = "screen_uvc";

esp_err_t screen_uvc_start(void)
{
    ESP_LOGI(TAG, "screen_uvc_start (skeleton)");
    return ESP_OK;
}
```

- [ ] **Step 5: 编译确认骨架可注册**

Run: `./agentic/idf_build.sh`（timeout 600000）
Expected: 编译成功，日志中出现 `screen_uvc` 组件被纳入构建（无 “component not found” / 链接错误）。

- [ ] **Step 6: 提交**

```bash
git add components/screen_uvc/
git commit -m "feat(screen_uvc): add component skeleton"
```

---

### Task 4: 实现完整 screen_uvc.c（捕获+PPA修红蓝+硬件JPEG+UVC回调）

**Files:**
- Modify: `components/screen_uvc/screen_uvc.c`（整文件替换为完整实现）

**说明：** 这是一个内聚的翻译单元，为保证每次提交都能编译/运行，本任务一步给出完整文件，再逐步做硬件在环验证。

- [ ] **Step 1: 用完整实现替换 `components/screen_uvc/screen_uvc.c`**

```c
#include "screen_uvc.h"

#include <string.h>
#include <sys/time.h>

#include "esp_log.h"
#include "esp_check.h"
#include "esp_err.h"
#include "esp_heap_caps.h"
#include "esp_cache.h"
#include "esp_timer.h"

#include "driver/ppa.h"
#include "driver/jpeg_encode.h"

#include "bsp_lcd.h"
#include "usb_device_uvc.h"

static const char *TAG = "screen_uvc";

// EK79007 面板分辨率（见 agentic/board.md）。DSI framebuffer 为 RGB888。
#define SCREEN_W            1024
#define SCREEN_H            600
#define RGB_BYTES_PER_PX    3
// 缓冲按最大分辨率(全屏)预分配，可覆盖主机协商到的任何更小尺寸。
#define MAX_RGB_SIZE        ((size_t)SCREEN_W * SCREEN_H * RGB_BYTES_PER_PX)
// JPEG 输出缓冲：1024x600 q80/444 经验峰值 ~300-450KB，取 768KB 留裕量。
#define JPEG_OUT_CAP        (768 * 1024)
#define UVC_XFER_CAP        JPEG_OUT_CAP
#define JPEG_QUALITY        80

static ppa_client_handle_t   s_ppa;             // SRM 客户端（做 rgb_swap + 缩放）
static jpeg_encoder_handle_t s_jpeg;            // 硬件 JPEG 编码器
static uint8_t              *s_rgb;             // PPA 输出 = JPEG 输入（R,G,B，编码器对齐）
static size_t                s_rgb_alloc;       // s_rgb 实际分配大小
static uint8_t              *s_jpeg_out;        // JPEG 码流输出缓冲
static uint8_t              *s_uvc_xfer;        // TinyUSB UVC 传输缓冲
static uvc_fb_t              s_fb;              // 交还给 UVC 的帧描述（复用单缓冲）

// 主机协商到的输出分辨率（start_cb 写入，fb_get 读取）。默认全屏。
static volatile int          s_out_w = SCREEN_W;
static volatile int          s_out_h = SCREEN_H;

// 从 DSI framebuffer 抓一帧 → PPA 硬件缩放并交换 R/B → 输出到 s_rgb（R,G,B，out_w×out_h）。
static esp_err_t capture_screen_rgb(int out_w, int out_h)
{
    void *fb0 = NULL, *fb1 = NULL;
    esp_err_t ret = BSP_LCD_GetFrameBuffers(&fb0, &fb1);
    if (ret != ESP_OK || fb0 == NULL) {
        return (ret == ESP_OK) ? ESP_ERR_INVALID_STATE : ret;
    }

    size_t out_size = (size_t)out_w * out_h * RGB_BYTES_PER_PX;
    // 抓 fb0。DOUBLE_DIRECT 抗撕裂下 LVGL 在两块 fb 间交替刷新，
    // PPA 搬运是亚毫秒级，撕裂窗口极小，用于监控足够。
    ppa_srm_oper_config_t srm = {
        .in = {
            .buffer = fb0,
            .pic_w = SCREEN_W,
            .pic_h = SCREEN_H,
            .block_w = SCREEN_W,
            .block_h = SCREEN_H,
            .block_offset_x = 0,
            .block_offset_y = 0,
            .srm_cm = PPA_SRM_COLOR_MODE_RGB888,
        },
        .out = {
            .buffer = s_rgb,
            .buffer_size = s_rgb_alloc,
            .pic_w = out_w,
            .pic_h = out_h,
            .block_offset_x = 0,
            .block_offset_y = 0,
            .srm_cm = PPA_SRM_COLOR_MODE_RGB888,
        },
        .rotation_angle = PPA_SRM_ROTATION_ANGLE_0,
        .scale_x = (float)out_w / SCREEN_W,
        .scale_y = (float)out_h / SCREEN_H,
        // framebuffer 内存序为 B,G,R；置位后 PPA 硬件把 R/B 交换成编码器要的 R,G,B。
        .rgb_swap = true,
        .mode = PPA_TRANS_MODE_BLOCKING,
    };
    ret = ppa_do_scale_rotate_mirror(s_ppa, &srm);
    if (ret != ESP_OK) {
        return ret;
    }
    // PPA 写的是 PSRAM，回读前把该区间从内存同步进 cache。
    return esp_cache_msync(s_rgb, out_size, ESP_CACHE_MSYNC_FLAG_DIR_M2C);
}

// UVC 主机请求一帧：抓屏(缩放到协商尺寸)→修色→硬件 JPEG，填 s_fb 返回。失败返回 NULL。
static uvc_fb_t *uvc_fb_get_cb(void *cb_ctx)
{
    (void)cb_ctx;
    int out_w = s_out_w, out_h = s_out_h;
    if (capture_screen_rgb(out_w, out_h) != ESP_OK) {
        return NULL;
    }

    jpeg_encode_cfg_t cfg = {
        .width = out_w,
        .height = out_h,
        .src_type = JPEG_ENCODE_IN_FORMAT_RGB888,
        .sub_sample = JPEG_DOWN_SAMPLING_YUV444,   // 444 不做色度下采样，UI/文字最清晰
        .image_quality = JPEG_QUALITY,
    };
    uint32_t out_len = 0;
    size_t in_size = (size_t)out_w * out_h * RGB_BYTES_PER_PX;
    esp_err_t ret = jpeg_encoder_process(s_jpeg, &cfg,
                                         s_rgb, in_size,
                                         s_jpeg_out, JPEG_OUT_CAP, &out_len);
    if (ret != ESP_OK || out_len == 0) {
        ESP_LOGW(TAG, "jpeg encode fail: %s len=%u", esp_err_to_name(ret), (unsigned)out_len);
        return NULL;
    }

    struct timeval tv;
    gettimeofday(&tv, NULL);
    s_fb.buf = s_jpeg_out;
    s_fb.len = out_len;
    s_fb.width = out_w;
    s_fb.height = out_h;
    s_fb.format = UVC_FORMAT_JPEG;
    s_fb.timestamp = tv;
    return &s_fb;
}

static esp_err_t uvc_start_cb(uvc_format_t format, int width, int height, int rate, void *cb_ctx)
{
    (void)cb_ctx;
    ESP_LOGI(TAG, "UVC start: fmt=%d %dx%d @%dfps", (int)format, width, height, rate);
    if (format != UVC_FORMAT_JPEG) {
        ESP_LOGE(TAG, "unsupported UVC format %d (only JPEG)", (int)format);
        return ESP_ERR_NOT_SUPPORTED;
    }
    // 记录主机协商分辨率；夹到全屏上限（缓冲按全屏预分配）。
    if (width <= 0 || width > SCREEN_W || height <= 0 || height > SCREEN_H) {
        ESP_LOGW(TAG, "negotiated %dx%d out of range, clamp to %dx%d", width, height, SCREEN_W, SCREEN_H);
        width = SCREEN_W;
        height = SCREEN_H;
    }
    s_out_w = width;
    s_out_h = height;
    return ESP_OK;
}

static void uvc_fb_return_cb(uvc_fb_t *fb, void *cb_ctx)
{
    (void)fb; (void)cb_ctx;
    // 单缓冲同步模型：TinyUSB 传完本帧才会再次 fb_get，无需额外处理。
}

static void uvc_stop_cb(void *cb_ctx)
{
    (void)cb_ctx;
    ESP_LOGI(TAG, "UVC stop (host closed stream)");
}

esp_err_t screen_uvc_start(void)
{
    // 1) JPEG 编码输入缓冲（同时作 PPA 输出）：按全屏最大尺寸用编码器对齐分配。
    jpeg_encode_memory_alloc_cfg_t in_mem = { .buffer_direction = JPEG_ENC_ALLOC_INPUT_BUFFER };
    s_rgb = (uint8_t *)jpeg_alloc_encoder_mem(MAX_RGB_SIZE, &in_mem, &s_rgb_alloc);
    if (!s_rgb) {
        ESP_LOGE(TAG, "alloc rgb input buffer failed (%u bytes)", (unsigned)MAX_RGB_SIZE);
        return ESP_ERR_NO_MEM;
    }

    // 2) JPEG 输出 + UVC 传输缓冲（PSRAM）。
    s_jpeg_out = (uint8_t *)heap_caps_malloc(JPEG_OUT_CAP, MALLOC_CAP_SPIRAM);
    s_uvc_xfer = (uint8_t *)heap_caps_malloc(UVC_XFER_CAP, MALLOC_CAP_SPIRAM);
    if (!s_jpeg_out || !s_uvc_xfer) {
        ESP_LOGE(TAG, "alloc jpeg/uvc buffer failed");
        return ESP_ERR_NO_MEM;
    }

    // 3) PPA SRM 客户端。
    ppa_client_config_t ppa_cfg = { .oper_type = PPA_OPERATION_SRM };
    ESP_RETURN_ON_ERROR(ppa_register_client(&ppa_cfg, &s_ppa), TAG, "ppa register");

    // 4) 硬件 JPEG 编码引擎。
    jpeg_encode_engine_cfg_t eng = { .intr_priority = 0, .timeout_ms = 100 };
    ESP_RETURN_ON_ERROR(jpeg_new_encoder_engine(&eng, &s_jpeg), TAG, "jpeg engine");

    // 5) 配置并初始化 UVC device。
    uvc_device_config_t cfg = {
        .uvc_buffer = s_uvc_xfer,
        .uvc_buffer_size = UVC_XFER_CAP,
        .start_cb = uvc_start_cb,
        .fb_get_cb = uvc_fb_get_cb,
        .fb_return_cb = uvc_fb_return_cb,
        .stop_cb = uvc_stop_cb,
        .cb_ctx = NULL,
    };
    ESP_RETURN_ON_ERROR(uvc_device_config(0, &cfg), TAG, "uvc config");
    ESP_RETURN_ON_ERROR(uvc_device_init(), TAG, "uvc init");

    ESP_LOGI(TAG, "screen UVC stream started: default %dx%d MJPEG q%d", SCREEN_W, SCREEN_H, JPEG_QUALITY);
    return ESP_OK;
}
```

> 注：`ESP_RETURN_ON_ERROR` 来自 `esp_check.h`，随 `esp_common` 自动可用；若编译报未声明，在文件顶部加 `#include "esp_check.h"`。

- [ ] **Step 2: 编译**

Run: `./agentic/idf_build.sh`（timeout 600000）
Expected: 编译链接成功。若报 `esp_cache_msync` 未定义 → 确认 `CMakeLists.txt` 的 REQUIRES 含 `esp_mm`；若报 UVC 头找不到 → 确认 Task 1 组件已拉取。

- [ ] **Step 3: 提交**

```bash
git add components/screen_uvc/screen_uvc.c
git commit -m "feat(screen_uvc): capture DSI framebuffer, PPA rgb_swap, HW JPEG, UVC callbacks"
```

---

### Task 5: 接入系统初始化

**Files:**
- Modify: `main/system_init.c`（加 include + 在 `vision_start();` 之后调用）
- Modify: `main/CMakeLists.txt`（PRIV_REQUIRES 增加 `screen_uvc`，若采用显式依赖列表）

- [ ] **Step 1: 加头文件包含**

在 `main/system_init.c` 的 include 区（`#include "system_monitor.h"` 之后）加：

```c
#include "screen_uvc.h"
```

- [ ] **Step 2: 在 vision_start() 之后启动 UVC**

在 `main/system_init.c` 中 `vision_start();`（约 :84）之后、`system_monitor();` 之前插入：

```c
    //启动全屏 UVC 视频流：DSI 合成屏 → PPA 修色 → 硬件 JPEG → USB UVC 推给 PC。
    //需在 vision_start 之后：此时 LVGL/DSI framebuffer 已在持续刷新合成画面。
    esp_err_t uvc_ret = screen_uvc_start();
    if (uvc_ret != ESP_OK) {
        ESP_LOGW(TAG, "screen UVC stream start failed: %s", esp_err_to_name(uvc_ret));
    }
```

- [ ] **Step 3: 确认 main 能找到 screen_uvc 组件**

查看 `main/CMakeLists.txt` 是否使用显式 `REQUIRES`/`PRIV_REQUIRES` 列表：
Run: `grep -n "REQUIRES" main/CMakeLists.txt`
- 若存在显式列表：在其中追加 `screen_uvc`。
- 若未使用显式列表（依赖自动全量可见）：无需改动，跳过。

- [ ] **Step 4: 编译**

Run: `./agentic/idf_build.sh`（timeout 600000）
Expected: 编译链接成功。

- [ ] **Step 5: 提交**

```bash
git add main/system_init.c main/CMakeLists.txt
git commit -m "feat: start screen UVC stream in system init after vision_start"
```

---

### Task 6: 烧录并用 RTT 验证初始化与编码

**Files:** 无（硬件在环验证）

- [ ] **Step 1: 烧录运行**

Run: `python3 agentic/esp_target.py flash-and-run build/ --app-only`
Expected: 烧录成功，固件启动。

- [ ] **Step 2: 启动 RTT 读取器（后台）**

Run（run_in_background）: `python3 agentic/rtt_reader.py --elf build/ESP32P4_Detection.elf --output agentic/.esp-agent/rtt.log --kill-existing`
（elf 名以 `build/*.elf` 实际文件为准，可先 `ls build/*.elf`。）

- [ ] **Step 3: 读日志确认启动无崩溃**

等待 3 秒后 Run: `tail -40 agentic/.esp-agent/rtt.log`
Expected：出现 `screen UVC stream started: 1024x600 MJPEG q80`，且无 panic/backtrace、无 `alloc ... failed`。

- [ ] **Step 4（若崩溃/内存不足）：诊断**

- 若 `alloc rgb input buffer failed` 或 `alloc jpeg/uvc buffer failed`：PSRAM 不足。核对可用 PSRAM（RTT 里 system_monitor 快照），必要时把 `JPEG_OUT_CAP`/`UVC_XFER_CAP` 调小或改推 800×480（见 Task 8 备选）。
- 若启动 panic：`python3 agentic/esp_target.py halt` → `cpu-regs` → 读 `mepc`/`mcause`/`mtval` + addr2line 定位（勿用运行态 GDB bt）。

---

### Task 7: PC 端识别与目视验证（颜色/内容）

**Files:** 无（硬件在环验证）

**接线：** 用一根 USB 数据线，连接开发板的 **USB 2.0 Type-C（OTG HS，Device）口**（不是 USB-Serial-JTAG 那个烧录口）到 PC。烧录口保持原样接着，两口可同时用。注意：板上 USB 2.0 Type-C 与 Type-A 不能同时用，我们用 Type-C。

- [ ] **Step 1: PC 识别为摄像头**

Windows：设备管理器 → “摄像头/图像设备” 应出现一个新 UVC 摄像头（免驱）。
可用软件：Windows「相机」App、`ffplay`、PotPlayer、OBS（视频采集设备）。

- [ ] **Step 2: 打开预览，目视核对内容**

用相机 App 或 `ffplay` 打开该摄像头。
Expected：看到与开发板屏幕一致的 1024×600 画面（LVGL 界面 + 实时检测框）。

命令行验证（若装了 ffmpeg）：
Run: `ffplay -f dshow -i video="<UVC设备名>"`（设备名用 `ffmpeg -list_devices true -f dshow -i dummy` 查）

- [ ] **Step 3: 核对红蓝是否正确**

观察画面中已知颜色（如 UI 里的红色告警/蓝色控件）。
- 颜色正确 → `rgb_swap` 方向正确，完成。
- 红蓝互换 → 把 `screen_uvc.c` 里 `.rgb_swap = true` 改为 `false`（或反之），重编译烧录再看。这是编译期常量，一次翻转即可定论。

- [ ] **Step 4: 若颜色修正需改动，提交**

```bash
git add components/screen_uvc/screen_uvc.c
git commit -m "fix(screen_uvc): correct RGB channel order for host preview"
```

- [ ] **Step 5: 观察流畅度与稳定性**

连续预览 1-2 分钟，确认：无花屏持续、无固件复位（RTT 无 panic）、帧率稳定（~15fps）。
- 若带宽/卡顿问题明显：进入 Task 8 备选（降分辨率或降质量）。

---

### Task 8（备选，按需）：带宽/内存不足时降规格

**触发条件：** Task 6 报 PSRAM 不足，或 Task 7 卡顿/带宽吃紧。

**Files:**
- Modify: `components/screen_uvc/screen_uvc.c`
- Modify: `sdkconfig` / `sdkconfig.defaults`（UVC 描述符分辨率同步改）

- [ ] **Step 1: 降到 800×480（PPA 顺带缩放，零额外 CPU）**

`screen_uvc.c` 里新增输出尺寸常量并让 PPA 缩放：

```c
#define OUT_W   800
#define OUT_H   480
#define OUT_RGB_SIZE ((size_t)OUT_W * OUT_H * RGB_BYTES_PER_PX)
```

把 `capture_screen_rgb` 中 `out.pic_w/pic_h` 改为 `OUT_W/OUT_H`，`scale_x=(float)OUT_W/SCREEN_W`、`scale_y=(float)OUT_H/SCREEN_H`；`s_rgb` 按 `OUT_RGB_SIZE` 分配；JPEG cfg 的 `width/height` 与 `jpeg_encoder_process` 的输入长度改用 `OUT_W/OUT_H`/`OUT_RGB_SIZE`。

- [ ] **Step 2: UVC 描述符分辨率同步改 800×480**

按 Task 2 的符号名把宽/高改为 800/480，reconfigure。

- [ ] **Step 3: 或仅降质量**

若只想省带宽、保持 1024×600：把 `JPEG_QUALITY` 由 80 降到 60；或把 `sub_sample` 改 `JPEG_DOWN_SAMPLING_YUV422`（色度减半，文字略糊但带宽降约 1/3）。

- [ ] **Step 4: 重编译、烧录、回到 Task 7 复验**

Run: `./agentic/idf_build.sh` → `python3 agentic/esp_target.py flash-and-run build/ --app-only`

- [ ] **Step 5: 提交**

```bash
git add components/screen_uvc/screen_uvc.c sdkconfig sdkconfig.defaults
git commit -m "perf(screen_uvc): reduce resolution/quality to fit bandwidth"
```

---

## Self-Review

**1. Spec 覆盖：**
- 全屏合成画面（UI+框）→ Task 4 `capture_screen_rgb` 取 `BSP_LCD_GetFrameBuffers`（DSI 合成帧）✔
- 硬件加速 / 低 CPU → PPA(硬件搬运+rgb_swap) + esp_driver_jpeg(硬件编码) + USB DMA，CPU 仅编排 ✔
- RGB888 尽量一致 → JPEG 444 无色度下采样 + q80；已向用户澄清 JPEG 有损、“RGB888”是编码器输入而非无损 ✔
- 红蓝反修正 → PPA `rgb_swap=true`（零 CPU）Task 4，Task 7 目视定论方向 ✔
- 多插哪根线/哪个口/PC 怎么看 → Task 7 接线说明 + 免驱 UVC + 相机App/ffplay ✔
- 与烧录口共存 → Task 2 背景说明（USB-Serial-JTAG 独立于 OTG HS）✔

**2. 占位符扫描：** 无 TBD/TODO；所有代码步骤给出完整可编译代码。唯一“以实际为准”的是 UVC 描述符 Kconfig 符号名（Task 1-Step3 先探明真实符号再用），这是刻意避免猜测硬件/组件常量，非占位。

**3. 类型一致性：** `screen_uvc_start()`（头/实现/调用点一致）；`uvc_fb_t`/`uvc_device_config_t`/`uvc_device_config`/`uvc_device_init`（与组件头一致）；`ppa_srm_oper_config_t.rgb_swap`（IDF v5.5.4 确认）；`jpeg_encode_cfg_t{width,height,src_type=JPEG_ENCODE_IN_FORMAT_RGB888,sub_sample=JPEG_DOWN_SAMPLING_YUV444,image_quality}`、`jpeg_new_encoder_engine`/`jpeg_encoder_process`/`jpeg_alloc_encoder_mem`（IDF v5.5.4 头确认）；`BSP_LCD_GetFrameBuffers(void**,void**)`（bsp_lcd.h 确认）。
