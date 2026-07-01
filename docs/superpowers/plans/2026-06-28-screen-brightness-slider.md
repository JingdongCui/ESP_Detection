# 屏幕亮度滑块亮度映射 Implementation Plan

> **For agentic workers:** 本计划面向 ESP32-P4 嵌入式 C 固件，无 pytest/单元测试框架。验证方式为「编译 → 烧录 → RTT/截图/寄存器观察」。本项目非 git 仓库，跳过所有 commit 步骤。逐任务执行，每步完成后勾选。

**Goal:** 让 dashboard "屏幕亮度" 滑块通过 LEDC PWM 实时调节 GPIO26 背光，按 gamma=2.2 感知曲线映射，保留最低亮度下限，开机默认 80%。

**Architecture:** 三层改动 + 函数指针注入解耦。BSP 把 GPIO26 从常亮 GPIO 改为 LEDC PWM 并暴露 `BSP_LCD_SetBrightness(percent)`（内部 gamma 映射）；UI 滑块绑 `LV_EVENT_VALUE_CHANGED` 回调，读值→更新百分比标签→调注入的函数指针；main 在 `setupUi()` 后把 BSP 函数注入 UI。UI 组件保持零硬件依赖。

**Tech Stack:** ESP-IDF v5.5.4, `esp_driver_ledc`, LVGL 9.x, anyui SDK。

---

## File Structure

| 文件 | 职责 | 操作 |
| --- | --- | --- |
| `components/bsp/bsp_lcd.c` | 背光 LEDC PWM 初始化 + gamma 调光实现 | 修改 |
| `components/bsp/include/bsp_lcd.h` | 背光 API 声明 + 默认亮度宏 | 修改 |
| `components/bsp/CMakeLists.txt` | 加 `esp_driver_ledc` 依赖 | 修改 |
| `components/UI/sdk/ui.h` | 亮度 handler 类型/setter/attach 声明 | 修改 |
| `components/UI/sdk/ui.c` | 滑块事件回调 + handler 存储 + attach 实现 | 修改 |
| `main/system_init.c` | 注入 BSP 背光函数并挂载滑块 | 修改 |

关键常量（贯穿全计划，名称必须一致）：
- `LCD_BK_LIGHT_GPIO` = 26（bsp_lcd.c 已有）
- `BSP_LCD_BL_LEDC_MODE` = `LEDC_LOW_SPEED_MODE`
- `BSP_LCD_BL_LEDC_TIMER` = `LEDC_TIMER_0`
- `BSP_LCD_BL_LEDC_CHANNEL` = `LEDC_CHANNEL_0`
- `BSP_LCD_BL_DUTY_RES` = `LEDC_TIMER_13_BIT`
- `BSP_LCD_BL_DUTY_MAX` = `8191`（2^13-1）
- `BSP_LCD_BL_FREQ_HZ` = `5000`
- `BSP_LCD_BL_GAMMA` = `2.2f`
- `BSP_LCD_BL_FLOOR_PCT` = `8`（最低亮度下限百分比）
- `BSP_LCD_BL_DEFAULT_PERCENT` = `80`（bsp_lcd.h 暴露）
- `UI_BRIGHTNESS_DEFAULT_PERCENT` = `80`（ui.h 独立定义，与上者同值，保 UI 零硬件依赖）
- UI 控件名：滑块 `scr_dashboard_slider_82EIlsYJ`，标签 `scr_dashboard_label_runtime_bright__data`

---

## Task 1: BSP 背光 LEDC PWM 层

**Files:**
- Modify: `components/bsp/bsp_lcd.c`
- Modify: `components/bsp/include/bsp_lcd.h`
- Modify: `components/bsp/CMakeLists.txt`

- [ ] **Step 1: CMakeLists 加 LEDC 依赖**

`components/bsp/CMakeLists.txt` 的 `PRIV_REQUIRES` 行末尾追加 `esp_driver_ledc`：

```cmake
    PRIV_REQUIRES espressif__esp_cam_sensor log esp_driver_mcpwm esp_driver_pcnt pid_ctrl esp_driver_ledc
```

- [ ] **Step 2: bsp_lcd.h 加 API 声明与默认值宏**

在 `components/bsp/include/bsp_lcd.h` 的 `BSP_LCD_GetFrameBuffers` 声明后、`#pragma once` 块内追加：

