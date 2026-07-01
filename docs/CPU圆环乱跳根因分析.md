# CPU 圆环乱跳根因分析（A2 双 fb 直刷方案）

## 一、现象

仪表盘上的 CPU 占用率圆环（lv_arc）显示异常跳动/截断：

- **初期**：仅 `cpu02`（core1 占用）圆环乱跳，`cpu01`（core0 占用）稳定。
- **后期**（关闭 lvgl_adapter 的 `enable_ppa_accel` 后）：两个圆环都开始乱跳。
- RTT 打印的 CPU 数值本身完全稳定、正确。

## 二、关键事实（已用源码 + 实验证据闭环，非推测）

### 2.1 圆环显示值 ≡ RTT 打印值

`components/system_monitor/system_monitor.c` 的 `monitor_task` 用**同一份 snapshot**
同时调用 `post_system_ui()`（更新圆环）和 `print_monitor_rtt()`（打印 RTT）。
RTT 数值稳定 → 圆环的"数值"是对的。**"乱跳"不是数值问题，是渲染问题。**

### 2.2 LVGL buf == panel framebuffer（同一物理内存）

`managed_components/espressif__esp_lvgl_adapter/src/display/display_manager.c:936-937`：

```c
cfg->draw_buf_primary   = cfg->frame_buffers[primary_idx];
cfg->draw_buf_secondary = frame_buffers[secondary_idx];
```

DOUBLE_DIRECT 模式下，LVGL 的 `buf_1`/`buf_2` 就是 panel 的 `fb0`/`fb1`，
两块物理 fb 交替作为 DPI 扫描源。

### 2.3 LVGL DOUBLE_DIRECT 的跨帧一致性靠 CPU memcpy 同步

`managed_components/lvgl__lvgl/src/core/lv_refr.c`：

- `lv_refresh` 末尾（424-433 行）：把**本帧失效区**记入 `sync_areas` 链表，供下一帧同步。
- `refr_sync_areas`（657-754 行）：下一帧开头，把 `on_screen` 的 sync 区
  用 `lv_draw_buf_copy`（CPU memcpy）复制到 `off_screen`（746-748 行），
  保证两块 fb 在"非本帧重绘区"上保持一致。

### 2.4 DPI flush 只 writeback「本帧失效区」，不是整屏（增量帧）

`D:/esp/v5.5.4/esp-idf/components/esp_lcd/dsi/esp_lcd_panel_dpi.c:511-517`：

DOUBLE_DIRECT 下 draw buffer 落在 fb 内存范围，走 `!do_copy` 分支，
只对 `[y_start, y_end)` 这段失效区做 `esp_cache_msync(... C2M)` 写回。
**本帧没失效的区域不写回。**

### 2.5 vision PPA blit 的 output cache invalidate 横扫整行

`D:/esp/v5.5.4/esp-idf/components/esp_driver_ppa/src/ppa_srm.c:268-273`：

```c
out_ext_window     = out.buffer + block_offset_y * out.pic_w * bytes;  // 视频顶行起
out_ext_window_len = out.pic_w * new_block_h * bytes;                  // 宽 = out.pic_w
esp_cache_msync(out_ext_window_aligned, ..., ESP_CACHE_MSYNC_FLAG_DIR_M2C);  // 丢弃 dirty cache
```

vision 的 `blit_preview_to_fb`（`components/vision/vision_app.c`）里
`out.pic_w = 1024`（整屏宽作行 stride）、`out.block_offset_y = s_preview_y`。
所以 PPA 在写 fb 前，对该 fb 做的 **M2C invalidate 范围是"视频顶行起、跨整屏宽 1024、
高 = 视频块高"的一整条横带**（不是仅视频那个矩形）。M2C invalidate **直接丢弃 dirty cache，不写回**。

### 2.6 vision 与 LVGL worker 严格串行（无并发竞态）

`esp_lv_adapter.c:1118-1120` 的 `lvgl_worker` 持 `s_ctx.lvgl_mutex` 跑 `lv_timer_handler`；
vision 的 `BSP_LVGL_Lock` 走同一把 `esp_lv_adapter_lock` → 同一 `lvgl_mutex`。
**所以这不是并发数据竞争，而是串行时序下的 cache 残留问题。**

### 2.7 圆环与视频区在 Y 轴重叠

视频容器 `cont_live_vedio`（`setup_scr_dashboard.c:5024-5028`）高 375；
乱跳的 runtime CPU 圆环（core1/core2，5555/5635 行）位于其下方区域，
Y 范围与 2.5 的 invalidate 横带相交。

## 三、根因链条

```
帧 N：arc 值变 → arc 区失效 → LVGL 渲染 arc 到 fbA → flush 只 writeback fbA 的 arc 失效区
帧 N+1：arc 值没变（不再失效）
        → refr_sync_areas 用 CPU memcpy 把 fbA 的 arc 区复制到 fbB
          （fbB 的 arc 区 cache 变 dirty）
        → 本帧 fbB 的失效区【不含 arc】→ DPI flush 不 writeback fbB 的 arc 区
        → fbB 的 arc dirty cache 残留，尚未落 PSRAM
        ↓
vision 拿到 LVGL 锁，PPA blit fbB：
        → M2C invalidate「视频横带整行」（含 arc 区）
        → 【直接丢弃 fbB 的 arc dirty cache，不写回】
        → fbB 的 arc 区 PSRAM 停留旧值（从没被写回过）
        ↓
DPI 翻转扫描 fbB → arc 显示旧值 → 与 fbA 不一致 → 视觉上乱跳
```

