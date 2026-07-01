# UI 替换指南

本文记录用 AnyUI 重新导出界面后，固件侧需要同步检查/修复的内容。

> **本项目的实际工作流：只覆盖 `components/UI/generated/` 文件夹。**
> `sdk/`（业务逻辑：`ui.c`/`cmd.c`/`evt.c`/`scr_list.c`）和 `simulator/dummy.c`
> 是手写并长期维护的，**不要用 AnyUI 导出覆盖它们**。因此本指南聚焦于：
> 替换 `generated/` 后，如何让它与不变的 `sdk/` 重新对齐、并通过 ESP-IDF 编译。

替换后所有修改集中在两类文件：

1. `components/UI/generated/`（被覆盖的新文件，需就地修几处）
2. `components/UI/CMakeLists.txt` 与 `components/UI/sdk/ui.c`（不被覆盖，但需同步新控件名）

---

## 0. 替换操作本身

把 AnyUI 新导出的 `generated/` 整个覆盖到：

```text
components/UI/generated/
```

覆盖后典型结构：

```text
components/UI/generated/
├── setup_ui.c
├── setup_ui.h
├── setup_scr_dashboard.c
├── animations.c / animations.h
├── images/*.c          # 图片资源
└── fonts/*.c           # 字体资源
```

**不要动** `components/UI/sdk/` 和 `components/UI/simulator/`。

替换后依次走第 1~6 节，最后第 7 节编译验证。

---

## 1. 同步 CMakeLists 的图片/字体清单（最易出错）

AnyUI 每次导出的 `images/` 和 `fonts/` 文件名会随界面增删而变化，而
`components/UI/CMakeLists.txt` 的 `SRCS` 是写死的文件列表。两者不一致会报：

- 列表里有、文件已删 → `No such file or directory` / `cannot find source file`
- 文件有、列表里没有 → 该图片/字体不参与编译，链接时 `undefined reference`

### 1.1 双向核对脚本

在项目根目录运行（核对哪些引用缺文件、哪些文件漏登记）：

```bash
# 列表引用了但文件不存在
for f in $(grep -o 'generated/images/[^"]*\.c' components/UI/CMakeLists.txt) \
         $(grep -o 'generated/fonts/[^"]*\.c'  components/UI/CMakeLists.txt); do
    [ -f "components/UI/$f" ] || echo "MISSING in disk: $f"
done
# 文件存在但列表没登记
for f in components/UI/generated/images/*.c components/UI/generated/fonts/*.c; do
    b=$(basename "$f")
    grep -q "$b" components/UI/CMakeLists.txt || echo "NOT in cmake: $f"
done
echo "check-done"
```

输出干净（只有 `check-done`）才算对齐。

### 1.2 偷懒重生成 SRCS 片段

文件增删较多时，直接重生成图片/字体两段 SRCS，贴回 CMakeLists：

```bash
for f in components/UI/generated/images/*.c; do echo "        \"generated/images/$(basename "$f")\""; done
for f in components/UI/generated/fonts/*.c;  do echo "        \"generated/fonts/$(basename "$f")\"";  done
```

> CMakeLists 的 `sdk/*.c` 和 `simulator/dummy.c` 行**保持不动**，
> 它们不随 `generated/` 替换而变。`REQUIRES lvgl` 也不要改。

---

## 2. 修正字体文件的 LVGL 头引用

AnyUI 生成的字体文件用条件 include，`#else` 分支是模拟器路径 `lvgl/lvgl.h`，
ESP-IDF 组件库里不存在该路径，会报：

```text
fatal error: lvgl/lvgl.h: No such file or directory
```

一键修正所有字体文件（把 `lvgl/lvgl.h` 改成 `lvgl.h`）：

```bash
for f in components/UI/generated/fonts/*.c; do
    sed -i 's#include "lvgl/lvgl.h"#include "lvgl.h"#' "$f"
done
grep -rln 'lvgl/lvgl.h' components/UI/generated/fonts/ || echo "ALL FIXED"
```

不要用 `LV_LVGL_H_INCLUDE_SIMPLE` 宏绕过，直接改 include 更干净。

---

