# History

## 2026-07-04 new_merge motor migration

- 按 `docs/agent/MERGE_EXPERIENCE.md` 流程，将 `lasttime_merge` 电机分拣链路移植到 `new_merge`。
- 按用户要求保留 `new_merge` UI 显示内容，不迁移 `lasttime_merge` 的 UI 差异。
- 在 `new_merge` 内初始化独立 git：
  - `base-before-motor`: `8b53744 checkpoint before motor migration`
  - `motor-two-stage`: `b0a1781 port sorter motor chain to two-stage model`
  - `motor-roi`: `a22a032 add traditional roi motor branch`
- 后续硬件修正：
  - `motor-two-stage`: `5ff571a move gt911 interrupt off motor gpio`
  - `motor-roi`: `a97ebf6 move gt911 interrupt off motor gpio`
  - GT911 touch INT 从 GPIO32 改为 GPIO24，避免与电机 PWM GPIO32 冲突。
- 串口占用修正：
  - `motor-two-stage`: `7a18be7 disable sorter sensors on console uart pins`
  - `motor-roi`: `d6fa369 disable sorter sensors on console uart pins`
  - ESP32-P4 启动日志显示 GPIO37/GPIO38 为 console UART I/O，故将 sorter S1/S3 GPIO 置为 `-1`；S2/S4 保持 GPIO23/GPIO22。
- 验证记录：
  - `motor-two-stage` 在临时 worktree `/tmp/new_merge_two_stage` 执行 `idf.py build` 通过。
  - `motor-roi` 执行 `idf.py build` 通过。
  - `motor-roi` 执行 `idf.py -p /dev/ttyUSB0 -b 921600 flash` 通过，所有写入数据 hash verified。
  - 运行期 monitor 需使用 115200；使用 `-b 921600 monitor` 会乱码。
  - `idf.py -p /dev/ttyUSB0 monitor` 启动日志到 `System initialization done`，S1/S3 显示未配置，S2/S4 正常配置，90 秒窗口未见 panic/reboot。
- 用户删除的 `model/jt30829206.bin`、`model/logo.espdl`、`model/yd29041906.bin`、`model/zt31241902.bin` 保持为未提交工作区删除，未恢复。
- 本轮未运行 TCP 模拟测试，符合用户“不跑 simulated TCP tests”的选择。
- UI 验证：`git diff -- components/UI components/UI/generated components/UI/sdk` 为空。

## 2026-07-04 two-stage package and ROI developer debug UI

- 按用户要求切到 `motor-two-stage` 后创建给队友的压缩包：
  - `/home/kazeform/2026esp/new_merge_motor_two_stage_20260704.tar.gz`
  - 大小 142M。
  - 打包排除 `build`、`.git`、`.codegraph`、`.cache`，保留 `components` 和 `managed_components`。
  - 打包时用户删除的模型文件不在工作树内，因此压缩包不包含这些已删文件。
- 切回 `motor-roi` 后先做 git 备份：
  - `f792882 backup roi before temporary developer debug ui`
  - 该提交记录了用户删除的 `jt30829206.bin`、`logo.espdl`、`yd29041906.bin`、`zt31241902.bin`。
- 新增临时开发者硬件调试页面：
  - 提交：`3e58b04 add temporary developer debug panel`
  - 入口：dashboard 右上方小 `DEV` 按钮。
  - 点击后弹出覆盖层调试页，参考 `lasttime_my` 的硬件调试页面。
  - 功能包含模式切换、电机输出开关、传感器开关、A/B/C 速度调节、安全延时/超时调节、S1-S4 状态、编码器距离、MTEST、CLASS1/2/3/FREE/ERROR 注入、活动包裹列表。
  - 临时改动集中在：
    - `components/UI/sdk/developer_debug_ui.c`
    - `components/UI/sdk/developer_debug_ui.h`
    - `components/UI/sdk/ui.c`
    - `components/UI/CMakeLists.txt`
  - 后续清理时删除 `developer_debug_ui.*`，从 `ui.c` 移除 include 和 `ui_developer_debug_attach()`，从 CMake 移除源文件；若 UI 不再需要 Sorter_app API，也移除 `Sorter_app` 依赖。
- 验证：
  - `idf.py build` 通过。
  - `idf.py -p /dev/ttyUSB0 -b 921600 flash` 通过，app/partition/storage hash verified。
  - `idf.py -p /dev/ttyUSB0 monitor` 使用 115200，启动到 `System initialization done`。
  - 90 秒 monitor 窗口未见新增 panic/reboot。
  - 启动日志出现 `dl::Model: Test output box0 does not match`，但系统继续启动；记录为模型自检现象，不是临时调试 UI 的启动阻塞。

## Archived

- `docs/agent/archive/2026-07-04-merge-ui-dependency-docs.md`
- `docs/agent/archive/2026-07-03-findlogo-merge.md`