**一句话：** LVGL 的 DOUBLE_DIRECT 同步会在 off_screen fb 产生「sync 复制进来、
但本帧未再失效因而未被 flush 写回」的 arc dirty cache；vision 的 PPA blit 抢在它
落盘前，用 M2C invalidate 横扫该 fb 整行把这块 dirty cache 丢弃，使该 fb 的 arc 区
PSRAM 停留旧值，造成两块 fb 的 arc 不一致 → DPI 翻转乱跳。

## 四、为什么现象是这样的（自洽性验证）

- **只 cpu02(c1) 跳、cpu01(c0) 稳**：c0 占用长期≈21% 几乎不变 → arc 极少失效 →
  sync 反复复制同值，即便 PSRAM 旧值也等于新值，缺陷不可见；c1 占用 2~5% 频繁变 →
  sync 复制新值产生 dirty cache 被 invalidate 丢弃 → PSRAM 旧值 → 跳。
- **关 PPA 加速后两个都跳**：软件渲染路径变慢、时序窗口变化，dirty cache 残留窗口
  扩大，连"几乎不变"的 c0 arc 偶发失效也暴露缺陷 → 两个都跳。
- **vision 完全不写 fb（诊断 A）→ 不跳**：无 invalidate，残留 dirty cache 由后续
  帧的 writeback 或自然 eviction 最终落 PSRAM，一致性恢复。
- **保留每帧锁、只跳过 blit（诊断 B）→ 不跳**：锁竞争被排除，证明罪魁是「写 fb 的
  PPA cache 副作用」本身，而非持锁频率。

## 五、对照实验（决定性证据）

| 实验 | 每帧持 LVGL 锁 | PPA 写两块 fb | 圆环表现 |
| --- | --- | --- | --- |
| 原方案 | 是 | 是 | **乱跳** |
| 诊断 A（跳锁+跳 blit） | 否 | 否 | 稳定 |
| 诊断 B（持锁、只跳 blit） | 是 | 否 | 稳定 |

诊断 A 与 B 同为稳定 → 隔离出唯一变量：**PPA 写 fb 的 M2C invalidate 副作用**。

## 六、修复方向（硬件直刷方案，不改用 lv_canvas）

核心：**在 vision 的 PPA blit 之前，先把目标 fb 视频横带区域的 dirty cache 主动
C2M writeback 落 PSRAM**，使 LVGL 残留的 arc dirty cache 先持久化；之后 PPA 的
M2C invalidate 只是作废 cache（PSRAM 已是新值，arc 不会回退），PPA DMA 再写视频区，
arc 区 PSRAM 始终保持新值。两块 fb 的 arc 因此一致，乱跳消除。

待实施后补充验证结果。

---

# 附:长跑卡死(872s / 3 万帧偶发)触发因子深挖

修复 arc 乱跳后,固件连续跑 **872 秒 / 约 3 万帧** 后**整屏冻结**。JTAG 现场诊断如下。

## 一、死锁现场(GDB 栈 + 硬件寄存器双重实证)

两核 pc 都停在 `esp_cpu_wait_for_intr`(IDLE),非崩溃——是任务**阻塞**导致核空转。三任务栈:

| 任务 | 核 | 卡在 | 证据 |
| --- | --- | --- | --- |
| `vision_disp` | c0 | `blit_preview_to_fb`(vision_app.c:254)→ `ppa_do_operation`(ppa_core.c:459)`xSemaphoreTake(trans_elm->sem, portMAX_DELAY)` **永久阻塞**,且**全程持 LVGL 锁** | Thread 11 |
| `lvgl_worker` | c1 | `esp_lv_adapter_lock` 等 LVGL 锁(被 vision_disp 持着) | Thread 10 |
| `sysmon` | c0 | `BSP_LVGL_Lock`→ `post_system_ui` 等同一把锁 | Thread 8 |
| `vision_det` | c1 | 正常 30fps(不碰 LVGL 锁) | RTT 仍在涨 |

现象自洽:SYSMON 停更、屏幕冻结,但 RTT 总行数还涨(c1 的 vision_det 在打印)、堆内存正常、up 计时停。

## 二、PPA 引擎卡死(halted 态读寄存器)

| 寄存器 | 值 | 含义 |
| --- | --- | --- |
| `SR_PARAM_ERR_ST` | `0x3` | **TX_DSCR_VB_ERR_ST + TX_DSCR_HB_ERR_ST**(输出/TX 输入描述符 垂直+水平块错误) |
| `INT_RAW` | `0x7` | SR_EOF + BLEND_EOF + **SR_PARAM_CFG_ERR_INT_RAW** 均置位 |
| `INT_ENA` | `0x0` | **参数错误中断被屏蔽,无人处理** |
| `SR_STATUS`(FSM) | `0x115` | 引擎 FSM 停在 TX 扫描中间态,非空闲 |

