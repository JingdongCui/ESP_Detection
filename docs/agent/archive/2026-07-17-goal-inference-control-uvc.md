# Goal 推理、控制、长稳与 UVC 阶段归档（2026-07-17）

## 目标与交付

完成 `goal.md` 中可在当前环境执行的软件工作：ESP32-P4 推理时延恢复、Host/板端 CONTROL_JSON 控制闭环、双 TCP 重连、ISP 配置、真实 IO 保留、任务/堆快照、长稳测试、UVC 调查，以及多版本 Git 标签与 bundle 备份。

推荐固件为 `c26dba8`（`backup/final-production-candidate-20260717`），Host 为 `6bcee3b`。生产 profile 默认 `CONFIG_SCREEN_UVC_ENABLE=n`。

## 关键决策

- 500～600 ms 系统性回退由 `sort_dbg` USB Serial/JTAG monitor 引起；关闭它不影响 `sort_real_io`、传感器或电机。
- UI 尾延迟来自 LVGL mutex 优先级继承。最终使用 lvgl/swdraw/vision_disp=3/3/2，fetch/detect/dl_mc0/1=4。
- ESP-DL worker guard 校验实际虚调用 target，作为已知 0x10 Instruction access fault 的防线；不宣称上游内存损坏根因已消除。
- UVC DMA descriptor 碎片通过早期连续块预留解决，但 USB Device controller 与 JPEG engine 都会独立显著拖慢 ESP-DL；生产继续关闭 UVC。
- 不把 RTS reset 称为物理断电，不把软件截图称为 LCD/真实包裹实拍，不在现场验收前创建 stable 标签。

## 核心测试结果

- 严格 5×60：P50/P95/max=67.312/71.393/132.003 ms，零 >=150/500 ms。
- UI 隔离 61 分钟：3660.179 s，boot/fatal/guard/连接失败=1/0/0/0；1048 样本 67.308/74.273/217.760 ms，8 个 >=150、零 >=500；连续性通过，长稳 max 门槛未通过。
- 最终全量重刷后 1×60：67.848/71.494/72.046 ms，四项严格门槛通过。
- CONTROL_JSON 正向/负向/恢复/restart、5000/5001 重连实板通过；Host build、CTest 和四页 UI 可见检查通过。
- 最终 `idf.py build`、`idf.py flash monitor` 通过，boot/app/partition/storage 全部 Hash verified。

## UVC 根因证据

- 生产 core timing 20 分钟：353 个抽样 infer，max 72.506 ms、零 >=150 ms。
- HS UVC 5 分钟：203 个 slow dispatch，worker wake 平均 50.6/22.2 us，但 exec 平均 85.146/80.036 ms；不是普通 worker 唤醒延迟。
- 原 FS 对照因组件未显式设置 Full-Speed PHY 而 stack init 失败；`838b0ed` 修复后 UVC Device Start 成功。
- 即使 JPEG engine 延迟到主机 start stream、空闲时从未创建，FS/HS USB Device controller 仍使推理约 463～476 ms。因此 UVC profile 只保留作实验版本。

## 候选与回退标签

- `backup/final-production-candidate-20260717` → `c26dba8`
- `backup/ui-inference-isolation-61min-pass-candidate-20260717` → `60c9f8a`
- `backup/uvc-starts-latency-regression-20260717` → `b0e7a02`
- `backup/uvc-fs-starts-latency-regression-20260717` → `838b0ed`
- `backup/dl-target-guard-candidate-20260717` → `b08a1a3`
- `backup/dl-guard-61min-pass-candidate-20260717` → `56a53fd`
- `backup/control-json-isp-reconnect-candidate-20260717` → `b1dfef5`
- `backup/inference-70ms-candidate-20260717` → `6ad4fd5`

完整矩阵、限制和证据路径见 `docs/goal-delivery-2026-07-17.md`。

## 未完成现场项目

- 五次物理断电冷启动。
- LCD 触摸、亮度、ISP 实际画面，以及真实包裹/传感器/三路电机照片录像。
- USB OTG 主机枚举、MJPEG 拉流、画质/帧率与视频长稳。
- 若验收要求覆盖 61 分钟所有样本 max<=150 ms，继续收敛 8 个 wait 尖峰。
