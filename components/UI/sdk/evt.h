/*
 * evt.h - Event helpers for anyui SDK
 *
 * This file is created and owned by anyui.
 *
 * COPYRIGHT 2025 anyui Team
 * All rights reserved.
 *
 * https://anyui.tech/
 *
 * Author: anyui Team
 */
#ifndef __EVT_H__
#define __EVT_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "lvgl.h"

#define EVT_BT 1
enum bt_evt_enum {
  EVT_BT_CHANGED = 1,
};

#define EVT_WIFI 2
enum wifi_evt_enum {
  EVT_WIFI_CHANGED = 1,
};

#define EVT_BATTERY 3
enum battery_evt_enum {
  EVT_BATTERY_CHANGED = 1,
};

#define EVT_DRAWER 4
enum drawer_evt_enum {
  EVT_DRAWER_OPEN = 1,
};

#define EVT_TIME 5
enum time_evt_enum {
  EVT_TIME_CHANGED = 1,
};

#define EVT_SYSTEM_MONITOR 6
enum system_monitor_evt_enum {
  EVT_SYSTEM_MONITOR_CHANGED = 1,
};

typedef struct {
  int cpu1_usage;
  int cpu2_usage;
  int cpu_total_usage;
  int memory_usage;
  int heap_free_kb;
  int heap_min_free_kb;
  int heap_max_block_kb;
  int internal_free_kb;
  int psram_usage;
  int psram_free_kb;
  int psram_total_kb;
  int ram_usage;
  int runtime_sec;
  int chip_temp;
  int cpu_freq_mhz;
  int task_count;
} system_monitor_event_data_t;

#define EVT_VISION 7
enum vision_evt_enum {
  EVT_VISION_RESULT_CHANGED = 1,
  EVT_VISION_LOG_APPENDED = 2,
};

typedef struct {
  char status[24];      // "识别成功" / "无目标" / "等待中"
  int  confidence;      // 面单最高分框置信度 %(0~100)；无框=0
  int  logo_confidence; // Logo 最高分框置信度 %(0~100)；无框=0
  int  fps_x10;         // 帧率 ×10（显示拆成 NN.N）
  int  infer_time_ms;   // 推理耗时 ms（假框阶段=0）
  char company[24];     // 类别名/快递公司（占位）
  int  jt_a;            // 极兔 A 概率 %(0~100)
  int  zt_a;            // 中通 A 概率 %(0~100)
  int  yd_a;            // 韵达 A 概率 %(0~100)
} vision_result_event_data_t;

typedef struct {
  char time[20];
  char company[24];
  char result[24];
  int waybill_confidence;
  int logo_confidence;
  int waybill_infer_time_ms;
  int logo_infer_time_ms;
} vision_log_event_data_t;

#define EVT_ETHERNET 8
enum ethernet_evt_enum {
  EVT_ETHERNET_STATUS_CHANGED = 1,
};

typedef struct {
  int connected;   // 1=以太网已连接(获取IP)；0=断开/连接失败
} ethernet_event_data_t;

typedef void (*event_cb_func_t)(uint8_t event, uint16_t code, uint16_t type,
                                uint16_t len, uint8_t *data, uint8_t status);

typedef struct event_node_t {
  uint8_t event;
  uint16_t code;
  uint16_t type;
  uint16_t len;
  event_cb_func_t callback;
  struct event_node_t *next;
} event_node_t;

typedef struct event_table_t {
  event_node_t *head;
} event_table_t;

uint32_t register_event(event_table_t *table, uint8_t event, uint16_t code,
                        uint16_t type, uint16_t len, event_cb_func_t callback);

uint32_t unregister_events(event_table_t *table);

uint32_t send_event(event_table_t *table, uint8_t event, uint16_t code,
                    uint8_t *data, uint8_t status);

void set_current_event_table(event_table_t *table);
event_table_t *get_current_event_table();

event_table_t *get_global_event_table();
void print_event_table(event_table_t *table);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
