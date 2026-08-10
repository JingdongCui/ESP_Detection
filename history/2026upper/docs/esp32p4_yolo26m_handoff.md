# ESP32-P4 YOLO26m Handoff

## Target Chain

The next engineering target is the real device chain:

```text
ESP32-P4 --Ethernet/TCP--> Qt host --HTTP localhost--> YOLO26m service
ESP32-P4 <--Ethernet/TCP-- Qt host <--HTTP response-- YOLO26m service
```

The intended image size is `1024 x 600`. Current host protocol supports RGB888 payloads, so one raw frame is:

```text
1024 * 600 * 3 = 1,843,200 bytes
```

This fits the current `8 MiB` host payload limit.

## Current Assets

Project root:

```text
/home/kazeform/2026upper
```

Trained YOLO26m checkpoint:

```text
/home/kazeform/runs/detect/runs/logo/logo_yolo26m_150/weights/best.pt
```

Dataset:

```text
/home/kazeform/2026upper/datasets/logo_train_quick/data.yaml
```

Classes:

```text
0 jt
1 zt
2 yd
```

Latest 26m test:

```text
/home/kazeform/runs/detect/runs/logo_eval/yolo26m_external_test_300_conf010_max1
```

Test settings and result:

- Model: YOLO26m best checkpoint
- Confidence: `0.1`
- Max detections per image: `1`
- Test images: `300`
- Detected: `295/300`
- `jt`: `98/100`
- `zt`: `100/100`
- `yd`: `97/100`
- Average Ultralytics inference time: `15.7 ms/image`

## Host Protocol

Protocol source:

```text
/home/kazeform/2026upper/esp32_host/packetprotocol.h
```

Packet header is 40 bytes, little-endian:

```text
0  u32 magic         0x32505345
4  u16 version       1
6  u16 type
8  u32 header_size   40
12 u32 seq
16 u64 timestamp_ms
24 u32 payload_len
28 u16 width
30 u16 height
32 u16 pixel_format
34 u16 reserved      0
36 u32 reserved      0
```

Packet types:

```text
0x01 image RGB888 from ESP32-P4 to host
0x02 metrics JSON from ESP32-P4 to host
0x03 detection JSON from ESP32-P4/demo to host
0x10 time sync JSON from host to ESP32-P4
0x11 control JSON from host to ESP32-P4
0x12 inference result JSON from host to ESP32-P4
```

Pixel format:

```text
1 RGB888
```

ESP32-P4 image packet requirements:

- `type = 0x01`
- `width = 1024`
- `height = 600`
- `pixel_format = 1`
- `payload_len = width * height * 3`
- payload byte order is RGBRGBRGB, row-major, no per-row padding

## Current Host Implementation

Relevant files:

```text
/home/kazeform/2026upper/esp32_host/hostcontroller.cpp
/home/kazeform/2026upper/esp32_host/hostcontroller.h
/home/kazeform/2026upper/ml/logo_inference_service.py
```

Current host behavior:

1. `HostController::startServer` listens on `192.168.10.1:5000`, falling back to `0.0.0.0:5000`.
2. `HostController::processBuffer` parses the 40-byte packet header and complete payload.
3. `HostController::handleImage` validates RGB888 size, saves the frame as PNG under the user's documents folder, then calls `requestInference`.
4. `HostController::requestInference` posts this JSON to the local inference service:

```json
{
  "frame_seq": 1,
  "image_path": "/path/to/saved/frame.png",
  "image_width": 1024,
  "image_height": 600
}
```

5. `ml/logo_inference_service.py` runs YOLO and returns normalized boxes:

```json
{
  "frame_seq": 1,
  "image_width": 1024,
  "image_height": 600,
  "model": "best.pt",
  "inference_ms": 16,
  "detections": [
    {
      "label": "zt",
      "confidence": 0.73,
      "x": 0.41,
      "y": 0.28,
      "w": 0.08,
      "h": 0.05
    }
  ]
}
```

6. `HostController::sendInferenceResultToDevice` sends the same JSON back to ESP32-P4 with packet type `0x12`.

Coordinate convention:

