# 基于 JTAG 的 Agentic 固件开发

本项目使用 JTAG 进行调试、日志捕获和寄存器检查。

烧录方式由 `esp_target_config.json` 中的设置决定

（`flash.method`：通过 OpenOCD `program_esp` 的 `jtag` 方式，或通过

`esptool.py` 的 `serial` 方式）。

## 配置

两个配置文件控制工具链：

**`agentic/esp_target_config.json`** — 项目级工具配置：

- 使用哪个芯片硬件描述
- OpenOCD 设置（板级配置、端口、烧录命令）
- 烧录方式（`jtag` 或 `serial`）以及适用时的串口参数
- 工具链前缀（用于推导 nm、objdump、addr2line 和默认 GDB）
- GDB 可执行文件（未指定时默认为 `{prefix}gdb`）
- 日志方式（rtt 或 apptrace）

**`agentic/chips/<chip>.json`** — 纯硬件参考（内存映射、架构）。

由 `esp_target_config.json` 引用。无需按项目编辑。

芯片 JSON 文件可能包含对应 SVD 文件的引用，其中定义了外设寄存器。

**`agentic/board.md`** — 描述具体的开发板：GPIO 引脚分配、I2C/SPI

总线连接、LED（类型、引脚、协议）、按钮、传感器、电源域，以及与

固件开发相关的其他硬件上下文。在编写任何与板载外设交互的代码之前

请先阅读此文件。

查看完整的解析后配置：

```
python3 agentic/esp_target.py info
```

查看芯片内存映射：

```
python3 agentic/esp_target.py memmap
```

所有工具位于 `agentic/` 子目录中。`esp_target.py` 和 `rtt_reader.py`

会自动从其所在目录读取 `esp_target_config.json`。

## 项目结构

```
project/
├── CLAUDE.md                     # 本文件
├── agentic/                      # agentic 工具（所有工具位于此处）
│   ├── esp_target_config.json    # 工具配置（OpenOCD、GDB、日志、烧录方式）
│   ├── board.md                  # 板级硬件描述（引脚、LED、总线）
│   ├── esp_target.py             # 目标控制工具
│   ├── svd_parser.py             # SVD 解析器（由 esp_target.py 使用）
│   ├── rtt_reader.py             # RTT 日志读取守护进程
│   ├── esp-session-start.sh      # 启动 OpenOCD
│   ├── esp-session-stop.sh       # 停止守护进程
│   ├── idf_build.sh              # idf.py 包装脚本（在 Git Bash 上绕过 MSYSTEM）
│   ├── chips/
│   │   ├── <chip>.json           # 硬件参考（内存映射、架构）
│   │   └── <chip>.svd            # 外设寄存器定义（可选）
│   └── .esp-agent/               # 运行时状态（会话启动时创建）
│       ├── openocd.log           # OpenOCD 守护进程日志
│       ├── rtt.log               # 固件 RTT 输出
│       └── rtt_reader.log        # rtt_reader 标准错误输出（可选）
├── main/
│   ├── CMakeLists.txt            # 组件注册（idf_component_register）
│   ├── *.c / *.h                 # 应用源代码
│   ├── SEGGER_RTT.c              # RTT 库（目标端）
│   ├── SEGGER_RTT.h
│   ├── SEGGER_RTT_Conf.h         # RTT 配置（架构相关的锁宏）
│   └── SEGGER_RTT_printf.c
├── CMakeLists.txt                # 顶层项目 CMakeLists
├── sdkconfig                     # ESP-IDF menuconfig 输出
└── build/                        # 构建输出（生成的）
    ├── flasher_args.json         # 烧录布局：哪个二进制文件在哪个偏移地址
    ├── <project>.bin             # 应用二进制文件
    ├── <project>.elf             # 带调试符号的 ELF 文件
    ├── bootloader/
    │   └── bootloader.bin
    └── partition_table/
        └── partition-table.bin
```

## 架构

