# Board UI Inventory

Date: 2026-07-05

## Scope

This document records the non-DEV board dashboard UI in `new_merge` and how the
desktop host should mirror it. The temporary board DEV/debug page is explicitly
out of scope for host UI parity.

## Dashboard / Live Monitoring

- Title and identity: ESP32-P4 express sorting / board-side vision dashboard.
- Live vision area:
  - Detection result/status.
  - Current confidence.
  - Vision FPS.
  - Processing/inference time label.
  - Current express company/category.
  - JT/ZT/YD probability bars.
  - Runtime CPU core 1 / core 2 arcs.
- Host mapping:
  - Keep the preview/detection page.
  - Show the latest board JPEG as the default live preview.
  - Keep confidence, FPS, processing time, category, and probability
    presentation when board detection JSON is available.
  - When only JPEG/metrics are available, show JPEG and link metrics without
    inventing local YOLO results.

## System Monitor

- Board displays:
  - Memory usage.
  - Internal RAM free.
  - PSRAM usage/free/total.
  - CPU01, CPU02, total CPU usage.
  - Free heap, minimum free heap, largest allocatable block.
  - Runtime, chip temperature, CPU frequency, task count.
- Host mapping:
  - Dashboard cards and charts should prefer board metrics with the same labels
    where the TCP metrics payload provides matching fields.
  - Host may additionally show JPEG link counters because these are board metrics
    produced by the TCP image path.

## Log / History

- Board displays paged log/history entries:
  - Time.
  - Event type.
  - Result.
  - Status.
  - Confidence.
  - Processing time.
  - Notes.
- Host mapping:
  - Keep the existing detection history and event flow style.
  - Remove local YOLO service language.
  - Use board detection records or board SIM line logs as source data.

## Settings / Controls

- Network settings visible on board:
  - Local IP.
  - Host IP.
  - Report interval.
  - Report image toggle.
  - Report metrics toggle.
- Display settings visible on board:
  - Screen brightness slider.
- Control settings visible on board:
  - Open-loop/PID mode.
  - Open-loop speed.
  - PID speed buttons: 20/30/40/50 cm/s.
- Detection settings visible on board:
  - Confidence threshold slider.
  - Detection enable switch.
  - Preview overlay switch.
- About/system actions visible on board:
  - Runtime.
  - Available memory/PSRAM.
  - Model information.
  - Factory reset.
  - Restart.

## Current Board Wiring Notes

- In the current board UI code, the brightness slider is wired to a BSP
  backlight handler.
- The logo button is wired to an ROI calibration handler.
- Confidence threshold, detection switch, preview overlay, open-loop/PID controls
  exist in generated UI, but no business handler was found in `components/UI/sdk`
  for the current code path.
- TCP SIM line supports sorter `CONFIG` fields including motor speeds:
  `a_speed`, `b_speed`, `c_speed`.

## Host Parity Boundary

- Mirror ordinary dashboard/settings concepts:
  - Live JPEG preview.
  - Board metrics.
  - Detection history/logs.
  - Confidence threshold.
  - Detection toggle.
  - Preview overlay toggle.
  - Motor speed control.
- Do not mirror temporary DEV/debug page content:
  - S1-S4 live sensor panel.
  - Encoder panel.
  - MTEST.
  - Package injection.
  - `ENC_CLEAR`.
  - Dedicated `HW_STATUS` control panel.
