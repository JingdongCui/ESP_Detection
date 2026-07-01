# ESP32-P4 PSRAM 总线仲裁优先级配置指南

## 结论先行

**sdkconfig (menuconfig) 中没有任何选项可以配置 AXI 总线仲裁优先级。**

ESP-IDF v5.5 框架本身也不会在启动时配置这些优先级，所有值保持硬件默认 0（Round-Robin）。
必须在用户代码中通过 HAL API 或直接寄存器操作来配置。

---

## 硬件架构

```
┌─────────────────────────────────────────────────────────────┐
│                    AXI ICM (Interconnect Matrix)             │
│                    基地址: 0x500A4000                         │
│                    13 Master × 7 Slave                       │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  Master 端口:                     Slave 端口:               │
│  ┌──────────────────┐            ┌──────────────────┐      │
│  │ Port 0: CPU      │            │ Slave: L2MEM     │      │
│  │ Port 1: CACHE    │───────────>│ Slave: Flash MSPI│      │
│  │ Port 5: DW_GDMA0 │───────────>│ Slave: PSRAM MSPI│     │
│  │ Port 6: DW_GDMA1 │───────────>│ Slave: MIPI LCD  │      │
│  │ Port 8: AXI_GDMA │            │ Slave: MIPI CAM  │      │
│  │ Port 10: DMA2D   │            └──────────────────┘      │
│  │ Port 11: H264_M0 │                                      │
│  │ Port 12: H264_M1 │                                      │
│  └──────────────────┘                                      │
│                                                             │
│  三层仲裁:                                                   │
│  1. 命令通道优先级 (MST_ARB_PRIORITY) — 4-bit, 0-15        │
│  2. QoS 信号优先级 (ARQOS/AWQOS)     — 4-bit, 0-15        │
│  3. 令牌桶速率限制 (QoS Regulator)   — 流量整形            │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│              PSRAM MSPI Controller (0x5008E000)              │
│              内部仲裁: 读/写通道优先级 + 权重                  │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
                    PSRAM (HEX 16-line, 250MHz DDR)
```

## Master 端口与外设对应关系

| Port ID | 枚举名 | 对应外设 | 本项目用途 |
|---------|--------|---------|-----------|
| 0 | AXI_ICM_MASTER_CPU | HP/LP CPU, USB, EMAC, SDMMC, AHB-GDMA | 聚合端口 |
| 1 | AXI_ICM_MASTER_CACHE | **L2 Cache** | CPU 读写 PSRAM（DL 推理、vision） |
| 5 | AXI_ICM_MASTER_DW_GDMA_M0 | DW-GDMA 主端口 0 | **CSI Camera DMA** |
| 6 | AXI_ICM_MASTER_DW_GDMA_M1 | DW-GDMA 主端口 1 | **DSI LCD DMA** |
| 8 | AXI_ICM_MASTER_GDMA | AXI-GDMA | 通用 DMA |
| 10 | AXI_ICM_MASTER_DMA2D | DMA2D | **PPA 图像处理** |
| 11 | AXI_ICM_MASTER_H264_M0 | H264 DMA 0 | 未使用 |
| 12 | AXI_ICM_MASTER_H264_M1 | H264 DMA 1 | 未使用 |

---

## 配置方法

### 方法一：HAL API（推荐）

```c
#include "hal/axi_icm_ll.h"
```

#### QoS 信号优先级（ARQOS/AWQOS）

```c
// 函数签名: (write_priority, read_priority)，范围 0-15，值越大优先级越高

// L2 Cache（CPU 访问 PSRAM 的通道）
axi_icm_ll_set_cache_qos_arbiter_prio(uint32_t write_prio, uint32_t read_prio);

// DMA2D / PPA
axi_icm_ll_set_dma2d_qos_arbiter_prio(uint32_t write_prio, uint32_t read_prio);

// AXI-GDMA
axi_icm_ll_set_gdma_qos_arbiter_prio(uint32_t write_prio, uint32_t read_prio);

// DW-GDMA（master_port: 0=CSI Camera, 1=DSI LCD）
axi_icm_ll_set_dw_gdma_qos_arbiter_prio(int master_port, uint32_t write_prio, uint32_t read_prio);

// CPU 聚合端口
axi_icm_ll_set_cpu_qos_arbiter_prio(uint32_t write_prio, uint32_t read_prio);

// H264 DMA
axi_icm_ll_set_h264_dma_qos_arbiter_prio(int master_port, uint32_t write_prio, uint32_t read_prio);
```

#### 令牌桶速率限制

