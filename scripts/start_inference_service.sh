#!/usr/bin/env bash
set -euo pipefail

cd /home/kazeform/2026upper
source .venv/bin/activate
python ml/logo_inference_service.py --model "${1:-models/logo_yolo26s_quick.pt}" --imgsz "${IMGSZ:-1024}" --conf "${CONF:-0.25}"