```c
// 背光默认亮度（开机值），UI 侧用独立同值宏保持组件解耦
#define BSP_LCD_BL_DEFAULT_PERCENT 80

// 设置屏幕背光亮度。percent: 0~100，内部按 gamma=2.2 感知曲线映射并保留最低下限。
// percent 越界自动 clamp 到 [0,100]。
void BSP_LCD_SetBrightness(int percent);
```

- [ ] **Step 3: bsp_lcd.c 顶部加头文件与常量**

在 `components/bsp/bsp_lcd.c` 现有 `#include "bsp_lcd.h"` 之后追加：

```c
#include <math.h>
#include "driver/ledc.h"
```

在 `#define LCD_BIT_PER_PIXEL 24` 之后追加背光 LEDC 常量：

```c
#define BSP_LCD_BL_LEDC_MODE     LEDC_LOW_SPEED_MODE
#define BSP_LCD_BL_LEDC_TIMER    LEDC_TIMER_0
#define BSP_LCD_BL_LEDC_CHANNEL  LEDC_CHANNEL_0
#define BSP_LCD_BL_DUTY_RES      LEDC_TIMER_13_BIT
#define BSP_LCD_BL_DUTY_MAX      8191      // 2^13 - 1
#define BSP_LCD_BL_FREQ_HZ       5000
#define BSP_LCD_BL_GAMMA         2.2f
#define BSP_LCD_BL_FLOOR_PCT     8         // 最低亮度下限(占 max 的百分比)
```

- [ ] **Step 4: bsp_lcd.c 实现 BSP_LCD_SetBrightness**

在 `components/bsp/bsp_lcd.c` 的 `BSP_LCD_GetPanelHandle` 函数之前（约第 95 行前）新增：

```c
void BSP_LCD_SetBrightness(int percent)
{
    if (percent < 0)   percent = 0;
    if (percent > 100) percent = 100;

    // gamma 感知映射 + 最低下限: duty = floor + (max-floor) * (pct/100)^gamma
    const float floor_duty = BSP_LCD_BL_DUTY_MAX * (BSP_LCD_BL_FLOOR_PCT / 100.0f);
    const float span = BSP_LCD_BL_DUTY_MAX - floor_duty;
    float norm = powf(percent / 100.0f, BSP_LCD_BL_GAMMA);
    uint32_t duty = (uint32_t)(floor_duty + span * norm + 0.5f);
    if (duty > BSP_LCD_BL_DUTY_MAX) {
        duty = BSP_LCD_BL_DUTY_MAX;
    }

    ESP_ERROR_CHECK(ledc_set_duty(BSP_LCD_BL_LEDC_MODE, BSP_LCD_BL_LEDC_CHANNEL, duty));
    ESP_ERROR_CHECK(ledc_update_duty(BSP_LCD_BL_LEDC_MODE, BSP_LCD_BL_LEDC_CHANNEL));
}
```

- [ ] **Step 5: bsp_lcd.c 把 GPIO 背光替换为 LEDC 初始化**

在 `BSP_LCD_Init()` 中，把现有背光块（约 27-36 行）：

```c
    // 背光
#if LCD_BK_LIGHT_GPIO >= 0
    gpio_config_t bk_cfg = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << LCD_BK_LIGHT_GPIO,
    };
    ESP_ERROR_CHECK(gpio_config(&bk_cfg));
    gpio_set_level(LCD_BK_LIGHT_GPIO, LCD_BK_LIGHT_ON_LEVEL);
    ESP_LOGI(TAG, "Backlight on");
#endif
```

整体替换为 LEDC PWM 初始化：

```c
    // 背光 LEDC PWM (GPIO26)，开机置默认亮度
#if LCD_BK_LIGHT_GPIO >= 0
    ledc_timer_config_t bl_timer = {
        .speed_mode      = BSP_LCD_BL_LEDC_MODE,
        .duty_resolution = BSP_LCD_BL_DUTY_RES,
        .timer_num       = BSP_LCD_BL_LEDC_TIMER,
        .freq_hz         = BSP_LCD_BL_FREQ_HZ,
        .clk_cfg         = LEDC_AUTO_CLK,
    };
    ESP_ERROR_CHECK(ledc_timer_config(&bl_timer));

    ledc_channel_config_t bl_ch = {
        .gpio_num   = LCD_BK_LIGHT_GPIO,
        .speed_mode = BSP_LCD_BL_LEDC_MODE,
        .channel    = BSP_LCD_BL_LEDC_CHANNEL,
        .intr_type  = LEDC_INTR_DISABLE,
        .timer_sel  = BSP_LCD_BL_LEDC_TIMER,
        .duty       = 0,
        .hpoint     = 0,
    };
    ESP_ERROR_CHECK(ledc_channel_config(&bl_ch));

    BSP_LCD_SetBrightness(BSP_LCD_BL_DEFAULT_PERCENT);
    ESP_LOGI(TAG, "Backlight LEDC PWM on, default %d%%", BSP_LCD_BL_DEFAULT_PERCENT);
#endif
```

