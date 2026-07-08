# Current Task

## Goal

补充模型量化策略细节到比赛作品报告和资料源。

## Current State

- 根目录 `.codegraph/` 不存在，跳过 CodeGraph。
- 本轮修改文件：
  - `docs/competition_report_draft.md`
  - `docs/report_system_information.md`
  - `docs/agent/CURRENT.md`
  - `docs/agent/HISTORY.md`

## Important Notes

- 用户补充量化策略：
  - MSE equalization，参数 `(10, 0.1)`。
  - 无 TQT。
  - 无 bias correction。
- 已抽取四张上位机界面截图：
  - `host_page_1_dashboard.png`：`host_p.mp4` 第 14 秒，性能总览。
  - `host_page_2_detection.png`：`HOST_SNAP.mp4` 第 36 秒，视觉检测。
  - `host_page_3_control.png`：`host_p.mp4` 第 30 秒，设备控制。
  - `host_page_4_system.png`：`host_p.mp4` 第 37 秒，系统维护。
- 已复制新增硬件图：
  - `hardware_power_driver_board.jpg`：电池、电源板、电机驱动板实物接线图。
- 后续发现并已提交三张新实物照片：
  - 根目录提交：`109ac8b docs: add system hardware photos`
  - `esp32p4_board_rear_mount.jpg`：ESP32-P4 板端背面安装。
  - `conveyor_layout_photo.jpg`：三段传送带位置布局。
  - `camera_mount_photo.jpg`：摄像头安装位置。
- 随后又补充三张板端 LCD 识别实测图：
  - 根目录提交：`963ab7b docs: add board detection result photos`
  - `board_detection_jt.jpg`：极兔识别实测。
  - `board_detection_yd.jpg`：韵达识别实测。
  - `board_detection_zt.jpg`：中通识别实测。
- 报告补充内容：
  - 嵌入四张上位机截图和硬件接线图。
  - 嵌入传送带布局、摄像头安装和板端安装实物图。
  - 嵌入板端 LCD 三类识别实测图。
  - 增加模型训练与轻量化部署小节。
  - 写入训练集约 5000 张、调整数据增强、量化策略为 MSE equalization `(10, 0.1)`，无 TQT，无 bias correction。
  - 同步更新 `hardware.md`、`docs/report_system_information.md`、`docs/report_unknown_information.md`。

## Verification

- 本次只修改 Markdown 文档，未修改 ESP 固件或上位机代码。
- 未执行 `idf.py build` / `idf.py flash monitor`，原因是无代码改动。
- `git diff --check` 通过。
- 报告初稿中未检出学校/指导老师相关字样。
- 十一个新增报告资产文件均存在且非空。

## Next Step

- 提交本次量化策略补充。
- 后续仍需补系统整体实物照片、板端 UI 照片和正式系统框图。

## Blockers

- 当前无板子，无法进行新的实机验证。
- 更正式的系统整体正面/斜 45 度照片仍可后续补充。
