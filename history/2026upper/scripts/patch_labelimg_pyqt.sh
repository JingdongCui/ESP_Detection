#!/usr/bin/env bash
set -euo pipefail

target="/home/kazeform/2026upper/.venv-labelimg/lib/python3.11/site-packages/labelImg/labelImg.py"
python - <<'PY'
from pathlib import Path

path = Path("/home/kazeform/2026upper/.venv-labelimg/lib/python3.11/site-packages/labelImg/labelImg.py")
text = path.read_text(encoding="utf-8")
old = "        bar.setValue(bar.value() + bar.singleStep() * units)\n"
new = "        bar.setValue(int(bar.value() + bar.singleStep() * units))\n"
if old in text:
    path.write_text(text.replace(old, new), encoding="utf-8")
print(path)
PY
