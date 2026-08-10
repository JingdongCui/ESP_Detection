#!/usr/bin/env python3
from __future__ import annotations

import argparse
import random
import re
import shutil
from collections import Counter, defaultdict
from dataclasses import dataclass
from pathlib import Path


CLASS_NAMES = ("jt", "zt", "yd")
CLASS_TO_ID = {name: idx for idx, name in enumerate(CLASS_NAMES)}
IMAGE_EXTS = {".jpg", ".jpeg", ".png", ".bmp", ".webp"}


@dataclass(frozen=True)
class ImageRecord:
    path: Path
    cls_name: str


@dataclass(frozen=True)
class KeptSample:
    source_image: Path
    output_name: str
    cls_id: int
    label_values: tuple[float, float, float, float]


def class_from_text(text: str) -> str | None:
    tokens = [t for t in re.split(r"[^a-zA-Z0-9]+", text.lower()) if t]
    found = [name for name in CLASS_NAMES if name in tokens]
    return found[0] if len(found) == 1 else None


def class_from_path(path: Path) -> str | None:
    parts = [path.name, *[p.name for p in path.parents]]
    for part in parts:
        cls_name = class_from_text(part)
        if cls_name:
            return cls_name
    return None


def source_priority(path: Path) -> tuple[int, str]:
    text = path.as_posix()
    priorities = (
        ("datasets/000_", 0),
        ("datasets/001_", 0),
        ("datasets/002_", 0),
        ("datasets/003_", 0),
        ("datasets/004_", 0),
        ("datasets/005_", 0),
        ("datasets/006_", 0),
        ("logo_external_test_300", 1),
        ("logo_external_test", 2),
        ("logo_label_450", 3),
        ("logo_yolo", 4),
        ("logo_train_quick", 5),
    )
    for needle, rank in priorities:
        if needle in text:
            return rank, text
    return 10, text


def build_image_index(datasets_root: Path, output_root: Path) -> dict[str, list[ImageRecord]]:
    by_name: dict[str, list[ImageRecord]] = defaultdict(list)
    resolved_output = output_root.resolve()
    for path in datasets_root.rglob("*"):
        if not path.is_file() or path.suffix.lower() not in IMAGE_EXTS:
            continue
        try:
            path.resolve().relative_to(resolved_output)
            continue
        except ValueError:
            pass
        cls_name = class_from_path(path.relative_to(datasets_root))
        if cls_name is None:
            continue
        by_name[path.name].append(ImageRecord(path=path, cls_name=cls_name))

    for records in by_name.values():
        records.sort(key=lambda rec: source_priority(rec.path))
    return by_name


def resolve_source(output_image: Path, index: dict[str, list[ImageRecord]]) -> ImageRecord | None:
    candidates = index.get(output_image.name, [])
    if not candidates:
        return None

    expected = class_from_text(output_image.stem)
    if expected:
        matching = [rec for rec in candidates if rec.cls_name == expected]
        if matching:
            return matching[0]

    classes = {rec.cls_name for rec in candidates}
    if len(classes) == 1:
        return candidates[0]
    return None


def read_best_matching_label(label_path: Path, target_cls_id: int) -> tuple[float, float, float, float] | None:
    if not label_path.exists():
        return None

    best: tuple[float, float, float, float] | None = None
    best_area: float | None = None
    for raw in label_path.read_text(encoding="utf-8").splitlines():
        parts = raw.split()
        if len(parts) < 5:
            continue
        try:
            cls_id = int(float(parts[0]))
            x, y, w, h = (float(v) for v in parts[1:5])
        except ValueError:
            continue
        if cls_id != target_cls_id:
            continue
        area = w * h
        if best_area is None or area < best_area:
            best_area = area
            best = (x, y, w, h)
    return best


def split_samples(samples: list[KeptSample], val_ratio: float, seed: int) -> tuple[list[KeptSample], list[KeptSample]]:
    rng = random.Random(seed)
    by_class: dict[int, list[KeptSample]] = defaultdict(list)
    for sample in samples:
        by_class[sample.cls_id].append(sample)

    train: list[KeptSample] = []
    val: list[KeptSample] = []
    for cls_samples in by_class.values():
        cls_samples.sort(key=lambda item: item.output_name)
        rng.shuffle(cls_samples)
        val_count = max(1, round(len(cls_samples) * val_ratio)) if len(cls_samples) > 1 else 0
        val.extend(cls_samples[:val_count])
        train.extend(cls_samples[val_count:])

    train.sort(key=lambda item: item.output_name)
    val.sort(key=lambda item: item.output_name)
    return train, val


