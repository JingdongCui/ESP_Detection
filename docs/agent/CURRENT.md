# Current Task

## Objective

Keep the `1024 x 600` image quality unchanged and stabilize the full chain:

ESP32-P4 camera -> full-frame JPEG upload -> host YOLO inference -> compact result downlink -> board and host overlay display.

## Current State

- Default upload remains full-frame `1024 x 600` JPEG, packet type `0x01`, `pixel_format=2`.
- JPEG quality is fixed at `70`; the temporary lower-quality fallback was removed per requirement.
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
- `idf.py -p /dev/ttyUSB0 monitor`: passed.
- Real-board dual-channel run:
  - Ethernet initialized successfully with RX/TX DMA buffers at `8/8`.
  - Board connected control `5000` and image `5001`.
  - Board confirmed `upload format set to JPEG`.
  - Frames ran through at `q=70`.
  - Representative frames through about `seq=160`: `total=359-365ms`, `encode=290-295ms`, `send=2-8ms`, `wait=60-68ms`, `infer=18-20ms`, `host=60-64ms`.
  - No previous 1-2 second wait spikes were observed in this run.

## Notes

- The current largest fixed cost is board JPEG encode, around `290ms`.
- Result downlink is not the bottleneck: result packets are small and stable, and `wait` now tracks host inference/response around `60ms`.
- Weight check on 2026-07-02:
  - Current service command loads `/home/kazeform/runs/detect/runs/logo/logo_yolo26m_150/weights/best.pt`.
  - That file is the trained 26m run output, not the original `models/yolo26m.pt`.
  - Fixed inference service color handling: OpenCV JPEG decode is kept as BGR for Ultralytics, and RGB888 board payload is converted RGB -> BGR before prediction.
  - Service was restarted with the fixed code, but non-interactive sudo was unavailable, so the new service process could not be reniced back to `-10`.
