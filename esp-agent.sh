#!/bin/bash
#
# esp-agent.sh — ESP32 agentic 统一入口
#
# 一条命令搞定一切：
#   - 首次运行：交互式配置 → 部署工具到当前项目 → 启动 OpenOCD
#   - 后续运行：直接启动 OpenOCD 会话
#
# 用法（在 ESP-IDF 终端中，进入你的项目目录后执行）：
#   bash /path/to/esp32-agentic/esp-agent.sh
#
# 部署完成后，项目根目录会有一份副本，以后直接：
#   bash esp-agent.sh
#

set -e

# ── 定位 ────────────────────────────────────────────

SCRIPT_PATH="$(cd "$(dirname "${BASH_SOURCE[0]:-$0}")" && pwd)/$(basename "${BASH_SOURCE[0]:-$0}")"
SCRIPT_DIR="$(dirname "$SCRIPT_PATH")"
PROJECT_DIR="$(pwd)"

CONFIG="$PROJECT_DIR/agentic/esp_target_config.json"

info() { echo "[esp-agent] $*"; }
die()  { echo "[esp-agent] ERROR: $*" >&2; exit 1; }

# ── 确保 ESP-IDF 环境已激活 ────────────────────────

if ! command -v openocd >/dev/null 2>&1; then
    if [ -n "$IDF_PATH" ] && [ -f "$IDF_PATH/export.sh" ]; then
        info "ESP-IDF 环境未激活，正在自动加载..."
        . "$IDF_PATH/export.sh" >/dev/null 2>&1
    elif [ -f "$HOME/esp/esp-idf/export.sh" ]; then
        info "ESP-IDF 环境未激活，正在从 ~/esp/esp-idf 加载..."
        export IDF_PATH="$HOME/esp/esp-idf"
        . "$IDF_PATH/export.sh" >/dev/null 2>&1
    fi
    if ! command -v openocd >/dev/null 2>&1; then
        die "openocd 不在 PATH 中。请先激活 ESP-IDF 环境（source \$IDF_PATH/export.sh）。"
    fi
fi

# ── 判断仓库位置 ────────────────────────────────────

# esp-agent.sh 可能在仓库根目录，也可能已被复制到用户项目
REPO_ROOT=""
if [ -d "$SCRIPT_DIR/templates/configs" ] && [ -d "$SCRIPT_DIR/agentic" ]; then
    REPO_ROOT="$SCRIPT_DIR"
elif [ -d "$PROJECT_DIR/esp32-agentic/templates/configs" ]; then
    REPO_ROOT="$PROJECT_DIR/esp32-agentic"
elif [ -d "$PROJECT_DIR/../esp32-agentic/templates/configs" ]; then
    REPO_ROOT="$(cd "$PROJECT_DIR/../esp32-agentic" && pwd)"
fi

# ══════════════════════════════════════════════════════
# 已配置 → 直接启动 session
# ══════════════════════════════════════════════════════

if [ -f "$CONFIG" ]; then
    SESSION_SH="$PROJECT_DIR/agentic/esp-session-start.sh"
    if [ ! -f "$SESSION_SH" ]; then
        die "配置文件存在但 esp-session-start.sh 缺失，请重新配置。"
    fi
    info "检测到已有配置，启动 OpenOCD 会话..."
    echo ""
    bash "$SESSION_SH"
    echo ""
    info "会话已启动。运行 claude 开始开发。"
    exit 0
fi

# ══════════════════════════════════════════════════════
# 未配置 → 进入配置向导
# ══════════════════════════════════════════════════════

if [ -z "$REPO_ROOT" ]; then
    die "找不到 esp32-agentic 仓库。请确保从仓库目录运行，或仓库在项目目录/同级目录中。"
fi

TEMPLATES="$REPO_ROOT/templates/configs"

