# ESP32-P4 Detection

基于 **ESP32-P4** 的视觉检测项目，使用 [ESP-DL](https://github.com/espressif/esp-dl) 推理框架运行 PicoDet 模型实现 Logo 检测，配合 LVGL 提供 UI 显示，支持 RTT 实时日志。

## 硬件要求

- ESP32-P4 开发板（USB-JTAG 内置，单线调试）
- 16 MB Flash
- 摄像头（具体型号见 `agentic/board.md`）
- LCD 屏（用于 LVGL UI 显示）

## 项目结构

```
.
├── main/                              # 应用入口
│   ├── main.c                         # System_Init + system_monitor_init
│   ├── system_init.{c,h}              # 系统初始化
│   ├── system_monitor.{c,h}           # 运行时监控（CPU/内存/cache 统计）
│   ├── SEGGER_RTT*                    # RTT 日志库（替代 UART printf）
│   ├── idf_component.yml              # ESP-IDF 组件依赖（lvgl/esp-dl/esp_cam_sensor 等）
│   └── CMakeLists.txt
│
├── components/
│   ├── bsp/                           # 板级支持包
│   │   ├── bsp_cam_sensor.c           # MIPI-CSI 摄像头 + cam_cap 任务
│   │   ├── bsp_lcd.c                  # LCD 面板初始化
│   │   ├── bsp_touch.c                # GT911 触摸驱动
│   │   ├── bsp_lvgl_adapter_init.c    # LVGL 适配器初始化（task_priority=5）
│   │   ├── include/
│   │   ├── Kconfig                    # bsp 组件 menuconfig
│   │   └── CMakeLists.txt
│   ├── UI/                            # LVGL 界面
│   │   ├── generated/                 #UI 代码
│   │   ├── sdk/                       # LVGL 相关 SDK
│   │   ├── simulator/                 # 仿真配置（lv_conf.h 等）
│   │   └── CMakeLists.txt
│   └── vision_app/                    # 视觉推理逻辑
│       ├── vision_app.cpp             # vision_task 主流程
│       ├── vision_preview.cpp         # 预览渲染
│       ├── app_yolo.cpp               # PicoDet/YOLO 模型封装
│       ├── yolo_decode.cpp            # 后处理解码
│       ├── include/
│       └── CMakeLists.txt
│
├── model/
│   ├── espdet_pico_416_416_logo.espdl # PicoDet 416×416 Logo 检测模型
│   └── flash_model.{sh,bat}           # 模型烧录到 SPIFFS 分区的脚本
│
├── agentic/                           # JTAG 调试工具链（OpenOCD + GDB + RTT）
│   ├── esp_target.py                  # 目标控制（烧录/复位/寄存器/内存）
│   ├── rtt_reader.py                  # SEGGER RTT 日志守护
│   ├── svd_parser.py                  # SVD 寄存器名称解析
│   ├── esp-session-{start,stop}.sh    # OpenOCD 会话管理
│   ├── idf_build.sh                   # idf.py 包装脚本（绕过 Git Bash MSYSTEM）
│   ├── esp_target_config.json         # 工具链配置（芯片/烧录方式/端口）
│   ├── board.md                       # 板级硬件描述（GPIO、I2C/SPI、LCD）
│   ├── chips/                         # 芯片硬件参考（esp32p4.json + .svd）
│   └── SEGGER_RTT*                    # RTT 源文件备份（按需复制到 main/）
│
├── Skills/                            # AI Agent 技能包（Document/superpowers/pua/Find）
├── assets/                            # 静态资源（图标、示例图等）
├── docx/                              # 项目设计文档（PSRAM 分析、配置指南等）
│
├── partitions.csv                     # 自定义分区：6M factory + 5M spiffs(模型存储)
├── sdkconfig.defaults                 # menuconfig 默认值（团队共享基线）
├── sdkconfig                          # menuconfig 实际输出（自动生成，不要手改）
├── dependencies.lock                  # Component Manager 依赖锁定文件
├── CMakeLists.txt                     # 顶层项目 CMakeLists
│
├── esp-agent.sh                       # agentic 工具链部署/配置向导
├── test-full-toolchain.sh             # 工具链自检脚本
├── backtest_run.py                    # 离线回归测试脚本
│
├── README.md                          # 本文件
├── CHANGELOG.md                       # 版本变更记录
├── CLAUDE.md                          # AI Agent 工作规则与项目约定
│
└── build/                             # 编译产物（已在 .gitignore，不入库）
    ├── flasher_args.json              # 烧录偏移地址权威来源
    ├── <project>.bin / .elf
    ├── bootloader/
    └── partition_table/
```

## 分区表

| 分区 | 大小 | 偏移 | 用途 |
| --- | --- | --- | --- |
| nvs | 24K | 0x9000 | 非易失存储 |
| phy_init | 4K | 0xf000 | 射频校准 |
| factory | 6M | 0x10000 | 应用固件 |
| storage | 5M | 0xb00000 | SPIFFS（存放 .espdl 模型） |

## 快速开始

### 1. 环境准备

- 安装 [ESP-IDF ≥ 5.5](https://docs.espressif.com/projects/esp-idf/zh_CN/latest/esp32p4/get-started/index.html)
- 激活环境：`. $IDF_PATH/export.sh`（Windows 推荐用 VSCode ESP-IDF 扩展自带的 Git Bash 终端）

### 2. 编译

```bash
./agentic/idf_build.sh
```

首次全量编译 3-10 分钟，增量编译通常 10-30 秒。

### 3. 烧录模型与固件

烧录模型到 SPIFFS 分区：

```bash
cd model && ./flash_model.sh
```

烧录应用固件：

```bash
python3 agentic/esp_target.py flash-and-run build/ --app-only
```

### 4. 查看日志

启动 RTT 读取器（后台运行）：

```bash
python3 agentic/rtt_reader.py --elf build/<project>.elf \
    --output agentic/.esp-agent/rtt.log --kill-existing
```

实时查看：

```bash
tail -f agentic/.esp-agent/rtt.log
```

## 调试工具链

`agentic/` 目录提供完整的 JTAG 调试套件，详见 [CLAUDE.md](CLAUDE.md)：

- `esp_target.py` — 烧录、复位、寄存器检查、内存读写
- `rtt_reader.py` — SEGGER RTT 实时日志守护
- `esp-session-{start,stop}.sh` — OpenOCD 会话管理
- SVD 感知的寄存器名访问（`read-reg GPIO.OUT` 等）

---

## 版本管理工作流

本项目使用 Git + GitHub 进行版本管理。每次改动后按以下流程同步到远程仓库。

### 前置准备（仅首次）

如果在中国大陆访问 GitHub 较慢，先给 git 配置 Clash 代理（端口按实际情况替换）：

```bash
git config --global http.proxy http://127.0.0.1:7897
git config --global https.proxy http://127.0.0.1:7897
```

取消代理：

```bash
git config --global --unset http.proxy
git config --global --unset https.proxy
```

### 日常三步走

每次改完代码后：

```bash
git add 改动的文件         # 或 git add . 提交全部改动
git commit -m "说明本次改动"
git push
```

查看当前改动状态：

```bash
git status
```

查看历史提交：

```bash
git log --oneline
```

### 提交消息规范（Conventional Commits）

推荐格式：`<类型>: <简短说明>`

| 类型 | 含义 | 示例 |
| --- | --- | --- |
| `feat` | 新功能 | `feat: add GT911 touch screen support` |
| `fix` | 修 bug | `fix: prevent camera buffer overflow` |
| `docs` | 文档改动 | `docs: update board.md pin mapping` |
| `refactor` | 重构（不改功能） | `refactor: extract LCD init into bsp` |
| `perf` | 性能优化 | `perf: enable PSRAM cache prefetch` |
| `chore` | 杂项（构建、依赖） | `chore: bump lvgl to 9.2.3` |

### 重要版本：CHANGELOG + Tag

阶段性里程碑时（修复重要 bug、发布新功能集合），更新 [CHANGELOG.md](CHANGELOG.md) 并打 tag：

```bash
# 1. 编辑 CHANGELOG.md，添加新版本条目
# 2. 提交 CHANGELOG
git add CHANGELOG.md
git commit -m "docs: update CHANGELOG for v0.2.0"
git push

# 3. 打 tag 并推送
git tag -a v0.2.0 -m "Release v0.2.0: 触摸交互 + 多目标检测"
git push origin v0.2.0
```

打 tag 后，GitHub 仓库的 **Releases** 页会自动显示该版本。

### 文件分工

| 文件 | 用途 | 维护频率 |
| --- | --- | --- |
| `git log` | 完整提交历史（自动） | 每次 commit |
| [CHANGELOG.md](CHANGELOG.md) | 给人看的版本变更摘要 | 阶段性更新 |
| Git tag / GitHub Releases | 重要版本快照 | 里程碑时 |
| README.md | 项目门面 | 功能/架构变化时 |

### 需要用户维护的核心文件

| 文件 | 作用 | 何时改 |
| --- | --- | --- |
| `agentic/board.md` | 板级硬件描述（GPIO、I2C/SPI、LCD、摄像头） | 换板子、加新外设、改飞线 |
| `partitions.csv` | Flash 分区表 | 调整模型/应用大小、加 OTA |
| `sdkconfig.defaults` | menuconfig 默认值（团队共享基线） | 启用新外设、改 PSRAM/Flash 模式 |
| `main/idf_component.yml` | ESP-IDF 组件依赖 | 加/升级第三方组件 |
| `CMakeLists.txt`（顶层 + main + components） | 构建脚本 | 加新源文件、改组件结构 |

---

## 多人协作

### 添加协作者（仓库主操作）

1. 打开仓库 → **Settings** → **Collaborators**
2. 点 **Add people**，输入协作者的 GitHub 用户名、邮箱或全名
3. 协作者收到邮件/站内通知，**7 天内** 点 Accept 才生效

个人仓库只有 Write（读写）权限一种，需要更细分级（Triage / Maintain / Admin）请把仓库转到 Organization 下。

### 协作者本地配置（仅首次）

```bash
git clone https://github.com/XiaoSongTongXue0505/ESP32P4_Detection.git
cd ESP32P4_Detection

# 配置自己的用户信息
git config user.name "你的名字"
git config user.email "你的邮箱"

# 国内访问需配代理（参考"前置准备"章节）
```

### 分支协作流程（推荐）

为避免多人直推 main 冲突，按"分支 + PR"工作流：

```bash
# 1. 拉最新 main
git checkout main
git pull

# 2. 开新分支干活（命名规则见下表）
git checkout -b feat/touch-support

# 3. 改代码、提交、推送分支
git add .
git commit -m "feat: add GT911 touch support"
git push -u origin feat/touch-support

# 4. 在 GitHub 网页发起 Pull Request → 仓库主 review → merge
# 5. 合并后清理本地分支
git checkout main
git pull
git branch -d feat/touch-support
```

### 分支命名规则

| 前缀 | 用途 | 示例 |
| --- | --- | --- |
| `feat/` | 新功能 | `feat/touch-support`、`feat/multi-class-detect` |
| `fix/` | 修 bug | `fix/cam-buffer-overflow` |
| `refactor/` | 重构 | `refactor/extract-bsp` |
| `perf/` | 性能优化 | `perf/lvgl-fps` |
| `docs/` | 文档改动 | `docs/update-board-md` |
| `exp/` | 实验性分支 | `exp/quantize-int4` |

### Pull Request 规范

- **PR 标题**遵循 [Conventional Commits](#提交消息规范-conventional-commits)：`feat: add touch support`
- **PR 描述**写清楚：改了什么、为什么改、如何验证（最好附 RTT 日志或截图）
- **小步快跑**：单个 PR 控制在 ~300 行内，方便 review
- **避免直接推 main**：仓库主可在 Settings → Branches 里给 main 加保护规则（要求 PR、要求 review）

### 处理冲突

`git pull` 时若提示冲突：

```bash
# 1. 编辑冲突文件，找 <<<<<<< HEAD ... ======= ... >>>>>>> 标记，手动解决
# 2. 标记已解决
git add 冲突文件
# 3. 完成合并
git commit
git push
```

**绝不要用 `git push --force` 强推到 main**，会覆盖别人的提交。仅在自己的临时分支上才可强推。

---

## License

待补充。
