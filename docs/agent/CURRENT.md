# Current Task

## Goal

检查并收敛 `new_merge` TCP 通信刷屏风险，默认关闭板端以太网模拟分拣链路。

## Current State

- `new_merge` 已同步分支：
  - `motor-two-stage`: `32dcd9f filter tcp sim packets and disable link by default`
  - `motor-roi`: `c7ccbc0 filter tcp sim packets and disable link by default`
- 修改前 checkpoint：`7060f44 checkpoint before tcp packet filtering review`。
- TCP 包审计：
  - packet header 固定 40 字节，type `0x02` metrics、`0x01` JPEG image、`0x12` SIM line。
  - metrics 已按 1 秒发送。
  - image 已按 5 秒尝试生成，队列深度 2，发送按 8KB chunk yield。
  - 原 control task 每 20ms 调用 `sorting_sim_control_tick()`；scheduler 会每 tick 发 `STATUS,reason=tick`，有活动包裹时还会每 tick 发 `PKG,...pos=...`，日志层有过滤但 TCP 发包层没有过滤，存在刷屏风险。
- 已修改：
  - `SORTER_TCP_LINK_ENABLE` 默认改为 `0`，板端默认不启动 Ethernet control/image 模拟链路。
  - TCP SIM line 发送前过滤：
    - 丢弃 `STATUS` reason=`tick`/`sensor1`/`package_new`/`vision`。
    - `PKG` 只在 id/belt/state/class 签名变化时发送；未变化时最多 1 秒心跳一次，忽略 20ms pos 刷新。
  - JPEG queued/sent 周期日志从 `ESP_LOGI` 降为 `ESP_LOGD`。

## Verification

- 板端：
  - `motor-two-stage` `idf.py build`：通过；app 大小 `0x4a9380`，factory 分区剩余 `0x156c80`，约 22%。
  - `motor-roi` `idf.py build`：通过；app 大小 `0x4ab490`，factory 分区剩余 `0x154b70`，约 22%。
- 本轮未执行 flash/monitor。

## Notes

- 默认关闭后现场不会自动连接 `192.168.10.1:5000/5001`，也不会生成 5 秒 JPEG 或 1 秒 metrics。
- 如需重新启用 TCP 联调，可用编译定义覆盖 `SORTER_TCP_LINK_ENABLE=1`，但默认固件保持关闭。

## Next Step

- 如需现场验证，烧录后 monitor 应看到 `硬件分拣调试: TCP模拟链路已关闭`，且不应有 Ethernet connect/JPEG 周期日志。

## Blockers

- 未阻塞。
