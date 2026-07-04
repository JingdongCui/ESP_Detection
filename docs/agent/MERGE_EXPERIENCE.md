# 合并移植经验

本文用于交接后续类似合并任务：把已经验证过的一套能力，从一个旧工程迁移到另一个结构相近但不完全一致的新工程，例如后续往 `teammatesproject_old` 这类工程里重做电机分拣、以太网模拟、视觉模型或传统 ROI 链路移植。

目标不是记录每一次命令的流水账，而是沉淀可复用的判断顺序、验证标准和风险边界。后续做相似合并时，优先复用这里的“先查什么、先保留什么、怎么证明成功”，不要只照搬文件。

## 总原则

不要把“功能移植”“依赖升级”“模型替换”“硬件调参”“UI 补功能”混成一个不可拆的大改动。每条链路先复刻可运行行为，再用实测日志决定是否需要改变底层依赖或架构。

每次移植都按三段走：

1. 建立 baseline：源工程或当前成功工程必须能用同一套模拟器/实机流程跑通。
2. 划清边界：明确本轮迁移的是哪条链路，哪些依赖、配置、模型、UI 和硬件映射不属于本轮。
3. 小步验证：每一类高风险改动后都要能回到一个可解释的 build/flash/monitor/TCP audit 结果。

开始前先并排确认三个对象：

- 源工程：提供已知能跑通的行为，例如电机分拣、TCP 上位机模拟、真实传感器链路、视觉级联链路。
- 当前成功工程：提供最近一次迁移后的落地做法和踩坑修正，优先级高于最早的 old project 记忆。
- 目标工程：只接受必要改动，尽量不顺手重构、不无证据升级依赖、不混入与本轮目标无关的 UI 或硬件改动。

## Checkpoint 和对照

合并前必须有可回退点。工程目录有 git 时在工程目录提交；根目录文档有改动时在根目录提交。不要依赖“我记得改了哪些文件”。

建议每一阶段至少保留这些提交或记录：

- 修改前 checkpoint。
- 依赖或 sdkconfig 改动前 checkpoint。
- 硬件烧录前 checkpoint。
- 模拟器验收通过后的 checkpoint。

对照时不要只看 `git status`。已经提交过的改动不会显示 dirty，但仍然可能改变了工程行为。例如 `old_project` 后来已经提交过 rev 降级配置，所以“当前 clean”不等于“和最初完全一样”。

## 依赖经验

ESP-IDF component manager 的依赖状态要同时看三处：

- `main/idf_component.yml` 和各组件自己的 `idf_component.yml`。
- `dependencies.lock`。
- `managed_components/<component>/idf_component.yml` 和 `.component_hash`。

不要只看 `main/idf_component.yml`。这次 `esp_video` 不是从 main manifest 直接来的，而是 `components/bsp/idf_component.yml` 和 BSP CMake 依赖带进来的。只改 main 不能完整控制求解结果。

不要无证据跑 `idf.py update-dependencies`。它可能把一批无关库一起升级，后面很难判断是功能代码问题还是库版本问题。若确实需要升级，必须记录：

- 升级前版本和 hash。
- 升级后版本和 hash。
- 触发升级的实机证据。
- 回退是否测试过。

本次经验：

- `findlogo.espdl` 在原锁定 `esp-dl 3.3.2` 下能 build，但实机加载模型时在 `fbs::FbsModel::get_operation_parameter(...)` 触发 Load access fault / Guru Meditation。
- 因此保留 `esp-dl 3.3.6` 是有证据的必要兼容更新。
- 但 `merge` 曾连带升级了 `esp_cam_sensor`、`esp_video`、`esp_ipa`、`usb_host_uvc`、`esp_lcd_ek79007`、`esp_lvgl_adapter`、`esp_new_jpeg` 等。后续要优先收窄到“只保留必要升级”，不要把所有新版本都当作模型接入的必要条件。

推荐的依赖处理方式：

