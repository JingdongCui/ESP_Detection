#!/usr/bin/env bash
set -euo pipefail

cd /home/kazeform/2026upper
source .venv-label/bin/activate
exec xanylabeling
