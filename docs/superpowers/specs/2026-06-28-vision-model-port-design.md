# 设计：移植旧识别模块到新零拷贝架构（接入真实模型）

日期：2026-06-28
状态：待评审

## 1. 问题与目标

新项目 `E:\ESP32P4_Detection` 的 `components/vision/vision_detect.c` 当前是**假框桩**
（碰壁反弹的固定框），仅验证 `peek → rescale → save → UI` 链路。需要把旧项目
`E:\test1_black\components\vision_app` 里**已验证可用的真实识别逻辑**移植过来，接入真实
ESP-DL 模型，做快递 logo（极兔/中通/韵达）三类目标检测。

旧项目用同步轮询 `BSP_Camera_GetFrame` 自取帧、自画框；新项目已改为三任务零拷贝 ringbuf
架构（fetch/display/detect）。因此**只移植"前处理 + 模型推理 + 后处理"内核，不移植旧的编排
与预览层**——后者新架构已用 `vision_app.c` + `vision_draw.c` 实现。

## 2. 已确认的范围决策

1. **整套搬**，含重量级 ROI 预定位（颜色阈值分割 + 连通域 + 形态学 + 角点/边缘裁剪 + CAL 校准）。
2. **保留 `vision_detect.c` 纯 C**，新写一个 C 封装层 `vision_model.cpp` 把 C++ AppYOLO 包成 C 接口。
3. **全部 19 个模型打包**进 storage，运行时可选实际加载哪个（首版固定默认，选择 UI 留后续）。
4. 模型输入 **224×224**，**3 类**（极兔/中通/韵达），conf 阈值 **0.25**（沿用旧值）。
5. CAL 校准用**默认阈值**（方案 A），`roi_tuning` 代码搬入但 CAL 按钮暂不接线，留接口待新 UI。
6. **分区重排**：factory 6M→4.5M、storage 5M→11M（紧贴消除空隙），装下 10.70M 全部模型。
7. **类别映射**默认 `0=极兔, 1=中通, 2=韵达`（未与训练标签核对，先按此跑通，实测错位再调）。
8. **首版默认模型** `train-11-zt-explore.espdl`。

## 3. 两个硬约束（必须随移植一并处理）

### 约束 1 — 分区容量

- 实测 `model/` 下 19 个 `.espdl` 合计 **10.70 MB**（11,221,936 字节）。
- 现有 `partitions.csv`：`factory 6M [0x10000..0x610000]`，之后空 4.94M 浪费，`storage 5M
  [0xb00000..0x1000000]`。Flash 总 16M。
- `build/sample_project.bin` 当前 2.56M，接 esp-dl 后预计涨到 3.5~4M，6M factory 用不满。
- 板载 SD/TF 槽默认禁用（需改电阻 R231→R100 焊接），不可行。
- **方案**：重排分区表为
  ```
  factory  app  factory  0x10000   4608K   # 4.5M
  storage  data spiffs   0x490000  11264K  # 11M，紧贴 factory
  ```
  （11M SPIFFS 实际可用约 10.7~10.8M，刚够 10.70M 模型；余量~50KB 偏紧，后续若加模型需复核。）
- **影响**：改分区表后必须**全擦 flash 重烧**（`erase` + 全量 `flash`），不能只烧 app。

### 约束 2 — 推理任务栈

- 新架构 `vision_app.c` 给 `vision_det` 任务仅配 `VISION_DET_STACK_SIZE = 4096` 字节。
- 旧项目跑同样的 AppYOLO+ROI 用 **32KB 栈且放 PSRAM**（`xTaskCreatePinnedToCoreWithCaps(...
  MALLOC_CAP_SPIRAM)`）。esp-dl 推理 + ROI 连通域 BFS + std::vector 在 4KB 栈必崩。
- **方案**：把 `VISION_DET_STACK_SIZE` 提到 **32768**，并将 detect 任务创建改为
  `xTaskCreatePinnedToCoreWithCaps(..., MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT)`（核仍钉 core1）。
  fetch/display 任务不动。

