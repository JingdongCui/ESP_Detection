# esp-who 视觉流水线移植方案（摄像头→预览→AI→画框）

> 目标项目：`E:\ESP32P4_Detection-main`
> 编写日期：2026-06-26
> 状态：已确认设计，待实现

---

## 一、背景（为什么做这件事）

当前项目的显示链路（LVGL + Dashboard）完整、esp-dl 框架与两个 `.espdl` 模型已就位、
app_ui 数据通道（`app_ui_post_vision_result`）已写好，但**视觉采集与推理链路是空的**：

- `bsp_cam_sensor.c` 实现不存在 —— 只有头文件契约 `bsp_cam_sensor.h` + 一个**注释掉的**
  `ethernet_app.c` 消费者，靠链接器 `--gc-sections` 剔除未引用静态函数才编译通过。
- 此前的 `vision_app`（采集 + 推理 + ROI + CAL + 预览画框）已被删除。
- Dashboard 里预留了预览容器 `scr_dashboard_cont_live_vedio`（640×375）但当前是空的。

**目标**：1:1 维持 esp-who 的链路（**V4L2 采集**），用项目自己的模型
`roi224_515.espdl`（224×224，3 类，检测型）跑通 **摄像头 → 预览 → 推理 → 画框**
端到端，结果经现有 `app_ui_post_vision_result()` 上屏。

---

## 二、已锁定的设计决策

| # | 决策点 | 选择 | 说明 |
|---|--------|------|------|
| 1 | 采集架构 | **V4L2 重包同名接口** | 新增 `espressif/esp_video`，照搬 esp-who V4L2 流程，但对外重包成现有 `cam_sensor_get_frame()/return_frame()` 契约，`ethernet_app.c` 无需改动 |
| 2 | 像素格式 | **ISP/V4L2 直出 RGB565** | LVGL canvas 零转换贴图；喂模型时由 `ImagePreprocessor` 从 `DL_IMAGE_PIX_TYPE_RGB565LE` 内部转模型输入（已确认支持） |
| 3 | 预览显示 | **LVGL canvas 嵌入容器** | 在 `scr_dashboard_cont_live_vedio` 内建 canvas，帧缩放到 640×375 贴图，检测框用 LVGL 图层画，与 Dashboard 融合 |
| 4 | 推理双核 | **原生 Model API + 多核** | 绕过写死单核的 `DetectImpl`，用 `dl::Model` + `ImagePreprocessor` + `*Postprocessor`，调 `model->run(RUNTIME_MODE_MULTI_CORE)` |
| 5 | 流程范围 | **检测流水线 + ROI 门控** | 采集→ROI 预筛选门控→（有盒才）推理→解码+NMS→画框；CAL 标定后续再补 |
| 6 | 任务架构 | **预览与推理解耦（双任务）** | 采集+预览一个任务跑摄像头 30fps 流畅画面；推理一个任务拿最新帧跑、出框后异步叠加。画面出现不等模型加载 |
| 7 | 加载期占位 | **空预览框** | 模型加载/首帧前预览容器保持空（深色底 0x0B1517），不放占位提示 |
| 8 | 推理门控 | **颜色 ROI 预筛选** | 推理前用轻量颜色阈值（HSV/RGB）找疑似快递盒区域，**没疑似盒就跳过推理、立即取下一帧，不花 140ms**；有盒才进模型。参数需实测调 |
| 9 | 以太网上传 | **仅识别成功时传带框 JPEG** | 识别成功→CPU 在帧副本上画框→`esp_new_jpeg` 编码→以太网上传。重启用现注释掉的 `ethernet_app` |

### 关键技术依据

- **esp-who P4 用的是 V4L2**：`who_p4_cam.cpp` 含 `esp_video_init` + `open(/dev/video)` +
  `VIDIOC_S_FMT/REQBUFS/QBUF/DQBUF` + mmap 多缓冲，依赖 `espressif/esp_video` 组件。
- **当前项目没有 esp_video**：只有底层 `esp_driver_cam`+`esp_driver_isp`+`esp_cam_sensor`。
  要维持 esp-who 链路必须新增 `esp_video`。
- **esp-dl 高层 `DetectImpl::run()` 把 `m_model->run()` 写死单核**（`dl_detect_base.cpp:64`）。
  要双核必须走原生 `dl::Model` + 手动组装预处理/后处理，调
  `model->run(RUNTIME_MODE_MULTI_CORE)`，靠 esp-dl 内置 `module_forward_dual_core`
  （信号量同步两核算 conv/gemm）加速。这正是被删的 `app_yolo.cpp` 的做法。
- **SC2336 支持 MIPI 2lane RAW8 1024×600 30fps**，匹配现有 `bsp_cam_sensor.h` 契约。

---

## 三、关键文件与改动

> **分层原则**（对齐现有项目约定）：
> - **BSP 层**只懂硬件，极简、零业务（如 `bsp_lcd.c` 仅 `BSP_LCD_Init()` + handle getter）。
> - **App 层**放业务逻辑：`include/xxx.h` 对外 API，`xxx_internal.h`（组件根目录）组件内共享，
>   核心 `.c` 管任务/数据，专职 `.c` 管单一职责。
> - 摄像头硬件归 BSP（`bsp_cam_sensor.c`），视觉业务（采集/推理/ROI/预览/上传）归新组件 `vision`。

