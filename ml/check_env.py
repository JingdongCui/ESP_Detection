#!/usr/bin/env python3
import torch
from ultralytics import YOLO


def main() -> None:
    print("torch", torch.__version__)
    print("cuda_available", torch.cuda.is_available())
    if torch.cuda.is_available():
        print("gpu", torch.cuda.get_device_name(0))
    model = YOLO("models/yolo26s.pt")
    result = model.predict(
        "datasets/002_zt/1253.jpg",
        imgsz=1024,
        device=0 if torch.cuda.is_available() else "cpu",
        verbose=False,
    )
    print("smoke_detections", len(result[0].boxes) if result[0].boxes is not None else 0)


if __name__ == "__main__":
    main()