- 先保留目标工程原锁定依赖。
- 如果模型加载崩溃，再只升级直接相关库，并保留崩溃日志证据。
- 对 `merge` 当前链路，`esp-dl 3.3.6` 是必要项；相机、视频、屏驱、LVGL adapter 等应单独验证是否需要升级。
- 全局组件缓存里同时存在多个版本是正常现象，不要先清缓存。工程按 lock 和 hash 取版本，真正要查的是 lock 是否被改脏。

## sdkconfig 和芯片 revision

ESP32-P4 rev `<3.0` 和 `>=3.0` 配置互斥。旧板和新板不能混用同一个 revision 配置烧录。

常见组合：

- 旧板：`ESP32P4_REV_MIN_FULL=0`、`REV_MAX_FULL=199`、CPU 常用 360MHz。
- 新板：`ESP32P4_REV_MIN_FULL=301`、`REV_MAX_FULL=399`、CPU 可用 400MHz。

踩坑点：

- `sdkconfig.defaults` 和实际 `sdkconfig` 可能不一致，最终 build/flash 以 `sdkconfig` 为准。
- `idf.py build` 可能 reconfigure，并根据 defaults 或 Kconfig 调整生成内容。改动后要查 `git diff`。
- 之前为了高版本板烧录，`merge` 保留过 rev301/400MHz；如果用户手上换成旧版本芯片，必须显式改回旧板配置。

验证时看启动日志：

- `boot: chip revision: ...`
- `efuse_init: Min chip rev`
- `efuse_init: Max chip rev`
- `cpu_start: cpu freq`

## build 目录和路径污染

不要相信从别的目录复制来的 `build/`。这次 `teammatesproject_old/ESP32P4_Detection/build/project_description.json` 里仍指向微信临时解压目录，而不是当前工程目录。

判断 build 目录是否可信，至少看：

- `build/project_description.json` 的 `build_dir`。
- `config_file`。
- `build_component_paths`。
- `project_version`。
- `idf_path`。

如果路径指向旧临时目录，先 `fullclean` 或删 build 后重新配置。否则 monitor、flasher args 和组件路径都可能误导排查。

## 视觉模型和 ROI

模型接入时先确认五件事：

- 模型文件 hash。
- SPIFFS 文件名长度，当前 `CONFIG_SPIFFS_OBJ_NAME_LEN=32`，模型短文件名更稳。
- ESP-DL 版本。
- 输入/输出 tensor 名称、数量、shape。
- `model->test()` 结果以及是否在 `test()` 后 `minimize()`。

本次 `findlogo.espdl` 关键事实：

- 部署文件：`merge/model/findlogo.espdl`。
- SHA256：`3c23a1ae917adba01020e42f9ae4cfaacc6f6999649fd8231f06488f3ec41477`。
- 类别顺序：`0=jt=极兔`、`1=yd=韵达`、`2=zt=中通`。
- 物理出口语义仍是 `JT->CLASS1`、`ZT->CLASS2`、`YD->CLASS3`。
- 所以 sorter 映射是 `cat0->CLASS1`、`cat1->CLASS3`、`cat2->CLASS2`。

不要把模型类别顺序和物理出口顺序混在一起。UI 公司名、概率接口、分拣出口是三套不同语义，必须逐一映射。

传统 ROI 链路接入时，建议先让第一阶段只做 ROI 框，第二阶段只消费 ROI。不要同时大改视觉 UI、模型后处理和分拣调度。当前成功形态是：

- 第一阶段：传统算法找面单 ROI，画红框。
- 第二阶段：裁剪 ROI 给 `findlogo.espdl`，画 logo 绿框。
- 左上角 LOGO 按钮触发 ROI 校准，旧的抓帧 dump 功能停用但代码可保留。

## RGB/BGR 经验

RGB/BGR 不要靠猜，也不要靠单个注释决定。需要记录整条链路：

- 相机实际输出。
- 传给 ESP-DL 的 `img_t.pix_type`。
- `ImagePreprocessor` 的目标格式。
- 模型训练/导出期望格式。

当前约定：

