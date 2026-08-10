# 人工分类与 A 带自适应停止

## 目标与决策

- 识别失败不再自动轮转分配类别，停止 A 主带后由 UI 弹窗人工确认。
- 模型类别映射保持：0 极兔到 CLASS1、1 韵达到 CLASS3、2 中通到 CLASS2；取消按极兔处理。
- A 带速度不超过 60% 直接停止；超过 60% 时按每秒 80 个百分点线性降到 50% 后停止。
- B/C 带和原有后续分拣流程不改。

## 实现

- 调度器增加人工分类等待事件、确认接口和 A 带减速状态机。
- 减速期间冻结对应包裹超时和 S2 放行，停止完成后再进入人工确认或原有 B 带交接。
- UI 顶层增加不可点外部关闭的人工分类弹窗，提供极兔、韵达、中通和取消/极兔按钮。
- 停止阈值、目标速度、减速率提供默认宏，并支持 `CONFIG a_stop_threshold/a_stop_target/a_decel_rate` 调整。
- 代码提交：`f82aaa2 feat: add manual classification and adaptive belt stop`。
- 修改前备份基线：`470a704 fix: sync sorter timing and speed defaults`。

## 验证

- `idf.py build` 成功；app 大小 `0x513790`，6 MB app 分区剩余 `0xec870`（15%）。
- 普通命令 `idf.py flash` 第一次成功，bootloader/app/partition/storage 全部 Hash 校验通过。
- 串口确认启动固件版本 `f82aaa2`，初始化完成且持续运行约 30 秒无崩溃。
- 既有 waybill 模型自测仍报告 box1 数值差异；logo 自测通过。
- Ethernet 主机 `192.168.10.1` 未提供服务，5000/5001 连接持续失败。
- `sort_real_io` 任务创建失败，S1-S4 真实轮询未启动，因此人工弹窗与实际包裹的完整联动尚未实机验证；需另行确认是否将该任务栈迁入 PSRAM。
