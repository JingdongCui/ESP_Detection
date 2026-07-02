# History

## 2026-07-02 Handoff Documentation

- Read root `AGENTS.md`.
- Initialized root git repository and committed the existing docs baseline:
  - commit `91456ef docs: record current project docs baseline`
- Confirmed `esp32_host/.codegraph` exists and used CodeGraph before inspecting host protocol code.
- Confirmed protocol constants in `esp32_host/packetprotocol.h`.
- Confirmed host image path:
  - `HostController::handleImage` validates RGB888 payload, saves PNG, updates UI state, then calls `requestInference`.
- Confirmed inference path:
  - `HostController::requestInference` posts JSON to `/infer`.
  - `HostController::sendInferenceResultToDevice` sends response JSON as packet type `0x12`.
- Confirmed 26m training output:
  - `/home/kazeform/runs/detect/runs/logo/logo_yolo26m_150/weights/best.pt`
- Confirmed latest 300-image external test summary:
  - `295/300` detections at confidence `0.1`, max one detection per image.

## 2026-07-02 Real ESP32-P4 Host-Inference Chain

- Created ESP32 project backup before refactor:
  - commit `e52ec9e backup: teammate ui camera host pipeline wip`
  - tag `backup/before-camera-net-refactor-20260702`
- Created root backup before inference endpoint fix:
  - commit `e7940a8 backup: inference service raw endpoint wip`
- Extracted teammate reference project:
  - `/tmp/ESP32P4_Detection_ref/ESP32P4_Detection`
- Imported teammate UI generated/sdk assets into `esp32_project/components/UI`.
- Backed up previous debug/system UI assets:
  - `docs/agent/archive/2026-07-02-current-ui-debug-backup/`
- Replaced board camera capture with teammate-style `esp_video` MIPI CSI V4L2 path:
  - `1024 x 600`
  - RGB888
  - 3 buffers
  - ISP/IPA AWB, ACC, AGC enabled
- Root cause for camera startup failure:
  - Starting camera after Ethernet/LwIP caused CSI internal queue allocation failure:
    - `CSI: esp_cam_new_csi_ctlr: no memory for transaction queue`
    - `no available csi controller`
  - Fix was to reserve LwIP task semaphore early, then start camera before Ethernet driver/socket buffers.
- Root cause for camera green cast:
  - Old camera link/config did not use the teammate esp-video + ISP/IPA color path.
  - Fix was to use `esp_video_init`, MIPI CSI video device, RGB888 output, and AWB/ACC/AGC config.
- Root cause for host raw endpoint 500:
  - `_predict()` referenced `request.image_width` and `request.image_height` outside the request object scope.
  - Fixed `_predict()` to use explicit `width` and `height` parameters.
- Root cause for LwIP `thread_sem_init: out of memory`:
  - TCP task first called socket APIs after camera had consumed most internal RAM.
  - Fixed by creating the TCP task right after `esp_netif_init()` and calling `lwip_socket_thread_init()` in that task before camera startup.
- Root cause for LwIP socket `errno=105` after enabling motor:
  - Sorter debug/MCPWM startup consumed enough internal RAM to prevent socket allocation.
  - Disabled sorter debug/MCPWM startup in this validation firmware; old assets are backed up for later resource-aware re-integration.
- Latency optimization:
  - Full `1024 x 600` RGB888 upload was too slow.
  - `320 x 188` worked but later stabilized around 3-6 seconds.
  - Final validation uses `224 x 132` RGB888 upload, 88,704 bytes.
  - Metrics packets delayed to 10 seconds to avoid competing with image/result traffic.
- Final real-board verification:
  - `idf.py build`: passed.
  - `idf.py -p /dev/ttyUSB0 flash`: passed.
  - `idf.py -p /dev/ttyUSB0 monitor`: passed.
  - Observed frames:
    - `seq=1 total=360ms infer=20ms`
    - `seq=2 total=122ms infer=18ms`
    - `seq=3 total=72ms infer=19ms`
    - sustained later frames around `1.6-2.6s`
- Note:
  - Validation scene returned `det=0`, so no visible box was drawn in that run. The returned result path, coordinate parser, preview update, and total-latency display were verified.

