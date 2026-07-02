#!/usr/bin/env python3
from __future__ import annotations

import argparse
import time
from pathlib import Path
from typing import Any

from fastapi import FastAPI, HTTPException
from pydantic import BaseModel
from ultralytics import YOLO
import uvicorn


class InferRequest(BaseModel):
    frame_seq: int
    image_path: str
    image_width: int | None = None
    image_height: int | None = None


class LogoInferenceService:
    def __init__(self, model_path: str, imgsz: int, conf: float, device: str) -> None:
        self.model_path = model_path
        self.imgsz = imgsz
        self.conf = conf
        self.device = device
        self.model = YOLO(model_path)

    def infer(self, request: InferRequest) -> dict[str, Any]:
        image_path = Path(request.image_path)
        if not image_path.is_file():
            raise HTTPException(status_code=404, detail=f"image not found: {image_path}")

        start = time.perf_counter()
        results = self.model.predict(
            source=str(image_path),
            imgsz=self.imgsz,
            conf=self.conf,
            device=self.device,
            verbose=False,
        )
        elapsed_ms = int((time.perf_counter() - start) * 1000)
        result = results[0]

        width = int(request.image_width or result.orig_shape[1])
        height = int(request.image_height or result.orig_shape[0])
        detections: list[dict[str, Any]] = []
        names = result.names
        boxes = result.boxes
        if boxes is not None:
            for box in boxes:
                x1, y1, x2, y2 = [float(v) for v in box.xyxy[0].tolist()]
                cls_id = int(box.cls[0].item())
                confidence = float(box.conf[0].item())
                detections.append(
                    {
                        "label": str(names.get(cls_id, cls_id)),
                        "confidence": confidence,
                        "x": max(0.0, x1 / width),
                        "y": max(0.0, y1 / height),
                        "w": max(0.0, (x2 - x1) / width),
                        "h": max(0.0, (y2 - y1) / height),
                    }
                )

        return {
            "frame_seq": request.frame_seq,
            "image_width": width,
            "image_height": height,
            "model": Path(self.model_path).name,
            "inference_ms": elapsed_ms,
            "detections": detections,
        }


def create_app(service: LogoInferenceService) -> FastAPI:
    app = FastAPI(title="ESP32 Logo Inference Service")

    @app.get("/health")
    def health() -> dict[str, Any]:
        return {
            "ok": True,
            "model": Path(service.model_path).name,
            "imgsz": service.imgsz,
            "conf": service.conf,
            "device": service.device,
        }

    @app.post("/infer")
    def infer(request: InferRequest) -> dict[str, Any]:
        return service.infer(request)

    return app


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--model", default="models/yolo26s.pt")
    parser.add_argument("--imgsz", type=int, default=1024)
    parser.add_argument("--conf", type=float, default=0.25)
    parser.add_argument("--device", default="0")
    parser.add_argument("--host", default="127.0.0.1")
    parser.add_argument("--port", type=int, default=8765)
    args = parser.parse_args()

    service = LogoInferenceService(args.model, args.imgsz, args.conf, args.device)
    uvicorn.run(create_app(service), host=args.host, port=args.port)


if __name__ == "__main__":
    main()
