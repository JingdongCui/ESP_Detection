# 61-Minute Stability Acceptance — 2026-07-17

## Verdict

FAIL. The run completed for 3660.059 wall-clock seconds, but the board panicked and rebooted once at elapsed 2412.116 s (about 40 minutes).

## Inference

- Samples: 826 `wb_only` diagnostic records
- P50: 75.533 ms
- P95: 101.564 ms
- Max: 161.204 ms
- `>=150 ms`: 15
- `>=500 ms`: 0
- Max wait: 136.697 ms
- Pearson correlation: `corr(wb,wait)=0.9961`, `corr(wb,cpu)=0.2250`

The persistent 500–600 ms regression remains eliminated. The strict P95<=100 ms and max<=150 ms limits still fail, and the long-tail samples are dominated by wait rather than caller-task CPU time.

## Crash

- Fatal: `Core 0 panic'ed (Instruction access fault)`
- `MEPC=0x10`, `RA=0x10`, `MCAUSE=1`, `MTVAL=0x10`
- Panic SP: `0x4ff20158`, inside the `dl_mc0` stack range (`0x4ff1fcf8`–`0x4ff203f0`)
- ELF SHA prefix: `c3b5f71d9`
- Reboot reason: `SW_CPU_RESET`
- Stack snapshot at startup reported `dl_mc0` free_min=928/1785 B; panic stack dump still contained substantial `0xA5` fill, so ordinary stack exhaustion is not the leading explanation.
- Symbols present in the panic stack resolve to `DualCoreWorkerTask`, `Module::forward_args`, `std::function` dispatch/destruction, and ESP32-P4 depthwise-convolution assembly. The invalid virtual/function dispatch target is `0x10`; exact corruption origin remains unresolved.

## Recovery and Memory

- Host PID 3266170 remained running.
- Firmware automatically rebooted, connected control and image channels again, synchronized time, and resumed inference. Final sockets were ESTABLISHED on ports 5000/5001.
- The 5-second Linux TCP-state sampler reported zero missing checks, but an old TCP socket can remain ESTABLISHED during a short board reboot; this metric alone is not a liveness proof. New firmware connection logs after reboot are the stronger recovery evidence.
- Both startup heap snapshots reported integrity=ok. The second snapshot is post-reboot startup, not a 60-minute end snapshot.
- Because the board rebooted before reaching 60 minutes of uninterrupted uptime, the scheduled 60-minute task/heap snapshot did not occur. Long-run minimum stack/heap acceptance remains incomplete.

## Evidence

- `2026-07-17-stability-61min.json`: incremental structured report
- `2026-07-17-stability-61min.log`: complete raw UART log including register and stack dump
- Firmware collector commit: `0150722`
