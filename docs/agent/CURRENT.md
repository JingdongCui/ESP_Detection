# Current Task

## Objective

Optimize and validate the real-board full-frame camera-to-inference chain:

ESP32-P4 `1024 x 600` camera frame -> upload over Ethernet -> host YOLO inference -> class/box result back to ESP32-P4 -> board preview overlay and one-frame latency display.

## Current State

- Final default upload path is `1024 x 600` JPEG over packet type `0x01`, `pixel_format=2`.
- RAW `1024 x 600` RGB888 upload remains supported as `pixel_format=1` for comparison/testing, but is not the default.
- Host can request upload format on connect with `ESP32_UPLOAD_FORMAT=jpeg` or `ESP32_UPLOAD_FORMAT=raw`.
- Board JPEG uses synchronous `esp_new_jpeg`; helper task mode is disabled because real-board validation could not create the helper task under current internal-RAM pressure.
- Board TCP client task priority is `12`, per-frame pause is disabled, result polling delay is 1 ms, and TCP_NODELAY is enabled.
- Host disables image file saving on the hot path for both JPEG and RAW; UI/log/detection updates are sampled so result downlink is prioritized.
- Host and inference service were run with `nice=-10` for validation.
- FastAPI service supports `/infer_jpeg` and `/infer_rgb888`; uvicorn access log is off by default.

## Verification

- `cmake --build esp32_host/build/linux-release`: passed.
- `.venv/bin/python -m py_compile ml/logo_inference_service.py`: passed.
- `idf.py build`: passed.
- `idf.py -p /dev/ttyUSB0 flash monitor`: passed for final JPEG default firmware.
- JPEG final smoke test:
  - `payload` around 15 KiB in the current scene.
  - `encode` around 273-274 ms.
  - `send` around 1 ms.
  - host inference around 18-19 ms after warm-up.
  - total usually 340-342 ms; occasional observed outliers around 580 ms.
- RAW full-frame test:
  - `payload=1843200` bytes.
  - observed `send=15980 ms`, `total=16801 ms` on first frame.
  - observed `send=41983 ms`, `total=42808 ms` on next frame.
  - Conclusion: direct `1024 x 600` RGB888 upload is not viable for the 0.5 s target on the current ESP32/LwIP path.

## Notes

- The validation scene did not contain a trained courier logo, so results were `det=0`; transport, inference response, and overlay update paths were still exercised.
- For a recognition-quality comparison, place a trained `jt/zt/yd` logo in view and run the same JPEG/RAW modes; the current latency conclusion does not depend on positive detections.
