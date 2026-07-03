# Current Task

## Goal

`merge` 视觉链路从“两阶段模型”改为“传统算法 ROI + findlogo 模型”：
- 第一阶段：传统 ROI 算法定位面单区域。
- 第二阶段：裁剪面单 ROI 后送 `findlogo.espdl` 做 logo 三分类。
- 左上角 LOGO 按钮改为 ROI 校准入口。
- 保留当前 ESP32-P4 rev3.1 / 400MHz 配置，不回退 revision。

## Current State

- 修改前备份提交：
  - `1f3fb87 checkpoint before traditional roi cascade`
- 本轮已完成：
  - 新增 `components/vision/detector/roi_algorithm.c/.h`。
  - `roi_tuning.c` 桩替换为旧工程真实 `roi_tuning.cpp` 校准实现。
  - `vision_model_run()` 第一阶段改为 `roi_algorithm_detect()`，成功后返回 `VISION_STAGE_WAYBILL` 红框并裁剪 ROI。
  - `vision_model_init()` 不再加载 `det_pico_224_224_waybill.espdl`，只加载 `findlogo.espdl`。
  - `main/system_init.c` 左上角 LOGO 按钮 handler 从 `vision_frame_dump_request` 改为 `roi_tuning_request_calibration`。
  - `vision_frame_dump_request()` 保留，但不再由左上角按钮触发。
- 本轮验证：
  - `idf.py build` 成功。
  - `/dev/ttyUSB0` 不存在，改用 `/dev/ttyACM0`。
  - `idf.py -p /dev/ttyACM0 -b 921600 flash` 成功，识别芯片 `ESP32-P4 revision v3.1`，app/partition/storage hash verified。
  - `idf.py -p /dev/ttyACM0 monitor` 启动到 `vision started`、SORTDBG、电机/传感器初始化，无 Guru Meditation。

## Immediate Next Step

- 现场用真实包裹确认 ROI 红框、logo 绿框、左上角 LOGO 按钮 ROI 校准日志和 sorter 分类提交。

## Blockers

- 当前 monitor 有大量 `ISP_AWB: subwindow size ...` warning，会影响日志观察；本轮未改日志策略。
