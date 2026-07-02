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