echo ""
echo "╔══════════════════════════════════════════════╗"
echo "║   ESP32 agentic — 首次配置向导               ║"
echo "╚══════════════════════════════════════════════╝"
echo ""
echo "  工具仓库: $REPO_ROOT"
echo "  目标项目: $PROJECT_DIR"
echo ""

# ── 1. 平台自动检测 ──────────────────────────────

case "$(uname -s)" in
    MINGW*|MSYS*|CYGWIN*) PLATFORM="windows" ;;
    Darwin*)              PLATFORM="macos" ;;
    *)                    PLATFORM="linux" ;;
esac
echo "  检测到平台: $PLATFORM"
echo ""

# ── 2. 芯片选择 ──────────────────────────────────

echo "请选择目标芯片（均内置 USB-JTAG）："
echo ""
echo "  a) ESP32-C3    b) ESP32-C6    c) ESP32-H2"
echo "  d) ESP32-S3    e) ESP32-P4"
echo ""
read -rp "输入字母 [a-e]: " chip_choice

case "$chip_choice" in
    a|A) CHIP="esp32c3" ;;
    b|B) CHIP="esp32c6" ;;
    c|C) CHIP="esp32h2" ;;
    d|D) CHIP="esp32s3" ;;
    e|E) CHIP="esp32p4" ;;
    *) echo "无效选择，退出。"; exit 1 ;;
esac
echo "  → 芯片: $CHIP"
echo ""

# ── 3. 烧录方式 ──────────────────────────────────

if [ "$CHIP" = "esp32p4" ]; then
    DEFAULT_FLASH="serial"
else
    DEFAULT_FLASH="jtag"
fi

echo "请选择烧录方式："
echo "  1) JTAG  — OpenOCD program_esp（推荐 C3/C6/H2/S3）"
echo "  2) 串口  — esptool.py USB CDC（推荐 P4 或大固件）"
echo ""
read -rp "输入编号 [1-2]（默认: $DEFAULT_FLASH）: " flash_choice

case "$flash_choice" in
    1) FLASH_METHOD="jtag" ;;
    2) FLASH_METHOD="serial" ;;
    "") FLASH_METHOD="$DEFAULT_FLASH" ;;
    *) echo "无效选择，退出。"; exit 1 ;;
esac
echo "  → 烧录方式: $FLASH_METHOD"
echo ""

# ── 4. 部署工具到项目 ───────────────────────────

info "部署 agentic/ 工具目录..."

mkdir -p "$PROJECT_DIR/agentic/chips"
cp "$REPO_ROOT/agentic/esp_target.py"       "$PROJECT_DIR/agentic/"
cp "$REPO_ROOT/agentic/rtt_reader.py"       "$PROJECT_DIR/agentic/"
cp "$REPO_ROOT/agentic/svd_parser.py"       "$PROJECT_DIR/agentic/"
cp "$REPO_ROOT/agentic/esp-session-start.sh" "$PROJECT_DIR/agentic/"
cp "$REPO_ROOT/agentic/esp-session-stop.sh"  "$PROJECT_DIR/agentic/"
cp "$REPO_ROOT/agentic/idf_build.sh"        "$PROJECT_DIR/agentic/"
cp "$REPO_ROOT/agentic/SEGGER_RTT_Conf.h"   "$PROJECT_DIR/agentic/"
cp "$REPO_ROOT/agentic/SEGGER_RTT.c"        "$PROJECT_DIR/agentic/"
cp "$REPO_ROOT/agentic/SEGGER_RTT.h"        "$PROJECT_DIR/agentic/"
cp "$REPO_ROOT/agentic/SEGGER_RTT_printf.c" "$PROJECT_DIR/agentic/"
chmod +x "$PROJECT_DIR/agentic/"*.sh

# 芯片配置 + SVD
cp "$REPO_ROOT/agentic/chips/$CHIP.json" "$PROJECT_DIR/agentic/chips/"
if [ -f "$REPO_ROOT/agentic/chips/$CHIP.svd" ]; then
    cp "$REPO_ROOT/agentic/chips/$CHIP.svd" "$PROJECT_DIR/agentic/chips/"
