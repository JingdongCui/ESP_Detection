#!/usr/bin/env bash
set -euo pipefail

cd /home/kazeform/2026upper
source .venv-labelimg/bin/activate

export QT_QPA_PLATFORM=xcb
export QT_STYLE_OVERRIDE=Fusion
export HOME=/home/kazeform/2026upper/.labelimg_home
mkdir -p "$HOME"
cp /home/kazeform/2026upper/datasets/logo_label_450/classes.txt \
  /home/kazeform/2026upper/datasets/logo_label_450/labels/classes.txt
python - <<'PY'
import pickle
from pathlib import Path

from libs.constants import (
    SETTING_AUTO_SAVE,
    SETTING_LABEL_FILE_FORMAT,
    SETTING_SAVE_DIR,
)
from libs.labelFile import LabelFileFormat

settings = {
    SETTING_LABEL_FILE_FORMAT: LabelFileFormat.YOLO,
    SETTING_SAVE_DIR: "/home/kazeform/2026upper/datasets/logo_label_450/labels",
    SETTING_AUTO_SAVE: False,
}
Path.home().joinpath(".labelImgSettings.pkl").write_bytes(
    pickle.dumps(settings, pickle.HIGHEST_PROTOCOL)
)
PY
exec labelImg \
  /home/kazeform/2026upper/datasets/logo_label_450/images \
  /home/kazeform/2026upper/datasets/logo_label_450/labels/classes.txt \
  /home/kazeform/2026upper/datasets/logo_label_450/labels
