# 2026 ESP32-P4 工作区

## 当前主工程

- `ESP32P4_Detection/`：从 `ESP32P4_Detection(9).zip` 解压建立的唯一活跃固件工程。
- 后续 ESP32-P4 固件开发、构建、烧录和调试都在该目录进行。

## 工程记录

- `docs/agent/PROJECT.md`：长期工程信息。
- `docs/agent/CURRENT.md`：当前任务和阻塞点。
- `docs/agent/HISTORY.md`：当前任务过程记录。

## 参考资料

- `reference/`：旧固件、上位机、数据集、历史工程、报告原始资料和第 9 版原包。
- 参考资料只用于查阅，不作为当前构建入口。分类说明见 `reference/README.md`。

## 常用命令

```bash
cd /home/kazeform/2026esp/ESP32P4_Detection
idf.py build
idf.py -p /dev/serial/by-id/usb-Silicon_Labs_CP2102N_USB_to_UART_Bridge_Controller_7ee2f3966ac3ee11be78b90f9e1b1c54-if00-port0 flash
idf.py -p /dev/serial/by-id/usb-Silicon_Labs_CP2102N_USB_to_UART_Bridge_Controller_7ee2f3966ac3ee11be78b90f9e1b1c54-if00-port0 monitor
```
