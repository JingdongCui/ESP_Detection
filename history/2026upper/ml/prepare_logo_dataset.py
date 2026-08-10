#!/usr/bin/env python3
from __future__ import annotations

import argparse
import random
import shutil
from pathlib import Path

import yaml


CLASS_BY_DIR = {
    "000_jt": "jt",
    "006_jt": "jt",
    "001_zt": "zt",
    "002_zt": "zt",
    "003_zt": "zt",
    "004_yd": "yd",
    "005_yd": "yd",
}
NAMES = ["jt", "zt", "yd"]


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--source", default="datasets")
    parser.add_argument("--output", default="datasets/logo_yolo")
    parser.add_argument("--seed", type=int, default=20260701)
    parser.add_argument("--val", type=float, default=0.15)
    parser.add_argument("--test", type=float, default=0.05)
    args = parser.parse_args()

    source = Path(args.source)
    output = Path(args.output)
    samples: list[tuple[Path, str]] = []
    for folder, label in CLASS_BY_DIR.items():
        for path in sorted((source / folder).glob("*.jpg")):
            samples.append((path, label))

    random.Random(args.seed).shuffle(samples)
    test_count = int(len(samples) * args.test)
    val_count = int(len(samples) * args.val)

    split_samples = {
        "test": samples[:test_count],
        "val": samples[test_count : test_count + val_count],
        "train": samples[test_count + val_count :],
    }

    for split, items in split_samples.items():
        image_dir = output / "images" / split
        label_dir = output / "labels" / split
        image_dir.mkdir(parents=True, exist_ok=True)
        label_dir.mkdir(parents=True, exist_ok=True)
        for src, label in items:
            dst = image_dir / f"{label}_{src.stem}{src.suffix.lower()}"
            if not dst.exists():
                shutil.copy2(src, dst)
            (label_dir / f"{dst.stem}.txt").touch(exist_ok=True)

    data = {
        "path": str(output.resolve()),
        "train": "images/train",
        "val": "images/val",
        "test": "images/test",
        "names": {i: name for i, name in enumerate(NAMES)},
    }
    with (output / "data.yaml").open("w", encoding="utf-8") as f:
        yaml.safe_dump(data, f, sort_keys=False, allow_unicode=True)
    (output / "classes.txt").write_text("\n".join(NAMES) + "\n", encoding="utf-8")

    print(f"Prepared {len(samples)} images under {output}")
    for split, items in split_samples.items():
        print(f"{split}: {len(items)}")


if __name__ == "__main__":
    main()