```
Coding Agent
  ├── idf.py build                    → 编译固件
  ├── esp_target.py (shell exec)      → 烧录、复位、检查寄存器
  ├── GDB batch scripts (on-demand)   → 符号感知调试
  ├── reads board.md                  → 板级描述
  ├── reads .esp-agent/rtt.log        → 固件日志输出
  └── reads .esp-agent/openocd.log    → 基础设施诊断

esp_target.py
  └── OpenOCD Tcl port                → mww/mdw, program_esp, halt/resume
      （或在 flash.method = serial 时委托给 esptool）

rtt_reader.py（后台守护进程）
  └── OpenOCD Tcl port                → 通过 mdw/mww 轮询 RTT 环形缓冲区

OpenOCD（持久守护进程）
  ├── Tcl port     — 来自 esp_target.py 和 rtt_reader.py 的命令
  ├── GDB RSP port — 按需符号感知调试
  └── USB-JTAG     → 目标 MCU
```

esp_target.py 和 rtt_reader.py 同时连接到 OpenOCD 的 Tcl 端口。

OpenOCD 在内部序列化 JTAG 事务。端口号在 `esp_target_config.json` 中定义。

## 前提条件

### 硬件

- 一块支持的 ESP32 开发板（C3 / C6 / H2 / S3 / P4）
- **一根 USB 数据线**连接开发板的 USB-JTAG 口

无需额外的 JTAG 适配器、UART 转接板或飞线。上述芯片均内置

USB-Serial-JTAG 控制器，一根 USB 线同时提供 JTAG 调试通道（OpenOCD

用于所有调试和监视功能）和串口烧录通道（esptool 用于串口模式烧录）。

RTT 日志功能额外要求固件中包含 SEGGER RTT 代码（纯软件依赖，无硬件要求）。

### 软件

ESP-IDF 环境必须在 shell 中处于激活状态 — `idf.py`、`openocd`

和交叉编译器必须在 PATH 中。如果任何命令报 "command not found" 错误，

请告知用户在启动会话的 shell 中运行 `. $IDF_PATH/export.sh`，然后重启。

**在 Windows 上**，打开 VSCode ESP-IDF 扩展的内置终端（Git Bash）。

`$IDF_PATH` 和 `IDF_PYTHON_ENV_PATH` 已由扩展设置 — 无需手动

`export.sh`。

假定 OpenOCD 已在运行并连接到目标。通过以下命令验证连接：

```
python3 agentic/esp_target.py health
```

如果 OpenOCD 无响应，重启会话：

```
./agentic/esp-session-stop.sh
./agentic/esp-session-start.sh
```

## 命令执行规则

**绝不在命令前加 `cd <project_dir> &&` 前缀。** 工作目录已经是项目根目录，

所有命令直接执行即可。`cd` 前缀会导致权限匹配失败。

正确：

```
python3 agentic/esp_target.py flash-and-run build/ --app-only
./agentic/idf_build.sh
python3 agentic/rtt_reader.py --elf build/app.elf --output agentic/.esp-agent/rtt.log --kill-existing
```

错误：

```
cd "/path/to/project" && python3 agentic/esp_target.py flash build/
```

同样，不要在命令末尾追加 `2>&1; echo "EXIT:$?"` 等 shell 修饰符 —

直接执行命令，工具会自动捕获输出和退出码。

### 工具完整性检查

如果执行 `agentic/` 下的任何工具时发现文件不存在（如 `rtt_reader.py`、

`esp_target.py`、`esp-session-start.sh` 等），说明工具部署不完整。

**自动修复**：在项目根目录重新运行配置向导：

```
bash esp-agent.sh
```

如果 `esp-agent.sh` 也不存在，告知用户从 esp32-agentic 仓库重新部署：

```
bash /path/to/esp32-agentic/esp-agent.sh
```

绝不尝试手动重写缺失的工具脚本——它们包含复杂的平台适配逻辑，

必须从仓库源复制。

**`esp-session-start.sh`** — 读取 `esp_target_config.json`，终止任何

残留的 OpenOCD 进程，启动新的 OpenOCD 守护进程（日志 →

`.esp-agent/openocd.log`），等待 Tcl 端口就绪，然后通过

`esp_target.py health` 验证目标响应。如果 OpenOCD 启动失败则退出并报错。

**`esp-session-stop.sh`** — 终止 OpenOCD（通过 `.esp-agent/openocd.pid`

