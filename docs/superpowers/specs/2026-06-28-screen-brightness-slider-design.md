# 屏幕亮度滑块 — 亮度映射设计

日期：2026-06-28
状态：待评审

## 1. 背景与问题

dashboard "显示" 标签页有一个"屏幕亮度"滑块（控件 `scr_dashboard_slider_82EIlsYJ`，
范围 0~100，初始值 0）和百分比标签 `scr_dashboard_label_runtime_bright__data`（写死 "00%"）。

当前这个滑块是**空壳**，三端都缺实现：

- **UI 业务层** (`sdk/ui.c`)：只注册了系统监控回调，亮度滑块**无任何事件回调**——拖动不触发动作，百分比标签从不更新。
- **BSP 背光层** (`bsp_lcd.c:28-36`)：GPIO26 在 board.md 标注为 "LCD 背光 PWM"，但代码只当**普通 GPIO 拉高常亮**，无 LEDC PWM，无任何调光 API。

目标：让滑块真正调节屏幕亮度，按人眼感知（gamma）映射，并实时更新百分比标签。

## 2. 设计决策（已与用户确认）

| 项 | 决策 | 理由 |
| --- | --- | --- |
| 映射曲线 | Gamma 感知曲线，γ≈2.2 | 人眼亮度感知非线性，线性 PWM 低端变化太快、高端看不出差别 |
| 最低亮度 | 留下限（duty floor） | 滑到 0% 时屏幕仍微亮，避免"调没了找不回" |
| 默认亮度 | 80% | 开机较亮，适合室内/演示 |
| 接线方式 | 回调注入解耦 | UI 组件零硬件依赖，符合 anyui 界面/业务解耦设计 |

## 3. 架构与数据流

```
用户拖动滑块
  │ LVGL LV_EVENT_VALUE_CHANGED
  ▼
sdk/ui.c  ui_brightness_slider_event_cb()
  │  ① 读取滑块值 pct (0~100)
  │  ② 更新百分比标签 "NN%"
  │  ③ 调用注册的回调 s_brightness_handler(pct)   ← 函数指针，UI 不知道背后是谁
  ▼
main/system_init.c  注入的 lambda → BSP_LCD_SetBrightness(pct)
  ▼
bsp_lcd.c  BSP_LCD_SetBrightness(pct)
  │  gamma 映射: duty = floor + (max-floor) * (pct/100)^2.2
  ▼
LEDC PWM @ GPIO26  调节背光占空比
```

反向流（与现有 system_monitor 的 BSP→UI 正好相反）通过**函数指针注入**实现解耦：
`sdk/ui.c` 暴露 setter，`main` 在初始化时把 BSP 的背光函数注册进来。UI 组件 CMakeLists 保持只依赖 lvgl。

## 4. 组件改动

### 4.1 BSP 背光层 (`components/bsp/`)

**`bsp_lcd.c`**
- 删除 GPIO26 的 `gpio_config` + `gpio_set_level` 拉高逻辑（28-36 行）。
- 改用 LEDC PWM 初始化背光：
  - `ledc_timer_config`: `LEDC_LOW_SPEED_MODE`（P4 仅低速）, `LEDC_TIMER_13_BIT`, 频率 5kHz, 时钟源 `LEDC_AUTO_CLK`（PLL_F80M 下 13-bit 最高约 9.7kHz，5kHz 安全）。
  - `ledc_channel_config`: GPIO26, channel 0, timer 0。
  - 初始化时按默认 80% 设置占空比。
- 新增亮度 API：
  ```c
  void BSP_LCD_SetBrightness(int percent);  // 0~100，内部 gamma 映射 + clamp
  ```
- Gamma 映射常量：
  - `BSP_LCD_BL_DUTY_MAX` = 8191 (2^13-1)
  - `BSP_LCD_BL_DUTY_FLOOR` = 8% × max ≈ 655（最低亮度下限）
  - duty = floor + (max - floor) × (pct/100)^2.2，用浮点 `powf` 计算（bsp 已开 `-ffast-math -O3`）。

