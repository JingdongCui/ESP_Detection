# 分拣参数同步与旧新工程差异审计

## 已修改

以归档旧工程 `reference/firmware/ESP32P4_Detection_before_9_20260716` 为准，将第 9 包活跃工程的分拣默认值同步为：

- A/B/C 电机速度：`60% / 100% / 100%`
- 交接延时：`100 ms`
- A/B/C 皮带超时：`4500 / 900 / 1300 ms`
- lost timeout 保持：`3000 / 6000 ms`

修改提交：`470a704 fix: sync sorter timing and speed defaults`。

## 验证

- 新旧 `sorter_debug_config.h` 已逐字一致。
- `idf.py build` 成功；app `0x512890`，6 MB app 分区剩余约 15%。
- 直接执行 `idf.py flash` 成功。
- 目标端口 `/dev/ttyUSB0`，ESP32-P4 revision v1.0。
- bootloader、app、partition table、storage 均写入成功并通过 Hash 校验，随后 RTS hard reset。

## 仅审计、未修改的差异

- 旧工程对视觉分类做最多 5 帧置信度加权投票；新工程首个有效类别立即定案。
- 旧工程将 `sort_real_io` 任务栈分配在 PSRAM，并输出内存不足诊断；新工程使用普通内部 RAM 任务栈。
- 旧工程支持 Ethernet early-init，并在启动分拣前等待 ready 最长 5 秒；新工程没有 early-init/wait-ready 接口。
- 旧工程截图重触发阈值为连续 miss 3 帧并使用独立 armed 状态；新工程为 miss 5 帧及检测上升沿触发。
- 旧工程目标 revision v3.1-v3.99、CPU 400 MHz；新工程目标 revision v0.0-v1.99、CPU 360 MHz。现场芯片是 v1.0，因此不能直接同步旧配置。
- 新工程增加 UI 日志 4 页翻页逻辑，旧工程没有。
- 旧工程停用了 encoder 和最终 sorter 硬件启动；新工程会初始化 encoder 并启动 sorter 电机/传感器控制。
- 视觉失败分类函数写法不同，但实际均按 `1→2→3` 循环，行为一致。
- 其余差异为本轮已经确认的初始化职责拆分和 `.gitignore` 本地工具忽略规则，不影响业务逻辑。
