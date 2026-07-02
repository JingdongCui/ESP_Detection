# ESP32-P4 Logo Detection Setup

## Paths

- Project root: `/home/kazeform/2026upper`
- Qt host: `/home/kazeform/2026upper/esp32_host`
- Raw images: `/home/kazeform/2026upper/datasets`
- YOLO dataset: `/home/kazeform/2026upper/datasets/logo_yolo`
- Models: `/home/kazeform/2026upper/models`

## Environment

```bash
cd /home/kazeform/2026upper
uv venv --python 3.12 .venv
source .venv/bin/activate
uv pip install torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu128
uv pip install -r ml/requirements.txt
python ml/download_models.py
python ml/check_env.py
```

If CUDA is working, `check_env.py` prints `cuda_available True` and the RTX 4060 name.

## Annotation

Launch X-AnyLabeling:

```bash
/home/kazeform/2026upper/scripts/start_anylabeling.sh
```

For the prepared 450-image subset, use the desktop shortcut `Logo Label 450` or run:

```bash
/home/kazeform/2026upper/scripts/label_logo_450.sh
```

Label classes:

- `jt`: images from `000_jt` and `006_jt`
- `zt`: images from `001_zt`, `002_zt`, `003_zt`
- `yd`: images from `004_yd`, `005_yd`

Rules:

- Draw a tight rectangle around the courier logo or brand text only.
- Do not label the whole waybill, barcode, QR code, package, or hand.
- If one image has multiple logo appearances, label all of them.
- Blurry but still recognizable logos should be labeled.
- If the logo is fully unreadable, leave the image without a box.

Prepare YOLO folder layout before or after labeling:

```bash
cd /home/kazeform/2026upper
source .venv/bin/activate
python ml/prepare_logo_dataset.py
```

When exporting from X-AnyLabeling, choose YOLO format and use the same class order:

```text
jt
zt
yd
```

After export, check several label files in `datasets/logo_yolo/labels/*`. YOLO rows must look like:

```text
class_id center_x center_y width height
```

All coordinates are normalized from 0 to 1.

## Training

Start with the small model:

```bash
cd /home/kazeform/2026upper
source .venv/bin/activate
python ml/train_logo_yolo.py --model models/yolo26s.pt --data datasets/logo_yolo/data.yaml --imgsz 1024 --epochs 120 --name yolo26s_1024
```

Then compare the medium model:

```bash
python ml/train_logo_yolo.py --model models/yolo26m.pt --data datasets/logo_yolo/data.yaml --imgsz 1024 --epochs 120 --name yolo26m_1024
```

If small-logo recall is poor:

- Try `--imgsz 1280` if VRAM allows.
- Use sliced inference for final deployment.
- Try `yolo26-p2.yaml` only after baseline results; Ultralytics ships P2 as architecture-only, not pretrained weights.

## Runtime

Start inference service:

```bash
/home/kazeform/2026upper/scripts/start_inference_service.sh
```

Start Qt host:

```bash
cd /home/kazeform/2026upper/esp32_host
cmake -S . -B build/linux-release -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build/linux-release
./build/linux-release/bin/esp32_host
```

Network defaults:

- PC Ethernet: `192.168.10.1/24`
- ESP32-P4: `192.168.10.2`
- TCP port: `5000`
- Inference service: `http://127.0.0.1:8765`

The Qt host saves incoming RGB888 frames, posts the saved image path to the local inference service, displays returned boxes, and sends the same result JSON back to ESP32-P4 as packet type `0x12`.