## 4. 文件落位

旧文件原样搬入 `components/vision/`（源）与 `components/vision/include/`（头）：

| 旧文件 | 新位置 | 说明 |
| --- | --- | --- |
| `app_yolo.cpp` / `app_yolo.hpp` | `vision/` / `vision/include/` | 原样，C++ 推理核 |
| `yolo_decode.cpp` / `yolo_decode.hpp` | 同上 | 原样，ESPDet-Pico 三尺度解码 |
| `roi_algorithm.c` / `roi_algorithm.h` | 同上 | 原样，纯 C ROI 预定位 |
| `roi_tuning.cpp` / `roi_tuning.h` | 同上 | 原样，CAL 阈值校准（暂不接线） |
| `vision_app.cpp` | **不搬** | 旧同步编排，新架构已有 fetch/ringbuf |
| `vision_preview.cpp` / `vision_preview.h` | **不搬** | 旧预览/画框，新架构 vision_draw.c 已实现 |
| 🆕 `vision_model.cpp` / `vision_model.h` | 新建 | C 封装层，包 C++ AppYOLO 暴露 C 接口 |

## 5. C 封装层接口（`vision_model.h`，纯 C）

```c
#pragma once
#include <stdbool.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct { int category; float score; int box[4]; } vision_model_det_t;

// 挂载 SPIFFS(/storage) + new AppYOLO(cfg) + load_model(默认模型)。
// vision_start 单线程阶段、detect 任务创建前调用。返回 false=失败。
bool vision_model_init(void);

// 对原图 RGB888 跑一次完整检测（内部含 ROI→letterbox→量化→model->run→decode）。
// 输出原图坐标系的框到 out[0..max_out)，返回框数(0~max_out)。-1=推理失败。
int  vision_model_run(const uint8_t *rgb888, int w, int h,
                      vision_model_det_t *out, int max_out);

// 取上次 vision_model_run 的推理耗时(ms)，供 UI infer_time_ms。
int  vision_model_last_infer_ms(void);

// 取上次三类概率×100，供 UI jt/zt/yd 字段。无结果时填 0。
void vision_model_get_class_probs(int *jt, int *zt, int *yd);

#ifdef __cplusplus
}
#endif
```

实现 `vision_model.cpp` 内 `extern "C"`，持 `static AppYOLO *s_yolo`。`detect()` 的
`std::vector<Detection>` 在封装层转成 `vision_model_det_t` 数组吐出，C++ 类型不泄漏到
`vision_detect.c`。默认模型路径 `#define VISION_MODEL_PATH "/storage/train-11-zt-explore.espdl"`，
`#define VISION_MODEL_INPUT_W/H 224`、`CLASSES 3`、`CONF 0.25f`（沿用旧值）。
预留 `vision_model_set_path()`（首版可不实现，留 TODO 供后续运行时选择 UI）。

## 6. 数据流（接入现有零拷贝链路）

```
fetch 任务（不变）         → ringbuf 持帧
display 任务（不变）       → peek → PPA 缩放 → 画框 → 刷 LCD
detect 任务（改造）：
  vision_start 阶段：vision_model_init()（挂 SPIFFS + 建 AppYOLO + load_model）
  循环：
    等 NEW_FRAME → peek 最新帧 fb（RGB888 原图裸 mmap 指针）
    【替换 vision_detect.c 假框块】
      n = vision_model_run(fb.buf, fb.width, fb.height, dets, VISION_MAX_BOXES)
        └ 封装层内：ROI 预定位 → letterbox/resize → BGR→RGB 量化
                   → model->run(多核; >700ms 回退单核) → yolo_decode → 原图坐标框
      填 result.items[0..n)（原图坐标系：category/score/box）
      result.count = n; result.timestamp = fb.timestamp
    rescale（原图→预览，不变） → vision_draw_save_result（不变）
    send_event 投 UI：
      status = n>0?"识别成功":"无目标"
      confidence = 最高分框×100
      company = 命中类别中文名（极兔/中通/韵达）
      fps_x10（现有窗口平均，不变）
      infer_time_ms = vision_model_last_infer_ms()
      jt/zt/yd = vision_model_get_class_probs()
```