> 注：`LCD_BK_LIGHT_ON_LEVEL` 宏不再使用，可保留（无害）或删除其 `#define`。本计划保留，避免牵连。

- [ ] **Step 6: 编译验证 BSP 层**

Run: `./agentic/idf_build.sh`（设置 timeout 600000）
Expected: 编译通过，无 error。若报 `ledc.h not found` → 检查 Step 1 的 CMakeLists 依赖。若报 `powf` 未定义 → 检查 Step 3 的 `#include <math.h>`。

---

## Task 2: UI 滑块亮度回调

**Files:**
- Modify: `components/UI/sdk/ui.h`
- Modify: `components/UI/sdk/ui.c`

- [ ] **Step 1: ui.h 加类型、默认值宏与函数声明**

在 `components/UI/sdk/ui.h` 的 `uint32_t ui_register_system_monitor_events(event_table_t *table);` 之后追加：

```c
// 开机默认亮度，与 BSP_LCD_BL_DEFAULT_PERCENT 保持同值(UI 不依赖 bsp 头故独立定义)
#define UI_BRIGHTNESS_DEFAULT_PERCENT 80

// 亮度回调：滑块值变化时被调用，percent 为 0~100。由 main 注入 BSP 背光函数。
typedef void (*ui_brightness_handler_t)(int percent);

// 注册亮度处理函数(函数指针注入，解耦 UI 与硬件层)
void ui_set_brightness_handler(ui_brightness_handler_t handler);

// 给亮度滑块挂事件回调，并把滑块/标签初值同步为 UI_BRIGHTNESS_DEFAULT_PERCENT。
// 必须在 setupUi() 之后、LVGL 锁内调用。
void ui_attach_brightness_slider(void);
```

- [ ] **Step 2: ui.c 加 extern 控件声明与 handler 静态变量**

在 `components/UI/sdk/ui.c` 现有 extern 控件声明块（约 153-172 行，`scr_dashboard_label_runtime_cpu_core1_usage` 等）之后追加：

```c
extern lv_obj_t *scr_dashboard_slider_82EIlsYJ;
extern lv_obj_t *scr_dashboard_label_runtime_bright__data;

static ui_brightness_handler_t s_brightness_handler = NULL;
```

- [ ] **Step 3: ui.c 实现 setter、事件回调与 attach**

在 `ui.c` 末尾（`ui_register_system_monitor_events` 函数之后）追加：

```c
void ui_set_brightness_handler(ui_brightness_handler_t handler)
{
    s_brightness_handler = handler;
}

static void ui_brightness_slider_event_cb(lv_event_t *e)
{
    lv_obj_t *slider = lv_event_get_target(e);
    int pct = (int)lv_slider_get_value(slider);
    pct = ui_clamp_percent(pct);

    if (scr_dashboard_label_runtime_bright__data) {
        lv_label_set_text_fmt(scr_dashboard_label_runtime_bright__data, "%d%%", pct);
    }
    if (s_brightness_handler) {
        s_brightness_handler(pct);
    }
}

void ui_attach_brightness_slider(void)
{
    if (!scr_dashboard_slider_82EIlsYJ) {
        return;
    }

    lv_obj_add_event_cb(scr_dashboard_slider_82EIlsYJ, ui_brightness_slider_event_cb,
                        LV_EVENT_VALUE_CHANGED, NULL);

    // 同步开机默认亮度：设滑块值并主动触发一次回调，使标签与背光与 UI 一致
    lv_slider_set_value(scr_dashboard_slider_82EIlsYJ, UI_BRIGHTNESS_DEFAULT_PERCENT, LV_ANIM_OFF);
    lv_obj_send_event(scr_dashboard_slider_82EIlsYJ, LV_EVENT_VALUE_CHANGED, NULL);
}
```

> 注：`ui_clamp_percent` 已在 ui.c 中定义（约 174 行），直接复用。

- [ ] **Step 4: 编译验证 UI 层**