中的 PID 和进程名），停止任何运行中的 `rtt_reader.py`，并保留

`.esp-agent/` 中的日志。

RTT 日志需要单独启动，在构建并烧录了支持 RTT 的固件之后：

```
python3 agentic/rtt_reader.py --elf build/<project>.elf --output agentic/.esp-agent/rtt.log --kill-existing
```

## 构建

```
./agentic/idf_build.sh
```

在 Git Bash / MSYS2 上，此包装脚本会在调用 `idf.py` 前从环境中移除

`$MSYSTEM`（ESP-IDF ≥ 5.5 否则拒绝运行）。在 Linux 和 macOS 上，

其行为与 `idf.py` 完全相同。

**重要：编译命令必须设置 10 分钟超时**（`timeout: 600000`）。
首次全量编译需要 3-10 分钟，增量编译通常 10-30 秒。

构建系统（Ninja）自动判断增量/全量：
- `build/` 不存在 → 全量编译
- 源码改动 → 增量编译（只编译改动的文件）
- `sdkconfig` 或 `CMakeLists.txt` 改动 → 重新 configure + 部分重编译
- 编译器版本/工具链切换后报错 → `rm -rf build` 清理后重新全量编译

其他 idf.py 子命令同样可用：

```
./agentic/idf_build.sh menuconfig
./agentic/idf_build.sh clean
./agentic/idf_build.sh size
```

解析编译器输出中的错误和警告。所有构建产物位于 `build/` 中。

关键输出为：

- `build/flasher_args.json` — 烧录偏移地址的权威来源
- `build/<project>.bin` — 应用二进制文件
- `build/<project>.elf` — 带调试符号的 ELF 文件（GDB 和 RTT 需要）

## ESP-IDF 参考

ESP-IDF 框架源码树位于 `$IDF_PATH`。可查阅其中的 API 用法、

外设驱动模式和工作示例。

```
$IDF_PATH/
├── examples/                     # 每个功能的工作示例
├── components/                   # 框架源代码
└── tools/
    └── esp_app_trace/            # 主机端 apptrace 解码器
```

实现外设驱动或功能时：

1. 在 `$IDF_PATH/examples/` 中查找工作参考
2. 阅读 `$IDF_PATH/components/<component>/include/` 中的组件头文件
3. 检查 `$IDF_PATH/components/<component>/Kconfig` 中的 menuconfig 选项

### 绝不猜测芯片特定的硬件常量

芯片特定的值 — GPIO 矩阵信号索引、寄存器位域位置、外设基地址 —

是任意的硬件分配，在不同芯片系列之间各不相同。它们不能从第一性原理

推导，绝不能猜测。

始终在以下位置查找：

```
$IDF_PATH/components/soc/<chip>/include/soc/
  gpio_sig_map.h      — GPIO 矩阵信号索引
  io_mux_reg.h        — IO_MUX 寄存器位定义
  <periph>_struct.h   — 外设寄存器布局

$IDF_PATH/components/hal/<chip>/include/hal/
  <periph>_ll.h       — 底层驱动常量
```

## 烧录

烧录所有组件（bootloader + 分区表 + 应用）：

```
python3 agentic/esp_target.py flash build/
```

仅烧录应用（更快，适用于迭代开发）：

```
python3 agentic/esp_target.py flash build/ --app-only
```

烧录、复位并运行一步完成：

```
python3 agentic/esp_target.py flash-and-run build/ --app-only
```

烧录方式由 `esp_target_config.json` 中的 `flash.method` 设置（配置向导时选择）：

- `"jtag"` — 使用 OpenOCD `program_esp`。OpenOCD 保持运行，不中断调试会话。
  烧录后工具自动执行 `reset run` 启动固件（USB-JTAG 芯片上可能触发
  USB 断开恢复，耗时较长；如遇问题改用串口模式）。
- `"serial"` — 使用 `esptool.py` 通过 USB CDC。完整生命周期如下。

### 串口烧录执行路径（硬性流程）

串口模式下 `flash` / `flash-and-run` / `erase` 的内部执行顺序：