**`bsp_lcd.h`**
- 声明 `void BSP_LCD_SetBrightness(int percent);`
- 内部默认亮度宏 `#define BSP_LCD_BL_DEFAULT_PERCENT 80`（BSP 自身初始化占空比用，不跨组件暴露）。

**`CMakeLists.txt`**
- `REQUIRES` 或 `PRIV_REQUIRES` 加 `esp_driver_ledc`。

### 4.2 UI 业务层 (`components/UI/sdk/`)

**`ui.c` / `ui.h`**
- 新增亮度回调类型与 setter：
  ```c
  typedef void (*ui_brightness_handler_t)(int percent);
  void ui_set_brightness_handler(ui_brightness_handler_t handler);
  ```
  内部静态变量 `s_brightness_handler` 保存指针。
- 新增滑块事件回调 `ui_brightness_slider_event_cb`：
  - `lv_slider_get_value()` 取值 → `lv_label_set_text_fmt(..., "%d%%", pct)` 更新百分比标签
  - 若 `s_brightness_handler` 非空则调用之
- 新增挂载函数：
  ```c
  void ui_attach_brightness_slider(void);
  ```
  内部 `extern` 引用 `scr_dashboard_slider_82EIlsYJ` 和 `scr_dashboard_label_runtime_bright__data`，
  `lv_obj_add_event_cb(slider, ui_brightness_slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL)`，
  并把滑块初值与标签同步为 `BSP_LCD_BL_DEFAULT_PERCENT`（80），触发一次回调让背光与 UI 一致。

  > 注：`BSP_LCD_BL_DEFAULT_PERCENT` 宏会让 UI 编译期依赖 bsp 头。为保持 UI 零硬件依赖，
  > 默认值在 UI 侧用独立宏 `UI_BRIGHTNESS_DEFAULT_PERCENT 80`，与 BSP 默认值保持一致（同为 80）。

### 4.3 主程序 (`main/system_init.c`)

- 在 `setupUi()` 之后、`BSP_LVGL_Unlock()` 之前（LVGL 锁内）：
  ```c
  ui_set_brightness_handler(BSP_LCD_SetBrightness);  // 函数签名一致，直接传函数指针
  ui_attach_brightness_slider();
  ```
- `main` 已同时 REQUIRES bsp 和 UI，无需改 CMakeLists。

## 5. 错误处理

- `BSP_LCD_SetBrightness`：percent 入参 clamp 到 [0,100]。
- LEDC 初始化用 `ESP_ERROR_CHECK`（与 bsp_lcd.c 现有风格一致，初始化失败直接 abort 便于定位）。
- `ui_attach_brightness_slider`：滑块/标签指针为 NULL 时早返回（防御 setupUi 未建好）。
- `s_brightness_handler` 为 NULL 时滑块仍更新标签，只是不调背光（UI 可独立工作）。

## 6. 验证

- **编译**：`./agentic/idf_build.sh`（10 分钟超时），修复所有错误/警告。
- **硬件验证**（烧录后）：
  1. `flash-and-run build/ --app-only` 启动固件。
  2. 进入 dashboard "显示" 标签，确认开机时滑块在 80%、标签显示 "80%"、屏幕较亮。
  3. 拖动滑块到低端，确认屏幕变暗但不全黑（下限生效），标签实时更新。
  4. 拖动到 100%，确认最亮。
  5. 拖到 0%，确认仍可见（gamma + floor 生效）。
- **寄存器佐证**（可选）：`read-reg` LEDC 占空比寄存器确认 PWM duty 随滑块变化。

## 7. 不做的事（YAGNI）

- 不做亮度持久化（NVS 保存）——本次只做实时调节。
- 不做亮度渐变动画——直接设值。
- 不改其他滑块（内存/PSRAM/置信度等）的行为。