def reset_output(output_root: Path) -> None:
    if output_root.exists():
        shutil.rmtree(output_root)
    for split in ("train", "val"):
        (output_root / "images" / split).mkdir(parents=True, exist_ok=True)
        (output_root / "labels" / split).mkdir(parents=True, exist_ok=True)


def write_split(output_root: Path, split: str, samples: list[KeptSample]) -> None:
    used_names: Counter[str] = Counter()
    for sample in samples:
        stem = Path(sample.output_name).stem
        ext = sample.source_image.suffix.lower()
        base_name = f"{stem}{ext}"
        used_names[base_name] += 1
        if used_names[base_name] > 1:
            base_name = f"{stem}_{used_names[base_name]}{ext}"

        dst_image = output_root / "images" / split / base_name
        dst_label = output_root / "labels" / split / f"{Path(base_name).stem}.txt"
        shutil.copy2(sample.source_image, dst_image)
        x, y, w, h = sample.label_values
        dst_label.write_text(f"{sample.cls_id} {x:.6g} {y:.6g} {w:.6g} {h:.6g}\n", encoding="utf-8")


def write_data_yaml(output_root: Path) -> None:
    names = "\n".join(f"  {idx}: {name}" for idx, name in enumerate(CLASS_NAMES))
    (output_root / "data.yaml").write_text(
        f"path: {output_root}\ntrain: images/train\nval: images/val\nnames:\n{names}\n",
        encoding="utf-8",
    )
    (output_root / "classes.txt").write_text("\n".join(CLASS_NAMES) + "\n", encoding="utf-8")


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Build a refined one-box-per-image YOLO dataset from a YOLO predict output."
    )
    parser.add_argument(
        "--predict-dir",
        type=Path,
        default=Path("/home/kazeform/runs/detect/runs/inspect/logo_yolo26m_all_datasets_best"),
    )
    parser.add_argument("--datasets-root", type=Path, default=Path("/home/kazeform/2026upper/datasets"))
    parser.add_argument("--output", type=Path, default=Path("/home/kazeform/2026upper/datasets/logo_refined_yolo26m"))
    parser.add_argument("--val-ratio", type=float, default=0.2)
    parser.add_argument("--seed", type=int, default=20260702)
    args = parser.parse_args()

    predict_dir = args.predict_dir
    labels_dir = predict_dir / "labels"
    if not predict_dir.is_dir():
        raise SystemExit(f"predict dir not found: {predict_dir}")
    if not labels_dir.is_dir():
        raise SystemExit(f"labels dir not found: {labels_dir}")

    image_index = build_image_index(args.datasets_root, args.output)
    kept: list[KeptSample] = []
    stats: Counter[str] = Counter()

    for output_image in sorted(
        (p for p in predict_dir.iterdir() if p.is_file() and p.suffix.lower() in IMAGE_EXTS),
        key=lambda path: path.name,
    ):
        source = resolve_source(output_image, image_index)
        if source is None:
            stats["unresolved_source"] += 1
            continue

        target_cls_id = CLASS_TO_ID[source.cls_name]
        label_values = read_best_matching_label(labels_dir / f"{output_image.stem}.txt", target_cls_id)
        if label_values is None:
            stats["no_correct_class_detection"] += 1
            continue

        kept.append(
            KeptSample(
                source_image=source.path,
                output_name=output_image.name,
                cls_id=target_cls_id,
                label_values=label_values,
            )
        )
        stats[f"kept_{source.cls_name}"] += 1

    train, val = split_samples(kept, args.val_ratio, args.seed)
    reset_output(args.output)
    write_split(args.output, "train", train)
    write_split(args.output, "val", val)
    write_data_yaml(args.output)

    report_lines = [
        "# Refined YOLO Dataset Report",
        "",
        f"- predict_dir: `{predict_dir}`",
        f"- datasets_root: `{args.datasets_root}`",
        f"- output: `{args.output}`",
        f"- kept_total: `{len(kept)}`",
        f"- train: `{len(train)}`",
        f"- val: `{len(val)}`",
        "",
        "## Stats",
        "",
    ]
    for key, value in sorted(stats.items()):
        report_lines.append(f"- {key}: `{value}`")
    report_lines.append("")
    (args.output / "refine_report.md").write_text("\n".join(report_lines), encoding="utf-8")

    print(f"output={args.output}")
    print(f"kept={len(kept)} train={len(train)} val={len(val)}")
    for key, value in sorted(stats.items()):
        print(f"{key}={value}")


if __name__ == "__main__":
    main()