```
1. 停止 OpenOCD（PowerShell Stop-Process / pkill）
2. 等待 1 秒（进程清理）
3. esptool.py --before=default_reset --after=hard_reset write_flash
   → 芯片进入下载模式 → 写入 flash → 硬复位 → 固件从 flash 启动
   → USB-JTAG 控制器随芯片复位断电重启
4. 轮询 USB 设备（VID 0x303A）出现（最长 15 秒，通常 3-6 秒）
5. 等待 3 秒（JTAG 接口比串口晚就绪）
6. 启动 OpenOCD（Python DETACHED_PROCESS，独立于父 shell）
7. 连接 OpenOCD Tcl 端口
8. resume（确保 CPU 在运行，OpenOCD 连接时可能 halt 了 CPU）
```

**关键约束**：

- **绝不在串口烧录后调用 `reset run`** — 芯片已被 esptool 硬复位，
  再次复位会导致 USB-JTAG 断开，OpenOCD 无法恢复。
- **绝不使用 `--after=no_reset`** — 芯片会留在 bootloader 中，
  OpenOCD 连接后调用 `reset run` 同样会断开 USB-JTAG。
- 正确做法是让 esptool 完成硬复位（固件自动启动），等 USB 重新枚举后
  再启动 OpenOCD "观察"已运行的固件。

### 串口烧录后的 OpenOCD 恢复

通过 `esp_target.py flash` / `flash-and-run` / `erase` 进行串口操作时，

工具会自动执行上述流程，无需手动干预。

但如果用户手动运行了 `esptool.py`、`idf.py flash`，或通过 IDE 按钮烧录，

OpenOCD 会被中断且不会自动恢复。此时后续的 `esp_target.py` 命令会报

连接错误。**遇到 OpenOCD 连接失败时，执行：**

```
./agentic/esp-session-start.sh
```

然后重新验证连接：

```
python3 agentic/esp_target.py health
```

绝不硬编码烧录偏移地址 — 它们来自 `build/flasher_args.json`。

烧录新固件后，重启 RTT 读取器（控制块地址可能已改变）：

```
python3 agentic/rtt_reader.py --elf build/<project>.elf --output agentic/.esp-agent/rtt.log --kill-existing
```

## 目标控制

所有命令通过 `agentic/esp_target.py` 执行。多核开发板的核心数由工具

运行时从 OpenOCD 自动发现，`halt`/`resume`/`state`/`cpu-regs`

会自动操作并显示所有核心。

```
# 复位
python3 agentic/esp_target.py reset run

# 检查执行状态（双核时显示每个核心的状态）
python3 agentic/esp_target.py state

# 暂停 / 恢复（双核时操作所有核心）
python3 agentic/esp_target.py halt
python3 agentic/esp_target.py wait-halt
python3 agentic/esp_target.py resume

# 擦除整个 flash
python3 agentic/esp_target.py erase

# 读取内存（CPU 运行时可用）
python3 agentic/esp_target.py read <addr> <count>
python3 agentic/esp_target.py read <addr> <count> --width 8

# 写入内存
python3 agentic/esp_target.py write <addr> <value> [<value> ...]
python3 agentic/esp_target.py write <addr> <value> --width 8

# 转储所有 CPU 寄存器（必须先暂停；多核开发板自动显示所有核心）
python3 agentic/esp_target.py halt
python3 agentic/esp_target.py cpu-regs
python3 agentic/esp_target.py resume

# 读取 / 写入单个 CPU 寄存器
python3 agentic/esp_target.py cpu-reg pc
python3 agentic/esp_target.py cpu-reg-write a0 0x1234

# 原始 OpenOCD 命令
python3 agentic/esp_target.py raw "targets"
```

OpenOCD 的 `reg <name> <value>` 写入响应在不同寄存器间不统一。

`pc` 在写入时可能回显先前的值。如果写入后的 `pc` 值很重要，

请随后执行显式的 `cpu-reg pc` 读取。

查看有效的 SRAM 和外设地址，请检查内存映射：

```
python3 agentic/esp_target.py memmap
```

访问 SRAM 时使用数据总线地址，而非指令总线别名。

## SVD 感知的寄存器检查

配置了 SVD 文件后，可以通过名称访问寄存器：

