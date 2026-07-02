# Project Context

## Goal

Build an ESP32-P4 to desktop host pipeline for small courier-logo detection:

1. ESP32-P4 captures a `1024 x 600` RGB image and uploads a JPEG-compressed full-frame image over Ethernet.
2. The Qt host receives the image in memory and calls a local YOLO inference service.
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
- Image packet from ESP32-P4: type `0x01`, JPEG payload on the optimized path; RGB888 remains supported for compatibility
- Metrics JSON from ESP32-P4: type `0x02`
- Detection JSON from ESP32-P4/demo path: type `0x03`
- Time sync JSON to ESP32-P4: type `0x10`
- Control JSON to ESP32-P4: type `0x11`
- Inference result JSON to ESP32-P4: type `0x12`
- Pixel format RGB888: `1`
- Pixel format JPEG: `2`

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
- Board sends a full-frame JPEG inference image to the host by default:
  - Camera/preview frame remains `1024 x 600` RGB888.
  - Inference upload frame is `1024 x 600` JPEG, default quality 70 with fallback to 60/50 if payload exceeds the target size.
  - RAW `1024 x 600` RGB888 can be requested for testing with host env `ESP32_UPLOAD_FORMAT=raw`, but real-board validation showed it is far too slow for the 0.5 s target.
  - Host returns normalized box coordinates; board maps them back to `1024 x 600` before drawing.
- TCP/inference latency tuning:
  - Board TCP client task priority is `12`.
  - `esp_new_jpeg` helper task is disabled because real-board validation showed helper task creation fails under current internal RAM pressure.
  - LwIP tcpip task remains priority `18`.
  - Per-frame pause is disabled; result polling delay is 1 ms.
  - Host and inference service should be run at elevated process priority for latency validation; `nice=-10` was used in the latest tests.
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

Run host requesting RAW upload for comparison:

```bash
cd /home/kazeform/2026upper/esp32_host
ESP32_UPLOAD_FORMAT=raw ./build/linux-release/bin/esp32_host
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
- The optimized host path uses `/infer_jpeg`; `/infer_rgb888` remains available for compatibility and RAW comparison. The host does not save incoming JPEG/RAW frames on the hot path.
- Latest real-board latency result with `nice=-10` host/service:
  - JPEG: usually `340-342 ms` total, occasional observed outliers around `580 ms`; payload around `15 KiB` in the current scene.
  - RAW RGB888: `16.8 s` then `42.8 s` total for `1,843,200` byte payloads; not viable for the current target.
- Detection result can be `det=0` if the camera scene does not contain one of the trained logo classes. The transport/result/overlay path is still exercised; with a positive detection the board draws the returned box.