```c
// 设置突发深度（1-256），启用速率调节器
axi_icm_ll_set_qos_burstiness(int master_id, uint32_t burstiness, axi_icm_access_type_t access_type);

// 设置峰值速率和事务速率
// peak_level: 0=1/2, 1=1/4, 2=1/8, ..., 11=1/4096（越小速率越高）
// transaction_level: 必须 >= peak_level
axi_icm_ll_set_qos_peak_transaction_rate(int master_id, uint32_t peak_level, uint32_t transaction_level, axi_icm_access_type_t access_type);

// access_type 枚举:
// AXI_ICM_ACCESS_READ  = 0
// AXI_ICM_ACCESS_WRITE = 1
```

### 方法二：直接寄存器操作

```c
#include "soc/icm_sys_struct.h"

extern axi_icm_dev_t AXI_ICM;
```

#### 命令通道仲裁优先级（与 QoS 信号是两套独立机制）

```c
// MST_ARB_PRIORITY_REG0 (0x500A401C) — 4-bit 每端口，0-15
AXI_ICM.mst_arb_priority_reg0.reg_cpu_priority = 0;       // CPU 聚合
AXI_ICM.mst_arb_priority_reg0.reg_cache_priority = 15;    // L2 Cache
AXI_ICM.mst_arb_priority_reg0.reg_dma2d_priority = 4;     // DMA2D/PPA
AXI_ICM.mst_arb_priority_reg0.reg_gdma_mst1_priority = 6; // DW-GDMA0 (CSI)
AXI_ICM.mst_arb_priority_reg0.reg_gdma_mst2_priority = 6; // DW-GDMA1 (DSI)
AXI_ICM.mst_arb_priority_reg0.reg_axi_pdma_priority = 4;  // AXI-GDMA
```

#### 从端口响应通道优先级

```c
// SLV_ARB_PRIORITY (0x500A4024) — 3-bit 每端口，0-7
AXI_ICM.slv_arb_priority.reg_psram_mspi_priority = 7;     // PSRAM 响应最高
AXI_ICM.slv_arb_priority.reg_flash_mspi_priority = 4;     // Flash
AXI_ICM.slv_arb_priority.reg_l2mem_priority = 4;          // L2MEM
```

#### PSRAM MSPI 内部读写仲裁

```c
#include "soc/spi_mem_s_reg.h"

// SPI_MEM_S_CACHE_FCTRL_REG (0x5008E03C)
// 启用加权仲裁
REG_SET_BIT(SPI_MEM_S_CACHE_FCTRL_REG, SPI_MEM_S_ARB_WEI_EN);

// 设置读优先级高于写（读=1, 写=0）
REG_SET_BIT(SPI_MEM_S_CACHE_FCTRL_REG, SPI_MEM_S_ARB_REQ0_PRI);   // 读优先
REG_CLR_BIT(SPI_MEM_S_CACHE_FCTRL_REG, SPI_MEM_S_ARB_REQ1_PRI);   // 写次之

// 设置读/写权重（4-bit，0-15）
REG_SET_FIELD(SPI_MEM_S_CACHE_FCTRL_REG, SPI_MEM_S_ARB_REQ0_WEI, 12); // 读权重 12
REG_SET_FIELD(SPI_MEM_S_CACHE_FCTRL_REG, SPI_MEM_S_ARB_REQ1_WEI, 4);  // 写权重 4
```

---

## 本项目推荐配置方案

### 目标

- 降低 DL 推理任务 (dlw0/dlw1) 的 PSRAM 访问延迟
- 保证 LCD DMA 不 underrun（撕裂/黑屏）
- 保证 Camera DMA 不丢帧

### 方案 A：保守方案（提升 Cache 优先级）