**零拷贝竞态注意**：`vision_model_run` 直接读 `fb.buf`（裸 mmap）。推理耗时若不可控且超过
ringbuf 深度时间窗口，该帧可能被 fetch 回收重填。旧项目用同步取帧不存在此问题；新架构下
**若实测推理耗时长导致花屏/错帧，需在封装层入口先 memcpy 一份再处理**（这是已知风险点，先按
零拷贝跑通，实测决定是否加拷贝）。

## 7. 类别映射

`components/UI/sdk/evt.h` 的 `vision_result_event_data_t` 已预留 `jt_a/jt_m`、`zt_a/zt_m`、
`yd_a/yd_m`。模型 `class_id` 默认映射 `{0:极兔, 1:中通, 2:韵达}`，`company` 填命中类别中文名。
**未与训练标签核对**，先按此跑通，实测错位再调封装层映射表（单点改动）。

## 8. CMake 改动

`components/vision/CMakeLists.txt`：

```cmake
idf_component_register(
    SRCS
        "vision_app.c" "vision_detect.c" "vision_draw.c"
        "app_yolo.cpp" "yolo_decode.cpp" "roi_algorithm.c"
        "roi_tuning.cpp" "vision_model.cpp"
    INCLUDE_DIRS "include"
    REQUIRES
        bsp UI esp_driver_ppa esp_lcd system_monitor
        esp-dl esp_timer spiffs esp_psram esp_mm
)
target_compile_options(${COMPONENT_LIB} PRIVATE -ffast-math -O3)
```

顶层 `CMakeLists.txt`：取消注释 `spiffs_create_partition_image(storage model FLASH_IN_PROJECT)`，
把 `model/` 全部打进 storage。

依赖前提：项目须能访问 `esp-dl` 组件（需确认其在 managed_components 或 components 中可用）。

## 9. 分区表改动

`partitions.csv`：
```
factory,  app,  factory, 0x10000,  4608K,
storage,  data, spiffs,  0x490000, 11264K,
```
改后必须 `erase` + 全量烧录（bootloader+分区表+app+storage）。

## 10. 验证计划

1. 编译通过（设 10 分钟超时；首次接 esp-dl 为全量编译）。
2. 全擦 + 全量烧录，确认分区表生效、storage 挂载成功、19 模型可见。
3. RTT 看 `vision_model_init` 加载 `train-11-zt-explore.espdl` 成功、输入缓冲分配成功。
4. 实物对准快递 logo，看 LCD 画框位置正确、UI 显示类别/置信度/帧率/推理耗时/三类概率。
5. 监控推理耗时与是否花屏（判定是否需要封装层拷贝帧）。
6. 检查 detect 任务栈高水位（确认 32KB 足够、未溢出）。

## 11. 待办（实现阶段细化）

- [ ] 分区表改 + 全擦重烧流程
- [ ] detect 任务栈 32KB + PSRAM
- [ ] 搬 6 个旧文件，建 vision_model.cpp/.h
- [ ] 改 vision_detect.c 假框块为 vision_model_run
- [ ] CMake + 顶层 spiffs 打包
- [ ] 编译/烧录/实测
- [ ] 运行时模型选择 UI（后续迭代）
- [ ] CAL 按钮接线（后续迭代）

## 12. 已知风险

- storage 余量~50KB 偏紧，后续加模型需复核容量。
- 零拷贝 peek + 长推理可能错帧，按需加拷贝。
- 类别顺序未核对，实测可能需调映射。
- 不同模型输入尺寸/类别可能不同，运行时切换需 delete+new+load_model（首版不涉及）。
- esp-dl 组件可用性需在实现首步确认。
