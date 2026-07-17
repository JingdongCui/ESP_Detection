# 2026-07-18 real IO 内部 RAM 优先栈扩容

## 问题与判断

- 用户反馈 S1 上升沿后系统崩溃，怀疑 `sort_real_io` 栈不足。
- 当前代码的 real IO 栈只有 2640 字节；S1 上升沿会进入建包、调度、日志和电机命令路径，比空闲轮询调用更深。
- 本地历史已有 4096 字节 PSRAM 栈的成功记录，说明 2640 字节回退具有明确风险。

## 修改

- 提交：`4fa6f3e fix: prefer internal RAM for larger real IO stack`。
- `sort_real_io` 栈提高到 4096 字节，优先使用 `MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT`。
- 仅当内部 RAM 创建失败时，回退到 `MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT`。
- 日志明确输出实际栈位置和两类内存剩余量。
- 任务优先级 4、core0 绑定、轮询周期和分拣逻辑均未修改。

## 验证

- `idf.py build` 通过，app `0x527840`，6 MB app 分区剩余 14%。
- 460800 波特率完整烧录，bootloader/app/partition/storage 全部 Hash verified。
- 115200 monitor 确认：`real IO task stack allocated in internal RAM (4096 bytes)`，未回退 PSRAM。
- S1=GPIO22、S2=GPIO23 初始化 valid=1，真实硬件链路开启。
- 约 27 秒内多次触发 S1 上升/下降沿，包裹号推进到 #15，期间经过 A/B/C 超时调度、电机正反转和队列满分支，无 stack overflow、panic 或重启。
- 因未按实际物流间距配合 S2，队列达到 8 个后出现 `queue_full`；这是压力触发的业务容量保护，不是栈崩溃。
- 动态限速分支已 rebase 到该公共修复，HEAD `d322c74`，`idf.py build` 通过；本次实机烧录的仍是主分支 `4fa6f3e`。
