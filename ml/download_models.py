#!/usr/bin/env python3
from pathlib import Path

from ultralytics import YOLO


def main() -> None:
    out_dir = Path("models")
    out_dir.mkdir(parents=True, exist_ok=True)
    for name in ("yolo26s.pt", "yolo26m.pt"):
        model = YOLO(name)
        source = Path(model.ckpt_path)
        target = out_dir / name
        if source.resolve() != target.resolve():
            target.write_bytes(source.read_bytes())
        print(target)


if __name__ == "__main__":
    main()