```
python3 agentic/esp_target.py list-periph
python3 agentic/esp_target.py list-regs GPIO
python3 agentic/esp_target.py read-reg GPIO.OUT
python3 agentic/esp_target.py decode GPIO.OUT
python3 agentic/esp_target.py inspect UART0
python3 agentic/esp_target.py write-reg GPIO.OUT_W1TS 0x400
```

寄存器路径表示法为 `PERIPHERAL.REGISTER` 或

`PERIPHERAL.REGISTER.FIELD`。`list-periph`、`list-regs` 和 `memmap`

可离线工作，无需 OpenOCD。

## GDB 调试

GDB 连接到 OpenOCD 的 GDB RSP 端口。通过以下命令查找可执行文件和端口：

```
python3 agentic/esp_target.py info
```

### 连接前提

GDB 连接前**必须先暂停 CPU**，否则连接可能被拒绝或立即断开：

```
python3 agentic/esp_target.py halt
<gdb_executable> -batch \
    -ex "set remotetimeout 10" \
    -ex "target remote :<gdb_port>" \
    -ex "<your commands>" \
    build/<project>.elf
python3 agentic/esp_target.py resume
```

**多核芯片（P4/S3）注意**：OpenOCD 为每个核心分配独立的 GDB 端口。

默认端口（通常 3333）连接 cpu0。如果连接被拒绝，先 halt 所有核心再重试。

### 批处理模式（agentic 使用的首选方式）

```
python3 agentic/esp_target.py halt
<gdb_executable> -batch \
    -ex "set remotetimeout 10" \
    -ex "target remote :<gdb_port>" \
    -ex "bt" \
    build/<project>.elf
python3 agentic/esp_target.py resume
```

### 暂停/恢复协议

**GDB 连接时会暂停 CPU。** 当 GDB 批处理会话退出时，CPU 保持暂停状态

— RTT 输出停止。之后务必恢复：

```
python3 agentic/esp_target.py resume
```

如果 GDB 之后 RTT 输出静默，检查

`python3 agentic/esp_target.py state` 并在暂停时恢复。

GDB 连接期间，不要使用 esp_target.py 的 halt/resume/reset 命令。

内存读取和 RTT 轮询可以与 GDB 安全并发。

## RTT 日志捕获

固件必须包含 SEGGER RTT 并写入通道 0：

```c
#include "SEGGER_RTT.h"
SEGGER_RTT_WriteString(0, "Hello from RTT\n");
SEGGER_RTT_printf(0, "value = %d\n", some_value);
```

### 验证 RTT 控制块

烧录后先确认 RTT 控制块存在：

```
python3 agentic/rtt_reader.py --elf build/<project>.elf --scan-only
```

如果输出 `RTT control block initialized` 和通道信息，说明固件 RTT 正常。

### 启动 RTT 读取器

**推荐方式**（跨平台可靠）：以后台任务运行，不加 `--daemonize`：

```
python3 agentic/rtt_reader.py --elf build/<project>.elf --output agentic/.esp-agent/rtt.log --kill-existing
```

此命令会持续运行并将 RTT 输出写入日志文件。在 Claude Code 中以后台方式

执行（`run_in_background`），然后读取日志文件获取输出。

### 读取 RTT 日志

等待 2-3 秒后读取：

```
cat agentic/.esp-agent/rtt.log
tail -20 agentic/.esp-agent/rtt.log
```

### 定位 RTT 控制块的选项

1. `--elf build/<project>.elf` — 默认方式，即时，始终正确
2. `--address <addr>` — 已知地址，即时
3. （无标志）— 扫描 SRAM，较慢；最后手段

### RTT 恢复

如果 RTT 产生乱码或停止接收数据：

1. 终止 rtt_reader.py 进程
2. 重新烧录：`python3 agentic/esp_target.py flash-and-run build/ --app-only`
3. 使用 `--elf` 重启读取器以获取新的控制块地址

## 访问日志

- **固件输出** — `agentic/.esp-agent/rtt.log`
- **OpenOCD 日志** — `agentic/.esp-agent/openocd.log`

## 重要约束

