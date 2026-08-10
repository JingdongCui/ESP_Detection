#!/usr/bin/env bash
set -euo pipefail

cd /home/kazeform/2026upper
source .venv/bin/activate

export PYTORCH_CUDA_ALLOC_CONF="${PYTORCH_CUDA_ALLOC_CONF:-expandable_segments:True}"

python ml/train_logo_yolo.py \
  --model "${MODEL:-models/yolo26m.pt}" \
  --data "${DATA:-datasets/logo_refined_yolo26m/data.yaml}" \
  --imgsz "${IMGSZ:-1024}" \
  --epochs "${EPOCHS:-25}" \
  --batch "${BATCH:-1}" \
  --name "${RUN_NAME:-logo_yolo26m_refined_25}"
