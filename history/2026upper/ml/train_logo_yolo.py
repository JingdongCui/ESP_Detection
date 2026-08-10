#!/usr/bin/env python3
from __future__ import annotations

import argparse

from ultralytics import YOLO


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--model", default="models/yolo26s.pt")
    parser.add_argument("--data", default="datasets/logo_yolo/data.yaml")
    parser.add_argument("--imgsz", type=int, default=1024)
    parser.add_argument("--epochs", type=int, default=120)
    parser.add_argument("--batch", default="-1")
    parser.add_argument("--device", default="0")
    parser.add_argument("--project", default="runs/logo")
    parser.add_argument("--name", default=None)
    args = parser.parse_args()

    batch_value: int | float
    batch_value = float(args.batch) if "." in args.batch else int(args.batch)

    model = YOLO(args.model)
    model.train(
        data=args.data,
        imgsz=args.imgsz,
        epochs=args.epochs,
        batch=batch_value,
        device=args.device,
        project=args.project,
        name=args.name,
        close_mosaic=15,
        cos_lr=True,
        patience=40,
        hsv_h=0.015,
        hsv_s=0.55,
        hsv_v=0.35,
        degrees=12,
        translate=0.08,
        scale=0.5,
        perspective=0.0008,
        mosaic=1.0,
        mixup=0.08,
        copy_paste=0.0,
    )


if __name__ == "__main__":
    main()