- 内存读取（`mdw`）在 CPU 运行时可用；CPU 寄存器读取需要先暂停
- GDB 会话活跃时不要通过 esp_target.py 暂停 CPU
- **`reset run` 会导致 USB-JTAG 断开**（所有内置 USB-JTAG 的芯片）。
  工具会尝试自动恢复（轮询 USB 枚举 + 重启 OpenOCD），但恢复不一定成功
  （USB 枚举时间不可预测）。**避免使用 `reset run`**，改用：
  - 需要重新运行固件 → `flash-and-run build/ --app-only`（可靠）
  - 需要暂停/恢复 → `halt` + `resume`（不断开 USB）
  - 必须复位时 → 使用后手动 `./agentic/esp-session-start.sh` 恢复
- RTT 控制块地址在固件以不同静态变量布局重新构建时会改变 —
  重新烧录后务必重启 rtt_reader.py
- 烧录偏移地址取决于芯片和项目 — 始终从 `build/flasher_args.json` 读取，
  绝不硬编码
- 查阅 `python3 agentic/esp_target.py memmap` 获取内存地址 —
  不要假设一个芯片的地址范围适用于另一个芯片
- **OpenOCD 连接失败时**（`Cannot connect to OpenOCD`、`Socket error`），
  先运行 `./agentic/esp-session-start.sh` 恢复会话，再重试操作。
  常见原因：用户手动烧录、USB 断开重连、OpenOCD 崩溃

## 典型开发周期

首次接手项目时，先了解配置：

```
python3 agentic/esp_target.py info
```

这会显示芯片架构、烧录方式、GDB 端口、工具链前缀等关键信息。

迭代开发流程：

1. 编辑 `main/` 中的源代码
2. `./agentic/idf_build.sh` — 修复任何编译错误
3. `python3 agentic/esp_target.py flash-and-run build/ --app-only`
4. 验证 RTT 控制块：`python3 agentic/rtt_reader.py --elf build/<project>.elf --scan-only`
5. 启动 RTT 读取器（后台运行）：
   `python3 agentic/rtt_reader.py --elf build/<project>.elf --output agentic/.esp-agent/rtt.log --kill-existing`
6. 等待 2-3 秒，读取 `agentic/.esp-agent/rtt.log`
7. 如果有问题，检查硬件状态：
   - `decode` / `inspect` 外设寄存器
   - 崩溃后 `halt` + `cpu-regs`
   - GDB 批处理进行符号感知检查
8. 诊断、编辑、重复

## 调试崩溃或挂起

1. 检查 `agentic/.esp-agent/rtt.log` 中的 panic 回溯
2. `python3 agentic/esp_target.py halt`
3. `python3 agentic/esp_target.py cpu-regs` — 检查 `pc`
4. 检查异常原因寄存器（架构相关）：
   - RISC-V（C3/C6/H2/P4）：`python3 agentic/esp_target.py cpu-reg mcause`
   - Xtensa（S3）：`python3 agentic/esp_target.py cpu-reg exccause`
5. 使用 GDB 进行符号感知诊断（步骤 2 已 halt，直接连接）：
   ```
   <gdb_executable> -batch \
       -ex "set remotetimeout 10" \
       -ex "target remote :<gdb_port>" \
       -ex "bt full" \
       -ex "info registers" \
       -ex "info threads" \
       build/<project>.elf
   ```
6. 检查外设状态：`decode <PERIPH>.<REG>`
7. 读取栈指针附近的内存
8. `python3 agentic/esp_target.py resume`

## OpenOCD Tcl 接口

`raw` 命令暴露完整的 OpenOCD 命令词汇：

```
python3 agentic/esp_target.py raw "targets"
python3 agentic/esp_target.py raw "flash info 0"
```

Tcl 接口是完整的 Tcl 解释器。需要重复查询硬件状态时（采样 GPIO、

探测多个寄存器），用 Tcl 脚本比多次调用 `read` 快得多：

```bash
python3 agentic/esp_target.py raw "set val [mdw 0x60004000 1]; return \$val"
```

可以使用循环、`after` 延迟，并在返回前聚合输出。调用者只能看到

脚本的返回值，不要在 Tcl 脚本中 print 到 stdout。

## ESP-IDF apptrace（替代日志方式）

apptrace 可将所有 `ESP_LOGx` 输出通过 JTAG 重定向。与 RTT 互补——