fi

# ── 5. 生成 esp_target_config.json ──────────────

TEMPLATE="$TEMPLATES/$CHIP.json"
[ -f "$TEMPLATE" ] || die "模板不存在: $TEMPLATE"

cp "$TEMPLATE" "$CONFIG"

python3 -c "
import json, sys
config_path, platform, flash_method = sys.argv[1], sys.argv[2], sys.argv[3]
with open(config_path) as f:
    cfg = json.load(f)
cfg['platform'] = platform
cfg.setdefault('flash', {})['method'] = flash_method
if flash_method == 'serial' and 'serial' not in cfg.get('flash', {}):
    cfg['flash']['serial'] = {
        'port': 'auto', 'baud': 460800,
        'flash_mode': 'dio', 'flash_freq': '40m', 'flash_size': '16MB'
    }
with open(config_path, 'w') as f:
    json.dump(cfg, f, indent=2)
    f.write('\n')
" "$CONFIG" "$PLATFORM" "$FLASH_METHOD"

# ── 6. 复制 CLAUDE.md + esp-agent.sh + 测试脚本 + Skills ──

cp "$REPO_ROOT/templates/CLAUDE.md" "$PROJECT_DIR/CLAUDE.md"
cp "$SCRIPT_PATH" "$PROJECT_DIR/esp-agent.sh"
chmod +x "$PROJECT_DIR/esp-agent.sh"

# 部署 Claude Code 权限预设（免询问执行编译/烧录/调试命令）
mkdir -p "$PROJECT_DIR/.claude"
cp "$REPO_ROOT/.claude/settings.json" "$PROJECT_DIR/.claude/settings.json"

# 部署测试脚本（用户可自行验证工具链）
if [ -f "$REPO_ROOT/tests/test-full-toolchain.sh" ]; then
    cp "$REPO_ROOT/tests/test-full-toolchain.sh" "$PROJECT_DIR/test-full-toolchain.sh"
    chmod +x "$PROJECT_DIR/test-full-toolchain.sh"
fi

# 部署 Skills 目录（AI 工作流技能包）
if [ -d "$REPO_ROOT/Skills" ]; then
    cp -r "$REPO_ROOT/Skills" "$PROJECT_DIR/Skills"
fi

# ── 7. 复制 board.md 模板 ───────────────────────

BOARD_SRC=""
case "$CHIP" in
    esp32p4) BOARD_SRC="$REPO_ROOT/boards/esp32p4_function_ev.md" ;;
    esp32c3) BOARD_SRC="$REPO_ROOT/boards/codecell_c3.md" ;;
    esp32c6) BOARD_SRC="$REPO_ROOT/boards/waveshare_esp32c6.md" ;;
    esp32h2) BOARD_SRC="$REPO_ROOT/boards/esp32h2.md" ;;
    esp32s3) BOARD_SRC="$REPO_ROOT/boards/esp32s3.md" ;;
esac

if [ -n "$BOARD_SRC" ] && [ -f "$BOARD_SRC" ]; then
    cp "$BOARD_SRC" "$PROJECT_DIR/agentic/board.md"
fi

# ── 8. 配置完成，自动启动 session ───────────────

echo ""
echo "╔══════════════════════════════════════════════╗"
echo "║   配置完成！正在启动 OpenOCD...               ║"
echo "╚══════════════════════════════════════════════╝"
echo ""
echo "  平台: $PLATFORM | 芯片: $CHIP | 烧录: $FLASH_METHOD"
echo ""

bash "$PROJECT_DIR/agentic/esp-session-start.sh"

echo ""
info "一切就绪。运行 claude 开始开发。"
echo ""
info "以后每次开发只需："
info "  bash esp-agent.sh"
info "  claude"