- 模型导出链路期望 RGB。
- 当前相机帧和 ROI 源按 `DL_IMAGE_PIX_TYPE_BGR888` 声明。
- ESP-DL `ImagePreprocessor` 负责 BGR 到 RGB 的转换。
- 不要再手写额外通道交换，除非后续实测推翻源格式声明。

如果颜色类别疑似反了，优先做可复现实验：固定样张、保存 ROI、分别用 BGR/RGB 声明跑同一模型，比较置信度。不要直接凭肉眼改通道。

## UI 经验

UI 问题不一定是 UI 代码或库问题。先查数据生产链路。

这次 `merge` 的 CPU/状态不刷新，直接原因是硬件调试模式里跳过了 `system_monitor()`：

- UI 仍注册 `EVT_SYSTEM_MONITOR_CHANGED`。
- `system_monitor()` 负责周期采集 CPU/内存并发送事件。
- 硬件调试版本为了让串口干净，把 `system_monitor()` 整个关掉了。
- 结果屏幕 CPU、内存、运行时间等状态都不刷新。

正确做法是把“UI 数据生产”和“串口刷屏”拆开：

- `system_monitor()` 应继续启动，保证 UI 事件还在。
- `sysmon` 的串口 INFO/RTT 输出可以降级、关闭或限频。
- 不要为了 monitor 干净关掉业务数据源。

生成 UI 迁移时还要明确哪些旧 UI 没迁移。当前 `merge` 没有完整搬旧工程手动分拣面板：

- 没有屏幕直接注入 `CLASS1/CLASS2/CLASS3`。
- 没有 `MOTOR_TEST` 面板。
- 没有编码器清零按钮。
- 没有 S1-S4 状态块。
- 没有活动包裹列表。
- 没有 per-belt 速度/超时编辑控件。

后续补 UI 时，优先接已有 C API，例如 `sorting_sim_control_get_settings()`、`sorting_sim_control_apply_settings()`、`sorting_sim_control_get_runtime_debug()`、`sorting_sim_control_get_hardware_status()`，不要让 UI 直接碰 scheduler 内部结构。

## 电机分拣经验

电机分拣链路要分清三层：

- Scheduler core：包裹状态、路由、占用、超时、事件。
- Control bridge：TCP/串口协议、真实/模拟模式、传感器轮询、调试状态、电机输出桥接。
- BSP：MCPWM、GPIO、方向、占空比、输出使能。

迁移时先保证 TCP 模拟通，再接真实传感器。TCP 20 件能验证调度核心和协议，但不能覆盖：

- GPIO 线序。
- active level。
- 防抖。
- 真实推理延迟。
- 编码器输入。
- 电机方向和 PWM 引脚实际接线。

物理出口语义必须保持稳定。旧语义是：

- `CLASS1`：JT 出口。
- `CLASS2`：ZT 出口。
- `CLASS3`：YD 出口。

模型类别变化时只改“视觉类别到 sorter class”的映射，不要改 physical class 的含义。

当前电机默认速度已经统一为 60%。但上位机模拟器启动会发送 `CONFIG`，可能覆盖固件默认值。验证默认速度时要么不让模拟器覆盖，要么在命令里显式加：

```bash
--motor-a-speed 60 --motor-b-speed 60 --motor-c-speed 60
```

## TCP 模拟验收

标准 20 包回归命令：

```bash
cd /home/kazeform/2026esp
python -m esp32_sorter_sim_py --headless --transport tcp --host 192.168.10.1 --port 5000 --count 20 --class-sequence 1,2,3,2,3,1 --feed-interval 0.25 --max-active 8 --timeout 180 --stall-timeout 18 --b-idle-threshold 3 --log-file esp32_sorter_sim_py/logs/<name>.log
python -m esp32_sorter_sim_py.log_audit esp32_sorter_sim_py/logs/<name>.log
```

通过标准：

