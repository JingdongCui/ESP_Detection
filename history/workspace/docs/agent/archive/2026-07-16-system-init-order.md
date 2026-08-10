# 系统初始化职责与顺序整理

## 目标

将 `main/system_init.c` 开头的调试/功能宏迁回所属模块，并参考归档旧工程整理初始化顺序。

## 修改

- 日志过滤配置迁入 `components/system_monitor/system_monitor.c`，由 `system_monitor_configure_logging()` 应用。
- TCP 链路总开关迁入 `components/Ethernet_app/ethernet_app.c`；关闭时 `ethernet_app_start()` 为安全空操作。
- 分拣硬件调试、默认电机输出、默认传感器输入配置迁入 `components/Sorter_app/sorting_sim_control.c`，统一由 `sorting_sim_control_start()` 启动。
- `System_Init` 只保留模块调用、错误处理和先后编排。

## 初始化顺序

LCD → Touch → Camera → Motor → Encoder → LVGL/UI → System Monitor → Vision → UVC → Ethernet → Sorter。

- Touch 先于 Camera：Camera SCCB 复用 Touch 创建的 I2C 总线。
- Vision 先于 UVC：UVC 使用 Vision 启动后的持续合成画面。
- System Monitor 放回旧工程的 UI 之后、Vision 之前。
- Ethernet 在 Vision/UVC 就绪后启动，Sorter 随后启用。
- 旧工程的 `ethernet_app_early_init()` / `ethernet_app_wait_ready()` 在第 9 包中不存在，本轮未移植或臆造这些接口。

## 验证

- `idf.py build`：成功。
- `sample_project.bin`：`0x512890`，6 MB app 分区剩余约 15%。
- `git diff --check`：通过。
- 活跃工程提交：`62d9bd6 refactor: organize system initialization ownership`。
- 用户要求暂不烧录，因此未执行 flash/monitor。
