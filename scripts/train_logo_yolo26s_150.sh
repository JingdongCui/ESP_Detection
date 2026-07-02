#!/usr/bin/env bash
set -euo pipefail

cd /home/kazeform/2026upper
source .venv/bin/activate

export PYTORCH_CUDA_ALLOC_CONF="${PYTORCH_CUDA_ALLOC_CONF:-expandable_segments:True}"

python ml/train_logo_yolo.py \
  --model models/yolo26s.pt \
  --data datasets/logo_train_quick/data.yaml \
  --imgsz "${IMGSZ:-1024}" \
  --epochs "${EPOCHS:-150}" \
  --batch "${BATCH:--1}" \
  --name "${RUN_NAME:-logo_yolo26s_150}"