RTT 用于持续的 agentic 日志，apptrace 用于诊断捕获会话。

### 启用 apptrace

1. `idf.py menuconfig` →
   `Component config → Application Level Tracing → Data Destination 1` → `JTAG`
2. 固件中：
   ```c
   #include "esp_app_trace.h"
   esp_log_set_vprintf(esp_apptrace_vprintf);
   ```
3. 低速率日志需显式刷新：
   ```c
   esp_apptrace_flush(ESP_APPTRACE_DEST_JTAG, 1000);
   ```

### 捕获和解码

```
python3 agentic/esp_target.py raw "reset run"
# 等待 2 秒
python3 agentic/esp_target.py raw "esp apptrace start file:///tmp/apptrace.log 1 -1 30 0"
```

解码：

```
python3 $IDF_PATH/tools/esp_app_trace/logtrace_proc.py /tmp/apptrace.log build/<project>.elf
```

### 关键限制

`esp apptrace start` **阻塞 OpenOCD 事件循环**。捕获期间

esp_target.py 和 rtt_reader.py 无法通信。仅作诊断捕获会话使用，

不适合持续开发循环。

|  | RTT | apptrace |
| --- | --- | --- |
| 日志来源 | 显式 SEGGER_RTT_printf() | 所有 ESP_LOGx 自动 |
| 输出格式 | 纯文本，即时 | 二进制，需解码 |
| 持续流式 | 是 | 否（定时窗口） |
| 阻塞 OpenOCD | 否 | 是 |
| 最适合 | agentic 开发循环 | 深度 ESP-IDF 诊断 |

## 为新固件添加 RTT

当用户要求查看固件日志、监控输出、或调试运行时行为时，**先检查项目是否

已集成 RTT**。检查方法：查看 `main/` 目录下是否存在 `SEGGER_RTT.h`。

如果 RTT 未集成，**自动执行以下步骤**（无需询问用户）：

RTT 源文件已内置在 `agentic/` 目录中，直接复制即可：

```bash
cp agentic/SEGGER_RTT.c agentic/SEGGER_RTT.h agentic/SEGGER_RTT_printf.c agentic/SEGGER_RTT_Conf.h main/
```

然后：

1. 在 `main/CMakeLists.txt` 的 `idf_component_register` 中添加 RTT 源文件：
   ```cmake
   idf_component_register(SRCS "SEGGER_RTT.c" "SEGGER_RTT_printf.c" "app_main.c"
                          PRIV_REQUIRES spi_flash
                          INCLUDE_DIRS ".")
   ```
2. 在 `app_main()` 入口添加 RTT 初始化输出：
   ```c
   #include "SEGGER_RTT.h"
   SEGGER_RTT_WriteString(0, "Boot complete\n");
   ```
3. 在需要监控的位置添加 `SEGGER_RTT_printf(0, ...)` 输出
4. 编译、烧录、启动 RTT 读取器

`SEGGER_RTT_Conf.h` 包含架构相关的中断锁宏：