## 三、共享资源结构(源码确认)

> **【2026-06-28 补正】** 本节原写「lvgl-flush 也提交 PPA、与 vision-scale 三方并发」，
> 经 codegraph + 源码复核**证伪**：本项目 ROTATE_0 + DOUBLE_DIRECT + RGB888 配置下，
> lvgl flush 走 `display_bridge_v9_flush_double_direct`（**零 PPA 调用**），且
> `lvgl_port_ppa_v9_init` 对非 RGB565 直接 return（blend/fill client 从不注册）。
> 故 **lvgl 运行时根本不向 PPA SR 引擎提交事务**，SR 引擎运行期为 **vision 独占**
> （scale + blit 在同一 `vision_display_task` 内顺序执行，互不重叠）。下文「三方并发」
> 描述保留作历史记录，但**不再作为承重前提**。

- **vision** 注册 1 个 `PPA_OPERATION_SRM` client(`s_ppa`)。
- **lvgl adapter** 也注册 `PPA_OPERATION_SRM` client(lvgl_bridge_v9.c:447),DOUBLE_DIRECT flush 时用它做 PPA blit。
- 两个 client **共用同一套物理 SR 引擎寄存器 + 一对 DMA 描述符**(`platform->hal.dev` / `dma_tx_desc`/`dma_rx_desc`,ppa_srm.c:49-81),靠 `engine->sem` 串行化执行。
- 三处 SRM 提交:vision-scale(vision_app.c:312,**锁外**,c0)、vision-blit(:333,持锁,c0)、lvgl-flush(持锁,c1)。其中 **vision-scale 与 lvgl-flush 不互斥**,高频(30fps×多次)并发提交到同一引擎。

## 四、驱动盲区(grep 全组件确认)

PPA 驱动(`esp_driver_ppa`)对 `SR_PARAM_CFG_ERR` **零处理**:
- 全组件搜不到任何 `PARAM_CFG_ERR`/`param_err`/`int_ena` 代码——不使能、不清除、不恢复。
- 唯一注册的回调是 `on_recv_eof = ppa_transaction_done_cb`(ppa_srm.c:128),**无错误回调、无超时、无引擎复位**。
- 驱动假定 RX EOF 必然到来。

## 五、触发链条

```
高频共享 SR 引擎(vision-scale 锁外 / vision-blit / lvgl-flush 三方,后两方持锁、前两方不互斥)
  → 某次 SRM 事务偶发触发 SR_PARAM_CFG_ERR(TX_DSCR 块错误,硬件级)
  → SR 引擎卡进错误态、FSM 停摆(SR_STATUS=0x115)
  → 因中断被屏蔽(INT_ENA=0)、驱动无错误处理 → 引擎永不复位、永不报错
  → RX DMA 永远收不到 SR 输出 → 永不产生 on_recv_eof
  → ppa_transaction_done_cb(ISR)永不执行
  → trans_elm->sem 与 engine->sem 永不释放
  → vision_disp 在 ppa_core.c:459 portMAX_DELAY 永久阻塞
  ↓ 【放大器】
  → vision_disp 全程持 LVGL 全局锁
  → lvgl_worker + sysmon 等锁全堵 → 屏幕 + c0 全任务冻结
  → (c1 的 vision_det 不碰锁 → 仍 30fps,故"半死不活")
```

## 六、责任划分

1. **架构放大器(确定,本项目可控)**:vision_disp **持全局 LVGL 锁** + `portMAX_DELAY` **无限阻塞**等 PPA。任何一次 PPA 不返回 → 永久全局死锁。这是"硬件直刷 + 与 LVGL 共享 fb"方案特有的——esp-who 无 LVGL 层,PPA 失败只丢一帧,绝不全局死锁。
2. **偶发硬件触发 + 驱动无恢复(IDF 层)**:SR 引擎偶发 TX_DSCR 参数错误(确切位级触发条件属 TRM/勘误范畴,源码无法定死;但参数固定、3 万帧才发,排除"参数写错"),叠加 PPA 驱动对该错误零监控零恢复,使一次瞬时错误变成引擎永久卡死。

## 七、与 cache writeback 修复的关系

本次 arc 修复在 `blit_preview_to_fb` 加的 C2M writeback 作用于**输出 fb 横带**;而本次错误是 **TX/输入描述符**(读 `s_preview_buf`)侧的块错误,两者是不同 buffer。故 cache 修复**不是触发因子**——这是方案本身既存的潜在脆弱性,只是跑满 14.5 分钟才显形。

## 八、修复方向(待确认)

- **必治(架构放大器)**:断开"单次 PPA 失败 → 全局死锁"的升级链。可选:① PPA blit 改有限超时 + 失败丢帧(不持锁死等);② 把 blit 移出 LVGL 全局锁的持有范围;③ 失败时主动复位 PPA 引擎。
- **可加固(触发因子)**:减少 SR 引擎并发压力(如 vision-scale 也纳入串行/错峰),或监控 SR_PARAM_CFG_ERR 主动恢复引擎。