Run: `./agentic/idf_build.sh`（timeout 600000）
Expected: 编译通过。若报 `lv_obj_send_event` 未定义 → LVGL 9.x 该 API 存在，确认 `lvgl.h` 已 include（ui.h 已含）。若报控件未声明 → 检查 Step 2 的 extern 名称拼写。

---

## Task 3: main 注入背光回调并挂载滑块

**Files:**
- Modify: `main/system_init.c`

- [ ] **Step 1: 在 setupUi 后注入并挂载**

在 `main/system_init.c` 的 `ui_register_system_monitor_events(get_current_event_table());`（第 39 行）之后追加：

```c
    // 亮度滑块: 注入 BSP 背光函数(签名一致可直接传)并挂载滑块事件,
    // attach 内部会把滑块/标签初值同步为默认亮度并触发一次回调点亮背光
    ui_set_brightness_handler(BSP_LCD_SetBrightness);
    ui_attach_brightness_slider();
```

> 说明：这两行在 `BSP_LVGL_Lock()` 与 `BSP_LVGL_Unlock()` 之间（LVGL 锁内），滑块操作安全。`bsp_lcd.h` 已被 system_init.c include（第 3 行），`ui.h` 已 include（第 10 行）。

- [ ] **Step 2: 编译验证全链路**

Run: `./agentic/idf_build.sh`（timeout 600000）
Expected: 编译通过，无 error/warning。若报 `BSP_LCD_SetBrightness` 隐式声明 → 确认 bsp_lcd.h Task1-Step2 已加声明。

---

## Task 4: 烧录与硬件验证

**Files:** 无（验证任务）

- [ ] **Step 1: 烧录运行**

Run: `python3 agentic/esp_target.py flash-and-run build/ --app-only`
Expected: 固件启动，OpenOCD 不断开。

- [ ] **Step 2: 截图确认开机默认亮度**

进入 dashboard "显示" 标签页（若非默认页，先 `find_widget` 定位标签按钮并 `click_widget` 切换）。
Run: `mcp__anyui__screenshot`
Expected: 亮度滑块滑块块位于约 80% 位置，百分比标签显示 "80%"，屏幕较亮。

- [ ] **Step 3: 拖到低端验证下限**

用 `find_widget` 找到 `scr_dashboard_slider_82EIlsYJ` 中心坐标，`click_widget` 点击滑块轨道左端（约 x=滑块左边界+10）。
Run: `mcp__anyui__screenshot`
Expected: 屏幕明显变暗但**不全黑**（8% floor 生效），标签更新为低百分比。

- [ ] **Step 4: 拖到 100% 验证最亮**

`click_widget` 点击滑块轨道最右端。
Run: `mcp__anyui__screenshot`
Expected: 屏幕最亮，标签显示 "100%"。

- [ ] **Step 5: 寄存器佐证(可选)**

确认 LEDC duty 随滑块变化：
Run: `python3 agentic/esp_target.py raw "reg ledc"` 或读 LEDC duty 寄存器地址。
Expected: duty 值在低端≈655(8%×8191)、高端≈8191。

---

## Self-Review

**1. Spec coverage:**
- Gamma 曲线 → Task1-Step4 `powf(.., 2.2f)` ✓
- 最低下限 → Task1-Step4 `floor_duty` (8%) ✓
- 默认 80% → Task1-Step2 宏 + Task1-Step5 初始化 + Task2 UI 同值宏 ✓
- 回调注入解耦 → Task2 setter + Task3 注入 ✓
- BSP LEDC PWM → Task1 ✓
- UI 滑块事件 + 标签更新 → Task2-Step3 ✓
- main 接线 → Task3 ✓
- CMakeLists 依赖 → Task1-Step1 ✓
- 验证 → Task4 ✓

**2. Placeholder scan:** 无 TBD/TODO，所有步骤含完整代码与精确命令。

**3. Type/名称一致性:**
- `BSP_LCD_SetBrightness(int percent)` — h 声明、c 实现、main 注入签名一致 ✓
- `ui_brightness_handler_t` = `void(*)(int)` 与 `BSP_LCD_SetBrightness` 签名匹配 ✓
- 滑块名 `scr_dashboard_slider_82EIlsYJ`、标签名 `scr_dashboard_label_runtime_bright__data` 全计划一致（已对照 setup_scr_dashboard.c 确认）✓
- LEDC 常量名贯穿 Task1 各步一致 ✓
