# vision_det 内部栈恢复与实机验证

## 目标

- 解决识别任务 `vision_det` 的栈溢出。
- 检查扩大栈后是否与 `sort_real_io` 内部 RAM 栈冲突。
- 不改变识别、投票、电机和真实 IO 业务逻辑。

## 根因与决策

- 队友包 12 的提交 `e43458e` 把 `VISION_DET_STACK_SIZE` 从历史稳定值 12 KiB 降到 3728 字节。
- 新识别日志/UI 路径增加了检测任务调用深度，现场随后出现 `vision_det` 栈溢出。
- 历史 61 分钟稳定版本使用 12 KiB 内部 RAM，检测任务高水位尚余 8524 字节，因此恢复为该已验证配置。
- `vision_det` 保持内部 RAM；不使用 PSRAM 回退，避免历史上 ESP-DL/中断并发下出现过的双核死锁风险。
- `sort_real_io` 保持 4096 字节、内部 RAM 优先且失败才回退 PSRAM。两个任务分别固定到 core 1 / core 0，不存在核绑定冲突；实机任务创建成功证明当前内部 RAM 容量足够。

## 改动

- 文件：`ESP32P4_Detection/components/vision/framework/vision_app.c`
- 唯一功能改动：`VISION_DET_STACK_SIZE` 从 3728 改为 `(12 * 1024)`，并更新就地说明。
- 主分支提交：`a709e33 fix: restore vision detection task stack margin`
- 修改前备份分支：`backup/before-vision-det-stack-fix-20260718`
- 动态限速 worktree 在新基线重放后提交：`1092a30 feat: cap belt A speed while belt B is busy`
- `sort_dbg` 仍未创建；真实 IO、电机参数、投票逻辑均未修改。

## 验证

### 构建

- 主 worktree `idf.py build`：通过，app 大小 `0x527840`，分区余量约 14%。
- 动态限速 worktree `idf.py build`：通过，app 大小 `0x5278c0`，分区余量约 14%。

### 烧录

- 串口：`/dev/serial/by-id/usb-Silicon_Labs_CP2102N_USB_to_UART_Bridge_Controller_7ee2f3966ac3ee11be78b90f9e1b1c54-if00-port0`
- 以 460800 波特率完整烧录主分支；bootloader、partition table、storage、app 均完成 Hash verified。

### 实机监视

- 115200 波特率连续监视至约 121 秒。
- 启动确认 `sort_real_io` 的 4096 字节栈实际从内部 RAM 分配，`vision_det` 创建并进入识别。
- 识别链路连续输出 `SORT 视觉识别` 和 `SORT 视觉投票`；包裹 #22 完成 5/5 投票并以 `vote_full` 定案。
- 随后在大量 S1 上升沿与持续识别并发下继续运行，没有 stack overflow、Guru Meditation、panic 或重启。
- 后段 `queue_full` 和 `vision_without_package` 是只反复触发 S1、没有按正常包裹时序用 S2 出队造成的业务告警，不是栈故障。

## 尚未覆盖

- 本轮没有按正常机械节拍完成 S1→S2→电机出口的真实包裹闭环；上硬件正式验收仍需补测完整分拣动作和两条速度策略。