```c
#include "hal/axi_icm_ll.h"
#include "soc/icm_sys_struct.h"
#include "soc/spi_mem_s_reg.h"

void psram_arbiter_config(void)
{
    /* ── 第一层：命令通道仲裁优先级 ── */
    AXI_ICM.mst_arb_priority_reg0.reg_cache_priority = 10;    // L2 Cache 高优先
    AXI_ICM.mst_arb_priority_reg0.reg_gdma_mst1_priority = 8; // CSI Camera
    AXI_ICM.mst_arb_priority_reg0.reg_gdma_mst2_priority = 8; // DSI LCD
    AXI_ICM.mst_arb_priority_reg0.reg_dma2d_priority = 6;     // PPA
    AXI_ICM.mst_arb_priority_reg0.reg_axi_pdma_priority = 6;  // AXI-GDMA

    /* ── 第二层：QoS 信号优先级 ── */
    axi_icm_ll_set_cache_qos_arbiter_prio(10, 12);            // Cache 读优先级最高
    axi_icm_ll_set_dw_gdma_qos_arbiter_prio(0, 6, 8);        // CSI: 写6, 读8
    axi_icm_ll_set_dw_gdma_qos_arbiter_prio(1, 8, 8);        // DSI: 写8, 读8
    axi_icm_ll_set_dma2d_qos_arbiter_prio(4, 4);             // PPA: 低

    /* ── PSRAM MSPI 内部：读优先于写 ── */
    REG_SET_BIT(SPI_MEM_S_CACHE_FCTRL_REG, SPI_MEM_S_ARB_WEI_EN);
    REG_SET_BIT(SPI_MEM_S_CACHE_FCTRL_REG, SPI_MEM_S_ARB_REQ0_PRI);
    REG_CLR_BIT(SPI_MEM_S_CACHE_FCTRL_REG, SPI_MEM_S_ARB_REQ1_PRI);
    REG_SET_FIELD(SPI_MEM_S_CACHE_FCTRL_REG, SPI_MEM_S_ARB_REQ0_WEI, 12);
    REG_SET_FIELD(SPI_MEM_S_CACHE_FCTRL_REG, SPI_MEM_S_ARB_REQ1_WEI, 4);
}
```

**逻辑：**
- Cache (CPU) 优先级 10-12，确保 DL 推理的 cache refill 优先被服务
- LCD/CAM DMA 优先级 8，略低于 Cache 但仍较高（有 FIFO 缓冲）
- PPA 优先级最低（突发性操作，可以等待）
- PSRAM 内部读优先于写（CPU 读 miss 比 DMA 写入更延迟敏感）

### 方案 B：激进方案（Cache 最高 + DMA 限速）

```c
void psram_arbiter_config_aggressive(void)
{
    /* ── 命令通道：Cache 绝对优先 ── */
    AXI_ICM.mst_arb_priority_reg0.reg_cache_priority = 15;
    AXI_ICM.mst_arb_priority_reg0.reg_gdma_mst1_priority = 6;
    AXI_ICM.mst_arb_priority_reg0.reg_gdma_mst2_priority = 6;
    AXI_ICM.mst_arb_priority_reg0.reg_dma2d_priority = 4;
    AXI_ICM.mst_arb_priority_reg0.reg_axi_pdma_priority = 4;

    /* ── QoS：Cache 最高 ── */
    axi_icm_ll_set_cache_qos_arbiter_prio(12, 15);
    axi_icm_ll_set_dw_gdma_qos_arbiter_prio(0, 4, 6);
    axi_icm_ll_set_dw_gdma_qos_arbiter_prio(1, 6, 6);
    axi_icm_ll_set_dma2d_qos_arbiter_prio(2, 2);

    /* ── 令牌桶：限制 LCD DMA 突发长度 ── */
    // 突发深度 32（限制单次 burst 最大 32 个 beat）
    axi_icm_ll_set_qos_burstiness(AXI_ICM_MASTER_DW_GDMA_M1, 32, AXI_ICM_ACCESS_READ);
    // 峰值速率 1/2（level=0），事务速率 1/4（level=1）
    axi_icm_ll_set_qos_peak_transaction_rate(AXI_ICM_MASTER_DW_GDMA_M1, 0, 1, AXI_ICM_ACCESS_READ);

    /* ── PSRAM 内部读优先 ── */
    REG_SET_BIT(SPI_MEM_S_CACHE_FCTRL_REG, SPI_MEM_S_ARB_WEI_EN);
    REG_SET_BIT(SPI_MEM_S_CACHE_FCTRL_REG, SPI_MEM_S_ARB_REQ0_PRI);
    REG_CLR_BIT(SPI_MEM_S_CACHE_FCTRL_REG, SPI_MEM_S_ARB_REQ1_PRI);
    REG_SET_FIELD(SPI_MEM_S_CACHE_FCTRL_REG, SPI_MEM_S_ARB_REQ0_WEI, 15);
    REG_SET_FIELD(SPI_MEM_S_CACHE_FCTRL_REG, SPI_MEM_S_ARB_REQ1_WEI, 2);
}
```

**风险：** LCD DMA 被限速后，如果 MIPI DSI FIFO 深度不足，可能出现 underrun。
需要实测验证。

---

## 调用位置

在 `app_main()` 中，**系统初始化之前**调用：

