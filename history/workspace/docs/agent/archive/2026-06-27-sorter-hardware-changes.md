# Hardware And Board-Side Changes

Last updated: 2026-06-27, Asia/Shanghai.

This document records board-side and hardware-link changes made while fixing the sorter simulator GUI/TCP movement issue.

## Behavior Assumptions

- The simulator-to-board link must behave like real hardware: sensor messages do not carry package IDs.
- Real hardware mode uses real sensors as the source of truth.
- Motor-plus-timeout behavior is a fallback path. If sensor release or completion information is missing and the timeout expires, the model assumes the package safely reached the next state.
- If sensor information exists, it has priority over timeout inference.

## Root Cause Found

The board scheduler was shared by more than one task:

- The Ethernet/TCP task drove the scheduler with `sorting_sim_control_tick(send_sim_line_packet, sock)`.
- The USB serial debug task also called `serial_tick_active()` every 100 ms.
- During GUI/TCP runs, the USB serial tick could win the handoff timing window.
- When that happened, the scheduler emitted `MOTOR,id=2,...` and `STATUS reason=s2_released` to USB serial instead of TCP.
- The next TCP tick later reported `PKG ... state=on_b_to_class1`, so the Python UI saw the package on B while M2 still appeared stopped.

This is why C scheduler unit tests and serial headless tests did not catch the bug. They did not reproduce the real TCP GUI path with the USB debug tick competing for the shared scheduler sender.

## Board-Side Fixes

### TCP Send Reliability

File:

- `merge_project/components/Ethernet_app/ethernet_app.c`

Changes:

- Added `TCP_SEND_WAIT_MS`.
- Added `wait_socket_writable()`.
- Updated `send_all()` to retry `EINTR` and wait/retry on `EAGAIN` or `EWOULDBLOCK`.
- After connect, restored the socket from nonblocking connect mode to blocking send mode.
- Added a send timeout with `SO_SNDTIMEO`.
- Added `s_tcp_send_failed`.
- `send_sim_line_packet()` now marks send failure and logs the failed SIM line.
- TCP client task reconnects if a SIM line send fails, instead of silently advancing scheduler state after a failed send.

Reason:

- Scheduler state must not advance while the simulator misses motor/status lines because of transient TCP send failure.

### Scheduler Sender Ownership

File:

- `merge_project/components/Sorter_app/sorting_sim_control.c`

Changes:

- `serial_tick_active()` now returns without ticking if `s_downstream_send_fn` is already set to a non-serial sender.
- Manual USB serial debug commands still work, but USB serial automatic tick no longer steals the active TCP scheduler sender.

Reason:

- During real GUI/TCP runs, TCP must be the only automatic scheduler owner. Otherwise handoff motor commands can be sent to serial while the GUI only receives later board package state.

### Scheduler Logic Context

File touched during the broader fix:

- `merge_project/components/Sorter_app/sorter_core/sorter_scheduler.c`

Relevant expected behavior:

- S2 active: board can stop M1 while holding.
- S2 release plus handoff delay: board emits `MOTOR,id=2,dir=forward|reverse,speed=35` and `STATUS reason=s2_released`.
- Class 1 on B requires M2 forward.
- Class 2 or 3 route to S4 requires M2 reverse.

## Required Verification

Build:

```bash
cd /home/kazeform/2026esp/merge_project
idf.py build
```

Flash:

```bash
cd /home/kazeform/2026esp/merge_project
idf.py -p /dev/ttyACM0 flash
```

Real GUI/TCP verification:

```bash
cd /home/kazeform/2026esp
DISPLAY=:1 python -m esp32_sorter_sim_py \
  --transport tcp \
  --host 192.168.10.1 \
  --port 5000 \
  --ui-test-batch 8 \
  --ui-test-exit-after 90 \
  --log-file esp32_sorter_sim_py/logs/gui_tcp_after_pose_grace_8_90.log
```

Audit:

```bash
cd /home/kazeform/2026esp
python -m esp32_sorter_sim_py.log_audit esp32_sorter_sim_py/logs/gui_tcp_after_pose_grace_8_90.log
```

Verified result:

```text
audit_status=ok
RESULT ok mode=ui active=0 warnings=9 drops=0 pose_asserts=0 reason=
```

The final verified log also had:

```text
drops=0
pose_asserts=0
desyncs=0
faults=0
transport_packets=2546
```

## Regression Signals

Treat these as board/hardware-link failures until proven otherwise:

- TCP log has `PKG ... state=on_b_to_class1` but no earlier TCP `MOTOR,id=2,dir=forward,speed=...`.
- TCP log has `PKG ... state=on_b_to_s4` but no earlier TCP `MOTOR,id=2,dir=reverse,speed=...`.
- `RX_PACKET_GAP`
- `RX_PARSE_DROP`
- `BOARD_MOTOR_MISMATCH`
- Board sends package progress over TCP while motor/status lines are only visible through USB serial.

