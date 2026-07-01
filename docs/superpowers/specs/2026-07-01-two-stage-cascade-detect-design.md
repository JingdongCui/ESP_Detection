# 两级级联检测链路设计

日期：2026-07-01
状态：待实现（模型留桩阶段）

## 背景与目标

现有视觉链路是单模型结构（1:1 复刻 esp-who），`vision_model_run` 为占位桩（恒返回 0 框）。
需要把画框链路与模型接口改造为**两级级联检测**：

1. 摄像头获取原始帧，**原图全程不被破坏**。
2. 模型1（waybill 单分类）跑整张原图 → 返回面单坐标 → 画框。
3. 同一帧，按面单坐标从原图裁剪 ROI 输入模型2（三分类：极兔/中通/韵达）→ 返回 logo 坐标 → 画框。

本次范围：**只改链路和接口，模型仍留桩**。要求接口以"单张图片 → 模型结果"为原子单元抽象，
后续真实 espdl 模型能直接替换桩体、丝滑对接，级联编排层无需改动。

## 复刻对应关系（硬性要求）

整条视觉链路 1:1 复刻 esp-who / esp-dl，本次新增部分同样照搬其结构：

- **级联封装范式** → esp-dl `HumanFaceDetect`（内部 MSR→crop→MNP→映射回原图）：
  对外一个入口，内部两级串联，第二级结果加偏移映射回原图坐标。
- **原子模型接口** → esp-dl `dl::detect::Detect::run(const dl::image::img_t &img)`：
  输入单张图（`img_t{data,width,height,pix_type}`），输出 `result_t` 列表。
- **外层检测框架** → esp-who `WhoDetect`（`set_model` / `run` / `rescale_detect_result` / `result_cb`），
  本项目已由 `vision_detect.c` 复刻，本次保持不动。
- **rescale** → `WhoDetect::rescale_detect_result`（框坐标乘 inv_rescale），
  本项目 `vision_detect.c` 已复刻为原图→预览坐标缩放 + clip。
- **画框** → esp-dl `draw_detect_results_on_img` / `draw_hollow_rectangle`，
  本项目 `vision_draw.c` 已复刻为纯 C 空心矩形。

## 数据流（原图不被破坏）

```
fetch(core0) -> ringbuf -> detect(core1) peek 原图 fb.buf(RGB888 裸指针，只读)
  |- vision_model_run(fb.buf, w, h, dets, max)        <- 级联编排层
  |    |- vision_detector_run(s_waybill, 整张原图)     -> 面单框列表
  |    |- 取最高分 1 个面单框                          [stage=WAYBILL]
  |    |- 按面单 box 从原图【拷贝】裁剪 ROI 子图 -> resize
  |    |- vision_detector_run(s_logo, ROI 子图)        -> logo 框(ROI 局部坐标)
  |    |- logo 框 + 面单左上角偏移 -> 映射回原图坐标    [stage=LOGO]
  |- dets[] 全在原图坐标系 -> 统一 rescale 到预览坐标(复刻 rescale_detect_result)
  |- vision_draw_save_result(带 stage 的框 + ev 文本)
display(core0) -> PPA 缩放原图到 preview_buf 副本 -> 按 stage 分色画框 -> blit 到 fb
```

原图不被破坏的保证：
- 推理只读 `fb.buf` 裸指针，从不写。
- 裁剪 ROI 是从原图**拷贝**出独立子图给模型2，不改原图。
- 画框只写 `preview_buf`（PPA 缩放后的副本），与原图物理隔离。

## 接口设计（两层）

可扩展性的核心：把"单图 → 结果"作为可替换的原子单元（对齐 esp-dl `Detect::run`），
级联作为组合层。后续真实模型只替换原子层桩体，编排层不动。

### 第一层 — 原子模型接口（单图 → 结果）

```c
// 不透明句柄。桩阶段为占位；真实阶段内部持 dl::detect::Detect* 及其前后处理器。
typedef struct vision_detector vision_detector_t;

// 加载一个检测模型。
// 桩：返回一个占位句柄（携带模型标识，供桩产不同假数据）。
// 真实：内部 new DetectImpl + load_model(model_name)（C++ 薄封装 extern "C"）。
vision_detector_t *vision_detector_load(const char *model_name);
void vision_detector_free(vision_detector_t *det);

// 单张 RGB888 图片输入 -> 该图坐标系的检测框输出。对齐 Detect::run(img_t)。
// img: width x height x 3 RGB888；out: 写入检测框(最多 max)；返回框数，<0 失败。
// 真实阶段桩体替换为：组装 img_t{img,width,height,RGB888} -> model->run(img)
//                    -> 遍历 result_t 拷进 out[]。
int vision_detector_run(vision_detector_t *det,
                        const uint8_t *img, int width, int height,
                        vision_model_det_t *out, int max);
```

约定整条链路及裁剪出的 ROI 均为 RGB888（与现有链路一致），故原子接口不引入像素格式参数
（YAGNI）；真实阶段在 C++ 封装内部把 `(img,w,h)` 组装成 `img_t` 时填 RGB888。

### 第二层 — 级联编排（组合原子单元，签名不变）

