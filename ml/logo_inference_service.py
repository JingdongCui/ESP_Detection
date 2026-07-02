#!/usr/bin/env python3
from __future__ import annotations

import argparse
import time
from pathlib import Path
from typing import Any

import numpy as np
from fastapi import FastAPI, HTTPException, Query, Request
from pydantic import BaseModel
from ultralytics import YOLO
import uvicorn


class InferRequest(BaseModel):
    frame_seq: int
    image_path: str
    image_width: int | None = None
    image_height: int | None = None


class LogoInferenceService:
    def __init__(self, model_path: str, imgsz: int, conf: float, device: str, max_det: int) -> None:
        self.model_path = model_path
        self.imgsz = imgsz
        self.conf = conf
        self.device = device
        self.max_det = max_det
        self.model = YOLO(model_path)

    def infer(self, request: InferRequest) -> dict[str, Any]:
        image_path = Path(request.image_path)
        if not image_path.is_file():
            raise HTTPException(status_code=404, detail=f"image not found: {image_path}")

        return self._predict(
            source=str(image_path),
            frame_seq=request.frame_seq,
            width=request.image_width,
            height=request.image_height,
        )

    def infer_rgb888(self, frame_seq: int, width: int, height: int, payload: bytes) -> dict[str, Any]:
        expected = width * height * 3
        if width <= 0 or height <= 0 or len(payload) != expected:
            raise HTTPException(
                status_code=400,
                detail=f"invalid RGB888 payload: got {len(payload)} bytes, expected {expected}",
            )
        image = np.frombuffer(payload, dtype=np.uint8).reshape((height, width, 3))
        return self._predict(source=image, frame_seq=frame_seq, width=width, height=height)

    def _predict(self, source: Any, frame_seq: int, width: int | None, height: int | None) -> dict[str, Any]:
        start = time.perf_counter()
        results = self.model.predict(
            source=source,
            imgsz=self.imgsz,
            conf=self.conf,
            device=self.device,
            max_det=self.max_det,
            verbose=False,
        )
        elapsed_ms = int((time.perf_counter() - start) * 1000)
        result = results[0]

        width = int(width or result.orig_shape[1])
        height = int(height or result.orig_shape[0])
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
            "frame_seq": frame_seq,
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
            "max_det": service.max_det,
        }

    @app.post("/infer")
    def infer(request: InferRequest) -> dict[str, Any]:
        return service.infer(request)

    @app.post("/infer_rgb888")
    async def infer_rgb888(
        request: Request,
        frame_seq: int = Query(...),
        image_width: int = Query(...),
        image_height: int = Query(...),
    ) -> dict[str, Any]:
        payload = await request.body()
        return service.infer_rgb888(frame_seq, image_width, image_height, payload)

    return app


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--model", default="models/yolo26s.pt")
    parser.add_argument("--imgsz", type=int, default=1024)
    parser.add_argument("--conf", type=float, default=0.25)
    parser.add_argument("--max-det", type=int, default=1)
    parser.add_argument("--device", default="0")
    parser.add_argument("--host", default="127.0.0.1")
    parser.add_argument("--port", type=int, default=8765)
    args = parser.parse_args()

    service = LogoInferenceService(args.model, args.imgsz, args.conf, args.device, args.max_det)
    uvicorn.run(create_app(service), host=args.host, port=args.port)


if __name__ == "__main__":
    main()
