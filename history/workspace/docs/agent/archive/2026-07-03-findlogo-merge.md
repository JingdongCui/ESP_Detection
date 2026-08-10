# 2026-07-03 Findlogo Merge Integration

## Goal

将 `merge` 接入 `findlogo.espdl` 作为第二阶段 logo 检测模型：先整帧检测面单，再裁剪面单 ROI 检测三分类 logo，并保持现有物理分拣出口语义。

## Checkpoint

- 按 Git 规则在修改前创建 checkpoint commit：
  - `ba2ee89 checkpoint before findlogo merge`

## Model

- 使用 `merge/model/findlogo.espdl`，保持短文件名，走现有 `storage` SPIFFS 分区打包。
- SHA256: `3c23a1ae917adba01020e42f9ae4cfaacc6f6999649fd8231f06488f3ec41477`
- 与 `/home/kazeform/2026upper/esp-detection/model/best/datasets5000_kl_MOSIC_NOINT16.espdl` 哈希一致。
- 不使用 `outputs/best/model.espdl`，因为它是另一份不同哈希模型。

## Category And Sorter Mapping

- 训练类别来自 `/home/kazeform/2026upper/esp-detection/cfg/logo.yaml`：
  - `0=jt/极兔`
  - `1=yd/韵达`
  - `2=zt/中通`
- UI 公司名：`{极兔, 韵达, 中通}`。
- `vision_model_get_class_probs(int *jt, int *zt, int *yd)` 参数顺序不变，内部归属为 `cat0->jt, cat1->yd, cat2->zt`。
- 物理分拣出口保留旧语义：
  - `JT->CLASS1`
  - `ZT->CLASS2`
  - `YD->CLASS3`
- 因训练顺序为 `jt,yd,zt`，代码提交 sorter 映射为：
  - `cat0->CLASS1`
  - `cat1->CLASS3`
  - `cat2->CLASS2`

## Code Changes

- `components/vision/detector/vision_model.cpp`
  - 保持两阶段检测：整帧 waybill，面单 ROI 内 findlogo。
  - findlogo 阈值从 `0.90f` 调整为 `score_thr=0.25f`，`nms_thr=0.70f`。
  - 构造时记录每个模型输入/输出 tensor 名、dtype、维度、shape。
  - 保持 `model->test()` 后再 `minimize()`。
  - 概率内部映射修正为 `cat0->jt, cat1->yd, cat2->zt`。
- `components/vision/detector/vision_model.h`
  - `VISION_STAGE_LOGO` 注释更新为 `0=极兔 1=韵达 2=中通`。
- `components/vision/framework/vision_detect.c`
  - UI 类别名改为 `{极兔, 韵达, 中通}`。
  - sorter 映射修正为 `cat0->CLASS1, cat1->CLASS3, cat2->CLASS2`。
- `CMakeLists.txt`
  - 注释改为明确打包两级检测模型到 `storage` SPIFFS。

## RGB/BGR Convention

- 模型导出链路期望 RGB。
- 当前相机帧和 ROI 源按 `DL_IMAGE_PIX_TYPE_BGR888` 声明。
- ESP-DL `ImagePreprocessor` 负责 BGR 到 RGB 的转换；未手写通道交换。

## Dependency Result

- 先按计划恢复到原锁定依赖状态并测试 `esp-dl 3.3.2`。
- `idf.py build` 在 `esp-dl 3.3.2` 下成功。
- 实机启动时 waybill 模型能加载，但加载 `findlogo.espdl` 时在 `fbs::FbsModel::get_operation_parameter(...)` 触发 Load access fault / Guru Meditation。
- 因存在实机失败证据，最终保留 checkpoint 中的 `esp-dl 3.3.6`，未保留 `sdkconfig` 大 diff。

## Verification

- 最终 `idf.py build` 成功。
- `idf.py -p /dev/ttyUSB0 -b 921600 flash` 成功，app 与 `storage` SPIFFS 分区均写入并 hash verified。
- `idf.py -p /dev/ttyUSB0 monitor` 使用 115200 可读；`-b 921600 monitor` 输出乱码，说明运行期 UART 仍为 115200。
- 启动日志确认 SPIFFS mounted。
- waybill：`model=/spiffs/det_pico_224_224_waybill.espdl inputs=1 outputs=6`，输入 `1x224x224x3`，输出 box/score 三尺度。
- findlogo：`model=/spiffs/findlogo.espdl inputs=1 outputs=6`，输入 `1x224x224x3`，score 输出为 `1x28x28x3`、`1x14x14x3`、`1x7x7x3`。
- findlogo `model->test()` 报告缺少导出 test values，但模型继续 `minimize()` 并加载成功。
- 启动后无 LoadProhibited / Guru Meditation。
- Ethernet 连接 `192.168.10.1:5000` 失败 `errno=119`，原因是本次未运行上位机模拟器，不影响模型加载验收。
- 后续已补跑 TCP 20 包分拣回归：
  - 日志：`esp32_sorter_sim_py/logs/merge_findlogo_tcp_20_20260703.log`
  - `RESULT ok completed=20/20`
  - `audit_status=ok`
  - `warnings=0,drops=0,pose_asserts=0,desyncs=0,faults=0`
  - 完成分布：`class1=7,class2=7,class3=6`

## Remaining Field Checks

- 真实画面确认 waybill 红框、logo 绿框。
- 确认 `cat0/cat1/cat2` 分别显示极兔/韵达/中通。
- 真实传感模式确认 `JT->CLASS1`、`ZT->CLASS2`、`YD->CLASS3`。