- `RESULT ok completed=20/20`。
- `audit_status=ok`。
- `warnings=0`。
- `drops=0`。
- `pose_asserts=0`。
- `desyncs=0`。
- `faults=0`。
- 完成分布符合输入序列预期，常用序列为 `class1=7,class2=7,class3=6`。

不要只看模拟器最终返回 ok。必须跑 `log_audit`，并保留日志路径。

## 烧录和 monitor

端口不要假设固定：

- 常见 `/dev/ttyUSB0`。
- 也可能是 `/dev/ttyACM0`。

先查：

```bash
find /dev -maxdepth 1 \( -name 'ttyUSB*' -o -name 'ttyACM*' \)
```

烧录波特率和运行期 monitor 波特率可能不同。本次经验：

- flash 可以用 `-b 921600`。
- 运行期 monitor 可读波特率是 115200。
- `idf.py -p <port> -b 921600 monitor` 可能只看到乱码。

`CHIP_USB_UART_RESET` 后出现 `Core1 Saved PC` 不一定是 panic。要结合是否有 Guru Meditation、LoadProhibited、abort、回溯和重启循环判断。

monitor 需要干净但不能丢关键信号。硬件调试阶段建议保留：

- 创建新包裹。
- 识别到的类别和置信度。
- 开始某类分拣。
- 传感器稳定跳变。
- 电机输出变化。
- 模型加载摘要。

同时压制刷屏噪声，例如 `ISP_AWB: subwindow size ...` 这类 warning，否则会影响硬件调试观察。

## 常见失败和定位顺序

模型加载崩溃：

- 查模型 hash。
- 查 SPIFFS 是否挂载、文件名是否超长。
- 查 ESP-DL 版本。
- 查 `inputs/outputs` 数量和 shape。
- 查 `model->test()` 结果。
- 查 PC/backtrace 是否落在 fbs loader/schema 相关代码。

UI 状态不刷新：

- 先查 UI 事件是否注册。
- 再查事件生产任务是否启动。
- 再查事件数据结构大小是否匹配。
- 最后才怀疑 LVGL 或 UI 库。

old 工程“什么都没动但跑不了”：

- 先确认是 build 失败、flash 失败、启动崩溃、UI 不动还是业务链路不跑。
- 查 git log，不要只查 status。
- 查 `dependencies.lock` 和 managed component hash。
- 查 `sdkconfig` 的 chip revision 和 CPU 频率。
- 查当前烧录板的实际 revision。

依赖疑似不干净：

- 对比 `dependencies.lock`，不要只看 manifest。
- 对比 `managed_components/*/.component_hash`。
- 查 component manager cache 只是辅助，不作为结论。
- 用临时副本验证依赖组合可否 reconfigure/build，再改正式工程。

串口乱码：

- 优先换 monitor 波特率 115200。
- 确认当前端口是否从 USB0 换成 ACM0。
- 不要把乱码误判为固件死机。

真实硬件链路没跑：

- 查默认模式是 Ethernet 模拟还是真实硬件链路。
- 查 motor output 是否 enabled。
- 查 sensor input 是否 enabled。
- 查 S1-S4 raw/active/stable 日志。
- 查是否有 GPIO 和 console UART 冲突。

## 推荐移植顺序

面向类似 `teammatesproject_old` 的后续工程，建议顺序如下：

1. 只做工程状态调查：git、依赖、sdkconfig、build 路径、端口、芯片 revision。
2. 建立空跑 baseline：`idf.py build`，必要时 flash/monitor，记录原始日志。
3. 移植电机分拣核心和 TCP 模拟链路，不接真实传感器。
4. 跑 TCP 20 包和 audit，锁定 scheduler/协议正确。
5. 接真实硬件输入输出，验证 S1-S4、电机方向、PWM 引脚和默认速度。
6. 接视觉输出到 sorter，只验证类别映射，不同时换模型。
7. 接 `findlogo` 或 ROI 级联，记录模型 hash、依赖、RGB/BGR、tensor。
8. 最后补 UI：只接现有 API 和事件，不直接耦合 scheduler 内部。

每一步都要能单独回滚、单独说明成功标准。

