# 更新日志 (CHANGELOG)

所有重要的项目变更都会记录在此文件中。

格式参考 [Keep a Changelog](https://keepachangelog.com/zh-CN/1.0.0/)。

---

## [v0.1.0] - 2025-05-14

### 初始版本

- 项目初始化，基于 ESP32-P4-Function-EV-Board
- 集成 ESP-DL 推理框架（PicoDet 416×416 Logo 检测模型）
- 集成 LVGL v9 UI 显示
- 集成 MIPI-CSI 摄像头采集
- 添加 SEGGER RTT 实时日志
- 添加系统监控模块（system_monitor）
- 配置自定义分区表（6M factory + 5M spiffs）
- 集成 agentic JTAG 调试工具链
