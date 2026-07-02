#!/usr/bin/env bash
set -euo pipefail

cd /home/kazeform/2026upper
source .venv/bin/activate
python ml/logo_inference_service.py \
    --model "${1:-/home/kazeform/runs/detect/runs/logo/logo_yolo26m_refined_25/weights/best.pt}" \
    --imgsz "${IMGSZ:-1024}" \
    --conf "${CONF:-0.1}" \
    --max-det "${MAX_DET:-1}" \
    --device "${DEVICE:-0}"
