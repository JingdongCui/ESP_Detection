# 2026-07-18 队友新包合并与烧录

## 输入

- 板端：`ESP32P4_Detection(13).zip`，SHA256 `f879bca008d3a6e733ecb4a12e1b6d7af2015c8ca190d0363f7b10b1e0814e55`。
- Host：`esp32_host_no_inference_control_20260717(1)(1).zip`，SHA256 `d1f335f59bd3cbd1e36e6a53e21b144d3272b0d3315c839c0d4079e0df7076af`。
- 两包均通过 ZIP 完整性检查。

## 合并决策

- Host 按用户要求用队友包源码整体覆盖，保留本地 `.git`，不导入 `build` 和工具缓存。
- 板端接收：新 UI、电机开关/速度的安全 handler、Ethernet 本地状态回传、ISP/分模型耗时指标、识别日志。
- 板端拒绝：`sort_dbg`/USB Serial-JTAG 调试任务、取消投票的单帧分类、A=100/交接=1000 ms/B-C timeout=2000 ms、默认关闭电机输出。
- 保留真实 IO 链路与参数：S1=22、S2=23、S3/S4=-1，A/B/C=65/100/100，交接=100 ms，A/B/C timeout=4500/900/1300 ms，5 次置信度加权投票。

## Git

- 板端主分支 `feat/screen-uvc-stream`：`24fa4a1 feat: merge teammate UI and telemetry updates`。
- 动态限速工作树 `feat/sorter-b-busy-speed-cap`：已 rebase，HEAD `e814b48`。
- Host `master`：`e9f8ae2 feat: integrate teammate host control update`。
- 回退点：板端 `backup/before-package13-merge-20260718`，Host `backup/before-teammate-overwrite-20260718`。

## 验证

- Host Release 全量编译通过，`packetprotocol_tests` 1/1 通过，offscreen 启动 8 秒无崩溃。
- 主分支 `idf.py build` 通过，app 大小 `0x527630`，剩余 14%。
- 动态限速分支 `idf.py build` 通过，app 大小 `0x5276b0`，剩余 14%。
- 主分支通过 CP2102N `/dev/ttyUSB0` 对应的稳定 by-id 路径烧录，bootloader/app/partition/storage 均 Hash verified。
- monitor 确认：ESP32-P4 v1.0，S1=22、S2=23、S3/S4 disabled，电机输出开启，M1=65%，真实硬件链路开启，视觉模型加载，最终进入 `System initialization done`，后续观察期间无 panic/重启。
- 模型自检仍打印 waybill/logo ground-truth mismatch，但随后固定图测试和实际模型加载继续完成；本次未修改该既有模型行为。

## 未完成的实物验证

- monitor 期间没有物理触发 S1/S2，因此还需要现场用包裹验证 S1 投票、S2 交接、实际电机时序和机械出口。