## 2026-07-02 Full-Frame JPEG Latency Work

- Assessed feasibility of the requested `1024 x 600` under 0.5 s chain:
  - Existing validated firmware uploaded `224 x 132` RGB888, not full-frame.
  - Raw `1024 x 600` RGB888 is 1.84 MiB/frame and was judged high risk for stable sub-0.5 s on the current ESP32/LwIP path.
  - Host GPU inference itself is not the bottleneck; local raw `1024 x 600` service calls were about 19-25 ms.
- Chosen implementation:
  - Upload full-frame JPEG instead of raw RGB888.
  - Keep packet type `0x01`; add `pixel_format=2` for JPEG and keep `pixel_format=1` RGB888 compatibility.
- Board changes:
  - `Ethernet_app` now encodes the `1024 x 600` RGB888 camera frame with `esp_new_jpeg`.
  - JPEG quality fallback is `70 -> 60 -> 50`; target payload is `256 KiB`; output buffer is `768 KiB`.
  - TCP client task priority increased to `12`; frame pause disabled; result polling delay reduced to 1 ms.
  - Logs now include `dq/jpeg/send/wait/preview/total`, payload size, quality, and inference time.
- Host/service changes:
  - Qt host accepts JPEG image packets, posts them to `/infer_jpeg`, and overwrites `latest_frame.jpg`.
  - FastAPI service added `/infer_jpeg`, OpenCV in-memory decode, and `decode_ms` in the result JSON.
- Verification so far:
  - `cmake --build esp32_host/build/linux-release`: passed.
  - Direct JPEG service test with synthetic `1024 x 600` JPEG: passed; warm runs about 19-20 ms total, `decode_ms` 0-1 ms, `inference_ms` 18 ms.
  - `idf.py build`: passed.
  - First `idf.py -p /dev/ttyUSB0 flash monitor`: flashed and booted, host connection succeeded after PC host restart, but `esp_new_jpeg` failed to create its helper task under current internal RAM pressure.
  - Changed JPEG encoder to synchronous mode (`task_enable=false`) to avoid creating the helper task.
- Remaining:
  - Run `idf.py -p /dev/ttyUSB0 flash monitor` and validate p95 end-to-end board `total` under 500 ms.

## 2026-07-02 JPEG/RAW Final Latency Validation

- Added runtime upload-format handling:
  - Board supports `UPLOAD_FORMAT_JPEG` and `UPLOAD_FORMAT_RAW_RGB888`.
  - Host sends `upload_format` control on connect based on `ESP32_UPLOAD_FORMAT=jpeg|raw`.
  - Fixed the board's string parser so it only matches real JSON keys, not a key name appearing inside another value.
- Removed host hot-path image saving:
  - JPEG and RAW packets are forwarded to inference in memory.
  - UI/image/log/detection updates are sampled; result packet `0x12` is sent to the board before host UI work.
- Raised priority for the validation run:
  - Host process and Python inference service were reniced to `nice=-10` using sudo.
  - Uvicorn access log is disabled by default.
- JPEG final validation:
  - Final default firmware flashed with `idf.py -p /dev/ttyUSB0 flash monitor`.
  - Board confirmed `upload format set to JPEG`.
  - Representative frames: `total=340-342ms`, `encode=273-274ms`, `send=1ms`, `wait=64-67ms`, `infer=18-19ms`.
  - Occasional observed outliers remained, including around `580ms`, but the normal path is under 0.5s.
- RAW full-frame validation:
  - Temporarily set default upload mode to RAW and flashed for direct comparison.
  - Frame `seq=1`: `payload=1843200`, `send=15980ms`, `total=16801ms`, `infer=27ms`.
  - Frame `seq=3`: `payload=1843200`, `send=41983ms`, `total=42808ms`, `infer=29ms`.
  - Conclusion: direct `1024 x 600` RGB888 upload is not viable on the current ESP32/LwIP path; JPEG is the only practical full-frame path for the 0.5s goal.
- Recognition note:
  - The real-board scene used in this validation did not contain a trained `jt/zt/yd` logo, so detections were `det=0`.
  - Result packet parsing, board result display, and overlay path were still exercised.
