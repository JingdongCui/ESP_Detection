# History

- 2026-07-16：用户暂缓 500ms 推理问题，要求先查看最新人工分类弹窗。
- 修改前已有备份标签 `backup/inference-priority-20260716-4b55e01`；未提交的 ESP-DL 优先级实验已完整撤销。
- 弹窗样式微调：删除覆盖面板左边缘的 `accent` 色条；面板边框从 1px 调整为四周统一 2px；面板圆角从 24px 调整为统一 18px。交互、三类按钮和模型类别映射未修改。
- 验证：`idf.py build` 成功；`idf.py flash` 成功且所有分区哈希校验通过；`idf.py monitor` 启动到 `System initialization done`，无新增启动错误。既有 UVC `ESP_ERR_NO_MEM` 仍按要求忽略。
- 2026-07-16：按用户要求撤销 A 带分阶段减速停车，恢复直接 STOP；修改前创建标签 `backup/before-b-belt-speed-control-20260716-5c5358a`。
- 新增 B 带占用联动：`b_owner == 0` 时 A 使用 `motor_a_speed_percent`；B 已预留或占用时 A 使用 `min(motor_a_speed_percent, 60)`。B 释放后由正常调度恢复配置速度。
- 删除不再使用的 `a_stop_threshold`、`a_stop_target`、`a_decel_rate` 配置、运行态字段及 TCP/串口 `CONFIG` 解析，避免旧减速状态继续影响后续事件时序。
- 主机侧调度器测试通过：A=100/80 在 B 忙时降为 60，A=50 不提速；B 释放后分别恢复 100/80/50；S2 路径先直接 STOP；识别失败直接 STOP 且人工确认后继续分拣。
- 验证：`git diff --check`、`idf.py build`、plain `idf.py flash` 均成功；ESP32-P4 v1.0 上所有镜像 Hash 校验通过，monitor 启动正常后退出。未自动触发现场传感器，真实机械节拍留待现场确认。