- `x`, `y`, `w`, `h` are normalized to `[0, 1]`.
- `x`, `y` are the top-left corner of the bounding box.
- Pixel coordinates on ESP32-P4:

```c
int x1 = round(x * image_width);
int y1 = round(y * image_height);
int x2 = round((x + w) * image_width);
int y2 = round((y + h) * image_height);
```

## Required Change Before 26m Deployment

The service launch script currently defaults to the quick YOLO26s model:

```text
/home/kazeform/2026upper/scripts/start_inference_service.sh
```

Current default:

```bash
models/logo_yolo26s_quick.pt
```

For YOLO26m deployment, either pass the model explicitly:

```bash
cd /home/kazeform/2026upper
CONF=0.1 scripts/start_inference_service.sh /home/kazeform/runs/detect/runs/logo/logo_yolo26m_150/weights/best.pt
```

Or copy the checkpoint to a stable path and update the script default:

```bash
cd /home/kazeform/2026upper
cp /home/kazeform/runs/detect/runs/logo/logo_yolo26m_150/weights/best.pt models/logo_yolo26m_best.pt
```

Recommended runtime settings for the current data:

```bash
CONF=0.1
IMGSZ=1024
```

If only one logo should be returned, update `ml/logo_inference_service.py` to pass `max_det=1` to `model.predict`. The 300-image test used `max_det=1`; the current service code does not yet enforce that.

## Manual Startup

Terminal 1, start inference:

```bash
cd /home/kazeform/2026upper
source .venv/bin/activate
python ml/logo_inference_service.py \
  --model /home/kazeform/runs/detect/runs/logo/logo_yolo26m_150/weights/best.pt \
  --imgsz 1024 \
  --conf 0.1 \
  --device 0
```

Health check:

```bash
curl http://127.0.0.1:8765/health
```

Terminal 2, start host:

```bash
cd /home/kazeform/2026upper/esp32_host
cmake -S . -B build/linux-release -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build/linux-release
./build/linux-release/bin/esp32_host
```

Expected host status:

```text
正在监听 192.168.10.1:5000
```

## ESP32-P4 Side Checklist

1. Configure Ethernet so the board can connect to `192.168.10.1:5000`.
2. Send one TCP stream containing repeated protocol packets.
3. For each image packet:
   - Fill the 40-byte little-endian header.
   - Send exactly `width * height * 3` RGB888 bytes after the header.
   - Increment `seq` per frame.
4. Keep the socket open and read host packets from the same TCP connection.
5. Parse packet type `0x12`.
6. Parse JSON and use `detections[0]` if only one logo is needed.
7. Convert normalized box coordinates to pixels using `image_width` and `image_height`.

## Verification Plan

First local verification without ESP32-P4:

1. Start the inference service with the 26m checkpoint and `CONF=0.1`.
2. Run `curl /health` and confirm the model path and confidence.
3. Send a known saved image to `/infer` and confirm detections.

Then host verification:

1. Start Qt host.
2. Ping inference service from the model workspace page.
3. Connect a TCP test sender or ESP32-P4.
4. Confirm the host saves frames under:

```text
~/Documents/ESP32Host/images
```

5. Confirm the UI shows the latest frame and detections.
6. Confirm ESP32-P4 receives packet type `0x12`.
7. Log one returned JSON payload on ESP32-P4.

Pass condition:

- One `1024 x 600` image reaches the host.
- Host runs YOLO26m inference.
- Host returns a `0x12` packet for the same `frame_seq`.
- ESP32-P4 parses label, confidence, and pixel coordinates correctly.

## Later Optimization

- Avoid saving every frame to PNG before inference; send bytes to FastAPI or run YOLO inside the Qt process boundary through a worker process.
- Add `max_det` as a service CLI parameter.
- Add a stable model selector in the host UI.
- Add a synthetic TCP image sender script for repeatable host tests before using the board.
- Add packet-level CRC or length validation if Ethernet noise or partial-frame recovery becomes a problem.
- Consider sending JPEG from ESP32-P4 only if bandwidth is the bottleneck; raw RGB888 keeps parsing simple and avoids decode cost on the host.