```c
void app_main(void)
{
    psram_arbiter_config();  // 尽早配置，在任何 DMA 启动之前

    System_Init();
    system_monitor_init();
    // ...
}
```

---

## 验证方法

配置后通过 RTT 监控观察效果：

1. **DL 推理 FPS 是否提升**（vision_task 的 AI FPS）
2. **dlw0 CPU 占用是否下降**（从 66% 降低说明 stall 减少）
3. **LCD 是否出现撕裂/闪烁**（DMA underrun 的表现）
4. **Camera 是否丢帧**（vision_task 的 cam_wait 时间是否异常增大）

### 读回寄存器验证配置生效

```c
#include "esp_log.h"

void print_icm_config(void)
{
    ESP_LOGI("ICM", "MST_ARB_PRIORITY = 0x%08lx",
             *(volatile uint32_t *)0x500A401C);
    ESP_LOGI("ICM", "SLV_ARB_PRIORITY = 0x%08lx",
             *(volatile uint32_t *)0x500A4024);
    ESP_LOGI("ICM", "MST_ARQOS = 0x%08lx",
             *(volatile uint32_t *)0x500A4028);
    ESP_LOGI("ICM", "MST_AWQOS = 0x%08lx",
             *(volatile uint32_t *)0x500A4030);
    ESP_LOGI("ICM", "SPI_MEM_S_CACHE_FCTRL = 0x%08lx",
             *(volatile uint32_t *)0x5008E03C);
}
```

---

## 寄存器速查表

### ICM_MST_ARB_PRIORITY_REG0 (0x500A401C)

| 位域 | 字段 | 默认 | 说明 |
|------|------|------|------|
| [3:0] | reg_cpu_priority | 0 | CPU 聚合端口 |
| [7:4] | reg_cache_priority | 0 | L2 Cache |
| [11:8] | reg_dma2d_priority | 0 | DMA2D (PPA) |
| [15:12] | reg_gdma_mst1_priority | 0 | DW-GDMA M0 (CSI Camera) |
| [19:16] | reg_gdma_mst2_priority | 0 | DW-GDMA M1 (DSI LCD) |
| [23:20] | reg_h264_m1_priority | 0 | H264 M0 |
| [27:24] | reg_h264_m2_priority | 0 | H264 M1 |
| [31:28] | reg_axi_pdma_priority | 0 | AXI PDMA |

### ICM_SLV_ARB_PRIORITY (0x500A4024)

| 位域 | 字段 | 默认 | 说明 |
|------|------|------|------|
| [5:3] | reg_l2mem_priority | 0 | L2MEM |
| [14:12] | reg_flash_mspi_priority | 0 | Flash MSPI |
| [17:15] | reg_psram_mspi_priority | 0 | PSRAM MSPI |
| [20:18] | reg_lcd_priority | 0 | MIPI LCD |
| [23:21] | reg_cam_priority | 0 | MIPI CAM |

### ICM_MST_ARQOS_REG0 (0x500A4028) — 读 QoS

布局与 MST_ARB_PRIORITY_REG0 相同，每端口 4-bit。

### ICM_MST_AWQOS_REG0 (0x500A4030) — 写 QoS

布局与 MST_ARB_PRIORITY_REG0 相同，每端口 4-bit。

### SPI_MEM_S_CACHE_FCTRL_REG (0x5008E03C) — PSRAM 内部仲裁

| 位 | 字段 | 默认 | 说明 |
|----|------|------|------|
| [9] | ARB_WEI_EN | 0 | 启用加权仲裁 |
| [10] | ARB_REQ0_PRI | 0 | AXI 读优先级 (1-bit) |
| [11] | ARB_REQ1_PRI | 0 | AXI 写优先级 (1-bit) |
| [15:12] | ARB_REQ0_WEI | 0 | AXI 读权重 (4-bit) |
| [19:16] | ARB_REQ1_WEI | 0 | AXI 写权重 (4-bit) |

---

## 关键文件路径（ESP-IDF v5.5.4）

```
$IDF_PATH/components/hal/esp32p4/include/hal/axi_icm_ll.h     — HAL API
$IDF_PATH/components/soc/esp32p4/register/hw_ver3/soc/
  icm_sys_reg.h          — ICM 寄存器宏定义
  icm_sys_struct.h       — ICM 寄存器结构体
  icm_sys_qos_reg.h      — ICM QoS 寄存器宏
  icm_sys_qos_struct.h   — ICM QoS 寄存器结构体
  spi_mem_s_reg.h        — PSRAM MSPI 寄存器（含 ARB 位域）
  reg_base.h             — 外设基地址
```
