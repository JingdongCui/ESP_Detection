# 最终长稳与 UVC profile 验证（2026-07-17）

## 生产推理 profile

推荐固件为 `c26dba8`，标签 `backup/final-production-candidate-20260717`。默认 `CONFIG_SCREEN_UVC_ENABLE=n`，保留完整相机、LCD、真实 S1/S2/S4、电机、Ethernet、CONTROL_JSON、Host 双通道和强化 ESP-DL guard。

其性能基线 `60c9f8a` 完成 3660.179 s 连续长稳：boot=1、fatal=0、guard rejection=0、双端口连接检查失败=0，并在 3602 s 取得第二份 24/24 task 与 heap snapshot。1048 样本 P50/P95/max=67.308/74.273/217.760 ms，8 个 `>=150 ms`、0 个 `>=500 ms`；wait P50/P95/max=42.898/48.326/192.626 ms，`corr(wb,wait)=0.9917`。连续性、P50/P95 和零 500 ms 通过，但完整长稳集合的 max 不通过 150 ms 门槛，不能标 stable。

60 分钟末 heap integrity=ok，heap free/min=6835531/6809627 B；internal free/largest=27279/14836 B，DMA free/largest=1715/48 B，PSRAM free/largest=6809124/6684660 B。24/24 task snapshot 成功，`dl_mc0/1` 最低剩余为 872/1785 B 与 880/1785 B，均大于 512 B 和 20%。

生产 profile 最终完整 `idf.py flash monitor` 所有镜像 Hash verified，启动明确打印 `screen UVC disabled in production inference profile`、Ethernet 192.168.10.2、5000/5001 connected、真实 sorter enabled、24 tasks、heap integrity=ok。随后 1×60 回归 P50/P95/max=67.718/71.507/72.325 ms，零 `>=150/500 ms`，四项严格门槛全部通过。

## UVC 专用 profile

`7a42b1f` 把 JPEG engine 的两个 cache-line 对齐内部 DMA descriptor 提前到 LCD 后、相机/Ethernet 前预留，解决启动末尾 DMA largest 48～76 B 导致的 `no memory for jpeg encoder rxlink`。实板启动日志确认：

```text
screen_uvc: jpeg engine reserved early (dma free=338663 largest=253940)
usbd_uvc: UVC Device Start, Version: 1.3.1
screen_uvc: screen UVC stream started: default 1024x600 MJPEG q90
```

`b0e7a02` 进一步把 TinyUSB/UVC task 从 5/4 降到 3/3，仍无法消除干扰。UVC 空闲 1×60 的 P50/P95/max=71.552/394.671/486.401 ms，12 个 `>=150 ms`、0 个 `>=500 ms`。因此该提交仅以 `backup/uvc-starts-latency-regression-20260717` 保存为 UVC 启动实验版，不用于生产推理。

现场电脑 `lsusb` 只有 CP2102 串口和既有 Sonix 摄像头，未接入或未枚举 ESP32-P4 USB OTG；所以本轮只证明板端 UVC/JPEG/TinyUSB 初始化成功，没有主机枚举、拉流画质、帧率或视频长稳证据。

最终 `c26dba8` 增加 `CONFIG_SCREEN_UVC_ENABLE` profile 开关，默认关闭。需要 UVC 专用构建时启用该配置即可得到已验证可启动的路径，但必须接受当前推理尾延迟回退，并在接好 USB OTG 后继续主机拉流测试。

## 证据文件

- `2026-07-17-stability-ui-isolated-61min.json/.log`
- `2026-07-17-inference-ui-isolated-5x60.json`
- `2026-07-17-inference-uvc-idle-1x60.json`
- `2026-07-17-inference-final-production-1x60.json`

## 尚需现场完成

- 五次真正断电冷启动；现有多轮为 CP2102N RTS reset。
- LCD 触摸/页面切换手感、亮度和 ISP 画面实操。
- 真实包裹 camera→检测框→分类→S1/S2/S4→三路电机全链路照片/录像。
- 接入 USB OTG 后的 UVC 枚举、MJPEG 拉流、画质/帧率和视频稳定性。
- 对完整长稳集合的 8 个 150～218 ms wait 尖峰继续做 core-specific ESP-DL worker/高优先级 ISP 中断诊断。
