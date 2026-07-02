# Current Task

## Objective

Prepare handoff documentation for connecting the trained YOLO26m model into the upper-computer pipeline:

ESP32-P4 sends image over Ethernet -> Qt host receives and saves frame -> host runs YOLO inference -> host sends class and coordinates back to ESP32-P4.

## Current State

- Trained 26m checkpoint exists:
  - `/home/kazeform/runs/detect/runs/logo/logo_yolo26m_150/weights/best.pt`
- The Qt host has an existing TCP server on port `5000`.
- The Qt host parses image packet type `0x01`.
- The Qt host posts saved image paths to FastAPI `/infer`.
- The Qt host sends inference result JSON back to ESP32-P4 as packet type `0x12`.
- The inference service currently returns normalized `x`, `y`, `w`, `h`, `label`, and `confidence`.
- Latest 300-image external test used `conf=0.1`, `max_det=1`, 26m best checkpoint:
  - Total detected: `295/300`
  - `jt`: `98/100`
  - `zt`: `100/100`
  - `yd`: `97/100`
  - Ultralytics average inference: `15.7 ms/image`

## Next Steps

1. Copy or symlink the 26m best checkpoint into `models/` with a stable deployment name.
2. Update `scripts/start_inference_service.sh` default model to the 26m deployment checkpoint.
3. Decide runtime threshold:
   - Use `CONF=0.1` for high recall.
   - Keep `max_det=1` if only one logo should be returned per frame.
4. Confirm ESP32-P4 packet writer matches the 40-byte little-endian header.
5. Run host app and inference service together, then test with ESP32-P4 real TCP image packets.
6. On ESP32-P4, parse packet type `0x12` JSON and consume normalized coordinates/classes.

## Blockers

- Real ESP32-P4 image sender firmware is not verified in this workspace.
- Real end-to-end board test has not been run after the 26m training.
- Need final decision whether runtime should return all detections or only the best one per frame.
