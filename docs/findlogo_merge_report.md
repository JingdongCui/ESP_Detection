# findlogo 合并报告

日期：2026-07-03

## 合并内容

本次在 `merge` 固件中接入 `findlogo.espdl`，作为视觉第二阶段模型。

运行链路：

1. 第一阶段用原有 waybill 模型在整帧上检测面单。
2. 根据面单框裁剪 ROI。
3. 第二阶段在 ROI 上运行 `findlogo.espdl` 检测快递 logo。
4. logo 结果用于 UI 公司名显示、概率输出和分拣类别提交。

没有新增 public API。

## 主要文件

- `CMakeLists.txt`
  - 继续把 `model/` 打包到 `storage` SPIFFS 分区。
  - 保持模型文件短文件名，避免超过 `CONFIG_SPIFFS_OBJ_NAME_LEN=32`。
- `components/vision/detector/vision_model.cpp`
  - 同时加载 `det_pico_224_224_waybill.espdl` 和 `findlogo.espdl`。
  - 记录两个模型的输入/输出 tensor 名称、dtype、维度和 shape。
  - 保留 `model->test()`，再执行 `minimize()`。
  - 在面单 ROI 上跑 findlogo。
  - findlogo 阈值使用 `score_thr=0.25f`、`nms_thr=0.70f`。
  - 修正 logo 概率类别归属。
- `components/vision/detector/vision_model.h`
  - 更新 `VISION_STAGE_LOGO` 的类别说明。
- `components/vision/framework/vision_detect.c`
  - 更新 UI 公司名。
  - 修正分拣物理出口映射。
- `model/findlogo.espdl`
  - 新增部署模型。

## 模型来源

部署文件：

```text
merge/model/findlogo.espdl
```

SHA256：

```text
3c23a1ae917adba01020e42f9ae4cfaacc6f6999649fd8231f06488f3ec41477
```

该哈希与下面文件一致：

```text
/home/kazeform/2026upper/esp-detection/model/best/datasets5000_kl_MOSIC_NOINT16.espdl
```

没有改成 `outputs/best/model.espdl`，因为那是另一份不同哈希模型。

## 库依赖

原锁定依赖中 `esp-dl` 是 `3.3.2`。回退到 `3.3.2` 后：

- `idf.py build` 能过。
- 实机加载 `findlogo.espdl` 失败。
- 崩溃点在 `fbs::FbsModel::get_operation_parameter(...)`，表现为 Load access fault / Guru Meditation。

因此当前保留：

```text
espressif/esp-dl = 3.3.6
```

这是基于实机失败证据的兼容性升级，不是无关依赖刷新。

## RGB/BGR 设定

模型导出链路期望 RGB 输入。

当前相机帧和 ROI 源缓冲声明为：

```cpp
DL_IMAGE_PIX_TYPE_BGR888
```

ESP-DL 的 `ImagePreprocessor` 会按声明完成 BGR 到 RGB 的转换。代码里没有额外手写通道交换。

后续除非实测证明相机源格式声明错误，否则不要再加一次手动 BGR/RGB swap。

## 类别和分拣映射

训练配置类别顺序：

```text
0 = jt = 极兔
1 = yd = 韵达
2 = zt = 中通
```

UI 显示：

```text
cat0 -> 极兔
cat1 -> 韵达
cat2 -> 中通
```

概率 API 参数顺序不变：

```cpp
vision_model_get_class_probs(int *jt, int *zt, int *yd)
```

内部概率归属：

```text
cat0 -> jt
cat1 -> yd
cat2 -> zt
```

物理分拣出口保留旧机器语义：

```text
JT -> CLASS1
ZT -> CLASS2
YD -> CLASS3
```

因为模型训练顺序是 `jt, yd, zt`，所以 sorter 提交映射为：

```text
cat0 -> CLASS1
cat1 -> CLASS3
cat2 -> CLASS2
```

## UI 相关说明

当前生成 UI 保留了视觉系统原有结构：

- 实时检测页：预览区域、检测状态、置信度、推理耗时、快递公司显示、JT/ZT/YD 概率/计数。
- 日志页：识别和推理日志。
- 设置页：网络、亮度、检测阈值、检测开关、预览叠加框、原有通用控制项。
- 系统页：内存、CPU、堆、运行时间、芯片温度、任务数、以太网状态。

没有完整迁移旧工程的分拣手动控制 UI：

- 没有屏幕上的 `CLASS1/CLASS2/CLASS3` 手动注入按钮。
- 没有屏幕上的 `MOTOR_TEST`。
- 没有屏幕上的编码器清零。
- 没有屏幕上的 S1-S4 传感器状态块。
- 没有屏幕上的活动包裹列表。
- 没有屏幕上的 A/B/C 速度和超时编辑控件。

分拣实时调参目前通过 TCP 或 USB 串口协议完成，详见 `motor_algorithm_review.md`。

## 验证记录

构建：

```bash
idf.py build
```

结果：通过。

烧录：

```bash
idf.py -p /dev/ttyUSB0 -b 921600 flash
```

结果：app 和 `storage` SPIFFS 写入并 hash verified。

monitor：

```bash
idf.py -p /dev/ttyUSB0 monitor
```

结果：

- SPIFFS 挂载成功。
- waybill 模型加载成功：`inputs=1 outputs=6`。
- findlogo 模型加载成功：`inputs=1 outputs=6`。
- findlogo score 输出为三尺度、每尺度 3 类：`1x28x28x3`、`1x14x14x3`、`1x7x7x3`。
- Ethernet 获取 IP：`192.168.10.2`。
- SORTDBG 和 motor BSP 初始化成功。
- 无 LoadProhibited / Guru Meditation。

## TCP 20 包回归

最新 60% 速度验证日志：

```text
reports/merge_motor_speed60_tcp_20_20260703.log
reports/merge_motor_speed60_tcp_20_20260703.audit.txt
```

审计结果：

```text
audit_status=ok
result=ok completed=20/20 active=0 created=20
warnings=0,drops=0,pose_asserts=0,desyncs=0,faults=0
完成分布：class1=7,class2=7,class3=6
```
