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
  - TCP control/result/metrics port: `5000`
  - TCP image upload port: `5001`
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
- Refined pseudo-label dataset from all-datasets predict output:
  - `/home/kazeform/2026upper/datasets/logo_refined_yolo26m/data.yaml`
  - built by `scripts/refine_yolo_predict_dataset.py`
  - rule: resolve class from source dataset folder/filename, keep at most one smallest correct-class box per image, drop images with no correct-class box
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
- Image packet from ESP32-P4: type `0x01`, JPEG payload on the optimized path; RGB888 remains supported for compatibility/testing
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
  - `CONFIG_ESP_VIDEO_ENABLE_ISP_PIPELINE_CONTROLLER=y` is required for automated ISP statistics -> IPA algorithm -> ISP/sensor adjustment. AWB/ACC/AGC config alone is not enough.
  - `CONFIG_ISP_PIPELINE_CONTROLLER_TASK_STACK_USE_PSRAM=y` is required in the current firmware; otherwise enabling the pipeline controller leaves too little internal RAM and EMAC RX task creation fails.
  - `CONFIG_ESP_IPA_DETECT_METHOD_STATIC_STORE=y` plus a main-component linker keepalive for `__wrap_esp_ipa_detect_get_array` is used to avoid an ESP-IPA prebuilt/static-library link-order failure.
- Camera green-cast root cause was the old capture/config path bypassing the teammate ISP/IPA pipeline, and later the ISP pipeline controller being left disabled. The current path uses esp-video MIPI CSI + ISP pipeline controller + IPA color algorithms and delivers RGB888 to the preview/network path.
- Resource-ordering constraint:
  - Call `esp_netif_init()` first so LwIP TLS keys exist.
  - Create the TCP task immediately after that and call `lwip_socket_thread_init()` inside the task so its per-thread socket semaphore is allocated before CSI/ISP consumes internal RAM.
  - Start camera before Ethernet driver/socket buffers.
  - Do not start sorter debug/MCPWM in the host-inference validation firmware; it consumes enough internal RAM to make LwIP socket allocation fail with `errno=105`.
- Board sends a full-frame JPEG inference image to the host by default:
  - Camera/preview frame remains `1024 x 600` RGB888.
  - Inference upload frame is `1024 x 600` JPEG with fixed quality 70. Do not lower JPEG quality for latency work unless the user explicitly changes this requirement.
  - RAW `1024 x 600` RGB888 can be requested for testing with host env `ESP32_UPLOAD_FORMAT=raw`, but real-board validation showed it is far too slow for the 0.5 s target.
  - Host returns normalized box coordinates; board maps them back to `1024 x 600` before drawing.
- Transport topology:
  - Board opens a control socket to host port `5000` for metrics, control, time sync, and inference results.
  - Board opens a separate image socket to host port `5001` for full-frame uploads.
  - Host networking is handled by `HostNetworkWorker` on a dedicated `QThread`; packet parsing, image receive, and inference requests do not run on the UI thread.
  - Host sends the compact inference result to the board before updating UI state.
  - Host displays the current inference image by sampling incoming frames to `~/Documents/ESP32Host/images/latest_preview.jpg` and refreshing `latestImageUrl`.
- TCP/inference latency tuning:
  - Board TCP client task priority is `12`.
  - `esp_new_jpeg` helper task is disabled because real-board validation showed helper task creation fails under current internal RAM pressure.
  - LwIP tcpip task remains priority `18`.
  - Per-frame pause is disabled; result polling delay is 1 ms.
  - Host and inference service should be run at elevated process priority for latency validation; `nice=-10` was used in the latest tests.
  - Ethernet DMA RX/TX buffers are currently `8/8`; `12/16` failed to boot after camera/LVGL because EMAC RX task allocation ran out of internal memory.
  - LwIP TCP send/window buffers are `65535`, IPv6 is disabled, and LwIP IRAM optimization is enabled.
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

Run host app offscreen for board latency validation:

```bash
cd /home/kazeform/2026upper/esp32_host
QT_QPA_PLATFORM=offscreen ./build/linux-release/bin/esp32_host
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

Train refined 26m for 25 epochs:

```bash
cd /home/kazeform/2026upper
scripts/train_logo_yolo26m_refined_25.sh
```

## Known Issues

- `scripts/start_inference_service.sh` now has been used with the 26m checkpoint during real-board validation:
  - `/home/kazeform/runs/detect/runs/logo/logo_yolo26m_150/weights/best.pt`
- The Qt host inference call and packet `0x12` return path have been verified with real ESP32-P4 image packets.
- `kMaxPayload` is 8 MiB, enough for `1024 * 600 * 3 = 1.84 MiB`.
- The optimized host path uses `/infer_jpeg`; `/infer_rgb888` remains available for compatibility and RAW comparison. The host does not save incoming JPEG/RAW frames on the hot path.
- Inference service image color convention:
  - Ultralytics numpy input is treated as BGR and internally converted to RGB.
  - JPEG path should pass OpenCV `imdecode()` BGR output directly to YOLO.
  - RGB888 board payload must be converted RGB -> BGR before YOLO.
  - Do not convert decoded JPEG BGR -> RGB before calling `YOLO.predict`; that reverses colors in the model input and hurts recognition.
- Latest real-board latency result with `nice=-10` host/service and dual TCP channels:
  - JPEG q70: stable `359-365 ms` total through about 160 frames; `encode=290-295ms`, `send=2-8ms`, `wait=60-68ms`, `infer=18-20ms`, `host=60-64ms`, payload around `39-46 KiB` in the current scene.
  - Previous 1-2 second result-wait spikes were not reproduced after splitting image/control sockets and moving host networking off the UI thread.
  - After enabling the ISP pipeline controller and moving its task stack to PSRAM, real-board frames were stable around `342-354 ms`; Ethernet, dual sockets, host inference, and result downlink remained healthy with detection results present.
  - RAW RGB888: `16.8 s` then `42.8 s` total for `1,843,200` byte payloads; not viable for the current target.
- Do not increase Ethernet DMA RX/TX above `8/8` without rechecking internal RAM; a tested `12/16` config failed with `create emac_rx task failed`.
- Detection result can be `det=0` if the camera scene does not contain one of the trained logo classes. The transport/result/overlay path is still exercised; with a positive detection the board draws the returned box.
