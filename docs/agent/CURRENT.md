# Current Task

## Goal

按用户要求编写上位机 host 技术展示文档，给队友作为作品视频拍摄和比赛评审展示参考。

## Current State

- 活跃上位机工程：`/home/kazeform/2026esp/esp32_host_no_inference`
- 目标文档：`/home/kazeform/2026esp/docs/host_technical_video_reference.md`
- 文档口径：
  - 中文 Markdown。
  - 面向专业评审。
  - 突出工程技术，不只写拍摄脚本。
  - 准确说明 host 无本地推理，板端负责视觉推理和分拣闭环，host 负责通信、监控、归档和控制入口。

## Implementation Notes

- 已检查 host 工程：
  - Qt 6 Quick + Network + C++17 + CMake。
  - `HostNetworkWorker` 独立线程监听 `5000/tcp` 控制链路和 `5001/tcp` 图像链路。
  - `packetprotocol` 使用 40 字节自定义二进制 header。
  - `HostController` 负责图像保存、遥测、包裹记录、控制节流和 QML 属性适配。
- 新增文档内容包括：
  - 系统架构和软件分层。
  - 双 TCP 链路、自定义 packet、QThread 网络线程。
  - JPEG/RGB 图像接收、增强、原子保存和历史归档。
  - CPU/PSRAM/heap/FPS/链路健康等遥测指标。
  - 电机速度 `CONFIG a_speed/b_speed/c_speed` 下发和 100 ms 节流。
  - 视频拍摄清单、60 秒/2 分钟答辩稿、评委追问和术语表。

## Verification

- 文档事实对照代码和 README 编写。
- 本次只新增文档和 agent 记录，不修改 ESP 固件或 host 运行代码。
- 因无代码改动，不运行 `idf.py build` / `idf.py flash monitor`；需在最终记录中明确原因。

## Next Step

- 检查 Markdown 内容和 git diff。
- 提交根目录文档改动。

## Blockers

- 无当前阻塞。
