# ESP32-P4-Function-EV-Board

ESP32-P4 芯片版本 v3.1 的多媒体开发板。

## 芯片

- **MCU**: ESP32-P4（双核 RISC-V，最大 400 MHz）
- **芯片版本**: v3.1（注意：不支持安全下载模式，勿启用）
- **片上 Flash**: 无（外挂 SPI Flash）
- **外挂 SPI Flash**: 16 MB
- **外挂 PSRAM**: 最大 32 MB

## 协处理器 / 无线模组

- **ESP32-C6-MINI-1**: 负责 Wi-Fi 6 (2.4 GHz) 和 Bluetooth 5 (LE) 通信，通过独立接口与 P4 连接

## I2C 总线

| 信号 | GPIO | 连接的设备 |
| --- | --- | --- |
| SCL | GPIO8 | ES8311 音频编解码器、LCD 触摸控制器 (GT911) |
| SDA | GPIO7 | 同上 |

## I2S 音频接口

| 信号 | GPIO | 说明 |
| --- | --- | --- |
| MCLK | GPIO13 | 主时钟 |
| SCLK (BCLK) | GPIO12 | 位时钟 |
| LCLK (WS) | GPIO10 | 字选择 |
| DOUT | GPIO9 | 数据输出（播放） |
| DIN | GPIO11 | 数据输入（录音） |
| PA_EN | 未使用 | GPIO53 已用于分拣 S1 传感器 |

- **编解码芯片**: ES8311（单声道，I2S + I2C 接口）
- **功率放大器**: NS4150（3 W，D 类），当前融合分拣硬件时不使用 PA_EN
- **扬声器输出**: 2.00 mm 间距端口，最大驱动 4 Ω 3 W 扬声器
- **麦克风**: 板载单麦克风，连接至 ES8311

## 显示屏（MIPI DSI）

| 信号 | GPIO | 说明 |
| --- | --- | --- |
| LCD 背光 PWM | GPIO26 | 1024×600 屏幕 |
| LCD RST | GPIO27 | 1024×600 屏幕 |

- **接口**: MIPI DSI（FPC 15 pin，间距 1.0 mm）
- **屏幕**: 7 英寸电容触摸屏，分辨率 1024 × 600
- **触摸控制器**: GT911（I2C 总线共享，SCL=GPIO8 / SDA=GPIO7）
  - **触摸 RST**: GPIO33
  - **触摸 INT**: GPIO21（原为 GPIO32，与 Motor1/B段 PWM_A 冲突，已改到 J1 空闲脚 GPIO21）
- **LCD 驱动**: EK79007 

## 摄像头（MIPI CSI）

- **接口**: MIPI CSI（FPC 15 pin，间距 1.0 mm）
- **支持**: OV5647 / SC2336
- **规格**: 200-500 万像素

## SD 卡（SDMMC 4-bit）

| 信号 | GPIO |
| --- | --- |
| D0 | GPIO39 |
| D1 | GPIO40 |
| D2 | GPIO41 |
| D3 | GPIO42 |
| CMD | GPIO44 |
| CLK | GPIO43 |

## USB

| 信号 | GPIO | 说明 |
| --- | --- | --- |
| USB_D+ | GPIO20 | USB 2.0 OTG HS |
| USB_D- | GPIO19 | USB 2.0 OTG HS |

### USB 接口汇总

| 接口 | 类型 | 用途 |
| --- | --- | --- |
| USB Serial/JTAG | Type-C | 烧录 + JTAG 调试（主要开发接口） |
| USB Full-speed | Type-C | USB 2.0 Full-speed，供电或通信 |
| USB 2.0 Type-C | Type-C | USB 2.0 OTG HS，P4 作 Device |
| USB 2.0 Type-A | Type-A | USB 2.0 OTG HS，P4 作 Host，最大 500 mA |

注意：USB 2.0 Type-C 和 Type-A 不能同时使用。

## 以太网

- **PHY 芯片**: IP101，连接 ESP32-P4 内部 EMAC RMII 接口
- **接口**: RJ45，支持 10/100 Mbps 自适应

| 信号 / 参数 | 配置 | 说明 |
| --- | --- | --- |
| MDC | GPIO31 | SMI 管理时钟 |
| MDIO | GPIO52 | SMI 管理数据 |
| PHY_RST | GPIO51 | PHY 复位 |
| PHY_ADDR | 1 | IP101 地址 |
| 板端静态 IP | 192.168.10.2 | ESP32-P4 |
| 上位机 / 网关 IP | 192.168.10.1 | PC 端网卡 |
| TCP 控制端口 | 5000 | 分拣命令、状态、metrics、SIM_LINE |
| TCP 图像端口 | 5001 | JPEG 快照 |