### 3.1 新增组件 `components/vision`（C 为主，仅推理引擎 C++）

文件结构与职责边界（每个 `.c` 单一职责，互不越界）：

| 文件 | 层/语言 | 职责（边界） |
|------|---------|------|
| `include/vision.h` | 对外 | 公共 API，**只暴露 `vision_start()`**（main 调用） |
| `vision_internal.h` | 组件内(根目录) | 组件内共享：最新帧缓冲结构 + 各模块函数声明 |
| `vision_app.c` | 协调 | **大脑**：建「采集+预览任务」+「推理任务」，管最新帧共享缓冲，串起下面各模块。不碰具体硬件/推理细节 |
| `vision_detect.cpp` | 推理(C++) | **唯一 C++**：`dl::Model` 加载（`/storage/roi224_515.espdl`）+ `ImagePreprocessor` + `EspDetPostprocessor` + `run(RUNTIME_MODE_MULTI_CORE)` + 解码NMS。**只懂喂图吐框**，不碰摄像头/LVGL/网络。`extern "C"` 对 C 暴露 |
| `vision_roi.c` | 门控 | ROI 颜色阈值（HSV/RGB）扫描帧，判断有无疑似快递盒。参考被删 `roi_algorithm.c`（y_min=150, sat_max=70, rgb_delta=50，需实测调）。**只判有无盒**，返回 bool/ROI |
| `vision_preview.c` | 显示 | 在 `scr_dashboard_cont_live_vedio` 建 `lv_canvas`（RGB565 buf 在 PSRAM）；帧 PPA 缩放 1024×600→640×375 贴图；检测框叠加在 canvas 之上独立 LVGL 图层；坐标 224→640×375 映射。**只懂画，LVGL 操作集中在此** |
| `vision_upload.c` | 上传 | 识别成功时：帧副本 CPU 画框 → `esp_new_jpeg` 编码 JPEG → 交以太网上传。**只懂打包上传** |
| `CMakeLists.txt` | — | 注册源文件，`REQUIRES bsp app_ui Ethernet_app espressif__esp-dl espressif__esp_lvgl_adapter lvgl esp_driver_ppa espressif__esp_new_jpeg` |

文字结果（快递公司/置信度/状态/FPS）走**现有 `app_ui_post_vision_result()`**，vision 不重复造仪表盘刷新。
app_ui 只管低频文字/状态（100ms 节奏），vision_preview 管高频实时视频（30fps），两者职责分离。

### 3.2 摄像头采集（V4L2 重包）

- **`components/bsp/bsp_cam_sensor.c`（新建）** — 内部用 esp-who `who_p4_cam.cpp` 的 V4L2 流程：
  ```
  esp_video_init(csi_config)
    → open(ESP_VIDEO_MIPI_CSI_DEVICE_NAME)
    → VIDIOC_S_FMT (RGB565)
    → VIDIOC_REQBUFS + mmap 多缓冲
    → VIDIOC_QBUF / DQBUF 循环取帧
  ```
  对外实现现有 `bsp_cam_sensor.h` 契约：`cam_sensor_init/start/stop/deinit` +
  `cam_sensor_get_frame(data,size,w,h,timeout)` / `cam_sensor_return_frame(data)`。
  引脚参考 `bsp_cam_sensor.h`（GPIO8=SCL, GPIO7=SDA）。
- **`components/bsp/CMakeLists.txt`** — `bsp_cam_sensor.c` 加入 SRCS；REQUIRES 加 `esp_video`。
- **`components/bsp/idf_component.yml`** — 加 `espressif/esp_video` 依赖。

参考实现：`E:/esp-who-master/components/who_peripherals/who_cam/who_p4_cam/who_p4_cam.cpp`

### 3.3 系统集成

- **`main/system_init.c`** — 初始化序列里**显式调 `cam_sensor_init(&cam_cfg)` + `cam_sensor_start()`**
  （同 `BSP_LCD_Init()` 风格，硬件初始化集中在 system_init），再在 `app_ui_init()` 之后调
  `vision_start()`；以太网若需上传，恢复 `ethernet_app_start()`（当前注释掉）。
  `cam_sensor_config_t cam_cfg` 在此填（GPIO8/7, 1024×600）。
- **`main/CMakeLists.txt`** — `REQUIRES` 加 `vision`。

### 3.3b 以太网上传（带框 JPEG）

- **`components/Ethernet_app/ethernet_app.c`** — 现 `ethernet_app_start()` 注释掉、
  `send_image_packet()` 传原始 RGB888 帧。改造：恢复启用；提供一个"上传带框 JPEG"入口，
  供 `vision_upload` 在识别成功时调用（传入已编码的 JPEG buffer + 元数据如 class/分数/框）。
- **`components/Ethernet_app/CMakeLists.txt`** — REQUIRES 视情况加 `espressif__esp_new_jpeg`
  （若编码放在 vision_upload 里则此处不需要）。
