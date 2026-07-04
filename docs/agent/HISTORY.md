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

## Archived

- `docs/agent/archive/2026-07-04-merge-ui-dependency-docs.md`
- `docs/agent/archive/2026-07-03-findlogo-merge.md`