## 分拣执行机构与传感器

当前分拣硬件连接以 `E:\two_stage_host_handoff_20260705\new_merge_motor_two_stage` 交接工程为准。

### 电机 PWM

| 电机 | PWM_A | PWM_B | 说明 |
| --- | --- | --- | --- |
| Motor0 / A 段 | GPIO2 | GPIO3 | MCPWM 双路方向控制 |
| Motor1 / B 段 | GPIO32 | GPIO36 | MCPWM 双路方向控制 |
| Motor2 / C 段 | GPIO4 | GPIO5 | MCPWM 双路方向控制 |

### 分拣传感器

| 传感器 | GPIO | 有效电平 | 说明 |
| --- | --- | --- | --- |
| S1 | GPIO53 | 高电平 | 视觉窗口 / 新包裹入口；GPIO53 不再作为音频 PA_EN 使用 |
| S2 | GPIO23 | 高电平 | A 到 B 交接 |
| S3 | 未接 | 高电平 | 交接工程中配置为 -1 |
| S4 | GPIO22 | 高电平 | B 到 C 交接 |

### 编码器

| 编码器 | A 相 | B 相 | 说明 |
| --- | --- | --- | --- |
| Encoder0 | 未接 | 未接 | 交接工程中配置为 -1 |
| Encoder1 | 未接 | 未接 | 交接工程中配置为 -1 |
| Encoder2 | 未接 | 未接 | 交接工程中配置为 -1 |

编码器参数保留：单相 12 脉冲/转、采样周期 100 ms、轮径 76 mm、毛刺滤波 1000 ns。

## 电源

- 供电接口：USB Serial/JTAG (Type-C)、USB Full-speed (Type-C)、USB 2.0 Type-C，任一均可供电
- 板载电源开关（背面）
- 5 V → 3.3 V LDO
- **LDO_VO3 / LDO_VO4**：为板上部分 VDD 电源域供电，需在软件中配置正确输出电压和使能状态

## JTAG / 调试接口

- **USB Serial/JTAG Port**（Type-C，编号16）：用于烧录固件和 JTAG 调试
- OpenOCD board config: `board/esp32p4-builtin.cfg`

## 按键

| 名称 | 功能 |
| --- | --- |
| Reset | 复位 ESP32-P4 |
| BOOT | 按住 BOOT 再按 Reset，进入固件下载模式 |

## 排针 J1（可用 GPIO）

| J1 序号 | GPIO | 备注 |
| --- | --- | --- |
| 3 | GPIO7 | I2C SDA（共享） |
| 5 | GPIO8 | I2C SCL（共享） |
| 7 | GPIO23 |
| 8 | GPIO37 | U0TXD |
| 10 | GPIO38 | U0RXD |
| 11 | GPIO21 | GT911 触摸中断 INT |
| 12 | GPIO22 |  |
| 13 | GPIO20 | USB D+（共享） |
| 15 | GPIO6 |  |
| 16 | GPIO5 |  |
| 18 | GPIO4 |  |
| 19 | GPIO3 |  |
| 21 | GPIO2 |  |
| 22 | GPIO1 | 默认禁用（XTAL_32K），需移动 R61→R199 启用 |
| 23 | GPIO0 | 默认禁用（XTAL_32K），需移动 R59→R197 启用 |
| 24 | GPIO36 |  |
| 26 | GPIO32 |  |
| 29 | GPIO33 | GT911 触摸复位 RST |
| 31 | GPIO26 | LCD PWM 背光（1024×600 屏） |
| 32 | GPIO54 |  |
| 33 | GPIO48 |  |
| 35 | GPIO53 | 分拣 S1 传感器（高电平有效）；不再作为 PA_EN 使用 |
| 36 | GPIO46 |  |
| 37 | GPIO47 |  |
| 38 | GPIO27 | LCD RST（1024×600 屏） |
| 40 | GPIO45 | 默认禁用（SD_PWRn），需移动 R231→R100 启用 |

电源排针：3.3 V（J1-1, J1-17）、5 V（J1-2, J1-4）、GND（J1-6, J1-9, J1-14, J1-20, J1-25, J1-30, J1-34, J1-39）

## 时钟

- 40 MHz 主晶振（系统时钟）
- 32.768 kHz 晶振（Deep-sleep 低功耗时钟）