```c
// 签名不变：输入整张原图，输出带 stage 的 dets[]，返回总框数。级联全在内部。
int vision_model_run(const uint8_t *buf, int w, int h,
                     vision_model_det_t *dets, int max);
```

内部流程：
1. `vision_detector_run(s_waybill, buf, w, h, ...)` -> 面单框列表。
2. 取最高分面单框写入 `dets[0]`（stage=WAYBILL）。无面单则返回 0。
3. 按面单 box 从原图裁剪 ROI 子图（独立缓冲）。
4. `vision_detector_run(s_logo, roi, roi_w, roi_h, ...)` -> logo 框（ROI 局部坐标）。
5. 每个 logo 框加面单左上角偏移，映射回原图坐标，写入 `dets[1..]`（stage=LOGO）。
6. 返回总框数。

`vision_model_init`：`vision_detector_load` 加载两个模型存 static 句柄（`s_waybill`、`s_logo`），
须在 detect 任务创建前的单线程阶段调用。桩恒返回 true。

## 数据结构改动

### `vision_model.h`

```c
typedef enum {
    VISION_STAGE_WAYBILL = 0,   // 模型1：面单框（category 恒 0）
    VISION_STAGE_LOGO    = 1,   // 模型2：logo 框（category 0=极兔 1=中通 2=韵达）
} vision_stage_t;

typedef struct {
    int box[4];            // 原图坐标系 [left_up_x, left_up_y, right_down_x, right_down_y]
    int category;
    float score;
    vision_stage_t stage;  // 新增：属于哪一级
} vision_model_det_t;
```

`vision_model_get_class_probs` 语义明确为来自模型2（logo 三分类）。

### `vision_internal.h`

`vision_det_result_t` 同步新增 `vision_stage_t stage`（预览坐标系框）。
`vision_detect.c` 把 `dets[i].stage` 透传到 `items[i].stage`。
`VISION_MAX_BOXES` 保持 8（最高分 1 面单 + 若干 logo，余量足够）。

## 桩假数据策略（验证两级链路）

模型仍留桩，桩需产假数据验证整条两级画框 + 偏移映射链路：

- **模型1 桩**（`vision_detector_run` 当句柄为 waybill）：在原图坐标系产 1 个假面单框，
  居中碰壁反弹（沿用原假框逻辑，static 保持位置），stage 由编排层置为 WAYBILL，category=0。
- **模型2 桩**（句柄为 logo）：在输入 ROI 子图**内部**产 1 个假 logo 框（ROI 中心的小框），
  category 在极兔/中通/韵达间轮换，score 给固定值。

验证点：面单红框在动，logo 绿框始终落在面单框**内部**并跟随移动 —— 证明裁剪偏移映射正确。

## 画框分色（`vision_draw.c`）

- `draw_hline` / `draw_vline` / `draw_hollow_rect` 增加 `(r, g, b)` 颜色参数。
- `draw_results_on_buf` 按 `items[i].stage` 选色：
  - 面单（WAYBILL）= 红 `{255,0,0}`
  - logo（LOGO）= 绿 `{0,255,0}`
  - 写入仍按预览缓冲的 B,G,R 字节序（见现有颜色宏说明）。
- 画框顺序：先画面单后画 logo，logo 在面单内部后画覆盖，视觉正常。

## UI 文本 ev（来源改为模型2）

- `ev.company` / `ev.confidence` 改由 **stage==LOGO 的最高分框**决定（三分类结果）。
- 无 logo 框时填"无目标"。
- `ev.jt/zt/yd` 概率来自 `vision_model_get_class_probs`（模型2）。
- 类名映射 `{"极兔","中通","韵达"}` 保持。
- `fps_x10` / `infer_time_ms` 保持现状（`infer_time_ms` 为两级合计耗时）。

## 后续真实模型对接路径（丝滑替换点）

本次留桩后，真实模型接入只需：
1. 把 `vision_detector_load` 桩体换成 esp-dl 模型加载（挂 SPIFFS + new DetectImpl + load_model）。
2. 把 `vision_detector_run` 桩体换成：组装 `img_t{img,w,h,RGB888}` -> `model->run(img)`
   -> 遍历 `std::list<result_t>` 拷进 `out[]`（C++ 薄封装 `extern "C"` 暴露本接口）。
3. 提供两个模型文件（waybill、logo），文件名走 ASCII（见项目 spiffsgen 中文名 bug 约束）。

级联编排 `vision_model_run`、rescale、画框、ev 组装全部不变。

## 不做的事（YAGNI）

- 不引入像素格式参数（全链路 RGB888）。
- 不支持多面单同时送模型2（本次只取最高分面单）。
- 不新增独立编排模块（级联内聚在 vision_model 层，对齐 esp-dl HumanFaceDetect）。
- 不改动 fetch/display/帧总线/PPA 等已复刻链路。

## 验证方式

- 编译通过（`./agentic/idf_build.sh`，10 分钟超时）。
- 烧录后经 RTT 观察：每帧日志含面单框 + logo 框数量。
- 屏幕预览：红色面单框碰壁反弹，绿色 logo 框始终在红框内部跟随移动。
