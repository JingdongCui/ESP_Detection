# Current Task

## Goal

完成 `new_merge` 低 revision 板端实机验证，并把板端 TCP/JPEG/metrics 链路与上位机 `esp32_host_no_inference` 对齐。

## Current State

- `new_merge` 分支：`motor-roi`。
- 当前已验证提交：`14ff3a2 move jpeg producer off busy core`。
- 关键提交链：
  - `9e4b23d optimize tcp image metrics link`
  - `ceae9b5 support low revision and restore uart console`
  - `50f132a allocate tcp task stacks in psram`
  - `12ec1cf fix jpeg snapshot cache sync`
  - `14ff3a2 move jpeg producer off busy core`
- 上位机 `esp32_host_no_inference` 已初始化独立 git：
  - 分支：`tcp-board-align`
  - 基线：`f9ae6d9 baseline host no inference`
  - 协议对齐：`2ad49e8 align host tcp protocol with board`

## Verification

- 上位机：
  - `cmake --preset debug`：通过。
  - `cmake --build --preset debug`：通过。
  - offscreen 启动后监听：
    - `192.168.10.1:5000`
    - `192.168.10.1:5001`
- 板端：
  - `idf.py build`：通过，app 大小 `0x4ec6c0`，factory 分区剩余约 18%。
  - `idf.py flash`：在 `12ec1cf` 全量烧录成功，bootloader/app/partition/storage 均 `Hash of data verified`。
  - `idf.py app-flash`：`14ff3a2` app 分区烧录成功并 `Hash of data verified`。
  - monitor 验证：
    - ESP32-P4 revision `v1.0`。
    - app `Min chip rev: v0.0`，`Max chip rev: v1.99`。
    - UART0 115200 console 正常输出。
    - S1=GPIO53，S2=GPIO23，S3 disabled，S4=GPIO22。
    - `System initialization done`。
    - TCP control/image 双连接建立，`ss` 显示 `5000/5001` 均 ESTABLISHED。
    - metrics 每秒写入 `~/Documents/ESP32Host/telemetry.jsonl`。
    - JPEG preview 正常：`image_encoded=15`、`image_sent=15`、drop/fail 均 0；最新 `latest_preview.jpg` 大小约 39 KB。

## Notes

- 低 revision 后 CPU 频率为 360 MHz，CPU1 长期接近 100%；JPEG producer 不能 pin 到 core1，否则会被视觉链路饿死。
- TCP task 栈需放到 PSRAM，否则 UI/vision 启动后内部 RAM 不足，三条 TCP task 会创建失败。
- ESP-IDF 5.5 的 `esp_cache_msync()` 在 M2C 方向不允许 `ESP_CACHE_MSYNC_FLAG_UNALIGNED`；snapshot buffer 需 cache-line 对齐后用纯 `ESP_CACHE_MSYNC_FLAG_DIR_M2C`。

## Next Step

- 将已验证的 TCP/revision 修正同步到 `motor-two-stage`：
  - `git switch motor-two-stage`
  - cherry-pick `9e4b23d ceae9b5 50f132a 12ec1cf 14ff3a2`
  - 解决冲突后 `idf.py build`
  - 保持最终语义：low revision、UART0 console、S1/S2/S3/S4 = `53/23/-1/22`、TCP task PSRAM stack、JPEG cache sync 修正、producer on core0。

## Blockers

- 未阻塞。串口偶发断流，`idf.py flash` 重试可成功；减少风险可用 `idf.py app-flash` 刷 app-only 改动。
