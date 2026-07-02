#!/usr/bin/env bash
set -euo pipefail

cd /home/kazeform/2026upper
source .venv-label/bin/activate

export QT_AUTO_SCREEN_SCALE_FACTOR=1
export QT_QPA_PLATFORM=xcb
export QT_ENABLE_HIGHDPI_SCALING=0
export QT_STYLE_OVERRIDE=Fusion
exec xanylabeling \
  --filename /home/kazeform/2026upper/datasets/logo_label_450/images \
  --output /home/kazeform/2026upper/datasets/logo_label_450/labels \
  --labels /home/kazeform/2026upper/datasets/logo_label_450/classes.txt
