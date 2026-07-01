#!/bin/bash
#
# esp-session-stop.sh — Tear down agentic firmware development infrastructure.
#
# Stops OpenOCD and any rtt_reader.py that may be running.
#

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
STATE_DIR="$SCRIPT_DIR/.esp-agent"

info() { echo "[session] $*"; }

# ── Platform detection ─────────────────────────────
case "$(uname -s)" in
    MINGW*|MSYS*|CYGWIN*) PLATFORM="windows" ;;
    *)                     PLATFORM="unix" ;;
esac

_kill_openocd_windows() {
    # PowerShell is reliable from Git Bash; taskkill is not
    powershell -NoProfile -Command "Get-Process openocd -ErrorAction SilentlyContinue | Stop-Process -Force" 2>/dev/null || true
}

if [ ! -d "$STATE_DIR" ]; then
    if [ "$PLATFORM" = "windows" ]; then
        _kill_openocd_windows && info "Killed orphan openocd.exe"
    else
        pkill -f "openocd" 2>/dev/null && info "Killed orphan openocd" || true
    fi
    info "No active session found."
    exit 0
fi

info "Stopping infrastructure..."

# Stop OpenOCD — kill all instances
if [ "$PLATFORM" = "windows" ]; then
    _kill_openocd_windows && info "Stopped OpenOCD"
else
    if [ -f "$STATE_DIR/openocd.pid" ]; then
        kill "$(cat "$STATE_DIR/openocd.pid")" 2>/dev/null && info "Stopped OpenOCD" || true
    fi
    pkill -f "openocd" 2>/dev/null || true
fi
rm -f "$STATE_DIR/openocd.pid"

# Kill any rtt_reader that may have been started during the session
if [ "$PLATFORM" = "windows" ]; then
    powershell -NoProfile -Command "Get-CimInstance Win32_Process | Where-Object { \$_.CommandLine -like '*rtt_reader*' } | ForEach-Object { Stop-Process -Id \$_.ProcessId -Force -ErrorAction SilentlyContinue }" 2>/dev/null || true
else
    pkill -f "rtt_reader.py" 2>/dev/null && info "Stopped RTT reader" || true
fi

# Wait for USB device release
sleep 2

info "Done. Logs preserved in $STATE_DIR/"