- 触发：**仅识别成功**那一帧，由推理任务驱动，避免持续占带宽。

### 3.4 模型确认（实现前必须落实）

- `roi224_515.espdl`：确认输出类型（检测带 box 的 ESPDet/Pico/YOLO vs 纯分类）、
  输入 224×224 RGB、3 类、阈值。决定后处理走哪个 `*Postprocessor` 及 score/NMS 阈值。
- `app_ui_company_from_class_id`（`app_ui.c:33`）已把 class 0/1/2 → 极兔/中通/圆通，
  与"3 类"吻合，沿用。

---

## 四、数据流（端到端）

```
SC2336 (MIPI-CSI RAW8 1024x600)
  → ISP RAW8→RGB565 (esp_video/V4L2 内部)
  → V4L2 DQBUF 取帧  ←─ bsp_cam_sensor.c 重包成 cam_sensor_get_frame()

【采集+预览任务 @core0, 摄像头 30fps】
  get_frame → PPA 缩放 1024x600→640x375 → lv_canvas 贴图 → 更新"最新帧"共享缓冲
  ★画面流畅显示，不受推理阻塞★

【推理任务 @core1, 异步】
  取"最新帧"(RGB565)
    → ROI 颜色预筛选(轻量, 几 ms): 找疑似快递盒区域
        ├─ 无疑似盒 → 跳过, 立即取下一帧   ★不花 140ms★
        └─ 有疑似盒 ↓
    → img_t{RGB565LE} → ImagePreprocessor(→224x224 模型输入)
    → model->run(RUNTIME_MODE_MULTI_CORE)   ← 双核, ~140ms
    → EspDetPostprocessor: 解码+NMS → list<result_t>{category,score,box}
    → 识别成功(有框):
        ├─ 框坐标 224→640x375 映射 → 更新 canvas 之上的检测框图层（屏幕显示）
        ├─ CPU 在帧副本上画框 → esp_new_jpeg 编码 → 以太网上传（带框 JPEG）
        └─ 填 app_ui_vision_result_t → app_ui_post_vision_result() → Dashboard 刷新
```

> **时序**：摄像头就绪后第一帧即贴图，画面出现**不等模型加载**；推理任务后台加载模型
> （百 ms~秒级），加载完才开始 ROI 门控 + 推理。无快递盒时推理任务几 ms 空转一轮、
> 不占 140ms；仅在识别成功时画框、上传 JPEG。

---

## 五、任务 / 核分配

- **采集+预览任务** @core0：`get_frame → PPA 缩放 → 贴 canvas → 更新最新帧缓冲`，
  跑摄像头 30fps，流畅画面不受推理阻塞。贴图通过 `BSP_LVGL_Lock/Unlock` 与 LVGL 串行化。
- **推理任务** @core1：后台加载模型 → 循环取最新帧 → **ROI 颜色门控**（无盒跳过、几 ms）
  →（有盒）预处理 → `model->run(多核)` → 解码+NMS → 识别成功则：更新检测框图层 +
  画框编码 JPEG 上传 + 投递 app_ui。
- **推理双核**：靠 esp-dl `RUNTIME_MODE_MULTI_CORE` **算子级双核**（非任务分核），
  两核协同算 conv/gemm。
- **app_ui 刷新任务**：保持 core0（现状不变）。
- **帧共享**："最新帧"机制 —— 推理任务取采集任务的最新一帧，不排队堆积，避免延迟累积。
  检测框画在 canvas 之上的独立 LVGL 图层，推理出框后只更新框图层，不阻塞预览贴图。

---

## 六、验证（端到端）

**实现前**

1. 确认 `roi224_515.espdl` 的输入尺寸/格式与输出张量结构，定后处理器。
   无现成离线命令则在固件里加载后打印 `model->print()` / 输出 tensor 名。

**实现后**（按 CLAUDE.md 工具链）

2. `./agentic/idf_build.sh`（超时 600000ms）— 修复编译/链接错误。
3. `python3 agentic/esp_target.py flash-and-run build/ --app-only`。
4. RTT 后台读日志，确认：摄像头初始化 OK、每帧推理耗时、检测框数量/类别/分数：
   ```
   python3 agentic/rtt_reader.py --elf build/<proj>.elf --output agentic/.esp-agent/rtt.log --kill-existing
   ```
5. 看屏：摄像头就绪后 `scr_dashboard_cont_live_vedio` **立即**出现流畅实时画面（不等模型）；
   模型加载完后检测框异步叠加；Dashboard 的识别状态/快递公司/置信度/FPS/推理耗时标签随结果刷新。
6. 量推理延迟（RTT 打印 pre/model/post），确认双核相比单核确有提速；不足再调。
7. **ROI 门控验证**：画面无快递盒时，RTT 应显示推理被跳过（不出现 140ms 推理日志、空转快）；
   有盒时才触发推理。实测调颜色阈值，平衡漏检/误触发。
8. **上传验证**：识别成功时，以太网对端应收到带框 JPEG；无盒/未识别时不上传。

---

## 七、后续（本次不做）

- CAL 标定按钮 + 阈值在线调参（ROI 门控参数本次先用固定值，实测手调）。
