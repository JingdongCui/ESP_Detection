# Current Task

## Objective

Keep the `1024 x 600` image quality unchanged and stabilize the full chain:

ESP32-P4 camera -> full-frame JPEG upload -> host YOLO inference -> compact result downlink -> board and host overlay display.

## Current State

- Default upload remains full-frame `1024 x 600` JPEG, packet type `0x01`, `pixel_format=2`.
- JPEG quality is fixed at `70`; the temporary lower-quality fallback was removed per requirement.
- Camera color correction now matches the teammate camera path more closely:
  - `CONFIG_ESP_VIDEO_ENABLE_ISP_PIPELINE_CONTROLLER=y`
  - `CONFIG_ESP_IPA_DETECT_METHOD_STATIC_STORE=y`
  - `CONFIG_ISP_PIPELINE_CONTROLLER_TASK_STACK_USE_PSRAM=y`
  - AWB/ACC/AGC remain enabled; image quality, resolution, and JPEG quality were not reduced.
- Host network path is split into two TCP channels:
  - control/result/metrics: `192.168.10.1:5000`
  - image upload: `192.168.10.1:5001`
- Host socket parsing/inference now runs in `HostNetworkWorker` on a dedicated `QThread`, keeping image receive and inference off the UI thread.
- Host sends compact result JSON before UI work; board downlink is typically `115-258` bytes depending on detection.
- Host displays the incoming inference image by atomically writing sampled previews to `~/Documents/ESP32Host/images/latest_preview.jpg` and refreshing `latestImageUrl`.
- Board uses `CONFIG_ETH_DMA_RX_BUFFER_NUM=8` and `CONFIG_ETH_DMA_TX_BUFFER_NUM=8`; the larger `12/16` attempt failed at boot because the EMAC RX task could not allocate internal memory.
- LwIP tuning kept:
  - `CONFIG_LWIP_TCP_SND_BUF_DEFAULT=65535`
  - `CONFIG_LWIP_TCP_WND_DEFAULT=65535`
  - `CONFIG_LWIP_IRAM_OPTIMIZATION=y`
  - IPv6 disabled

## Verification

- `cmake --build esp32_host/build/linux-release`: passed.
- `idf.py build`: passed.
- `idf.py -p /dev/ttyUSB0 flash`: passed.
- `idf.py -p /dev/ttyUSB0 monitor`: passed when run separately with a TTY.
- `idf.py -p /dev/ttyUSB0 flash monitor`: flash passed, but monitor failed in the non-interactive exec environment because stdin was not a TTY.
- Real-board dual-channel run:
  - Ethernet initialized successfully with RX/TX DMA buffers at `8/8`.
  - Board connected control `5000` and image `5001`.
  - Board confirmed `upload format set to JPEG`.
  - Frames ran through at `q=70`.
  - After enabling ISP pipeline controller with PSRAM stack, camera init left `internal=21335`, `largest_internal=19456`, and EMAC RX task creation succeeded.
  - Representative frames through about `seq=69`: `total=342-354ms`, `encode=276-284ms`, `send=2-6ms`, `wait=60-67ms`, `infer=18-19ms`, `host=60-62ms`.
  - Detection-result frames were present and did not cause the previous visible stalls during this monitor window.
  - Representative frames through about `seq=160`: `total=359-365ms`, `encode=290-295ms`, `send=2-8ms`, `wait=60-68ms`, `infer=18-20ms`, `host=60-64ms`.
  - No previous 1-2 second wait spikes were observed in this run.

## Notes

- The current largest fixed cost is board JPEG encode, around `290ms`.
- Result downlink is not the bottleneck: result packets are small and stable, and `wait` now tracks host inference/response around `60ms`.
- Weight check on 2026-07-02:
  - Current service script loads `/home/kazeform/runs/detect/runs/logo/logo_yolo26m_refined_25/weights/best.pt` by default.
  - Previous service command loaded `/home/kazeform/runs/detect/runs/logo/logo_yolo26m_150/weights/best.pt`; that file is the trained 26m run output, not the original `models/yolo26m.pt`.
  - Fixed inference service color handling: OpenCV JPEG decode is kept as BGR for Ultralytics, and RGB888 board payload is converted RGB -> BGR before prediction.
  - Service was restarted with the fixed code, but non-interactive sudo was unavailable, so the new service process could not be reniced back to `-10`.
- Full dataset inspection output:
  - Report: `/home/kazeform/runs/detect/runs/inspect/logo_yolo26m_dataset_report.md`
  - Train annotated predictions: `/home/kazeform/runs/detect/runs/inspect/logo_yolo26m_train_best`
  - Val annotated predictions: `/home/kazeform/runs/detect/runs/inspect/logo_yolo26m_val_best`
  - Val metrics/plots: `/home/kazeform/runs/detect/runs/inspect/logo_yolo26m_val_metrics`

## Current YOLO Refinement Task

- Source prediction output: `/home/kazeform/runs/detect/runs/inspect/logo_yolo26m_all_datasets_best`.
- Refinement script: `scripts/refine_yolo_predict_dataset.py`.
- Refined dataset: `datasets/logo_refined_yolo26m`.
- Class source rule:
  - Use `/home/kazeform/2026upper/datasets` folder names and file names to resolve ground-truth class.
  - Numeric-only images are resolved through original continuous folders such as `000_jt`, `001_zt`, `004_yd`, `006_jt`.
  - Prediction boxes with a class different from the resolved class are treated as false detections.
  - If multiple boxes match the resolved class, keep the smallest `w*h` box.
  - If no correct-class box exists, remove that image from the refined training set.
- Refined dataset result:
  - total kept: `5947`
  - train: `4757`
  - val: `1190`
  - deleted for no correct-class detection: `107`
  - per class kept: `jt=1816`, `zt=2558`, `yd=1573`
- Training entry: `scripts/train_logo_yolo26m_refined_25.sh`.
- Completed 25-epoch training:
  - run: `/home/kazeform/runs/detect/runs/logo/logo_yolo26m_refined_25`
  - log: `logs/train_logo_yolo26m_refined_25.log`
  - best checkpoint: `/home/kazeform/runs/detect/runs/logo/logo_yolo26m_refined_25/weights/best.pt`
  - last checkpoint: `/home/kazeform/runs/detect/runs/logo/logo_yolo26m_refined_25/weights/last.pt`
  - command uses `models/yolo26m.pt`, `imgsz=1024`, `epochs=25`, `batch=1`.
- Inference integration:
  - `scripts/start_inference_service.sh` default model now points to the refined 25-epoch `best.pt`.
  - No inference test was run per user request.
- Green-cast follow-up:
  - Config comparison against `ESP32P4_Detection(4).zip` showed AWB/ACC/AGC and SC2336 defaults were already present, but the current firmware had the ISP pipeline controller disabled.
  - The code/config fix has been flashed and the transport path is stable; final visual confirmation of whether the physical screen is no longer green still needs human inspection of the panel.
