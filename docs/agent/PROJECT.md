# Project Context

## Goal

Build an ESP32-P4 to desktop host pipeline for small courier-logo detection:

1. ESP32-P4 captures or forwards a `1024 x 600` RGB image over Ethernet.
2. The Qt host receives the image, saves it, and calls a local YOLO inference service.
3. The host sends detected logo coordinates, class, and confidence back to ESP32-P4.
4. The host UI displays connection status, latest image, detections, model status, and logs.

## Hardware And Runtime

- Development machine: `/home/kazeform/2026upper`
- GPU: NVIDIA RTX 4060 Laptop GPU
- Python env: `/home/kazeform/2026upper/.venv`
- CUDA PyTorch installed in `.venv`
- Qt host app: `/home/kazeform/2026upper/esp32_host`
- Inference service: FastAPI + Ultralytics in `ml/logo_inference_service.py`
- Default network plan:
  - PC Ethernet: `192.168.10.1/24`
  - ESP32-P4: `192.168.10.2`
  - TCP port: `5000`
  - Local inference API: `http://127.0.0.1:8765`

## Model And Data

- Base models:
  - `models/yolo26s.pt`
  - `models/yolo26m.pt`
- Quick deployed s model:
  - `models/logo_yolo26s_quick.pt`
- Trained 26m best checkpoint:
  - `/home/kazeform/runs/detect/runs/logo/logo_yolo26m_150/weights/best.pt`
- YOLO dataset:
  - `/home/kazeform/2026upper/datasets/logo_train_quick/data.yaml`
- Classes:
  - `0`: `jt`
  - `1`: `zt`
  - `2`: `yd`

## Current Protocol

Packet header is defined in `esp32_host/packetprotocol.h`.

- Magic: `0x32505345` (`ESP2`, little-endian)
- Version: `1`
- Header size: `40`
- Max payload: `8 * 1024 * 1024`
- Image packet from ESP32-P4: type `0x01`, RGB888 payload
- Metrics JSON from ESP32-P4: type `0x02`
- Detection JSON from ESP32-P4/demo path: type `0x03`
- Time sync JSON to ESP32-P4: type `0x10`
- Control JSON to ESP32-P4: type `0x11`
- Inference result JSON to ESP32-P4: type `0x12`
- Pixel format RGB888: `1`

Header fields are little-endian:

```text
0  u32 magic
4  u16 version
6  u16 type
8  u32 header_size
12 u32 seq
16 u64 timestamp_ms
24 u32 payload_len
28 u16 width
30 u16 height
32 u16 pixel_format
34 u16 reserved
36 u32 reserved
```

## ESP32-P4 Current Validation Firmware

- Active board project: `/home/kazeform/2026upper/esp32_project`
- Teammate UI/camera reference was imported from `ESP32P4_Detection(4).zip`.
- Previous system/debug UI assets were backed up under:
  - `docs/agent/archive/2026-07-02-current-ui-debug-backup/`
- Current board preview/camera path:
  - SC2336 sensor over MIPI CSI through `esp_video`.
  - V4L2 output format is RGB888 (`V4L2_PIX_FMT_RGB24`), `1024 x 600`, 3 frame buffers.
  - ISP/IPA color controls are enabled in sdkconfig: AWB, ACC, AGC.
- Camera green-cast root cause was the old capture/config path bypassing the teammate ISP/IPA pipeline. The current path uses esp-video MIPI CSI + ISP color algorithms and delivers RGB888 to the preview/network path.
- Resource-ordering constraint:
  - Call `esp_netif_init()` first so LwIP TLS keys exist.
  - Create the TCP task immediately after that and call `lwip_socket_thread_init()` inside the task so its per-thread socket semaphore is allocated before CSI/ISP consumes internal RAM.
  - Start camera before Ethernet driver/socket buffers.
  - Do not start sorter debug/MCPWM in the host-inference validation firmware; it consumes enough internal RAM to make LwIP socket allocation fail with `errno=105`.
- Board sends a downscaled RGB888 inference frame to the host:
  - Camera/preview frame remains `1024 x 600`.
  - Inference upload frame is `224 x 132` RGB888, 88,704 bytes.
  - Host returns normalized box coordinates; board maps them back to `1024 x 600` before drawing.
- Metrics packets are delayed to a 10 second interval so they do not compete with image/result latency during validation.

## Useful Commands

Start inference service:

```bash
cd /home/kazeform/2026upper
source .venv/bin/activate
python ml/logo_inference_service.py --model /home/kazeform/runs/detect/runs/logo/logo_yolo26m_150/weights/best.pt --imgsz 1024 --conf 0.1 --device 0
```

Start host app:

```bash
cd /home/kazeform/2026upper/esp32_host
cmake -S . -B build/linux-release -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build/linux-release
./build/linux-release/bin/esp32_host
```

Train 26m:

```bash
cd /home/kazeform/2026upper
EPOCHS=150 scripts/train_logo_yolo26m_150.sh
```

## Known Issues

- `scripts/start_inference_service.sh` now has been used with the 26m checkpoint during real-board validation:
  - `/home/kazeform/runs/detect/runs/logo/logo_yolo26m_150/weights/best.pt`
- The Qt host inference call and packet `0x12` return path have been verified with real ESP32-P4 image packets.
- `kMaxPayload` is 8 MiB, enough for `1024 * 600 * 3 = 1.84 MiB`.
- The host currently saves incoming frames as PNG on the original `/infer` path. The validation path uses `/infer_rgb888` to avoid PNG disk I/O for board-sent raw RGB888 frames.
- Detection result can be `det=0` if the camera scene does not contain one of the trained logo classes. The transport/result/overlay path is still exercised; with a positive detection the board draws the returned box.
