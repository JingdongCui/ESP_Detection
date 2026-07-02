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

- `scripts/start_inference_service.sh` still defaults to `models/logo_yolo26s_quick.pt`; switch it to the 26m checkpoint before deployment or pass the 26m path as the first argument.
- The Qt host already has the inference call and packet `0x12` send path, but full ESP32-P4 real-device verification is not completed.
- `kMaxPayload` is 8 MiB, enough for `1024 * 600 * 3 = 1.84 MiB`.
- The host currently saves incoming frames as PNG before inference. This is simple and works, but adds disk I/O latency. Later optimization can pass in-memory bytes to the inference service.