## 3. 修正 `lv_arc_get_value()` 格式符

LVGL 9 中 `lv_arc_get_value()` 返回 `int32_t`。ESP-IDF 开了 `-Werror=format`，
AnyUI 生成的 `lv_label_set_text_fmt(label, "%d", lv_arc_get_value(arc))` 会报：

```text
format '%d' expects argument of type 'int', but argument has type 'int32_t'
```

定位所有出现处：

```bash
grep -n 'lv_arc_get_value' components/UI/generated/setup_scr_dashboard.c
```

把每处的 `"%d"` 改成 `"%ld"`（其余参数不动）。批量修可用：

```bash
sed -i 's/"%d", lv_arc_get_value/"%ld", lv_arc_get_value/g' \
    components/UI/generated/setup_scr_dashboard.c
```

---

## 4. 给 `setupUi()` 补声明

`main/system_init.c` 调用 `setupUi()`，但 AnyUI 生成的 `setup_ui.h` 常只声明
`setup_scr_dashboard()`，漏掉 `setupUi()`，会报：

```text
implicit declaration of function 'setupUi'
```

确认 `components/UI/generated/setup_ui.h` 里同时有这两行（缺则补）：

```c
lv_obj_t * setup_scr_dashboard(void);
void setupUi(void);
```

---

## 5. 核对 `sdk/ui.c` 引用的控件名仍存在（关键）

`sdk/ui.c` 不被覆盖，它通过 `extern lv_obj_t *` 引用一批由
`generated/setup_scr_dashboard.c` 定义的全局控件。**新 UI 一旦改名或删除这些
控件，链接就报 `undefined reference to scr_dashboard_*`。**

`ui.c` 目前引用的控件分两组：

**A. 系统监控刷新组**（`ui_system_monitor_event_cb`，BSP→UI 数据刷新）：

```
scr_dashboard_label_runtime_cpu_core1_usage   scr_dashboard_label_runtime_cpu_core2_usage
scr_dashboard_label_cpu_02_value              scr_dashboard_label_cpu_03_value
scr_dashboard_label_cpu_total_value           scr_dashboard_slider_memory_usage
scr_dashboard_label_memory_usage_value        scr_dashboard_label_ram_value
scr_dashboard_label_psram_usage_value         scr_dashboard_slider_psram_usage
scr_dashboard_label_psram_free_value          scr_dashboard_label_psram_total_value
scr_dashboard_label_heap_free_value           scr_dashboard_label_heap_min_free_value
scr_dashboard_label_heap_max_block_value      scr_dashboard_label_runtime_value
scr_dashboard_label_chip_temp_value           scr_dashboard_label_cpu_freq_value
scr_dashboard_label_task_count_value
```

**B. 亮度滑块组**（`ui_attach_brightness_slider`，UI→BSP 背光调节）：

```
scr_dashboard_slider_82EIlsYJ                 scr_dashboard_label_runtime_bright__data
```

### 5.1 一键核对

```bash
for n in \
  scr_dashboard_label_runtime_cpu_core1_usage scr_dashboard_label_runtime_cpu_core2_usage \
  scr_dashboard_label_cpu_02_value scr_dashboard_label_cpu_03_value scr_dashboard_label_cpu_total_value \
  scr_dashboard_slider_memory_usage scr_dashboard_label_memory_usage_value scr_dashboard_label_ram_value \
  scr_dashboard_label_psram_usage_value scr_dashboard_slider_psram_usage scr_dashboard_label_psram_free_value \
  scr_dashboard_label_psram_total_value scr_dashboard_label_heap_free_value scr_dashboard_label_heap_min_free_value \
  scr_dashboard_label_heap_max_block_value scr_dashboard_label_runtime_value scr_dashboard_label_chip_temp_value \
  scr_dashboard_label_cpu_freq_value scr_dashboard_label_task_count_value \
  scr_dashboard_slider_82EIlsYJ scr_dashboard_label_runtime_bright__data; do
    grep -q "lv_obj_t \* $n = NULL;" components/UI/generated/setup_scr_dashboard.c \
        && echo "OK   $n" || echo "MISS $n"
done
```

