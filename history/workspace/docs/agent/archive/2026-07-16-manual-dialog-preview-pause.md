# 人工分类弹窗与摄像头预览暂停

## 目标

- 人工分类弹窗出现时停止摄像头画面覆盖弹窗。
- 将弹窗改为产品化、不可跳过的三选一交互。
- 核验人工选择后继续分拣，以及 S1 是否以上升沿创建包裹。

## 实现

- 在 Vision 增加 `vision_set_preview_paused()` 原子门控。
- 显示任务在 PPA 缩放前和持 LVGL 锁准备直刷 framebuffer 前各检查一次门控，消除并发中的最后一帧覆盖。
- 暂停只影响 LCD 预览直刷，不停止摄像头采集和推理。
- UI 通过注入的 `manual_class_dialog_state` handler 在弹窗显示前暂停预览，隐藏后恢复预览，保持 UI 与 Vision 解耦。
- 弹窗改为全屏不透明模态层、居中卡片、状态标识和三张分类按钮；删除会错误映射到极兔的 `Cancel / 极兔` 按钮。
- 新文案逐字核对裁剪后的 MiSans 16/24 字库，最终文案不存在缺失字形。

## 类别与状态机核验

- UI/模型类别：`0=极兔、1=韵达、2=中通`。
- 控制层映射：`0→CLASS1`、`1→CLASS3`、`2→CLASS2`，按公司名对齐而不是枚举顺序相加。
- `sorter_scheduler_confirm_manual_class()` 成功后清除 `manual_class_pending`；若包裹停在 S2，则重新占用 B 带并恢复 A→B，S2 释放后继续 B/C 分拣。
- `update_vision_s1_locked()` 只在 `active && !s_vision_s1_active` 时调用 `open_vision_window_locked()`，因此真实 S1 经去抖后的上升沿创建包裹，持续高电平不会重复创建。

## 验证

- Git 提交：`32dc471 feat: polish manual classification dialog`。
- `idf.py build` 通过；app 大小 `0x513eb0`，factory 分区剩余 `0xec150`（15%）。
- plain `idf.py flash` 在 `/dev/ttyUSB0` 成功；bootloader、partition、app、storage 均 Hash verified。
- `idf.py monitor` 确认运行固件版本 `32dc471`，两个模型测试通过，Vision/Ethernet/Sorter 启动，最终到达 `System initialization done`，未见 panic/reboot。
- UVC 仍因末尾初始化时 JPEG DMA 内存不足返回 `ESP_ERR_NO_MEM`；用户要求本阶段先不处理。
- 串口监控会话发送调试命令未被 SORTDBG 接收，因此未自动注入 S1/S2；触屏三选一、画面遮挡和真实传送带恢复仍需现场放置未识别包裹完成最终验收。

