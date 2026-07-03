<!-- CODEGRAPH_START -->
## CodeGraph

In repositories indexed by CodeGraph (a `.codegraph/` directory exists at the repo root), reach for it BEFORE grep/find or reading files when you need to understand or locate code:

- **MCP tools** (when available): `codegraph_explore` answers most code questions in one call — the relevant symbols' verbatim source plus the call paths between them. `codegraph_node` returns one symbol's source + callers, or reads a whole file with line numbers. If the tools are listed but deferred, load them by name via tool search.
- **Shell** (always works): `codegraph explore "<symbol names or question>"` and `codegraph node <symbol-or-file>` print the same output.

If there is no `.codegraph/` directory, skip CodeGraph entirely — indexing is the user's decision.
<!-- CODEGRAPH_END -->

## Project Notes

- This is a Qt 6 desktop host application for communicating with an ESP32 board.
- Current build system: CMake, C++17, Qt 6.5 or newer.
- Required Qt modules currently declared in `CMakeLists.txt`: `Quick` and `Network`.
- The project is not hard-bound to a specific compiler or Windows-only kit in CMake.
- The current checked-in Qt Creator user configuration shows a Linux `Desktop` target using Ninja (`/usr/bin/ninja`) and Qt from `%{Qt:QT_INSTALL_PREFIX}`.
- No MinGW-specific or MSVC-specific configuration is currently committed in the CMake project files.
- For Windows development, use a Qt 6 Desktop kit, typically either MinGW or MSVC, as long as the selected Qt installation provides the required modules and matching compiler toolchain.
- Maintain compatibility with Qt for Windows MinGW kits. Do not introduce compiler-specific code that breaks MinGW unless there is a documented fallback.
- For Linux development, use a Qt 6 Desktop kit with Ninja or another supported CMake generator.

## Compatibility Goals

- Keep the host app compatible with both Windows and Linux desktop environments.
- Keep Windows MinGW compatibility as a first-class target, not only MSVC compatibility.
- Avoid platform-specific APIs unless guarded with clear `WIN32`, `UNIX`, or Qt abstraction checks.
- Avoid C++ compiler extensions and platform assumptions that are not supported by the targeted Qt MinGW toolchain.
- Prefer Qt cross-platform APIs for networking, filesystem paths, timers, threading, settings, and UI integration.
- Keep deployment behavior compatible with Qt deployment tooling:
  - Windows should remain compatible with Qt deployment output under `dist/windows`.
  - Linux should remain compatible with distribution Qt packages or a later AppImage/package workflow.
- If serial communication is added later, prefer Qt Serial Port and add `SerialPort` explicitly to `find_package(Qt6 REQUIRED COMPONENTS ...)` and `target_link_libraries(...)`.

## ESP32 Host App Direction

- The application is an upper-computer/desktop host for communicating with ESP32 hardware.
- Preserve support for Ethernet/network-based communication already represented by `Qt6::Network`.
- Design communication code so that future transports, such as serial, TCP, UDP, or protocol framing changes, can be isolated from UI code.
- Keep the engineering structure low-coupled: UI, transport, protocol parsing, device state, persistence, and presentation models should remain separated.
- Prefer small QObject/QML-facing adapter classes over exposing transport or protocol internals directly to QML.
- Keep protocol and communication logic testable without launching the QML UI.
- Runtime data should remain stored in a user-writable application data or documents location, not beside the executable.

## UI And UX Direction

- Target a polished, premium, high-quality desktop tool style.
- The UI should feel like a serious engineering/control application: clean hierarchy, refined spacing, restrained color, clear status indication, and high information density where useful.
- Use advanced, smooth, purposeful animation where it improves state changes, telemetry feedback, transitions, and perceived responsiveness.
- Prefer subtle motion design with clear timing and easing over distracting decorative movement.
- Keep animations performant on both Windows and Linux desktop builds, including MinGW builds.
- Avoid toy-like, noisy, or overly decorative UI.
- Prefer Qt Quick/QML components and reusable styling patterns that work consistently on Windows and Linux.
- Ensure layouts scale cleanly across common desktop resolutions and do not rely on platform-specific fonts or pixel-perfect assumptions.
