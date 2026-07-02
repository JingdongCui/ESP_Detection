# Current Task

## Objective

Complete the real-board validation chain:

ESP32-P4 camera frame -> Ethernet to host -> host YOLO inference -> class/box result back to ESP32-P4 -> board preview overlay and one-frame total latency display.

## Current State

- Implemented with teammate UI/camera path imported from `ESP32P4_Detection(4).zip`.
- Backed up the previous system/debug UI assets under:
  - `docs/agent/archive/2026-07-02-current-ui-debug-backup/`
- Board camera now uses `esp_video` MIPI CSI V4L2 RGB888 at `1024 x 600`, 3 buffers.
- sdkconfig enables the relevant color/ISP path:
  - `CONFIG_ESP_VIDEO_ENABLE_MIPI_CSI_VIDEO_DEVICE=y`
  - `CONFIG_ESP_VIDEO_ENABLE_ISP_VIDEO_DEVICE=y`
  - `CONFIG_ESP_IPA_AWB_ALGORITHM=y`
  - `CONFIG_ESP_IPA_ACC_ALGORITHM=y`
  - `CONFIG_ESP_IPA_AGC_ALGORITHM=y`
- Board sends `224 x 132` RGB888 frames to the host for inference, while drawing the preview from the original `1024 x 600` camera frame.
- Host returns packet type `0x12`; board parses normalized coordinates and maps them to the original preview size.
- Board runtime label displays total one-frame latency when host inference returns.
- Sorter debug/MCPWM startup is disabled in the validation firmware to keep enough internal RAM for CSI/ISP and LwIP sockets. The old debug UI/system files are backed up for later re-integration.

## Verification

- `idf.py build`: passed.
- `idf.py -p /dev/ttyUSB0 flash`: passed.
- `idf.py -p /dev/ttyUSB0 monitor`: passed real-board chain verification.
- Inference service running on host:
  - PID `650945`
  - model `/home/kazeform/runs/detect/runs/logo/logo_yolo26m_150/weights/best.pt`
  - `imgsz=1024`, `conf=0.1`, `max_det=1`, `device=0`

Observed board logs from final validation:

```text
camera init: 1024x600 RGB888, 3 buffers, streaming
connected to host
send frame seq=1 camera=1024x600 infer=224x132 88704 bytes
host result seq=1 valid=0 ... infer=20ms
frame seq=1 total=360ms infer=20ms det=0
frame seq=2 total=122ms infer=18ms det=0
frame seq=3 total=72ms infer=19ms det=0
frame seq=16 total=1982ms infer=18ms det=0
frame seq=17 total=2097ms infer=18ms det=0
frame seq=19 total=2335ms infer=19ms det=0
```

## Blockers

- No active blocker for the requested chain.
- The camera scene during validation did not contain a trained courier logo, so returned detections were `det=0`; the result packet path and board latency display were still verified.