> 上面这份控件清单应与 `sdk/ui.c` 顶部的 `extern` 块保持同源。改了 ui.c 后，
> 记得把本节清单一并更新，避免下次替换时漏检。

### 5.2 出现 MISS 时怎么改

- **被改名**：在 AnyUI 里把控件名改回原名（推荐），或同步改 `sdk/ui.c` 顶部
  `extern` 声明和 `ui_system_monitor_event_cb` 里的引用。
- **被删除**：从 `sdk/ui.c` 删掉对应的 `extern` 声明 + 刷新调用两处。
  （历史案例：某次新 UI 删了 runtime 组的内存占用 arc
  `scr_dashboard_label_runtime_memory_usage`，内存数据仍由
  `slider_memory_usage` + `label_memory_usage_value` 显示，故直接删引用即可，
  无数据丢失。）
- **亮度滑块控件名变了**：`scr_dashboard_slider_82EIlsYJ` 是 AnyUI 随机后缀名，
  若新导出变了，需同步 `sdk/ui.c` 里 `ui_attach_brightness_slider` /
  `ui_brightness_slider_event_cb` 中的引用。建议在 AnyUI 里给该滑块固定一个
  语义名（如 `slider_runtime_bright`）以后就稳定了。

---

## 6. 检查内置字体是否启用

新 UI 若用到 LVGL 内置字体（如 `lv_font_montserrat_14`），需确认 sdkconfig 已开：

```bash
grep "CONFIG_LV_FONT_MONTSERRAT_14=y" sdkconfig
```

未开则会报 `'lv_font_montserrat_14' undeclared`。通过
`./agentic/idf_build.sh menuconfig` →
`Component config → LVGL → Font usage` 勾选对应字号，或在 `sdkconfig.defaults`
里加 `CONFIG_LV_FONT_MONTSERRAT_14=y`。

> 当前项目已启用 `montserrat_14`。仅当新 UI 引入新字号时才需关注本节。

---

## 7. 构建验证

```bash
./agentic/idf_build.sh
```

构建命令需要 **10 分钟超时**。`generated/` 大量文件变动时建议先 reconfigure
（改了 CMakeLists 会自动触发）。通过标志：

```text
Project build complete.
```

已知可忽略的警告：`setup_scr_dashboard.c` 中
`unused variable 'global_statusbar'` —— AnyUI 生成代码固有，非错误。

---

## 8. 常见错误对照

| 报错 | 原因 | 修复 |
| --- | --- | --- |
| `cannot find source file: generated/images/xxx.c` | CMakeLists 列表里的图片在新 UI 中已删 | §1 同步图片清单 |
| 某图片/字体显示不出 + 链接 `undefined reference` 到该资源符号 | 新增的图片/字体没登记进 CMakeLists | §1 补登记 |
| `lvgl/lvgl.h: No such file or directory` | 字体文件 `#else` 分支引用模拟器路径 | §2 改为 `lvgl.h` |
| `format '%d' ... has type 'int32_t'` | `lv_arc_get_value` 返回 int32_t | §3 `%d` 改 `%ld` |
| `implicit declaration of function 'setupUi'` | `setup_ui.h` 没声明 `setupUi()` | §4 补 `void setupUi(void);` |
| `undefined reference to scr_dashboard_*` | 新 UI 改名/删了 ui.c 引用的控件 | §5 核对并对齐 ui.c |
| `'lv_font_montserrat_14' undeclared` | sdkconfig 没启用该内置字体 | §6 menuconfig 勾选 |

---

## 9. 替换后改动过的文件清单

```text
components/UI/CMakeLists.txt                    # §1 图片/字体清单
components/UI/generated/setup_ui.h              # §4 setupUi 声明
components/UI/generated/setup_scr_dashboard.c   # §3 arc 格式符
components/UI/generated/fonts/*.c               # §2 LVGL 头引用
components/UI/sdk/ui.c                          # §5 仅当控件名变化时
```

`sdk/` 其余文件（`cmd.c`/`evt.c`/`scr_list.c`）和 `simulator/dummy.c`
（提供 `process_command()` 占位实现，被 `sdk/cmd.c` 引用）正常情况下无需改动。