- RISC-V（C3/C6/H2/P4）：`csrrci`/`csrw` 操作 `mstatus` MIE 位
- Xtensa（S3）：`rsil`/`wsr.ps` 设置中断级别
- 编译器自动选择，无需手动配置
   ```



 Project Skills Configuration

本项目配置了四个 skill 包，遇到对应场景时**必须主动读取对应 SKILL.md 并严格按其指令执行**。

---

## Skill 1 — Document-skills（文档处理）

**SKILL.md 路径**：

- Word 文档：`Skills/Document-skills/skills/docx/SKILL.md`
- PDF 文件：`Skills/Document-skills/skills/pdf/SKILL.md`
- Excel 表格：`Skills/Document-skills/skills/xlsx/SKILL.md`
- PPT 演示：`Skills/Document-skills/skills/pptx/SKILL.md`

**触发条件（满足任意一条立即读取对应 SKILL.md）**：

- **docx**：用户提到 Word 文档、.docx、报告、备忘录、信件、模板，或要求生成带格式的专业文档（目录、页眉、页码、信头）
- **pdf**：用户提到 .pdf 文件，或要求提取/合并/拆分/旋转/加水印/填写表单/OCR/加密 PDF
- **xlsx**：用户提到 .xlsx、.xlsm、.csv、.tsv、电子表格，或要求创建/编辑/分析表格数据、公式、图表、财务模型
- **pptx**：用户提到 .pptx、演示文稿、幻灯片、deck、pitch deck，或要求创建/编辑/提取 PPT 内容

**不触发**：用户只是讨论文档内容而非操作文件本身。

---

## Skill 2 — superpowers-skills（开发工作流）

**SKILL.md 根路径**：`Skills/superpowers-skills/skills/`

**触发条件（满足任意一条立即读取对应 SKILL.md）**：

| 场景 | 读取的 SKILL.md |
| --- | --- |
| 用户要求新增功能、创建组件、修改行为，**任何创意/实现工作开始前** | `brainstorming/SKILL.md` |
| 遇到 bug、测试失败、意外行为，**提出修复方案之前** | `systematic-debugging/SKILL.md` |
| 即将声称任务完成、已修复、已通过，**提交或汇报结果之前** | `verification-before-completion/SKILL.md` |
| 用户要求编写测试、TDD 开发 | `test-driven-development/SKILL.md` |
| 设计已确认，需要制定实施计划 | `writing-plans/SKILL.md` |
| 有实施计划，开始逐步执行 | `executing-plans/SKILL.md` |
| 需要发起代码审查 | `requesting-code-review/SKILL.md` |
| 收到代码审查意见需要处理 | `receiving-code-review/SKILL.md` |
| 需要并行派发多个子 Agent | `dispatching-parallel-agents/SKILL.md` |
| 分支开发完成，准备合并 | `finishing-a-development-branch/SKILL.md` |
| 使用 Git worktree 工作流 | `using-git-worktrees/SKILL.md` |

**重要**：`brainstorming` 是硬性要求——任何新功能开发前必须先走 brainstorming，未经设计确认不得写代码。

---

## Skill 3 — pua-skills（高绩效激励）

**SKILL.md 路径**：`Skills/pua-skills/skills/pua/SKILL.md`

**触发条件（满足任意一条立即读取 SKILL.md）**：

- 用户明确输入 `/pua` 或说"PUA 模式"
- 用户表达挫败或施压：`加油`、`别偷懒`、`你再试试`、`为什么还不行`、`你怎么又失败了`、`又错了`、`质量太差`、`换个方法`、`try harder`、`figure it out`、`stop giving up`、`you keep failing`、`you broke it`、`stop spinning`
- 同一问题连续失败 **2 次及以上**
- 用户抱怨输出质量差、敷衍、不够努力

**不触发**：正常的首次编码请求或信息查询。

---

## Skill 4 — Find-skills（技能发现）

**SKILL.md 路径**：`Skills/Find-skills/skills/find-skills/SKILL.md`

**触发条件（满足任意一条立即读取 SKILL.md）**：

- 用户问"有没有能做 X 的 skill？"
- 用户问"怎么实现 X？"且 X 可能有现成 skill
- 用户说"找一个 skill for X"、"is there a skill that can..."
- 用户想扩展 Claude 的能力或搜索工具/模板/工作流

---

## 执行规则

1. 触发条件匹配后，**先用 Read 工具读取对应 SKILL.md**，再按其中指令执行，不得跳过。
2. 多个 skill 同时匹配时，按优先级：`brainstorming` > `systematic-debugging` > `verification-before-completion` > 其他。
3. SKILL.md 中的指令**优先级高于**默认行为。

---

## 乐鑫技术资料查询

遇到 ESP32 系列芯片（ESP32-P4、ESP32-S3、ESP32-C3 等）的不明确技术问题时，
**必须**访问乐鑫官方 MCP 服务获取权威技术资料：

https://mcp.espressif.com/docs

适用场景：
- 寄存器含义不确定（cache 计数器、DMA 仲裁、外设配置）
- ESP-IDF API 行为不明确
- 芯片硬件特性（cache 层级、总线架构、DMA 通道）需要确认
- 外设驱动配置参数的正确值

**绝不猜测硬件行为或寄存器语义**，必须查阅技术资料确认。
