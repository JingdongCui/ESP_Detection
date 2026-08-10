# sort_real_io 任务栈迁移 PSRAM

## 修改

- 参考旧工程，将 `sort_real_io` 的 4096 字节任务栈从普通 `xTaskCreatePinnedToCore` 改为 `xTaskCreatePinnedToCoreWithCaps`。
- 栈能力使用 `MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT`，任务优先级和 core0 绑定保持不变。
- 创建失败日志补充内部 RAM 和 PSRAM 可用量。
- 提交：`e1cbd5d fix: move sorter real IO stack to PSRAM`。

## 验证

- `idf.py build` 成功；app 大小 `0x5137b0`。
- 普通 `idf.py flash` 第一次成功，全部分区 Hash 校验通过。
- 串口启动版本为 `e1cbd5d`，完成 `System initialization done`。
- 原 `create real IO task failed` 不再出现。
- real-IO 任务输出 S1/S2/S3/S4 初始化日志，证明任务已成功运行；S1、S2、S4 valid=1，S3 因 GPIO 未配置为 valid=0。
- Ethernet 上位机 192.168.10.1:5000/5001 未提供服务，连接失败不影响本次任务栈验证。
