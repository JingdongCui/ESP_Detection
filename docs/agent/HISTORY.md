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
