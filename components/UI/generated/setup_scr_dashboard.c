/**
 *
 * This file is created and owned by anyui.
 *
 * Version: 1.0.0
 *
 * COPYRIGHT 2026 anyui Team
 * All rights reserved.
 *
 * https://anyui.tech/
 *
 * Author: anyui Team
 */

#include "setup_ui.h"


lv_obj_t * scr_dashboard = NULL;
lv_obj_t * scr_dashboard_cont_background1 = NULL;
lv_obj_t * scr_dashboard_cont_background2 = NULL;
lv_obj_t * scr_dashboard_cont_system = NULL;
lv_obj_t * scr_dashboard_cont_system_memory = NULL;
lv_obj_t * scr_dashboard_label_memory_title = NULL;
lv_obj_t * scr_dashboard_slider_memory_usage = NULL;
lv_obj_t * scr_dashboard_label_memory_usage_value = NULL;
lv_obj_t * scr_dashboard_label_memory_usage_title = NULL;
lv_obj_t * scr_dashboard_cont_system_memory_ram = NULL;
lv_obj_t * scr_dashboard_label_ram_unit = NULL;
lv_obj_t * scr_dashboard_label_ram_value = NULL;
lv_obj_t * scr_dashboard_label_ram_title = NULL;
lv_obj_t * scr_dashboard_cont_system_memory_psram = NULL;
lv_obj_t * scr_dashboard_label_psram_usage_value = NULL;
lv_obj_t * scr_dashboard_label_psram_title = NULL;
lv_obj_t * scr_dashboard_label_psram_free_value = NULL;
lv_obj_t * scr_dashboard_slider_psram_usage = NULL;
lv_obj_t * scr_dashboard_label_psram_free_unit = NULL;
lv_obj_t * scr_dashboard_label_psram_total_value = NULL;
lv_obj_t * scr_dashboard_label_psram_total_unit = NULL;
lv_obj_t * scr_dashboard_label_psram_free_title = NULL;
lv_obj_t * scr_dashboard_label_psram_total_title = NULL;
lv_obj_t * scr_dashboard_cont_system_cpu = NULL;
lv_obj_t * scr_dashboard_label_cpu_title = NULL;
lv_obj_t * scr_dashboard_cont_system_cpu_container = NULL;
lv_obj_t * scr_dashboard_cont_system_cpu_03 = NULL;
lv_obj_t * scr_dashboard_label_cpu_03_title = NULL;
lv_obj_t * scr_dashboard_label_cpu_03_value = NULL;
lv_obj_t * scr_dashboard_cont_system_cpu_total = NULL;
lv_obj_t * scr_dashboard_label_cpu_total_title = NULL;
lv_obj_t * scr_dashboard_label_cpu_total_value = NULL;
lv_obj_t * scr_dashboard_cont_system_cpu_02 = NULL;
lv_obj_t * scr_dashboard_label_cpu_02_title = NULL;
lv_obj_t * scr_dashboard_label_cpu_02_value = NULL;
lv_obj_t * scr_dashboard_cont_system_heap = NULL;
lv_obj_t * scr_dashboard_label_heap_title = NULL;
lv_obj_t * scr_dashboard_label_heap_free_title = NULL;
lv_obj_t * scr_dashboard_label_heap_min_free_title = NULL;
lv_obj_t * scr_dashboard_label_heap_max_block_title = NULL;
lv_obj_t * scr_dashboard_label_heap_free_value = NULL;
lv_obj_t * scr_dashboard_label_heap_free_unit = NULL;
lv_obj_t * scr_dashboard_label_heap_min_free_value = NULL;
lv_obj_t * scr_dashboard_label_heap_min_free_unit = NULL;
lv_obj_t * scr_dashboard_label_heap_max_block_value = NULL;
lv_obj_t * scr_dashboard_label_heap_max_block_unit = NULL;
lv_obj_t * scr_dashboard_cont_system_status = NULL;
lv_obj_t * scr_dashboard_label_status_title = NULL;
lv_obj_t * scr_dashboard_cont_system_runtime = NULL;
lv_obj_t * scr_dashboard_label_runtime_value = NULL;
lv_obj_t * scr_dashboard_label_runtime_title = NULL;
lv_obj_t * scr_dashboard_cont_system_chip_temp = NULL;
lv_obj_t * scr_dashboard_label_chip_temp_value = NULL;
lv_obj_t * scr_dashboard_label_chip_temp_title = NULL;
lv_obj_t * scr_dashboard_cont_system_cpu_freq = NULL;
lv_obj_t * scr_dashboard_label_cpu_freq_unit = NULL;
lv_obj_t * scr_dashboard_label_cpu_freq_value = NULL;
lv_obj_t * scr_dashboard_label_cpu_freq_title = NULL;
lv_obj_t * scr_dashboard_cont_system_task_count = NULL;
lv_obj_t * scr_dashboard_label_task_count_value = NULL;
lv_obj_t * scr_dashboard_label_task_count_title = NULL;
lv_obj_t * scr_dashboard_cont_log = NULL;
lv_obj_t * scr_dashboard_cont_event_details = NULL;
lv_obj_t * scr_dashboard_cont_detail_20 = NULL;
lv_obj_t * scr_dashboard_label_detail_result_20 = NULL;
lv_obj_t * scr_dashboard_label_logo_conf_20 = NULL;
lv_obj_t * scr_dashboard_label_logo_inft_20 = NULL;
lv_obj_t * scr_dashboard_label_detail_time_20 = NULL;
lv_obj_t * scr_dashboard_label_mian_conf_20 = NULL;
lv_obj_t * scr_dashboard_label_mian_inft_20 = NULL;
lv_obj_t * scr_dashboard_cont_detail_badge_20 = NULL;
lv_obj_t * scr_dashboard_label_detail_badge_20 = NULL;
lv_obj_t * scr_dashboard_cont_detail_19 = NULL;
lv_obj_t * scr_dashboard_label_detail_result_19 = NULL;
lv_obj_t * scr_dashboard_label_logo_conf_19 = NULL;
lv_obj_t * scr_dashboard_label_logo_inft_19 = NULL;
lv_obj_t * scr_dashboard_label_detail_time_19 = NULL;
lv_obj_t * scr_dashboard_label_mian_conf_19 = NULL;
lv_obj_t * scr_dashboard_label_mian_inft_19 = NULL;
lv_obj_t * scr_dashboard_cont_detail_badge_19 = NULL;
lv_obj_t * scr_dashboard_label_detail_badge_19 = NULL;
lv_obj_t * scr_dashboard_cont_detail_18 = NULL;
lv_obj_t * scr_dashboard_label_detail_result_18 = NULL;
lv_obj_t * scr_dashboard_label_logo_conf_18 = NULL;
lv_obj_t * scr_dashboard_label_logo_inft_18 = NULL;
lv_obj_t * scr_dashboard_label_detail_time_18 = NULL;
lv_obj_t * scr_dashboard_label_mian_conf_18 = NULL;
lv_obj_t * scr_dashboard_label_mian_inft_18 = NULL;
lv_obj_t * scr_dashboard_cont_detail_badge_18 = NULL;
lv_obj_t * scr_dashboard_label_detail_badge_18 = NULL;
lv_obj_t * scr_dashboard_cont_detail_17 = NULL;
lv_obj_t * scr_dashboard_label_detail_result_17 = NULL;
lv_obj_t * scr_dashboard_label_logo_conf_17 = NULL;
lv_obj_t * scr_dashboard_label_logo_inft_17 = NULL;
lv_obj_t * scr_dashboard_label_detail_time_17 = NULL;
lv_obj_t * scr_dashboard_label_mian_conf_17 = NULL;
lv_obj_t * scr_dashboard_label_mian_inft_17 = NULL;
lv_obj_t * scr_dashboard_cont_detail_badge_17 = NULL;
lv_obj_t * scr_dashboard_label_detail_badge_17 = NULL;
lv_obj_t * scr_dashboard_cont_detail_16 = NULL;
lv_obj_t * scr_dashboard_label_detail_result_16 = NULL;
lv_obj_t * scr_dashboard_label_logo_conf_16 = NULL;
lv_obj_t * scr_dashboard_label_logo_inft_16 = NULL;
lv_obj_t * scr_dashboard_label_detail_time_16 = NULL;
lv_obj_t * scr_dashboard_label_mian_conf_16 = NULL;
lv_obj_t * scr_dashboard_label_mian_inft_16 = NULL;
lv_obj_t * scr_dashboard_cont_detail_badge_16 = NULL;
lv_obj_t * scr_dashboard_label_detail_badge_16 = NULL;
lv_obj_t * scr_dashboard_cont_detail_15 = NULL;
lv_obj_t * scr_dashboard_label_detail_result_15 = NULL;
lv_obj_t * scr_dashboard_label_logo_conf_15 = NULL;
lv_obj_t * scr_dashboard_label_logo_inft_15 = NULL;
lv_obj_t * scr_dashboard_label_detail_time_15 = NULL;
lv_obj_t * scr_dashboard_label_mian_conf_15 = NULL;
lv_obj_t * scr_dashboard_label_mian_inft_15 = NULL;
lv_obj_t * scr_dashboard_cont_detail_badge_15 = NULL;
lv_obj_t * scr_dashboard_label_detail_badge_15 = NULL;
lv_obj_t * scr_dashboard_cont_detail_14 = NULL;
lv_obj_t * scr_dashboard_label_detail_result_14 = NULL;
lv_obj_t * scr_dashboard_label_logo_conf_14 = NULL;
lv_obj_t * scr_dashboard_label_logo_inft_14 = NULL;
lv_obj_t * scr_dashboard_label_detail_time_14 = NULL;
lv_obj_t * scr_dashboard_label_mian_conf_14 = NULL;
lv_obj_t * scr_dashboard_label_mian_inft_14 = NULL;
lv_obj_t * scr_dashboard_cont_detail_badge_14 = NULL;
lv_obj_t * scr_dashboard_label_detail_badge_14 = NULL;
lv_obj_t * scr_dashboard_cont_detail_13 = NULL;
lv_obj_t * scr_dashboard_label_detail_result_13 = NULL;
lv_obj_t * scr_dashboard_label_logo_conf_13 = NULL;
lv_obj_t * scr_dashboard_label_logo_inft_13 = NULL;
lv_obj_t * scr_dashboard_label_detail_time_13 = NULL;
lv_obj_t * scr_dashboard_label_mian_conf_13 = NULL;
lv_obj_t * scr_dashboard_label_mian_inft_13 = NULL;
lv_obj_t * scr_dashboard_cont_detail_badge_13 = NULL;
lv_obj_t * scr_dashboard_label_detail_badge_13 = NULL;
lv_obj_t * scr_dashboard_cont_detail_12 = NULL;
lv_obj_t * scr_dashboard_label_detail_result_12 = NULL;
lv_obj_t * scr_dashboard_label_logo_conf_12 = NULL;
lv_obj_t * scr_dashboard_label_logo_inft_12 = NULL;
lv_obj_t * scr_dashboard_label_detail_time_12 = NULL;
lv_obj_t * scr_dashboard_label_mian_conf_12 = NULL;
lv_obj_t * scr_dashboard_label_mian_inft_12 = NULL;
lv_obj_t * scr_dashboard_cont_detail_badge_12 = NULL;
lv_obj_t * scr_dashboard_label_detail_badge_12 = NULL;
lv_obj_t * scr_dashboard_cont_detail_11 = NULL;
lv_obj_t * scr_dashboard_label_detail_result_11 = NULL;
lv_obj_t * scr_dashboard_label_logo_conf_11 = NULL;
lv_obj_t * scr_dashboard_label_logo_inft_11 = NULL;
lv_obj_t * scr_dashboard_label_detail_time_11 = NULL;
lv_obj_t * scr_dashboard_label_mian_conf_11 = NULL;
lv_obj_t * scr_dashboard_label_mian_inft_11 = NULL;
lv_obj_t * scr_dashboard_cont_detail_badge_11 = NULL;
lv_obj_t * scr_dashboard_label_detail_badge_11 = NULL;
lv_obj_t * scr_dashboard_cont_detail_10 = NULL;
lv_obj_t * scr_dashboard_label_detail_result_10 = NULL;
lv_obj_t * scr_dashboard_label_logo_conf_10 = NULL;
lv_obj_t * scr_dashboard_label_logo_inft_10 = NULL;
lv_obj_t * scr_dashboard_label_detail_time_10 = NULL;
lv_obj_t * scr_dashboard_label_mian_conf_10 = NULL;
lv_obj_t * scr_dashboard_label_mian_inft_10 = NULL;
lv_obj_t * scr_dashboard_cont_detail_badge_10 = NULL;
lv_obj_t * scr_dashboard_label_detail_badge_10 = NULL;
lv_obj_t * scr_dashboard_cont_detail_09 = NULL;
lv_obj_t * scr_dashboard_label_detail_result_09 = NULL;
lv_obj_t * scr_dashboard_label_logo_conf_09 = NULL;
lv_obj_t * scr_dashboard_label_logo_inft_09 = NULL;
lv_obj_t * scr_dashboard_label_detail_time_09 = NULL;
lv_obj_t * scr_dashboard_label_mian_conf_09 = NULL;
lv_obj_t * scr_dashboard_label_mian_inft_09 = NULL;
lv_obj_t * scr_dashboard_cont_detail_badge_09 = NULL;
lv_obj_t * scr_dashboard_label_detail_badge_09 = NULL;
lv_obj_t * scr_dashboard_cont_detail_08 = NULL;
lv_obj_t * scr_dashboard_label_detail_result_08 = NULL;
lv_obj_t * scr_dashboard_label_logo_conf_08 = NULL;
lv_obj_t * scr_dashboard_label_logo_inft_08 = NULL;
lv_obj_t * scr_dashboard_label_detail_time_08 = NULL;
lv_obj_t * scr_dashboard_label_mian_conf_08 = NULL;
lv_obj_t * scr_dashboard_label_mian_inft_08 = NULL;
lv_obj_t * scr_dashboard_cont_detail_badge_08 = NULL;
lv_obj_t * scr_dashboard_label_detail_badge_08 = NULL;
lv_obj_t * scr_dashboard_cont_detail_07 = NULL;
lv_obj_t * scr_dashboard_label_detail_result_07 = NULL;
lv_obj_t * scr_dashboard_label_logo_conf_07 = NULL;
lv_obj_t * scr_dashboard_label_logo_inft_07 = NULL;
lv_obj_t * scr_dashboard_label_detail_time_07 = NULL;
lv_obj_t * scr_dashboard_label_mian_conf_07 = NULL;
lv_obj_t * scr_dashboard_label_mian_inft_07 = NULL;
lv_obj_t * scr_dashboard_cont_detail_badge_07 = NULL;
lv_obj_t * scr_dashboard_label_detail_badge_07 = NULL;
lv_obj_t * scr_dashboard_cont_detail_06 = NULL;
lv_obj_t * scr_dashboard_label_detail_result_06 = NULL;
lv_obj_t * scr_dashboard_label_logo_conf_06 = NULL;
lv_obj_t * scr_dashboard_label_logo_inft_06 = NULL;
lv_obj_t * scr_dashboard_label_detail_time_06 = NULL;
lv_obj_t * scr_dashboard_label_mian_conf_06 = NULL;
lv_obj_t * scr_dashboard_label_mian_inft_06 = NULL;
lv_obj_t * scr_dashboard_cont_detail_badge_06 = NULL;
lv_obj_t * scr_dashboard_label_detail_badge_06 = NULL;
lv_obj_t * scr_dashboard_cont_detail_05 = NULL;
lv_obj_t * scr_dashboard_label_detail_result_05 = NULL;
lv_obj_t * scr_dashboard_label_logo_conf_05 = NULL;
lv_obj_t * scr_dashboard_label_logo_inft_05 = NULL;
lv_obj_t * scr_dashboard_label_detail_time_05 = NULL;
lv_obj_t * scr_dashboard_label_mian_conf_05 = NULL;
lv_obj_t * scr_dashboard_label_mian_inft_05 = NULL;
lv_obj_t * scr_dashboard_cont_detail_badge_05 = NULL;
lv_obj_t * scr_dashboard_label_detail_badge_05 = NULL;
lv_obj_t * scr_dashboard_cont_detail_04 = NULL;
lv_obj_t * scr_dashboard_label_detail_result_04 = NULL;
lv_obj_t * scr_dashboard_label_logo_conf_04 = NULL;
lv_obj_t * scr_dashboard_label_logo_inft_04 = NULL;
lv_obj_t * scr_dashboard_label_detail_time_04 = NULL;
lv_obj_t * scr_dashboard_label_mian_conf_04 = NULL;
lv_obj_t * scr_dashboard_label_mian_inft_04 = NULL;
lv_obj_t * scr_dashboard_cont_detail_badge_04 = NULL;
lv_obj_t * scr_dashboard_label_detail_badge_04 = NULL;
lv_obj_t * scr_dashboard_cont_detail_03 = NULL;
lv_obj_t * scr_dashboard_label_detail_result_03 = NULL;
lv_obj_t * scr_dashboard_label_logo_conf_03 = NULL;
lv_obj_t * scr_dashboard_label_logo_inft_03 = NULL;
lv_obj_t * scr_dashboard_label_detail_time_03 = NULL;
lv_obj_t * scr_dashboard_label_mian_conf_03 = NULL;
lv_obj_t * scr_dashboard_label_mian_inft_03 = NULL;
lv_obj_t * scr_dashboard_cont_detail_badge_03 = NULL;
lv_obj_t * scr_dashboard_label_detail_badge_03 = NULL;
lv_obj_t * scr_dashboard_cont_detail_02 = NULL;
lv_obj_t * scr_dashboard_label_detail_result_02 = NULL;
lv_obj_t * scr_dashboard_label_logo_conf_02 = NULL;
lv_obj_t * scr_dashboard_label_mian_conf_02 = NULL;
lv_obj_t * scr_dashboard_label_detail_time_02 = NULL;
lv_obj_t * scr_dashboard_label_logo_inft_02 = NULL;
lv_obj_t * scr_dashboard_label_mian_inft_02 = NULL;
lv_obj_t * scr_dashboard_cont_detail_badge_02 = NULL;
lv_obj_t * scr_dashboard_label_detail_badge_02 = NULL;
lv_obj_t * scr_dashboard_cont_detail_01 = NULL;
lv_obj_t * scr_dashboard_label_detail_result_01 = NULL;
lv_obj_t * scr_dashboard_label_logo_conf_01 = NULL;
lv_obj_t * scr_dashboard_label_mian_conf_01 = NULL;
lv_obj_t * scr_dashboard_label_detail_time_01 = NULL;
lv_obj_t * scr_dashboard_label_logo_inft_01 = NULL;
lv_obj_t * scr_dashboard_label_mian_inft_01 = NULL;
lv_obj_t * scr_dashboard_cont_detail_badge_01 = NULL;
lv_obj_t * scr_dashboard_label_detail_badge_01 = NULL;
lv_obj_t * scr_dashboard_label_title_event = NULL;
lv_obj_t * scr_dashboard_label_title_conf_mian = NULL;
lv_obj_t * scr_dashboard_label_title_result = NULL;
lv_obj_t * scr_dashboard_label_title_conf_logo = NULL;
lv_obj_t * scr_dashboard_label_title_inft_logo = NULL;
lv_obj_t * scr_dashboard_label_title_time = NULL;
lv_obj_t * scr_dashboard_label_title_inft_mian = NULL;
lv_obj_t * scr_dashboard_cont_under2 = NULL;
lv_obj_t * scr_dashboard_cont_log_record = NULL;
lv_obj_t * scr_dashboard_cont_log_page4 = NULL;
lv_obj_t * scr_dashboard_cont_log_20 = NULL;
lv_obj_t * scr_dashboard_label_time_20 = NULL;
lv_obj_t * scr_dashboard_label_company_20 = NULL;
lv_obj_t * scr_dashboard_label_result_20 = NULL;
lv_obj_t * scr_dashboard_label_confidence_20 = NULL;
lv_obj_t * scr_dashboard_cont_log_19 = NULL;
lv_obj_t * scr_dashboard_label_time_19 = NULL;
lv_obj_t * scr_dashboard_label_company_19 = NULL;
lv_obj_t * scr_dashboard_label_result_19 = NULL;
lv_obj_t * scr_dashboard_label_confidence_19 = NULL;
lv_obj_t * scr_dashboard_cont_log_18 = NULL;
lv_obj_t * scr_dashboard_label_time_18 = NULL;
lv_obj_t * scr_dashboard_label_company_18 = NULL;
lv_obj_t * scr_dashboard_label_result_18 = NULL;
lv_obj_t * scr_dashboard_label_confidence_18 = NULL;
lv_obj_t * scr_dashboard_cont_log_17 = NULL;
lv_obj_t * scr_dashboard_label_time_17 = NULL;
lv_obj_t * scr_dashboard_label_company_17 = NULL;
lv_obj_t * scr_dashboard_label_result_17 = NULL;
lv_obj_t * scr_dashboard_label_confidence_17 = NULL;
lv_obj_t * scr_dashboard_cont_log_16 = NULL;
lv_obj_t * scr_dashboard_label_time_16 = NULL;
lv_obj_t * scr_dashboard_label_company_16 = NULL;
lv_obj_t * scr_dashboard_label_result_16 = NULL;
lv_obj_t * scr_dashboard_label_confidence_16 = NULL;
lv_obj_t * scr_dashboard_cont_log_page3 = NULL;
lv_obj_t * scr_dashboard_cont_log_15 = NULL;
lv_obj_t * scr_dashboard_label_time_15 = NULL;
lv_obj_t * scr_dashboard_label_company_15 = NULL;
lv_obj_t * scr_dashboard_label_result_15 = NULL;
lv_obj_t * scr_dashboard_label_confidence_15 = NULL;
lv_obj_t * scr_dashboard_cont_log_14 = NULL;
lv_obj_t * scr_dashboard_label_time_14 = NULL;
lv_obj_t * scr_dashboard_label_company_14 = NULL;
lv_obj_t * scr_dashboard_label_result_14 = NULL;
lv_obj_t * scr_dashboard_label_confidence_14 = NULL;
lv_obj_t * scr_dashboard_cont_log_13 = NULL;
lv_obj_t * scr_dashboard_label_time_13 = NULL;
lv_obj_t * scr_dashboard_label_company_13 = NULL;
lv_obj_t * scr_dashboard_label_result_13 = NULL;
lv_obj_t * scr_dashboard_label_confidence_13 = NULL;
lv_obj_t * scr_dashboard_cont_log_12 = NULL;
lv_obj_t * scr_dashboard_label_time_12 = NULL;
lv_obj_t * scr_dashboard_label_company_12 = NULL;
lv_obj_t * scr_dashboard_label_result_12 = NULL;
lv_obj_t * scr_dashboard_label_confidence_12 = NULL;
lv_obj_t * scr_dashboard_cont_log_11 = NULL;
lv_obj_t * scr_dashboard_label_time_11 = NULL;
lv_obj_t * scr_dashboard_label_company_11 = NULL;
lv_obj_t * scr_dashboard_label_result_11 = NULL;
lv_obj_t * scr_dashboard_label_confidence_11 = NULL;
lv_obj_t * scr_dashboard_cont_log_page2 = NULL;
lv_obj_t * scr_dashboard_cont_log_10 = NULL;
lv_obj_t * scr_dashboard_label_time_10 = NULL;
lv_obj_t * scr_dashboard_label_company_10 = NULL;
lv_obj_t * scr_dashboard_label_result_10 = NULL;
lv_obj_t * scr_dashboard_label_confidence_10 = NULL;
lv_obj_t * scr_dashboard_cont_log_09 = NULL;
lv_obj_t * scr_dashboard_label_time_09 = NULL;
lv_obj_t * scr_dashboard_label_company_09 = NULL;
lv_obj_t * scr_dashboard_label_result_09 = NULL;
lv_obj_t * scr_dashboard_label_confidence_09 = NULL;
lv_obj_t * scr_dashboard_cont_log_08 = NULL;
lv_obj_t * scr_dashboard_label_time_08 = NULL;
lv_obj_t * scr_dashboard_label_company_08 = NULL;
lv_obj_t * scr_dashboard_label_result_08 = NULL;
lv_obj_t * scr_dashboard_label_confidence_08 = NULL;
lv_obj_t * scr_dashboard_cont_log_07 = NULL;
lv_obj_t * scr_dashboard_label_time_07 = NULL;
lv_obj_t * scr_dashboard_label_company_07 = NULL;
lv_obj_t * scr_dashboard_label_result_07 = NULL;
lv_obj_t * scr_dashboard_label_confidence_07 = NULL;
lv_obj_t * scr_dashboard_cont_log_06 = NULL;
lv_obj_t * scr_dashboard_label_time_06 = NULL;
lv_obj_t * scr_dashboard_label_company_06 = NULL;
lv_obj_t * scr_dashboard_label_result_06 = NULL;
lv_obj_t * scr_dashboard_label_confidence_06 = NULL;
lv_obj_t * scr_dashboard_cont_log_page1 = NULL;
lv_obj_t * scr_dashboard_cont_log_05 = NULL;
lv_obj_t * scr_dashboard_label_time_05 = NULL;
lv_obj_t * scr_dashboard_label_company_05 = NULL;
lv_obj_t * scr_dashboard_label_result_05 = NULL;
lv_obj_t * scr_dashboard_label_confidence_05 = NULL;
lv_obj_t * scr_dashboard_cont_log_04 = NULL;
lv_obj_t * scr_dashboard_label_time_04 = NULL;
lv_obj_t * scr_dashboard_label_company_04 = NULL;
lv_obj_t * scr_dashboard_label_result_04 = NULL;
lv_obj_t * scr_dashboard_label_confidence_04 = NULL;
lv_obj_t * scr_dashboard_cont_log_03 = NULL;
lv_obj_t * scr_dashboard_label_time_03 = NULL;
lv_obj_t * scr_dashboard_label_company_03 = NULL;
lv_obj_t * scr_dashboard_label_result_03 = NULL;
lv_obj_t * scr_dashboard_label_confidence_03 = NULL;
lv_obj_t * scr_dashboard_cont_log_02 = NULL;
lv_obj_t * scr_dashboard_label_time_02 = NULL;
lv_obj_t * scr_dashboard_label_company_02 = NULL;
lv_obj_t * scr_dashboard_label_result_02 = NULL;
lv_obj_t * scr_dashboard_label_confidence_02 = NULL;
lv_obj_t * scr_dashboard_cont_log_01 = NULL;
lv_obj_t * scr_dashboard_label_time_01 = NULL;
lv_obj_t * scr_dashboard_label_company_01 = NULL;
lv_obj_t * scr_dashboard_label_result_01 = NULL;
lv_obj_t * scr_dashboard_label_confidence_01 = NULL;
lv_obj_t * scr_dashboard_label_text_log = NULL;
lv_obj_t * scr_dashboard_cont_under1 = NULL;
lv_obj_t * scr_dashboard_label_current_page = NULL;
lv_obj_t * scr_dashboard_imgbtn_page_back = NULL;
lv_obj_t * scr_dashboard_imgbtn_page_back_label = NULL;
lv_obj_t * scr_dashboard_imgbtn_page_next = NULL;
lv_obj_t * scr_dashboard_imgbtn_page_next_label = NULL;
lv_obj_t * scr_dashboard_cont_set = NULL;
lv_obj_t * scr_dashboard_cont_set_line = NULL;
lv_obj_t * scr_dashboard_imgbtn_net = NULL;
lv_obj_t * scr_dashboard_imgbtn_net_label = NULL;
lv_obj_t * scr_dashboard_imgbtn_control = NULL;
lv_obj_t * scr_dashboard_imgbtn_control_label = NULL;
lv_obj_t * scr_dashboard_imgbtn_about = NULL;
lv_obj_t * scr_dashboard_imgbtn_about_label = NULL;
lv_obj_t * scr_dashboard_imgbtn_detect = NULL;
lv_obj_t * scr_dashboard_imgbtn_detect_label = NULL;
lv_obj_t * scr_dashboard_imgbtn_display = NULL;
lv_obj_t * scr_dashboard_imgbtn_display_label = NULL;
lv_obj_t * scr_dashboard_cont_about = NULL;
lv_obj_t * scr_dashboard_cont_about_info = NULL;
lv_obj_t * scr_dashboard_label_runtime = NULL;
lv_obj_t * scr_dashboard_label_memory = NULL;
lv_obj_t * scr_dashboard_label_about_runtime_value = NULL;
lv_obj_t * scr_dashboard_label_model_info = NULL;
lv_obj_t * scr_dashboard_label_memory_value = NULL;
lv_obj_t * scr_dashboard_label_model_info_value = NULL;
lv_obj_t * scr_dashboard_cont_about_reset = NULL;
lv_obj_t * scr_dashboard_label_factory_reset = NULL;
lv_obj_t * scr_dashboard_btn_factory_reset = NULL;
lv_obj_t * scr_dashboard_btn_factory_reset_label = NULL;
lv_obj_t * scr_dashboard_cont_about_restart = NULL;
lv_obj_t * scr_dashboard_label_restart = NULL;
lv_obj_t * scr_dashboard_btn_restart = NULL;
lv_obj_t * scr_dashboard_btn_restart_label = NULL;
lv_obj_t * scr_dashboard_cont_net = NULL;
lv_obj_t * scr_dashboard_cont_net_ip = NULL;
lv_obj_t * scr_dashboard_label_local_ip = NULL;
lv_obj_t * scr_dashboard_label_host_ip = NULL;
lv_obj_t * scr_dashboard_label_local_ip_value = NULL;
lv_obj_t * scr_dashboard_label_host_ip_value = NULL;
lv_obj_t * scr_dashboard_cont_net_interval = NULL;
lv_obj_t * scr_dashboard_slider_report_interval = NULL;
lv_obj_t * scr_dashboard_label_report_interval = NULL;
lv_obj_t * scr_dashboard_label_report_interval_value = NULL;
lv_obj_t * scr_dashboard_cont_net_image = NULL;
lv_obj_t * scr_dashboard_label_report_image = NULL;
lv_obj_t * scr_dashboard_sw_report_image = NULL;
lv_obj_t * scr_dashboard_cont_net_metrics = NULL;
lv_obj_t * scr_dashboard_label_report_metrics = NULL;
lv_obj_t * scr_dashboard_sw_report_metrics = NULL;
lv_obj_t * scr_dashboard_cont_display = NULL;
lv_obj_t * scr_dashboard_cont_bright = NULL;
lv_obj_t * scr_dashboard_label_text_bright = NULL;
lv_obj_t * scr_dashboard_label_runtime_bright__data = NULL;
lv_obj_t * scr_dashboard_slider_82EIlsYJ = NULL;
lv_obj_t * scr_dashboard_cont_control = NULL;
lv_obj_t * scr_dashboard_cont_control_pid = NULL;
lv_obj_t * scr_dashboard_label_pid_control = NULL;
lv_obj_t * scr_dashboard_btn_openloop_mode = NULL;
lv_obj_t * scr_dashboard_btn_openloop_mode_label = NULL;
lv_obj_t * scr_dashboard_btn_pid_mode = NULL;
lv_obj_t * scr_dashboard_btn_pid_mode_label = NULL;
lv_obj_t * scr_dashboard_cont_control_openloop = NULL;
lv_obj_t * scr_dashboard_slider_openloop_speed = NULL;
lv_obj_t * scr_dashboard_label_openloop_speed = NULL;
lv_obj_t * scr_dashboard_cont_control_pid_speed = NULL;
lv_obj_t * scr_dashboard_label_pid_speed = NULL;
lv_obj_t * scr_dashboard_btn_pid_speed_20 = NULL;
lv_obj_t * scr_dashboard_btn_pid_speed_20_label = NULL;
lv_obj_t * scr_dashboard_btn_pid_speed_30 = NULL;
lv_obj_t * scr_dashboard_btn_pid_speed_30_label = NULL;
lv_obj_t * scr_dashboard_btn_pid_speed_40 = NULL;
lv_obj_t * scr_dashboard_btn_pid_speed_40_label = NULL;
lv_obj_t * scr_dashboard_btn_pid_speed_50 = NULL;
lv_obj_t * scr_dashboard_btn_pid_speed_50_label = NULL;
lv_obj_t * scr_dashboard_cont_detect = NULL;
lv_obj_t * scr_dashboard_cont_detect_confidence = NULL;
lv_obj_t * scr_dashboard_slider_confidence_threshold_mian = NULL;
lv_obj_t * scr_dashboard_label_confidence_value_mian = NULL;
lv_obj_t * scr_dashboard_slider_confidence_threshold_logo = NULL;
lv_obj_t * scr_dashboard_label_confidence_value_logo = NULL;
lv_obj_t * scr_dashboard_label_confidence_threshold = NULL;
lv_obj_t * scr_dashboard_label_text_mian = NULL;
lv_obj_t * scr_dashboard_label_text_logo = NULL;
lv_obj_t * scr_dashboard_cont_detect_switch = NULL;
lv_obj_t * scr_dashboard_label_detect_switch = NULL;
lv_obj_t * scr_dashboard_sw_detect = NULL;
lv_obj_t * scr_dashboard_cont_detect_overlay = NULL;
lv_obj_t * scr_dashboard_label_preview_overlay = NULL;
lv_obj_t * scr_dashboard_sw_preview_overlay = NULL;
lv_obj_t * scr_dashboard_cont_button = NULL;
lv_obj_t * scr_dashboard_imgbtn_dash = NULL;
lv_obj_t * scr_dashboard_imgbtn_dash_label = NULL;
lv_obj_t * scr_dashboard_imgbtn_set = NULL;
lv_obj_t * scr_dashboard_imgbtn_set_label = NULL;
lv_obj_t * scr_dashboard_imgbtn_log = NULL;
lv_obj_t * scr_dashboard_imgbtn_log_label = NULL;
lv_obj_t * scr_dashboard_imgbtn_sys = NULL;
lv_obj_t * scr_dashboard_imgbtn_sys_label = NULL;
lv_obj_t * scr_dashboard_cont_dashboard = NULL;
lv_obj_t * scr_dashboard_cont_container_live = NULL;
lv_obj_t * scr_dashboard_cont_live_vedio = NULL;
lv_obj_t * scr_dashboard_label_text_live = NULL;
lv_obj_t * scr_dashboard_cont_detection = NULL;
lv_obj_t * scr_dashboard_slider_runtime_confidence_mian = NULL;
lv_obj_t * scr_dashboard_slider_runtime_confidence_logo = NULL;
lv_obj_t * scr_dashboard_label_runtime_confidence_mian = NULL;
lv_obj_t * scr_dashboard_label_runtime_confidence_logo = NULL;
lv_obj_t * scr_dashboard_label_runtime_recognition_fps = NULL;
lv_obj_t * scr_dashboard_label_runtime_recognition_status = NULL;
lv_obj_t * scr_dashboard_label_text_mian_confidence = NULL;
lv_obj_t * scr_dashboard_label_text_logo_confidence = NULL;
lv_obj_t * scr_dashboard_label_text_detection = NULL;
lv_obj_t * scr_dashboard_label_text_fps = NULL;
lv_obj_t * scr_dashboard_cont_data = NULL;
lv_obj_t * scr_dashboard_cont_01 = NULL;
lv_obj_t * scr_dashboard_label_ms = NULL;
lv_obj_t * scr_dashboard_label_runtime_infer_time = NULL;
lv_obj_t * scr_dashboard_label_text01 = NULL;
lv_obj_t * scr_dashboard_cont_03 = NULL;
lv_obj_t * scr_dashboard_label_runtime_confidence_threshole_mian = NULL;
lv_obj_t * scr_dashboard_label_runtime_confidence_threshole_logo = NULL;
lv_obj_t * scr_dashboard_label_mian = NULL;
lv_obj_t * scr_dashboard_labe_logo = NULL;
lv_obj_t * scr_dashboard_label_text03 = NULL;
lv_obj_t * scr_dashboard_cont_04 = NULL;
lv_obj_t * scr_dashboard_label_YD = NULL;
lv_obj_t * scr_dashboard_label_ZT = NULL;
lv_obj_t * scr_dashboard_label_JT = NULL;
lv_obj_t * scr_dashboard_label_init_express_company = NULL;
lv_obj_t * scr_dashboard_label_text04 = NULL;
lv_obj_t * scr_dashboard_cont_live_data = NULL;
lv_obj_t * scr_dashboard_label_text_JT = NULL;
lv_obj_t * scr_dashboard_label_text_ZT = NULL;
lv_obj_t * scr_dashboard_label_JT_A = NULL;
lv_obj_t * scr_dashboard_label_text_YD = NULL;
lv_obj_t * scr_dashboard_label_ZT_A = NULL;
lv_obj_t * scr_dashboard_label_YD_A = NULL;
lv_obj_t * scr_dashboard_slider_jt = NULL;
lv_obj_t * scr_dashboard_slider_zt = NULL;
lv_obj_t * scr_dashboard_slider_yd = NULL;
lv_obj_t * scr_dashboard_cont_circle_data = NULL;
lv_obj_t * scr_dashboard_cont_cpu_02 = NULL;
lv_obj_t * scr_dashboard_label_text_cpu_2 = NULL;
lv_obj_t * scr_dashboard_label_runtime_cpu_core2_usage = NULL;
lv_obj_t * scr_dashboard_cont_cpu_01 = NULL;
lv_obj_t * scr_dashboard_label_runtime_cpu_core1_usage = NULL;
lv_obj_t * scr_dashboard_label_text_cpu_1 = NULL;
lv_obj_t * scr_dashboard_cont_esp32 = NULL;
lv_obj_t * scr_dashboard_label_esp32 = NULL;
lv_obj_t * scr_dashboard_label_title_A = NULL;
lv_obj_t * scr_dashboard_img_runtime_ethernet_status = NULL;
lv_obj_t * scr_dashboard_img_runtime_ethernet_status_label = NULL;
lv_obj_t * scr_dashboard_imgbtn_logo = NULL;
lv_obj_t * scr_dashboard_imgbtn_logo_label = NULL;
static event_table_t scr_dashboard_event_table = {0};
static lv_obj_t * scr_dashboard_arc_system_cpu_03 = NULL;
static lv_obj_t * scr_dashboard_arc_system_cpu_03_label = NULL;
static lv_obj_t * scr_dashboard_arc_system_cpu_total = NULL;
static lv_obj_t * scr_dashboard_arc_system_cpu_total_label = NULL;
static lv_obj_t * scr_dashboard_arc_system_cpu_02 = NULL;
static lv_obj_t * scr_dashboard_arc_system_cpu_02_label = NULL;
static lv_obj_t * scr_dashboard_arc_runtime_cpu_core2_usage = NULL;
static lv_obj_t * scr_dashboard_arc_runtime_cpu_core2_usage_label = NULL;
static lv_obj_t * scr_dashboard_arc_runtime_cpu_core1_usage = NULL;
static lv_obj_t * scr_dashboard_arc_runtime_cpu_core1_usage_label = NULL;
static void register_sys_events(event_table_t *table);
static void init_states(void);
static void scr_dashboard_imgbtn_sys_event_handler(lv_event_t * e);
static void scr_dashboard_imgbtn_log_event_handler(lv_event_t * e);
static void scr_dashboard_imgbtn_set_event_handler(lv_event_t * e);
static void scr_dashboard_imgbtn_dash_event_handler(lv_event_t * e);
static void scr_dashboard_btn_pid_speed_50_event_handler(lv_event_t * e);
static void scr_dashboard_btn_pid_speed_40_event_handler(lv_event_t * e);
static void scr_dashboard_btn_pid_speed_30_event_handler(lv_event_t * e);
static void scr_dashboard_btn_pid_speed_20_event_handler(lv_event_t * e);
static void scr_dashboard_btn_pid_mode_event_handler(lv_event_t * e);
static void scr_dashboard_btn_openloop_mode_event_handler(lv_event_t * e);
static void scr_dashboard_imgbtn_display_event_handler(lv_event_t * e);
static void scr_dashboard_imgbtn_detect_event_handler(lv_event_t * e);
static void scr_dashboard_imgbtn_about_event_handler(lv_event_t * e);
static void scr_dashboard_imgbtn_control_event_handler(lv_event_t * e);
static void scr_dashboard_imgbtn_net_event_handler(lv_event_t * e);
static void scr_dashboard_cont_log_01_event_handler(lv_event_t * e);
static void scr_dashboard_cont_log_02_event_handler(lv_event_t * e);
static void scr_dashboard_cont_log_03_event_handler(lv_event_t * e);
static void scr_dashboard_cont_log_04_event_handler(lv_event_t * e);
static void scr_dashboard_cont_log_05_event_handler(lv_event_t * e);
static void scr_dashboard_cont_log_06_event_handler(lv_event_t * e);
static void scr_dashboard_cont_log_07_event_handler(lv_event_t * e);
static void scr_dashboard_cont_log_08_event_handler(lv_event_t * e);
static void scr_dashboard_cont_log_09_event_handler(lv_event_t * e);
static void scr_dashboard_cont_log_10_event_handler(lv_event_t * e);
static void scr_dashboard_cont_log_11_event_handler(lv_event_t * e);
static void scr_dashboard_cont_log_12_event_handler(lv_event_t * e);
static void scr_dashboard_cont_log_13_event_handler(lv_event_t * e);
static void scr_dashboard_cont_log_14_event_handler(lv_event_t * e);
static void scr_dashboard_cont_log_15_event_handler(lv_event_t * e);
static void scr_dashboard_cont_log_16_event_handler(lv_event_t * e);
static void scr_dashboard_cont_log_17_event_handler(lv_event_t * e);
static void scr_dashboard_cont_log_18_event_handler(lv_event_t * e);
static void scr_dashboard_cont_log_19_event_handler(lv_event_t * e);
static void scr_dashboard_cont_log_20_event_handler(lv_event_t * e);
static void register_ui_events(void);
static lv_obj_t * create_ui(void);


static void register_sys_events(event_table_t *table) {
}
static void init_states(void) {
    extern lv_obj_t * global_statusbar;
    set_current_event_table(&scr_dashboard_event_table);
}
static void scr_dashboard_imgbtn_sys_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED: {
        ui_flag_modify(scr_dashboard_cont_dashboard, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_set, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_log, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_sys, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &sys_a_212x66, NULL);
        ui_state_modify(scr_dashboard_imgbtn_sys, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_log, LV_IMGBTN_STATE_RELEASED, NULL, &logc_212x66, NULL);
        ui_state_modify(scr_dashboard_imgbtn_log, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_set, LV_IMGBTN_STATE_RELEASED, NULL, &setc_212x66, NULL);
        ui_state_modify(scr_dashboard_imgbtn_set, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_dash, LV_IMGBTN_STATE_RELEASED, NULL, &dash_c_212x66, NULL);
        ui_state_modify(scr_dashboard_imgbtn_dash, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_system, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_REMOVE);
        break;
    }
    default: {
        break;
    }
    }
}
static void scr_dashboard_imgbtn_log_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED: {
        ui_flag_modify(scr_dashboard_cont_log, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_set, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_dashboard, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_sys, LV_IMGBTN_STATE_RELEASED, NULL, &sysc_212x66, NULL);
        ui_state_modify(scr_dashboard_imgbtn_sys, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_log, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &log_a_212x66, NULL);
        ui_state_modify(scr_dashboard_imgbtn_log, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_set, LV_IMGBTN_STATE_RELEASED, NULL, &setc_212x66, NULL);
        ui_state_modify(scr_dashboard_imgbtn_set, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_dash, LV_IMGBTN_STATE_RELEASED, NULL, &dash_c_212x66, NULL);
        ui_state_modify(scr_dashboard_imgbtn_dash, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_system, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        break;
    }
    default: {
        break;
    }
    }
}
static void scr_dashboard_imgbtn_set_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED: {
        ui_flag_modify(scr_dashboard_cont_log, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_set, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_dashboard, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_sys, LV_IMGBTN_STATE_RELEASED, NULL, &sysc_212x66, NULL);
        ui_state_modify(scr_dashboard_imgbtn_sys, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_log, LV_IMGBTN_STATE_RELEASED, NULL, &logc_212x66, NULL);
        ui_state_modify(scr_dashboard_imgbtn_log, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_set, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &set_a_212x66, NULL);
        ui_state_modify(scr_dashboard_imgbtn_set, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_dash, LV_IMGBTN_STATE_RELEASED, NULL, &dash_c_212x66, NULL);
        ui_state_modify(scr_dashboard_imgbtn_dash, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_system, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        break;
    }
    default: {
        break;
    }
    }
}
static void scr_dashboard_imgbtn_dash_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED: {
        ui_flag_modify(scr_dashboard_cont_dashboard, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_set, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_log, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_dash, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &dash_a_212x66, NULL);
        ui_state_modify(scr_dashboard_imgbtn_dash, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_sys, LV_IMGBTN_STATE_RELEASED, NULL, &sysc_212x66, NULL);
        ui_state_modify(scr_dashboard_imgbtn_sys, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_log, LV_IMGBTN_STATE_RELEASED, NULL, &logc_212x66, NULL);
        ui_state_modify(scr_dashboard_imgbtn_log, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_set, LV_IMGBTN_STATE_RELEASED, NULL, &setc_212x66, NULL);
        ui_state_modify(scr_dashboard_imgbtn_set, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_system, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        break;
    }
    default: {
        break;
    }
    }
}
static void scr_dashboard_btn_pid_speed_50_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED: {
        ui_state_modify(scr_dashboard_btn_pid_speed_50, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
        ui_state_modify(scr_dashboard_btn_pid_speed_40, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_btn_pid_speed_30, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_btn_pid_speed_20, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        break;
    }
    default: {
        break;
    }
    }
}
static void scr_dashboard_btn_pid_speed_40_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED: {
        ui_state_modify(scr_dashboard_btn_pid_speed_50, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_btn_pid_speed_40, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
        ui_state_modify(scr_dashboard_btn_pid_speed_30, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_btn_pid_speed_20, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        break;
    }
    default: {
        break;
    }
    }
}
static void scr_dashboard_btn_pid_speed_30_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED: {
        ui_state_modify(scr_dashboard_btn_pid_speed_50, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_btn_pid_speed_40, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_btn_pid_speed_30, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
        ui_state_modify(scr_dashboard_btn_pid_speed_20, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        break;
    }
    default: {
        break;
    }
    }
}
static void scr_dashboard_btn_pid_speed_20_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED: {
        ui_state_modify(scr_dashboard_btn_pid_speed_50, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_btn_pid_speed_40, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_btn_pid_speed_30, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_btn_pid_speed_20, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
        break;
    }
    default: {
        break;
    }
    }
}
static void scr_dashboard_btn_pid_mode_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED: {
        ui_flag_modify(scr_dashboard_cont_control_pid_speed, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_control_openloop, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_state_modify(scr_dashboard_btn_pid_mode, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
        ui_state_modify(scr_dashboard_btn_openloop_mode, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        break;
    }
    default: {
        break;
    }
    }
}
static void scr_dashboard_btn_openloop_mode_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED: {
        ui_flag_modify(scr_dashboard_cont_control_pid_speed, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_control_openloop, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_btn_pid_mode, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_btn_openloop_mode, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
        ui_state_modify(scr_dashboard_btn_pid_speed_50, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_btn_pid_speed_40, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_btn_pid_speed_30, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_btn_pid_speed_20, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        break;
    }
    default: {
        break;
    }
    }
}
static void scr_dashboard_imgbtn_display_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED: {
        ui_flag_modify(scr_dashboard_cont_display, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_detect, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_control, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_net, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_about, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_display, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &display_b_188x54, NULL);
        ui_state_modify(scr_dashboard_imgbtn_display, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_detect, LV_IMGBTN_STATE_RELEASED, NULL, &detect_188x54, NULL);
        ui_state_modify(scr_dashboard_imgbtn_detect, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_control, LV_IMGBTN_STATE_RELEASED, NULL, &control_188x54, NULL);
        ui_state_modify(scr_dashboard_imgbtn_control, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_net, LV_IMGBTN_STATE_RELEASED, NULL, &net_188x54, NULL);
        ui_state_modify(scr_dashboard_imgbtn_net, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_about, LV_IMGBTN_STATE_RELEASED, NULL, &about_188x54, NULL);
        ui_state_modify(scr_dashboard_imgbtn_about, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        break;
    }
    default: {
        break;
    }
    }
}
static void scr_dashboard_imgbtn_detect_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED: {
        ui_flag_modify(scr_dashboard_cont_detect, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_control, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_net, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_about, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_detect, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &detect_b_188x54, NULL);
        ui_state_modify(scr_dashboard_imgbtn_detect, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_control, LV_IMGBTN_STATE_RELEASED, NULL, &control_188x54, NULL);
        ui_state_modify(scr_dashboard_imgbtn_control, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_net, LV_IMGBTN_STATE_RELEASED, NULL, &net_188x54, NULL);
        ui_state_modify(scr_dashboard_imgbtn_net, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_about, LV_IMGBTN_STATE_RELEASED, NULL, &about_188x54, NULL);
        ui_state_modify(scr_dashboard_imgbtn_about, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_display, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_display, LV_IMGBTN_STATE_RELEASED, NULL, &display_188x54, NULL);
        ui_state_modify(scr_dashboard_imgbtn_display, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        break;
    }
    default: {
        break;
    }
    }
}
static void scr_dashboard_imgbtn_about_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED: {
        ui_flag_modify(scr_dashboard_cont_about, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_detect, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_control, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_net, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_about, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &about_b_188x54, NULL);
        ui_state_modify(scr_dashboard_imgbtn_about, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_detect, LV_IMGBTN_STATE_RELEASED, NULL, &detect_188x54, NULL);
        ui_state_modify(scr_dashboard_imgbtn_detect, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_control, LV_IMGBTN_STATE_RELEASED, NULL, &control_188x54, NULL);
        ui_state_modify(scr_dashboard_imgbtn_control, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_net, LV_IMGBTN_STATE_RELEASED, NULL, &net_188x54, NULL);
        ui_state_modify(scr_dashboard_imgbtn_net, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_display, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_display, LV_IMGBTN_STATE_RELEASED, NULL, &display_188x54, NULL);
        ui_state_modify(scr_dashboard_imgbtn_display, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        break;
    }
    default: {
        break;
    }
    }
}
static void scr_dashboard_imgbtn_control_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED: {
        ui_flag_modify(scr_dashboard_cont_control, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_detect, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_net, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_about, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_control, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &control_b_188x54, NULL);
        ui_state_modify(scr_dashboard_imgbtn_control, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_detect, LV_IMGBTN_STATE_RELEASED, NULL, &detect_188x54, NULL);
        ui_state_modify(scr_dashboard_imgbtn_detect, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_net, LV_IMGBTN_STATE_RELEASED, NULL, &net_188x54, NULL);
        ui_state_modify(scr_dashboard_imgbtn_net, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_about, LV_IMGBTN_STATE_RELEASED, NULL, &about_188x54, NULL);
        ui_state_modify(scr_dashboard_imgbtn_about, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_display, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_display, LV_IMGBTN_STATE_RELEASED, NULL, &display_188x54, NULL);
        ui_state_modify(scr_dashboard_imgbtn_display, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        break;
    }
    default: {
        break;
    }
    }
}
static void scr_dashboard_imgbtn_net_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED: {
        ui_flag_modify(scr_dashboard_cont_net, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_detect, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_control, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_about, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_net, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &net_b_188x54, NULL);
        ui_state_modify(scr_dashboard_imgbtn_net, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_detect, LV_IMGBTN_STATE_RELEASED, NULL, &detect_188x54, NULL);
        ui_state_modify(scr_dashboard_imgbtn_detect, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_control, LV_IMGBTN_STATE_RELEASED, NULL, &control_188x54, NULL);
        ui_state_modify(scr_dashboard_imgbtn_control, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_about, LV_IMGBTN_STATE_RELEASED, NULL, &about_188x54, NULL);
        ui_state_modify(scr_dashboard_imgbtn_about, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        lv_imgbtn_set_src(scr_dashboard_imgbtn_display, LV_IMGBTN_STATE_RELEASED, NULL, &display_188x54, NULL);
        ui_state_modify(scr_dashboard_imgbtn_display, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_display, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        break;
    }
    default: {
        break;
    }
    }
}
static void scr_dashboard_cont_log_01_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED: {
        ui_state_modify(scr_dashboard_cont_log_01, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
        ui_state_modify(scr_dashboard_cont_log_02, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_03, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_04, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_05, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_06, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_07, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_08, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_09, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_10, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_11, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_12, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_13, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_14, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_15, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_16, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_17, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_18, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_19, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_20, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_detail_01, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_detail_02, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_03, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_04, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_05, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_06, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_07, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_08, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_09, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_10, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_11, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_12, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_13, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_14, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_15, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_16, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_17, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_18, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_19, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_20, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        break;
    }
    default: {
        break;
    }
    }
}
static void scr_dashboard_cont_log_02_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED: {
        ui_state_modify(scr_dashboard_cont_log_02, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
        ui_state_modify(scr_dashboard_cont_log_01, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_03, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_04, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_05, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_06, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_07, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_08, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_09, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_10, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_11, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_12, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_13, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_14, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_15, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_16, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_17, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_18, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_19, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_20, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_detail_02, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_detail_01, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_03, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_04, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_05, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_06, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_07, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_08, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_09, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_10, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_11, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_12, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_13, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_14, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_15, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_16, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_17, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_18, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_19, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_20, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        break;
    }
    default: {
        break;
    }
    }
}
static void scr_dashboard_cont_log_03_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED: {
        ui_state_modify(scr_dashboard_cont_log_03, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
        ui_state_modify(scr_dashboard_cont_log_01, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_02, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_04, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_05, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_06, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_07, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_08, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_09, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_10, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_11, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_12, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_13, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_14, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_15, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_16, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_17, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_18, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_19, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_20, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_detail_03, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_detail_01, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_02, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_04, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_05, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_06, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_07, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_08, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_09, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_10, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_11, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_12, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_13, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_14, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_15, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_16, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_17, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_18, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_19, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_20, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        break;
    }
    default: {
        break;
    }
    }
}
static void scr_dashboard_cont_log_04_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED: {
        ui_state_modify(scr_dashboard_cont_log_04, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
        ui_state_modify(scr_dashboard_cont_log_01, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_02, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_03, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_05, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_06, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_07, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_08, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_09, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_10, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_11, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_12, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_13, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_14, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_15, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_16, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_17, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_18, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_19, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_20, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_detail_04, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_detail_01, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_02, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_03, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_05, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_06, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_07, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_08, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_09, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_10, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_11, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_12, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_13, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_14, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_15, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_16, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_17, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_18, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_19, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_20, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        break;
    }
    default: {
        break;
    }
    }
}
static void scr_dashboard_cont_log_05_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED: {
        ui_state_modify(scr_dashboard_cont_log_05, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
        ui_state_modify(scr_dashboard_cont_log_01, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_02, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_03, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_04, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_06, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_07, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_08, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_09, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_10, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_11, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_12, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_13, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_14, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_15, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_16, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_17, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_18, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_19, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_20, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_detail_05, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_detail_01, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_02, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_03, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_04, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_06, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_07, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_08, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_09, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_10, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_11, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_12, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_13, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_14, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_15, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_16, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_17, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_18, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_19, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_20, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        break;
    }
    default: {
        break;
    }
    }
}
static void scr_dashboard_cont_log_06_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED: {
        ui_state_modify(scr_dashboard_cont_log_06, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
        ui_state_modify(scr_dashboard_cont_log_01, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_02, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_03, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_04, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_05, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_07, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_08, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_09, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_10, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_11, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_12, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_13, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_14, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_15, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_16, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_17, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_18, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_19, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_20, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_detail_06, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_detail_01, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_02, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_03, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_04, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_05, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_07, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_08, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_09, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_10, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_11, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_12, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_13, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_14, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_15, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_16, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_17, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_18, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_19, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_20, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        break;
    }
    default: {
        break;
    }
    }
}
static void scr_dashboard_cont_log_07_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED: {
        ui_state_modify(scr_dashboard_cont_log_07, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
        ui_state_modify(scr_dashboard_cont_log_01, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_02, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_03, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_04, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_05, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_06, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_08, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_09, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_10, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_11, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_12, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_13, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_14, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_15, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_16, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_17, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_18, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_19, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_20, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_detail_07, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_detail_01, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_02, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_03, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_04, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_05, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_06, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_08, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_09, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_10, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_11, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_12, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_13, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_14, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_15, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_16, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_17, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_18, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_19, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_20, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        break;
    }
    default: {
        break;
    }
    }
}
static void scr_dashboard_cont_log_08_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED: {
        ui_state_modify(scr_dashboard_cont_log_08, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
        ui_state_modify(scr_dashboard_cont_log_01, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_02, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_03, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_04, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_05, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_06, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_07, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_09, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_10, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_11, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_12, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_13, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_14, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_15, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_16, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_17, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_18, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_19, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_20, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_detail_08, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_detail_01, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_02, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_03, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_04, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_05, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_06, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_07, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_09, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_10, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_11, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_12, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_13, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_14, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_15, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_16, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_17, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_18, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_19, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_20, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        break;
    }
    default: {
        break;
    }
    }
}
static void scr_dashboard_cont_log_09_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED: {
        ui_state_modify(scr_dashboard_cont_log_09, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
        ui_state_modify(scr_dashboard_cont_log_01, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_02, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_03, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_04, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_05, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_06, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_07, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_08, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_10, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_11, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_12, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_13, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_14, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_15, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_16, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_17, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_18, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_19, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_20, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_detail_09, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_detail_01, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_02, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_03, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_04, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_05, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_06, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_07, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_08, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_10, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_11, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_12, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_13, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_14, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_15, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_16, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_17, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_18, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_19, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_20, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        break;
    }
    default: {
        break;
    }
    }
}
static void scr_dashboard_cont_log_10_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED: {
        ui_state_modify(scr_dashboard_cont_log_10, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
        ui_state_modify(scr_dashboard_cont_log_01, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_02, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_03, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_04, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_05, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_06, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_07, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_08, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_09, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_11, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_12, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_13, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_14, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_15, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_16, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_17, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_18, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_19, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_20, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_detail_10, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_detail_01, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_02, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_03, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_04, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_05, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_06, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_07, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_08, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_09, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_11, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_12, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_13, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_14, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_15, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_16, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_17, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_18, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_19, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_20, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        break;
    }
    default: {
        break;
    }
    }
}
static void scr_dashboard_cont_log_11_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED: {
        ui_state_modify(scr_dashboard_cont_log_11, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
        ui_state_modify(scr_dashboard_cont_log_01, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_02, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_03, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_04, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_05, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_06, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_07, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_08, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_09, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_10, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_12, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_13, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_14, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_15, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_16, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_17, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_18, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_19, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_20, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_detail_11, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_detail_01, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_02, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_03, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_04, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_05, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_06, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_07, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_08, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_09, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_10, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_12, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_13, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_14, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_15, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_16, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_17, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_18, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_19, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_20, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        break;
    }
    default: {
        break;
    }
    }
}
static void scr_dashboard_cont_log_12_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED: {
        ui_state_modify(scr_dashboard_cont_log_12, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
        ui_state_modify(scr_dashboard_cont_log_01, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_02, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_03, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_04, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_05, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_06, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_07, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_08, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_09, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_10, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_11, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_13, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_14, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_15, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_16, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_17, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_18, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_19, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_20, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_detail_12, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_detail_01, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_02, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_03, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_04, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_05, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_06, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_07, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_08, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_09, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_10, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_11, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_13, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_14, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_15, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_16, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_17, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_18, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_19, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_20, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        break;
    }
    default: {
        break;
    }
    }
}
static void scr_dashboard_cont_log_13_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED: {
        ui_state_modify(scr_dashboard_cont_log_13, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
        ui_state_modify(scr_dashboard_cont_log_01, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_02, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_03, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_04, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_05, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_06, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_07, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_08, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_09, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_10, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_11, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_12, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_14, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_15, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_16, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_17, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_18, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_19, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_20, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_detail_13, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_detail_01, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_02, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_03, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_04, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_05, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_06, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_07, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_08, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_09, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_10, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_11, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_12, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_14, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_15, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_16, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_17, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_18, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_19, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_20, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        break;
    }
    default: {
        break;
    }
    }
}
static void scr_dashboard_cont_log_14_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED: {
        ui_state_modify(scr_dashboard_cont_log_14, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
        ui_state_modify(scr_dashboard_cont_log_01, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_02, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_03, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_04, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_05, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_06, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_07, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_08, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_09, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_10, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_11, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_12, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_13, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_15, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_16, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_17, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_18, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_19, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_20, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_detail_14, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_detail_01, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_02, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_03, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_04, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_05, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_06, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_07, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_08, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_09, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_10, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_11, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_12, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_13, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_15, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_16, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_17, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_18, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_19, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_20, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        break;
    }
    default: {
        break;
    }
    }
}
static void scr_dashboard_cont_log_15_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED: {
        ui_state_modify(scr_dashboard_cont_log_15, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
        ui_state_modify(scr_dashboard_cont_log_01, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_02, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_03, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_04, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_05, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_06, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_07, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_08, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_09, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_10, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_11, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_12, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_13, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_14, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_16, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_17, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_18, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_19, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_20, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_detail_15, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_detail_01, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_02, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_03, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_04, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_05, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_06, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_07, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_08, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_09, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_10, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_11, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_12, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_13, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_14, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_16, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_17, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_18, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_19, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_20, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        break;
    }
    default: {
        break;
    }
    }
}
static void scr_dashboard_cont_log_16_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED: {
        ui_state_modify(scr_dashboard_cont_log_16, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
        ui_state_modify(scr_dashboard_cont_log_01, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_02, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_03, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_04, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_05, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_06, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_07, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_08, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_09, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_10, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_11, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_12, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_13, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_14, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_15, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_17, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_18, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_19, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_20, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_detail_16, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_detail_01, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_02, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_03, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_04, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_05, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_06, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_07, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_08, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_09, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_10, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_11, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_12, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_13, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_14, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_15, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_17, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_18, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_19, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_20, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        break;
    }
    default: {
        break;
    }
    }
}
static void scr_dashboard_cont_log_17_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED: {
        ui_state_modify(scr_dashboard_cont_log_17, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
        ui_state_modify(scr_dashboard_cont_log_01, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_02, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_03, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_04, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_05, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_06, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_07, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_08, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_09, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_10, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_11, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_12, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_13, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_14, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_15, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_16, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_18, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_19, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_20, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_detail_17, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_detail_01, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_02, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_03, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_04, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_05, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_06, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_07, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_08, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_09, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_10, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_11, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_12, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_13, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_14, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_15, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_16, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_18, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_19, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_20, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        break;
    }
    default: {
        break;
    }
    }
}
static void scr_dashboard_cont_log_18_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED: {
        ui_state_modify(scr_dashboard_cont_log_18, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
        ui_state_modify(scr_dashboard_cont_log_01, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_02, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_03, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_04, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_05, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_06, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_07, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_08, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_09, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_10, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_11, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_12, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_13, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_14, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_15, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_16, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_17, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_19, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_20, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_detail_18, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_detail_01, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_02, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_03, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_04, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_05, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_06, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_07, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_08, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_09, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_10, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_11, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_12, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_13, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_14, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_15, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_16, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_17, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_19, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_20, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        break;
    }
    default: {
        break;
    }
    }
}
static void scr_dashboard_cont_log_19_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED: {
        ui_state_modify(scr_dashboard_cont_log_19, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
        ui_state_modify(scr_dashboard_cont_log_01, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_02, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_03, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_04, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_05, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_06, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_07, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_08, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_09, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_10, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_11, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_12, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_13, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_14, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_15, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_16, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_17, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_18, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_20, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_detail_19, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_detail_01, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_02, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_03, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_04, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_05, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_06, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_07, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_08, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_09, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_10, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_11, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_12, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_13, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_14, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_15, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_16, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_17, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_18, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_20, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        break;
    }
    default: {
        break;
    }
    }
}
static void scr_dashboard_cont_log_20_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED: {
        ui_state_modify(scr_dashboard_cont_log_20, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
        ui_state_modify(scr_dashboard_cont_log_01, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_02, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_03, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_04, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_05, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_06, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_07, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_08, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_09, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_10, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_11, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_12, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_13, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_14, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_15, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_16, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_17, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_18, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_state_modify(scr_dashboard_cont_log_19, LV_STATE_CHECKED, UI_STATE_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_detail_20, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_REMOVE);
        ui_flag_modify(scr_dashboard_cont_detail_01, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_02, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_03, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_04, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_05, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_06, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_07, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_08, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_09, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_10, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_11, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_12, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_13, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_14, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_15, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_16, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_17, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_18, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        ui_flag_modify(scr_dashboard_cont_detail_19, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
        break;
    }
    default: {
        break;
    }
    }
}
static void register_ui_events(void) {
    lv_obj_add_event_cb(scr_dashboard_imgbtn_sys, scr_dashboard_imgbtn_sys_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(scr_dashboard_imgbtn_log, scr_dashboard_imgbtn_log_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(scr_dashboard_imgbtn_set, scr_dashboard_imgbtn_set_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(scr_dashboard_imgbtn_dash, scr_dashboard_imgbtn_dash_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(scr_dashboard_btn_pid_speed_50, scr_dashboard_btn_pid_speed_50_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(scr_dashboard_btn_pid_speed_40, scr_dashboard_btn_pid_speed_40_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(scr_dashboard_btn_pid_speed_30, scr_dashboard_btn_pid_speed_30_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(scr_dashboard_btn_pid_speed_20, scr_dashboard_btn_pid_speed_20_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(scr_dashboard_btn_pid_mode, scr_dashboard_btn_pid_mode_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(scr_dashboard_btn_openloop_mode, scr_dashboard_btn_openloop_mode_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(scr_dashboard_imgbtn_display, scr_dashboard_imgbtn_display_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(scr_dashboard_imgbtn_detect, scr_dashboard_imgbtn_detect_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(scr_dashboard_imgbtn_about, scr_dashboard_imgbtn_about_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(scr_dashboard_imgbtn_control, scr_dashboard_imgbtn_control_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(scr_dashboard_imgbtn_net, scr_dashboard_imgbtn_net_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(scr_dashboard_cont_log_01, scr_dashboard_cont_log_01_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(scr_dashboard_cont_log_02, scr_dashboard_cont_log_02_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(scr_dashboard_cont_log_03, scr_dashboard_cont_log_03_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(scr_dashboard_cont_log_04, scr_dashboard_cont_log_04_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(scr_dashboard_cont_log_05, scr_dashboard_cont_log_05_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(scr_dashboard_cont_log_06, scr_dashboard_cont_log_06_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(scr_dashboard_cont_log_07, scr_dashboard_cont_log_07_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(scr_dashboard_cont_log_08, scr_dashboard_cont_log_08_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(scr_dashboard_cont_log_09, scr_dashboard_cont_log_09_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(scr_dashboard_cont_log_10, scr_dashboard_cont_log_10_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(scr_dashboard_cont_log_11, scr_dashboard_cont_log_11_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(scr_dashboard_cont_log_12, scr_dashboard_cont_log_12_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(scr_dashboard_cont_log_13, scr_dashboard_cont_log_13_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(scr_dashboard_cont_log_14, scr_dashboard_cont_log_14_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(scr_dashboard_cont_log_15, scr_dashboard_cont_log_15_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(scr_dashboard_cont_log_16, scr_dashboard_cont_log_16_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(scr_dashboard_cont_log_17, scr_dashboard_cont_log_17_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(scr_dashboard_cont_log_18, scr_dashboard_cont_log_18_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(scr_dashboard_cont_log_19, scr_dashboard_cont_log_19_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(scr_dashboard_cont_log_20, scr_dashboard_cont_log_20_event_handler, LV_EVENT_ALL, NULL);
}
static lv_obj_t * create_ui(void) {
    LV_LOG_USER("Initializing scr_dashboard ...");
    scr_dashboard = lv_obj_create(NULL);
    lv_obj_set_scrollbar_mode(scr_dashboard, LV_SCROLLBAR_MODE_OFF);
    // Add style for scr_dashboard - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard, LV_OBJ_FLAG_CLICKABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard, LV_OBJ_FLAG_SNAPPABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard, LV_OBJ_FLAG_CLICK_FOCUSABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard, LV_OBJ_FLAG_GESTURE_BUBBLE, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_cont_background1
    scr_dashboard_cont_background1 = lv_obj_create(scr_dashboard);
    lv_obj_set_x(scr_dashboard_cont_background1, 2);
    lv_obj_set_y(scr_dashboard_cont_background1, 2);
    lv_obj_set_width(scr_dashboard_cont_background1, 1020);
    lv_obj_set_height(scr_dashboard_cont_background1, 596);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_background1, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_background1, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_background1 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_background1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_background1, 58, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_background1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_background1, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_background1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_background1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_background1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_background1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_background1, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_background1, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_background1, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_background1, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_background1, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_cont_background2
    scr_dashboard_cont_background2 = lv_obj_create(scr_dashboard_cont_background1);
    lv_obj_set_x(scr_dashboard_cont_background2, 2);
    lv_obj_set_y(scr_dashboard_cont_background2, 2);
    lv_obj_set_width(scr_dashboard_cont_background2, 1012);
    lv_obj_set_height(scr_dashboard_cont_background2, 588);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_background2, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_background2, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_background2 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_background2, lv_color_hex(0x0B1517), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_background2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_background2, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_background2, 104, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_background2, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_background2, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_background2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_background2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_background2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_background2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(scr_dashboard_cont_background2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(scr_dashboard_cont_background2, 111, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(scr_dashboard_cont_background2, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(scr_dashboard_cont_background2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_background2, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_background2, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_background2, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_background2, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_background2, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_cont_system
    scr_dashboard_cont_system = lv_obj_create(scr_dashboard_cont_background2);
    lv_obj_set_x(scr_dashboard_cont_system, 4);
    lv_obj_set_y(scr_dashboard_cont_system, 76);
    lv_obj_set_width(scr_dashboard_cont_system, 1010);
    lv_obj_set_height(scr_dashboard_cont_system, 436);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_system, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_system, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_system - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_opa(scr_dashboard_cont_system, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_system, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_system, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_system, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_system, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_system, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_system, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_system, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_cont_system_memory
    scr_dashboard_cont_system_memory = lv_obj_create(scr_dashboard_cont_system);
    lv_obj_set_x(scr_dashboard_cont_system_memory, 424);
    lv_obj_set_y(scr_dashboard_cont_system_memory, 9);
    lv_obj_set_width(scr_dashboard_cont_system_memory, 534);
    lv_obj_set_height(scr_dashboard_cont_system_memory, 247);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_system_memory, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_system_memory, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_system_memory - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_system_memory, lv_color_hex(0x454545), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_system_memory, lv_color_hex(0x1E2426), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_system_memory, 60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_system_memory, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_system_memory, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_system_memory, 60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_system_memory, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_system_memory, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_system_memory, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_system_memory, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_system_memory, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_system_memory, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_system_memory, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_memory, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_memory, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_memory, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_memory, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_memory_title
    scr_dashboard_label_memory_title = lv_label_create(scr_dashboard_cont_system_memory);
    lv_obj_set_x(scr_dashboard_label_memory_title, 19);
    lv_obj_set_y(scr_dashboard_label_memory_title, 16);
    lv_obj_set_width(scr_dashboard_label_memory_title, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_memory_title, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_memory_title, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_memory_title, "内存资源");
    lv_label_set_long_mode(scr_dashboard_label_memory_title, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_memory_title - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_memory_title, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_memory_title, &lv_font_MiSans_Heavy_16_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_memory_title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_slider_memory_usage
    scr_dashboard_slider_memory_usage = lv_slider_create(scr_dashboard_cont_system_memory);
    lv_obj_set_x(scr_dashboard_slider_memory_usage, 131);
    lv_obj_set_y(scr_dashboard_slider_memory_usage, 67);
    lv_obj_set_width(scr_dashboard_slider_memory_usage, 300);
    lv_obj_set_height(scr_dashboard_slider_memory_usage, 8);
    lv_obj_set_scrollbar_mode(scr_dashboard_slider_memory_usage, LV_SCROLLBAR_MODE_OFF);
    lv_slider_set_range(scr_dashboard_slider_memory_usage, 0, 100);
    lv_slider_set_value(scr_dashboard_slider_memory_usage, 0, LV_ANIM_OFF);
    // Add style for scr_dashboard_slider_memory_usage - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_slider_memory_usage, lv_color_hex(0x0B0E0E), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_slider_memory_usage, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_slider_memory_usage - LV_PART_INDICATOR | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_slider_memory_usage, lv_color_hex(0x21966F), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_slider_memory_usage, lv_color_hex(0x70D296), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_slider_memory_usage, 200, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_slider_memory_usage, LV_GRAD_DIR_NONE, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_slider_memory_usage - LV_PART_KNOB | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_slider_memory_usage, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_slider_memory_usage, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_slider_memory_usage, LV_OBJ_FLAG_CLICKABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_slider_memory_usage, LV_OBJ_FLAG_SNAPPABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_slider_memory_usage, LV_OBJ_FLAG_CLICK_FOCUSABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_slider_memory_usage, LV_OBJ_FLAG_GESTURE_BUBBLE, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_memory_usage_value
    scr_dashboard_label_memory_usage_value = lv_label_create(scr_dashboard_cont_system_memory);
    lv_obj_set_x(scr_dashboard_label_memory_usage_value, 457);
    lv_obj_set_y(scr_dashboard_label_memory_usage_value, 64);
    lv_obj_set_width(scr_dashboard_label_memory_usage_value, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_memory_usage_value, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_memory_usage_value, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_memory_usage_value, "00%");
    lv_label_set_long_mode(scr_dashboard_label_memory_usage_value, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_memory_usage_value - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_memory_usage_value, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(scr_dashboard_label_memory_usage_value, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(scr_dashboard_label_memory_usage_value, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_memory_usage_value, &lv_font_Misans_Heavy_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_memory_usage_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_memory_usage_title
    scr_dashboard_label_memory_usage_title = lv_label_create(scr_dashboard_cont_system_memory);
    lv_obj_set_x(scr_dashboard_label_memory_usage_title, 40);
    lv_obj_set_y(scr_dashboard_label_memory_usage_title, 64);
    lv_obj_set_width(scr_dashboard_label_memory_usage_title, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_memory_usage_title, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_memory_usage_title, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_memory_usage_title, "内存使用率");
    lv_label_set_long_mode(scr_dashboard_label_memory_usage_title, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_memory_usage_title - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_memory_usage_title, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_memory_usage_title, &lv_font_Misans_Heavy_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_memory_usage_title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_system_memory_ram
    scr_dashboard_cont_system_memory_ram = lv_obj_create(scr_dashboard_cont_system_memory);
    lv_obj_set_x(scr_dashboard_cont_system_memory_ram, 34);
    lv_obj_set_y(scr_dashboard_cont_system_memory_ram, 110);
    lv_obj_set_width(scr_dashboard_cont_system_memory_ram, 184);
    lv_obj_set_height(scr_dashboard_cont_system_memory_ram, 112);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_system_memory_ram, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_system_memory_ram, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_system_memory_ram - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_system_memory_ram, lv_color_hex(0x2C3538), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_system_memory_ram, 120, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_system_memory_ram, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_system_memory_ram, 60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_system_memory_ram, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_system_memory_ram, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_system_memory_ram, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_system_memory_ram, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_system_memory_ram, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_system_memory_ram, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_system_memory_ram, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_memory_ram, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_memory_ram, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_memory_ram, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_memory_ram, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_ram_unit
    scr_dashboard_label_ram_unit = lv_label_create(scr_dashboard_cont_system_memory_ram);
    lv_obj_set_x(scr_dashboard_label_ram_unit, 132);
    lv_obj_set_y(scr_dashboard_label_ram_unit, 59);
    lv_obj_set_width(scr_dashboard_label_ram_unit, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_ram_unit, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_ram_unit, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_ram_unit, "KB");
    lv_label_set_long_mode(scr_dashboard_label_ram_unit, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_ram_unit - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_ram_unit, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_ram_unit, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_ram_unit, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_ram_unit, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_ram_value
    scr_dashboard_label_ram_value = lv_label_create(scr_dashboard_cont_system_memory_ram);
    lv_obj_set_x(scr_dashboard_label_ram_value, 42);
    lv_obj_set_y(scr_dashboard_label_ram_value, 47);
    lv_obj_set_width(scr_dashboard_label_ram_value, 100);
    lv_obj_set_height(scr_dashboard_label_ram_value, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_ram_value, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_ram_value, "---");
    lv_label_set_long_mode(scr_dashboard_label_ram_value, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_ram_value - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_ram_value, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_ram_value, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(scr_dashboard_label_ram_value, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(scr_dashboard_label_ram_value, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_ram_value, &lv_font_MiSans_Heavy_24_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_ram_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_ram_title
    scr_dashboard_label_ram_title = lv_label_create(scr_dashboard_cont_system_memory_ram);
    lv_obj_set_x(scr_dashboard_label_ram_title, 14);
    lv_obj_set_y(scr_dashboard_label_ram_title, 8);
    lv_obj_set_width(scr_dashboard_label_ram_title, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_ram_title, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_ram_title, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_ram_title, "内部RAM空闲");
    lv_label_set_long_mode(scr_dashboard_label_ram_title, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_ram_title - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_ram_title, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_ram_title, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_ram_title, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_ram_title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_system_memory_psram
    scr_dashboard_cont_system_memory_psram = lv_obj_create(scr_dashboard_cont_system_memory);
    lv_obj_set_x(scr_dashboard_cont_system_memory_psram, 229);
    lv_obj_set_y(scr_dashboard_cont_system_memory_psram, 109);
    lv_obj_set_width(scr_dashboard_cont_system_memory_psram, 269);
    lv_obj_set_height(scr_dashboard_cont_system_memory_psram, 113);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_system_memory_psram, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_system_memory_psram, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_system_memory_psram - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_system_memory_psram, lv_color_hex(0x2C3538), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_system_memory_psram, 120, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_system_memory_psram, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_system_memory_psram, 60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_system_memory_psram, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_system_memory_psram, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_system_memory_psram, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_system_memory_psram, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_system_memory_psram, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_system_memory_psram, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_system_memory_psram, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_memory_psram, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_memory_psram, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_memory_psram, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_memory_psram, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_psram_usage_value
    scr_dashboard_label_psram_usage_value = lv_label_create(scr_dashboard_cont_system_memory_psram);
    lv_obj_set_x(scr_dashboard_label_psram_usage_value, 213);
    lv_obj_set_y(scr_dashboard_label_psram_usage_value, 33);
    lv_obj_set_width(scr_dashboard_label_psram_usage_value, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_psram_usage_value, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_psram_usage_value, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_psram_usage_value, "00%");
    lv_label_set_long_mode(scr_dashboard_label_psram_usage_value, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_psram_usage_value - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_psram_usage_value, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(scr_dashboard_label_psram_usage_value, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(scr_dashboard_label_psram_usage_value, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_psram_usage_value, &lv_font_Misans_Heavy_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_psram_usage_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_psram_title
    scr_dashboard_label_psram_title = lv_label_create(scr_dashboard_cont_system_memory_psram);
    lv_obj_set_x(scr_dashboard_label_psram_title, 14);
    lv_obj_set_y(scr_dashboard_label_psram_title, 8);
    lv_obj_set_width(scr_dashboard_label_psram_title, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_psram_title, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_psram_title, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_psram_title, "PSRAM资源");
    lv_label_set_long_mode(scr_dashboard_label_psram_title, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_psram_title - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_psram_title, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_psram_title, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_psram_title, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_psram_title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_psram_free_value
    scr_dashboard_label_psram_free_value = lv_label_create(scr_dashboard_cont_system_memory_psram);
    lv_obj_set_x(scr_dashboard_label_psram_free_value, 149);
    lv_obj_set_y(scr_dashboard_label_psram_free_value, 65);
    lv_obj_set_width(scr_dashboard_label_psram_free_value, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_psram_free_value, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_psram_free_value, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_psram_free_value, "----");
    lv_label_set_long_mode(scr_dashboard_label_psram_free_value, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_psram_free_value - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_psram_free_value, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(scr_dashboard_label_psram_free_value, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(scr_dashboard_label_psram_free_value, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_psram_free_value, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_psram_free_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_slider_psram_usage
    scr_dashboard_slider_psram_usage = lv_slider_create(scr_dashboard_cont_system_memory_psram);
    lv_obj_set_x(scr_dashboard_slider_psram_usage, 28);
    lv_obj_set_y(scr_dashboard_slider_psram_usage, 37);
    lv_obj_set_width(scr_dashboard_slider_psram_usage, 160);
    lv_obj_set_height(scr_dashboard_slider_psram_usage, 6);
    lv_obj_set_scrollbar_mode(scr_dashboard_slider_psram_usage, LV_SCROLLBAR_MODE_OFF);
    lv_slider_set_range(scr_dashboard_slider_psram_usage, 0, 100);
    lv_slider_set_value(scr_dashboard_slider_psram_usage, 0, LV_ANIM_OFF);
    // Add style for scr_dashboard_slider_psram_usage - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_slider_psram_usage, lv_color_hex(0x05161A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_slider_psram_usage, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_slider_psram_usage - LV_PART_INDICATOR | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_slider_psram_usage, lv_color_hex(0x217A96), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_slider_psram_usage, lv_color_hex(0x84D5E0), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_slider_psram_usage, 200, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_slider_psram_usage, LV_GRAD_DIR_NONE, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_slider_psram_usage - LV_PART_KNOB | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_slider_psram_usage, lv_color_hex(0xBDD3D6), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_slider_psram_usage, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_slider_psram_usage, LV_OBJ_FLAG_CLICKABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_slider_psram_usage, LV_OBJ_FLAG_SNAPPABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_slider_psram_usage, LV_OBJ_FLAG_CLICK_FOCUSABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_slider_psram_usage, LV_OBJ_FLAG_GESTURE_BUBBLE, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_psram_free_unit
    scr_dashboard_label_psram_free_unit = lv_label_create(scr_dashboard_cont_system_memory_psram);
    lv_obj_set_x(scr_dashboard_label_psram_free_unit, 203);
    lv_obj_set_y(scr_dashboard_label_psram_free_unit, 65);
    lv_obj_set_width(scr_dashboard_label_psram_free_unit, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_psram_free_unit, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_psram_free_unit, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_psram_free_unit, "KB");
    lv_label_set_long_mode(scr_dashboard_label_psram_free_unit, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_psram_free_unit - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_psram_free_unit, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(scr_dashboard_label_psram_free_unit, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(scr_dashboard_label_psram_free_unit, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_psram_free_unit, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_psram_free_unit, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_psram_total_value
    scr_dashboard_label_psram_total_value = lv_label_create(scr_dashboard_cont_system_memory_psram);
    lv_obj_set_x(scr_dashboard_label_psram_total_value, 149);
    lv_obj_set_y(scr_dashboard_label_psram_total_value, 84);
    lv_obj_set_width(scr_dashboard_label_psram_total_value, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_psram_total_value, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_psram_total_value, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_psram_total_value, "----");
    lv_label_set_long_mode(scr_dashboard_label_psram_total_value, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_psram_total_value - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_psram_total_value, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(scr_dashboard_label_psram_total_value, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(scr_dashboard_label_psram_total_value, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_psram_total_value, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_psram_total_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_psram_total_unit
    scr_dashboard_label_psram_total_unit = lv_label_create(scr_dashboard_cont_system_memory_psram);
    lv_obj_set_x(scr_dashboard_label_psram_total_unit, 203);
    lv_obj_set_y(scr_dashboard_label_psram_total_unit, 86);
    lv_obj_set_width(scr_dashboard_label_psram_total_unit, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_psram_total_unit, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_psram_total_unit, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_psram_total_unit, "KB");
    lv_label_set_long_mode(scr_dashboard_label_psram_total_unit, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_psram_total_unit - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_psram_total_unit, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(scr_dashboard_label_psram_total_unit, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(scr_dashboard_label_psram_total_unit, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_psram_total_unit, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_psram_total_unit, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_psram_free_title
    scr_dashboard_label_psram_free_title = lv_label_create(scr_dashboard_cont_system_memory_psram);
    lv_obj_set_x(scr_dashboard_label_psram_free_title, 28);
    lv_obj_set_y(scr_dashboard_label_psram_free_title, 65);
    lv_obj_set_width(scr_dashboard_label_psram_free_title, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_psram_free_title, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_psram_free_title, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_psram_free_title, "空闲:");
    lv_label_set_long_mode(scr_dashboard_label_psram_free_title, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_psram_free_title - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_psram_free_title, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_psram_free_title, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_psram_free_title, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_psram_free_title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_psram_total_title
    scr_dashboard_label_psram_total_title = lv_label_create(scr_dashboard_cont_system_memory_psram);
    lv_obj_set_x(scr_dashboard_label_psram_total_title, 28);
    lv_obj_set_y(scr_dashboard_label_psram_total_title, 85);
    lv_obj_set_width(scr_dashboard_label_psram_total_title, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_psram_total_title, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_psram_total_title, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_psram_total_title, "总量:");
    lv_label_set_long_mode(scr_dashboard_label_psram_total_title, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_psram_total_title - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_psram_total_title, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_psram_total_title, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_psram_total_title, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_psram_total_title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_system_cpu
    scr_dashboard_cont_system_cpu = lv_obj_create(scr_dashboard_cont_system);
    lv_obj_set_x(scr_dashboard_cont_system_cpu, 49);
    lv_obj_set_y(scr_dashboard_cont_system_cpu, 9);
    lv_obj_set_width(scr_dashboard_cont_system_cpu, 347);
    lv_obj_set_height(scr_dashboard_cont_system_cpu, 247);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_system_cpu, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_system_cpu, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_system_cpu - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_system_cpu, lv_color_hex(0x454545), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_system_cpu, lv_color_hex(0x1E2426), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_system_cpu, 60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_system_cpu, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_system_cpu, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_system_cpu, 60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_system_cpu, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_system_cpu, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_system_cpu, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_system_cpu, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_system_cpu, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_system_cpu, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_system_cpu, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_cpu, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_cpu, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_cpu, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_cpu, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_cpu_title
    scr_dashboard_label_cpu_title = lv_label_create(scr_dashboard_cont_system_cpu);
    lv_obj_set_x(scr_dashboard_label_cpu_title, 19);
    lv_obj_set_y(scr_dashboard_label_cpu_title, 16);
    lv_obj_set_width(scr_dashboard_label_cpu_title, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_cpu_title, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_cpu_title, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_cpu_title, "CPU监控");
    lv_label_set_long_mode(scr_dashboard_label_cpu_title, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_cpu_title - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_cpu_title, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_cpu_title, &lv_font_MiSans_Heavy_16_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_cpu_title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_system_cpu_container
    scr_dashboard_cont_system_cpu_container = lv_obj_create(scr_dashboard_cont_system_cpu);
    lv_obj_set_x(scr_dashboard_cont_system_cpu_container, 24);
    lv_obj_set_y(scr_dashboard_cont_system_cpu_container, 16);
    lv_obj_set_width(scr_dashboard_cont_system_cpu_container, 282);
    lv_obj_set_height(scr_dashboard_cont_system_cpu_container, 222);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_system_cpu_container, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_system_cpu_container, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_system_cpu_container - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_system_cpu_container, lv_color_hex(0x1E2426), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_system_cpu_container, lv_color_hex(0x454545), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_system_cpu_container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_system_cpu_container, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_system_cpu_container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_system_cpu_container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_system_cpu_container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_system_cpu_container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_system_cpu_container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_system_cpu_container, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_cpu_container, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_cpu_container, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_cpu_container, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_cpu_container, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_cont_system_cpu_03
    scr_dashboard_cont_system_cpu_03 = lv_obj_create(scr_dashboard_cont_system_cpu_container);
    lv_obj_set_x(scr_dashboard_cont_system_cpu_03, 173);
    lv_obj_set_y(scr_dashboard_cont_system_cpu_03, -10);
    lv_obj_set_width(scr_dashboard_cont_system_cpu_03, 100);
    lv_obj_set_height(scr_dashboard_cont_system_cpu_03, 104);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_system_cpu_03, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_system_cpu_03, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_system_cpu_03 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_system_cpu_03, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_system_cpu_03, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_system_cpu_03, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_system_cpu_03, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_system_cpu_03, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_system_cpu_03, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_system_cpu_03, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_system_cpu_03, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_cpu_03, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_cpu_03, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_cpu_03, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_cpu_03, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_cpu_03_title
    scr_dashboard_label_cpu_03_title = lv_label_create(scr_dashboard_cont_system_cpu_03);
    lv_obj_set_x(scr_dashboard_label_cpu_03_title, 25);
    lv_obj_set_y(scr_dashboard_label_cpu_03_title, 88);
    lv_obj_set_width(scr_dashboard_label_cpu_03_title, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_cpu_03_title, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_cpu_03_title, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_cpu_03_title, "CPU02");
    lv_label_set_long_mode(scr_dashboard_label_cpu_03_title, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_cpu_03_title - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_cpu_03_title, lv_color_hex(0x7F98A0), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_cpu_03_title, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_cpu_03_title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_arc_system_cpu_03
    scr_dashboard_arc_system_cpu_03 = lv_arc_create(scr_dashboard_cont_system_cpu_03);
    lv_obj_set_x(scr_dashboard_arc_system_cpu_03, 8);
    lv_obj_set_y(scr_dashboard_arc_system_cpu_03, 8);
    lv_obj_set_width(scr_dashboard_arc_system_cpu_03, 80);
    lv_obj_set_height(scr_dashboard_arc_system_cpu_03, 80);
    lv_obj_set_scrollbar_mode(scr_dashboard_arc_system_cpu_03, LV_SCROLLBAR_MODE_OFF);
    lv_arc_set_mode(scr_dashboard_arc_system_cpu_03, LV_ARC_MODE_NORMAL);
    lv_arc_set_range(scr_dashboard_arc_system_cpu_03, 0, 100);
    lv_arc_set_bg_angles(scr_dashboard_arc_system_cpu_03, 135, 45);
    lv_arc_set_value(scr_dashboard_arc_system_cpu_03, 0);
    lv_arc_set_rotation(scr_dashboard_arc_system_cpu_03, 0);
    scr_dashboard_arc_system_cpu_03_label = lv_label_create(scr_dashboard_arc_system_cpu_03);
    lv_label_set_text_fmt(scr_dashboard_arc_system_cpu_03_label, "%ld", lv_arc_get_value(scr_dashboard_arc_system_cpu_03));
    lv_arc_align_obj_to_angle(scr_dashboard_arc_system_cpu_03, scr_dashboard_arc_system_cpu_03_label, 25);
    lv_obj_add_flag(scr_dashboard_arc_system_cpu_03_label, LV_OBJ_FLAG_HIDDEN);
    // Add style for scr_dashboard_arc_system_cpu_03 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_arc_color(scr_dashboard_arc_system_cpu_03, lv_color_hex(0x454545), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(scr_dashboard_arc_system_cpu_03, 155, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(scr_dashboard_arc_system_cpu_03, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(scr_dashboard_arc_system_cpu_03, true, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_arc_system_cpu_03 - LV_PART_INDICATOR | LV_STATE_DEFAULT
    lv_obj_set_style_arc_color(scr_dashboard_arc_system_cpu_03, lv_color_hex(0x71CF97), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(scr_dashboard_arc_system_cpu_03, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(scr_dashboard_arc_system_cpu_03, 6, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(scr_dashboard_arc_system_cpu_03, true, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_arc_system_cpu_03 - LV_PART_KNOB | LV_STATE_DEFAULT
    lv_obj_set_style_bg_opa(scr_dashboard_arc_system_cpu_03, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_arc_system_cpu_03, LV_OBJ_FLAG_CLICKABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_arc_system_cpu_03, LV_OBJ_FLAG_SNAPPABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_arc_system_cpu_03, LV_OBJ_FLAG_CLICK_FOCUSABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_arc_system_cpu_03, LV_OBJ_FLAG_GESTURE_BUBBLE, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_cpu_03_value
    scr_dashboard_label_cpu_03_value = lv_label_create(scr_dashboard_cont_system_cpu_03);
    lv_obj_set_x(scr_dashboard_label_cpu_03_value, -2);
    lv_obj_set_y(scr_dashboard_label_cpu_03_value, 41);
    lv_obj_set_width(scr_dashboard_label_cpu_03_value, 100);
    lv_obj_set_height(scr_dashboard_label_cpu_03_value, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_cpu_03_value, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_cpu_03_value, "00%");
    lv_label_set_long_mode(scr_dashboard_label_cpu_03_value, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_cpu_03_value - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_cpu_03_value, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_cpu_03_value, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_cpu_03_value, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_cpu_03_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_system_cpu_total
    scr_dashboard_cont_system_cpu_total = lv_obj_create(scr_dashboard_cont_system_cpu_container);
    lv_obj_set_x(scr_dashboard_cont_system_cpu_total, -12);
    lv_obj_set_y(scr_dashboard_cont_system_cpu_total, 23);
    lv_obj_set_width(scr_dashboard_cont_system_cpu_total, 167);
    lv_obj_set_height(scr_dashboard_cont_system_cpu_total, 175);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_system_cpu_total, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_system_cpu_total, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_system_cpu_total - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_system_cpu_total, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_system_cpu_total, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_system_cpu_total, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_system_cpu_total, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_system_cpu_total, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_system_cpu_total, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_system_cpu_total, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_system_cpu_total, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_cpu_total, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_cpu_total, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_cpu_total, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_cpu_total, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_cpu_total_title
    scr_dashboard_label_cpu_total_title = lv_label_create(scr_dashboard_cont_system_cpu_total);
    lv_obj_set_x(scr_dashboard_label_cpu_total_title, 35);
    lv_obj_set_y(scr_dashboard_label_cpu_total_title, 148);
    lv_obj_set_width(scr_dashboard_label_cpu_total_title, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_cpu_total_title, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_cpu_total_title, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_cpu_total_title, "整机CPU利用率");
    lv_label_set_long_mode(scr_dashboard_label_cpu_total_title, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_cpu_total_title - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_cpu_total_title, lv_color_hex(0x7F98A0), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_cpu_total_title, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_cpu_total_title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_arc_system_cpu_total
    scr_dashboard_arc_system_cpu_total = lv_arc_create(scr_dashboard_cont_system_cpu_total);
    lv_obj_set_x(scr_dashboard_arc_system_cpu_total, 14);
    lv_obj_set_y(scr_dashboard_arc_system_cpu_total, 8);
    lv_obj_set_width(scr_dashboard_arc_system_cpu_total, 140);
    lv_obj_set_height(scr_dashboard_arc_system_cpu_total, 140);
    lv_obj_set_scrollbar_mode(scr_dashboard_arc_system_cpu_total, LV_SCROLLBAR_MODE_OFF);
    lv_arc_set_mode(scr_dashboard_arc_system_cpu_total, LV_ARC_MODE_NORMAL);
    lv_arc_set_range(scr_dashboard_arc_system_cpu_total, 0, 100);
    lv_arc_set_bg_angles(scr_dashboard_arc_system_cpu_total, 136, 45);
    lv_arc_set_value(scr_dashboard_arc_system_cpu_total, 0);
    lv_arc_set_rotation(scr_dashboard_arc_system_cpu_total, 0);
    scr_dashboard_arc_system_cpu_total_label = lv_label_create(scr_dashboard_arc_system_cpu_total);
    lv_label_set_text_fmt(scr_dashboard_arc_system_cpu_total_label, "%ld", lv_arc_get_value(scr_dashboard_arc_system_cpu_total));
    lv_arc_align_obj_to_angle(scr_dashboard_arc_system_cpu_total, scr_dashboard_arc_system_cpu_total_label, 25);
    lv_obj_add_flag(scr_dashboard_arc_system_cpu_total_label, LV_OBJ_FLAG_HIDDEN);
    // Add style for scr_dashboard_arc_system_cpu_total - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_arc_color(scr_dashboard_arc_system_cpu_total, lv_color_hex(0x454545), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(scr_dashboard_arc_system_cpu_total, 155, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(scr_dashboard_arc_system_cpu_total, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(scr_dashboard_arc_system_cpu_total, true, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_arc_system_cpu_total - LV_PART_INDICATOR | LV_STATE_DEFAULT
    lv_obj_set_style_arc_color(scr_dashboard_arc_system_cpu_total, lv_color_hex(0x3FB2D1), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(scr_dashboard_arc_system_cpu_total, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(scr_dashboard_arc_system_cpu_total, 8, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(scr_dashboard_arc_system_cpu_total, true, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_arc_system_cpu_total - LV_PART_KNOB | LV_STATE_DEFAULT
    lv_obj_set_style_bg_opa(scr_dashboard_arc_system_cpu_total, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_arc_system_cpu_total, LV_OBJ_FLAG_CLICKABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_arc_system_cpu_total, LV_OBJ_FLAG_SNAPPABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_arc_system_cpu_total, LV_OBJ_FLAG_CLICK_FOCUSABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_arc_system_cpu_total, LV_OBJ_FLAG_GESTURE_BUBBLE, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_cpu_total_value
    scr_dashboard_label_cpu_total_value = lv_label_create(scr_dashboard_cont_system_cpu_total);
    lv_obj_set_x(scr_dashboard_label_cpu_total_value, 32);
    lv_obj_set_y(scr_dashboard_label_cpu_total_value, 68);
    lv_obj_set_width(scr_dashboard_label_cpu_total_value, 100);
    lv_obj_set_height(scr_dashboard_label_cpu_total_value, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_cpu_total_value, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_cpu_total_value, "00%");
    lv_label_set_long_mode(scr_dashboard_label_cpu_total_value, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_cpu_total_value - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_cpu_total_value, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_cpu_total_value, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_cpu_total_value, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_cpu_total_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_system_cpu_02
    scr_dashboard_cont_system_cpu_02 = lv_obj_create(scr_dashboard_cont_system_cpu_container);
    lv_obj_set_x(scr_dashboard_cont_system_cpu_02, 173);
    lv_obj_set_y(scr_dashboard_cont_system_cpu_02, 94);
    lv_obj_set_width(scr_dashboard_cont_system_cpu_02, 100);
    lv_obj_set_height(scr_dashboard_cont_system_cpu_02, 104);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_system_cpu_02, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_system_cpu_02, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_system_cpu_02 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_system_cpu_02, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_system_cpu_02, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_system_cpu_02, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_system_cpu_02, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_system_cpu_02, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_system_cpu_02, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_system_cpu_02, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_system_cpu_02, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_cpu_02, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_cpu_02, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_cpu_02, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_cpu_02, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_cpu_02_title
    scr_dashboard_label_cpu_02_title = lv_label_create(scr_dashboard_cont_system_cpu_02);
    lv_obj_set_x(scr_dashboard_label_cpu_02_title, 25);
    lv_obj_set_y(scr_dashboard_label_cpu_02_title, 88);
    lv_obj_set_width(scr_dashboard_label_cpu_02_title, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_cpu_02_title, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_cpu_02_title, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_cpu_02_title, "CPU01");
    lv_label_set_long_mode(scr_dashboard_label_cpu_02_title, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_cpu_02_title - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_cpu_02_title, lv_color_hex(0x7F98A0), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_cpu_02_title, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_cpu_02_title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_arc_system_cpu_02
    scr_dashboard_arc_system_cpu_02 = lv_arc_create(scr_dashboard_cont_system_cpu_02);
    lv_obj_set_x(scr_dashboard_arc_system_cpu_02, 8);
    lv_obj_set_y(scr_dashboard_arc_system_cpu_02, 8);
    lv_obj_set_width(scr_dashboard_arc_system_cpu_02, 80);
    lv_obj_set_height(scr_dashboard_arc_system_cpu_02, 80);
    lv_obj_set_scrollbar_mode(scr_dashboard_arc_system_cpu_02, LV_SCROLLBAR_MODE_OFF);
    lv_arc_set_mode(scr_dashboard_arc_system_cpu_02, LV_ARC_MODE_NORMAL);
    lv_arc_set_range(scr_dashboard_arc_system_cpu_02, 0, 100);
    lv_arc_set_bg_angles(scr_dashboard_arc_system_cpu_02, 135, 45);
    lv_arc_set_value(scr_dashboard_arc_system_cpu_02, 0);
    lv_arc_set_rotation(scr_dashboard_arc_system_cpu_02, 0);
    scr_dashboard_arc_system_cpu_02_label = lv_label_create(scr_dashboard_arc_system_cpu_02);
    lv_label_set_text_fmt(scr_dashboard_arc_system_cpu_02_label, "%ld", lv_arc_get_value(scr_dashboard_arc_system_cpu_02));
    lv_arc_align_obj_to_angle(scr_dashboard_arc_system_cpu_02, scr_dashboard_arc_system_cpu_02_label, 25);
    lv_obj_add_flag(scr_dashboard_arc_system_cpu_02_label, LV_OBJ_FLAG_HIDDEN);
    // Add style for scr_dashboard_arc_system_cpu_02 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_arc_color(scr_dashboard_arc_system_cpu_02, lv_color_hex(0x454545), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(scr_dashboard_arc_system_cpu_02, 155, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(scr_dashboard_arc_system_cpu_02, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(scr_dashboard_arc_system_cpu_02, true, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_arc_system_cpu_02 - LV_PART_INDICATOR | LV_STATE_DEFAULT
    lv_obj_set_style_arc_color(scr_dashboard_arc_system_cpu_02, lv_color_hex(0xEBE186), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(scr_dashboard_arc_system_cpu_02, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(scr_dashboard_arc_system_cpu_02, 6, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(scr_dashboard_arc_system_cpu_02, true, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_arc_system_cpu_02 - LV_PART_KNOB | LV_STATE_DEFAULT
    lv_obj_set_style_bg_opa(scr_dashboard_arc_system_cpu_02, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_arc_system_cpu_02, LV_OBJ_FLAG_CLICKABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_arc_system_cpu_02, LV_OBJ_FLAG_SNAPPABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_arc_system_cpu_02, LV_OBJ_FLAG_CLICK_FOCUSABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_arc_system_cpu_02, LV_OBJ_FLAG_GESTURE_BUBBLE, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_cpu_02_value
    scr_dashboard_label_cpu_02_value = lv_label_create(scr_dashboard_cont_system_cpu_02);
    lv_obj_set_x(scr_dashboard_label_cpu_02_value, -2);
    lv_obj_set_y(scr_dashboard_label_cpu_02_value, 43);
    lv_obj_set_width(scr_dashboard_label_cpu_02_value, 100);
    lv_obj_set_height(scr_dashboard_label_cpu_02_value, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_cpu_02_value, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_cpu_02_value, "00%");
    lv_label_set_long_mode(scr_dashboard_label_cpu_02_value, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_cpu_02_value - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_cpu_02_value, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_cpu_02_value, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_cpu_02_value, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_cpu_02_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_system_heap
    scr_dashboard_cont_system_heap = lv_obj_create(scr_dashboard_cont_system);
    lv_obj_set_x(scr_dashboard_cont_system_heap, 50);
    lv_obj_set_y(scr_dashboard_cont_system_heap, 273);
    lv_obj_set_width(scr_dashboard_cont_system_heap, 347);
    lv_obj_set_height(scr_dashboard_cont_system_heap, 149);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_system_heap, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_system_heap, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_system_heap - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_system_heap, lv_color_hex(0x454545), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_system_heap, lv_color_hex(0x1E2426), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_system_heap, 60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_system_heap, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_system_heap, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_system_heap, 60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_system_heap, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_system_heap, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_system_heap, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_system_heap, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_system_heap, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_system_heap, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_system_heap, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_heap, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_heap, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_heap, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_heap, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_heap_title
    scr_dashboard_label_heap_title = lv_label_create(scr_dashboard_cont_system_heap);
    lv_obj_set_x(scr_dashboard_label_heap_title, 19);
    lv_obj_set_y(scr_dashboard_label_heap_title, 16);
    lv_obj_set_width(scr_dashboard_label_heap_title, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_heap_title, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_heap_title, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_heap_title, "堆内存碎片");
    lv_label_set_long_mode(scr_dashboard_label_heap_title, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_heap_title - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_heap_title, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_heap_title, &lv_font_MiSans_Heavy_16_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_heap_title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_heap_free_title
    scr_dashboard_label_heap_free_title = lv_label_create(scr_dashboard_cont_system_heap);
    lv_obj_set_x(scr_dashboard_label_heap_free_title, 45);
    lv_obj_set_y(scr_dashboard_label_heap_free_title, 49);
    lv_obj_set_width(scr_dashboard_label_heap_free_title, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_heap_free_title, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_heap_free_title, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_heap_free_title, "空闲堆内存");
    lv_label_set_long_mode(scr_dashboard_label_heap_free_title, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_heap_free_title - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_heap_free_title, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_heap_free_title, &lv_font_MiSansVF_16_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_heap_free_title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_heap_min_free_title
    scr_dashboard_label_heap_min_free_title = lv_label_create(scr_dashboard_cont_system_heap);
    lv_obj_set_x(scr_dashboard_label_heap_min_free_title, 45);
    lv_obj_set_y(scr_dashboard_label_heap_min_free_title, 81);
    lv_obj_set_width(scr_dashboard_label_heap_min_free_title, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_heap_min_free_title, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_heap_min_free_title, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_heap_min_free_title, "历史最小空闲堆");
    lv_label_set_long_mode(scr_dashboard_label_heap_min_free_title, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_heap_min_free_title - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_heap_min_free_title, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_heap_min_free_title, &lv_font_MiSansVF_16_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_heap_min_free_title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_heap_max_block_title
    scr_dashboard_label_heap_max_block_title = lv_label_create(scr_dashboard_cont_system_heap);
    lv_obj_set_x(scr_dashboard_label_heap_max_block_title, 45);
    lv_obj_set_y(scr_dashboard_label_heap_max_block_title, 113);
    lv_obj_set_width(scr_dashboard_label_heap_max_block_title, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_heap_max_block_title, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_heap_max_block_title, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_heap_max_block_title, "最大连续可分配块");
    lv_label_set_long_mode(scr_dashboard_label_heap_max_block_title, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_heap_max_block_title - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_heap_max_block_title, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_heap_max_block_title, &lv_font_MiSansVF_16_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_heap_max_block_title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_heap_free_value
    scr_dashboard_label_heap_free_value = lv_label_create(scr_dashboard_cont_system_heap);
    lv_obj_set_x(scr_dashboard_label_heap_free_value, 231);
    lv_obj_set_y(scr_dashboard_label_heap_free_value, 49);
    lv_obj_set_width(scr_dashboard_label_heap_free_value, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_heap_free_value, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_heap_free_value, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_heap_free_value, "----");
    lv_label_set_long_mode(scr_dashboard_label_heap_free_value, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_heap_free_value - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_heap_free_value, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_heap_free_value, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_heap_free_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_heap_free_unit
    scr_dashboard_label_heap_free_unit = lv_label_create(scr_dashboard_cont_system_heap);
    lv_obj_set_x(scr_dashboard_label_heap_free_unit, 280);
    lv_obj_set_y(scr_dashboard_label_heap_free_unit, 49);
    lv_obj_set_width(scr_dashboard_label_heap_free_unit, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_heap_free_unit, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_heap_free_unit, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_heap_free_unit, "KB");
    lv_label_set_long_mode(scr_dashboard_label_heap_free_unit, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_heap_free_unit - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_heap_free_unit, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_heap_free_unit, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_heap_free_unit, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_heap_min_free_value
    scr_dashboard_label_heap_min_free_value = lv_label_create(scr_dashboard_cont_system_heap);
    lv_obj_set_x(scr_dashboard_label_heap_min_free_value, 231);
    lv_obj_set_y(scr_dashboard_label_heap_min_free_value, 81);
    lv_obj_set_width(scr_dashboard_label_heap_min_free_value, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_heap_min_free_value, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_heap_min_free_value, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_heap_min_free_value, "----");
    lv_label_set_long_mode(scr_dashboard_label_heap_min_free_value, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_heap_min_free_value - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_heap_min_free_value, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_heap_min_free_value, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_heap_min_free_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_heap_min_free_unit
    scr_dashboard_label_heap_min_free_unit = lv_label_create(scr_dashboard_cont_system_heap);
    lv_obj_set_x(scr_dashboard_label_heap_min_free_unit, 280);
    lv_obj_set_y(scr_dashboard_label_heap_min_free_unit, 81);
    lv_obj_set_width(scr_dashboard_label_heap_min_free_unit, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_heap_min_free_unit, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_heap_min_free_unit, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_heap_min_free_unit, "KB");
    lv_label_set_long_mode(scr_dashboard_label_heap_min_free_unit, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_heap_min_free_unit - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_heap_min_free_unit, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_heap_min_free_unit, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_heap_min_free_unit, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_heap_max_block_value
    scr_dashboard_label_heap_max_block_value = lv_label_create(scr_dashboard_cont_system_heap);
    lv_obj_set_x(scr_dashboard_label_heap_max_block_value, 231);
    lv_obj_set_y(scr_dashboard_label_heap_max_block_value, 113);
    lv_obj_set_width(scr_dashboard_label_heap_max_block_value, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_heap_max_block_value, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_heap_max_block_value, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_heap_max_block_value, "----");
    lv_label_set_long_mode(scr_dashboard_label_heap_max_block_value, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_heap_max_block_value - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_heap_max_block_value, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_heap_max_block_value, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_heap_max_block_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_heap_max_block_unit
    scr_dashboard_label_heap_max_block_unit = lv_label_create(scr_dashboard_cont_system_heap);
    lv_obj_set_x(scr_dashboard_label_heap_max_block_unit, 280);
    lv_obj_set_y(scr_dashboard_label_heap_max_block_unit, 113);
    lv_obj_set_width(scr_dashboard_label_heap_max_block_unit, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_heap_max_block_unit, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_heap_max_block_unit, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_heap_max_block_unit, "KB");
    lv_label_set_long_mode(scr_dashboard_label_heap_max_block_unit, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_heap_max_block_unit - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_heap_max_block_unit, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_heap_max_block_unit, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_heap_max_block_unit, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_system_status
    scr_dashboard_cont_system_status = lv_obj_create(scr_dashboard_cont_system);
    lv_obj_set_x(scr_dashboard_cont_system_status, 424);
    lv_obj_set_y(scr_dashboard_cont_system_status, 273);
    lv_obj_set_width(scr_dashboard_cont_system_status, 534);
    lv_obj_set_height(scr_dashboard_cont_system_status, 149);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_system_status, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_system_status, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_system_status - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_system_status, lv_color_hex(0x454545), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_system_status, lv_color_hex(0x1E2426), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_system_status, 60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_system_status, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_system_status, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_system_status, 60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_system_status, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_system_status, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_system_status, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_system_status, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_system_status, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_system_status, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_system_status, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_status, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_status, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_status, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_status, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_status_title
    scr_dashboard_label_status_title = lv_label_create(scr_dashboard_cont_system_status);
    lv_obj_set_x(scr_dashboard_label_status_title, 19);
    lv_obj_set_y(scr_dashboard_label_status_title, 16);
    lv_obj_set_width(scr_dashboard_label_status_title, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_status_title, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_status_title, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_status_title, "运行状态");
    lv_label_set_long_mode(scr_dashboard_label_status_title, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_status_title - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_status_title, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_status_title, &lv_font_MiSans_Heavy_16_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_status_title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_system_runtime
    scr_dashboard_cont_system_runtime = lv_obj_create(scr_dashboard_cont_system_status);
    lv_obj_set_x(scr_dashboard_cont_system_runtime, 19);
    lv_obj_set_y(scr_dashboard_cont_system_runtime, 44);
    lv_obj_set_width(scr_dashboard_cont_system_runtime, 118);
    lv_obj_set_height(scr_dashboard_cont_system_runtime, 88);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_system_runtime, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_system_runtime, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_system_runtime - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_system_runtime, lv_color_hex(0x2C3538), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_system_runtime, 160, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_system_runtime, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_system_runtime, 60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_system_runtime, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_system_runtime, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_system_runtime, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_system_runtime, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_system_runtime, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_system_runtime, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_system_runtime, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_runtime, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_runtime, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_runtime, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_runtime, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_runtime_value
    scr_dashboard_label_runtime_value = lv_label_create(scr_dashboard_cont_system_runtime);
    lv_obj_set_x(scr_dashboard_label_runtime_value, 8);
    lv_obj_set_y(scr_dashboard_label_runtime_value, 43);
    lv_obj_set_width(scr_dashboard_label_runtime_value, 100);
    lv_obj_set_height(scr_dashboard_label_runtime_value, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_runtime_value, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_runtime_value, "--");
    lv_label_set_long_mode(scr_dashboard_label_runtime_value, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_runtime_value - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_runtime_value, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_runtime_value, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(scr_dashboard_label_runtime_value, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(scr_dashboard_label_runtime_value, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_runtime_value, &lv_font_MiSans_Heavy_16_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_runtime_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_runtime_title
    scr_dashboard_label_runtime_title = lv_label_create(scr_dashboard_cont_system_runtime);
    lv_obj_set_x(scr_dashboard_label_runtime_title, 12);
    lv_obj_set_y(scr_dashboard_label_runtime_title, 7);
    lv_obj_set_width(scr_dashboard_label_runtime_title, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_runtime_title, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_runtime_title, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_runtime_title, "系统运行时间");
    lv_label_set_long_mode(scr_dashboard_label_runtime_title, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_runtime_title - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_runtime_title, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_runtime_title, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_runtime_title, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_runtime_title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_system_chip_temp
    scr_dashboard_cont_system_chip_temp = lv_obj_create(scr_dashboard_cont_system_status);
    lv_obj_set_x(scr_dashboard_cont_system_chip_temp, 147);
    lv_obj_set_y(scr_dashboard_cont_system_chip_temp, 44);
    lv_obj_set_width(scr_dashboard_cont_system_chip_temp, 118);
    lv_obj_set_height(scr_dashboard_cont_system_chip_temp, 88);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_system_chip_temp, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_system_chip_temp, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_system_chip_temp - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_system_chip_temp, lv_color_hex(0x2C3538), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_system_chip_temp, 160, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_system_chip_temp, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_system_chip_temp, 60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_system_chip_temp, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_system_chip_temp, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_system_chip_temp, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_system_chip_temp, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_system_chip_temp, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_system_chip_temp, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_system_chip_temp, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_chip_temp, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_chip_temp, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_chip_temp, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_chip_temp, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_chip_temp_value
    scr_dashboard_label_chip_temp_value = lv_label_create(scr_dashboard_cont_system_chip_temp);
    lv_obj_set_x(scr_dashboard_label_chip_temp_value, 8);
    lv_obj_set_y(scr_dashboard_label_chip_temp_value, 43);
    lv_obj_set_width(scr_dashboard_label_chip_temp_value, 100);
    lv_obj_set_height(scr_dashboard_label_chip_temp_value, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_chip_temp_value, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_chip_temp_value, "---");
    lv_label_set_long_mode(scr_dashboard_label_chip_temp_value, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_chip_temp_value - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_chip_temp_value, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_chip_temp_value, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(scr_dashboard_label_chip_temp_value, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(scr_dashboard_label_chip_temp_value, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_chip_temp_value, &lv_font_MiSans_Heavy_16_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_chip_temp_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_chip_temp_title
    scr_dashboard_label_chip_temp_title = lv_label_create(scr_dashboard_cont_system_chip_temp);
    lv_obj_set_x(scr_dashboard_label_chip_temp_title, 12);
    lv_obj_set_y(scr_dashboard_label_chip_temp_title, 7);
    lv_obj_set_width(scr_dashboard_label_chip_temp_title, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_chip_temp_title, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_chip_temp_title, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_chip_temp_title, "芯片温度");
    lv_label_set_long_mode(scr_dashboard_label_chip_temp_title, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_chip_temp_title - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_chip_temp_title, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_chip_temp_title, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_chip_temp_title, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_chip_temp_title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_system_cpu_freq
    scr_dashboard_cont_system_cpu_freq = lv_obj_create(scr_dashboard_cont_system_status);
    lv_obj_set_x(scr_dashboard_cont_system_cpu_freq, 274);
    lv_obj_set_y(scr_dashboard_cont_system_cpu_freq, 44);
    lv_obj_set_width(scr_dashboard_cont_system_cpu_freq, 118);
    lv_obj_set_height(scr_dashboard_cont_system_cpu_freq, 88);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_system_cpu_freq, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_system_cpu_freq, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_system_cpu_freq - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_system_cpu_freq, lv_color_hex(0x2C3538), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_system_cpu_freq, 160, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_system_cpu_freq, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_system_cpu_freq, 60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_system_cpu_freq, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_system_cpu_freq, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_system_cpu_freq, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_system_cpu_freq, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_system_cpu_freq, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_system_cpu_freq, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_system_cpu_freq, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_cpu_freq, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_cpu_freq, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_cpu_freq, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_cpu_freq, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_cpu_freq_unit
    scr_dashboard_label_cpu_freq_unit = lv_label_create(scr_dashboard_cont_system_cpu_freq);
    lv_obj_set_x(scr_dashboard_label_cpu_freq_unit, 76);
    lv_obj_set_y(scr_dashboard_label_cpu_freq_unit, 47);
    lv_obj_set_width(scr_dashboard_label_cpu_freq_unit, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_cpu_freq_unit, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_cpu_freq_unit, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_cpu_freq_unit, "MHz");
    lv_label_set_long_mode(scr_dashboard_label_cpu_freq_unit, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_cpu_freq_unit - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_cpu_freq_unit, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_cpu_freq_unit, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_cpu_freq_unit, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_cpu_freq_unit, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_cpu_freq_value
    scr_dashboard_label_cpu_freq_value = lv_label_create(scr_dashboard_cont_system_cpu_freq);
    lv_obj_set_x(scr_dashboard_label_cpu_freq_value, 8);
    lv_obj_set_y(scr_dashboard_label_cpu_freq_value, 43);
    lv_obj_set_width(scr_dashboard_label_cpu_freq_value, 100);
    lv_obj_set_height(scr_dashboard_label_cpu_freq_value, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_cpu_freq_value, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_cpu_freq_value, "--");
    lv_label_set_long_mode(scr_dashboard_label_cpu_freq_value, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_cpu_freq_value - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_cpu_freq_value, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_cpu_freq_value, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(scr_dashboard_label_cpu_freq_value, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(scr_dashboard_label_cpu_freq_value, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_cpu_freq_value, &lv_font_MiSans_Heavy_16_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_cpu_freq_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_cpu_freq_title
    scr_dashboard_label_cpu_freq_title = lv_label_create(scr_dashboard_cont_system_cpu_freq);
    lv_obj_set_x(scr_dashboard_label_cpu_freq_title, 12);
    lv_obj_set_y(scr_dashboard_label_cpu_freq_title, 7);
    lv_obj_set_width(scr_dashboard_label_cpu_freq_title, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_cpu_freq_title, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_cpu_freq_title, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_cpu_freq_title, "当前CPU频率");
    lv_label_set_long_mode(scr_dashboard_label_cpu_freq_title, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_cpu_freq_title - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_cpu_freq_title, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_cpu_freq_title, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_cpu_freq_title, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_cpu_freq_title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_system_task_count
    scr_dashboard_cont_system_task_count = lv_obj_create(scr_dashboard_cont_system_status);
    lv_obj_set_x(scr_dashboard_cont_system_task_count, 401);
    lv_obj_set_y(scr_dashboard_cont_system_task_count, 44);
    lv_obj_set_width(scr_dashboard_cont_system_task_count, 118);
    lv_obj_set_height(scr_dashboard_cont_system_task_count, 88);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_system_task_count, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_system_task_count, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_system_task_count - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_system_task_count, lv_color_hex(0x2C3538), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_system_task_count, 160, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_system_task_count, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_system_task_count, 60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_system_task_count, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_system_task_count, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_system_task_count, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_system_task_count, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_system_task_count, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_system_task_count, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_system_task_count, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_task_count, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_task_count, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_task_count, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_system_task_count, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_task_count_value
    scr_dashboard_label_task_count_value = lv_label_create(scr_dashboard_cont_system_task_count);
    lv_obj_set_x(scr_dashboard_label_task_count_value, 8);
    lv_obj_set_y(scr_dashboard_label_task_count_value, 43);
    lv_obj_set_width(scr_dashboard_label_task_count_value, 100);
    lv_obj_set_height(scr_dashboard_label_task_count_value, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_task_count_value, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_task_count_value, "--");
    lv_label_set_long_mode(scr_dashboard_label_task_count_value, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_task_count_value - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_task_count_value, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_task_count_value, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(scr_dashboard_label_task_count_value, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(scr_dashboard_label_task_count_value, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_task_count_value, &lv_font_MiSans_Heavy_16_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_task_count_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_task_count_title
    scr_dashboard_label_task_count_title = lv_label_create(scr_dashboard_cont_system_task_count);
    lv_obj_set_x(scr_dashboard_label_task_count_title, 12);
    lv_obj_set_y(scr_dashboard_label_task_count_title, 7);
    lv_obj_set_width(scr_dashboard_label_task_count_title, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_task_count_title, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_task_count_title, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_task_count_title, "任务数量");
    lv_label_set_long_mode(scr_dashboard_label_task_count_title, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_task_count_title - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_task_count_title, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_task_count_title, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_task_count_title, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_task_count_title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_log
    scr_dashboard_cont_log = lv_obj_create(scr_dashboard_cont_background2);
    lv_obj_set_x(scr_dashboard_cont_log, 3);
    lv_obj_set_y(scr_dashboard_cont_log, 69);
    lv_obj_set_width(scr_dashboard_cont_log, 1010);
    lv_obj_set_height(scr_dashboard_cont_log, 436);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_log, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_log, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_log - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_log, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_log, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_cont_event_details
    scr_dashboard_cont_event_details = lv_obj_create(scr_dashboard_cont_log);
    lv_obj_set_x(scr_dashboard_cont_event_details, 596);
    lv_obj_set_y(scr_dashboard_cont_event_details, 24);
    lv_obj_set_width(scr_dashboard_cont_event_details, 380);
    lv_obj_set_height(scr_dashboard_cont_event_details, 384);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_event_details, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_event_details, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_event_details - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_event_details, lv_color_hex(0x232C2C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_event_details, lv_color_hex(0x070A0A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_event_details, 180, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_event_details, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_event_details, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_event_details, 46, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_event_details, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_event_details, 16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_event_details, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_event_details, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_event_details, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_event_details, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_event_details, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_event_details, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_event_details, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_event_details, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_event_details, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_cont_detail_20
    scr_dashboard_cont_detail_20 = lv_obj_create(scr_dashboard_cont_event_details);
    lv_obj_set_x(scr_dashboard_cont_detail_20, 9);
    lv_obj_set_y(scr_dashboard_cont_detail_20, 7);
    lv_obj_set_width(scr_dashboard_cont_detail_20, 360);
    lv_obj_set_height(scr_dashboard_cont_detail_20, 370);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_detail_20, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_detail_20, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_detail_20 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_detail_20, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_detail_20, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_20, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_20, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_20, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_20, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_detail_result_20
    scr_dashboard_label_detail_result_20 = lv_label_create(scr_dashboard_cont_detail_20);
    lv_obj_set_x(scr_dashboard_label_detail_result_20, 204);
    lv_obj_set_y(scr_dashboard_label_detail_result_20, 114);
    lv_obj_set_width(scr_dashboard_label_detail_result_20, 100);
    lv_obj_set_height(scr_dashboard_label_detail_result_20, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_result_20, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_result_20, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_result_20, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_result_20 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_result_20, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_result_20, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_result_20, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_logo_conf_20
    scr_dashboard_label_logo_conf_20 = lv_label_create(scr_dashboard_cont_detail_20);
    lv_obj_set_x(scr_dashboard_label_logo_conf_20, 204);
    lv_obj_set_y(scr_dashboard_label_logo_conf_20, 164);
    lv_obj_set_width(scr_dashboard_label_logo_conf_20, 100);
    lv_obj_set_height(scr_dashboard_label_logo_conf_20, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_logo_conf_20, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_logo_conf_20, "--");
    lv_label_set_long_mode(scr_dashboard_label_logo_conf_20, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_logo_conf_20 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_logo_conf_20, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_logo_conf_20, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_logo_conf_20, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_logo_conf_20, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_logo_inft_20
    scr_dashboard_label_logo_inft_20 = lv_label_create(scr_dashboard_cont_detail_20);
    lv_obj_set_x(scr_dashboard_label_logo_inft_20, 204);
    lv_obj_set_y(scr_dashboard_label_logo_inft_20, 261);
    lv_obj_set_width(scr_dashboard_label_logo_inft_20, 100);
    lv_obj_set_height(scr_dashboard_label_logo_inft_20, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_logo_inft_20, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_logo_inft_20, "--");
    lv_label_set_long_mode(scr_dashboard_label_logo_inft_20, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_logo_inft_20 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_logo_inft_20, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_logo_inft_20, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_logo_inft_20, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_detail_time_20
    scr_dashboard_label_detail_time_20 = lv_label_create(scr_dashboard_cont_detail_20);
    lv_obj_set_x(scr_dashboard_label_detail_time_20, 204);
    lv_obj_set_y(scr_dashboard_label_detail_time_20, 68);
    lv_obj_set_width(scr_dashboard_label_detail_time_20, 100);
    lv_obj_set_height(scr_dashboard_label_detail_time_20, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_time_20, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_time_20, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_time_20, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_time_20 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_time_20, lv_color_hex(0x718A92), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_time_20, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_time_20, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_mian_conf_20
    scr_dashboard_label_mian_conf_20 = lv_label_create(scr_dashboard_cont_detail_20);
    lv_obj_set_x(scr_dashboard_label_mian_conf_20, 204);
    lv_obj_set_y(scr_dashboard_label_mian_conf_20, 210);
    lv_obj_set_width(scr_dashboard_label_mian_conf_20, 100);
    lv_obj_set_height(scr_dashboard_label_mian_conf_20, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_mian_conf_20, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_mian_conf_20, "--");
    lv_label_set_long_mode(scr_dashboard_label_mian_conf_20, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_mian_conf_20 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_mian_conf_20, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_mian_conf_20, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_mian_conf_20, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_mian_inft_20
    scr_dashboard_label_mian_inft_20 = lv_label_create(scr_dashboard_cont_detail_20);
    lv_obj_set_x(scr_dashboard_label_mian_inft_20, 204);
    lv_obj_set_y(scr_dashboard_label_mian_inft_20, 308);
    lv_obj_set_width(scr_dashboard_label_mian_inft_20, 100);
    lv_obj_set_height(scr_dashboard_label_mian_inft_20, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_mian_inft_20, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_mian_inft_20, "--");
    lv_label_set_long_mode(scr_dashboard_label_mian_inft_20, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_mian_inft_20 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_mian_inft_20, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_mian_inft_20, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_mian_inft_20, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_detail_badge_20
    scr_dashboard_cont_detail_badge_20 = lv_obj_create(scr_dashboard_cont_detail_20);
    lv_obj_set_x(scr_dashboard_cont_detail_badge_20, 274);
    lv_obj_set_y(scr_dashboard_cont_detail_badge_20, 6);
    lv_obj_set_width(scr_dashboard_cont_detail_badge_20, 72);
    lv_obj_set_height(scr_dashboard_cont_detail_badge_20, 24);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_detail_badge_20, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_detail_badge_20, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_detail_badge_20 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_detail_badge_20, lv_color_hex(0x232C2C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_badge_20, 220, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_detail_badge_20, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_detail_badge_20, 66, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_badge_20, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_detail_badge_20, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_badge_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_badge_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_badge_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_badge_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_cont_detail_badge_20 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_cont_detail_badge_20, lv_color_hex(0x3A2A12), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_badge_20, 220, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_cont_detail_badge_20, lv_color_hex(0xE8B84A), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_cont_detail_badge_20, 120, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_badge_20, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_cont_detail_badge_20, 12, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_badge_20, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_badge_20, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_badge_20, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_badge_20, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    ui_flag_modify(scr_dashboard_cont_detail_badge_20, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_20, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_20, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_20, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_20, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_detail_badge_20
    scr_dashboard_label_detail_badge_20 = lv_label_create(scr_dashboard_cont_detail_badge_20);
    lv_obj_set_x(scr_dashboard_label_detail_badge_20, -14);
    lv_obj_set_y(scr_dashboard_label_detail_badge_20, 5);
    lv_obj_set_width(scr_dashboard_label_detail_badge_20, 100);
    lv_obj_set_height(scr_dashboard_label_detail_badge_20, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_badge_20, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_badge_20, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_badge_20, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_badge_20 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_badge_20, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_detail_badge_20, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_label_detail_badge_20 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_text_color(scr_dashboard_label_detail_badge_20, lv_color_hex(0xE8B84A), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(scr_dashboard_label_detail_badge_20, 255, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_badge_20, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_badge_20, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_badge_20, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_badge_20, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_CHECKED);
    // Create scr_dashboard_cont_detail_19
    scr_dashboard_cont_detail_19 = lv_obj_create(scr_dashboard_cont_event_details);
    lv_obj_set_x(scr_dashboard_cont_detail_19, 9);
    lv_obj_set_y(scr_dashboard_cont_detail_19, 7);
    lv_obj_set_width(scr_dashboard_cont_detail_19, 360);
    lv_obj_set_height(scr_dashboard_cont_detail_19, 370);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_detail_19, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_detail_19, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_detail_19 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_detail_19, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_detail_19, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_19, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_19, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_19, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_19, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_detail_result_19
    scr_dashboard_label_detail_result_19 = lv_label_create(scr_dashboard_cont_detail_19);
    lv_obj_set_x(scr_dashboard_label_detail_result_19, 204);
    lv_obj_set_y(scr_dashboard_label_detail_result_19, 114);
    lv_obj_set_width(scr_dashboard_label_detail_result_19, 100);
    lv_obj_set_height(scr_dashboard_label_detail_result_19, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_result_19, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_result_19, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_result_19, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_result_19 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_result_19, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_result_19, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_result_19, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_logo_conf_19
    scr_dashboard_label_logo_conf_19 = lv_label_create(scr_dashboard_cont_detail_19);
    lv_obj_set_x(scr_dashboard_label_logo_conf_19, 204);
    lv_obj_set_y(scr_dashboard_label_logo_conf_19, 164);
    lv_obj_set_width(scr_dashboard_label_logo_conf_19, 100);
    lv_obj_set_height(scr_dashboard_label_logo_conf_19, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_logo_conf_19, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_logo_conf_19, "--");
    lv_label_set_long_mode(scr_dashboard_label_logo_conf_19, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_logo_conf_19 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_logo_conf_19, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_logo_conf_19, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_logo_conf_19, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_logo_conf_19, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_logo_inft_19
    scr_dashboard_label_logo_inft_19 = lv_label_create(scr_dashboard_cont_detail_19);
    lv_obj_set_x(scr_dashboard_label_logo_inft_19, 204);
    lv_obj_set_y(scr_dashboard_label_logo_inft_19, 261);
    lv_obj_set_width(scr_dashboard_label_logo_inft_19, 100);
    lv_obj_set_height(scr_dashboard_label_logo_inft_19, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_logo_inft_19, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_logo_inft_19, "--");
    lv_label_set_long_mode(scr_dashboard_label_logo_inft_19, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_logo_inft_19 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_logo_inft_19, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_logo_inft_19, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_logo_inft_19, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_detail_time_19
    scr_dashboard_label_detail_time_19 = lv_label_create(scr_dashboard_cont_detail_19);
    lv_obj_set_x(scr_dashboard_label_detail_time_19, 204);
    lv_obj_set_y(scr_dashboard_label_detail_time_19, 68);
    lv_obj_set_width(scr_dashboard_label_detail_time_19, 100);
    lv_obj_set_height(scr_dashboard_label_detail_time_19, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_time_19, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_time_19, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_time_19, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_time_19 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_time_19, lv_color_hex(0x718A92), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_time_19, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_time_19, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_mian_conf_19
    scr_dashboard_label_mian_conf_19 = lv_label_create(scr_dashboard_cont_detail_19);
    lv_obj_set_x(scr_dashboard_label_mian_conf_19, 204);
    lv_obj_set_y(scr_dashboard_label_mian_conf_19, 210);
    lv_obj_set_width(scr_dashboard_label_mian_conf_19, 100);
    lv_obj_set_height(scr_dashboard_label_mian_conf_19, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_mian_conf_19, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_mian_conf_19, "--");
    lv_label_set_long_mode(scr_dashboard_label_mian_conf_19, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_mian_conf_19 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_mian_conf_19, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_mian_conf_19, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_mian_conf_19, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_mian_inft_19
    scr_dashboard_label_mian_inft_19 = lv_label_create(scr_dashboard_cont_detail_19);
    lv_obj_set_x(scr_dashboard_label_mian_inft_19, 204);
    lv_obj_set_y(scr_dashboard_label_mian_inft_19, 308);
    lv_obj_set_width(scr_dashboard_label_mian_inft_19, 100);
    lv_obj_set_height(scr_dashboard_label_mian_inft_19, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_mian_inft_19, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_mian_inft_19, "--");
    lv_label_set_long_mode(scr_dashboard_label_mian_inft_19, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_mian_inft_19 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_mian_inft_19, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_mian_inft_19, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_mian_inft_19, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_detail_badge_19
    scr_dashboard_cont_detail_badge_19 = lv_obj_create(scr_dashboard_cont_detail_19);
    lv_obj_set_x(scr_dashboard_cont_detail_badge_19, 274);
    lv_obj_set_y(scr_dashboard_cont_detail_badge_19, 6);
    lv_obj_set_width(scr_dashboard_cont_detail_badge_19, 72);
    lv_obj_set_height(scr_dashboard_cont_detail_badge_19, 24);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_detail_badge_19, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_detail_badge_19, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_detail_badge_19 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_detail_badge_19, lv_color_hex(0x232C2C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_badge_19, 220, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_detail_badge_19, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_detail_badge_19, 66, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_badge_19, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_detail_badge_19, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_badge_19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_badge_19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_badge_19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_badge_19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_cont_detail_badge_19 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_cont_detail_badge_19, lv_color_hex(0x3A2A12), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_badge_19, 220, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_cont_detail_badge_19, lv_color_hex(0xE8B84A), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_cont_detail_badge_19, 120, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_badge_19, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_cont_detail_badge_19, 12, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_badge_19, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_badge_19, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_badge_19, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_badge_19, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    ui_flag_modify(scr_dashboard_cont_detail_badge_19, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_19, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_19, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_19, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_19, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_detail_badge_19
    scr_dashboard_label_detail_badge_19 = lv_label_create(scr_dashboard_cont_detail_badge_19);
    lv_obj_set_x(scr_dashboard_label_detail_badge_19, -14);
    lv_obj_set_y(scr_dashboard_label_detail_badge_19, 5);
    lv_obj_set_width(scr_dashboard_label_detail_badge_19, 100);
    lv_obj_set_height(scr_dashboard_label_detail_badge_19, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_badge_19, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_badge_19, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_badge_19, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_badge_19 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_badge_19, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_detail_badge_19, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_label_detail_badge_19 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_text_color(scr_dashboard_label_detail_badge_19, lv_color_hex(0xE8B84A), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(scr_dashboard_label_detail_badge_19, 255, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_badge_19, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_badge_19, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_badge_19, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_badge_19, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_CHECKED);
    // Create scr_dashboard_cont_detail_18
    scr_dashboard_cont_detail_18 = lv_obj_create(scr_dashboard_cont_event_details);
    lv_obj_set_x(scr_dashboard_cont_detail_18, 9);
    lv_obj_set_y(scr_dashboard_cont_detail_18, 7);
    lv_obj_set_width(scr_dashboard_cont_detail_18, 360);
    lv_obj_set_height(scr_dashboard_cont_detail_18, 370);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_detail_18, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_detail_18, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_detail_18 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_18, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_18, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_18, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_18, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_18, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_18, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_detail_18, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_detail_18, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_18, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_18, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_18, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_18, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_detail_result_18
    scr_dashboard_label_detail_result_18 = lv_label_create(scr_dashboard_cont_detail_18);
    lv_obj_set_x(scr_dashboard_label_detail_result_18, 204);
    lv_obj_set_y(scr_dashboard_label_detail_result_18, 114);
    lv_obj_set_width(scr_dashboard_label_detail_result_18, 100);
    lv_obj_set_height(scr_dashboard_label_detail_result_18, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_result_18, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_result_18, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_result_18, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_result_18 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_result_18, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_result_18, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_result_18, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_logo_conf_18
    scr_dashboard_label_logo_conf_18 = lv_label_create(scr_dashboard_cont_detail_18);
    lv_obj_set_x(scr_dashboard_label_logo_conf_18, 204);
    lv_obj_set_y(scr_dashboard_label_logo_conf_18, 164);
    lv_obj_set_width(scr_dashboard_label_logo_conf_18, 100);
    lv_obj_set_height(scr_dashboard_label_logo_conf_18, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_logo_conf_18, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_logo_conf_18, "--");
    lv_label_set_long_mode(scr_dashboard_label_logo_conf_18, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_logo_conf_18 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_logo_conf_18, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_logo_conf_18, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_logo_conf_18, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_logo_conf_18, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_logo_inft_18
    scr_dashboard_label_logo_inft_18 = lv_label_create(scr_dashboard_cont_detail_18);
    lv_obj_set_x(scr_dashboard_label_logo_inft_18, 204);
    lv_obj_set_y(scr_dashboard_label_logo_inft_18, 261);
    lv_obj_set_width(scr_dashboard_label_logo_inft_18, 100);
    lv_obj_set_height(scr_dashboard_label_logo_inft_18, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_logo_inft_18, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_logo_inft_18, "--");
    lv_label_set_long_mode(scr_dashboard_label_logo_inft_18, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_logo_inft_18 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_logo_inft_18, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_logo_inft_18, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_logo_inft_18, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_detail_time_18
    scr_dashboard_label_detail_time_18 = lv_label_create(scr_dashboard_cont_detail_18);
    lv_obj_set_x(scr_dashboard_label_detail_time_18, 204);
    lv_obj_set_y(scr_dashboard_label_detail_time_18, 68);
    lv_obj_set_width(scr_dashboard_label_detail_time_18, 100);
    lv_obj_set_height(scr_dashboard_label_detail_time_18, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_time_18, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_time_18, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_time_18, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_time_18 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_time_18, lv_color_hex(0x718A92), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_time_18, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_time_18, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_mian_conf_18
    scr_dashboard_label_mian_conf_18 = lv_label_create(scr_dashboard_cont_detail_18);
    lv_obj_set_x(scr_dashboard_label_mian_conf_18, 204);
    lv_obj_set_y(scr_dashboard_label_mian_conf_18, 210);
    lv_obj_set_width(scr_dashboard_label_mian_conf_18, 100);
    lv_obj_set_height(scr_dashboard_label_mian_conf_18, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_mian_conf_18, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_mian_conf_18, "--");
    lv_label_set_long_mode(scr_dashboard_label_mian_conf_18, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_mian_conf_18 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_mian_conf_18, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_mian_conf_18, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_mian_conf_18, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_mian_inft_18
    scr_dashboard_label_mian_inft_18 = lv_label_create(scr_dashboard_cont_detail_18);
    lv_obj_set_x(scr_dashboard_label_mian_inft_18, 204);
    lv_obj_set_y(scr_dashboard_label_mian_inft_18, 308);
    lv_obj_set_width(scr_dashboard_label_mian_inft_18, 100);
    lv_obj_set_height(scr_dashboard_label_mian_inft_18, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_mian_inft_18, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_mian_inft_18, "--");
    lv_label_set_long_mode(scr_dashboard_label_mian_inft_18, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_mian_inft_18 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_mian_inft_18, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_mian_inft_18, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_mian_inft_18, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_detail_badge_18
    scr_dashboard_cont_detail_badge_18 = lv_obj_create(scr_dashboard_cont_detail_18);
    lv_obj_set_x(scr_dashboard_cont_detail_badge_18, 274);
    lv_obj_set_y(scr_dashboard_cont_detail_badge_18, 6);
    lv_obj_set_width(scr_dashboard_cont_detail_badge_18, 72);
    lv_obj_set_height(scr_dashboard_cont_detail_badge_18, 24);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_detail_badge_18, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_detail_badge_18, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_detail_badge_18 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_detail_badge_18, lv_color_hex(0x232C2C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_badge_18, 220, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_detail_badge_18, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_detail_badge_18, 66, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_badge_18, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_detail_badge_18, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_badge_18, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_badge_18, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_badge_18, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_badge_18, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_cont_detail_badge_18 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_cont_detail_badge_18, lv_color_hex(0x3A2A12), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_badge_18, 220, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_cont_detail_badge_18, lv_color_hex(0xE8B84A), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_cont_detail_badge_18, 120, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_badge_18, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_cont_detail_badge_18, 12, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_badge_18, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_badge_18, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_badge_18, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_badge_18, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    ui_flag_modify(scr_dashboard_cont_detail_badge_18, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_18, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_18, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_18, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_18, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_detail_badge_18
    scr_dashboard_label_detail_badge_18 = lv_label_create(scr_dashboard_cont_detail_badge_18);
    lv_obj_set_x(scr_dashboard_label_detail_badge_18, -14);
    lv_obj_set_y(scr_dashboard_label_detail_badge_18, 5);
    lv_obj_set_width(scr_dashboard_label_detail_badge_18, 100);
    lv_obj_set_height(scr_dashboard_label_detail_badge_18, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_badge_18, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_badge_18, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_badge_18, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_badge_18 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_badge_18, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_detail_badge_18, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_label_detail_badge_18 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_text_color(scr_dashboard_label_detail_badge_18, lv_color_hex(0xE8B84A), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(scr_dashboard_label_detail_badge_18, 255, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_badge_18, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_badge_18, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_badge_18, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_badge_18, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_CHECKED);
    // Create scr_dashboard_cont_detail_17
    scr_dashboard_cont_detail_17 = lv_obj_create(scr_dashboard_cont_event_details);
    lv_obj_set_x(scr_dashboard_cont_detail_17, 9);
    lv_obj_set_y(scr_dashboard_cont_detail_17, 7);
    lv_obj_set_width(scr_dashboard_cont_detail_17, 360);
    lv_obj_set_height(scr_dashboard_cont_detail_17, 370);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_detail_17, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_detail_17, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_detail_17 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_detail_17, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_detail_17, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_17, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_17, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_17, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_17, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_detail_result_17
    scr_dashboard_label_detail_result_17 = lv_label_create(scr_dashboard_cont_detail_17);
    lv_obj_set_x(scr_dashboard_label_detail_result_17, 204);
    lv_obj_set_y(scr_dashboard_label_detail_result_17, 114);
    lv_obj_set_width(scr_dashboard_label_detail_result_17, 100);
    lv_obj_set_height(scr_dashboard_label_detail_result_17, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_result_17, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_result_17, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_result_17, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_result_17 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_result_17, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_result_17, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_result_17, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_logo_conf_17
    scr_dashboard_label_logo_conf_17 = lv_label_create(scr_dashboard_cont_detail_17);
    lv_obj_set_x(scr_dashboard_label_logo_conf_17, 204);
    lv_obj_set_y(scr_dashboard_label_logo_conf_17, 164);
    lv_obj_set_width(scr_dashboard_label_logo_conf_17, 100);
    lv_obj_set_height(scr_dashboard_label_logo_conf_17, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_logo_conf_17, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_logo_conf_17, "--");
    lv_label_set_long_mode(scr_dashboard_label_logo_conf_17, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_logo_conf_17 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_logo_conf_17, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_logo_conf_17, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_logo_conf_17, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_logo_conf_17, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_logo_inft_17
    scr_dashboard_label_logo_inft_17 = lv_label_create(scr_dashboard_cont_detail_17);
    lv_obj_set_x(scr_dashboard_label_logo_inft_17, 204);
    lv_obj_set_y(scr_dashboard_label_logo_inft_17, 261);
    lv_obj_set_width(scr_dashboard_label_logo_inft_17, 100);
    lv_obj_set_height(scr_dashboard_label_logo_inft_17, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_logo_inft_17, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_logo_inft_17, "--");
    lv_label_set_long_mode(scr_dashboard_label_logo_inft_17, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_logo_inft_17 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_logo_inft_17, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_logo_inft_17, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_logo_inft_17, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_detail_time_17
    scr_dashboard_label_detail_time_17 = lv_label_create(scr_dashboard_cont_detail_17);
    lv_obj_set_x(scr_dashboard_label_detail_time_17, 204);
    lv_obj_set_y(scr_dashboard_label_detail_time_17, 68);
    lv_obj_set_width(scr_dashboard_label_detail_time_17, 100);
    lv_obj_set_height(scr_dashboard_label_detail_time_17, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_time_17, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_time_17, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_time_17, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_time_17 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_time_17, lv_color_hex(0x718A92), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_time_17, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_time_17, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_mian_conf_17
    scr_dashboard_label_mian_conf_17 = lv_label_create(scr_dashboard_cont_detail_17);
    lv_obj_set_x(scr_dashboard_label_mian_conf_17, 204);
    lv_obj_set_y(scr_dashboard_label_mian_conf_17, 210);
    lv_obj_set_width(scr_dashboard_label_mian_conf_17, 100);
    lv_obj_set_height(scr_dashboard_label_mian_conf_17, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_mian_conf_17, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_mian_conf_17, "--");
    lv_label_set_long_mode(scr_dashboard_label_mian_conf_17, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_mian_conf_17 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_mian_conf_17, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_mian_conf_17, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_mian_conf_17, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_mian_inft_17
    scr_dashboard_label_mian_inft_17 = lv_label_create(scr_dashboard_cont_detail_17);
    lv_obj_set_x(scr_dashboard_label_mian_inft_17, 204);
    lv_obj_set_y(scr_dashboard_label_mian_inft_17, 308);
    lv_obj_set_width(scr_dashboard_label_mian_inft_17, 100);
    lv_obj_set_height(scr_dashboard_label_mian_inft_17, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_mian_inft_17, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_mian_inft_17, "--");
    lv_label_set_long_mode(scr_dashboard_label_mian_inft_17, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_mian_inft_17 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_mian_inft_17, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_mian_inft_17, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_mian_inft_17, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_detail_badge_17
    scr_dashboard_cont_detail_badge_17 = lv_obj_create(scr_dashboard_cont_detail_17);
    lv_obj_set_x(scr_dashboard_cont_detail_badge_17, 274);
    lv_obj_set_y(scr_dashboard_cont_detail_badge_17, 6);
    lv_obj_set_width(scr_dashboard_cont_detail_badge_17, 72);
    lv_obj_set_height(scr_dashboard_cont_detail_badge_17, 24);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_detail_badge_17, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_detail_badge_17, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_detail_badge_17 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_detail_badge_17, lv_color_hex(0x232C2C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_badge_17, 220, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_detail_badge_17, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_detail_badge_17, 66, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_badge_17, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_detail_badge_17, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_badge_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_badge_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_badge_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_badge_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_cont_detail_badge_17 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_cont_detail_badge_17, lv_color_hex(0x3A2A12), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_badge_17, 220, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_cont_detail_badge_17, lv_color_hex(0xE8B84A), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_cont_detail_badge_17, 120, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_badge_17, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_cont_detail_badge_17, 12, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_badge_17, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_badge_17, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_badge_17, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_badge_17, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    ui_flag_modify(scr_dashboard_cont_detail_badge_17, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_17, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_17, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_17, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_17, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_detail_badge_17
    scr_dashboard_label_detail_badge_17 = lv_label_create(scr_dashboard_cont_detail_badge_17);
    lv_obj_set_x(scr_dashboard_label_detail_badge_17, -14);
    lv_obj_set_y(scr_dashboard_label_detail_badge_17, 5);
    lv_obj_set_width(scr_dashboard_label_detail_badge_17, 100);
    lv_obj_set_height(scr_dashboard_label_detail_badge_17, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_badge_17, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_badge_17, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_badge_17, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_badge_17 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_badge_17, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_detail_badge_17, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_label_detail_badge_17 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_text_color(scr_dashboard_label_detail_badge_17, lv_color_hex(0xE8B84A), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(scr_dashboard_label_detail_badge_17, 255, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_badge_17, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_badge_17, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_badge_17, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_badge_17, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_CHECKED);
    // Create scr_dashboard_cont_detail_16
    scr_dashboard_cont_detail_16 = lv_obj_create(scr_dashboard_cont_event_details);
    lv_obj_set_x(scr_dashboard_cont_detail_16, 9);
    lv_obj_set_y(scr_dashboard_cont_detail_16, 7);
    lv_obj_set_width(scr_dashboard_cont_detail_16, 360);
    lv_obj_set_height(scr_dashboard_cont_detail_16, 370);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_detail_16, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_detail_16, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_detail_16 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_detail_16, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_detail_16, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_16, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_16, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_16, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_16, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_detail_result_16
    scr_dashboard_label_detail_result_16 = lv_label_create(scr_dashboard_cont_detail_16);
    lv_obj_set_x(scr_dashboard_label_detail_result_16, 204);
    lv_obj_set_y(scr_dashboard_label_detail_result_16, 114);
    lv_obj_set_width(scr_dashboard_label_detail_result_16, 100);
    lv_obj_set_height(scr_dashboard_label_detail_result_16, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_result_16, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_result_16, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_result_16, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_result_16 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_result_16, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_result_16, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_result_16, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_logo_conf_16
    scr_dashboard_label_logo_conf_16 = lv_label_create(scr_dashboard_cont_detail_16);
    lv_obj_set_x(scr_dashboard_label_logo_conf_16, 204);
    lv_obj_set_y(scr_dashboard_label_logo_conf_16, 164);
    lv_obj_set_width(scr_dashboard_label_logo_conf_16, 100);
    lv_obj_set_height(scr_dashboard_label_logo_conf_16, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_logo_conf_16, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_logo_conf_16, "--");
    lv_label_set_long_mode(scr_dashboard_label_logo_conf_16, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_logo_conf_16 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_logo_conf_16, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_logo_conf_16, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_logo_conf_16, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_logo_conf_16, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_logo_inft_16
    scr_dashboard_label_logo_inft_16 = lv_label_create(scr_dashboard_cont_detail_16);
    lv_obj_set_x(scr_dashboard_label_logo_inft_16, 204);
    lv_obj_set_y(scr_dashboard_label_logo_inft_16, 261);
    lv_obj_set_width(scr_dashboard_label_logo_inft_16, 100);
    lv_obj_set_height(scr_dashboard_label_logo_inft_16, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_logo_inft_16, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_logo_inft_16, "--");
    lv_label_set_long_mode(scr_dashboard_label_logo_inft_16, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_logo_inft_16 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_logo_inft_16, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_logo_inft_16, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_logo_inft_16, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_detail_time_16
    scr_dashboard_label_detail_time_16 = lv_label_create(scr_dashboard_cont_detail_16);
    lv_obj_set_x(scr_dashboard_label_detail_time_16, 204);
    lv_obj_set_y(scr_dashboard_label_detail_time_16, 68);
    lv_obj_set_width(scr_dashboard_label_detail_time_16, 100);
    lv_obj_set_height(scr_dashboard_label_detail_time_16, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_time_16, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_time_16, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_time_16, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_time_16 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_time_16, lv_color_hex(0x718A92), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_time_16, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_time_16, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_mian_conf_16
    scr_dashboard_label_mian_conf_16 = lv_label_create(scr_dashboard_cont_detail_16);
    lv_obj_set_x(scr_dashboard_label_mian_conf_16, 204);
    lv_obj_set_y(scr_dashboard_label_mian_conf_16, 210);
    lv_obj_set_width(scr_dashboard_label_mian_conf_16, 100);
    lv_obj_set_height(scr_dashboard_label_mian_conf_16, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_mian_conf_16, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_mian_conf_16, "--");
    lv_label_set_long_mode(scr_dashboard_label_mian_conf_16, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_mian_conf_16 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_mian_conf_16, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_mian_conf_16, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_mian_conf_16, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_mian_inft_16
    scr_dashboard_label_mian_inft_16 = lv_label_create(scr_dashboard_cont_detail_16);
    lv_obj_set_x(scr_dashboard_label_mian_inft_16, 204);
    lv_obj_set_y(scr_dashboard_label_mian_inft_16, 308);
    lv_obj_set_width(scr_dashboard_label_mian_inft_16, 100);
    lv_obj_set_height(scr_dashboard_label_mian_inft_16, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_mian_inft_16, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_mian_inft_16, "--");
    lv_label_set_long_mode(scr_dashboard_label_mian_inft_16, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_mian_inft_16 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_mian_inft_16, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_mian_inft_16, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_mian_inft_16, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_detail_badge_16
    scr_dashboard_cont_detail_badge_16 = lv_obj_create(scr_dashboard_cont_detail_16);
    lv_obj_set_x(scr_dashboard_cont_detail_badge_16, 274);
    lv_obj_set_y(scr_dashboard_cont_detail_badge_16, 6);
    lv_obj_set_width(scr_dashboard_cont_detail_badge_16, 72);
    lv_obj_set_height(scr_dashboard_cont_detail_badge_16, 24);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_detail_badge_16, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_detail_badge_16, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_detail_badge_16 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_detail_badge_16, lv_color_hex(0x232C2C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_badge_16, 220, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_detail_badge_16, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_detail_badge_16, 66, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_badge_16, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_detail_badge_16, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_badge_16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_badge_16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_badge_16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_badge_16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_cont_detail_badge_16 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_cont_detail_badge_16, lv_color_hex(0x3A2A12), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_badge_16, 220, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_cont_detail_badge_16, lv_color_hex(0xE8B84A), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_cont_detail_badge_16, 120, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_badge_16, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_cont_detail_badge_16, 12, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_badge_16, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_badge_16, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_badge_16, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_badge_16, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    ui_flag_modify(scr_dashboard_cont_detail_badge_16, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_16, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_16, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_16, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_16, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_detail_badge_16
    scr_dashboard_label_detail_badge_16 = lv_label_create(scr_dashboard_cont_detail_badge_16);
    lv_obj_set_x(scr_dashboard_label_detail_badge_16, -14);
    lv_obj_set_y(scr_dashboard_label_detail_badge_16, 5);
    lv_obj_set_width(scr_dashboard_label_detail_badge_16, 100);
    lv_obj_set_height(scr_dashboard_label_detail_badge_16, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_badge_16, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_badge_16, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_badge_16, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_badge_16 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_badge_16, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_detail_badge_16, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_label_detail_badge_16 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_text_color(scr_dashboard_label_detail_badge_16, lv_color_hex(0xE8B84A), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(scr_dashboard_label_detail_badge_16, 255, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_badge_16, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_badge_16, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_badge_16, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_badge_16, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_CHECKED);
    // Create scr_dashboard_cont_detail_15
    scr_dashboard_cont_detail_15 = lv_obj_create(scr_dashboard_cont_event_details);
    lv_obj_set_x(scr_dashboard_cont_detail_15, 9);
    lv_obj_set_y(scr_dashboard_cont_detail_15, 7);
    lv_obj_set_width(scr_dashboard_cont_detail_15, 360);
    lv_obj_set_height(scr_dashboard_cont_detail_15, 370);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_detail_15, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_detail_15, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_detail_15 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_detail_15, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_detail_15, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_15, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_15, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_15, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_15, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_detail_result_15
    scr_dashboard_label_detail_result_15 = lv_label_create(scr_dashboard_cont_detail_15);
    lv_obj_set_x(scr_dashboard_label_detail_result_15, 204);
    lv_obj_set_y(scr_dashboard_label_detail_result_15, 114);
    lv_obj_set_width(scr_dashboard_label_detail_result_15, 100);
    lv_obj_set_height(scr_dashboard_label_detail_result_15, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_result_15, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_result_15, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_result_15, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_result_15 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_result_15, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_result_15, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_result_15, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_logo_conf_15
    scr_dashboard_label_logo_conf_15 = lv_label_create(scr_dashboard_cont_detail_15);
    lv_obj_set_x(scr_dashboard_label_logo_conf_15, 204);
    lv_obj_set_y(scr_dashboard_label_logo_conf_15, 164);
    lv_obj_set_width(scr_dashboard_label_logo_conf_15, 100);
    lv_obj_set_height(scr_dashboard_label_logo_conf_15, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_logo_conf_15, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_logo_conf_15, "--");
    lv_label_set_long_mode(scr_dashboard_label_logo_conf_15, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_logo_conf_15 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_logo_conf_15, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_logo_conf_15, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_logo_conf_15, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_logo_conf_15, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_logo_inft_15
    scr_dashboard_label_logo_inft_15 = lv_label_create(scr_dashboard_cont_detail_15);
    lv_obj_set_x(scr_dashboard_label_logo_inft_15, 204);
    lv_obj_set_y(scr_dashboard_label_logo_inft_15, 261);
    lv_obj_set_width(scr_dashboard_label_logo_inft_15, 100);
    lv_obj_set_height(scr_dashboard_label_logo_inft_15, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_logo_inft_15, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_logo_inft_15, "--");
    lv_label_set_long_mode(scr_dashboard_label_logo_inft_15, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_logo_inft_15 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_logo_inft_15, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_logo_inft_15, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_logo_inft_15, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_detail_time_15
    scr_dashboard_label_detail_time_15 = lv_label_create(scr_dashboard_cont_detail_15);
    lv_obj_set_x(scr_dashboard_label_detail_time_15, 204);
    lv_obj_set_y(scr_dashboard_label_detail_time_15, 68);
    lv_obj_set_width(scr_dashboard_label_detail_time_15, 100);
    lv_obj_set_height(scr_dashboard_label_detail_time_15, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_time_15, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_time_15, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_time_15, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_time_15 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_time_15, lv_color_hex(0x718A92), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_time_15, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_time_15, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_mian_conf_15
    scr_dashboard_label_mian_conf_15 = lv_label_create(scr_dashboard_cont_detail_15);
    lv_obj_set_x(scr_dashboard_label_mian_conf_15, 204);
    lv_obj_set_y(scr_dashboard_label_mian_conf_15, 210);
    lv_obj_set_width(scr_dashboard_label_mian_conf_15, 100);
    lv_obj_set_height(scr_dashboard_label_mian_conf_15, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_mian_conf_15, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_mian_conf_15, "--");
    lv_label_set_long_mode(scr_dashboard_label_mian_conf_15, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_mian_conf_15 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_mian_conf_15, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_mian_conf_15, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_mian_conf_15, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_mian_inft_15
    scr_dashboard_label_mian_inft_15 = lv_label_create(scr_dashboard_cont_detail_15);
    lv_obj_set_x(scr_dashboard_label_mian_inft_15, 204);
    lv_obj_set_y(scr_dashboard_label_mian_inft_15, 308);
    lv_obj_set_width(scr_dashboard_label_mian_inft_15, 100);
    lv_obj_set_height(scr_dashboard_label_mian_inft_15, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_mian_inft_15, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_mian_inft_15, "--");
    lv_label_set_long_mode(scr_dashboard_label_mian_inft_15, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_mian_inft_15 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_mian_inft_15, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_mian_inft_15, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_mian_inft_15, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_detail_badge_15
    scr_dashboard_cont_detail_badge_15 = lv_obj_create(scr_dashboard_cont_detail_15);
    lv_obj_set_x(scr_dashboard_cont_detail_badge_15, 274);
    lv_obj_set_y(scr_dashboard_cont_detail_badge_15, 6);
    lv_obj_set_width(scr_dashboard_cont_detail_badge_15, 72);
    lv_obj_set_height(scr_dashboard_cont_detail_badge_15, 24);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_detail_badge_15, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_detail_badge_15, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_detail_badge_15 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_detail_badge_15, lv_color_hex(0x232C2C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_badge_15, 220, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_detail_badge_15, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_detail_badge_15, 66, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_badge_15, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_detail_badge_15, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_badge_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_badge_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_badge_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_badge_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_cont_detail_badge_15 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_cont_detail_badge_15, lv_color_hex(0x3A2A12), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_badge_15, 220, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_cont_detail_badge_15, lv_color_hex(0xE8B84A), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_cont_detail_badge_15, 120, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_badge_15, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_cont_detail_badge_15, 12, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_badge_15, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_badge_15, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_badge_15, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_badge_15, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    ui_flag_modify(scr_dashboard_cont_detail_badge_15, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_15, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_15, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_15, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_15, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_detail_badge_15
    scr_dashboard_label_detail_badge_15 = lv_label_create(scr_dashboard_cont_detail_badge_15);
    lv_obj_set_x(scr_dashboard_label_detail_badge_15, -14);
    lv_obj_set_y(scr_dashboard_label_detail_badge_15, 5);
    lv_obj_set_width(scr_dashboard_label_detail_badge_15, 100);
    lv_obj_set_height(scr_dashboard_label_detail_badge_15, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_badge_15, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_badge_15, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_badge_15, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_badge_15 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_badge_15, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_detail_badge_15, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_label_detail_badge_15 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_text_color(scr_dashboard_label_detail_badge_15, lv_color_hex(0xE8B84A), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(scr_dashboard_label_detail_badge_15, 255, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_badge_15, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_badge_15, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_badge_15, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_badge_15, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_CHECKED);
    // Create scr_dashboard_cont_detail_14
    scr_dashboard_cont_detail_14 = lv_obj_create(scr_dashboard_cont_event_details);
    lv_obj_set_x(scr_dashboard_cont_detail_14, 9);
    lv_obj_set_y(scr_dashboard_cont_detail_14, 7);
    lv_obj_set_width(scr_dashboard_cont_detail_14, 360);
    lv_obj_set_height(scr_dashboard_cont_detail_14, 370);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_detail_14, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_detail_14, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_detail_14 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_detail_14, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_detail_14, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_14, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_14, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_14, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_14, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_detail_result_14
    scr_dashboard_label_detail_result_14 = lv_label_create(scr_dashboard_cont_detail_14);
    lv_obj_set_x(scr_dashboard_label_detail_result_14, 204);
    lv_obj_set_y(scr_dashboard_label_detail_result_14, 114);
    lv_obj_set_width(scr_dashboard_label_detail_result_14, 100);
    lv_obj_set_height(scr_dashboard_label_detail_result_14, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_result_14, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_result_14, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_result_14, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_result_14 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_result_14, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_result_14, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_result_14, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_logo_conf_14
    scr_dashboard_label_logo_conf_14 = lv_label_create(scr_dashboard_cont_detail_14);
    lv_obj_set_x(scr_dashboard_label_logo_conf_14, 204);
    lv_obj_set_y(scr_dashboard_label_logo_conf_14, 164);
    lv_obj_set_width(scr_dashboard_label_logo_conf_14, 100);
    lv_obj_set_height(scr_dashboard_label_logo_conf_14, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_logo_conf_14, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_logo_conf_14, "--");
    lv_label_set_long_mode(scr_dashboard_label_logo_conf_14, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_logo_conf_14 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_logo_conf_14, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_logo_conf_14, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_logo_conf_14, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_logo_conf_14, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_logo_inft_14
    scr_dashboard_label_logo_inft_14 = lv_label_create(scr_dashboard_cont_detail_14);
    lv_obj_set_x(scr_dashboard_label_logo_inft_14, 204);
    lv_obj_set_y(scr_dashboard_label_logo_inft_14, 261);
    lv_obj_set_width(scr_dashboard_label_logo_inft_14, 100);
    lv_obj_set_height(scr_dashboard_label_logo_inft_14, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_logo_inft_14, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_logo_inft_14, "--");
    lv_label_set_long_mode(scr_dashboard_label_logo_inft_14, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_logo_inft_14 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_logo_inft_14, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_logo_inft_14, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_logo_inft_14, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_detail_time_14
    scr_dashboard_label_detail_time_14 = lv_label_create(scr_dashboard_cont_detail_14);
    lv_obj_set_x(scr_dashboard_label_detail_time_14, 204);
    lv_obj_set_y(scr_dashboard_label_detail_time_14, 68);
    lv_obj_set_width(scr_dashboard_label_detail_time_14, 100);
    lv_obj_set_height(scr_dashboard_label_detail_time_14, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_time_14, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_time_14, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_time_14, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_time_14 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_time_14, lv_color_hex(0x718A92), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_time_14, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_time_14, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_mian_conf_14
    scr_dashboard_label_mian_conf_14 = lv_label_create(scr_dashboard_cont_detail_14);
    lv_obj_set_x(scr_dashboard_label_mian_conf_14, 204);
    lv_obj_set_y(scr_dashboard_label_mian_conf_14, 210);
    lv_obj_set_width(scr_dashboard_label_mian_conf_14, 100);
    lv_obj_set_height(scr_dashboard_label_mian_conf_14, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_mian_conf_14, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_mian_conf_14, "--");
    lv_label_set_long_mode(scr_dashboard_label_mian_conf_14, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_mian_conf_14 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_mian_conf_14, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_mian_conf_14, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_mian_conf_14, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_mian_inft_14
    scr_dashboard_label_mian_inft_14 = lv_label_create(scr_dashboard_cont_detail_14);
    lv_obj_set_x(scr_dashboard_label_mian_inft_14, 204);
    lv_obj_set_y(scr_dashboard_label_mian_inft_14, 308);
    lv_obj_set_width(scr_dashboard_label_mian_inft_14, 100);
    lv_obj_set_height(scr_dashboard_label_mian_inft_14, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_mian_inft_14, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_mian_inft_14, "--");
    lv_label_set_long_mode(scr_dashboard_label_mian_inft_14, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_mian_inft_14 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_mian_inft_14, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_mian_inft_14, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_mian_inft_14, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_detail_badge_14
    scr_dashboard_cont_detail_badge_14 = lv_obj_create(scr_dashboard_cont_detail_14);
    lv_obj_set_x(scr_dashboard_cont_detail_badge_14, 274);
    lv_obj_set_y(scr_dashboard_cont_detail_badge_14, 6);
    lv_obj_set_width(scr_dashboard_cont_detail_badge_14, 72);
    lv_obj_set_height(scr_dashboard_cont_detail_badge_14, 24);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_detail_badge_14, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_detail_badge_14, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_detail_badge_14 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_detail_badge_14, lv_color_hex(0x232C2C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_badge_14, 220, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_detail_badge_14, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_detail_badge_14, 66, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_badge_14, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_detail_badge_14, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_badge_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_badge_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_badge_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_badge_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_cont_detail_badge_14 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_cont_detail_badge_14, lv_color_hex(0x3A2A12), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_badge_14, 220, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_cont_detail_badge_14, lv_color_hex(0xE8B84A), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_cont_detail_badge_14, 120, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_badge_14, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_cont_detail_badge_14, 12, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_badge_14, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_badge_14, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_badge_14, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_badge_14, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    ui_flag_modify(scr_dashboard_cont_detail_badge_14, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_14, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_14, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_14, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_14, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_detail_badge_14
    scr_dashboard_label_detail_badge_14 = lv_label_create(scr_dashboard_cont_detail_badge_14);
    lv_obj_set_x(scr_dashboard_label_detail_badge_14, -14);
    lv_obj_set_y(scr_dashboard_label_detail_badge_14, 5);
    lv_obj_set_width(scr_dashboard_label_detail_badge_14, 100);
    lv_obj_set_height(scr_dashboard_label_detail_badge_14, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_badge_14, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_badge_14, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_badge_14, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_badge_14 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_badge_14, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_detail_badge_14, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_label_detail_badge_14 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_text_color(scr_dashboard_label_detail_badge_14, lv_color_hex(0xE8B84A), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(scr_dashboard_label_detail_badge_14, 255, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_badge_14, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_badge_14, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_badge_14, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_badge_14, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_CHECKED);
    // Create scr_dashboard_cont_detail_13
    scr_dashboard_cont_detail_13 = lv_obj_create(scr_dashboard_cont_event_details);
    lv_obj_set_x(scr_dashboard_cont_detail_13, 9);
    lv_obj_set_y(scr_dashboard_cont_detail_13, 7);
    lv_obj_set_width(scr_dashboard_cont_detail_13, 360);
    lv_obj_set_height(scr_dashboard_cont_detail_13, 370);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_detail_13, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_detail_13, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_detail_13 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_detail_13, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_detail_13, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_13, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_13, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_13, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_13, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_detail_result_13
    scr_dashboard_label_detail_result_13 = lv_label_create(scr_dashboard_cont_detail_13);
    lv_obj_set_x(scr_dashboard_label_detail_result_13, 204);
    lv_obj_set_y(scr_dashboard_label_detail_result_13, 114);
    lv_obj_set_width(scr_dashboard_label_detail_result_13, 100);
    lv_obj_set_height(scr_dashboard_label_detail_result_13, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_result_13, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_result_13, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_result_13, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_result_13 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_result_13, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_result_13, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_result_13, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_logo_conf_13
    scr_dashboard_label_logo_conf_13 = lv_label_create(scr_dashboard_cont_detail_13);
    lv_obj_set_x(scr_dashboard_label_logo_conf_13, 204);
    lv_obj_set_y(scr_dashboard_label_logo_conf_13, 164);
    lv_obj_set_width(scr_dashboard_label_logo_conf_13, 100);
    lv_obj_set_height(scr_dashboard_label_logo_conf_13, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_logo_conf_13, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_logo_conf_13, "--");
    lv_label_set_long_mode(scr_dashboard_label_logo_conf_13, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_logo_conf_13 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_logo_conf_13, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_logo_conf_13, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_logo_conf_13, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_logo_conf_13, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_logo_inft_13
    scr_dashboard_label_logo_inft_13 = lv_label_create(scr_dashboard_cont_detail_13);
    lv_obj_set_x(scr_dashboard_label_logo_inft_13, 204);
    lv_obj_set_y(scr_dashboard_label_logo_inft_13, 261);
    lv_obj_set_width(scr_dashboard_label_logo_inft_13, 100);
    lv_obj_set_height(scr_dashboard_label_logo_inft_13, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_logo_inft_13, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_logo_inft_13, "--");
    lv_label_set_long_mode(scr_dashboard_label_logo_inft_13, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_logo_inft_13 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_logo_inft_13, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_logo_inft_13, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_logo_inft_13, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_detail_time_13
    scr_dashboard_label_detail_time_13 = lv_label_create(scr_dashboard_cont_detail_13);
    lv_obj_set_x(scr_dashboard_label_detail_time_13, 204);
    lv_obj_set_y(scr_dashboard_label_detail_time_13, 68);
    lv_obj_set_width(scr_dashboard_label_detail_time_13, 100);
    lv_obj_set_height(scr_dashboard_label_detail_time_13, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_time_13, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_time_13, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_time_13, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_time_13 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_time_13, lv_color_hex(0x718A92), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_time_13, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_time_13, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_mian_conf_13
    scr_dashboard_label_mian_conf_13 = lv_label_create(scr_dashboard_cont_detail_13);
    lv_obj_set_x(scr_dashboard_label_mian_conf_13, 204);
    lv_obj_set_y(scr_dashboard_label_mian_conf_13, 210);
    lv_obj_set_width(scr_dashboard_label_mian_conf_13, 100);
    lv_obj_set_height(scr_dashboard_label_mian_conf_13, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_mian_conf_13, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_mian_conf_13, "--");
    lv_label_set_long_mode(scr_dashboard_label_mian_conf_13, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_mian_conf_13 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_mian_conf_13, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_mian_conf_13, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_mian_conf_13, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_mian_inft_13
    scr_dashboard_label_mian_inft_13 = lv_label_create(scr_dashboard_cont_detail_13);
    lv_obj_set_x(scr_dashboard_label_mian_inft_13, 204);
    lv_obj_set_y(scr_dashboard_label_mian_inft_13, 308);
    lv_obj_set_width(scr_dashboard_label_mian_inft_13, 100);
    lv_obj_set_height(scr_dashboard_label_mian_inft_13, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_mian_inft_13, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_mian_inft_13, "--");
    lv_label_set_long_mode(scr_dashboard_label_mian_inft_13, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_mian_inft_13 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_mian_inft_13, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_mian_inft_13, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_mian_inft_13, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_detail_badge_13
    scr_dashboard_cont_detail_badge_13 = lv_obj_create(scr_dashboard_cont_detail_13);
    lv_obj_set_x(scr_dashboard_cont_detail_badge_13, 274);
    lv_obj_set_y(scr_dashboard_cont_detail_badge_13, 6);
    lv_obj_set_width(scr_dashboard_cont_detail_badge_13, 72);
    lv_obj_set_height(scr_dashboard_cont_detail_badge_13, 24);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_detail_badge_13, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_detail_badge_13, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_detail_badge_13 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_detail_badge_13, lv_color_hex(0x232C2C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_badge_13, 220, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_detail_badge_13, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_detail_badge_13, 66, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_badge_13, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_detail_badge_13, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_badge_13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_badge_13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_badge_13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_badge_13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_cont_detail_badge_13 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_cont_detail_badge_13, lv_color_hex(0x3A2A12), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_badge_13, 220, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_cont_detail_badge_13, lv_color_hex(0xE8B84A), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_cont_detail_badge_13, 120, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_badge_13, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_cont_detail_badge_13, 12, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_badge_13, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_badge_13, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_badge_13, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_badge_13, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    ui_flag_modify(scr_dashboard_cont_detail_badge_13, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_13, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_13, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_13, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_13, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_detail_badge_13
    scr_dashboard_label_detail_badge_13 = lv_label_create(scr_dashboard_cont_detail_badge_13);
    lv_obj_set_x(scr_dashboard_label_detail_badge_13, -14);
    lv_obj_set_y(scr_dashboard_label_detail_badge_13, 5);
    lv_obj_set_width(scr_dashboard_label_detail_badge_13, 100);
    lv_obj_set_height(scr_dashboard_label_detail_badge_13, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_badge_13, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_badge_13, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_badge_13, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_badge_13 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_badge_13, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_detail_badge_13, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_label_detail_badge_13 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_text_color(scr_dashboard_label_detail_badge_13, lv_color_hex(0xE8B84A), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(scr_dashboard_label_detail_badge_13, 255, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_badge_13, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_badge_13, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_badge_13, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_badge_13, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_CHECKED);
    // Create scr_dashboard_cont_detail_12
    scr_dashboard_cont_detail_12 = lv_obj_create(scr_dashboard_cont_event_details);
    lv_obj_set_x(scr_dashboard_cont_detail_12, 9);
    lv_obj_set_y(scr_dashboard_cont_detail_12, 7);
    lv_obj_set_width(scr_dashboard_cont_detail_12, 360);
    lv_obj_set_height(scr_dashboard_cont_detail_12, 370);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_detail_12, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_detail_12, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_detail_12 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_detail_12, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_detail_12, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_12, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_12, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_12, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_12, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_detail_result_12
    scr_dashboard_label_detail_result_12 = lv_label_create(scr_dashboard_cont_detail_12);
    lv_obj_set_x(scr_dashboard_label_detail_result_12, 204);
    lv_obj_set_y(scr_dashboard_label_detail_result_12, 114);
    lv_obj_set_width(scr_dashboard_label_detail_result_12, 100);
    lv_obj_set_height(scr_dashboard_label_detail_result_12, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_result_12, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_result_12, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_result_12, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_result_12 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_result_12, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_result_12, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_result_12, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_logo_conf_12
    scr_dashboard_label_logo_conf_12 = lv_label_create(scr_dashboard_cont_detail_12);
    lv_obj_set_x(scr_dashboard_label_logo_conf_12, 204);
    lv_obj_set_y(scr_dashboard_label_logo_conf_12, 164);
    lv_obj_set_width(scr_dashboard_label_logo_conf_12, 100);
    lv_obj_set_height(scr_dashboard_label_logo_conf_12, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_logo_conf_12, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_logo_conf_12, "--");
    lv_label_set_long_mode(scr_dashboard_label_logo_conf_12, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_logo_conf_12 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_logo_conf_12, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_logo_conf_12, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_logo_conf_12, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_logo_conf_12, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_logo_inft_12
    scr_dashboard_label_logo_inft_12 = lv_label_create(scr_dashboard_cont_detail_12);
    lv_obj_set_x(scr_dashboard_label_logo_inft_12, 204);
    lv_obj_set_y(scr_dashboard_label_logo_inft_12, 261);
    lv_obj_set_width(scr_dashboard_label_logo_inft_12, 100);
    lv_obj_set_height(scr_dashboard_label_logo_inft_12, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_logo_inft_12, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_logo_inft_12, "--");
    lv_label_set_long_mode(scr_dashboard_label_logo_inft_12, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_logo_inft_12 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_logo_inft_12, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_logo_inft_12, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_logo_inft_12, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_detail_time_12
    scr_dashboard_label_detail_time_12 = lv_label_create(scr_dashboard_cont_detail_12);
    lv_obj_set_x(scr_dashboard_label_detail_time_12, 204);
    lv_obj_set_y(scr_dashboard_label_detail_time_12, 68);
    lv_obj_set_width(scr_dashboard_label_detail_time_12, 100);
    lv_obj_set_height(scr_dashboard_label_detail_time_12, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_time_12, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_time_12, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_time_12, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_time_12 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_time_12, lv_color_hex(0x718A92), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_time_12, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_time_12, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_mian_conf_12
    scr_dashboard_label_mian_conf_12 = lv_label_create(scr_dashboard_cont_detail_12);
    lv_obj_set_x(scr_dashboard_label_mian_conf_12, 204);
    lv_obj_set_y(scr_dashboard_label_mian_conf_12, 210);
    lv_obj_set_width(scr_dashboard_label_mian_conf_12, 100);
    lv_obj_set_height(scr_dashboard_label_mian_conf_12, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_mian_conf_12, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_mian_conf_12, "--");
    lv_label_set_long_mode(scr_dashboard_label_mian_conf_12, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_mian_conf_12 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_mian_conf_12, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_mian_conf_12, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_mian_conf_12, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_mian_inft_12
    scr_dashboard_label_mian_inft_12 = lv_label_create(scr_dashboard_cont_detail_12);
    lv_obj_set_x(scr_dashboard_label_mian_inft_12, 204);
    lv_obj_set_y(scr_dashboard_label_mian_inft_12, 308);
    lv_obj_set_width(scr_dashboard_label_mian_inft_12, 100);
    lv_obj_set_height(scr_dashboard_label_mian_inft_12, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_mian_inft_12, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_mian_inft_12, "--");
    lv_label_set_long_mode(scr_dashboard_label_mian_inft_12, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_mian_inft_12 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_mian_inft_12, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_mian_inft_12, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_mian_inft_12, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_detail_badge_12
    scr_dashboard_cont_detail_badge_12 = lv_obj_create(scr_dashboard_cont_detail_12);
    lv_obj_set_x(scr_dashboard_cont_detail_badge_12, 274);
    lv_obj_set_y(scr_dashboard_cont_detail_badge_12, 6);
    lv_obj_set_width(scr_dashboard_cont_detail_badge_12, 72);
    lv_obj_set_height(scr_dashboard_cont_detail_badge_12, 24);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_detail_badge_12, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_detail_badge_12, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_detail_badge_12 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_detail_badge_12, lv_color_hex(0x232C2C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_badge_12, 220, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_detail_badge_12, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_detail_badge_12, 66, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_badge_12, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_detail_badge_12, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_badge_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_badge_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_badge_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_badge_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_cont_detail_badge_12 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_cont_detail_badge_12, lv_color_hex(0x3A2A12), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_badge_12, 220, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_cont_detail_badge_12, lv_color_hex(0xE8B84A), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_cont_detail_badge_12, 120, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_badge_12, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_cont_detail_badge_12, 12, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_badge_12, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_badge_12, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_badge_12, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_badge_12, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    ui_flag_modify(scr_dashboard_cont_detail_badge_12, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_12, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_12, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_12, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_12, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_detail_badge_12
    scr_dashboard_label_detail_badge_12 = lv_label_create(scr_dashboard_cont_detail_badge_12);
    lv_obj_set_x(scr_dashboard_label_detail_badge_12, -14);
    lv_obj_set_y(scr_dashboard_label_detail_badge_12, 5);
    lv_obj_set_width(scr_dashboard_label_detail_badge_12, 100);
    lv_obj_set_height(scr_dashboard_label_detail_badge_12, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_badge_12, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_badge_12, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_badge_12, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_badge_12 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_badge_12, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_detail_badge_12, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_label_detail_badge_12 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_text_color(scr_dashboard_label_detail_badge_12, lv_color_hex(0xE8B84A), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(scr_dashboard_label_detail_badge_12, 255, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_badge_12, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_badge_12, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_badge_12, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_badge_12, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_CHECKED);
    // Create scr_dashboard_cont_detail_11
    scr_dashboard_cont_detail_11 = lv_obj_create(scr_dashboard_cont_event_details);
    lv_obj_set_x(scr_dashboard_cont_detail_11, 9);
    lv_obj_set_y(scr_dashboard_cont_detail_11, 7);
    lv_obj_set_width(scr_dashboard_cont_detail_11, 360);
    lv_obj_set_height(scr_dashboard_cont_detail_11, 370);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_detail_11, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_detail_11, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_detail_11 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_detail_11, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_detail_11, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_11, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_11, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_11, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_11, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_detail_result_11
    scr_dashboard_label_detail_result_11 = lv_label_create(scr_dashboard_cont_detail_11);
    lv_obj_set_x(scr_dashboard_label_detail_result_11, 204);
    lv_obj_set_y(scr_dashboard_label_detail_result_11, 114);
    lv_obj_set_width(scr_dashboard_label_detail_result_11, 100);
    lv_obj_set_height(scr_dashboard_label_detail_result_11, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_result_11, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_result_11, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_result_11, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_result_11 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_result_11, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_result_11, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_result_11, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_logo_conf_11
    scr_dashboard_label_logo_conf_11 = lv_label_create(scr_dashboard_cont_detail_11);
    lv_obj_set_x(scr_dashboard_label_logo_conf_11, 204);
    lv_obj_set_y(scr_dashboard_label_logo_conf_11, 164);
    lv_obj_set_width(scr_dashboard_label_logo_conf_11, 100);
    lv_obj_set_height(scr_dashboard_label_logo_conf_11, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_logo_conf_11, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_logo_conf_11, "--");
    lv_label_set_long_mode(scr_dashboard_label_logo_conf_11, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_logo_conf_11 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_logo_conf_11, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_logo_conf_11, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_logo_conf_11, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_logo_conf_11, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_logo_inft_11
    scr_dashboard_label_logo_inft_11 = lv_label_create(scr_dashboard_cont_detail_11);
    lv_obj_set_x(scr_dashboard_label_logo_inft_11, 204);
    lv_obj_set_y(scr_dashboard_label_logo_inft_11, 261);
    lv_obj_set_width(scr_dashboard_label_logo_inft_11, 100);
    lv_obj_set_height(scr_dashboard_label_logo_inft_11, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_logo_inft_11, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_logo_inft_11, "--");
    lv_label_set_long_mode(scr_dashboard_label_logo_inft_11, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_logo_inft_11 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_logo_inft_11, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_logo_inft_11, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_logo_inft_11, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_detail_time_11
    scr_dashboard_label_detail_time_11 = lv_label_create(scr_dashboard_cont_detail_11);
    lv_obj_set_x(scr_dashboard_label_detail_time_11, 204);
    lv_obj_set_y(scr_dashboard_label_detail_time_11, 68);
    lv_obj_set_width(scr_dashboard_label_detail_time_11, 100);
    lv_obj_set_height(scr_dashboard_label_detail_time_11, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_time_11, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_time_11, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_time_11, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_time_11 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_time_11, lv_color_hex(0x718A92), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_time_11, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_time_11, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_mian_conf_11
    scr_dashboard_label_mian_conf_11 = lv_label_create(scr_dashboard_cont_detail_11);
    lv_obj_set_x(scr_dashboard_label_mian_conf_11, 204);
    lv_obj_set_y(scr_dashboard_label_mian_conf_11, 210);
    lv_obj_set_width(scr_dashboard_label_mian_conf_11, 100);
    lv_obj_set_height(scr_dashboard_label_mian_conf_11, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_mian_conf_11, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_mian_conf_11, "--");
    lv_label_set_long_mode(scr_dashboard_label_mian_conf_11, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_mian_conf_11 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_mian_conf_11, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_mian_conf_11, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_mian_conf_11, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_mian_inft_11
    scr_dashboard_label_mian_inft_11 = lv_label_create(scr_dashboard_cont_detail_11);
    lv_obj_set_x(scr_dashboard_label_mian_inft_11, 204);
    lv_obj_set_y(scr_dashboard_label_mian_inft_11, 308);
    lv_obj_set_width(scr_dashboard_label_mian_inft_11, 100);
    lv_obj_set_height(scr_dashboard_label_mian_inft_11, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_mian_inft_11, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_mian_inft_11, "--");
    lv_label_set_long_mode(scr_dashboard_label_mian_inft_11, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_mian_inft_11 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_mian_inft_11, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_mian_inft_11, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_mian_inft_11, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_detail_badge_11
    scr_dashboard_cont_detail_badge_11 = lv_obj_create(scr_dashboard_cont_detail_11);
    lv_obj_set_x(scr_dashboard_cont_detail_badge_11, 274);
    lv_obj_set_y(scr_dashboard_cont_detail_badge_11, 6);
    lv_obj_set_width(scr_dashboard_cont_detail_badge_11, 72);
    lv_obj_set_height(scr_dashboard_cont_detail_badge_11, 24);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_detail_badge_11, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_detail_badge_11, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_detail_badge_11 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_detail_badge_11, lv_color_hex(0x232C2C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_badge_11, 220, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_detail_badge_11, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_detail_badge_11, 66, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_badge_11, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_detail_badge_11, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_badge_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_badge_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_badge_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_badge_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_cont_detail_badge_11 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_cont_detail_badge_11, lv_color_hex(0x3A2A12), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_badge_11, 220, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_cont_detail_badge_11, lv_color_hex(0xE8B84A), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_cont_detail_badge_11, 120, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_badge_11, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_cont_detail_badge_11, 12, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_badge_11, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_badge_11, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_badge_11, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_badge_11, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    ui_flag_modify(scr_dashboard_cont_detail_badge_11, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_11, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_11, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_11, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_11, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_detail_badge_11
    scr_dashboard_label_detail_badge_11 = lv_label_create(scr_dashboard_cont_detail_badge_11);
    lv_obj_set_x(scr_dashboard_label_detail_badge_11, -14);
    lv_obj_set_y(scr_dashboard_label_detail_badge_11, 5);
    lv_obj_set_width(scr_dashboard_label_detail_badge_11, 100);
    lv_obj_set_height(scr_dashboard_label_detail_badge_11, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_badge_11, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_badge_11, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_badge_11, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_badge_11 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_badge_11, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_detail_badge_11, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_label_detail_badge_11 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_text_color(scr_dashboard_label_detail_badge_11, lv_color_hex(0xE8B84A), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(scr_dashboard_label_detail_badge_11, 255, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_badge_11, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_badge_11, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_badge_11, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_badge_11, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_CHECKED);
    // Create scr_dashboard_cont_detail_10
    scr_dashboard_cont_detail_10 = lv_obj_create(scr_dashboard_cont_event_details);
    lv_obj_set_x(scr_dashboard_cont_detail_10, 9);
    lv_obj_set_y(scr_dashboard_cont_detail_10, 7);
    lv_obj_set_width(scr_dashboard_cont_detail_10, 360);
    lv_obj_set_height(scr_dashboard_cont_detail_10, 370);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_detail_10, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_detail_10, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_detail_10 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_detail_10, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_detail_10, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_10, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_10, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_10, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_10, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_detail_result_10
    scr_dashboard_label_detail_result_10 = lv_label_create(scr_dashboard_cont_detail_10);
    lv_obj_set_x(scr_dashboard_label_detail_result_10, 204);
    lv_obj_set_y(scr_dashboard_label_detail_result_10, 114);
    lv_obj_set_width(scr_dashboard_label_detail_result_10, 100);
    lv_obj_set_height(scr_dashboard_label_detail_result_10, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_result_10, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_result_10, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_result_10, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_result_10 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_result_10, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_result_10, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_result_10, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_logo_conf_10
    scr_dashboard_label_logo_conf_10 = lv_label_create(scr_dashboard_cont_detail_10);
    lv_obj_set_x(scr_dashboard_label_logo_conf_10, 204);
    lv_obj_set_y(scr_dashboard_label_logo_conf_10, 164);
    lv_obj_set_width(scr_dashboard_label_logo_conf_10, 100);
    lv_obj_set_height(scr_dashboard_label_logo_conf_10, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_logo_conf_10, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_logo_conf_10, "--");
    lv_label_set_long_mode(scr_dashboard_label_logo_conf_10, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_logo_conf_10 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_logo_conf_10, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_logo_conf_10, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_logo_conf_10, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_logo_conf_10, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_logo_inft_10
    scr_dashboard_label_logo_inft_10 = lv_label_create(scr_dashboard_cont_detail_10);
    lv_obj_set_x(scr_dashboard_label_logo_inft_10, 204);
    lv_obj_set_y(scr_dashboard_label_logo_inft_10, 261);
    lv_obj_set_width(scr_dashboard_label_logo_inft_10, 100);
    lv_obj_set_height(scr_dashboard_label_logo_inft_10, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_logo_inft_10, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_logo_inft_10, "--");
    lv_label_set_long_mode(scr_dashboard_label_logo_inft_10, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_logo_inft_10 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_logo_inft_10, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_logo_inft_10, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_logo_inft_10, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_detail_time_10
    scr_dashboard_label_detail_time_10 = lv_label_create(scr_dashboard_cont_detail_10);
    lv_obj_set_x(scr_dashboard_label_detail_time_10, 204);
    lv_obj_set_y(scr_dashboard_label_detail_time_10, 68);
    lv_obj_set_width(scr_dashboard_label_detail_time_10, 100);
    lv_obj_set_height(scr_dashboard_label_detail_time_10, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_time_10, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_time_10, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_time_10, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_time_10 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_time_10, lv_color_hex(0x718A92), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_time_10, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_time_10, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_mian_conf_10
    scr_dashboard_label_mian_conf_10 = lv_label_create(scr_dashboard_cont_detail_10);
    lv_obj_set_x(scr_dashboard_label_mian_conf_10, 204);
    lv_obj_set_y(scr_dashboard_label_mian_conf_10, 210);
    lv_obj_set_width(scr_dashboard_label_mian_conf_10, 100);
    lv_obj_set_height(scr_dashboard_label_mian_conf_10, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_mian_conf_10, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_mian_conf_10, "--");
    lv_label_set_long_mode(scr_dashboard_label_mian_conf_10, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_mian_conf_10 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_mian_conf_10, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_mian_conf_10, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_mian_conf_10, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_mian_inft_10
    scr_dashboard_label_mian_inft_10 = lv_label_create(scr_dashboard_cont_detail_10);
    lv_obj_set_x(scr_dashboard_label_mian_inft_10, 204);
    lv_obj_set_y(scr_dashboard_label_mian_inft_10, 308);
    lv_obj_set_width(scr_dashboard_label_mian_inft_10, 100);
    lv_obj_set_height(scr_dashboard_label_mian_inft_10, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_mian_inft_10, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_mian_inft_10, "--");
    lv_label_set_long_mode(scr_dashboard_label_mian_inft_10, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_mian_inft_10 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_mian_inft_10, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_mian_inft_10, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_mian_inft_10, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_detail_badge_10
    scr_dashboard_cont_detail_badge_10 = lv_obj_create(scr_dashboard_cont_detail_10);
    lv_obj_set_x(scr_dashboard_cont_detail_badge_10, 274);
    lv_obj_set_y(scr_dashboard_cont_detail_badge_10, 6);
    lv_obj_set_width(scr_dashboard_cont_detail_badge_10, 72);
    lv_obj_set_height(scr_dashboard_cont_detail_badge_10, 24);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_detail_badge_10, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_detail_badge_10, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_detail_badge_10 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_detail_badge_10, lv_color_hex(0x232C2C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_badge_10, 220, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_detail_badge_10, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_detail_badge_10, 66, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_badge_10, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_detail_badge_10, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_badge_10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_badge_10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_badge_10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_badge_10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_cont_detail_badge_10 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_cont_detail_badge_10, lv_color_hex(0x3A2A12), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_badge_10, 220, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_cont_detail_badge_10, lv_color_hex(0xE8B84A), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_cont_detail_badge_10, 120, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_badge_10, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_cont_detail_badge_10, 12, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_badge_10, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_badge_10, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_badge_10, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_badge_10, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    ui_flag_modify(scr_dashboard_cont_detail_badge_10, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_10, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_10, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_10, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_10, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_detail_badge_10
    scr_dashboard_label_detail_badge_10 = lv_label_create(scr_dashboard_cont_detail_badge_10);
    lv_obj_set_x(scr_dashboard_label_detail_badge_10, -14);
    lv_obj_set_y(scr_dashboard_label_detail_badge_10, 5);
    lv_obj_set_width(scr_dashboard_label_detail_badge_10, 100);
    lv_obj_set_height(scr_dashboard_label_detail_badge_10, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_badge_10, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_badge_10, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_badge_10, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_badge_10 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_badge_10, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_detail_badge_10, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_label_detail_badge_10 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_text_color(scr_dashboard_label_detail_badge_10, lv_color_hex(0xE8B84A), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(scr_dashboard_label_detail_badge_10, 255, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_badge_10, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_badge_10, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_badge_10, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_badge_10, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_CHECKED);
    // Create scr_dashboard_cont_detail_09
    scr_dashboard_cont_detail_09 = lv_obj_create(scr_dashboard_cont_event_details);
    lv_obj_set_x(scr_dashboard_cont_detail_09, 9);
    lv_obj_set_y(scr_dashboard_cont_detail_09, 7);
    lv_obj_set_width(scr_dashboard_cont_detail_09, 360);
    lv_obj_set_height(scr_dashboard_cont_detail_09, 370);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_detail_09, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_detail_09, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_detail_09 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_09, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_09, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_09, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_09, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_09, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_09, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_detail_09, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_detail_09, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_09, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_09, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_09, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_09, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_detail_result_09
    scr_dashboard_label_detail_result_09 = lv_label_create(scr_dashboard_cont_detail_09);
    lv_obj_set_x(scr_dashboard_label_detail_result_09, 204);
    lv_obj_set_y(scr_dashboard_label_detail_result_09, 114);
    lv_obj_set_width(scr_dashboard_label_detail_result_09, 100);
    lv_obj_set_height(scr_dashboard_label_detail_result_09, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_result_09, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_result_09, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_result_09, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_result_09 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_result_09, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_result_09, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_result_09, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_logo_conf_09
    scr_dashboard_label_logo_conf_09 = lv_label_create(scr_dashboard_cont_detail_09);
    lv_obj_set_x(scr_dashboard_label_logo_conf_09, 204);
    lv_obj_set_y(scr_dashboard_label_logo_conf_09, 164);
    lv_obj_set_width(scr_dashboard_label_logo_conf_09, 100);
    lv_obj_set_height(scr_dashboard_label_logo_conf_09, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_logo_conf_09, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_logo_conf_09, "--");
    lv_label_set_long_mode(scr_dashboard_label_logo_conf_09, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_logo_conf_09 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_logo_conf_09, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_logo_conf_09, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_logo_conf_09, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_logo_conf_09, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_logo_inft_09
    scr_dashboard_label_logo_inft_09 = lv_label_create(scr_dashboard_cont_detail_09);
    lv_obj_set_x(scr_dashboard_label_logo_inft_09, 204);
    lv_obj_set_y(scr_dashboard_label_logo_inft_09, 261);
    lv_obj_set_width(scr_dashboard_label_logo_inft_09, 100);
    lv_obj_set_height(scr_dashboard_label_logo_inft_09, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_logo_inft_09, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_logo_inft_09, "--");
    lv_label_set_long_mode(scr_dashboard_label_logo_inft_09, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_logo_inft_09 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_logo_inft_09, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_logo_inft_09, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_logo_inft_09, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_detail_time_09
    scr_dashboard_label_detail_time_09 = lv_label_create(scr_dashboard_cont_detail_09);
    lv_obj_set_x(scr_dashboard_label_detail_time_09, 204);
    lv_obj_set_y(scr_dashboard_label_detail_time_09, 68);
    lv_obj_set_width(scr_dashboard_label_detail_time_09, 100);
    lv_obj_set_height(scr_dashboard_label_detail_time_09, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_time_09, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_time_09, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_time_09, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_time_09 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_time_09, lv_color_hex(0x718A92), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_time_09, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_time_09, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_mian_conf_09
    scr_dashboard_label_mian_conf_09 = lv_label_create(scr_dashboard_cont_detail_09);
    lv_obj_set_x(scr_dashboard_label_mian_conf_09, 204);
    lv_obj_set_y(scr_dashboard_label_mian_conf_09, 210);
    lv_obj_set_width(scr_dashboard_label_mian_conf_09, 100);
    lv_obj_set_height(scr_dashboard_label_mian_conf_09, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_mian_conf_09, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_mian_conf_09, "--");
    lv_label_set_long_mode(scr_dashboard_label_mian_conf_09, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_mian_conf_09 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_mian_conf_09, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_mian_conf_09, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_mian_conf_09, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_mian_inft_09
    scr_dashboard_label_mian_inft_09 = lv_label_create(scr_dashboard_cont_detail_09);
    lv_obj_set_x(scr_dashboard_label_mian_inft_09, 204);
    lv_obj_set_y(scr_dashboard_label_mian_inft_09, 308);
    lv_obj_set_width(scr_dashboard_label_mian_inft_09, 100);
    lv_obj_set_height(scr_dashboard_label_mian_inft_09, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_mian_inft_09, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_mian_inft_09, "--");
    lv_label_set_long_mode(scr_dashboard_label_mian_inft_09, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_mian_inft_09 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_mian_inft_09, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_mian_inft_09, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_mian_inft_09, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_detail_badge_09
    scr_dashboard_cont_detail_badge_09 = lv_obj_create(scr_dashboard_cont_detail_09);
    lv_obj_set_x(scr_dashboard_cont_detail_badge_09, 274);
    lv_obj_set_y(scr_dashboard_cont_detail_badge_09, 6);
    lv_obj_set_width(scr_dashboard_cont_detail_badge_09, 72);
    lv_obj_set_height(scr_dashboard_cont_detail_badge_09, 24);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_detail_badge_09, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_detail_badge_09, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_detail_badge_09 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_detail_badge_09, lv_color_hex(0x232C2C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_badge_09, 220, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_detail_badge_09, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_detail_badge_09, 66, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_badge_09, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_detail_badge_09, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_badge_09, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_badge_09, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_badge_09, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_badge_09, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_cont_detail_badge_09 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_cont_detail_badge_09, lv_color_hex(0x3A2A12), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_badge_09, 220, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_cont_detail_badge_09, lv_color_hex(0xE8B84A), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_cont_detail_badge_09, 120, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_badge_09, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_cont_detail_badge_09, 12, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_badge_09, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_badge_09, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_badge_09, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_badge_09, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    ui_flag_modify(scr_dashboard_cont_detail_badge_09, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_09, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_09, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_09, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_09, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_detail_badge_09
    scr_dashboard_label_detail_badge_09 = lv_label_create(scr_dashboard_cont_detail_badge_09);
    lv_obj_set_x(scr_dashboard_label_detail_badge_09, -14);
    lv_obj_set_y(scr_dashboard_label_detail_badge_09, 5);
    lv_obj_set_width(scr_dashboard_label_detail_badge_09, 100);
    lv_obj_set_height(scr_dashboard_label_detail_badge_09, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_badge_09, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_badge_09, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_badge_09, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_badge_09 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_badge_09, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_detail_badge_09, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_label_detail_badge_09 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_text_color(scr_dashboard_label_detail_badge_09, lv_color_hex(0xE8B84A), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(scr_dashboard_label_detail_badge_09, 255, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_badge_09, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_badge_09, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_badge_09, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_badge_09, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_CHECKED);
    // Create scr_dashboard_cont_detail_08
    scr_dashboard_cont_detail_08 = lv_obj_create(scr_dashboard_cont_event_details);
    lv_obj_set_x(scr_dashboard_cont_detail_08, 9);
    lv_obj_set_y(scr_dashboard_cont_detail_08, 7);
    lv_obj_set_width(scr_dashboard_cont_detail_08, 360);
    lv_obj_set_height(scr_dashboard_cont_detail_08, 370);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_detail_08, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_detail_08, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_detail_08 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_08, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_08, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_08, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_08, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_08, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_08, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_detail_08, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_detail_08, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_08, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_08, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_08, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_08, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_detail_result_08
    scr_dashboard_label_detail_result_08 = lv_label_create(scr_dashboard_cont_detail_08);
    lv_obj_set_x(scr_dashboard_label_detail_result_08, 204);
    lv_obj_set_y(scr_dashboard_label_detail_result_08, 114);
    lv_obj_set_width(scr_dashboard_label_detail_result_08, 100);
    lv_obj_set_height(scr_dashboard_label_detail_result_08, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_result_08, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_result_08, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_result_08, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_result_08 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_result_08, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_result_08, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_result_08, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_logo_conf_08
    scr_dashboard_label_logo_conf_08 = lv_label_create(scr_dashboard_cont_detail_08);
    lv_obj_set_x(scr_dashboard_label_logo_conf_08, 204);
    lv_obj_set_y(scr_dashboard_label_logo_conf_08, 164);
    lv_obj_set_width(scr_dashboard_label_logo_conf_08, 100);
    lv_obj_set_height(scr_dashboard_label_logo_conf_08, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_logo_conf_08, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_logo_conf_08, "--");
    lv_label_set_long_mode(scr_dashboard_label_logo_conf_08, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_logo_conf_08 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_logo_conf_08, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_logo_conf_08, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_logo_conf_08, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_logo_conf_08, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_logo_inft_08
    scr_dashboard_label_logo_inft_08 = lv_label_create(scr_dashboard_cont_detail_08);
    lv_obj_set_x(scr_dashboard_label_logo_inft_08, 204);
    lv_obj_set_y(scr_dashboard_label_logo_inft_08, 261);
    lv_obj_set_width(scr_dashboard_label_logo_inft_08, 100);
    lv_obj_set_height(scr_dashboard_label_logo_inft_08, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_logo_inft_08, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_logo_inft_08, "--");
    lv_label_set_long_mode(scr_dashboard_label_logo_inft_08, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_logo_inft_08 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_logo_inft_08, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_logo_inft_08, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_logo_inft_08, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_detail_time_08
    scr_dashboard_label_detail_time_08 = lv_label_create(scr_dashboard_cont_detail_08);
    lv_obj_set_x(scr_dashboard_label_detail_time_08, 204);
    lv_obj_set_y(scr_dashboard_label_detail_time_08, 68);
    lv_obj_set_width(scr_dashboard_label_detail_time_08, 100);
    lv_obj_set_height(scr_dashboard_label_detail_time_08, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_time_08, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_time_08, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_time_08, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_time_08 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_time_08, lv_color_hex(0x718A92), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_time_08, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_time_08, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_mian_conf_08
    scr_dashboard_label_mian_conf_08 = lv_label_create(scr_dashboard_cont_detail_08);
    lv_obj_set_x(scr_dashboard_label_mian_conf_08, 204);
    lv_obj_set_y(scr_dashboard_label_mian_conf_08, 210);
    lv_obj_set_width(scr_dashboard_label_mian_conf_08, 100);
    lv_obj_set_height(scr_dashboard_label_mian_conf_08, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_mian_conf_08, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_mian_conf_08, "--");
    lv_label_set_long_mode(scr_dashboard_label_mian_conf_08, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_mian_conf_08 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_mian_conf_08, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_mian_conf_08, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_mian_conf_08, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_mian_inft_08
    scr_dashboard_label_mian_inft_08 = lv_label_create(scr_dashboard_cont_detail_08);
    lv_obj_set_x(scr_dashboard_label_mian_inft_08, 204);
    lv_obj_set_y(scr_dashboard_label_mian_inft_08, 308);
    lv_obj_set_width(scr_dashboard_label_mian_inft_08, 100);
    lv_obj_set_height(scr_dashboard_label_mian_inft_08, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_mian_inft_08, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_mian_inft_08, "--");
    lv_label_set_long_mode(scr_dashboard_label_mian_inft_08, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_mian_inft_08 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_mian_inft_08, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_mian_inft_08, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_mian_inft_08, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_detail_badge_08
    scr_dashboard_cont_detail_badge_08 = lv_obj_create(scr_dashboard_cont_detail_08);
    lv_obj_set_x(scr_dashboard_cont_detail_badge_08, 274);
    lv_obj_set_y(scr_dashboard_cont_detail_badge_08, 6);
    lv_obj_set_width(scr_dashboard_cont_detail_badge_08, 72);
    lv_obj_set_height(scr_dashboard_cont_detail_badge_08, 24);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_detail_badge_08, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_detail_badge_08, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_detail_badge_08 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_detail_badge_08, lv_color_hex(0x232C2C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_badge_08, 220, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_detail_badge_08, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_detail_badge_08, 66, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_badge_08, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_detail_badge_08, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_badge_08, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_badge_08, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_badge_08, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_badge_08, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_cont_detail_badge_08 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_cont_detail_badge_08, lv_color_hex(0x3A2A12), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_badge_08, 220, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_cont_detail_badge_08, lv_color_hex(0xE8B84A), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_cont_detail_badge_08, 120, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_badge_08, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_cont_detail_badge_08, 12, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_badge_08, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_badge_08, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_badge_08, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_badge_08, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    ui_flag_modify(scr_dashboard_cont_detail_badge_08, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_08, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_08, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_08, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_08, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_detail_badge_08
    scr_dashboard_label_detail_badge_08 = lv_label_create(scr_dashboard_cont_detail_badge_08);
    lv_obj_set_x(scr_dashboard_label_detail_badge_08, -14);
    lv_obj_set_y(scr_dashboard_label_detail_badge_08, 5);
    lv_obj_set_width(scr_dashboard_label_detail_badge_08, 100);
    lv_obj_set_height(scr_dashboard_label_detail_badge_08, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_badge_08, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_badge_08, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_badge_08, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_badge_08 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_badge_08, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_detail_badge_08, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_label_detail_badge_08 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_text_color(scr_dashboard_label_detail_badge_08, lv_color_hex(0xE8B84A), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(scr_dashboard_label_detail_badge_08, 255, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_badge_08, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_badge_08, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_badge_08, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_badge_08, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_CHECKED);
    // Create scr_dashboard_cont_detail_07
    scr_dashboard_cont_detail_07 = lv_obj_create(scr_dashboard_cont_event_details);
    lv_obj_set_x(scr_dashboard_cont_detail_07, 9);
    lv_obj_set_y(scr_dashboard_cont_detail_07, 7);
    lv_obj_set_width(scr_dashboard_cont_detail_07, 360);
    lv_obj_set_height(scr_dashboard_cont_detail_07, 370);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_detail_07, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_detail_07, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_detail_07 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_07, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_07, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_07, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_07, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_07, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_07, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_detail_07, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_detail_07, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_07, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_07, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_07, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_07, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_detail_result_07
    scr_dashboard_label_detail_result_07 = lv_label_create(scr_dashboard_cont_detail_07);
    lv_obj_set_x(scr_dashboard_label_detail_result_07, 204);
    lv_obj_set_y(scr_dashboard_label_detail_result_07, 114);
    lv_obj_set_width(scr_dashboard_label_detail_result_07, 100);
    lv_obj_set_height(scr_dashboard_label_detail_result_07, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_result_07, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_result_07, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_result_07, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_result_07 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_result_07, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_result_07, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_result_07, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_logo_conf_07
    scr_dashboard_label_logo_conf_07 = lv_label_create(scr_dashboard_cont_detail_07);
    lv_obj_set_x(scr_dashboard_label_logo_conf_07, 204);
    lv_obj_set_y(scr_dashboard_label_logo_conf_07, 164);
    lv_obj_set_width(scr_dashboard_label_logo_conf_07, 100);
    lv_obj_set_height(scr_dashboard_label_logo_conf_07, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_logo_conf_07, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_logo_conf_07, "--");
    lv_label_set_long_mode(scr_dashboard_label_logo_conf_07, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_logo_conf_07 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_logo_conf_07, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_logo_conf_07, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_logo_conf_07, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_logo_conf_07, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_logo_inft_07
    scr_dashboard_label_logo_inft_07 = lv_label_create(scr_dashboard_cont_detail_07);
    lv_obj_set_x(scr_dashboard_label_logo_inft_07, 204);
    lv_obj_set_y(scr_dashboard_label_logo_inft_07, 261);
    lv_obj_set_width(scr_dashboard_label_logo_inft_07, 100);
    lv_obj_set_height(scr_dashboard_label_logo_inft_07, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_logo_inft_07, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_logo_inft_07, "--");
    lv_label_set_long_mode(scr_dashboard_label_logo_inft_07, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_logo_inft_07 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_logo_inft_07, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_logo_inft_07, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_logo_inft_07, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_detail_time_07
    scr_dashboard_label_detail_time_07 = lv_label_create(scr_dashboard_cont_detail_07);
    lv_obj_set_x(scr_dashboard_label_detail_time_07, 204);
    lv_obj_set_y(scr_dashboard_label_detail_time_07, 68);
    lv_obj_set_width(scr_dashboard_label_detail_time_07, 100);
    lv_obj_set_height(scr_dashboard_label_detail_time_07, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_time_07, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_time_07, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_time_07, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_time_07 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_time_07, lv_color_hex(0x718A92), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_time_07, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_time_07, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_mian_conf_07
    scr_dashboard_label_mian_conf_07 = lv_label_create(scr_dashboard_cont_detail_07);
    lv_obj_set_x(scr_dashboard_label_mian_conf_07, 204);
    lv_obj_set_y(scr_dashboard_label_mian_conf_07, 210);
    lv_obj_set_width(scr_dashboard_label_mian_conf_07, 100);
    lv_obj_set_height(scr_dashboard_label_mian_conf_07, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_mian_conf_07, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_mian_conf_07, "--");
    lv_label_set_long_mode(scr_dashboard_label_mian_conf_07, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_mian_conf_07 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_mian_conf_07, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_mian_conf_07, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_mian_conf_07, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_mian_inft_07
    scr_dashboard_label_mian_inft_07 = lv_label_create(scr_dashboard_cont_detail_07);
    lv_obj_set_x(scr_dashboard_label_mian_inft_07, 204);
    lv_obj_set_y(scr_dashboard_label_mian_inft_07, 308);
    lv_obj_set_width(scr_dashboard_label_mian_inft_07, 100);
    lv_obj_set_height(scr_dashboard_label_mian_inft_07, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_mian_inft_07, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_mian_inft_07, "--");
    lv_label_set_long_mode(scr_dashboard_label_mian_inft_07, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_mian_inft_07 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_mian_inft_07, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_mian_inft_07, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_mian_inft_07, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_detail_badge_07
    scr_dashboard_cont_detail_badge_07 = lv_obj_create(scr_dashboard_cont_detail_07);
    lv_obj_set_x(scr_dashboard_cont_detail_badge_07, 274);
    lv_obj_set_y(scr_dashboard_cont_detail_badge_07, 6);
    lv_obj_set_width(scr_dashboard_cont_detail_badge_07, 72);
    lv_obj_set_height(scr_dashboard_cont_detail_badge_07, 24);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_detail_badge_07, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_detail_badge_07, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_detail_badge_07 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_detail_badge_07, lv_color_hex(0x232C2C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_badge_07, 220, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_detail_badge_07, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_detail_badge_07, 66, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_badge_07, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_detail_badge_07, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_badge_07, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_badge_07, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_badge_07, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_badge_07, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_cont_detail_badge_07 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_cont_detail_badge_07, lv_color_hex(0x3A2A12), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_badge_07, 220, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_cont_detail_badge_07, lv_color_hex(0xE8B84A), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_cont_detail_badge_07, 120, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_badge_07, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_cont_detail_badge_07, 12, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_badge_07, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_badge_07, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_badge_07, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_badge_07, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    ui_flag_modify(scr_dashboard_cont_detail_badge_07, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_07, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_07, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_07, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_07, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_detail_badge_07
    scr_dashboard_label_detail_badge_07 = lv_label_create(scr_dashboard_cont_detail_badge_07);
    lv_obj_set_x(scr_dashboard_label_detail_badge_07, -14);
    lv_obj_set_y(scr_dashboard_label_detail_badge_07, 5);
    lv_obj_set_width(scr_dashboard_label_detail_badge_07, 100);
    lv_obj_set_height(scr_dashboard_label_detail_badge_07, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_badge_07, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_badge_07, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_badge_07, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_badge_07 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_badge_07, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_detail_badge_07, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_label_detail_badge_07 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_text_color(scr_dashboard_label_detail_badge_07, lv_color_hex(0xE8B84A), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(scr_dashboard_label_detail_badge_07, 255, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_badge_07, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_badge_07, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_badge_07, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_badge_07, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_CHECKED);
    // Create scr_dashboard_cont_detail_06
    scr_dashboard_cont_detail_06 = lv_obj_create(scr_dashboard_cont_event_details);
    lv_obj_set_x(scr_dashboard_cont_detail_06, 9);
    lv_obj_set_y(scr_dashboard_cont_detail_06, 7);
    lv_obj_set_width(scr_dashboard_cont_detail_06, 360);
    lv_obj_set_height(scr_dashboard_cont_detail_06, 370);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_detail_06, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_detail_06, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_detail_06 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_06, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_06, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_06, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_06, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_06, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_06, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_detail_06, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_detail_06, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_06, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_06, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_06, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_06, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_detail_result_06
    scr_dashboard_label_detail_result_06 = lv_label_create(scr_dashboard_cont_detail_06);
    lv_obj_set_x(scr_dashboard_label_detail_result_06, 204);
    lv_obj_set_y(scr_dashboard_label_detail_result_06, 114);
    lv_obj_set_width(scr_dashboard_label_detail_result_06, 100);
    lv_obj_set_height(scr_dashboard_label_detail_result_06, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_result_06, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_result_06, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_result_06, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_result_06 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_result_06, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_result_06, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_result_06, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_logo_conf_06
    scr_dashboard_label_logo_conf_06 = lv_label_create(scr_dashboard_cont_detail_06);
    lv_obj_set_x(scr_dashboard_label_logo_conf_06, 204);
    lv_obj_set_y(scr_dashboard_label_logo_conf_06, 164);
    lv_obj_set_width(scr_dashboard_label_logo_conf_06, 100);
    lv_obj_set_height(scr_dashboard_label_logo_conf_06, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_logo_conf_06, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_logo_conf_06, "--");
    lv_label_set_long_mode(scr_dashboard_label_logo_conf_06, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_logo_conf_06 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_logo_conf_06, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_logo_conf_06, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_logo_conf_06, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_logo_conf_06, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_logo_inft_06
    scr_dashboard_label_logo_inft_06 = lv_label_create(scr_dashboard_cont_detail_06);
    lv_obj_set_x(scr_dashboard_label_logo_inft_06, 204);
    lv_obj_set_y(scr_dashboard_label_logo_inft_06, 261);
    lv_obj_set_width(scr_dashboard_label_logo_inft_06, 100);
    lv_obj_set_height(scr_dashboard_label_logo_inft_06, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_logo_inft_06, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_logo_inft_06, "--");
    lv_label_set_long_mode(scr_dashboard_label_logo_inft_06, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_logo_inft_06 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_logo_inft_06, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_logo_inft_06, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_logo_inft_06, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_detail_time_06
    scr_dashboard_label_detail_time_06 = lv_label_create(scr_dashboard_cont_detail_06);
    lv_obj_set_x(scr_dashboard_label_detail_time_06, 204);
    lv_obj_set_y(scr_dashboard_label_detail_time_06, 68);
    lv_obj_set_width(scr_dashboard_label_detail_time_06, 100);
    lv_obj_set_height(scr_dashboard_label_detail_time_06, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_time_06, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_time_06, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_time_06, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_time_06 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_time_06, lv_color_hex(0x718A92), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_time_06, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_time_06, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_mian_conf_06
    scr_dashboard_label_mian_conf_06 = lv_label_create(scr_dashboard_cont_detail_06);
    lv_obj_set_x(scr_dashboard_label_mian_conf_06, 204);
    lv_obj_set_y(scr_dashboard_label_mian_conf_06, 210);
    lv_obj_set_width(scr_dashboard_label_mian_conf_06, 100);
    lv_obj_set_height(scr_dashboard_label_mian_conf_06, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_mian_conf_06, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_mian_conf_06, "--");
    lv_label_set_long_mode(scr_dashboard_label_mian_conf_06, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_mian_conf_06 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_mian_conf_06, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_mian_conf_06, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_mian_conf_06, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_mian_inft_06
    scr_dashboard_label_mian_inft_06 = lv_label_create(scr_dashboard_cont_detail_06);
    lv_obj_set_x(scr_dashboard_label_mian_inft_06, 204);
    lv_obj_set_y(scr_dashboard_label_mian_inft_06, 308);
    lv_obj_set_width(scr_dashboard_label_mian_inft_06, 100);
    lv_obj_set_height(scr_dashboard_label_mian_inft_06, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_mian_inft_06, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_mian_inft_06, "--");
    lv_label_set_long_mode(scr_dashboard_label_mian_inft_06, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_mian_inft_06 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_mian_inft_06, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_mian_inft_06, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_mian_inft_06, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_detail_badge_06
    scr_dashboard_cont_detail_badge_06 = lv_obj_create(scr_dashboard_cont_detail_06);
    lv_obj_set_x(scr_dashboard_cont_detail_badge_06, 274);
    lv_obj_set_y(scr_dashboard_cont_detail_badge_06, 6);
    lv_obj_set_width(scr_dashboard_cont_detail_badge_06, 72);
    lv_obj_set_height(scr_dashboard_cont_detail_badge_06, 24);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_detail_badge_06, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_detail_badge_06, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_detail_badge_06 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_detail_badge_06, lv_color_hex(0x232C2C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_badge_06, 220, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_detail_badge_06, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_detail_badge_06, 66, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_badge_06, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_detail_badge_06, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_badge_06, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_badge_06, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_badge_06, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_badge_06, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_cont_detail_badge_06 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_cont_detail_badge_06, lv_color_hex(0x3A2A12), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_badge_06, 220, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_cont_detail_badge_06, lv_color_hex(0xE8B84A), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_cont_detail_badge_06, 120, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_badge_06, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_cont_detail_badge_06, 12, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_badge_06, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_badge_06, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_badge_06, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_badge_06, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    ui_flag_modify(scr_dashboard_cont_detail_badge_06, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_06, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_06, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_06, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_06, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_detail_badge_06
    scr_dashboard_label_detail_badge_06 = lv_label_create(scr_dashboard_cont_detail_badge_06);
    lv_obj_set_x(scr_dashboard_label_detail_badge_06, -14);
    lv_obj_set_y(scr_dashboard_label_detail_badge_06, 5);
    lv_obj_set_width(scr_dashboard_label_detail_badge_06, 100);
    lv_obj_set_height(scr_dashboard_label_detail_badge_06, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_badge_06, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_badge_06, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_badge_06, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_badge_06 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_badge_06, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_detail_badge_06, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_label_detail_badge_06 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_text_color(scr_dashboard_label_detail_badge_06, lv_color_hex(0xE8B84A), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(scr_dashboard_label_detail_badge_06, 255, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_badge_06, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_badge_06, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_badge_06, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_badge_06, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_CHECKED);
    // Create scr_dashboard_cont_detail_05
    scr_dashboard_cont_detail_05 = lv_obj_create(scr_dashboard_cont_event_details);
    lv_obj_set_x(scr_dashboard_cont_detail_05, 9);
    lv_obj_set_y(scr_dashboard_cont_detail_05, 7);
    lv_obj_set_width(scr_dashboard_cont_detail_05, 360);
    lv_obj_set_height(scr_dashboard_cont_detail_05, 370);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_detail_05, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_detail_05, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_detail_05 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_05, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_05, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_05, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_05, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_05, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_05, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_detail_05, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_detail_05, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_05, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_05, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_05, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_05, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_detail_result_05
    scr_dashboard_label_detail_result_05 = lv_label_create(scr_dashboard_cont_detail_05);
    lv_obj_set_x(scr_dashboard_label_detail_result_05, 204);
    lv_obj_set_y(scr_dashboard_label_detail_result_05, 114);
    lv_obj_set_width(scr_dashboard_label_detail_result_05, 100);
    lv_obj_set_height(scr_dashboard_label_detail_result_05, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_result_05, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_result_05, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_result_05, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_result_05 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_result_05, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_result_05, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_result_05, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_logo_conf_05
    scr_dashboard_label_logo_conf_05 = lv_label_create(scr_dashboard_cont_detail_05);
    lv_obj_set_x(scr_dashboard_label_logo_conf_05, 204);
    lv_obj_set_y(scr_dashboard_label_logo_conf_05, 164);
    lv_obj_set_width(scr_dashboard_label_logo_conf_05, 100);
    lv_obj_set_height(scr_dashboard_label_logo_conf_05, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_logo_conf_05, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_logo_conf_05, "--");
    lv_label_set_long_mode(scr_dashboard_label_logo_conf_05, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_logo_conf_05 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_logo_conf_05, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_logo_conf_05, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_logo_conf_05, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_logo_conf_05, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_logo_inft_05
    scr_dashboard_label_logo_inft_05 = lv_label_create(scr_dashboard_cont_detail_05);
    lv_obj_set_x(scr_dashboard_label_logo_inft_05, 204);
    lv_obj_set_y(scr_dashboard_label_logo_inft_05, 261);
    lv_obj_set_width(scr_dashboard_label_logo_inft_05, 100);
    lv_obj_set_height(scr_dashboard_label_logo_inft_05, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_logo_inft_05, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_logo_inft_05, "--");
    lv_label_set_long_mode(scr_dashboard_label_logo_inft_05, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_logo_inft_05 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_logo_inft_05, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_logo_inft_05, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_logo_inft_05, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_detail_time_05
    scr_dashboard_label_detail_time_05 = lv_label_create(scr_dashboard_cont_detail_05);
    lv_obj_set_x(scr_dashboard_label_detail_time_05, 204);
    lv_obj_set_y(scr_dashboard_label_detail_time_05, 68);
    lv_obj_set_width(scr_dashboard_label_detail_time_05, 100);
    lv_obj_set_height(scr_dashboard_label_detail_time_05, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_time_05, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_time_05, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_time_05, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_time_05 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_time_05, lv_color_hex(0x718A92), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_time_05, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_time_05, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_mian_conf_05
    scr_dashboard_label_mian_conf_05 = lv_label_create(scr_dashboard_cont_detail_05);
    lv_obj_set_x(scr_dashboard_label_mian_conf_05, 204);
    lv_obj_set_y(scr_dashboard_label_mian_conf_05, 210);
    lv_obj_set_width(scr_dashboard_label_mian_conf_05, 100);
    lv_obj_set_height(scr_dashboard_label_mian_conf_05, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_mian_conf_05, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_mian_conf_05, "--");
    lv_label_set_long_mode(scr_dashboard_label_mian_conf_05, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_mian_conf_05 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_mian_conf_05, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_mian_conf_05, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_mian_conf_05, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_mian_inft_05
    scr_dashboard_label_mian_inft_05 = lv_label_create(scr_dashboard_cont_detail_05);
    lv_obj_set_x(scr_dashboard_label_mian_inft_05, 204);
    lv_obj_set_y(scr_dashboard_label_mian_inft_05, 308);
    lv_obj_set_width(scr_dashboard_label_mian_inft_05, 100);
    lv_obj_set_height(scr_dashboard_label_mian_inft_05, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_mian_inft_05, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_mian_inft_05, "--");
    lv_label_set_long_mode(scr_dashboard_label_mian_inft_05, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_mian_inft_05 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_mian_inft_05, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_mian_inft_05, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_mian_inft_05, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_detail_badge_05
    scr_dashboard_cont_detail_badge_05 = lv_obj_create(scr_dashboard_cont_detail_05);
    lv_obj_set_x(scr_dashboard_cont_detail_badge_05, 274);
    lv_obj_set_y(scr_dashboard_cont_detail_badge_05, 6);
    lv_obj_set_width(scr_dashboard_cont_detail_badge_05, 72);
    lv_obj_set_height(scr_dashboard_cont_detail_badge_05, 24);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_detail_badge_05, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_detail_badge_05, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_detail_badge_05 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_detail_badge_05, lv_color_hex(0x232C2C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_badge_05, 220, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_detail_badge_05, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_detail_badge_05, 66, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_badge_05, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_detail_badge_05, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_badge_05, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_badge_05, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_badge_05, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_badge_05, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_cont_detail_badge_05 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_cont_detail_badge_05, lv_color_hex(0x3A2A12), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_badge_05, 220, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_cont_detail_badge_05, lv_color_hex(0xE8B84A), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_cont_detail_badge_05, 120, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_badge_05, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_cont_detail_badge_05, 12, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_badge_05, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_badge_05, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_badge_05, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_badge_05, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    ui_flag_modify(scr_dashboard_cont_detail_badge_05, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_05, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_05, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_05, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_05, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_detail_badge_05
    scr_dashboard_label_detail_badge_05 = lv_label_create(scr_dashboard_cont_detail_badge_05);
    lv_obj_set_x(scr_dashboard_label_detail_badge_05, -14);
    lv_obj_set_y(scr_dashboard_label_detail_badge_05, 5);
    lv_obj_set_width(scr_dashboard_label_detail_badge_05, 100);
    lv_obj_set_height(scr_dashboard_label_detail_badge_05, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_badge_05, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_badge_05, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_badge_05, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_badge_05 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_badge_05, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_detail_badge_05, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_label_detail_badge_05 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_text_color(scr_dashboard_label_detail_badge_05, lv_color_hex(0xE8B84A), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(scr_dashboard_label_detail_badge_05, 255, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_badge_05, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_badge_05, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_badge_05, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_badge_05, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_CHECKED);
    // Create scr_dashboard_cont_detail_04
    scr_dashboard_cont_detail_04 = lv_obj_create(scr_dashboard_cont_event_details);
    lv_obj_set_x(scr_dashboard_cont_detail_04, 9);
    lv_obj_set_y(scr_dashboard_cont_detail_04, 7);
    lv_obj_set_width(scr_dashboard_cont_detail_04, 360);
    lv_obj_set_height(scr_dashboard_cont_detail_04, 370);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_detail_04, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_detail_04, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_detail_04 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_04, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_04, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_04, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_04, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_04, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_04, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_detail_04, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_detail_04, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_04, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_04, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_04, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_04, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_detail_result_04
    scr_dashboard_label_detail_result_04 = lv_label_create(scr_dashboard_cont_detail_04);
    lv_obj_set_x(scr_dashboard_label_detail_result_04, 204);
    lv_obj_set_y(scr_dashboard_label_detail_result_04, 114);
    lv_obj_set_width(scr_dashboard_label_detail_result_04, 100);
    lv_obj_set_height(scr_dashboard_label_detail_result_04, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_result_04, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_result_04, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_result_04, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_result_04 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_result_04, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_result_04, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_result_04, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_logo_conf_04
    scr_dashboard_label_logo_conf_04 = lv_label_create(scr_dashboard_cont_detail_04);
    lv_obj_set_x(scr_dashboard_label_logo_conf_04, 204);
    lv_obj_set_y(scr_dashboard_label_logo_conf_04, 164);
    lv_obj_set_width(scr_dashboard_label_logo_conf_04, 100);
    lv_obj_set_height(scr_dashboard_label_logo_conf_04, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_logo_conf_04, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_logo_conf_04, "--");
    lv_label_set_long_mode(scr_dashboard_label_logo_conf_04, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_logo_conf_04 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_logo_conf_04, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_logo_conf_04, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_logo_conf_04, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_logo_conf_04, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_logo_inft_04
    scr_dashboard_label_logo_inft_04 = lv_label_create(scr_dashboard_cont_detail_04);
    lv_obj_set_x(scr_dashboard_label_logo_inft_04, 204);
    lv_obj_set_y(scr_dashboard_label_logo_inft_04, 261);
    lv_obj_set_width(scr_dashboard_label_logo_inft_04, 100);
    lv_obj_set_height(scr_dashboard_label_logo_inft_04, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_logo_inft_04, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_logo_inft_04, "--");
    lv_label_set_long_mode(scr_dashboard_label_logo_inft_04, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_logo_inft_04 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_logo_inft_04, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_logo_inft_04, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_logo_inft_04, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_detail_time_04
    scr_dashboard_label_detail_time_04 = lv_label_create(scr_dashboard_cont_detail_04);
    lv_obj_set_x(scr_dashboard_label_detail_time_04, 204);
    lv_obj_set_y(scr_dashboard_label_detail_time_04, 68);
    lv_obj_set_width(scr_dashboard_label_detail_time_04, 100);
    lv_obj_set_height(scr_dashboard_label_detail_time_04, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_time_04, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_time_04, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_time_04, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_time_04 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_time_04, lv_color_hex(0x718A92), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_time_04, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_time_04, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_mian_conf_04
    scr_dashboard_label_mian_conf_04 = lv_label_create(scr_dashboard_cont_detail_04);
    lv_obj_set_x(scr_dashboard_label_mian_conf_04, 204);
    lv_obj_set_y(scr_dashboard_label_mian_conf_04, 210);
    lv_obj_set_width(scr_dashboard_label_mian_conf_04, 100);
    lv_obj_set_height(scr_dashboard_label_mian_conf_04, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_mian_conf_04, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_mian_conf_04, "--");
    lv_label_set_long_mode(scr_dashboard_label_mian_conf_04, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_mian_conf_04 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_mian_conf_04, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_mian_conf_04, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_mian_conf_04, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_mian_inft_04
    scr_dashboard_label_mian_inft_04 = lv_label_create(scr_dashboard_cont_detail_04);
    lv_obj_set_x(scr_dashboard_label_mian_inft_04, 204);
    lv_obj_set_y(scr_dashboard_label_mian_inft_04, 308);
    lv_obj_set_width(scr_dashboard_label_mian_inft_04, 100);
    lv_obj_set_height(scr_dashboard_label_mian_inft_04, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_mian_inft_04, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_mian_inft_04, "--");
    lv_label_set_long_mode(scr_dashboard_label_mian_inft_04, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_mian_inft_04 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_mian_inft_04, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_mian_inft_04, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_mian_inft_04, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_detail_badge_04
    scr_dashboard_cont_detail_badge_04 = lv_obj_create(scr_dashboard_cont_detail_04);
    lv_obj_set_x(scr_dashboard_cont_detail_badge_04, 274);
    lv_obj_set_y(scr_dashboard_cont_detail_badge_04, 6);
    lv_obj_set_width(scr_dashboard_cont_detail_badge_04, 72);
    lv_obj_set_height(scr_dashboard_cont_detail_badge_04, 24);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_detail_badge_04, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_detail_badge_04, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_detail_badge_04 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_detail_badge_04, lv_color_hex(0x232C2C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_badge_04, 220, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_detail_badge_04, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_detail_badge_04, 66, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_badge_04, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_detail_badge_04, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_badge_04, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_badge_04, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_badge_04, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_badge_04, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_cont_detail_badge_04 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_cont_detail_badge_04, lv_color_hex(0x3A2A12), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_badge_04, 220, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_cont_detail_badge_04, lv_color_hex(0xE8B84A), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_cont_detail_badge_04, 120, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_badge_04, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_cont_detail_badge_04, 12, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_badge_04, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_badge_04, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_badge_04, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_badge_04, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    ui_flag_modify(scr_dashboard_cont_detail_badge_04, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_04, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_04, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_04, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_04, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_detail_badge_04
    scr_dashboard_label_detail_badge_04 = lv_label_create(scr_dashboard_cont_detail_badge_04);
    lv_obj_set_x(scr_dashboard_label_detail_badge_04, -14);
    lv_obj_set_y(scr_dashboard_label_detail_badge_04, 5);
    lv_obj_set_width(scr_dashboard_label_detail_badge_04, 100);
    lv_obj_set_height(scr_dashboard_label_detail_badge_04, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_badge_04, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_badge_04, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_badge_04, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_badge_04 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_badge_04, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_detail_badge_04, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_label_detail_badge_04 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_text_color(scr_dashboard_label_detail_badge_04, lv_color_hex(0xE8B84A), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(scr_dashboard_label_detail_badge_04, 255, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_badge_04, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_badge_04, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_badge_04, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_badge_04, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_CHECKED);
    // Create scr_dashboard_cont_detail_03
    scr_dashboard_cont_detail_03 = lv_obj_create(scr_dashboard_cont_event_details);
    lv_obj_set_x(scr_dashboard_cont_detail_03, 9);
    lv_obj_set_y(scr_dashboard_cont_detail_03, 7);
    lv_obj_set_width(scr_dashboard_cont_detail_03, 360);
    lv_obj_set_height(scr_dashboard_cont_detail_03, 370);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_detail_03, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_detail_03, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_detail_03 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_03, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_03, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_03, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_03, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_03, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_03, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_detail_03, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_detail_03, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_03, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_03, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_03, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_03, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_detail_result_03
    scr_dashboard_label_detail_result_03 = lv_label_create(scr_dashboard_cont_detail_03);
    lv_obj_set_x(scr_dashboard_label_detail_result_03, 204);
    lv_obj_set_y(scr_dashboard_label_detail_result_03, 114);
    lv_obj_set_width(scr_dashboard_label_detail_result_03, 100);
    lv_obj_set_height(scr_dashboard_label_detail_result_03, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_result_03, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_result_03, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_result_03, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_result_03 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_result_03, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_result_03, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_result_03, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_logo_conf_03
    scr_dashboard_label_logo_conf_03 = lv_label_create(scr_dashboard_cont_detail_03);
    lv_obj_set_x(scr_dashboard_label_logo_conf_03, 204);
    lv_obj_set_y(scr_dashboard_label_logo_conf_03, 164);
    lv_obj_set_width(scr_dashboard_label_logo_conf_03, 100);
    lv_obj_set_height(scr_dashboard_label_logo_conf_03, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_logo_conf_03, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_logo_conf_03, "--");
    lv_label_set_long_mode(scr_dashboard_label_logo_conf_03, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_logo_conf_03 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_logo_conf_03, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_logo_conf_03, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_logo_conf_03, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_logo_conf_03, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_logo_inft_03
    scr_dashboard_label_logo_inft_03 = lv_label_create(scr_dashboard_cont_detail_03);
    lv_obj_set_x(scr_dashboard_label_logo_inft_03, 204);
    lv_obj_set_y(scr_dashboard_label_logo_inft_03, 261);
    lv_obj_set_width(scr_dashboard_label_logo_inft_03, 100);
    lv_obj_set_height(scr_dashboard_label_logo_inft_03, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_logo_inft_03, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_logo_inft_03, "--");
    lv_label_set_long_mode(scr_dashboard_label_logo_inft_03, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_logo_inft_03 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_logo_inft_03, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_logo_inft_03, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_logo_inft_03, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_detail_time_03
    scr_dashboard_label_detail_time_03 = lv_label_create(scr_dashboard_cont_detail_03);
    lv_obj_set_x(scr_dashboard_label_detail_time_03, 204);
    lv_obj_set_y(scr_dashboard_label_detail_time_03, 68);
    lv_obj_set_width(scr_dashboard_label_detail_time_03, 100);
    lv_obj_set_height(scr_dashboard_label_detail_time_03, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_time_03, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_time_03, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_time_03, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_time_03 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_time_03, lv_color_hex(0x718A92), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_time_03, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_time_03, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_mian_conf_03
    scr_dashboard_label_mian_conf_03 = lv_label_create(scr_dashboard_cont_detail_03);
    lv_obj_set_x(scr_dashboard_label_mian_conf_03, 204);
    lv_obj_set_y(scr_dashboard_label_mian_conf_03, 210);
    lv_obj_set_width(scr_dashboard_label_mian_conf_03, 100);
    lv_obj_set_height(scr_dashboard_label_mian_conf_03, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_mian_conf_03, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_mian_conf_03, "--");
    lv_label_set_long_mode(scr_dashboard_label_mian_conf_03, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_mian_conf_03 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_mian_conf_03, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_mian_conf_03, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_mian_conf_03, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_mian_inft_03
    scr_dashboard_label_mian_inft_03 = lv_label_create(scr_dashboard_cont_detail_03);
    lv_obj_set_x(scr_dashboard_label_mian_inft_03, 204);
    lv_obj_set_y(scr_dashboard_label_mian_inft_03, 308);
    lv_obj_set_width(scr_dashboard_label_mian_inft_03, 100);
    lv_obj_set_height(scr_dashboard_label_mian_inft_03, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_mian_inft_03, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_mian_inft_03, "--");
    lv_label_set_long_mode(scr_dashboard_label_mian_inft_03, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_mian_inft_03 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_mian_inft_03, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_mian_inft_03, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_mian_inft_03, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_detail_badge_03
    scr_dashboard_cont_detail_badge_03 = lv_obj_create(scr_dashboard_cont_detail_03);
    lv_obj_set_x(scr_dashboard_cont_detail_badge_03, 274);
    lv_obj_set_y(scr_dashboard_cont_detail_badge_03, 6);
    lv_obj_set_width(scr_dashboard_cont_detail_badge_03, 72);
    lv_obj_set_height(scr_dashboard_cont_detail_badge_03, 24);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_detail_badge_03, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_detail_badge_03, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_detail_badge_03 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_detail_badge_03, lv_color_hex(0x232C2C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_badge_03, 220, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_detail_badge_03, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_detail_badge_03, 66, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_badge_03, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_detail_badge_03, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_badge_03, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_badge_03, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_badge_03, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_badge_03, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_cont_detail_badge_03 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_cont_detail_badge_03, lv_color_hex(0x3A2A12), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_badge_03, 220, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_cont_detail_badge_03, lv_color_hex(0xE8B84A), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_cont_detail_badge_03, 120, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_badge_03, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_cont_detail_badge_03, 12, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_badge_03, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_badge_03, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_badge_03, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_badge_03, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    ui_flag_modify(scr_dashboard_cont_detail_badge_03, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_03, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_03, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_03, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_03, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_detail_badge_03
    scr_dashboard_label_detail_badge_03 = lv_label_create(scr_dashboard_cont_detail_badge_03);
    lv_obj_set_x(scr_dashboard_label_detail_badge_03, -14);
    lv_obj_set_y(scr_dashboard_label_detail_badge_03, 5);
    lv_obj_set_width(scr_dashboard_label_detail_badge_03, 100);
    lv_obj_set_height(scr_dashboard_label_detail_badge_03, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_badge_03, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_badge_03, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_badge_03, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_badge_03 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_badge_03, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_detail_badge_03, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_label_detail_badge_03 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_text_color(scr_dashboard_label_detail_badge_03, lv_color_hex(0xE8B84A), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(scr_dashboard_label_detail_badge_03, 255, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_badge_03, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_badge_03, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_badge_03, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_badge_03, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_CHECKED);
    // Create scr_dashboard_cont_detail_02
    scr_dashboard_cont_detail_02 = lv_obj_create(scr_dashboard_cont_event_details);
    lv_obj_set_x(scr_dashboard_cont_detail_02, 9);
    lv_obj_set_y(scr_dashboard_cont_detail_02, 7);
    lv_obj_set_width(scr_dashboard_cont_detail_02, 360);
    lv_obj_set_height(scr_dashboard_cont_detail_02, 370);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_detail_02, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_detail_02, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_detail_02 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_02, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_02, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_02, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_02, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_02, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_02, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_detail_02, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_detail_02, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_02, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_02, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_02, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_02, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_detail_result_02
    scr_dashboard_label_detail_result_02 = lv_label_create(scr_dashboard_cont_detail_02);
    lv_obj_set_x(scr_dashboard_label_detail_result_02, 204);
    lv_obj_set_y(scr_dashboard_label_detail_result_02, 114);
    lv_obj_set_width(scr_dashboard_label_detail_result_02, 100);
    lv_obj_set_height(scr_dashboard_label_detail_result_02, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_result_02, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_result_02, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_result_02, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_result_02 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_result_02, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_result_02, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_result_02, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_logo_conf_02
    scr_dashboard_label_logo_conf_02 = lv_label_create(scr_dashboard_cont_detail_02);
    lv_obj_set_x(scr_dashboard_label_logo_conf_02, 204);
    lv_obj_set_y(scr_dashboard_label_logo_conf_02, 164);
    lv_obj_set_width(scr_dashboard_label_logo_conf_02, 100);
    lv_obj_set_height(scr_dashboard_label_logo_conf_02, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_logo_conf_02, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_logo_conf_02, "--");
    lv_label_set_long_mode(scr_dashboard_label_logo_conf_02, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_logo_conf_02 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_logo_conf_02, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_logo_conf_02, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_logo_conf_02, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_logo_conf_02, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_mian_conf_02
    scr_dashboard_label_mian_conf_02 = lv_label_create(scr_dashboard_cont_detail_02);
    lv_obj_set_x(scr_dashboard_label_mian_conf_02, 204);
    lv_obj_set_y(scr_dashboard_label_mian_conf_02, 210);
    lv_obj_set_width(scr_dashboard_label_mian_conf_02, 100);
    lv_obj_set_height(scr_dashboard_label_mian_conf_02, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_mian_conf_02, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_mian_conf_02, "--");
    lv_label_set_long_mode(scr_dashboard_label_mian_conf_02, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_mian_conf_02 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_mian_conf_02, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_mian_conf_02, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_mian_conf_02, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_detail_time_02
    scr_dashboard_label_detail_time_02 = lv_label_create(scr_dashboard_cont_detail_02);
    lv_obj_set_x(scr_dashboard_label_detail_time_02, 204);
    lv_obj_set_y(scr_dashboard_label_detail_time_02, 68);
    lv_obj_set_width(scr_dashboard_label_detail_time_02, 100);
    lv_obj_set_height(scr_dashboard_label_detail_time_02, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_time_02, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_time_02, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_time_02, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_time_02 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_time_02, lv_color_hex(0x718A92), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_time_02, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_time_02, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_logo_inft_02
    scr_dashboard_label_logo_inft_02 = lv_label_create(scr_dashboard_cont_detail_02);
    lv_obj_set_x(scr_dashboard_label_logo_inft_02, 204);
    lv_obj_set_y(scr_dashboard_label_logo_inft_02, 261);
    lv_obj_set_width(scr_dashboard_label_logo_inft_02, 100);
    lv_obj_set_height(scr_dashboard_label_logo_inft_02, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_logo_inft_02, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_logo_inft_02, "--");
    lv_label_set_long_mode(scr_dashboard_label_logo_inft_02, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_logo_inft_02 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_logo_inft_02, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_logo_inft_02, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_logo_inft_02, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_mian_inft_02
    scr_dashboard_label_mian_inft_02 = lv_label_create(scr_dashboard_cont_detail_02);
    lv_obj_set_x(scr_dashboard_label_mian_inft_02, 204);
    lv_obj_set_y(scr_dashboard_label_mian_inft_02, 308);
    lv_obj_set_width(scr_dashboard_label_mian_inft_02, 100);
    lv_obj_set_height(scr_dashboard_label_mian_inft_02, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_mian_inft_02, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_mian_inft_02, "--");
    lv_label_set_long_mode(scr_dashboard_label_mian_inft_02, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_mian_inft_02 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_mian_inft_02, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_mian_inft_02, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_mian_inft_02, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_detail_badge_02
    scr_dashboard_cont_detail_badge_02 = lv_obj_create(scr_dashboard_cont_detail_02);
    lv_obj_set_x(scr_dashboard_cont_detail_badge_02, 274);
    lv_obj_set_y(scr_dashboard_cont_detail_badge_02, 6);
    lv_obj_set_width(scr_dashboard_cont_detail_badge_02, 72);
    lv_obj_set_height(scr_dashboard_cont_detail_badge_02, 24);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_detail_badge_02, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_detail_badge_02, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_detail_badge_02 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_detail_badge_02, lv_color_hex(0x232C2C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_badge_02, 220, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_detail_badge_02, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_detail_badge_02, 66, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_badge_02, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_detail_badge_02, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_badge_02, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_badge_02, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_badge_02, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_badge_02, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_cont_detail_badge_02 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_cont_detail_badge_02, lv_color_hex(0x3A2A12), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_badge_02, 220, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_cont_detail_badge_02, lv_color_hex(0xE8B84A), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_cont_detail_badge_02, 120, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_badge_02, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_cont_detail_badge_02, 12, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_badge_02, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_badge_02, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_badge_02, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_badge_02, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    ui_flag_modify(scr_dashboard_cont_detail_badge_02, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_02, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_02, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_02, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_02, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_detail_badge_02
    scr_dashboard_label_detail_badge_02 = lv_label_create(scr_dashboard_cont_detail_badge_02);
    lv_obj_set_x(scr_dashboard_label_detail_badge_02, -14);
    lv_obj_set_y(scr_dashboard_label_detail_badge_02, 5);
    lv_obj_set_width(scr_dashboard_label_detail_badge_02, 100);
    lv_obj_set_height(scr_dashboard_label_detail_badge_02, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_badge_02, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_badge_02, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_badge_02, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_badge_02 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_badge_02, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_detail_badge_02, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_label_detail_badge_02 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_text_color(scr_dashboard_label_detail_badge_02, lv_color_hex(0xE8B84A), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(scr_dashboard_label_detail_badge_02, 255, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_badge_02, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_badge_02, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_badge_02, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_badge_02, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_CHECKED);
    // Create scr_dashboard_cont_detail_01
    scr_dashboard_cont_detail_01 = lv_obj_create(scr_dashboard_cont_event_details);
    lv_obj_set_x(scr_dashboard_cont_detail_01, 9);
    lv_obj_set_y(scr_dashboard_cont_detail_01, 7);
    lv_obj_set_width(scr_dashboard_cont_detail_01, 360);
    lv_obj_set_height(scr_dashboard_cont_detail_01, 370);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_detail_01, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_detail_01, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_detail_01 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_01, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_01, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_01, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_01, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_01, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_01, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_detail_01, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_01, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_01, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_01, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_01, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_detail_result_01
    scr_dashboard_label_detail_result_01 = lv_label_create(scr_dashboard_cont_detail_01);
    lv_obj_set_x(scr_dashboard_label_detail_result_01, 204);
    lv_obj_set_y(scr_dashboard_label_detail_result_01, 114);
    lv_obj_set_width(scr_dashboard_label_detail_result_01, 100);
    lv_obj_set_height(scr_dashboard_label_detail_result_01, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_result_01, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_result_01, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_result_01, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_result_01 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_result_01, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_result_01, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_result_01, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_logo_conf_01
    scr_dashboard_label_logo_conf_01 = lv_label_create(scr_dashboard_cont_detail_01);
    lv_obj_set_x(scr_dashboard_label_logo_conf_01, 204);
    lv_obj_set_y(scr_dashboard_label_logo_conf_01, 164);
    lv_obj_set_width(scr_dashboard_label_logo_conf_01, 100);
    lv_obj_set_height(scr_dashboard_label_logo_conf_01, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_logo_conf_01, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_logo_conf_01, "--");
    lv_label_set_long_mode(scr_dashboard_label_logo_conf_01, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_logo_conf_01 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_logo_conf_01, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_logo_conf_01, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_logo_conf_01, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_logo_conf_01, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_mian_conf_01
    scr_dashboard_label_mian_conf_01 = lv_label_create(scr_dashboard_cont_detail_01);
    lv_obj_set_x(scr_dashboard_label_mian_conf_01, 204);
    lv_obj_set_y(scr_dashboard_label_mian_conf_01, 210);
    lv_obj_set_width(scr_dashboard_label_mian_conf_01, 100);
    lv_obj_set_height(scr_dashboard_label_mian_conf_01, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_mian_conf_01, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_mian_conf_01, "--");
    lv_label_set_long_mode(scr_dashboard_label_mian_conf_01, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_mian_conf_01 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_mian_conf_01, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_mian_conf_01, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_mian_conf_01, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_detail_time_01
    scr_dashboard_label_detail_time_01 = lv_label_create(scr_dashboard_cont_detail_01);
    lv_obj_set_x(scr_dashboard_label_detail_time_01, 204);
    lv_obj_set_y(scr_dashboard_label_detail_time_01, 68);
    lv_obj_set_width(scr_dashboard_label_detail_time_01, 100);
    lv_obj_set_height(scr_dashboard_label_detail_time_01, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_time_01, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_time_01, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_time_01, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_time_01 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_time_01, lv_color_hex(0x718A92), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_time_01, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_time_01, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_logo_inft_01
    scr_dashboard_label_logo_inft_01 = lv_label_create(scr_dashboard_cont_detail_01);
    lv_obj_set_x(scr_dashboard_label_logo_inft_01, 204);
    lv_obj_set_y(scr_dashboard_label_logo_inft_01, 261);
    lv_obj_set_width(scr_dashboard_label_logo_inft_01, 100);
    lv_obj_set_height(scr_dashboard_label_logo_inft_01, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_logo_inft_01, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_logo_inft_01, "--");
    lv_label_set_long_mode(scr_dashboard_label_logo_inft_01, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_logo_inft_01 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_logo_inft_01, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_logo_inft_01, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_logo_inft_01, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_mian_inft_01
    scr_dashboard_label_mian_inft_01 = lv_label_create(scr_dashboard_cont_detail_01);
    lv_obj_set_x(scr_dashboard_label_mian_inft_01, 204);
    lv_obj_set_y(scr_dashboard_label_mian_inft_01, 308);
    lv_obj_set_width(scr_dashboard_label_mian_inft_01, 100);
    lv_obj_set_height(scr_dashboard_label_mian_inft_01, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_mian_inft_01, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_mian_inft_01, "--");
    lv_label_set_long_mode(scr_dashboard_label_mian_inft_01, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_mian_inft_01 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_mian_inft_01, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_mian_inft_01, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_mian_inft_01, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_detail_badge_01
    scr_dashboard_cont_detail_badge_01 = lv_obj_create(scr_dashboard_cont_detail_01);
    lv_obj_set_x(scr_dashboard_cont_detail_badge_01, 274);
    lv_obj_set_y(scr_dashboard_cont_detail_badge_01, 6);
    lv_obj_set_width(scr_dashboard_cont_detail_badge_01, 72);
    lv_obj_set_height(scr_dashboard_cont_detail_badge_01, 24);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_detail_badge_01, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_detail_badge_01, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_detail_badge_01 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_detail_badge_01, lv_color_hex(0x232C2C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_badge_01, 220, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_detail_badge_01, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_detail_badge_01, 66, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_badge_01, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_detail_badge_01, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_badge_01, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_badge_01, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_badge_01, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_badge_01, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_cont_detail_badge_01 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_cont_detail_badge_01, lv_color_hex(0x3A2A12), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detail_badge_01, 220, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_cont_detail_badge_01, lv_color_hex(0xE8B84A), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_cont_detail_badge_01, 120, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_cont_detail_badge_01, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_cont_detail_badge_01, 12, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detail_badge_01, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detail_badge_01, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detail_badge_01, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detail_badge_01, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    ui_flag_modify(scr_dashboard_cont_detail_badge_01, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_01, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_01, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_01, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detail_badge_01, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_detail_badge_01
    scr_dashboard_label_detail_badge_01 = lv_label_create(scr_dashboard_cont_detail_badge_01);
    lv_obj_set_x(scr_dashboard_label_detail_badge_01, -14);
    lv_obj_set_y(scr_dashboard_label_detail_badge_01, 5);
    lv_obj_set_width(scr_dashboard_label_detail_badge_01, 100);
    lv_obj_set_height(scr_dashboard_label_detail_badge_01, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detail_badge_01, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detail_badge_01, "--");
    lv_label_set_long_mode(scr_dashboard_label_detail_badge_01, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detail_badge_01 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detail_badge_01, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_detail_badge_01, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_label_detail_badge_01 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_text_color(scr_dashboard_label_detail_badge_01, lv_color_hex(0xE8B84A), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(scr_dashboard_label_detail_badge_01, 255, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_badge_01, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_badge_01, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detail_badge_01, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_align(scr_dashboard_label_detail_badge_01, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_CHECKED);
    // Create scr_dashboard_label_title_event
    scr_dashboard_label_title_event = lv_label_create(scr_dashboard_cont_event_details);
    lv_obj_set_x(scr_dashboard_label_title_event, 26);
    lv_obj_set_y(scr_dashboard_label_title_event, 9);
    lv_obj_set_width(scr_dashboard_label_title_event, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_title_event, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_title_event, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_title_event, "事件详情");
    lv_label_set_long_mode(scr_dashboard_label_title_event, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_title_event - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_title_event, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_title_event, &lv_font_Misans_Heavy_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_title_event, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_title_conf_mian
    scr_dashboard_label_title_conf_mian = lv_label_create(scr_dashboard_cont_event_details);
    lv_obj_set_x(scr_dashboard_label_title_conf_mian, 72);
    lv_obj_set_y(scr_dashboard_label_title_conf_mian, 219);
    lv_obj_set_width(scr_dashboard_label_title_conf_mian, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_title_conf_mian, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_title_conf_mian, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_title_conf_mian, "面单置信度");
    lv_label_set_long_mode(scr_dashboard_label_title_conf_mian, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_title_conf_mian - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_title_conf_mian, lv_color_hex(0x7F98A0), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_title_conf_mian, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_title_conf_mian, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_title_result
    scr_dashboard_label_title_result = lv_label_create(scr_dashboard_cont_event_details);
    lv_obj_set_x(scr_dashboard_label_title_result, 72);
    lv_obj_set_y(scr_dashboard_label_title_result, 122);
    lv_obj_set_width(scr_dashboard_label_title_result, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_title_result, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_title_result, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_title_result, "结果");
    lv_label_set_long_mode(scr_dashboard_label_title_result, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_title_result - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_title_result, lv_color_hex(0x7F98A0), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_title_result, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_title_result, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_title_conf_logo
    scr_dashboard_label_title_conf_logo = lv_label_create(scr_dashboard_cont_event_details);
    lv_obj_set_x(scr_dashboard_label_title_conf_logo, 72);
    lv_obj_set_y(scr_dashboard_label_title_conf_logo, 171);
    lv_obj_set_width(scr_dashboard_label_title_conf_logo, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_title_conf_logo, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_title_conf_logo, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_title_conf_logo, "Logo置信度");
    lv_label_set_long_mode(scr_dashboard_label_title_conf_logo, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_title_conf_logo - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_title_conf_logo, lv_color_hex(0x7F98A0), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_title_conf_logo, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_title_conf_logo, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_title_inft_logo
    scr_dashboard_label_title_inft_logo = lv_label_create(scr_dashboard_cont_event_details);
    lv_obj_set_x(scr_dashboard_label_title_inft_logo, 72);
    lv_obj_set_y(scr_dashboard_label_title_inft_logo, 267);
    lv_obj_set_width(scr_dashboard_label_title_inft_logo, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_title_inft_logo, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_title_inft_logo, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_title_inft_logo, "Logo推理耗时");
    lv_label_set_long_mode(scr_dashboard_label_title_inft_logo, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_title_inft_logo - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_title_inft_logo, lv_color_hex(0x7F98A0), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_title_inft_logo, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_title_inft_logo, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_title_time
    scr_dashboard_label_title_time = lv_label_create(scr_dashboard_cont_event_details);
    lv_obj_set_x(scr_dashboard_label_title_time, 72);
    lv_obj_set_y(scr_dashboard_label_title_time, 73);
    lv_obj_set_width(scr_dashboard_label_title_time, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_title_time, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_title_time, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_title_time, "时间");
    lv_label_set_long_mode(scr_dashboard_label_title_time, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_title_time - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_title_time, lv_color_hex(0x7F98A0), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_title_time, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_title_time, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_title_inft_mian
    scr_dashboard_label_title_inft_mian = lv_label_create(scr_dashboard_cont_event_details);
    lv_obj_set_x(scr_dashboard_label_title_inft_mian, 72);
    lv_obj_set_y(scr_dashboard_label_title_inft_mian, 315);
    lv_obj_set_width(scr_dashboard_label_title_inft_mian, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_title_inft_mian, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_title_inft_mian, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_title_inft_mian, "面单推理耗时");
    lv_label_set_long_mode(scr_dashboard_label_title_inft_mian, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_title_inft_mian - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_title_inft_mian, lv_color_hex(0x7F98A0), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_title_inft_mian, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_title_inft_mian, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_under2
    scr_dashboard_cont_under2 = lv_obj_create(scr_dashboard_cont_event_details);
    lv_obj_set_x(scr_dashboard_cont_under2, 36);
    lv_obj_set_y(scr_dashboard_cont_under2, 31);
    lv_obj_set_width(scr_dashboard_cont_under2, 36);
    lv_obj_set_height(scr_dashboard_cont_under2, 2);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_under2, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_under2, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_under2 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_under2, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_under2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_under2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_under2, 999, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_under2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_under2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_under2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_under2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_under2, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_under2, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_under2, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_under2, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_under2, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_cont_log_record
    scr_dashboard_cont_log_record = lv_obj_create(scr_dashboard_cont_log);
    lv_obj_set_x(scr_dashboard_cont_log_record, 12);
    lv_obj_set_y(scr_dashboard_cont_log_record, 22);
    lv_obj_set_width(scr_dashboard_cont_log_record, 572);
    lv_obj_set_height(scr_dashboard_cont_log_record, 384);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_log_record, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_log_record, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_log_record - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_record, lv_color_hex(0x232C2C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_log_record, lv_color_hex(0x070A0A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_record, 108, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_log_record, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_record, lv_color_hex(0x4C6770), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_record, 44, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_record, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_log_record, 16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_record, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_record, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_record, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_record, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_log_record, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_record, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_record, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_record, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_record, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_cont_log_page4
    scr_dashboard_cont_log_page4 = lv_obj_create(scr_dashboard_cont_log_record);
    lv_obj_set_x(scr_dashboard_cont_log_page4, -3);
    lv_obj_set_y(scr_dashboard_cont_log_page4, 45);
    lv_obj_set_width(scr_dashboard_cont_log_page4, 578);
    lv_obj_set_height(scr_dashboard_cont_log_page4, 300);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_log_page4, LV_SCROLLBAR_MODE_ACTIVE);
    lv_obj_set_layout(scr_dashboard_cont_log_page4, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_log_page4 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_page4, lv_color_hex(0x232C2C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_log_page4, lv_color_hex(0x1A2827), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_page4, 108, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_log_page4, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_page4, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_page4, 60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_page4, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_log_page4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_page4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_page4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_page4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_page4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_log_page4, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_log_page4, LV_OBJ_FLAG_SCROLL_CHAIN_VER, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_log_page4, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_page4, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_cont_log_20
    scr_dashboard_cont_log_20 = lv_obj_create(scr_dashboard_cont_log_page4);
    lv_obj_set_x(scr_dashboard_cont_log_20, 21);
    lv_obj_set_y(scr_dashboard_cont_log_20, 241);
    lv_obj_set_width(scr_dashboard_cont_log_20, 534);
    lv_obj_set_height(scr_dashboard_cont_log_20, 44);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_log_20, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_log_20, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_log_20 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_20, lv_color_hex(0x1E2426), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_20, 160, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_20, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_20, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_20, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_log_20, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_cont_log_20 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_20, lv_color_hex(0x3F5250), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_log_20, lv_color_hex(0x1A2827), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_20, 100, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_log_20, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_20, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_20, 80, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_20, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_cont_log_20, 4, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_20, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_20, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_20, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_20, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    ui_flag_modify(scr_dashboard_cont_log_20, LV_OBJ_FLAG_CHECKABLE, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_log_20, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_20, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_20, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_20, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_20, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_time_20
    scr_dashboard_label_time_20 = lv_label_create(scr_dashboard_cont_log_20);
    lv_obj_set_x(scr_dashboard_label_time_20, 0);
    lv_obj_set_y(scr_dashboard_label_time_20, 14);
    lv_obj_set_width(scr_dashboard_label_time_20, 100);
    lv_obj_set_height(scr_dashboard_label_time_20, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_time_20, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_time_20, "--");
    lv_label_set_long_mode(scr_dashboard_label_time_20, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_time_20 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_time_20, lv_color_hex(0x718A92), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_time_20, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_time_20, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_company_20
    scr_dashboard_label_company_20 = lv_label_create(scr_dashboard_cont_log_20);
    lv_obj_set_x(scr_dashboard_label_company_20, 111);
    lv_obj_set_y(scr_dashboard_label_company_20, 14);
    lv_obj_set_width(scr_dashboard_label_company_20, 100);
    lv_obj_set_height(scr_dashboard_label_company_20, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_company_20, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_company_20, "--");
    lv_label_set_long_mode(scr_dashboard_label_company_20, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_company_20 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_company_20, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_company_20, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_company_20, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_result_20
    scr_dashboard_label_result_20 = lv_label_create(scr_dashboard_cont_log_20);
    lv_obj_set_x(scr_dashboard_label_result_20, 282);
    lv_obj_set_y(scr_dashboard_label_result_20, 14);
    lv_obj_set_width(scr_dashboard_label_result_20, 100);
    lv_obj_set_height(scr_dashboard_label_result_20, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_result_20, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_result_20, "--");
    lv_label_set_long_mode(scr_dashboard_label_result_20, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_result_20 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_result_20, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_result_20, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_result_20, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_confidence_20
    scr_dashboard_label_confidence_20 = lv_label_create(scr_dashboard_cont_log_20);
    lv_obj_set_x(scr_dashboard_label_confidence_20, 417);
    lv_obj_set_y(scr_dashboard_label_confidence_20, 14);
    lv_obj_set_width(scr_dashboard_label_confidence_20, 100);
    lv_obj_set_height(scr_dashboard_label_confidence_20, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_confidence_20, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_confidence_20, "--");
    lv_label_set_long_mode(scr_dashboard_label_confidence_20, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_confidence_20 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_confidence_20, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_confidence_20, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_confidence_20, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_log_19
    scr_dashboard_cont_log_19 = lv_obj_create(scr_dashboard_cont_log_page4);
    lv_obj_set_x(scr_dashboard_cont_log_19, 21);
    lv_obj_set_y(scr_dashboard_cont_log_19, 185);
    lv_obj_set_width(scr_dashboard_cont_log_19, 534);
    lv_obj_set_height(scr_dashboard_cont_log_19, 44);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_log_19, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_log_19, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_log_19 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_19, lv_color_hex(0x1E2426), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_19, 160, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_19, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_19, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_19, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_log_19, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_cont_log_19 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_19, lv_color_hex(0x3F5250), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_log_19, lv_color_hex(0x1A2827), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_19, 100, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_log_19, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_19, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_19, 80, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_19, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_cont_log_19, 4, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_19, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_19, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_19, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_19, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    ui_flag_modify(scr_dashboard_cont_log_19, LV_OBJ_FLAG_CHECKABLE, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_log_19, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_19, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_19, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_19, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_19, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_time_19
    scr_dashboard_label_time_19 = lv_label_create(scr_dashboard_cont_log_19);
    lv_obj_set_x(scr_dashboard_label_time_19, 0);
    lv_obj_set_y(scr_dashboard_label_time_19, 14);
    lv_obj_set_width(scr_dashboard_label_time_19, 100);
    lv_obj_set_height(scr_dashboard_label_time_19, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_time_19, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_time_19, "--");
    lv_label_set_long_mode(scr_dashboard_label_time_19, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_time_19 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_time_19, lv_color_hex(0x718A92), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_time_19, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_time_19, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_company_19
    scr_dashboard_label_company_19 = lv_label_create(scr_dashboard_cont_log_19);
    lv_obj_set_x(scr_dashboard_label_company_19, 111);
    lv_obj_set_y(scr_dashboard_label_company_19, 14);
    lv_obj_set_width(scr_dashboard_label_company_19, 100);
    lv_obj_set_height(scr_dashboard_label_company_19, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_company_19, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_company_19, "--");
    lv_label_set_long_mode(scr_dashboard_label_company_19, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_company_19 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_company_19, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_company_19, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_company_19, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_result_19
    scr_dashboard_label_result_19 = lv_label_create(scr_dashboard_cont_log_19);
    lv_obj_set_x(scr_dashboard_label_result_19, 282);
    lv_obj_set_y(scr_dashboard_label_result_19, 14);
    lv_obj_set_width(scr_dashboard_label_result_19, 100);
    lv_obj_set_height(scr_dashboard_label_result_19, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_result_19, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_result_19, "--");
    lv_label_set_long_mode(scr_dashboard_label_result_19, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_result_19 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_result_19, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_result_19, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_result_19, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_confidence_19
    scr_dashboard_label_confidence_19 = lv_label_create(scr_dashboard_cont_log_19);
    lv_obj_set_x(scr_dashboard_label_confidence_19, 417);
    lv_obj_set_y(scr_dashboard_label_confidence_19, 14);
    lv_obj_set_width(scr_dashboard_label_confidence_19, 100);
    lv_obj_set_height(scr_dashboard_label_confidence_19, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_confidence_19, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_confidence_19, "--");
    lv_label_set_long_mode(scr_dashboard_label_confidence_19, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_confidence_19 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_confidence_19, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_confidence_19, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_confidence_19, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_log_18
    scr_dashboard_cont_log_18 = lv_obj_create(scr_dashboard_cont_log_page4);
    lv_obj_set_x(scr_dashboard_cont_log_18, 21);
    lv_obj_set_y(scr_dashboard_cont_log_18, 129);
    lv_obj_set_width(scr_dashboard_cont_log_18, 534);
    lv_obj_set_height(scr_dashboard_cont_log_18, 44);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_log_18, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_log_18, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_log_18 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_18, lv_color_hex(0x1E2426), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_18, 160, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_18, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_18, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_18, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_log_18, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_18, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_18, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_18, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_18, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_cont_log_18 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_18, lv_color_hex(0x3F5250), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_log_18, lv_color_hex(0x1A2827), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_18, 100, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_log_18, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_18, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_18, 80, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_18, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_cont_log_18, 4, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_18, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_18, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_18, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_18, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    ui_flag_modify(scr_dashboard_cont_log_18, LV_OBJ_FLAG_CHECKABLE, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_log_18, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_18, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_18, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_18, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_18, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_time_18
    scr_dashboard_label_time_18 = lv_label_create(scr_dashboard_cont_log_18);
    lv_obj_set_x(scr_dashboard_label_time_18, 0);
    lv_obj_set_y(scr_dashboard_label_time_18, 14);
    lv_obj_set_width(scr_dashboard_label_time_18, 100);
    lv_obj_set_height(scr_dashboard_label_time_18, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_time_18, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_time_18, "--");
    lv_label_set_long_mode(scr_dashboard_label_time_18, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_time_18 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_time_18, lv_color_hex(0x718A92), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_time_18, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_time_18, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_company_18
    scr_dashboard_label_company_18 = lv_label_create(scr_dashboard_cont_log_18);
    lv_obj_set_x(scr_dashboard_label_company_18, 111);
    lv_obj_set_y(scr_dashboard_label_company_18, 14);
    lv_obj_set_width(scr_dashboard_label_company_18, 100);
    lv_obj_set_height(scr_dashboard_label_company_18, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_company_18, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_company_18, "--");
    lv_label_set_long_mode(scr_dashboard_label_company_18, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_company_18 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_company_18, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_company_18, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_company_18, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_result_18
    scr_dashboard_label_result_18 = lv_label_create(scr_dashboard_cont_log_18);
    lv_obj_set_x(scr_dashboard_label_result_18, 282);
    lv_obj_set_y(scr_dashboard_label_result_18, 14);
    lv_obj_set_width(scr_dashboard_label_result_18, 100);
    lv_obj_set_height(scr_dashboard_label_result_18, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_result_18, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_result_18, "--");
    lv_label_set_long_mode(scr_dashboard_label_result_18, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_result_18 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_result_18, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_result_18, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_result_18, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_confidence_18
    scr_dashboard_label_confidence_18 = lv_label_create(scr_dashboard_cont_log_18);
    lv_obj_set_x(scr_dashboard_label_confidence_18, 417);
    lv_obj_set_y(scr_dashboard_label_confidence_18, 14);
    lv_obj_set_width(scr_dashboard_label_confidence_18, 100);
    lv_obj_set_height(scr_dashboard_label_confidence_18, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_confidence_18, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_confidence_18, "--");
    lv_label_set_long_mode(scr_dashboard_label_confidence_18, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_confidence_18 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_confidence_18, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_confidence_18, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_confidence_18, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_log_17
    scr_dashboard_cont_log_17 = lv_obj_create(scr_dashboard_cont_log_page4);
    lv_obj_set_x(scr_dashboard_cont_log_17, 21);
    lv_obj_set_y(scr_dashboard_cont_log_17, 72);
    lv_obj_set_width(scr_dashboard_cont_log_17, 534);
    lv_obj_set_height(scr_dashboard_cont_log_17, 44);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_log_17, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_log_17, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_log_17 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_17, lv_color_hex(0x1E2426), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_17, 160, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_17, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_17, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_17, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_log_17, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_cont_log_17 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_17, lv_color_hex(0x3F5250), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_log_17, lv_color_hex(0x1A2827), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_17, 100, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_log_17, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_17, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_17, 80, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_17, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_cont_log_17, 4, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_17, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_17, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_17, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_17, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    ui_flag_modify(scr_dashboard_cont_log_17, LV_OBJ_FLAG_CHECKABLE, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_log_17, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_17, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_17, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_17, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_17, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_time_17
    scr_dashboard_label_time_17 = lv_label_create(scr_dashboard_cont_log_17);
    lv_obj_set_x(scr_dashboard_label_time_17, 0);
    lv_obj_set_y(scr_dashboard_label_time_17, 14);
    lv_obj_set_width(scr_dashboard_label_time_17, 100);
    lv_obj_set_height(scr_dashboard_label_time_17, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_time_17, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_time_17, "--");
    lv_label_set_long_mode(scr_dashboard_label_time_17, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_time_17 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_time_17, lv_color_hex(0x718A92), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_time_17, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_time_17, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_company_17
    scr_dashboard_label_company_17 = lv_label_create(scr_dashboard_cont_log_17);
    lv_obj_set_x(scr_dashboard_label_company_17, 111);
    lv_obj_set_y(scr_dashboard_label_company_17, 14);
    lv_obj_set_width(scr_dashboard_label_company_17, 100);
    lv_obj_set_height(scr_dashboard_label_company_17, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_company_17, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_company_17, "--");
    lv_label_set_long_mode(scr_dashboard_label_company_17, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_company_17 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_company_17, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_company_17, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_company_17, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_result_17
    scr_dashboard_label_result_17 = lv_label_create(scr_dashboard_cont_log_17);
    lv_obj_set_x(scr_dashboard_label_result_17, 282);
    lv_obj_set_y(scr_dashboard_label_result_17, 14);
    lv_obj_set_width(scr_dashboard_label_result_17, 100);
    lv_obj_set_height(scr_dashboard_label_result_17, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_result_17, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_result_17, "--");
    lv_label_set_long_mode(scr_dashboard_label_result_17, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_result_17 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_result_17, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_result_17, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_result_17, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_confidence_17
    scr_dashboard_label_confidence_17 = lv_label_create(scr_dashboard_cont_log_17);
    lv_obj_set_x(scr_dashboard_label_confidence_17, 417);
    lv_obj_set_y(scr_dashboard_label_confidence_17, 14);
    lv_obj_set_width(scr_dashboard_label_confidence_17, 100);
    lv_obj_set_height(scr_dashboard_label_confidence_17, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_confidence_17, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_confidence_17, "--");
    lv_label_set_long_mode(scr_dashboard_label_confidence_17, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_confidence_17 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_confidence_17, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_confidence_17, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_confidence_17, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_log_16
    scr_dashboard_cont_log_16 = lv_obj_create(scr_dashboard_cont_log_page4);
    lv_obj_set_x(scr_dashboard_cont_log_16, 21);
    lv_obj_set_y(scr_dashboard_cont_log_16, 15);
    lv_obj_set_width(scr_dashboard_cont_log_16, 534);
    lv_obj_set_height(scr_dashboard_cont_log_16, 44);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_log_16, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_log_16, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_log_16 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_16, lv_color_hex(0x1E2426), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_16, 160, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_16, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_16, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_16, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_log_16, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_cont_log_16 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_16, lv_color_hex(0x3F5250), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_log_16, lv_color_hex(0x1A2827), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_16, 100, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_log_16, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_16, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_16, 80, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_16, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_cont_log_16, 4, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_16, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_16, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_16, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_16, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    ui_flag_modify(scr_dashboard_cont_log_16, LV_OBJ_FLAG_CHECKABLE, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_log_16, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_16, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_16, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_16, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_16, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_time_16
    scr_dashboard_label_time_16 = lv_label_create(scr_dashboard_cont_log_16);
    lv_obj_set_x(scr_dashboard_label_time_16, 0);
    lv_obj_set_y(scr_dashboard_label_time_16, 14);
    lv_obj_set_width(scr_dashboard_label_time_16, 100);
    lv_obj_set_height(scr_dashboard_label_time_16, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_time_16, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_time_16, "--");
    lv_label_set_long_mode(scr_dashboard_label_time_16, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_time_16 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_time_16, lv_color_hex(0x718A92), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_time_16, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_time_16, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_company_16
    scr_dashboard_label_company_16 = lv_label_create(scr_dashboard_cont_log_16);
    lv_obj_set_x(scr_dashboard_label_company_16, 111);
    lv_obj_set_y(scr_dashboard_label_company_16, 14);
    lv_obj_set_width(scr_dashboard_label_company_16, 100);
    lv_obj_set_height(scr_dashboard_label_company_16, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_company_16, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_company_16, "--");
    lv_label_set_long_mode(scr_dashboard_label_company_16, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_company_16 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_company_16, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_company_16, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_company_16, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_result_16
    scr_dashboard_label_result_16 = lv_label_create(scr_dashboard_cont_log_16);
    lv_obj_set_x(scr_dashboard_label_result_16, 282);
    lv_obj_set_y(scr_dashboard_label_result_16, 14);
    lv_obj_set_width(scr_dashboard_label_result_16, 100);
    lv_obj_set_height(scr_dashboard_label_result_16, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_result_16, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_result_16, "--");
    lv_label_set_long_mode(scr_dashboard_label_result_16, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_result_16 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_result_16, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_result_16, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_result_16, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_confidence_16
    scr_dashboard_label_confidence_16 = lv_label_create(scr_dashboard_cont_log_16);
    lv_obj_set_x(scr_dashboard_label_confidence_16, 417);
    lv_obj_set_y(scr_dashboard_label_confidence_16, 14);
    lv_obj_set_width(scr_dashboard_label_confidence_16, 100);
    lv_obj_set_height(scr_dashboard_label_confidence_16, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_confidence_16, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_confidence_16, "--");
    lv_label_set_long_mode(scr_dashboard_label_confidence_16, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_confidence_16 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_confidence_16, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_confidence_16, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_confidence_16, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_log_page3
    scr_dashboard_cont_log_page3 = lv_obj_create(scr_dashboard_cont_log_record);
    lv_obj_set_x(scr_dashboard_cont_log_page3, -3);
    lv_obj_set_y(scr_dashboard_cont_log_page3, 45);
    lv_obj_set_width(scr_dashboard_cont_log_page3, 578);
    lv_obj_set_height(scr_dashboard_cont_log_page3, 300);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_log_page3, LV_SCROLLBAR_MODE_ACTIVE);
    lv_obj_set_layout(scr_dashboard_cont_log_page3, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_log_page3 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_page3, lv_color_hex(0x232C2C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_log_page3, lv_color_hex(0x1A2827), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_page3, 108, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_log_page3, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_page3, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_page3, 60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_page3, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_log_page3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_page3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_page3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_page3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_page3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_log_page3, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_log_page3, LV_OBJ_FLAG_SCROLL_CHAIN_VER, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_log_page3, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_page3, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_cont_log_15
    scr_dashboard_cont_log_15 = lv_obj_create(scr_dashboard_cont_log_page3);
    lv_obj_set_x(scr_dashboard_cont_log_15, 21);
    lv_obj_set_y(scr_dashboard_cont_log_15, 241);
    lv_obj_set_width(scr_dashboard_cont_log_15, 534);
    lv_obj_set_height(scr_dashboard_cont_log_15, 44);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_log_15, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_log_15, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_log_15 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_15, lv_color_hex(0x1E2426), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_15, 160, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_15, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_15, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_15, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_log_15, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_cont_log_15 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_15, lv_color_hex(0x3F5250), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_log_15, lv_color_hex(0x1A2827), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_15, 100, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_log_15, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_15, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_15, 80, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_15, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_cont_log_15, 4, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_15, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_15, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_15, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_15, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    ui_flag_modify(scr_dashboard_cont_log_15, LV_OBJ_FLAG_CHECKABLE, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_log_15, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_15, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_15, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_15, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_15, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_time_15
    scr_dashboard_label_time_15 = lv_label_create(scr_dashboard_cont_log_15);
    lv_obj_set_x(scr_dashboard_label_time_15, 0);
    lv_obj_set_y(scr_dashboard_label_time_15, 14);
    lv_obj_set_width(scr_dashboard_label_time_15, 100);
    lv_obj_set_height(scr_dashboard_label_time_15, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_time_15, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_time_15, "--");
    lv_label_set_long_mode(scr_dashboard_label_time_15, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_time_15 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_time_15, lv_color_hex(0x718A92), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_time_15, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_time_15, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_company_15
    scr_dashboard_label_company_15 = lv_label_create(scr_dashboard_cont_log_15);
    lv_obj_set_x(scr_dashboard_label_company_15, 111);
    lv_obj_set_y(scr_dashboard_label_company_15, 14);
    lv_obj_set_width(scr_dashboard_label_company_15, 100);
    lv_obj_set_height(scr_dashboard_label_company_15, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_company_15, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_company_15, "--");
    lv_label_set_long_mode(scr_dashboard_label_company_15, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_company_15 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_company_15, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_company_15, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_company_15, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_result_15
    scr_dashboard_label_result_15 = lv_label_create(scr_dashboard_cont_log_15);
    lv_obj_set_x(scr_dashboard_label_result_15, 282);
    lv_obj_set_y(scr_dashboard_label_result_15, 14);
    lv_obj_set_width(scr_dashboard_label_result_15, 100);
    lv_obj_set_height(scr_dashboard_label_result_15, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_result_15, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_result_15, "--");
    lv_label_set_long_mode(scr_dashboard_label_result_15, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_result_15 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_result_15, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_result_15, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_result_15, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_confidence_15
    scr_dashboard_label_confidence_15 = lv_label_create(scr_dashboard_cont_log_15);
    lv_obj_set_x(scr_dashboard_label_confidence_15, 417);
    lv_obj_set_y(scr_dashboard_label_confidence_15, 14);
    lv_obj_set_width(scr_dashboard_label_confidence_15, 100);
    lv_obj_set_height(scr_dashboard_label_confidence_15, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_confidence_15, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_confidence_15, "--");
    lv_label_set_long_mode(scr_dashboard_label_confidence_15, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_confidence_15 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_confidence_15, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_confidence_15, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_confidence_15, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_log_14
    scr_dashboard_cont_log_14 = lv_obj_create(scr_dashboard_cont_log_page3);
    lv_obj_set_x(scr_dashboard_cont_log_14, 21);
    lv_obj_set_y(scr_dashboard_cont_log_14, 185);
    lv_obj_set_width(scr_dashboard_cont_log_14, 534);
    lv_obj_set_height(scr_dashboard_cont_log_14, 44);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_log_14, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_log_14, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_log_14 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_14, lv_color_hex(0x1E2426), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_14, 160, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_14, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_14, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_14, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_log_14, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_cont_log_14 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_14, lv_color_hex(0x3F5250), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_log_14, lv_color_hex(0x1A2827), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_14, 100, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_log_14, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_14, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_14, 80, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_14, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_cont_log_14, 4, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_14, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_14, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_14, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_14, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    ui_flag_modify(scr_dashboard_cont_log_14, LV_OBJ_FLAG_CHECKABLE, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_log_14, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_14, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_14, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_14, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_14, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_time_14
    scr_dashboard_label_time_14 = lv_label_create(scr_dashboard_cont_log_14);
    lv_obj_set_x(scr_dashboard_label_time_14, 0);
    lv_obj_set_y(scr_dashboard_label_time_14, 14);
    lv_obj_set_width(scr_dashboard_label_time_14, 100);
    lv_obj_set_height(scr_dashboard_label_time_14, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_time_14, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_time_14, "--");
    lv_label_set_long_mode(scr_dashboard_label_time_14, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_time_14 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_time_14, lv_color_hex(0x718A92), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_time_14, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_time_14, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_company_14
    scr_dashboard_label_company_14 = lv_label_create(scr_dashboard_cont_log_14);
    lv_obj_set_x(scr_dashboard_label_company_14, 111);
    lv_obj_set_y(scr_dashboard_label_company_14, 14);
    lv_obj_set_width(scr_dashboard_label_company_14, 100);
    lv_obj_set_height(scr_dashboard_label_company_14, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_company_14, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_company_14, "--");
    lv_label_set_long_mode(scr_dashboard_label_company_14, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_company_14 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_company_14, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_company_14, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_company_14, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_result_14
    scr_dashboard_label_result_14 = lv_label_create(scr_dashboard_cont_log_14);
    lv_obj_set_x(scr_dashboard_label_result_14, 282);
    lv_obj_set_y(scr_dashboard_label_result_14, 14);
    lv_obj_set_width(scr_dashboard_label_result_14, 100);
    lv_obj_set_height(scr_dashboard_label_result_14, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_result_14, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_result_14, "--");
    lv_label_set_long_mode(scr_dashboard_label_result_14, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_result_14 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_result_14, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_result_14, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_result_14, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_confidence_14
    scr_dashboard_label_confidence_14 = lv_label_create(scr_dashboard_cont_log_14);
    lv_obj_set_x(scr_dashboard_label_confidence_14, 417);
    lv_obj_set_y(scr_dashboard_label_confidence_14, 14);
    lv_obj_set_width(scr_dashboard_label_confidence_14, 100);
    lv_obj_set_height(scr_dashboard_label_confidence_14, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_confidence_14, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_confidence_14, "--");
    lv_label_set_long_mode(scr_dashboard_label_confidence_14, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_confidence_14 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_confidence_14, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_confidence_14, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_confidence_14, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_log_13
    scr_dashboard_cont_log_13 = lv_obj_create(scr_dashboard_cont_log_page3);
    lv_obj_set_x(scr_dashboard_cont_log_13, 21);
    lv_obj_set_y(scr_dashboard_cont_log_13, 129);
    lv_obj_set_width(scr_dashboard_cont_log_13, 534);
    lv_obj_set_height(scr_dashboard_cont_log_13, 44);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_log_13, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_log_13, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_log_13 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_13, lv_color_hex(0x1E2426), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_13, 160, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_13, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_13, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_13, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_log_13, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_cont_log_13 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_13, lv_color_hex(0x3F5250), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_log_13, lv_color_hex(0x1A2827), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_13, 100, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_log_13, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_13, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_13, 80, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_13, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_cont_log_13, 4, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_13, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_13, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_13, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_13, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    ui_flag_modify(scr_dashboard_cont_log_13, LV_OBJ_FLAG_CHECKABLE, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_log_13, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_13, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_13, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_13, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_13, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_time_13
    scr_dashboard_label_time_13 = lv_label_create(scr_dashboard_cont_log_13);
    lv_obj_set_x(scr_dashboard_label_time_13, 0);
    lv_obj_set_y(scr_dashboard_label_time_13, 14);
    lv_obj_set_width(scr_dashboard_label_time_13, 100);
    lv_obj_set_height(scr_dashboard_label_time_13, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_time_13, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_time_13, "--");
    lv_label_set_long_mode(scr_dashboard_label_time_13, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_time_13 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_time_13, lv_color_hex(0x718A92), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_time_13, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_time_13, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_company_13
    scr_dashboard_label_company_13 = lv_label_create(scr_dashboard_cont_log_13);
    lv_obj_set_x(scr_dashboard_label_company_13, 111);
    lv_obj_set_y(scr_dashboard_label_company_13, 14);
    lv_obj_set_width(scr_dashboard_label_company_13, 100);
    lv_obj_set_height(scr_dashboard_label_company_13, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_company_13, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_company_13, "--");
    lv_label_set_long_mode(scr_dashboard_label_company_13, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_company_13 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_company_13, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_company_13, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_company_13, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_result_13
    scr_dashboard_label_result_13 = lv_label_create(scr_dashboard_cont_log_13);
    lv_obj_set_x(scr_dashboard_label_result_13, 282);
    lv_obj_set_y(scr_dashboard_label_result_13, 14);
    lv_obj_set_width(scr_dashboard_label_result_13, 100);
    lv_obj_set_height(scr_dashboard_label_result_13, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_result_13, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_result_13, "--");
    lv_label_set_long_mode(scr_dashboard_label_result_13, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_result_13 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_result_13, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_result_13, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_result_13, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_confidence_13
    scr_dashboard_label_confidence_13 = lv_label_create(scr_dashboard_cont_log_13);
    lv_obj_set_x(scr_dashboard_label_confidence_13, 417);
    lv_obj_set_y(scr_dashboard_label_confidence_13, 14);
    lv_obj_set_width(scr_dashboard_label_confidence_13, 100);
    lv_obj_set_height(scr_dashboard_label_confidence_13, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_confidence_13, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_confidence_13, "--");
    lv_label_set_long_mode(scr_dashboard_label_confidence_13, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_confidence_13 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_confidence_13, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_confidence_13, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_confidence_13, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_log_12
    scr_dashboard_cont_log_12 = lv_obj_create(scr_dashboard_cont_log_page3);
    lv_obj_set_x(scr_dashboard_cont_log_12, 21);
    lv_obj_set_y(scr_dashboard_cont_log_12, 72);
    lv_obj_set_width(scr_dashboard_cont_log_12, 534);
    lv_obj_set_height(scr_dashboard_cont_log_12, 44);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_log_12, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_log_12, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_log_12 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_12, lv_color_hex(0x1E2426), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_12, 160, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_12, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_12, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_12, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_log_12, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_cont_log_12 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_12, lv_color_hex(0x3F5250), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_log_12, lv_color_hex(0x1A2827), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_12, 100, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_log_12, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_12, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_12, 80, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_12, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_cont_log_12, 4, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_12, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_12, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_12, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_12, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    ui_flag_modify(scr_dashboard_cont_log_12, LV_OBJ_FLAG_CHECKABLE, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_log_12, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_12, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_12, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_12, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_12, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_time_12
    scr_dashboard_label_time_12 = lv_label_create(scr_dashboard_cont_log_12);
    lv_obj_set_x(scr_dashboard_label_time_12, 0);
    lv_obj_set_y(scr_dashboard_label_time_12, 14);
    lv_obj_set_width(scr_dashboard_label_time_12, 100);
    lv_obj_set_height(scr_dashboard_label_time_12, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_time_12, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_time_12, "--");
    lv_label_set_long_mode(scr_dashboard_label_time_12, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_time_12 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_time_12, lv_color_hex(0x718A92), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_time_12, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_time_12, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_company_12
    scr_dashboard_label_company_12 = lv_label_create(scr_dashboard_cont_log_12);
    lv_obj_set_x(scr_dashboard_label_company_12, 111);
    lv_obj_set_y(scr_dashboard_label_company_12, 14);
    lv_obj_set_width(scr_dashboard_label_company_12, 100);
    lv_obj_set_height(scr_dashboard_label_company_12, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_company_12, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_company_12, "--");
    lv_label_set_long_mode(scr_dashboard_label_company_12, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_company_12 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_company_12, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_company_12, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_company_12, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_result_12
    scr_dashboard_label_result_12 = lv_label_create(scr_dashboard_cont_log_12);
    lv_obj_set_x(scr_dashboard_label_result_12, 282);
    lv_obj_set_y(scr_dashboard_label_result_12, 14);
    lv_obj_set_width(scr_dashboard_label_result_12, 100);
    lv_obj_set_height(scr_dashboard_label_result_12, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_result_12, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_result_12, "--");
    lv_label_set_long_mode(scr_dashboard_label_result_12, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_result_12 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_result_12, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_result_12, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_result_12, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_confidence_12
    scr_dashboard_label_confidence_12 = lv_label_create(scr_dashboard_cont_log_12);
    lv_obj_set_x(scr_dashboard_label_confidence_12, 417);
    lv_obj_set_y(scr_dashboard_label_confidence_12, 14);
    lv_obj_set_width(scr_dashboard_label_confidence_12, 100);
    lv_obj_set_height(scr_dashboard_label_confidence_12, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_confidence_12, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_confidence_12, "--");
    lv_label_set_long_mode(scr_dashboard_label_confidence_12, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_confidence_12 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_confidence_12, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_confidence_12, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_confidence_12, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_log_11
    scr_dashboard_cont_log_11 = lv_obj_create(scr_dashboard_cont_log_page3);
    lv_obj_set_x(scr_dashboard_cont_log_11, 21);
    lv_obj_set_y(scr_dashboard_cont_log_11, 15);
    lv_obj_set_width(scr_dashboard_cont_log_11, 534);
    lv_obj_set_height(scr_dashboard_cont_log_11, 44);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_log_11, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_log_11, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_log_11 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_11, lv_color_hex(0x1E2426), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_11, 160, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_11, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_11, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_11, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_log_11, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_cont_log_11 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_11, lv_color_hex(0x3F5250), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_log_11, lv_color_hex(0x1A2827), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_11, 100, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_log_11, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_11, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_11, 80, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_11, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_cont_log_11, 4, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_11, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_11, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_11, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_11, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    ui_flag_modify(scr_dashboard_cont_log_11, LV_OBJ_FLAG_CHECKABLE, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_log_11, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_11, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_11, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_11, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_11, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_time_11
    scr_dashboard_label_time_11 = lv_label_create(scr_dashboard_cont_log_11);
    lv_obj_set_x(scr_dashboard_label_time_11, 0);
    lv_obj_set_y(scr_dashboard_label_time_11, 14);
    lv_obj_set_width(scr_dashboard_label_time_11, 100);
    lv_obj_set_height(scr_dashboard_label_time_11, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_time_11, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_time_11, "--");
    lv_label_set_long_mode(scr_dashboard_label_time_11, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_time_11 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_time_11, lv_color_hex(0x718A92), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_time_11, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_time_11, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_company_11
    scr_dashboard_label_company_11 = lv_label_create(scr_dashboard_cont_log_11);
    lv_obj_set_x(scr_dashboard_label_company_11, 111);
    lv_obj_set_y(scr_dashboard_label_company_11, 14);
    lv_obj_set_width(scr_dashboard_label_company_11, 100);
    lv_obj_set_height(scr_dashboard_label_company_11, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_company_11, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_company_11, "--");
    lv_label_set_long_mode(scr_dashboard_label_company_11, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_company_11 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_company_11, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_company_11, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_company_11, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_result_11
    scr_dashboard_label_result_11 = lv_label_create(scr_dashboard_cont_log_11);
    lv_obj_set_x(scr_dashboard_label_result_11, 282);
    lv_obj_set_y(scr_dashboard_label_result_11, 14);
    lv_obj_set_width(scr_dashboard_label_result_11, 100);
    lv_obj_set_height(scr_dashboard_label_result_11, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_result_11, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_result_11, "--");
    lv_label_set_long_mode(scr_dashboard_label_result_11, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_result_11 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_result_11, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_result_11, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_result_11, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_confidence_11
    scr_dashboard_label_confidence_11 = lv_label_create(scr_dashboard_cont_log_11);
    lv_obj_set_x(scr_dashboard_label_confidence_11, 417);
    lv_obj_set_y(scr_dashboard_label_confidence_11, 14);
    lv_obj_set_width(scr_dashboard_label_confidence_11, 100);
    lv_obj_set_height(scr_dashboard_label_confidence_11, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_confidence_11, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_confidence_11, "--");
    lv_label_set_long_mode(scr_dashboard_label_confidence_11, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_confidence_11 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_confidence_11, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_confidence_11, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_confidence_11, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_log_page2
    scr_dashboard_cont_log_page2 = lv_obj_create(scr_dashboard_cont_log_record);
    lv_obj_set_x(scr_dashboard_cont_log_page2, -3);
    lv_obj_set_y(scr_dashboard_cont_log_page2, 45);
    lv_obj_set_width(scr_dashboard_cont_log_page2, 578);
    lv_obj_set_height(scr_dashboard_cont_log_page2, 300);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_log_page2, LV_SCROLLBAR_MODE_ACTIVE);
    lv_obj_set_layout(scr_dashboard_cont_log_page2, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_log_page2 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_page2, lv_color_hex(0x232C2C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_log_page2, lv_color_hex(0x1A2827), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_page2, 108, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_log_page2, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_page2, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_page2, 60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_page2, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_log_page2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_page2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_page2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_page2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_page2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_log_page2, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_log_page2, LV_OBJ_FLAG_SCROLL_CHAIN_VER, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_log_page2, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_page2, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_cont_log_10
    scr_dashboard_cont_log_10 = lv_obj_create(scr_dashboard_cont_log_page2);
    lv_obj_set_x(scr_dashboard_cont_log_10, 21);
    lv_obj_set_y(scr_dashboard_cont_log_10, 241);
    lv_obj_set_width(scr_dashboard_cont_log_10, 534);
    lv_obj_set_height(scr_dashboard_cont_log_10, 44);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_log_10, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_log_10, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_log_10 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_10, lv_color_hex(0x1E2426), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_10, 160, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_10, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_10, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_10, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_log_10, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_cont_log_10 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_10, lv_color_hex(0x3F5250), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_log_10, lv_color_hex(0x1A2827), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_10, 100, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_log_10, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_10, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_10, 80, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_10, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_cont_log_10, 4, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_10, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_10, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_10, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_10, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    ui_flag_modify(scr_dashboard_cont_log_10, LV_OBJ_FLAG_CHECKABLE, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_log_10, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_10, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_10, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_10, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_10, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_time_10
    scr_dashboard_label_time_10 = lv_label_create(scr_dashboard_cont_log_10);
    lv_obj_set_x(scr_dashboard_label_time_10, 0);
    lv_obj_set_y(scr_dashboard_label_time_10, 14);
    lv_obj_set_width(scr_dashboard_label_time_10, 100);
    lv_obj_set_height(scr_dashboard_label_time_10, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_time_10, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_time_10, "--");
    lv_label_set_long_mode(scr_dashboard_label_time_10, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_time_10 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_time_10, lv_color_hex(0x718A92), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_time_10, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_time_10, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_company_10
    scr_dashboard_label_company_10 = lv_label_create(scr_dashboard_cont_log_10);
    lv_obj_set_x(scr_dashboard_label_company_10, 111);
    lv_obj_set_y(scr_dashboard_label_company_10, 14);
    lv_obj_set_width(scr_dashboard_label_company_10, 100);
    lv_obj_set_height(scr_dashboard_label_company_10, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_company_10, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_company_10, "--");
    lv_label_set_long_mode(scr_dashboard_label_company_10, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_company_10 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_company_10, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_company_10, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_company_10, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_result_10
    scr_dashboard_label_result_10 = lv_label_create(scr_dashboard_cont_log_10);
    lv_obj_set_x(scr_dashboard_label_result_10, 282);
    lv_obj_set_y(scr_dashboard_label_result_10, 14);
    lv_obj_set_width(scr_dashboard_label_result_10, 100);
    lv_obj_set_height(scr_dashboard_label_result_10, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_result_10, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_result_10, "--");
    lv_label_set_long_mode(scr_dashboard_label_result_10, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_result_10 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_result_10, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_result_10, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_result_10, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_confidence_10
    scr_dashboard_label_confidence_10 = lv_label_create(scr_dashboard_cont_log_10);
    lv_obj_set_x(scr_dashboard_label_confidence_10, 417);
    lv_obj_set_y(scr_dashboard_label_confidence_10, 14);
    lv_obj_set_width(scr_dashboard_label_confidence_10, 100);
    lv_obj_set_height(scr_dashboard_label_confidence_10, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_confidence_10, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_confidence_10, "--");
    lv_label_set_long_mode(scr_dashboard_label_confidence_10, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_confidence_10 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_confidence_10, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_confidence_10, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_confidence_10, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_log_09
    scr_dashboard_cont_log_09 = lv_obj_create(scr_dashboard_cont_log_page2);
    lv_obj_set_x(scr_dashboard_cont_log_09, 21);
    lv_obj_set_y(scr_dashboard_cont_log_09, 185);
    lv_obj_set_width(scr_dashboard_cont_log_09, 534);
    lv_obj_set_height(scr_dashboard_cont_log_09, 44);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_log_09, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_log_09, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_log_09 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_09, lv_color_hex(0x1E2426), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_09, 160, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_09, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_09, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_09, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_log_09, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_09, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_09, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_09, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_09, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_cont_log_09 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_09, lv_color_hex(0x3F5250), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_log_09, lv_color_hex(0x1A2827), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_09, 100, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_log_09, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_09, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_09, 80, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_09, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_cont_log_09, 4, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_09, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_09, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_09, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_09, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    ui_flag_modify(scr_dashboard_cont_log_09, LV_OBJ_FLAG_CHECKABLE, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_log_09, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_09, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_09, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_09, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_09, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_time_09
    scr_dashboard_label_time_09 = lv_label_create(scr_dashboard_cont_log_09);
    lv_obj_set_x(scr_dashboard_label_time_09, 0);
    lv_obj_set_y(scr_dashboard_label_time_09, 14);
    lv_obj_set_width(scr_dashboard_label_time_09, 100);
    lv_obj_set_height(scr_dashboard_label_time_09, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_time_09, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_time_09, "--");
    lv_label_set_long_mode(scr_dashboard_label_time_09, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_time_09 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_time_09, lv_color_hex(0x718A92), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_time_09, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_time_09, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_company_09
    scr_dashboard_label_company_09 = lv_label_create(scr_dashboard_cont_log_09);
    lv_obj_set_x(scr_dashboard_label_company_09, 111);
    lv_obj_set_y(scr_dashboard_label_company_09, 14);
    lv_obj_set_width(scr_dashboard_label_company_09, 100);
    lv_obj_set_height(scr_dashboard_label_company_09, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_company_09, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_company_09, "--");
    lv_label_set_long_mode(scr_dashboard_label_company_09, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_company_09 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_company_09, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_company_09, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_company_09, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_result_09
    scr_dashboard_label_result_09 = lv_label_create(scr_dashboard_cont_log_09);
    lv_obj_set_x(scr_dashboard_label_result_09, 282);
    lv_obj_set_y(scr_dashboard_label_result_09, 14);
    lv_obj_set_width(scr_dashboard_label_result_09, 100);
    lv_obj_set_height(scr_dashboard_label_result_09, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_result_09, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_result_09, "--");
    lv_label_set_long_mode(scr_dashboard_label_result_09, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_result_09 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_result_09, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_result_09, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_result_09, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_confidence_09
    scr_dashboard_label_confidence_09 = lv_label_create(scr_dashboard_cont_log_09);
    lv_obj_set_x(scr_dashboard_label_confidence_09, 417);
    lv_obj_set_y(scr_dashboard_label_confidence_09, 14);
    lv_obj_set_width(scr_dashboard_label_confidence_09, 100);
    lv_obj_set_height(scr_dashboard_label_confidence_09, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_confidence_09, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_confidence_09, "--");
    lv_label_set_long_mode(scr_dashboard_label_confidence_09, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_confidence_09 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_confidence_09, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_confidence_09, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_confidence_09, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_log_08
    scr_dashboard_cont_log_08 = lv_obj_create(scr_dashboard_cont_log_page2);
    lv_obj_set_x(scr_dashboard_cont_log_08, 21);
    lv_obj_set_y(scr_dashboard_cont_log_08, 129);
    lv_obj_set_width(scr_dashboard_cont_log_08, 534);
    lv_obj_set_height(scr_dashboard_cont_log_08, 44);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_log_08, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_log_08, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_log_08 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_08, lv_color_hex(0x1E2426), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_08, 160, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_08, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_08, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_08, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_log_08, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_08, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_08, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_08, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_08, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_cont_log_08 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_08, lv_color_hex(0x3F5250), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_log_08, lv_color_hex(0x1A2827), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_08, 100, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_log_08, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_08, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_08, 80, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_08, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_cont_log_08, 4, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_08, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_08, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_08, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_08, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    ui_flag_modify(scr_dashboard_cont_log_08, LV_OBJ_FLAG_CHECKABLE, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_log_08, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_08, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_08, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_08, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_08, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_time_08
    scr_dashboard_label_time_08 = lv_label_create(scr_dashboard_cont_log_08);
    lv_obj_set_x(scr_dashboard_label_time_08, 0);
    lv_obj_set_y(scr_dashboard_label_time_08, 14);
    lv_obj_set_width(scr_dashboard_label_time_08, 100);
    lv_obj_set_height(scr_dashboard_label_time_08, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_time_08, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_time_08, "--");
    lv_label_set_long_mode(scr_dashboard_label_time_08, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_time_08 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_time_08, lv_color_hex(0x718A92), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_time_08, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_time_08, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_company_08
    scr_dashboard_label_company_08 = lv_label_create(scr_dashboard_cont_log_08);
    lv_obj_set_x(scr_dashboard_label_company_08, 111);
    lv_obj_set_y(scr_dashboard_label_company_08, 14);
    lv_obj_set_width(scr_dashboard_label_company_08, 100);
    lv_obj_set_height(scr_dashboard_label_company_08, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_company_08, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_company_08, "--");
    lv_label_set_long_mode(scr_dashboard_label_company_08, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_company_08 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_company_08, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_company_08, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_company_08, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_result_08
    scr_dashboard_label_result_08 = lv_label_create(scr_dashboard_cont_log_08);
    lv_obj_set_x(scr_dashboard_label_result_08, 282);
    lv_obj_set_y(scr_dashboard_label_result_08, 14);
    lv_obj_set_width(scr_dashboard_label_result_08, 100);
    lv_obj_set_height(scr_dashboard_label_result_08, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_result_08, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_result_08, "--");
    lv_label_set_long_mode(scr_dashboard_label_result_08, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_result_08 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_result_08, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_result_08, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_result_08, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_confidence_08
    scr_dashboard_label_confidence_08 = lv_label_create(scr_dashboard_cont_log_08);
    lv_obj_set_x(scr_dashboard_label_confidence_08, 417);
    lv_obj_set_y(scr_dashboard_label_confidence_08, 14);
    lv_obj_set_width(scr_dashboard_label_confidence_08, 100);
    lv_obj_set_height(scr_dashboard_label_confidence_08, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_confidence_08, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_confidence_08, "--");
    lv_label_set_long_mode(scr_dashboard_label_confidence_08, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_confidence_08 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_confidence_08, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_confidence_08, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_confidence_08, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_log_07
    scr_dashboard_cont_log_07 = lv_obj_create(scr_dashboard_cont_log_page2);
    lv_obj_set_x(scr_dashboard_cont_log_07, 21);
    lv_obj_set_y(scr_dashboard_cont_log_07, 72);
    lv_obj_set_width(scr_dashboard_cont_log_07, 534);
    lv_obj_set_height(scr_dashboard_cont_log_07, 44);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_log_07, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_log_07, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_log_07 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_07, lv_color_hex(0x1E2426), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_07, 160, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_07, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_07, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_07, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_log_07, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_07, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_07, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_07, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_07, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_cont_log_07 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_07, lv_color_hex(0x3F5250), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_log_07, lv_color_hex(0x1A2827), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_07, 100, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_log_07, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_07, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_07, 80, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_07, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_cont_log_07, 4, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_07, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_07, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_07, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_07, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    ui_flag_modify(scr_dashboard_cont_log_07, LV_OBJ_FLAG_CHECKABLE, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_log_07, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_07, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_07, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_07, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_07, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_time_07
    scr_dashboard_label_time_07 = lv_label_create(scr_dashboard_cont_log_07);
    lv_obj_set_x(scr_dashboard_label_time_07, 0);
    lv_obj_set_y(scr_dashboard_label_time_07, 14);
    lv_obj_set_width(scr_dashboard_label_time_07, 100);
    lv_obj_set_height(scr_dashboard_label_time_07, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_time_07, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_time_07, "--");
    lv_label_set_long_mode(scr_dashboard_label_time_07, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_time_07 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_time_07, lv_color_hex(0x718A92), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_time_07, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_time_07, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_company_07
    scr_dashboard_label_company_07 = lv_label_create(scr_dashboard_cont_log_07);
    lv_obj_set_x(scr_dashboard_label_company_07, 111);
    lv_obj_set_y(scr_dashboard_label_company_07, 14);
    lv_obj_set_width(scr_dashboard_label_company_07, 100);
    lv_obj_set_height(scr_dashboard_label_company_07, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_company_07, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_company_07, "--");
    lv_label_set_long_mode(scr_dashboard_label_company_07, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_company_07 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_company_07, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_company_07, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_company_07, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_result_07
    scr_dashboard_label_result_07 = lv_label_create(scr_dashboard_cont_log_07);
    lv_obj_set_x(scr_dashboard_label_result_07, 282);
    lv_obj_set_y(scr_dashboard_label_result_07, 14);
    lv_obj_set_width(scr_dashboard_label_result_07, 100);
    lv_obj_set_height(scr_dashboard_label_result_07, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_result_07, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_result_07, "--");
    lv_label_set_long_mode(scr_dashboard_label_result_07, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_result_07 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_result_07, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_result_07, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_result_07, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_confidence_07
    scr_dashboard_label_confidence_07 = lv_label_create(scr_dashboard_cont_log_07);
    lv_obj_set_x(scr_dashboard_label_confidence_07, 417);
    lv_obj_set_y(scr_dashboard_label_confidence_07, 14);
    lv_obj_set_width(scr_dashboard_label_confidence_07, 100);
    lv_obj_set_height(scr_dashboard_label_confidence_07, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_confidence_07, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_confidence_07, "--");
    lv_label_set_long_mode(scr_dashboard_label_confidence_07, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_confidence_07 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_confidence_07, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_confidence_07, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_confidence_07, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_log_06
    scr_dashboard_cont_log_06 = lv_obj_create(scr_dashboard_cont_log_page2);
    lv_obj_set_x(scr_dashboard_cont_log_06, 21);
    lv_obj_set_y(scr_dashboard_cont_log_06, 15);
    lv_obj_set_width(scr_dashboard_cont_log_06, 534);
    lv_obj_set_height(scr_dashboard_cont_log_06, 44);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_log_06, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_log_06, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_log_06 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_06, lv_color_hex(0x1E2426), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_06, 160, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_06, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_06, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_06, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_log_06, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_06, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_06, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_06, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_06, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_cont_log_06 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_06, lv_color_hex(0x3F5250), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_log_06, lv_color_hex(0x1A2827), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_06, 100, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_log_06, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_06, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_06, 80, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_06, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_cont_log_06, 4, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_06, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_06, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_06, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_06, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    ui_flag_modify(scr_dashboard_cont_log_06, LV_OBJ_FLAG_CHECKABLE, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_log_06, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_06, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_06, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_06, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_06, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_time_06
    scr_dashboard_label_time_06 = lv_label_create(scr_dashboard_cont_log_06);
    lv_obj_set_x(scr_dashboard_label_time_06, 0);
    lv_obj_set_y(scr_dashboard_label_time_06, 14);
    lv_obj_set_width(scr_dashboard_label_time_06, 100);
    lv_obj_set_height(scr_dashboard_label_time_06, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_time_06, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_time_06, "--");
    lv_label_set_long_mode(scr_dashboard_label_time_06, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_time_06 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_time_06, lv_color_hex(0x718A92), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_time_06, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_time_06, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_company_06
    scr_dashboard_label_company_06 = lv_label_create(scr_dashboard_cont_log_06);
    lv_obj_set_x(scr_dashboard_label_company_06, 111);
    lv_obj_set_y(scr_dashboard_label_company_06, 14);
    lv_obj_set_width(scr_dashboard_label_company_06, 100);
    lv_obj_set_height(scr_dashboard_label_company_06, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_company_06, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_company_06, "--");
    lv_label_set_long_mode(scr_dashboard_label_company_06, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_company_06 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_company_06, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_company_06, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_company_06, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_result_06
    scr_dashboard_label_result_06 = lv_label_create(scr_dashboard_cont_log_06);
    lv_obj_set_x(scr_dashboard_label_result_06, 282);
    lv_obj_set_y(scr_dashboard_label_result_06, 14);
    lv_obj_set_width(scr_dashboard_label_result_06, 100);
    lv_obj_set_height(scr_dashboard_label_result_06, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_result_06, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_result_06, "--");
    lv_label_set_long_mode(scr_dashboard_label_result_06, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_result_06 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_result_06, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_result_06, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_result_06, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_confidence_06
    scr_dashboard_label_confidence_06 = lv_label_create(scr_dashboard_cont_log_06);
    lv_obj_set_x(scr_dashboard_label_confidence_06, 417);
    lv_obj_set_y(scr_dashboard_label_confidence_06, 14);
    lv_obj_set_width(scr_dashboard_label_confidence_06, 100);
    lv_obj_set_height(scr_dashboard_label_confidence_06, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_confidence_06, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_confidence_06, "--");
    lv_label_set_long_mode(scr_dashboard_label_confidence_06, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_confidence_06 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_confidence_06, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_confidence_06, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_confidence_06, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_log_page1
    scr_dashboard_cont_log_page1 = lv_obj_create(scr_dashboard_cont_log_record);
    lv_obj_set_x(scr_dashboard_cont_log_page1, -3);
    lv_obj_set_y(scr_dashboard_cont_log_page1, 45);
    lv_obj_set_width(scr_dashboard_cont_log_page1, 578);
    lv_obj_set_height(scr_dashboard_cont_log_page1, 300);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_log_page1, LV_SCROLLBAR_MODE_ACTIVE);
    lv_obj_set_layout(scr_dashboard_cont_log_page1, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_log_page1 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_page1, lv_color_hex(0x232C2C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_log_page1, lv_color_hex(0x1A2827), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_page1, 40, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_log_page1, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_page1, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_page1, 60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_page1, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_log_page1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_page1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_page1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_page1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_page1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_log_page1, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_page1, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_page1, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_page1, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_page1, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_cont_log_05
    scr_dashboard_cont_log_05 = lv_obj_create(scr_dashboard_cont_log_page1);
    lv_obj_set_x(scr_dashboard_cont_log_05, 21);
    lv_obj_set_y(scr_dashboard_cont_log_05, 241);
    lv_obj_set_width(scr_dashboard_cont_log_05, 534);
    lv_obj_set_height(scr_dashboard_cont_log_05, 44);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_log_05, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_log_05, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_log_05 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_05, lv_color_hex(0x1E2426), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_05, 160, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_05, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_05, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_05, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_log_05, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_05, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_05, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_05, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_05, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_cont_log_05 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_05, lv_color_hex(0x3F5250), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_log_05, lv_color_hex(0x1A2827), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_05, 100, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_log_05, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_05, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_05, 80, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_05, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_cont_log_05, 4, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_05, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_05, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_05, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_05, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    ui_flag_modify(scr_dashboard_cont_log_05, LV_OBJ_FLAG_CHECKABLE, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_log_05, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_05, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_05, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_05, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_05, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_time_05
    scr_dashboard_label_time_05 = lv_label_create(scr_dashboard_cont_log_05);
    lv_obj_set_x(scr_dashboard_label_time_05, 0);
    lv_obj_set_y(scr_dashboard_label_time_05, 14);
    lv_obj_set_width(scr_dashboard_label_time_05, 100);
    lv_obj_set_height(scr_dashboard_label_time_05, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_time_05, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_time_05, "--");
    lv_label_set_long_mode(scr_dashboard_label_time_05, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_time_05 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_time_05, lv_color_hex(0x718A92), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_time_05, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_time_05, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_company_05
    scr_dashboard_label_company_05 = lv_label_create(scr_dashboard_cont_log_05);
    lv_obj_set_x(scr_dashboard_label_company_05, 111);
    lv_obj_set_y(scr_dashboard_label_company_05, 14);
    lv_obj_set_width(scr_dashboard_label_company_05, 100);
    lv_obj_set_height(scr_dashboard_label_company_05, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_company_05, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_company_05, "--");
    lv_label_set_long_mode(scr_dashboard_label_company_05, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_company_05 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_company_05, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_company_05, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_company_05, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_result_05
    scr_dashboard_label_result_05 = lv_label_create(scr_dashboard_cont_log_05);
    lv_obj_set_x(scr_dashboard_label_result_05, 282);
    lv_obj_set_y(scr_dashboard_label_result_05, 14);
    lv_obj_set_width(scr_dashboard_label_result_05, 100);
    lv_obj_set_height(scr_dashboard_label_result_05, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_result_05, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_result_05, "--");
    lv_label_set_long_mode(scr_dashboard_label_result_05, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_result_05 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_result_05, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_result_05, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_result_05, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_confidence_05
    scr_dashboard_label_confidence_05 = lv_label_create(scr_dashboard_cont_log_05);
    lv_obj_set_x(scr_dashboard_label_confidence_05, 417);
    lv_obj_set_y(scr_dashboard_label_confidence_05, 14);
    lv_obj_set_width(scr_dashboard_label_confidence_05, 100);
    lv_obj_set_height(scr_dashboard_label_confidence_05, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_confidence_05, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_confidence_05, "--");
    lv_label_set_long_mode(scr_dashboard_label_confidence_05, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_confidence_05 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_confidence_05, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_confidence_05, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_confidence_05, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_log_04
    scr_dashboard_cont_log_04 = lv_obj_create(scr_dashboard_cont_log_page1);
    lv_obj_set_x(scr_dashboard_cont_log_04, 21);
    lv_obj_set_y(scr_dashboard_cont_log_04, 185);
    lv_obj_set_width(scr_dashboard_cont_log_04, 534);
    lv_obj_set_height(scr_dashboard_cont_log_04, 44);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_log_04, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_log_04, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_log_04 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_04, lv_color_hex(0x1E2426), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_04, 160, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_04, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_04, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_04, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_log_04, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_04, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_04, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_04, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_04, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_cont_log_04 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_04, lv_color_hex(0x3F5250), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_log_04, lv_color_hex(0x1A2827), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_04, 100, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_log_04, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_04, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_04, 80, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_04, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_cont_log_04, 4, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_04, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_04, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_04, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_04, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    ui_flag_modify(scr_dashboard_cont_log_04, LV_OBJ_FLAG_CHECKABLE, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_log_04, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_04, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_04, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_04, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_04, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_time_04
    scr_dashboard_label_time_04 = lv_label_create(scr_dashboard_cont_log_04);
    lv_obj_set_x(scr_dashboard_label_time_04, 0);
    lv_obj_set_y(scr_dashboard_label_time_04, 14);
    lv_obj_set_width(scr_dashboard_label_time_04, 100);
    lv_obj_set_height(scr_dashboard_label_time_04, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_time_04, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_time_04, "--");
    lv_label_set_long_mode(scr_dashboard_label_time_04, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_time_04 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_time_04, lv_color_hex(0x718A92), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_time_04, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_time_04, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_company_04
    scr_dashboard_label_company_04 = lv_label_create(scr_dashboard_cont_log_04);
    lv_obj_set_x(scr_dashboard_label_company_04, 111);
    lv_obj_set_y(scr_dashboard_label_company_04, 14);
    lv_obj_set_width(scr_dashboard_label_company_04, 100);
    lv_obj_set_height(scr_dashboard_label_company_04, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_company_04, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_company_04, "--");
    lv_label_set_long_mode(scr_dashboard_label_company_04, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_company_04 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_company_04, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_company_04, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_company_04, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_result_04
    scr_dashboard_label_result_04 = lv_label_create(scr_dashboard_cont_log_04);
    lv_obj_set_x(scr_dashboard_label_result_04, 282);
    lv_obj_set_y(scr_dashboard_label_result_04, 14);
    lv_obj_set_width(scr_dashboard_label_result_04, 100);
    lv_obj_set_height(scr_dashboard_label_result_04, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_result_04, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_result_04, "--");
    lv_label_set_long_mode(scr_dashboard_label_result_04, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_result_04 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_result_04, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_result_04, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_result_04, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_confidence_04
    scr_dashboard_label_confidence_04 = lv_label_create(scr_dashboard_cont_log_04);
    lv_obj_set_x(scr_dashboard_label_confidence_04, 417);
    lv_obj_set_y(scr_dashboard_label_confidence_04, 14);
    lv_obj_set_width(scr_dashboard_label_confidence_04, 100);
    lv_obj_set_height(scr_dashboard_label_confidence_04, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_confidence_04, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_confidence_04, "--");
    lv_label_set_long_mode(scr_dashboard_label_confidence_04, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_confidence_04 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_confidence_04, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_confidence_04, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_confidence_04, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_log_03
    scr_dashboard_cont_log_03 = lv_obj_create(scr_dashboard_cont_log_page1);
    lv_obj_set_x(scr_dashboard_cont_log_03, 21);
    lv_obj_set_y(scr_dashboard_cont_log_03, 129);
    lv_obj_set_width(scr_dashboard_cont_log_03, 534);
    lv_obj_set_height(scr_dashboard_cont_log_03, 44);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_log_03, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_log_03, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_log_03 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_03, lv_color_hex(0x1E2426), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_03, 160, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_03, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_03, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_03, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_log_03, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_03, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_03, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_03, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_03, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_cont_log_03 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_03, lv_color_hex(0x3F5250), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_log_03, lv_color_hex(0x1A2827), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_03, 100, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_log_03, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_03, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_03, 80, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_03, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_cont_log_03, 4, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_03, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_03, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_03, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_03, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    ui_flag_modify(scr_dashboard_cont_log_03, LV_OBJ_FLAG_CHECKABLE, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_log_03, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_03, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_03, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_03, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_03, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_time_03
    scr_dashboard_label_time_03 = lv_label_create(scr_dashboard_cont_log_03);
    lv_obj_set_x(scr_dashboard_label_time_03, 0);
    lv_obj_set_y(scr_dashboard_label_time_03, 14);
    lv_obj_set_width(scr_dashboard_label_time_03, 100);
    lv_obj_set_height(scr_dashboard_label_time_03, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_time_03, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_time_03, "--");
    lv_label_set_long_mode(scr_dashboard_label_time_03, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_time_03 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_time_03, lv_color_hex(0x718A92), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_time_03, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_time_03, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_company_03
    scr_dashboard_label_company_03 = lv_label_create(scr_dashboard_cont_log_03);
    lv_obj_set_x(scr_dashboard_label_company_03, 111);
    lv_obj_set_y(scr_dashboard_label_company_03, 14);
    lv_obj_set_width(scr_dashboard_label_company_03, 100);
    lv_obj_set_height(scr_dashboard_label_company_03, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_company_03, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_company_03, "--");
    lv_label_set_long_mode(scr_dashboard_label_company_03, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_company_03 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_company_03, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_company_03, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_company_03, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_result_03
    scr_dashboard_label_result_03 = lv_label_create(scr_dashboard_cont_log_03);
    lv_obj_set_x(scr_dashboard_label_result_03, 282);
    lv_obj_set_y(scr_dashboard_label_result_03, 14);
    lv_obj_set_width(scr_dashboard_label_result_03, 100);
    lv_obj_set_height(scr_dashboard_label_result_03, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_result_03, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_result_03, "--");
    lv_label_set_long_mode(scr_dashboard_label_result_03, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_result_03 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_result_03, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_result_03, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_result_03, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_confidence_03
    scr_dashboard_label_confidence_03 = lv_label_create(scr_dashboard_cont_log_03);
    lv_obj_set_x(scr_dashboard_label_confidence_03, 417);
    lv_obj_set_y(scr_dashboard_label_confidence_03, 14);
    lv_obj_set_width(scr_dashboard_label_confidence_03, 100);
    lv_obj_set_height(scr_dashboard_label_confidence_03, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_confidence_03, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_confidence_03, "--");
    lv_label_set_long_mode(scr_dashboard_label_confidence_03, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_confidence_03 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_confidence_03, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_confidence_03, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_confidence_03, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_log_02
    scr_dashboard_cont_log_02 = lv_obj_create(scr_dashboard_cont_log_page1);
    lv_obj_set_x(scr_dashboard_cont_log_02, 21);
    lv_obj_set_y(scr_dashboard_cont_log_02, 72);
    lv_obj_set_width(scr_dashboard_cont_log_02, 534);
    lv_obj_set_height(scr_dashboard_cont_log_02, 44);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_log_02, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_log_02, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_log_02 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_02, lv_color_hex(0x1E2426), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_02, 160, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_02, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_02, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_02, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_log_02, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_02, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_02, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_02, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_02, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_cont_log_02 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_02, lv_color_hex(0x3F5250), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_log_02, lv_color_hex(0x1A2827), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_02, 100, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_log_02, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_02, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_02, 80, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_02, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_cont_log_02, 4, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_02, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_02, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_02, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_02, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    ui_flag_modify(scr_dashboard_cont_log_02, LV_OBJ_FLAG_CHECKABLE, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_log_02, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_02, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_02, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_02, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_02, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_time_02
    scr_dashboard_label_time_02 = lv_label_create(scr_dashboard_cont_log_02);
    lv_obj_set_x(scr_dashboard_label_time_02, 0);
    lv_obj_set_y(scr_dashboard_label_time_02, 14);
    lv_obj_set_width(scr_dashboard_label_time_02, 100);
    lv_obj_set_height(scr_dashboard_label_time_02, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_time_02, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_time_02, "--");
    lv_label_set_long_mode(scr_dashboard_label_time_02, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_time_02 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_time_02, lv_color_hex(0x718A92), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_time_02, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_time_02, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_company_02
    scr_dashboard_label_company_02 = lv_label_create(scr_dashboard_cont_log_02);
    lv_obj_set_x(scr_dashboard_label_company_02, 111);
    lv_obj_set_y(scr_dashboard_label_company_02, 14);
    lv_obj_set_width(scr_dashboard_label_company_02, 100);
    lv_obj_set_height(scr_dashboard_label_company_02, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_company_02, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_company_02, "--");
    lv_label_set_long_mode(scr_dashboard_label_company_02, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_company_02 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_company_02, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_company_02, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_company_02, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_result_02
    scr_dashboard_label_result_02 = lv_label_create(scr_dashboard_cont_log_02);
    lv_obj_set_x(scr_dashboard_label_result_02, 282);
    lv_obj_set_y(scr_dashboard_label_result_02, 14);
    lv_obj_set_width(scr_dashboard_label_result_02, 100);
    lv_obj_set_height(scr_dashboard_label_result_02, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_result_02, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_result_02, "--");
    lv_label_set_long_mode(scr_dashboard_label_result_02, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_result_02 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_result_02, lv_color_hex(0xEAF6F3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_result_02, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_result_02, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_confidence_02
    scr_dashboard_label_confidence_02 = lv_label_create(scr_dashboard_cont_log_02);
    lv_obj_set_x(scr_dashboard_label_confidence_02, 417);
    lv_obj_set_y(scr_dashboard_label_confidence_02, 14);
    lv_obj_set_width(scr_dashboard_label_confidence_02, 100);
    lv_obj_set_height(scr_dashboard_label_confidence_02, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_confidence_02, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_confidence_02, "--");
    lv_label_set_long_mode(scr_dashboard_label_confidence_02, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_confidence_02 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_confidence_02, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_confidence_02, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_confidence_02, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_log_01
    scr_dashboard_cont_log_01 = lv_obj_create(scr_dashboard_cont_log_page1);
    lv_obj_set_x(scr_dashboard_cont_log_01, 21);
    lv_obj_set_y(scr_dashboard_cont_log_01, 15);
    lv_obj_set_width(scr_dashboard_cont_log_01, 534);
    lv_obj_set_height(scr_dashboard_cont_log_01, 44);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_log_01, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_log_01, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_log_01 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_01, lv_color_hex(0x1E2426), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_01, 160, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_01, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_01, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_01, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_log_01, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_01, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_01, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_01, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_01, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_cont_log_01 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_cont_log_01, lv_color_hex(0x3F5250), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_log_01, lv_color_hex(0x1A2827), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_log_01, 100, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_log_01, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_cont_log_01, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_cont_log_01, 80, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_cont_log_01, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_cont_log_01, 4, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(scr_dashboard_cont_log_01, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_log_01, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_left(scr_dashboard_cont_log_01, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_pad_right(scr_dashboard_cont_log_01, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    ui_flag_modify(scr_dashboard_cont_log_01, LV_OBJ_FLAG_CHECKABLE, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_log_01, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_01, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_01, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_01, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_log_01, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    ui_state_modify(scr_dashboard_cont_log_01, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
    // Create scr_dashboard_label_time_01
    scr_dashboard_label_time_01 = lv_label_create(scr_dashboard_cont_log_01);
    lv_obj_set_x(scr_dashboard_label_time_01, 0);
    lv_obj_set_y(scr_dashboard_label_time_01, 14);
    lv_obj_set_width(scr_dashboard_label_time_01, 100);
    lv_obj_set_height(scr_dashboard_label_time_01, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_time_01, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_time_01, "--");
    lv_label_set_long_mode(scr_dashboard_label_time_01, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_time_01 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_time_01, lv_color_hex(0x718A92), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_time_01, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_time_01, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_company_01
    scr_dashboard_label_company_01 = lv_label_create(scr_dashboard_cont_log_01);
    lv_obj_set_x(scr_dashboard_label_company_01, 111);
    lv_obj_set_y(scr_dashboard_label_company_01, 14);
    lv_obj_set_width(scr_dashboard_label_company_01, 100);
    lv_obj_set_height(scr_dashboard_label_company_01, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_company_01, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_company_01, "--");
    lv_label_set_long_mode(scr_dashboard_label_company_01, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_company_01 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_company_01, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_company_01, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_company_01, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_result_01
    scr_dashboard_label_result_01 = lv_label_create(scr_dashboard_cont_log_01);
    lv_obj_set_x(scr_dashboard_label_result_01, 282);
    lv_obj_set_y(scr_dashboard_label_result_01, 14);
    lv_obj_set_width(scr_dashboard_label_result_01, 100);
    lv_obj_set_height(scr_dashboard_label_result_01, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_result_01, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_result_01, "--");
    lv_label_set_long_mode(scr_dashboard_label_result_01, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_result_01 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_result_01, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_result_01, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_result_01, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_confidence_01
    scr_dashboard_label_confidence_01 = lv_label_create(scr_dashboard_cont_log_01);
    lv_obj_set_x(scr_dashboard_label_confidence_01, 417);
    lv_obj_set_y(scr_dashboard_label_confidence_01, 14);
    lv_obj_set_width(scr_dashboard_label_confidence_01, 100);
    lv_obj_set_height(scr_dashboard_label_confidence_01, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_confidence_01, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_confidence_01, "--");
    lv_label_set_long_mode(scr_dashboard_label_confidence_01, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_confidence_01 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_confidence_01, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_confidence_01, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_confidence_01, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_text_log
    scr_dashboard_label_text_log = lv_label_create(scr_dashboard_cont_log_record);
    lv_obj_set_x(scr_dashboard_label_text_log, 32);
    lv_obj_set_y(scr_dashboard_label_text_log, 12);
    lv_obj_set_width(scr_dashboard_label_text_log, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_text_log, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_text_log, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_text_log, "日志记录");
    lv_label_set_long_mode(scr_dashboard_label_text_log, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_text_log - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_text_log, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_text_log, &lv_font_Misans_Heavy_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_text_log, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_under1
    scr_dashboard_cont_under1 = lv_obj_create(scr_dashboard_cont_log_record);
    lv_obj_set_x(scr_dashboard_cont_under1, 42);
    lv_obj_set_y(scr_dashboard_cont_under1, 32);
    lv_obj_set_width(scr_dashboard_cont_under1, 36);
    lv_obj_set_height(scr_dashboard_cont_under1, 2);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_under1, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_under1, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_under1 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_under1, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_under1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_under1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_under1, 999, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_under1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_under1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_under1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_under1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_under1, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_under1, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_under1, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_under1, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_under1, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_current_page
    scr_dashboard_label_current_page = lv_label_create(scr_dashboard_cont_log_record);
    lv_obj_set_x(scr_dashboard_label_current_page, 248);
    lv_obj_set_y(scr_dashboard_label_current_page, 358);
    lv_obj_set_width(scr_dashboard_label_current_page, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_current_page, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_current_page, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_current_page, "第1页/共4页");
    lv_label_set_long_mode(scr_dashboard_label_current_page, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_current_page - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_current_page, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_current_page, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_current_page, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_imgbtn_page_back
    scr_dashboard_imgbtn_page_back = lv_imgbtn_create(scr_dashboard_cont_log_record);
    lv_obj_set_x(scr_dashboard_imgbtn_page_back, 209);
    lv_obj_set_y(scr_dashboard_imgbtn_page_back, 354);
    lv_obj_set_width(scr_dashboard_imgbtn_page_back, 22);
    lv_obj_set_height(scr_dashboard_imgbtn_page_back, 22);
    lv_obj_set_scrollbar_mode(scr_dashboard_imgbtn_page_back, LV_SCROLLBAR_MODE_OFF);
    scr_dashboard_imgbtn_page_back_label = lv_label_create(scr_dashboard_imgbtn_page_back);
    lv_label_set_text(scr_dashboard_imgbtn_page_back_label, "");
    lv_label_set_long_mode(scr_dashboard_imgbtn_page_back_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(scr_dashboard_imgbtn_page_back_label, LV_PCT(100));
    lv_obj_set_align(scr_dashboard_imgbtn_page_back_label, LV_ALIGN_CENTER);
    lv_imgbtn_set_src(scr_dashboard_imgbtn_page_back, LV_IMGBTN_STATE_RELEASED, NULL, &back_22x22, NULL);
    lv_obj_add_flag(scr_dashboard_imgbtn_page_back, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_set_style_text_align(scr_dashboard_imgbtn_page_back, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_imgbtn_page_next
    scr_dashboard_imgbtn_page_next = lv_imgbtn_create(scr_dashboard_cont_log_record);
    lv_obj_set_x(scr_dashboard_imgbtn_page_next, 341);
    lv_obj_set_y(scr_dashboard_imgbtn_page_next, 354);
    lv_obj_set_width(scr_dashboard_imgbtn_page_next, 22);
    lv_obj_set_height(scr_dashboard_imgbtn_page_next, 22);
    lv_obj_set_scrollbar_mode(scr_dashboard_imgbtn_page_next, LV_SCROLLBAR_MODE_OFF);
    scr_dashboard_imgbtn_page_next_label = lv_label_create(scr_dashboard_imgbtn_page_next);
    lv_label_set_text(scr_dashboard_imgbtn_page_next_label, "");
    lv_label_set_long_mode(scr_dashboard_imgbtn_page_next_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(scr_dashboard_imgbtn_page_next_label, LV_PCT(100));
    lv_obj_set_align(scr_dashboard_imgbtn_page_next_label, LV_ALIGN_CENTER);
    lv_imgbtn_set_src(scr_dashboard_imgbtn_page_next, LV_IMGBTN_STATE_RELEASED, NULL, &next_22x22, NULL);
    lv_obj_add_flag(scr_dashboard_imgbtn_page_next, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_set_style_text_align(scr_dashboard_imgbtn_page_next, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_set
    scr_dashboard_cont_set = lv_obj_create(scr_dashboard_cont_background2);
    lv_obj_set_x(scr_dashboard_cont_set, 3);
    lv_obj_set_y(scr_dashboard_cont_set, 85);
    lv_obj_set_width(scr_dashboard_cont_set, 1010);
    lv_obj_set_height(scr_dashboard_cont_set, 423);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_set, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_set, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_set - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_opa(scr_dashboard_cont_set, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_set, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_set, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_set, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_set, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_set, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_set, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_set, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_set, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_set, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_set, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_set, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_cont_set_line
    scr_dashboard_cont_set_line = lv_obj_create(scr_dashboard_cont_set);
    lv_obj_set_x(scr_dashboard_cont_set_line, 24);
    lv_obj_set_y(scr_dashboard_cont_set_line, 12);
    lv_obj_set_width(scr_dashboard_cont_set_line, 200);
    lv_obj_set_height(scr_dashboard_cont_set_line, 311);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_set_line, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_set_line, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_set_line - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_set_line, lv_color_hex(0x232C2C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_set_line, lv_color_hex(0x111919), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_set_line, 160, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_set_line, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_set_line, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_set_line, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_set_line, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_set_line, 16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_set_line, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_set_line, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_set_line, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_set_line, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_set_line, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_set_line, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_set_line, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_set_line, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_set_line, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_imgbtn_net
    scr_dashboard_imgbtn_net = lv_imgbtn_create(scr_dashboard_cont_set_line);
    lv_obj_set_x(scr_dashboard_imgbtn_net, 5.5);
    lv_obj_set_y(scr_dashboard_imgbtn_net, 122);
    lv_obj_set_width(scr_dashboard_imgbtn_net, 188);
    lv_obj_set_height(scr_dashboard_imgbtn_net, 54);
    lv_obj_set_scrollbar_mode(scr_dashboard_imgbtn_net, LV_SCROLLBAR_MODE_OFF);
    scr_dashboard_imgbtn_net_label = lv_label_create(scr_dashboard_imgbtn_net);
    lv_label_set_text(scr_dashboard_imgbtn_net_label, "");
    lv_label_set_long_mode(scr_dashboard_imgbtn_net_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(scr_dashboard_imgbtn_net_label, LV_PCT(100));
    lv_obj_set_align(scr_dashboard_imgbtn_net_label, LV_ALIGN_CENTER);
    lv_imgbtn_set_src(scr_dashboard_imgbtn_net, LV_IMGBTN_STATE_RELEASED, NULL, &net_188x54, NULL);
    lv_obj_add_flag(scr_dashboard_imgbtn_net, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_set_style_text_align(scr_dashboard_imgbtn_net, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_imgbtn_control
    scr_dashboard_imgbtn_control = lv_imgbtn_create(scr_dashboard_cont_set_line);
    lv_obj_set_x(scr_dashboard_imgbtn_control, 5.5);
    lv_obj_set_y(scr_dashboard_imgbtn_control, 68);
    lv_obj_set_width(scr_dashboard_imgbtn_control, 188);
    lv_obj_set_height(scr_dashboard_imgbtn_control, 54);
    lv_obj_set_scrollbar_mode(scr_dashboard_imgbtn_control, LV_SCROLLBAR_MODE_OFF);
    scr_dashboard_imgbtn_control_label = lv_label_create(scr_dashboard_imgbtn_control);
    lv_label_set_text(scr_dashboard_imgbtn_control_label, "");
    lv_label_set_long_mode(scr_dashboard_imgbtn_control_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(scr_dashboard_imgbtn_control_label, LV_PCT(100));
    lv_obj_set_align(scr_dashboard_imgbtn_control_label, LV_ALIGN_CENTER);
    lv_imgbtn_set_src(scr_dashboard_imgbtn_control, LV_IMGBTN_STATE_RELEASED, NULL, &control_188x54, NULL);
    lv_obj_add_flag(scr_dashboard_imgbtn_control, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_set_style_text_align(scr_dashboard_imgbtn_control, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_imgbtn_about
    scr_dashboard_imgbtn_about = lv_imgbtn_create(scr_dashboard_cont_set_line);
    lv_obj_set_x(scr_dashboard_imgbtn_about, 5.5);
    lv_obj_set_y(scr_dashboard_imgbtn_about, 230);
    lv_obj_set_width(scr_dashboard_imgbtn_about, 188);
    lv_obj_set_height(scr_dashboard_imgbtn_about, 54);
    lv_obj_set_scrollbar_mode(scr_dashboard_imgbtn_about, LV_SCROLLBAR_MODE_OFF);
    scr_dashboard_imgbtn_about_label = lv_label_create(scr_dashboard_imgbtn_about);
    lv_label_set_text(scr_dashboard_imgbtn_about_label, "");
    lv_label_set_long_mode(scr_dashboard_imgbtn_about_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(scr_dashboard_imgbtn_about_label, LV_PCT(100));
    lv_obj_set_align(scr_dashboard_imgbtn_about_label, LV_ALIGN_CENTER);
    lv_imgbtn_set_src(scr_dashboard_imgbtn_about, LV_IMGBTN_STATE_RELEASED, NULL, &about_188x54, NULL);
    lv_obj_add_flag(scr_dashboard_imgbtn_about, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_set_style_text_align(scr_dashboard_imgbtn_about, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_imgbtn_detect
    scr_dashboard_imgbtn_detect = lv_imgbtn_create(scr_dashboard_cont_set_line);
    lv_obj_set_x(scr_dashboard_imgbtn_detect, 5.5);
    lv_obj_set_y(scr_dashboard_imgbtn_detect, 14);
    lv_obj_set_width(scr_dashboard_imgbtn_detect, 188);
    lv_obj_set_height(scr_dashboard_imgbtn_detect, 54);
    lv_obj_set_scrollbar_mode(scr_dashboard_imgbtn_detect, LV_SCROLLBAR_MODE_OFF);
    scr_dashboard_imgbtn_detect_label = lv_label_create(scr_dashboard_imgbtn_detect);
    lv_label_set_text(scr_dashboard_imgbtn_detect_label, "");
    lv_label_set_long_mode(scr_dashboard_imgbtn_detect_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(scr_dashboard_imgbtn_detect_label, LV_PCT(100));
    lv_obj_set_align(scr_dashboard_imgbtn_detect_label, LV_ALIGN_CENTER);
    lv_imgbtn_set_src(scr_dashboard_imgbtn_detect, LV_IMGBTN_STATE_RELEASED, NULL, &detect_188x54, NULL);
    lv_imgbtn_set_src(scr_dashboard_imgbtn_detect, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &detect_b_188x54, NULL);
    lv_obj_add_flag(scr_dashboard_imgbtn_detect, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_set_style_text_align(scr_dashboard_imgbtn_detect, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_state_modify(scr_dashboard_imgbtn_detect, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
    // Create scr_dashboard_imgbtn_display
    scr_dashboard_imgbtn_display = lv_imgbtn_create(scr_dashboard_cont_set_line);
    lv_obj_set_x(scr_dashboard_imgbtn_display, 5.5);
    lv_obj_set_y(scr_dashboard_imgbtn_display, 176);
    lv_obj_set_width(scr_dashboard_imgbtn_display, 188);
    lv_obj_set_height(scr_dashboard_imgbtn_display, 54);
    lv_obj_set_scrollbar_mode(scr_dashboard_imgbtn_display, LV_SCROLLBAR_MODE_OFF);
    scr_dashboard_imgbtn_display_label = lv_label_create(scr_dashboard_imgbtn_display);
    lv_label_set_text(scr_dashboard_imgbtn_display_label, "");
    lv_label_set_long_mode(scr_dashboard_imgbtn_display_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(scr_dashboard_imgbtn_display_label, LV_PCT(100));
    lv_obj_set_align(scr_dashboard_imgbtn_display_label, LV_ALIGN_CENTER);
    lv_imgbtn_set_src(scr_dashboard_imgbtn_display, LV_IMGBTN_STATE_RELEASED, NULL, &display_188x54, NULL);
    lv_obj_add_flag(scr_dashboard_imgbtn_display, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_set_style_text_align(scr_dashboard_imgbtn_display, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_imgbtn_display, LV_OBJ_FLAG_CHECKABLE, UI_FLAG_ACTION_ADD);
    // Create scr_dashboard_cont_about
    scr_dashboard_cont_about = lv_obj_create(scr_dashboard_cont_set);
    lv_obj_set_x(scr_dashboard_cont_about, 258);
    lv_obj_set_y(scr_dashboard_cont_about, 10);
    lv_obj_set_width(scr_dashboard_cont_about, 700);
    lv_obj_set_height(scr_dashboard_cont_about, 375);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_about, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_about, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_about - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_about, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_about, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_about, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_about, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_about, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_about, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_about, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_about, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_about, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_about, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_about, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_about, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_about, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_cont_about_info
    scr_dashboard_cont_about_info = lv_obj_create(scr_dashboard_cont_about);
    lv_obj_set_x(scr_dashboard_cont_about_info, 15);
    lv_obj_set_y(scr_dashboard_cont_about_info, 7);
    lv_obj_set_width(scr_dashboard_cont_about_info, 669);
    lv_obj_set_height(scr_dashboard_cont_about_info, 104);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_about_info, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_about_info, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_about_info - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_about_info, lv_color_hex(0x2D4145), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_about_info, lv_color_hex(0x314144), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_about_info, 44, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_about_info, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_about_info, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_about_info, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_about_info, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_about_info, 14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_about_info, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_about_info, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_about_info, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_about_info, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_about_info, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_about_info, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_about_info, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_about_info, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_about_info, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_runtime
    scr_dashboard_label_runtime = lv_label_create(scr_dashboard_cont_about_info);
    lv_obj_set_x(scr_dashboard_label_runtime, 23);
    lv_obj_set_y(scr_dashboard_label_runtime, 18);
    lv_obj_set_width(scr_dashboard_label_runtime, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_runtime, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_runtime, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_runtime, "运行时长");
    lv_label_set_long_mode(scr_dashboard_label_runtime, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_runtime - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_runtime, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_runtime, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_runtime, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_runtime, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_memory
    scr_dashboard_label_memory = lv_label_create(scr_dashboard_cont_about_info);
    lv_obj_set_x(scr_dashboard_label_memory, 23);
    lv_obj_set_y(scr_dashboard_label_memory, 45);
    lv_obj_set_width(scr_dashboard_label_memory, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_memory, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_memory, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_memory, "可用内存/PSRAM");
    lv_label_set_long_mode(scr_dashboard_label_memory, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_memory - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_memory, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_memory, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_memory, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_memory, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_about_runtime_value
    scr_dashboard_label_about_runtime_value = lv_label_create(scr_dashboard_cont_about_info);
    lv_obj_set_x(scr_dashboard_label_about_runtime_value, 571);
    lv_obj_set_y(scr_dashboard_label_about_runtime_value, 18);
    lv_obj_set_width(scr_dashboard_label_about_runtime_value, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_about_runtime_value, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_about_runtime_value, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_about_runtime_value, "---");
    lv_label_set_long_mode(scr_dashboard_label_about_runtime_value, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_about_runtime_value - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_about_runtime_value, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_about_runtime_value, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_about_runtime_value, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_about_runtime_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_model_info
    scr_dashboard_label_model_info = lv_label_create(scr_dashboard_cont_about_info);
    lv_obj_set_x(scr_dashboard_label_model_info, 23);
    lv_obj_set_y(scr_dashboard_label_model_info, 71);
    lv_obj_set_width(scr_dashboard_label_model_info, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_model_info, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_model_info, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_model_info, "模型信息");
    lv_label_set_long_mode(scr_dashboard_label_model_info, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_model_info - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_model_info, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_model_info, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_model_info, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_model_info, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_memory_value
    scr_dashboard_label_memory_value = lv_label_create(scr_dashboard_cont_about_info);
    lv_obj_set_x(scr_dashboard_label_memory_value, 571);
    lv_obj_set_y(scr_dashboard_label_memory_value, 45);
    lv_obj_set_width(scr_dashboard_label_memory_value, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_memory_value, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_memory_value, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_memory_value, "---");
    lv_label_set_long_mode(scr_dashboard_label_memory_value, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_memory_value - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_memory_value, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_memory_value, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_memory_value, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_memory_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_model_info_value
    scr_dashboard_label_model_info_value = lv_label_create(scr_dashboard_cont_about_info);
    lv_obj_set_x(scr_dashboard_label_model_info_value, 571);
    lv_obj_set_y(scr_dashboard_label_model_info_value, 71);
    lv_obj_set_width(scr_dashboard_label_model_info_value, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_model_info_value, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_model_info_value, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_model_info_value, "---");
    lv_label_set_long_mode(scr_dashboard_label_model_info_value, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_model_info_value - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_model_info_value, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_model_info_value, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_model_info_value, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_model_info_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_about_reset
    scr_dashboard_cont_about_reset = lv_obj_create(scr_dashboard_cont_about);
    lv_obj_set_x(scr_dashboard_cont_about_reset, 16);
    lv_obj_set_y(scr_dashboard_cont_about_reset, 154);
    lv_obj_set_width(scr_dashboard_cont_about_reset, 669);
    lv_obj_set_height(scr_dashboard_cont_about_reset, 56);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_about_reset, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_about_reset, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_about_reset - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_about_reset, lv_color_hex(0x2D4145), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_about_reset, lv_color_hex(0x314144), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_about_reset, 44, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_about_reset, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_about_reset, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_about_reset, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_about_reset, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_about_reset, 14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_about_reset, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_about_reset, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_about_reset, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_about_reset, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_about_reset, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_about_reset, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_about_reset, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_about_reset, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_about_reset, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_factory_reset
    scr_dashboard_label_factory_reset = lv_label_create(scr_dashboard_cont_about_reset);
    lv_obj_set_x(scr_dashboard_label_factory_reset, 25);
    lv_obj_set_y(scr_dashboard_label_factory_reset, 21);
    lv_obj_set_width(scr_dashboard_label_factory_reset, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_factory_reset, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_factory_reset, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_factory_reset, "恢复出厂设置");
    lv_label_set_long_mode(scr_dashboard_label_factory_reset, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_factory_reset - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_factory_reset, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_factory_reset, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_factory_reset, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_factory_reset, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_btn_factory_reset
    scr_dashboard_btn_factory_reset = lv_btn_create(scr_dashboard_cont_about_reset);
    lv_obj_set_x(scr_dashboard_btn_factory_reset, 605);
    lv_obj_set_y(scr_dashboard_btn_factory_reset, 15);
    lv_obj_set_width(scr_dashboard_btn_factory_reset, 20);
    lv_obj_set_height(scr_dashboard_btn_factory_reset, 20);
    scr_dashboard_btn_factory_reset_label = lv_label_create(scr_dashboard_btn_factory_reset);
    lv_obj_set_scrollbar_mode(scr_dashboard_btn_factory_reset, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_btn_factory_reset_label, "");
    lv_obj_set_style_pad_all(scr_dashboard_btn_factory_reset, 0, LV_STATE_DEFAULT);
    lv_obj_align(scr_dashboard_btn_factory_reset_label, LV_ALIGN_CENTER, 0, 0);
    // Add style for scr_dashboard_btn_factory_reset - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_btn_factory_reset, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_btn_factory_reset, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_btn_factory_reset, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_btn_factory_reset, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_btn_factory_reset, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(scr_dashboard_btn_factory_reset, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_btn_factory_reset - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_btn_factory_reset, lv_color_hex(0x58BFF8), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_btn_factory_reset, 255, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_btn_factory_reset, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_btn_factory_reset, 255, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_btn_factory_reset, 4, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_color(scr_dashboard_btn_factory_reset, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_CHECKED);
    // Add style for scr_dashboard_btn_factory_reset - LV_PART_MAIN | LV_STATE_PRESSED
    lv_obj_set_style_bg_color(scr_dashboard_btn_factory_reset, lv_color_hex(0x58BFF8), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(scr_dashboard_btn_factory_reset, 255, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_border_color(scr_dashboard_btn_factory_reset, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_border_opa(scr_dashboard_btn_factory_reset, 255, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_border_width(scr_dashboard_btn_factory_reset, 4, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_text_color(scr_dashboard_btn_factory_reset, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_text_align(scr_dashboard_btn_factory_reset, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_btn_factory_reset, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_align(scr_dashboard_btn_factory_reset, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_PRESSED);
    // Create scr_dashboard_cont_about_restart
    scr_dashboard_cont_about_restart = lv_obj_create(scr_dashboard_cont_about);
    lv_obj_set_x(scr_dashboard_cont_about_restart, 17);
    lv_obj_set_y(scr_dashboard_cont_about_restart, 223);
    lv_obj_set_width(scr_dashboard_cont_about_restart, 669);
    lv_obj_set_height(scr_dashboard_cont_about_restart, 56);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_about_restart, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_about_restart, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_about_restart - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_about_restart, lv_color_hex(0x2D4145), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_about_restart, lv_color_hex(0x314144), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_about_restart, 44, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_about_restart, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_about_restart, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_about_restart, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_about_restart, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_about_restart, 14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_about_restart, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_about_restart, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_about_restart, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_about_restart, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_about_restart, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_about_restart, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_about_restart, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_about_restart, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_about_restart, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_restart
    scr_dashboard_label_restart = lv_label_create(scr_dashboard_cont_about_restart);
    lv_obj_set_x(scr_dashboard_label_restart, 25);
    lv_obj_set_y(scr_dashboard_label_restart, 21);
    lv_obj_set_width(scr_dashboard_label_restart, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_restart, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_restart, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_restart, "重启设备");
    lv_label_set_long_mode(scr_dashboard_label_restart, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_restart - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_restart, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_restart, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_restart, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_restart, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_btn_restart
    scr_dashboard_btn_restart = lv_btn_create(scr_dashboard_cont_about_restart);
    lv_obj_set_x(scr_dashboard_btn_restart, 605);
    lv_obj_set_y(scr_dashboard_btn_restart, 15);
    lv_obj_set_width(scr_dashboard_btn_restart, 20);
    lv_obj_set_height(scr_dashboard_btn_restart, 20);
    scr_dashboard_btn_restart_label = lv_label_create(scr_dashboard_btn_restart);
    lv_obj_set_scrollbar_mode(scr_dashboard_btn_restart, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_btn_restart_label, "");
    lv_obj_set_style_pad_all(scr_dashboard_btn_restart, 0, LV_STATE_DEFAULT);
    lv_obj_align(scr_dashboard_btn_restart_label, LV_ALIGN_CENTER, 0, 0);
    // Add style for scr_dashboard_btn_restart - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_btn_restart, lv_color_hex(0xC0C0C0), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_btn_restart, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_btn_restart, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_btn_restart, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_btn_restart, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(scr_dashboard_btn_restart, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_btn_restart - LV_PART_MAIN | LV_STATE_PRESSED
    lv_obj_set_style_bg_color(scr_dashboard_btn_restart, lv_color_hex(0x58BFF8), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(scr_dashboard_btn_restart, 255, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_border_color(scr_dashboard_btn_restart, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_border_opa(scr_dashboard_btn_restart, 255, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_border_width(scr_dashboard_btn_restart, 3, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_text_color(scr_dashboard_btn_restart, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_PRESSED);
    // Add style for scr_dashboard_btn_restart - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_btn_restart, lv_color_hex(0x58BFF8), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_btn_restart, 255, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_btn_restart, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_btn_restart, 255, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_btn_restart, 3, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_color(scr_dashboard_btn_restart, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_align(scr_dashboard_btn_restart, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_btn_restart, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_text_align(scr_dashboard_btn_restart, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_CHECKED);
    // Create scr_dashboard_cont_net
    scr_dashboard_cont_net = lv_obj_create(scr_dashboard_cont_set);
    lv_obj_set_x(scr_dashboard_cont_net, 258);
    lv_obj_set_y(scr_dashboard_cont_net, 10);
    lv_obj_set_width(scr_dashboard_cont_net, 700);
    lv_obj_set_height(scr_dashboard_cont_net, 375);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_net, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_net, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_net - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_net, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_net, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_net, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_net, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_net, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_net, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_net, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_net, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_net, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_net, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_net, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_net, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_net, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_cont_net_ip
    scr_dashboard_cont_net_ip = lv_obj_create(scr_dashboard_cont_net);
    lv_obj_set_x(scr_dashboard_cont_net_ip, 16);
    lv_obj_set_y(scr_dashboard_cont_net_ip, 0);
    lv_obj_set_width(scr_dashboard_cont_net_ip, 669);
    lv_obj_set_height(scr_dashboard_cont_net_ip, 78);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_net_ip, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_net_ip, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_net_ip - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_net_ip, lv_color_hex(0x2D4145), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_net_ip, lv_color_hex(0x314144), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_net_ip, 44, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_net_ip, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_net_ip, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_net_ip, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_net_ip, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_net_ip, 14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_net_ip, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_net_ip, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_net_ip, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_net_ip, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_net_ip, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_net_ip, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_net_ip, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_net_ip, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_net_ip, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_local_ip
    scr_dashboard_label_local_ip = lv_label_create(scr_dashboard_cont_net_ip);
    lv_obj_set_x(scr_dashboard_label_local_ip, 23);
    lv_obj_set_y(scr_dashboard_label_local_ip, 18);
    lv_obj_set_width(scr_dashboard_label_local_ip, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_local_ip, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_local_ip, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_local_ip, "本机IP");
    lv_label_set_long_mode(scr_dashboard_label_local_ip, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_local_ip - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_local_ip, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_local_ip, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_local_ip, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_local_ip, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_host_ip
    scr_dashboard_label_host_ip = lv_label_create(scr_dashboard_cont_net_ip);
    lv_obj_set_x(scr_dashboard_label_host_ip, 23);
    lv_obj_set_y(scr_dashboard_label_host_ip, 46);
    lv_obj_set_width(scr_dashboard_label_host_ip, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_host_ip, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_host_ip, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_host_ip, "主机IP");
    lv_label_set_long_mode(scr_dashboard_label_host_ip, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_host_ip - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_host_ip, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_host_ip, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_host_ip, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_host_ip, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_local_ip_value
    scr_dashboard_label_local_ip_value = lv_label_create(scr_dashboard_cont_net_ip);
    lv_obj_set_x(scr_dashboard_label_local_ip_value, 509);
    lv_obj_set_y(scr_dashboard_label_local_ip_value, 18);
    lv_obj_set_width(scr_dashboard_label_local_ip_value, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_local_ip_value, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_local_ip_value, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_local_ip_value, "--------");
    lv_label_set_long_mode(scr_dashboard_label_local_ip_value, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_local_ip_value - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_local_ip_value, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_local_ip_value, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_local_ip_value, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_local_ip_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_host_ip_value
    scr_dashboard_label_host_ip_value = lv_label_create(scr_dashboard_cont_net_ip);
    lv_obj_set_x(scr_dashboard_label_host_ip_value, 510);
    lv_obj_set_y(scr_dashboard_label_host_ip_value, 46);
    lv_obj_set_width(scr_dashboard_label_host_ip_value, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_host_ip_value, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_host_ip_value, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_host_ip_value, "--------");
    lv_label_set_long_mode(scr_dashboard_label_host_ip_value, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_host_ip_value - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_host_ip_value, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_host_ip_value, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_host_ip_value, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_host_ip_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_net_interval
    scr_dashboard_cont_net_interval = lv_obj_create(scr_dashboard_cont_net);
    lv_obj_set_x(scr_dashboard_cont_net_interval, 16);
    lv_obj_set_y(scr_dashboard_cont_net_interval, 103);
    lv_obj_set_width(scr_dashboard_cont_net_interval, 669);
    lv_obj_set_height(scr_dashboard_cont_net_interval, 56);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_net_interval, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_net_interval, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_net_interval - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_net_interval, lv_color_hex(0x2D4145), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_net_interval, lv_color_hex(0x314144), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_net_interval, 44, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_net_interval, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_net_interval, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_net_interval, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_net_interval, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_net_interval, 14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_net_interval, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_net_interval, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_net_interval, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_net_interval, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_net_interval, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_net_interval, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_net_interval, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_net_interval, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_net_interval, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_slider_report_interval
    scr_dashboard_slider_report_interval = lv_slider_create(scr_dashboard_cont_net_interval);
    lv_obj_set_x(scr_dashboard_slider_report_interval, 137);
    lv_obj_set_y(scr_dashboard_slider_report_interval, 24);
    lv_obj_set_width(scr_dashboard_slider_report_interval, 470);
    lv_obj_set_height(scr_dashboard_slider_report_interval, 8);
    lv_obj_set_scrollbar_mode(scr_dashboard_slider_report_interval, LV_SCROLLBAR_MODE_OFF);
    lv_slider_set_range(scr_dashboard_slider_report_interval, 0, 60);
    lv_slider_set_value(scr_dashboard_slider_report_interval, 0, LV_ANIM_OFF);
    // Add style for scr_dashboard_slider_report_interval - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_slider_report_interval, lv_color_hex(0x05161A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_slider_report_interval, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_slider_report_interval - LV_PART_INDICATOR | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_slider_report_interval, lv_color_hex(0x217A96), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_slider_report_interval, lv_color_hex(0x84D5E0), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_slider_report_interval, 200, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_slider_report_interval, LV_GRAD_DIR_NONE, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_slider_report_interval - LV_PART_KNOB | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_slider_report_interval, lv_color_hex(0xBDD3D6), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_slider_report_interval, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_report_interval
    scr_dashboard_label_report_interval = lv_label_create(scr_dashboard_cont_net_interval);
    lv_obj_set_x(scr_dashboard_label_report_interval, 23);
    lv_obj_set_y(scr_dashboard_label_report_interval, 21);
    lv_obj_set_width(scr_dashboard_label_report_interval, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_report_interval, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_report_interval, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_report_interval, "上报时间间隔");
    lv_label_set_long_mode(scr_dashboard_label_report_interval, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_report_interval - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_report_interval, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_report_interval, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_report_interval, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_report_interval, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_report_interval_value
    scr_dashboard_label_report_interval_value = lv_label_create(scr_dashboard_cont_net_interval);
    lv_obj_set_x(scr_dashboard_label_report_interval_value, 623);
    lv_obj_set_y(scr_dashboard_label_report_interval_value, 21);
    lv_obj_set_width(scr_dashboard_label_report_interval_value, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_report_interval_value, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_report_interval_value, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_report_interval_value, "00s");
    lv_label_set_long_mode(scr_dashboard_label_report_interval_value, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_report_interval_value - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_report_interval_value, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_report_interval_value, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_report_interval_value, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_report_interval_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_net_image
    scr_dashboard_cont_net_image = lv_obj_create(scr_dashboard_cont_net);
    lv_obj_set_x(scr_dashboard_cont_net_image, 16);
    lv_obj_set_y(scr_dashboard_cont_net_image, 176);
    lv_obj_set_width(scr_dashboard_cont_net_image, 669);
    lv_obj_set_height(scr_dashboard_cont_net_image, 56);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_net_image, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_net_image, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_net_image - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_net_image, lv_color_hex(0x2D4145), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_net_image, lv_color_hex(0x314144), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_net_image, 44, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_net_image, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_net_image, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_net_image, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_net_image, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_net_image, 14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_net_image, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_net_image, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_net_image, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_net_image, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_net_image, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_net_image, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_net_image, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_net_image, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_net_image, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_report_image
    scr_dashboard_label_report_image = lv_label_create(scr_dashboard_cont_net_image);
    lv_obj_set_x(scr_dashboard_label_report_image, 25);
    lv_obj_set_y(scr_dashboard_label_report_image, 21);
    lv_obj_set_width(scr_dashboard_label_report_image, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_report_image, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_report_image, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_report_image, "是否上报图像");
    lv_label_set_long_mode(scr_dashboard_label_report_image, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_report_image - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_report_image, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_report_image, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_report_image, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_report_image, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_sw_report_image
    scr_dashboard_sw_report_image = lv_switch_create(scr_dashboard_cont_net_image);
    lv_obj_set_x(scr_dashboard_sw_report_image, 579);
    lv_obj_set_y(scr_dashboard_sw_report_image, 18);
    lv_obj_set_width(scr_dashboard_sw_report_image, 40);
    lv_obj_set_height(scr_dashboard_sw_report_image, 20);
    lv_obj_set_scrollbar_mode(scr_dashboard_sw_report_image, LV_SCROLLBAR_MODE_OFF);
    // Create scr_dashboard_cont_net_metrics
    scr_dashboard_cont_net_metrics = lv_obj_create(scr_dashboard_cont_net);
    lv_obj_set_x(scr_dashboard_cont_net_metrics, 16);
    lv_obj_set_y(scr_dashboard_cont_net_metrics, 248);
    lv_obj_set_width(scr_dashboard_cont_net_metrics, 669);
    lv_obj_set_height(scr_dashboard_cont_net_metrics, 56);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_net_metrics, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_net_metrics, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_net_metrics - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_net_metrics, lv_color_hex(0x2D4145), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_net_metrics, lv_color_hex(0x314144), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_net_metrics, 44, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_net_metrics, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_net_metrics, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_net_metrics, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_net_metrics, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_net_metrics, 14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_net_metrics, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_net_metrics, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_net_metrics, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_net_metrics, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_net_metrics, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_net_metrics, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_net_metrics, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_net_metrics, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_net_metrics, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_report_metrics
    scr_dashboard_label_report_metrics = lv_label_create(scr_dashboard_cont_net_metrics);
    lv_obj_set_x(scr_dashboard_label_report_metrics, 25);
    lv_obj_set_y(scr_dashboard_label_report_metrics, 21);
    lv_obj_set_width(scr_dashboard_label_report_metrics, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_report_metrics, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_report_metrics, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_report_metrics, "是否上报指标");
    lv_label_set_long_mode(scr_dashboard_label_report_metrics, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_report_metrics - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_report_metrics, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_report_metrics, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_report_metrics, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_report_metrics, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_sw_report_metrics
    scr_dashboard_sw_report_metrics = lv_switch_create(scr_dashboard_cont_net_metrics);
    lv_obj_set_x(scr_dashboard_sw_report_metrics, 579);
    lv_obj_set_y(scr_dashboard_sw_report_metrics, 18);
    lv_obj_set_width(scr_dashboard_sw_report_metrics, 40);
    lv_obj_set_height(scr_dashboard_sw_report_metrics, 20);
    lv_obj_set_scrollbar_mode(scr_dashboard_sw_report_metrics, LV_SCROLLBAR_MODE_OFF);
    // Create scr_dashboard_cont_display
    scr_dashboard_cont_display = lv_obj_create(scr_dashboard_cont_set);
    lv_obj_set_x(scr_dashboard_cont_display, 258);
    lv_obj_set_y(scr_dashboard_cont_display, 10);
    lv_obj_set_width(scr_dashboard_cont_display, 700);
    lv_obj_set_height(scr_dashboard_cont_display, 375);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_display, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_display, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_display - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_display, lv_color_hex(0x0B1517), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_display, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_display, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_display, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_display, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_display, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_display, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_display, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_display, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_display, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_display, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_display, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_display, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_cont_bright
    scr_dashboard_cont_bright = lv_obj_create(scr_dashboard_cont_display);
    lv_obj_set_x(scr_dashboard_cont_bright, 16);
    lv_obj_set_y(scr_dashboard_cont_bright, 8);
    lv_obj_set_width(scr_dashboard_cont_bright, 669);
    lv_obj_set_height(scr_dashboard_cont_bright, 56);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_bright, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_bright, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_bright - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_bright, lv_color_hex(0x2D4145), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_bright, lv_color_hex(0x314144), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_bright, 44, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_bright, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_bright, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_bright, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_bright, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_bright, 14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_bright, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_bright, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_bright, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_bright, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_bright, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_bright, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_bright, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_bright, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_bright, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_text_bright
    scr_dashboard_label_text_bright = lv_label_create(scr_dashboard_cont_bright);
    lv_obj_set_x(scr_dashboard_label_text_bright, 19);
    lv_obj_set_y(scr_dashboard_label_text_bright, 21);
    lv_obj_set_width(scr_dashboard_label_text_bright, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_text_bright, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_text_bright, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_text_bright, "屏幕亮度");
    lv_label_set_long_mode(scr_dashboard_label_text_bright, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_text_bright - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_text_bright, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_text_bright, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_text_bright, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_text_bright, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_runtime_bright__data
    scr_dashboard_label_runtime_bright__data = lv_label_create(scr_dashboard_cont_bright);
    lv_obj_set_x(scr_dashboard_label_runtime_bright__data, 613);
    lv_obj_set_y(scr_dashboard_label_runtime_bright__data, 21);
    lv_obj_set_width(scr_dashboard_label_runtime_bright__data, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_runtime_bright__data, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_runtime_bright__data, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_runtime_bright__data, "00%");
    lv_label_set_long_mode(scr_dashboard_label_runtime_bright__data, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_runtime_bright__data - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_runtime_bright__data, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_runtime_bright__data, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_runtime_bright__data, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_runtime_bright__data, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_slider_82EIlsYJ
    scr_dashboard_slider_82EIlsYJ = lv_slider_create(scr_dashboard_cont_bright);
    lv_obj_set_x(scr_dashboard_slider_82EIlsYJ, 102);
    lv_obj_set_y(scr_dashboard_slider_82EIlsYJ, 24);
    lv_obj_set_width(scr_dashboard_slider_82EIlsYJ, 500);
    lv_obj_set_height(scr_dashboard_slider_82EIlsYJ, 8);
    lv_obj_set_scrollbar_mode(scr_dashboard_slider_82EIlsYJ, LV_SCROLLBAR_MODE_OFF);
    lv_slider_set_range(scr_dashboard_slider_82EIlsYJ, 0, 100);
    lv_slider_set_value(scr_dashboard_slider_82EIlsYJ, 0, LV_ANIM_OFF);
    // Add style for scr_dashboard_slider_82EIlsYJ - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_slider_82EIlsYJ, lv_color_hex(0x05161A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_slider_82EIlsYJ, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_slider_82EIlsYJ - LV_PART_INDICATOR | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_slider_82EIlsYJ, lv_color_hex(0x217A96), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_slider_82EIlsYJ, lv_color_hex(0x84D5E0), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_slider_82EIlsYJ, 200, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_slider_82EIlsYJ, LV_GRAD_DIR_NONE, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_slider_82EIlsYJ - LV_PART_KNOB | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_slider_82EIlsYJ, lv_color_hex(0xBDD3D6), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_slider_82EIlsYJ, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_control
    scr_dashboard_cont_control = lv_obj_create(scr_dashboard_cont_set);
    lv_obj_set_x(scr_dashboard_cont_control, 258);
    lv_obj_set_y(scr_dashboard_cont_control, 10);
    lv_obj_set_width(scr_dashboard_cont_control, 700);
    lv_obj_set_height(scr_dashboard_cont_control, 375);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_control, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_control, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_control - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_control, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_control, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_control, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_control, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_control, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_control, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_control, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_control, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_control, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_control, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_control, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_control, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_control, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_cont_control_pid
    scr_dashboard_cont_control_pid = lv_obj_create(scr_dashboard_cont_control);
    lv_obj_set_x(scr_dashboard_cont_control_pid, 16);
    lv_obj_set_y(scr_dashboard_cont_control_pid, 8);
    lv_obj_set_width(scr_dashboard_cont_control_pid, 669);
    lv_obj_set_height(scr_dashboard_cont_control_pid, 56);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_control_pid, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_control_pid, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_control_pid - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_control_pid, lv_color_hex(0x2D4145), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_control_pid, lv_color_hex(0x314144), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_control_pid, 44, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_control_pid, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_control_pid, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_control_pid, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_control_pid, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_control_pid, 14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_control_pid, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_control_pid, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_control_pid, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_control_pid, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_control_pid, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_control_pid, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_control_pid, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_control_pid, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_control_pid, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_pid_control
    scr_dashboard_label_pid_control = lv_label_create(scr_dashboard_cont_control_pid);
    lv_obj_set_x(scr_dashboard_label_pid_control, 25);
    lv_obj_set_y(scr_dashboard_label_pid_control, 21);
    lv_obj_set_width(scr_dashboard_label_pid_control, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_pid_control, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_pid_control, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_pid_control, "控制模式");
    lv_label_set_long_mode(scr_dashboard_label_pid_control, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_pid_control - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_pid_control, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_pid_control, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_pid_control, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_pid_control, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_btn_openloop_mode
    scr_dashboard_btn_openloop_mode = lv_btn_create(scr_dashboard_cont_control_pid);
    lv_obj_set_x(scr_dashboard_btn_openloop_mode, 570);
    lv_obj_set_y(scr_dashboard_btn_openloop_mode, 11);
    lv_obj_set_width(scr_dashboard_btn_openloop_mode, 90);
    lv_obj_set_height(scr_dashboard_btn_openloop_mode, 32);
    scr_dashboard_btn_openloop_mode_label = lv_label_create(scr_dashboard_btn_openloop_mode);
    lv_obj_set_scrollbar_mode(scr_dashboard_btn_openloop_mode, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_btn_openloop_mode_label, "开环控制");
    lv_obj_set_style_pad_all(scr_dashboard_btn_openloop_mode, 0, LV_STATE_DEFAULT);
    lv_obj_align(scr_dashboard_btn_openloop_mode_label, LV_ALIGN_CENTER, 0, 0);
    // Add style for scr_dashboard_btn_openloop_mode - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_btn_openloop_mode, lv_color_hex(0x111919), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_btn_openloop_mode, lv_color_hex(0x232C2C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_btn_openloop_mode, 180, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_btn_openloop_mode, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_btn_openloop_mode, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_btn_openloop_mode, 80, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_btn_openloop_mode, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_btn_openloop_mode, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(scr_dashboard_btn_openloop_mode, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(scr_dashboard_btn_openloop_mode, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_btn_openloop_mode - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_btn_openloop_mode, lv_color_hex(0x111919), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_color(scr_dashboard_btn_openloop_mode, lv_color_hex(0x232C2C), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_btn_openloop_mode, 180, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_btn_openloop_mode, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_btn_openloop_mode, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_btn_openloop_mode, 80, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_btn_openloop_mode, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_btn_openloop_mode, 10, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_color(scr_dashboard_btn_openloop_mode, lv_color_hex(0x71CF97), LV_PART_MAIN | LV_STATE_CHECKED);
    // Add style for scr_dashboard_btn_openloop_mode - LV_PART_MAIN | LV_STATE_PRESSED
    lv_obj_set_style_bg_color(scr_dashboard_btn_openloop_mode, lv_color_hex(0x111919), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_grad_color(scr_dashboard_btn_openloop_mode, lv_color_hex(0x232C2C), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(scr_dashboard_btn_openloop_mode, 180, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_btn_openloop_mode, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_border_color(scr_dashboard_btn_openloop_mode, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_border_opa(scr_dashboard_btn_openloop_mode, 80, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_border_width(scr_dashboard_btn_openloop_mode, 1, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_radius(scr_dashboard_btn_openloop_mode, 10, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_text_color(scr_dashboard_btn_openloop_mode, lv_color_hex(0x71CF97), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_text_font(scr_dashboard_btn_openloop_mode, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_btn_openloop_mode, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_btn_openloop_mode, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_align(scr_dashboard_btn_openloop_mode, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_font(scr_dashboard_btn_openloop_mode, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_text_align(scr_dashboard_btn_openloop_mode, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_PRESSED);
    ui_flag_modify(scr_dashboard_btn_openloop_mode, LV_OBJ_FLAG_CHECKABLE, UI_FLAG_ACTION_ADD);
    ui_state_modify(scr_dashboard_btn_openloop_mode, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
    // Create scr_dashboard_btn_pid_mode
    scr_dashboard_btn_pid_mode = lv_btn_create(scr_dashboard_cont_control_pid);
    lv_obj_set_x(scr_dashboard_btn_pid_mode, 470);
    lv_obj_set_y(scr_dashboard_btn_pid_mode, 11);
    lv_obj_set_width(scr_dashboard_btn_pid_mode, 90);
    lv_obj_set_height(scr_dashboard_btn_pid_mode, 32);
    scr_dashboard_btn_pid_mode_label = lv_label_create(scr_dashboard_btn_pid_mode);
    lv_obj_set_scrollbar_mode(scr_dashboard_btn_pid_mode, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_btn_pid_mode_label, "PID控制");
    lv_obj_set_style_pad_all(scr_dashboard_btn_pid_mode, 0, LV_STATE_DEFAULT);
    lv_obj_align(scr_dashboard_btn_pid_mode_label, LV_ALIGN_CENTER, 0, 0);
    // Add style for scr_dashboard_btn_pid_mode - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_btn_pid_mode, lv_color_hex(0x111919), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_btn_pid_mode, lv_color_hex(0x232C2C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_btn_pid_mode, 180, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_btn_pid_mode, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_btn_pid_mode, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_btn_pid_mode, 80, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_btn_pid_mode, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_btn_pid_mode, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(scr_dashboard_btn_pid_mode, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(scr_dashboard_btn_pid_mode, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_btn_pid_mode - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_btn_pid_mode, lv_color_hex(0x111919), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_color(scr_dashboard_btn_pid_mode, lv_color_hex(0x232C2C), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_btn_pid_mode, 180, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_btn_pid_mode, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_btn_pid_mode, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_btn_pid_mode, 80, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_btn_pid_mode, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_btn_pid_mode, 10, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_color(scr_dashboard_btn_pid_mode, lv_color_hex(0x71CF97), LV_PART_MAIN | LV_STATE_CHECKED);
    // Add style for scr_dashboard_btn_pid_mode - LV_PART_MAIN | LV_STATE_PRESSED
    lv_obj_set_style_bg_color(scr_dashboard_btn_pid_mode, lv_color_hex(0x111919), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_grad_color(scr_dashboard_btn_pid_mode, lv_color_hex(0x232C2C), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(scr_dashboard_btn_pid_mode, 180, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_btn_pid_mode, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_border_color(scr_dashboard_btn_pid_mode, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_border_opa(scr_dashboard_btn_pid_mode, 80, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_border_width(scr_dashboard_btn_pid_mode, 1, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_radius(scr_dashboard_btn_pid_mode, 10, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_text_color(scr_dashboard_btn_pid_mode, lv_color_hex(0x71CF97), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_text_font(scr_dashboard_btn_pid_mode, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_btn_pid_mode, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_btn_pid_mode, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_align(scr_dashboard_btn_pid_mode, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_font(scr_dashboard_btn_pid_mode, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_text_align(scr_dashboard_btn_pid_mode, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_PRESSED);
    ui_flag_modify(scr_dashboard_btn_pid_mode, LV_OBJ_FLAG_CHECKABLE, UI_FLAG_ACTION_ADD);
    // Create scr_dashboard_cont_control_openloop
    scr_dashboard_cont_control_openloop = lv_obj_create(scr_dashboard_cont_control);
    lv_obj_set_x(scr_dashboard_cont_control_openloop, 16);
    lv_obj_set_y(scr_dashboard_cont_control_openloop, 95);
    lv_obj_set_width(scr_dashboard_cont_control_openloop, 669);
    lv_obj_set_height(scr_dashboard_cont_control_openloop, 56);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_control_openloop, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_control_openloop, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_control_openloop - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_control_openloop, lv_color_hex(0x2D4145), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_control_openloop, lv_color_hex(0x314144), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_control_openloop, 44, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_control_openloop, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_control_openloop, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_control_openloop, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_control_openloop, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_control_openloop, 14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_control_openloop, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_control_openloop, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_control_openloop, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_control_openloop, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_control_openloop, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_control_openloop, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_control_openloop, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_control_openloop, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_control_openloop, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_slider_openloop_speed
    scr_dashboard_slider_openloop_speed = lv_slider_create(scr_dashboard_cont_control_openloop);
    lv_obj_set_x(scr_dashboard_slider_openloop_speed, 139);
    lv_obj_set_y(scr_dashboard_slider_openloop_speed, 24);
    lv_obj_set_width(scr_dashboard_slider_openloop_speed, 500);
    lv_obj_set_height(scr_dashboard_slider_openloop_speed, 8);
    lv_obj_set_scrollbar_mode(scr_dashboard_slider_openloop_speed, LV_SCROLLBAR_MODE_OFF);
    lv_slider_set_range(scr_dashboard_slider_openloop_speed, 0, 100);
    lv_slider_set_value(scr_dashboard_slider_openloop_speed, 0, LV_ANIM_OFF);
    // Add style for scr_dashboard_slider_openloop_speed - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_slider_openloop_speed, lv_color_hex(0x05161A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_slider_openloop_speed, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_slider_openloop_speed - LV_PART_INDICATOR | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_slider_openloop_speed, lv_color_hex(0x217A96), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_slider_openloop_speed, lv_color_hex(0x84D5E0), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_slider_openloop_speed, 200, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_slider_openloop_speed, LV_GRAD_DIR_NONE, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_slider_openloop_speed - LV_PART_KNOB | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_slider_openloop_speed, lv_color_hex(0xBDD3D6), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_slider_openloop_speed, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_openloop_speed
    scr_dashboard_label_openloop_speed = lv_label_create(scr_dashboard_cont_control_openloop);
    lv_obj_set_x(scr_dashboard_label_openloop_speed, 27);
    lv_obj_set_y(scr_dashboard_label_openloop_speed, 21);
    lv_obj_set_width(scr_dashboard_label_openloop_speed, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_openloop_speed, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_openloop_speed, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_openloop_speed, "开环控制速度");
    lv_label_set_long_mode(scr_dashboard_label_openloop_speed, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_openloop_speed - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_openloop_speed, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_openloop_speed, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_openloop_speed, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_openloop_speed, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_control_pid_speed
    scr_dashboard_cont_control_pid_speed = lv_obj_create(scr_dashboard_cont_control);
    lv_obj_set_x(scr_dashboard_cont_control_pid_speed, 16);
    lv_obj_set_y(scr_dashboard_cont_control_pid_speed, 95);
    lv_obj_set_width(scr_dashboard_cont_control_pid_speed, 669);
    lv_obj_set_height(scr_dashboard_cont_control_pid_speed, 56);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_control_pid_speed, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_control_pid_speed, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_control_pid_speed - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_control_pid_speed, lv_color_hex(0x2D4145), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_control_pid_speed, lv_color_hex(0x314144), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_control_pid_speed, 44, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_control_pid_speed, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_control_pid_speed, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_control_pid_speed, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_control_pid_speed, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_control_pid_speed, 14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_control_pid_speed, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_control_pid_speed, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_control_pid_speed, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_control_pid_speed, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_control_pid_speed, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
    ui_flag_modify(scr_dashboard_cont_control_pid_speed, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_control_pid_speed, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_control_pid_speed, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_control_pid_speed, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_control_pid_speed, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_pid_speed
    scr_dashboard_label_pid_speed = lv_label_create(scr_dashboard_cont_control_pid_speed);
    lv_obj_set_x(scr_dashboard_label_pid_speed, 27);
    lv_obj_set_y(scr_dashboard_label_pid_speed, 21);
    lv_obj_set_width(scr_dashboard_label_pid_speed, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_pid_speed, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_pid_speed, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_pid_speed, "设置PID速度");
    lv_label_set_long_mode(scr_dashboard_label_pid_speed, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_pid_speed - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_pid_speed, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_pid_speed, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_pid_speed, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_pid_speed, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_btn_pid_speed_20
    scr_dashboard_btn_pid_speed_20 = lv_btn_create(scr_dashboard_cont_control_pid_speed);
    lv_obj_set_x(scr_dashboard_btn_pid_speed_20, 309);
    lv_obj_set_y(scr_dashboard_btn_pid_speed_20, 12);
    lv_obj_set_width(scr_dashboard_btn_pid_speed_20, 72);
    lv_obj_set_height(scr_dashboard_btn_pid_speed_20, 32);
    scr_dashboard_btn_pid_speed_20_label = lv_label_create(scr_dashboard_btn_pid_speed_20);
    lv_obj_set_scrollbar_mode(scr_dashboard_btn_pid_speed_20, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_btn_pid_speed_20_label, "20cm/s");
    lv_obj_set_style_pad_all(scr_dashboard_btn_pid_speed_20, 0, LV_STATE_DEFAULT);
    lv_obj_align(scr_dashboard_btn_pid_speed_20_label, LV_ALIGN_CENTER, 0, 0);
    // Add style for scr_dashboard_btn_pid_speed_20 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_btn_pid_speed_20, lv_color_hex(0x0B242B), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_btn_pid_speed_20, lv_color_hex(0x124040), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_btn_pid_speed_20, 120, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_btn_pid_speed_20, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_btn_pid_speed_20, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_btn_pid_speed_20, 80, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_btn_pid_speed_20, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_btn_pid_speed_20, 16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(scr_dashboard_btn_pid_speed_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(scr_dashboard_btn_pid_speed_20, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_btn_pid_speed_20 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_btn_pid_speed_20, lv_color_hex(0x0B242B), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_color(scr_dashboard_btn_pid_speed_20, lv_color_hex(0x124040), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_btn_pid_speed_20, 120, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_btn_pid_speed_20, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_btn_pid_speed_20, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_btn_pid_speed_20, 80, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_btn_pid_speed_20, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_btn_pid_speed_20, 16, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_color(scr_dashboard_btn_pid_speed_20, lv_color_hex(0x71CF97), LV_PART_MAIN | LV_STATE_CHECKED);
    // Add style for scr_dashboard_btn_pid_speed_20 - LV_PART_MAIN | LV_STATE_PRESSED
    lv_obj_set_style_bg_color(scr_dashboard_btn_pid_speed_20, lv_color_hex(0x0B242B), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_grad_color(scr_dashboard_btn_pid_speed_20, lv_color_hex(0x124040), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(scr_dashboard_btn_pid_speed_20, 120, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_btn_pid_speed_20, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_border_color(scr_dashboard_btn_pid_speed_20, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_border_opa(scr_dashboard_btn_pid_speed_20, 80, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_border_width(scr_dashboard_btn_pid_speed_20, 1, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_radius(scr_dashboard_btn_pid_speed_20, 16, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_text_color(scr_dashboard_btn_pid_speed_20, lv_color_hex(0x71CF97), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_text_font(scr_dashboard_btn_pid_speed_20, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_btn_pid_speed_20, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_btn_pid_speed_20, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_align(scr_dashboard_btn_pid_speed_20, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_font(scr_dashboard_btn_pid_speed_20, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_text_align(scr_dashboard_btn_pid_speed_20, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_PRESSED);
    ui_flag_modify(scr_dashboard_btn_pid_speed_20, LV_OBJ_FLAG_CHECKABLE, UI_FLAG_ACTION_ADD);
    // Create scr_dashboard_btn_pid_speed_30
    scr_dashboard_btn_pid_speed_30 = lv_btn_create(scr_dashboard_cont_control_pid_speed);
    lv_obj_set_x(scr_dashboard_btn_pid_speed_30, 393);
    lv_obj_set_y(scr_dashboard_btn_pid_speed_30, 12);
    lv_obj_set_width(scr_dashboard_btn_pid_speed_30, 72);
    lv_obj_set_height(scr_dashboard_btn_pid_speed_30, 32);
    scr_dashboard_btn_pid_speed_30_label = lv_label_create(scr_dashboard_btn_pid_speed_30);
    lv_obj_set_scrollbar_mode(scr_dashboard_btn_pid_speed_30, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_btn_pid_speed_30_label, "30cm/s");
    lv_obj_set_style_pad_all(scr_dashboard_btn_pid_speed_30, 0, LV_STATE_DEFAULT);
    lv_obj_align(scr_dashboard_btn_pid_speed_30_label, LV_ALIGN_CENTER, 0, 0);
    // Add style for scr_dashboard_btn_pid_speed_30 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_btn_pid_speed_30, lv_color_hex(0x0B242B), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_btn_pid_speed_30, lv_color_hex(0x124040), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_btn_pid_speed_30, 120, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_btn_pid_speed_30, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_btn_pid_speed_30, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_btn_pid_speed_30, 80, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_btn_pid_speed_30, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_btn_pid_speed_30, 16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(scr_dashboard_btn_pid_speed_30, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(scr_dashboard_btn_pid_speed_30, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_btn_pid_speed_30 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_btn_pid_speed_30, lv_color_hex(0x0B242B), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_color(scr_dashboard_btn_pid_speed_30, lv_color_hex(0x124040), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_btn_pid_speed_30, 120, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_btn_pid_speed_30, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_btn_pid_speed_30, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_btn_pid_speed_30, 80, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_btn_pid_speed_30, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_btn_pid_speed_30, 16, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_color(scr_dashboard_btn_pid_speed_30, lv_color_hex(0x71CF97), LV_PART_MAIN | LV_STATE_CHECKED);
    // Add style for scr_dashboard_btn_pid_speed_30 - LV_PART_MAIN | LV_STATE_PRESSED
    lv_obj_set_style_bg_color(scr_dashboard_btn_pid_speed_30, lv_color_hex(0x0B242B), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_grad_color(scr_dashboard_btn_pid_speed_30, lv_color_hex(0x124040), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(scr_dashboard_btn_pid_speed_30, 120, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_btn_pid_speed_30, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_border_color(scr_dashboard_btn_pid_speed_30, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_border_opa(scr_dashboard_btn_pid_speed_30, 80, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_border_width(scr_dashboard_btn_pid_speed_30, 1, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_radius(scr_dashboard_btn_pid_speed_30, 16, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_text_color(scr_dashboard_btn_pid_speed_30, lv_color_hex(0x71CF97), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_text_font(scr_dashboard_btn_pid_speed_30, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_btn_pid_speed_30, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_btn_pid_speed_30, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_align(scr_dashboard_btn_pid_speed_30, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_font(scr_dashboard_btn_pid_speed_30, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_text_align(scr_dashboard_btn_pid_speed_30, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_PRESSED);
    ui_flag_modify(scr_dashboard_btn_pid_speed_30, LV_OBJ_FLAG_CHECKABLE, UI_FLAG_ACTION_ADD);
    // Create scr_dashboard_btn_pid_speed_40
    scr_dashboard_btn_pid_speed_40 = lv_btn_create(scr_dashboard_cont_control_pid_speed);
    lv_obj_set_x(scr_dashboard_btn_pid_speed_40, 477);
    lv_obj_set_y(scr_dashboard_btn_pid_speed_40, 12);
    lv_obj_set_width(scr_dashboard_btn_pid_speed_40, 72);
    lv_obj_set_height(scr_dashboard_btn_pid_speed_40, 32);
    scr_dashboard_btn_pid_speed_40_label = lv_label_create(scr_dashboard_btn_pid_speed_40);
    lv_obj_set_scrollbar_mode(scr_dashboard_btn_pid_speed_40, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_btn_pid_speed_40_label, "40cm/s");
    lv_obj_set_style_pad_all(scr_dashboard_btn_pid_speed_40, 0, LV_STATE_DEFAULT);
    lv_obj_align(scr_dashboard_btn_pid_speed_40_label, LV_ALIGN_CENTER, 0, 0);
    // Add style for scr_dashboard_btn_pid_speed_40 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_btn_pid_speed_40, lv_color_hex(0x0B242B), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_btn_pid_speed_40, lv_color_hex(0x124040), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_btn_pid_speed_40, 120, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_btn_pid_speed_40, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_btn_pid_speed_40, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_btn_pid_speed_40, 80, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_btn_pid_speed_40, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_btn_pid_speed_40, 16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(scr_dashboard_btn_pid_speed_40, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(scr_dashboard_btn_pid_speed_40, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_btn_pid_speed_40 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_btn_pid_speed_40, lv_color_hex(0x0B242B), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_color(scr_dashboard_btn_pid_speed_40, lv_color_hex(0x124040), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_btn_pid_speed_40, 120, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_btn_pid_speed_40, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_btn_pid_speed_40, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_btn_pid_speed_40, 80, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_btn_pid_speed_40, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_btn_pid_speed_40, 16, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_color(scr_dashboard_btn_pid_speed_40, lv_color_hex(0x71CF97), LV_PART_MAIN | LV_STATE_CHECKED);
    // Add style for scr_dashboard_btn_pid_speed_40 - LV_PART_MAIN | LV_STATE_PRESSED
    lv_obj_set_style_bg_color(scr_dashboard_btn_pid_speed_40, lv_color_hex(0x0B242B), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_grad_color(scr_dashboard_btn_pid_speed_40, lv_color_hex(0x124040), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(scr_dashboard_btn_pid_speed_40, 120, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_btn_pid_speed_40, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_border_color(scr_dashboard_btn_pid_speed_40, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_border_opa(scr_dashboard_btn_pid_speed_40, 80, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_border_width(scr_dashboard_btn_pid_speed_40, 1, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_radius(scr_dashboard_btn_pid_speed_40, 16, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_text_color(scr_dashboard_btn_pid_speed_40, lv_color_hex(0x71CF97), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_text_font(scr_dashboard_btn_pid_speed_40, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_btn_pid_speed_40, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_btn_pid_speed_40, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_align(scr_dashboard_btn_pid_speed_40, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_font(scr_dashboard_btn_pid_speed_40, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_text_align(scr_dashboard_btn_pid_speed_40, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_PRESSED);
    ui_flag_modify(scr_dashboard_btn_pid_speed_40, LV_OBJ_FLAG_CHECKABLE, UI_FLAG_ACTION_ADD);
    // Create scr_dashboard_btn_pid_speed_50
    scr_dashboard_btn_pid_speed_50 = lv_btn_create(scr_dashboard_cont_control_pid_speed);
    lv_obj_set_x(scr_dashboard_btn_pid_speed_50, 561);
    lv_obj_set_y(scr_dashboard_btn_pid_speed_50, 12);
    lv_obj_set_width(scr_dashboard_btn_pid_speed_50, 72);
    lv_obj_set_height(scr_dashboard_btn_pid_speed_50, 32);
    scr_dashboard_btn_pid_speed_50_label = lv_label_create(scr_dashboard_btn_pid_speed_50);
    lv_obj_set_scrollbar_mode(scr_dashboard_btn_pid_speed_50, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_btn_pid_speed_50_label, "50cm/s");
    lv_obj_set_style_pad_all(scr_dashboard_btn_pid_speed_50, 0, LV_STATE_DEFAULT);
    lv_obj_align(scr_dashboard_btn_pid_speed_50_label, LV_ALIGN_CENTER, 0, 0);
    // Add style for scr_dashboard_btn_pid_speed_50 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_btn_pid_speed_50, lv_color_hex(0x0B242B), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_btn_pid_speed_50, lv_color_hex(0x124040), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_btn_pid_speed_50, 120, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_btn_pid_speed_50, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_btn_pid_speed_50, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_btn_pid_speed_50, 80, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_btn_pid_speed_50, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_btn_pid_speed_50, 16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(scr_dashboard_btn_pid_speed_50, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(scr_dashboard_btn_pid_speed_50, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_btn_pid_speed_50 - LV_PART_MAIN | LV_STATE_CHECKED
    lv_obj_set_style_bg_color(scr_dashboard_btn_pid_speed_50, lv_color_hex(0x0B242B), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_color(scr_dashboard_btn_pid_speed_50, lv_color_hex(0x124040), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(scr_dashboard_btn_pid_speed_50, 120, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_btn_pid_speed_50, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(scr_dashboard_btn_pid_speed_50, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(scr_dashboard_btn_pid_speed_50, 80, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(scr_dashboard_btn_pid_speed_50, 1, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(scr_dashboard_btn_pid_speed_50, 16, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_color(scr_dashboard_btn_pid_speed_50, lv_color_hex(0x71CF97), LV_PART_MAIN | LV_STATE_CHECKED);
    // Add style for scr_dashboard_btn_pid_speed_50 - LV_PART_MAIN | LV_STATE_PRESSED
    lv_obj_set_style_bg_color(scr_dashboard_btn_pid_speed_50, lv_color_hex(0x0B242B), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_grad_color(scr_dashboard_btn_pid_speed_50, lv_color_hex(0x124040), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(scr_dashboard_btn_pid_speed_50, 120, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_btn_pid_speed_50, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_border_color(scr_dashboard_btn_pid_speed_50, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_border_opa(scr_dashboard_btn_pid_speed_50, 80, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_border_width(scr_dashboard_btn_pid_speed_50, 1, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_radius(scr_dashboard_btn_pid_speed_50, 16, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_text_color(scr_dashboard_btn_pid_speed_50, lv_color_hex(0x71CF97), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_text_font(scr_dashboard_btn_pid_speed_50, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_btn_pid_speed_50, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_btn_pid_speed_50, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_align(scr_dashboard_btn_pid_speed_50, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_font(scr_dashboard_btn_pid_speed_50, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_text_align(scr_dashboard_btn_pid_speed_50, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_PRESSED);
    ui_flag_modify(scr_dashboard_btn_pid_speed_50, LV_OBJ_FLAG_CHECKABLE, UI_FLAG_ACTION_ADD);
    // Create scr_dashboard_cont_detect
    scr_dashboard_cont_detect = lv_obj_create(scr_dashboard_cont_set);
    lv_obj_set_x(scr_dashboard_cont_detect, 258);
    lv_obj_set_y(scr_dashboard_cont_detect, 10);
    lv_obj_set_width(scr_dashboard_cont_detect, 700);
    lv_obj_set_height(scr_dashboard_cont_detect, 375);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_detect, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_detect, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_detect - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_detect, lv_color_hex(0x0B1517), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detect, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_detect, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detect, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detect, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detect, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detect, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_detect, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detect, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detect, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detect, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detect, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_cont_detect_confidence
    scr_dashboard_cont_detect_confidence = lv_obj_create(scr_dashboard_cont_detect);
    lv_obj_set_x(scr_dashboard_cont_detect_confidence, 16);
    lv_obj_set_y(scr_dashboard_cont_detect_confidence, 0);
    lv_obj_set_width(scr_dashboard_cont_detect_confidence, 669);
    lv_obj_set_height(scr_dashboard_cont_detect_confidence, 130);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_detect_confidence, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_detect_confidence, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_detect_confidence - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_detect_confidence, lv_color_hex(0x2D4145), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_detect_confidence, lv_color_hex(0x314144), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detect_confidence, 44, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_detect_confidence, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_detect_confidence, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_detect_confidence, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_detect_confidence, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_detect_confidence, 14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detect_confidence, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detect_confidence, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detect_confidence, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detect_confidence, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_detect_confidence, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detect_confidence, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detect_confidence, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detect_confidence, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detect_confidence, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_slider_confidence_threshold_mian
    scr_dashboard_slider_confidence_threshold_mian = lv_slider_create(scr_dashboard_cont_detect_confidence);
    lv_obj_set_x(scr_dashboard_slider_confidence_threshold_mian, 121);
    lv_obj_set_y(scr_dashboard_slider_confidence_threshold_mian, 60);
    lv_obj_set_width(scr_dashboard_slider_confidence_threshold_mian, 480);
    lv_obj_set_height(scr_dashboard_slider_confidence_threshold_mian, 8);
    lv_obj_set_scrollbar_mode(scr_dashboard_slider_confidence_threshold_mian, LV_SCROLLBAR_MODE_OFF);
    lv_slider_set_range(scr_dashboard_slider_confidence_threshold_mian, 0, 100);
    lv_slider_set_value(scr_dashboard_slider_confidence_threshold_mian, 0, LV_ANIM_OFF);
    // Add style for scr_dashboard_slider_confidence_threshold_mian - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_slider_confidence_threshold_mian, lv_color_hex(0x05161A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_slider_confidence_threshold_mian, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_slider_confidence_threshold_mian - LV_PART_INDICATOR | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_slider_confidence_threshold_mian, lv_color_hex(0x217A96), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_slider_confidence_threshold_mian, lv_color_hex(0x84D5E0), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_slider_confidence_threshold_mian, 200, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_slider_confidence_threshold_mian, LV_GRAD_DIR_NONE, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_slider_confidence_threshold_mian - LV_PART_KNOB | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_slider_confidence_threshold_mian, lv_color_hex(0xBDD3D6), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_slider_confidence_threshold_mian, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_confidence_value_mian
    scr_dashboard_label_confidence_value_mian = lv_label_create(scr_dashboard_cont_detect_confidence);
    lv_obj_set_x(scr_dashboard_label_confidence_value_mian, 618);
    lv_obj_set_y(scr_dashboard_label_confidence_value_mian, 57);
    lv_obj_set_width(scr_dashboard_label_confidence_value_mian, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_confidence_value_mian, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_confidence_value_mian, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_confidence_value_mian, "00%");
    lv_label_set_long_mode(scr_dashboard_label_confidence_value_mian, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_confidence_value_mian - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_confidence_value_mian, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_confidence_value_mian, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_confidence_value_mian, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_confidence_value_mian, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_slider_confidence_threshold_logo
    scr_dashboard_slider_confidence_threshold_logo = lv_slider_create(scr_dashboard_cont_detect_confidence);
    lv_obj_set_x(scr_dashboard_slider_confidence_threshold_logo, 121);
    lv_obj_set_y(scr_dashboard_slider_confidence_threshold_logo, 96);
    lv_obj_set_width(scr_dashboard_slider_confidence_threshold_logo, 480);
    lv_obj_set_height(scr_dashboard_slider_confidence_threshold_logo, 8);
    lv_obj_set_scrollbar_mode(scr_dashboard_slider_confidence_threshold_logo, LV_SCROLLBAR_MODE_OFF);
    lv_slider_set_range(scr_dashboard_slider_confidence_threshold_logo, 0, 100);
    lv_slider_set_value(scr_dashboard_slider_confidence_threshold_logo, 0, LV_ANIM_OFF);
    // Add style for scr_dashboard_slider_confidence_threshold_logo - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_slider_confidence_threshold_logo, lv_color_hex(0x05161A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_slider_confidence_threshold_logo, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_slider_confidence_threshold_logo - LV_PART_INDICATOR | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_slider_confidence_threshold_logo, lv_color_hex(0x217A96), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_slider_confidence_threshold_logo, lv_color_hex(0x84D5E0), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_slider_confidence_threshold_logo, 200, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_slider_confidence_threshold_logo, LV_GRAD_DIR_NONE, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_slider_confidence_threshold_logo - LV_PART_KNOB | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_slider_confidence_threshold_logo, lv_color_hex(0xBDD3D6), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_slider_confidence_threshold_logo, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_confidence_value_logo
    scr_dashboard_label_confidence_value_logo = lv_label_create(scr_dashboard_cont_detect_confidence);
    lv_obj_set_x(scr_dashboard_label_confidence_value_logo, 618);
    lv_obj_set_y(scr_dashboard_label_confidence_value_logo, 93);
    lv_obj_set_width(scr_dashboard_label_confidence_value_logo, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_confidence_value_logo, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_confidence_value_logo, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_confidence_value_logo, "00%");
    lv_label_set_long_mode(scr_dashboard_label_confidence_value_logo, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_confidence_value_logo - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_confidence_value_logo, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_confidence_value_logo, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_confidence_value_logo, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_confidence_value_logo, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_confidence_threshold
    scr_dashboard_label_confidence_threshold = lv_label_create(scr_dashboard_cont_detect_confidence);
    lv_obj_set_x(scr_dashboard_label_confidence_threshold, 25);
    lv_obj_set_y(scr_dashboard_label_confidence_threshold, 19);
    lv_obj_set_width(scr_dashboard_label_confidence_threshold, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_confidence_threshold, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_confidence_threshold, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_confidence_threshold, "置信度阈值");
    lv_label_set_long_mode(scr_dashboard_label_confidence_threshold, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_confidence_threshold - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_confidence_threshold, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_confidence_threshold, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_confidence_threshold, &lv_font_Misans_Heavy_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_confidence_threshold, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_text_mian
    scr_dashboard_label_text_mian = lv_label_create(scr_dashboard_cont_detect_confidence);
    lv_obj_set_x(scr_dashboard_label_text_mian, 46);
    lv_obj_set_y(scr_dashboard_label_text_mian, 54);
    lv_obj_set_width(scr_dashboard_label_text_mian, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_text_mian, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_text_mian, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_text_mian, "面单");
    lv_label_set_long_mode(scr_dashboard_label_text_mian, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_text_mian - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_text_mian, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_text_mian, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_text_mian, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_text_logo
    scr_dashboard_label_text_logo = lv_label_create(scr_dashboard_cont_detect_confidence);
    lv_obj_set_x(scr_dashboard_label_text_logo, 46);
    lv_obj_set_y(scr_dashboard_label_text_logo, 93);
    lv_obj_set_width(scr_dashboard_label_text_logo, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_text_logo, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_text_logo, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_text_logo, "Logo");
    lv_label_set_long_mode(scr_dashboard_label_text_logo, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_text_logo - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_text_logo, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_text_logo, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_text_logo, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_detect_switch
    scr_dashboard_cont_detect_switch = lv_obj_create(scr_dashboard_cont_detect);
    lv_obj_set_x(scr_dashboard_cont_detect_switch, 16);
    lv_obj_set_y(scr_dashboard_cont_detect_switch, 142);
    lv_obj_set_width(scr_dashboard_cont_detect_switch, 669);
    lv_obj_set_height(scr_dashboard_cont_detect_switch, 56);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_detect_switch, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_detect_switch, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_detect_switch - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_detect_switch, lv_color_hex(0x2D4145), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_detect_switch, lv_color_hex(0x314144), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detect_switch, 44, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_detect_switch, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_detect_switch, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_detect_switch, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_detect_switch, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_detect_switch, 14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detect_switch, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detect_switch, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detect_switch, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detect_switch, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_detect_switch, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detect_switch, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detect_switch, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detect_switch, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detect_switch, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_detect_switch
    scr_dashboard_label_detect_switch = lv_label_create(scr_dashboard_cont_detect_switch);
    lv_obj_set_x(scr_dashboard_label_detect_switch, 25);
    lv_obj_set_y(scr_dashboard_label_detect_switch, 21);
    lv_obj_set_width(scr_dashboard_label_detect_switch, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_detect_switch, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_detect_switch, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_detect_switch, "检测");
    lv_label_set_long_mode(scr_dashboard_label_detect_switch, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_detect_switch - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_detect_switch, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_detect_switch, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_detect_switch, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_detect_switch, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_sw_detect
    scr_dashboard_sw_detect = lv_switch_create(scr_dashboard_cont_detect_switch);
    lv_obj_set_x(scr_dashboard_sw_detect, 579);
    lv_obj_set_y(scr_dashboard_sw_detect, 18);
    lv_obj_set_width(scr_dashboard_sw_detect, 40);
    lv_obj_set_height(scr_dashboard_sw_detect, 20);
    lv_obj_set_scrollbar_mode(scr_dashboard_sw_detect, LV_SCROLLBAR_MODE_OFF);
    // Create scr_dashboard_cont_detect_overlay
    scr_dashboard_cont_detect_overlay = lv_obj_create(scr_dashboard_cont_detect);
    lv_obj_set_x(scr_dashboard_cont_detect_overlay, 16);
    lv_obj_set_y(scr_dashboard_cont_detect_overlay, 210);
    lv_obj_set_width(scr_dashboard_cont_detect_overlay, 669);
    lv_obj_set_height(scr_dashboard_cont_detect_overlay, 56);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_detect_overlay, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_detect_overlay, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_detect_overlay - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_detect_overlay, lv_color_hex(0x2D4145), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_detect_overlay, lv_color_hex(0x314144), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detect_overlay, 44, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_detect_overlay, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_detect_overlay, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_detect_overlay, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_detect_overlay, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_detect_overlay, 14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detect_overlay, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detect_overlay, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detect_overlay, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detect_overlay, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_detect_overlay, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detect_overlay, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detect_overlay, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detect_overlay, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detect_overlay, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_preview_overlay
    scr_dashboard_label_preview_overlay = lv_label_create(scr_dashboard_cont_detect_overlay);
    lv_obj_set_x(scr_dashboard_label_preview_overlay, 25);
    lv_obj_set_y(scr_dashboard_label_preview_overlay, 21);
    lv_obj_set_width(scr_dashboard_label_preview_overlay, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_preview_overlay, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_preview_overlay, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_preview_overlay, "预览叠加框");
    lv_label_set_long_mode(scr_dashboard_label_preview_overlay, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_preview_overlay - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_preview_overlay, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_preview_overlay, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_preview_overlay, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_preview_overlay, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_sw_preview_overlay
    scr_dashboard_sw_preview_overlay = lv_switch_create(scr_dashboard_cont_detect_overlay);
    lv_obj_set_x(scr_dashboard_sw_preview_overlay, 579);
    lv_obj_set_y(scr_dashboard_sw_preview_overlay, 18);
    lv_obj_set_width(scr_dashboard_sw_preview_overlay, 40);
    lv_obj_set_height(scr_dashboard_sw_preview_overlay, 20);
    lv_obj_set_scrollbar_mode(scr_dashboard_sw_preview_overlay, LV_SCROLLBAR_MODE_OFF);
    // Create scr_dashboard_cont_button
    scr_dashboard_cont_button = lv_obj_create(scr_dashboard_cont_background2);
    lv_obj_set_x(scr_dashboard_cont_button, -1);
    lv_obj_set_y(scr_dashboard_cont_button, 523);
    lv_obj_set_width(scr_dashboard_cont_button, 1018);
    lv_obj_set_height(scr_dashboard_cont_button, 70);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_button, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_button, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_button - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_button, lv_color_hex(0x1E2426), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_button, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_button, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_button, 60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_button, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_button, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_button, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_button, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_button, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_button, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_imgbtn_dash
    scr_dashboard_imgbtn_dash = lv_imgbtn_create(scr_dashboard_cont_button);
    lv_obj_set_x(scr_dashboard_imgbtn_dash, 38);
    lv_obj_set_y(scr_dashboard_imgbtn_dash, 1);
    lv_obj_set_width(scr_dashboard_imgbtn_dash, 212);
    lv_obj_set_height(scr_dashboard_imgbtn_dash, 66);
    lv_obj_set_scrollbar_mode(scr_dashboard_imgbtn_dash, LV_SCROLLBAR_MODE_OFF);
    scr_dashboard_imgbtn_dash_label = lv_label_create(scr_dashboard_imgbtn_dash);
    lv_label_set_text(scr_dashboard_imgbtn_dash_label, "");
    lv_label_set_long_mode(scr_dashboard_imgbtn_dash_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(scr_dashboard_imgbtn_dash_label, LV_PCT(100));
    lv_obj_set_align(scr_dashboard_imgbtn_dash_label, LV_ALIGN_CENTER);
    lv_imgbtn_set_src(scr_dashboard_imgbtn_dash, LV_IMGBTN_STATE_RELEASED, NULL, &dash_c_212x66, NULL);
    lv_imgbtn_set_src(scr_dashboard_imgbtn_dash, LV_IMGBTN_STATE_PRESSED, NULL, &dash_c2_212x66, NULL);
    lv_imgbtn_set_src(scr_dashboard_imgbtn_dash, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &dash_a_212x66, NULL);
    lv_obj_add_flag(scr_dashboard_imgbtn_dash, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_set_style_text_align(scr_dashboard_imgbtn_dash, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_imgbtn_dash, LV_OBJ_FLAG_CHECKABLE, UI_FLAG_ACTION_ADD);
    ui_state_modify(scr_dashboard_imgbtn_dash, LV_STATE_CHECKED, UI_STATE_ACTION_ADD);
    // Create scr_dashboard_imgbtn_set
    scr_dashboard_imgbtn_set = lv_imgbtn_create(scr_dashboard_cont_button);
    lv_obj_set_x(scr_dashboard_imgbtn_set, 281);
    lv_obj_set_y(scr_dashboard_imgbtn_set, 1);
    lv_obj_set_width(scr_dashboard_imgbtn_set, 212);
    lv_obj_set_height(scr_dashboard_imgbtn_set, 66);
    lv_obj_set_scrollbar_mode(scr_dashboard_imgbtn_set, LV_SCROLLBAR_MODE_OFF);
    scr_dashboard_imgbtn_set_label = lv_label_create(scr_dashboard_imgbtn_set);
    lv_label_set_text(scr_dashboard_imgbtn_set_label, "");
    lv_label_set_long_mode(scr_dashboard_imgbtn_set_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(scr_dashboard_imgbtn_set_label, LV_PCT(100));
    lv_obj_set_align(scr_dashboard_imgbtn_set_label, LV_ALIGN_CENTER);
    lv_imgbtn_set_src(scr_dashboard_imgbtn_set, LV_IMGBTN_STATE_RELEASED, NULL, &setc_212x66, NULL);
    lv_imgbtn_set_src(scr_dashboard_imgbtn_set, LV_IMGBTN_STATE_PRESSED, NULL, &setc1_212x66, NULL);
    lv_obj_add_flag(scr_dashboard_imgbtn_set, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_set_style_text_align(scr_dashboard_imgbtn_set, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_imgbtn_set, LV_OBJ_FLAG_CHECKABLE, UI_FLAG_ACTION_ADD);
    // Create scr_dashboard_imgbtn_log
    scr_dashboard_imgbtn_log = lv_imgbtn_create(scr_dashboard_cont_button);
    lv_obj_set_x(scr_dashboard_imgbtn_log, 524);
    lv_obj_set_y(scr_dashboard_imgbtn_log, 1);
    lv_obj_set_width(scr_dashboard_imgbtn_log, 212);
    lv_obj_set_height(scr_dashboard_imgbtn_log, 66);
    lv_obj_set_scrollbar_mode(scr_dashboard_imgbtn_log, LV_SCROLLBAR_MODE_OFF);
    scr_dashboard_imgbtn_log_label = lv_label_create(scr_dashboard_imgbtn_log);
    lv_label_set_text(scr_dashboard_imgbtn_log_label, "");
    lv_label_set_long_mode(scr_dashboard_imgbtn_log_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(scr_dashboard_imgbtn_log_label, LV_PCT(100));
    lv_obj_set_align(scr_dashboard_imgbtn_log_label, LV_ALIGN_CENTER);
    lv_imgbtn_set_src(scr_dashboard_imgbtn_log, LV_IMGBTN_STATE_RELEASED, NULL, &logc_212x66, NULL);
    lv_imgbtn_set_src(scr_dashboard_imgbtn_log, LV_IMGBTN_STATE_PRESSED, NULL, &logc1_212x66, NULL);
    lv_obj_add_flag(scr_dashboard_imgbtn_log, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_set_style_text_align(scr_dashboard_imgbtn_log, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_imgbtn_log, LV_OBJ_FLAG_CHECKABLE, UI_FLAG_ACTION_ADD);
    // Create scr_dashboard_imgbtn_sys
    scr_dashboard_imgbtn_sys = lv_imgbtn_create(scr_dashboard_cont_button);
    lv_obj_set_x(scr_dashboard_imgbtn_sys, 767);
    lv_obj_set_y(scr_dashboard_imgbtn_sys, 1);
    lv_obj_set_width(scr_dashboard_imgbtn_sys, 212);
    lv_obj_set_height(scr_dashboard_imgbtn_sys, 66);
    lv_obj_set_scrollbar_mode(scr_dashboard_imgbtn_sys, LV_SCROLLBAR_MODE_OFF);
    scr_dashboard_imgbtn_sys_label = lv_label_create(scr_dashboard_imgbtn_sys);
    lv_label_set_text(scr_dashboard_imgbtn_sys_label, "");
    lv_label_set_long_mode(scr_dashboard_imgbtn_sys_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(scr_dashboard_imgbtn_sys_label, LV_PCT(100));
    lv_obj_set_align(scr_dashboard_imgbtn_sys_label, LV_ALIGN_CENTER);
    lv_imgbtn_set_src(scr_dashboard_imgbtn_sys, LV_IMGBTN_STATE_RELEASED, NULL, &sysc_212x66, NULL);
    lv_imgbtn_set_src(scr_dashboard_imgbtn_sys, LV_IMGBTN_STATE_PRESSED, NULL, &sysc1_212x66, NULL);
    lv_obj_add_flag(scr_dashboard_imgbtn_sys, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_set_style_text_align(scr_dashboard_imgbtn_sys, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_imgbtn_sys, LV_OBJ_FLAG_CHECKABLE, UI_FLAG_ACTION_ADD);
    // Create scr_dashboard_cont_dashboard
    scr_dashboard_cont_dashboard = lv_obj_create(scr_dashboard_cont_background2);
    lv_obj_set_x(scr_dashboard_cont_dashboard, 6);
    lv_obj_set_y(scr_dashboard_cont_dashboard, 69);
    lv_obj_set_width(scr_dashboard_cont_dashboard, 1000);
    lv_obj_set_height(scr_dashboard_cont_dashboard, 444);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_dashboard, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_dashboard, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_dashboard - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_dashboard, lv_color_hex(0x0B1517), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_dashboard, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_dashboard, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_dashboard, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_dashboard, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_dashboard, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_dashboard, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_dashboard, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_dashboard, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_dashboard, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_dashboard, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_dashboard, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_cont_container_live
    scr_dashboard_cont_container_live = lv_obj_create(scr_dashboard_cont_dashboard);
    lv_obj_set_x(scr_dashboard_cont_container_live, 4);
    lv_obj_set_y(scr_dashboard_cont_container_live, 5);
    lv_obj_set_width(scr_dashboard_cont_container_live, 640);
    lv_obj_set_height(scr_dashboard_cont_container_live, 410);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_container_live, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_container_live, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_container_live - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_container_live, lv_color_hex(0x232C2C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_container_live, lv_color_hex(0x070A0A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_container_live, 220, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_container_live, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_container_live, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_container_live, 60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_container_live, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_container_live, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_container_live, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_container_live, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_container_live, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_container_live, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_container_live, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_container_live, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_container_live, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_container_live, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_container_live, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_cont_live_vedio
    scr_dashboard_cont_live_vedio = lv_obj_create(scr_dashboard_cont_container_live);
    lv_obj_set_x(scr_dashboard_cont_live_vedio, -1);
    lv_obj_set_y(scr_dashboard_cont_live_vedio, 34);
    lv_obj_set_width(scr_dashboard_cont_live_vedio, 640);
    lv_obj_set_height(scr_dashboard_cont_live_vedio, 375);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_live_vedio, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_live_vedio, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_live_vedio - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_live_vedio, lv_color_hex(0x0B1517), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_live_vedio, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_live_vedio, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_live_vedio, 80, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_live_vedio, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_live_vedio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_live_vedio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_live_vedio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_live_vedio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_live_vedio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_live_vedio, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_live_vedio, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_live_vedio, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_live_vedio, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_live_vedio, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_text_live
    scr_dashboard_label_text_live = lv_label_create(scr_dashboard_cont_container_live);
    lv_obj_set_x(scr_dashboard_label_text_live, 13);
    lv_obj_set_y(scr_dashboard_label_text_live, 9);
    lv_obj_set_width(scr_dashboard_label_text_live, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_text_live, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_text_live, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_text_live, "实时监测");
    lv_label_set_long_mode(scr_dashboard_label_text_live, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_text_live - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_text_live, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_text_live, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(scr_dashboard_label_text_live, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(scr_dashboard_label_text_live, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_text_live, &lv_font_Misans_Heavy_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_text_live, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_detection
    scr_dashboard_cont_detection = lv_obj_create(scr_dashboard_cont_dashboard);
    lv_obj_set_x(scr_dashboard_cont_detection, 652);
    lv_obj_set_y(scr_dashboard_cont_detection, 5);
    lv_obj_set_width(scr_dashboard_cont_detection, 340);
    lv_obj_set_height(scr_dashboard_cont_detection, 108);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_detection, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_detection, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_detection - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_detection, lv_color_hex(0x3F5250), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_detection, lv_color_hex(0x1E2426), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_detection, 108, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_detection, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_detection, lv_color_hex(0x75B8A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_detection, 60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_detection, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_detection, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_detection, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_detection, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_detection, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_detection, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_detection, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detection, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detection, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detection, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_detection, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_slider_runtime_confidence_mian
    scr_dashboard_slider_runtime_confidence_mian = lv_slider_create(scr_dashboard_cont_detection);
    lv_obj_set_x(scr_dashboard_slider_runtime_confidence_mian, 127);
    lv_obj_set_y(scr_dashboard_slider_runtime_confidence_mian, 71);
    lv_obj_set_width(scr_dashboard_slider_runtime_confidence_mian, 200);
    lv_obj_set_height(scr_dashboard_slider_runtime_confidence_mian, 6);
    lv_obj_set_scrollbar_mode(scr_dashboard_slider_runtime_confidence_mian, LV_SCROLLBAR_MODE_OFF);
    lv_slider_set_range(scr_dashboard_slider_runtime_confidence_mian, 0, 100);
    lv_slider_set_value(scr_dashboard_slider_runtime_confidence_mian, 0, LV_ANIM_OFF);
    // Add style for scr_dashboard_slider_runtime_confidence_mian - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_slider_runtime_confidence_mian, lv_color_hex(0x0B0E0E), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_slider_runtime_confidence_mian, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_slider_runtime_confidence_mian - LV_PART_INDICATOR | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_slider_runtime_confidence_mian, lv_color_hex(0x70D296), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_slider_runtime_confidence_mian, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_slider_runtime_confidence_mian - LV_PART_KNOB | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_slider_runtime_confidence_mian, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_slider_runtime_confidence_mian, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_slider_runtime_confidence_mian, LV_OBJ_FLAG_CLICKABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_slider_runtime_confidence_mian, LV_OBJ_FLAG_SNAPPABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_slider_runtime_confidence_mian, LV_OBJ_FLAG_CLICK_FOCUSABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_slider_runtime_confidence_mian, LV_OBJ_FLAG_GESTURE_BUBBLE, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_slider_runtime_confidence_logo
    scr_dashboard_slider_runtime_confidence_logo = lv_slider_create(scr_dashboard_cont_detection);
    lv_obj_set_x(scr_dashboard_slider_runtime_confidence_logo, 127);
    lv_obj_set_y(scr_dashboard_slider_runtime_confidence_logo, 91);
    lv_obj_set_width(scr_dashboard_slider_runtime_confidence_logo, 200);
    lv_obj_set_height(scr_dashboard_slider_runtime_confidence_logo, 6);
    lv_obj_set_scrollbar_mode(scr_dashboard_slider_runtime_confidence_logo, LV_SCROLLBAR_MODE_OFF);
    lv_slider_set_range(scr_dashboard_slider_runtime_confidence_logo, 0, 100);
    lv_slider_set_value(scr_dashboard_slider_runtime_confidence_logo, 0, LV_ANIM_OFF);
    // Add style for scr_dashboard_slider_runtime_confidence_logo - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_slider_runtime_confidence_logo, lv_color_hex(0x0B0E0E), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_slider_runtime_confidence_logo, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_slider_runtime_confidence_logo - LV_PART_INDICATOR | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_slider_runtime_confidence_logo, lv_color_hex(0x70D296), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_slider_runtime_confidence_logo, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_slider_runtime_confidence_logo - LV_PART_KNOB | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_slider_runtime_confidence_logo, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_slider_runtime_confidence_logo, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_slider_runtime_confidence_logo, LV_OBJ_FLAG_CLICKABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_slider_runtime_confidence_logo, LV_OBJ_FLAG_SNAPPABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_slider_runtime_confidence_logo, LV_OBJ_FLAG_CLICK_FOCUSABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_slider_runtime_confidence_logo, LV_OBJ_FLAG_GESTURE_BUBBLE, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_runtime_confidence_mian
    scr_dashboard_label_runtime_confidence_mian = lv_label_create(scr_dashboard_cont_detection);
    lv_obj_set_x(scr_dashboard_label_runtime_confidence_mian, 93);
    lv_obj_set_y(scr_dashboard_label_runtime_confidence_mian, 64);
    lv_obj_set_width(scr_dashboard_label_runtime_confidence_mian, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_runtime_confidence_mian, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_runtime_confidence_mian, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_runtime_confidence_mian, "--");
    lv_label_set_long_mode(scr_dashboard_label_runtime_confidence_mian, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_runtime_confidence_mian - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_runtime_confidence_mian, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(scr_dashboard_label_runtime_confidence_mian, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(scr_dashboard_label_runtime_confidence_mian, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_runtime_confidence_mian, &lv_font_Misans_Heavy_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_runtime_confidence_mian, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_runtime_confidence_logo
    scr_dashboard_label_runtime_confidence_logo = lv_label_create(scr_dashboard_cont_detection);
    lv_obj_set_x(scr_dashboard_label_runtime_confidence_logo, 93);
    lv_obj_set_y(scr_dashboard_label_runtime_confidence_logo, 84);
    lv_obj_set_width(scr_dashboard_label_runtime_confidence_logo, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_runtime_confidence_logo, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_runtime_confidence_logo, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_runtime_confidence_logo, "--");
    lv_label_set_long_mode(scr_dashboard_label_runtime_confidence_logo, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_runtime_confidence_logo - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_runtime_confidence_logo, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(scr_dashboard_label_runtime_confidence_logo, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(scr_dashboard_label_runtime_confidence_logo, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_runtime_confidence_logo, &lv_font_Misans_Heavy_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_runtime_confidence_logo, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_runtime_recognition_fps
    scr_dashboard_label_runtime_recognition_fps = lv_label_create(scr_dashboard_cont_detection);
    lv_obj_set_x(scr_dashboard_label_runtime_recognition_fps, 207);
    lv_obj_set_y(scr_dashboard_label_runtime_recognition_fps, 13);
    lv_obj_set_width(scr_dashboard_label_runtime_recognition_fps, 100);
    lv_obj_set_height(scr_dashboard_label_runtime_recognition_fps, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_runtime_recognition_fps, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_runtime_recognition_fps, "--");
    lv_label_set_long_mode(scr_dashboard_label_runtime_recognition_fps, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_runtime_recognition_fps - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_runtime_recognition_fps, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_runtime_recognition_fps, &lv_font_MiSans_Heavy_16_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_runtime_recognition_fps, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_runtime_recognition_status
    scr_dashboard_label_runtime_recognition_status = lv_label_create(scr_dashboard_cont_detection);
    lv_obj_set_x(scr_dashboard_label_runtime_recognition_status, 76);
    lv_obj_set_y(scr_dashboard_label_runtime_recognition_status, 29);
    lv_obj_set_width(scr_dashboard_label_runtime_recognition_status, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_runtime_recognition_status, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_runtime_recognition_status, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_runtime_recognition_status, "等待中");
    lv_label_set_long_mode(scr_dashboard_label_runtime_recognition_status, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_runtime_recognition_status - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_runtime_recognition_status, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(scr_dashboard_label_runtime_recognition_status, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(scr_dashboard_label_runtime_recognition_status, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_runtime_recognition_status, &lv_font_MiSansVF_24_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_runtime_recognition_status, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_text_mian_confidence
    scr_dashboard_label_text_mian_confidence = lv_label_create(scr_dashboard_cont_detection);
    lv_obj_set_x(scr_dashboard_label_text_mian_confidence, 10);
    lv_obj_set_y(scr_dashboard_label_text_mian_confidence, 64);
    lv_obj_set_width(scr_dashboard_label_text_mian_confidence, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_text_mian_confidence, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_text_mian_confidence, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_text_mian_confidence, "面单置信度:");
    lv_label_set_long_mode(scr_dashboard_label_text_mian_confidence, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_text_mian_confidence - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_text_mian_confidence, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_text_mian_confidence, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_text_mian_confidence, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_text_logo_confidence
    scr_dashboard_label_text_logo_confidence = lv_label_create(scr_dashboard_cont_detection);
    lv_obj_set_x(scr_dashboard_label_text_logo_confidence, 10);
    lv_obj_set_y(scr_dashboard_label_text_logo_confidence, 84);
    lv_obj_set_width(scr_dashboard_label_text_logo_confidence, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_text_logo_confidence, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_text_logo_confidence, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_text_logo_confidence, "Logo置信度:");
    lv_label_set_long_mode(scr_dashboard_label_text_logo_confidence, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_text_logo_confidence - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_text_logo_confidence, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_text_logo_confidence, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_text_logo_confidence, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_text_detection
    scr_dashboard_label_text_detection = lv_label_create(scr_dashboard_cont_detection);
    lv_obj_set_x(scr_dashboard_label_text_detection, 12);
    lv_obj_set_y(scr_dashboard_label_text_detection, 9);
    lv_obj_set_width(scr_dashboard_label_text_detection, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_text_detection, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_text_detection, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_text_detection, "检测结果");
    lv_label_set_long_mode(scr_dashboard_label_text_detection, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_text_detection - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_text_detection, lv_color_hex(0x8EA5AD), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_text_detection, &lv_font_Misans_Heavy_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_text_detection, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_text_fps
    scr_dashboard_label_text_fps = lv_label_create(scr_dashboard_cont_detection);
    lv_obj_set_x(scr_dashboard_label_text_fps, 285);
    lv_obj_set_y(scr_dashboard_label_text_fps, 13);
    lv_obj_set_width(scr_dashboard_label_text_fps, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_text_fps, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_text_fps, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_text_fps, "FPS");
    lv_label_set_long_mode(scr_dashboard_label_text_fps, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_text_fps - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_text_fps, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_text_fps, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_text_fps, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_data
    scr_dashboard_cont_data = lv_obj_create(scr_dashboard_cont_dashboard);
    lv_obj_set_x(scr_dashboard_cont_data, 652);
    lv_obj_set_y(scr_dashboard_cont_data, 233);
    lv_obj_set_width(scr_dashboard_cont_data, 334);
    lv_obj_set_height(scr_dashboard_cont_data, 182);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_data, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_data, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_data - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_data, lv_color_hex(0x454545), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_data, lv_color_hex(0x1E2426), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_data, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_data, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_data, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_data, 40, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_data, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_data, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_data, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_data, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_data, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_data, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_data, LV_OBJ_FLAG_CLICKABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_data, LV_OBJ_FLAG_SNAPPABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_data, LV_OBJ_FLAG_CLICK_FOCUSABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_data, LV_OBJ_FLAG_GESTURE_BUBBLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_data, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_data, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_data, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_data, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_data, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_cont_01
    scr_dashboard_cont_01 = lv_obj_create(scr_dashboard_cont_data);
    lv_obj_set_x(scr_dashboard_cont_01, 6);
    lv_obj_set_y(scr_dashboard_cont_01, 7);
    lv_obj_set_width(scr_dashboard_cont_01, 105);
    lv_obj_set_height(scr_dashboard_cont_01, 72);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_01, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_01, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_01 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_01, lv_color_hex(0x2C3538), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_01, 160, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_01, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_01, 60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_01, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_01, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_01, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_01, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_01, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_01, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_01, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_01, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_01, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_01, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_01, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_ms
    scr_dashboard_label_ms = lv_label_create(scr_dashboard_cont_01);
    lv_obj_set_x(scr_dashboard_label_ms, 75);
    lv_obj_set_y(scr_dashboard_label_ms, 44);
    lv_obj_set_width(scr_dashboard_label_ms, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_ms, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_ms, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_ms, "ms");
    lv_label_set_long_mode(scr_dashboard_label_ms, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_ms - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_ms, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_ms, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_ms, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_ms, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_runtime_infer_time
    scr_dashboard_label_runtime_infer_time = lv_label_create(scr_dashboard_cont_01);
    lv_obj_set_x(scr_dashboard_label_runtime_infer_time, 2);
    lv_obj_set_y(scr_dashboard_label_runtime_infer_time, 32);
    lv_obj_set_width(scr_dashboard_label_runtime_infer_time, 100);
    lv_obj_set_height(scr_dashboard_label_runtime_infer_time, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_runtime_infer_time, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_runtime_infer_time, "--");
    lv_label_set_long_mode(scr_dashboard_label_runtime_infer_time, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_runtime_infer_time - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_runtime_infer_time, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_runtime_infer_time, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(scr_dashboard_label_runtime_infer_time, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(scr_dashboard_label_runtime_infer_time, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_runtime_infer_time, &lv_font_MiSans_Heavy_24_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_runtime_infer_time, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_text01
    scr_dashboard_label_text01 = lv_label_create(scr_dashboard_cont_01);
    lv_obj_set_x(scr_dashboard_label_text01, 8);
    lv_obj_set_y(scr_dashboard_label_text01, 7);
    lv_obj_set_width(scr_dashboard_label_text01, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_text01, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_text01, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_text01, "推理耗时");
    lv_label_set_long_mode(scr_dashboard_label_text01, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_text01 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_text01, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_text01, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_text01, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_text01, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_03
    scr_dashboard_cont_03 = lv_obj_create(scr_dashboard_cont_data);
    lv_obj_set_x(scr_dashboard_cont_03, 221);
    lv_obj_set_y(scr_dashboard_cont_03, 7);
    lv_obj_set_width(scr_dashboard_cont_03, 105);
    lv_obj_set_height(scr_dashboard_cont_03, 72);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_03, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_03, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_03 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_03, lv_color_hex(0x2C3538), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_03, 160, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_03, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_03, 60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_03, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_03, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_03, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_03, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_03, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_03, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_03, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_03, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_03, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_03, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_03, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_runtime_confidence_threshole_mian
    scr_dashboard_label_runtime_confidence_threshole_mian = lv_label_create(scr_dashboard_cont_03);
    lv_obj_set_x(scr_dashboard_label_runtime_confidence_threshole_mian, 22);
    lv_obj_set_y(scr_dashboard_label_runtime_confidence_threshole_mian, 27);
    lv_obj_set_width(scr_dashboard_label_runtime_confidence_threshole_mian, 100);
    lv_obj_set_height(scr_dashboard_label_runtime_confidence_threshole_mian, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_runtime_confidence_threshole_mian, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_runtime_confidence_threshole_mian, "--");
    lv_label_set_long_mode(scr_dashboard_label_runtime_confidence_threshole_mian, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_runtime_confidence_threshole_mian - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_runtime_confidence_threshole_mian, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_runtime_confidence_threshole_mian, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(scr_dashboard_label_runtime_confidence_threshole_mian, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(scr_dashboard_label_runtime_confidence_threshole_mian, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_runtime_confidence_threshole_mian, &lv_font_MiSans_Heavy_16_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_runtime_confidence_threshole_mian, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_runtime_confidence_threshole_logo
    scr_dashboard_label_runtime_confidence_threshole_logo = lv_label_create(scr_dashboard_cont_03);
    lv_obj_set_x(scr_dashboard_label_runtime_confidence_threshole_logo, 22);
    lv_obj_set_y(scr_dashboard_label_runtime_confidence_threshole_logo, 49);
    lv_obj_set_width(scr_dashboard_label_runtime_confidence_threshole_logo, 100);
    lv_obj_set_height(scr_dashboard_label_runtime_confidence_threshole_logo, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_runtime_confidence_threshole_logo, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_runtime_confidence_threshole_logo, "--");
    lv_label_set_long_mode(scr_dashboard_label_runtime_confidence_threshole_logo, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_runtime_confidence_threshole_logo - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_runtime_confidence_threshole_logo, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_runtime_confidence_threshole_logo, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(scr_dashboard_label_runtime_confidence_threshole_logo, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(scr_dashboard_label_runtime_confidence_threshole_logo, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_runtime_confidence_threshole_logo, &lv_font_MiSans_Heavy_16_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_runtime_confidence_threshole_logo, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_mian
    scr_dashboard_label_mian = lv_label_create(scr_dashboard_cont_03);
    lv_obj_set_x(scr_dashboard_label_mian, 15);
    lv_obj_set_y(scr_dashboard_label_mian, 29);
    lv_obj_set_width(scr_dashboard_label_mian, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_mian, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_mian, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_mian, "面单");
    lv_label_set_long_mode(scr_dashboard_label_mian, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_mian - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_mian, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_mian, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(scr_dashboard_label_mian, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(scr_dashboard_label_mian, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_mian, &lv_font_MiSansVF_12_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_mian, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_labe_logo
    scr_dashboard_labe_logo = lv_label_create(scr_dashboard_cont_03);
    lv_obj_set_x(scr_dashboard_labe_logo, 15);
    lv_obj_set_y(scr_dashboard_labe_logo, 49);
    lv_obj_set_width(scr_dashboard_labe_logo, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_labe_logo, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_labe_logo, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_labe_logo, "Logo");
    lv_label_set_long_mode(scr_dashboard_labe_logo, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_labe_logo - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_labe_logo, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_labe_logo, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(scr_dashboard_labe_logo, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(scr_dashboard_labe_logo, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_labe_logo, &lv_font_MiSansVF_12_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_labe_logo, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_text03
    scr_dashboard_label_text03 = lv_label_create(scr_dashboard_cont_03);
    lv_obj_set_x(scr_dashboard_label_text03, 6);
    lv_obj_set_y(scr_dashboard_label_text03, 6);
    lv_obj_set_width(scr_dashboard_label_text03, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_text03, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_text03, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_text03, "置信度阈值");
    lv_label_set_long_mode(scr_dashboard_label_text03, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_text03 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_text03, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_text03, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_text03, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_text03, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_04
    scr_dashboard_cont_04 = lv_obj_create(scr_dashboard_cont_data);
    lv_obj_set_x(scr_dashboard_cont_04, 114);
    lv_obj_set_y(scr_dashboard_cont_04, 7);
    lv_obj_set_width(scr_dashboard_cont_04, 105);
    lv_obj_set_height(scr_dashboard_cont_04, 72);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_04, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_04, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_04 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_04, lv_color_hex(0x2C3538), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_04, 160, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_04, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_04, 60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_04, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_04, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_04, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_04, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_04, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_04, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_04, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_04, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_04, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_04, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_04, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_YD
    scr_dashboard_label_YD = lv_label_create(scr_dashboard_cont_04);
    lv_obj_set_x(scr_dashboard_label_YD, 2);
    lv_obj_set_y(scr_dashboard_label_YD, 32);
    lv_obj_set_width(scr_dashboard_label_YD, 100);
    lv_obj_set_height(scr_dashboard_label_YD, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_YD, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_YD, "韵达");
    lv_label_set_long_mode(scr_dashboard_label_YD, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_YD - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_YD, lv_color_hex(0xCDAD3D), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_YD, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(scr_dashboard_label_YD, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(scr_dashboard_label_YD, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_YD, &lv_font_MiSans_Heavy_24_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_YD, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_label_YD, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
    // Create scr_dashboard_label_ZT
    scr_dashboard_label_ZT = lv_label_create(scr_dashboard_cont_04);
    lv_obj_set_x(scr_dashboard_label_ZT, 2);
    lv_obj_set_y(scr_dashboard_label_ZT, 32);
    lv_obj_set_width(scr_dashboard_label_ZT, 100);
    lv_obj_set_height(scr_dashboard_label_ZT, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_ZT, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_ZT, "中通");
    lv_label_set_long_mode(scr_dashboard_label_ZT, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_ZT - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_ZT, lv_color_hex(0x39A0BE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_ZT, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(scr_dashboard_label_ZT, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(scr_dashboard_label_ZT, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_ZT, &lv_font_MiSans_Heavy_24_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_ZT, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_label_ZT, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
    // Create scr_dashboard_label_JT
    scr_dashboard_label_JT = lv_label_create(scr_dashboard_cont_04);
    lv_obj_set_x(scr_dashboard_label_JT, 2);
    lv_obj_set_y(scr_dashboard_label_JT, 32);
    lv_obj_set_width(scr_dashboard_label_JT, 100);
    lv_obj_set_height(scr_dashboard_label_JT, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_JT, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_JT, "极兔");
    lv_label_set_long_mode(scr_dashboard_label_JT, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_JT - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_JT, lv_color_hex(0xC23B3B), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_JT, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(scr_dashboard_label_JT, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(scr_dashboard_label_JT, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_JT, &lv_font_MiSans_Heavy_24_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_JT, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_label_JT, LV_OBJ_FLAG_HIDDEN, UI_FLAG_ACTION_ADD);
    // Create scr_dashboard_label_init_express_company
    scr_dashboard_label_init_express_company = lv_label_create(scr_dashboard_cont_04);
    lv_obj_set_x(scr_dashboard_label_init_express_company, 2);
    lv_obj_set_y(scr_dashboard_label_init_express_company, 32);
    lv_obj_set_width(scr_dashboard_label_init_express_company, 100);
    lv_obj_set_height(scr_dashboard_label_init_express_company, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_init_express_company, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_init_express_company, "---");
    lv_label_set_long_mode(scr_dashboard_label_init_express_company, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_init_express_company - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_init_express_company, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_init_express_company, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(scr_dashboard_label_init_express_company, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(scr_dashboard_label_init_express_company, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_init_express_company, &lv_font_MiSans_Heavy_24_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_init_express_company, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_text04
    scr_dashboard_label_text04 = lv_label_create(scr_dashboard_cont_04);
    lv_obj_set_x(scr_dashboard_label_text04, 7);
    lv_obj_set_y(scr_dashboard_label_text04, 7);
    lv_obj_set_width(scr_dashboard_label_text04, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_text04, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_text04, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_text04, "当前快递归属");
    lv_label_set_long_mode(scr_dashboard_label_text04, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_text04 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_text04, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_text04, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_text04, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_text04, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_live_data
    scr_dashboard_cont_live_data = lv_obj_create(scr_dashboard_cont_data);
    lv_obj_set_x(scr_dashboard_cont_live_data, 6);
    lv_obj_set_y(scr_dashboard_cont_live_data, 86);
    lv_obj_set_width(scr_dashboard_cont_live_data, 320);
    lv_obj_set_height(scr_dashboard_cont_live_data, 88);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_live_data, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_live_data, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_live_data - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_live_data, lv_color_hex(0x232C2C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_live_data, 160, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_live_data, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_live_data, 40, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_live_data, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_live_data, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_live_data, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_live_data, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_live_data, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_live_data, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_live_data, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_live_data, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_live_data, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_live_data, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_live_data, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_text_JT
    scr_dashboard_label_text_JT = lv_label_create(scr_dashboard_cont_live_data);
    lv_obj_set_x(scr_dashboard_label_text_JT, 12);
    lv_obj_set_y(scr_dashboard_label_text_JT, 11);
    lv_obj_set_width(scr_dashboard_label_text_JT, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_text_JT, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_text_JT, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_text_JT, "极兔");
    lv_label_set_long_mode(scr_dashboard_label_text_JT, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_text_JT - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_text_JT, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_text_JT, &lv_font_MiSans_Heavy_16_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_text_JT, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_text_ZT
    scr_dashboard_label_text_ZT = lv_label_create(scr_dashboard_cont_live_data);
    lv_obj_set_x(scr_dashboard_label_text_ZT, 12);
    lv_obj_set_y(scr_dashboard_label_text_ZT, 36);
    lv_obj_set_width(scr_dashboard_label_text_ZT, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_text_ZT, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_text_ZT, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_text_ZT, "中通");
    lv_label_set_long_mode(scr_dashboard_label_text_ZT, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_text_ZT - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_text_ZT, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_text_ZT, &lv_font_MiSans_Heavy_16_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_text_ZT, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_JT_A
    scr_dashboard_label_JT_A = lv_label_create(scr_dashboard_cont_live_data);
    lv_obj_set_x(scr_dashboard_label_JT_A, 52);
    lv_obj_set_y(scr_dashboard_label_JT_A, 11);
    lv_obj_set_width(scr_dashboard_label_JT_A, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_JT_A, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_JT_A, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_JT_A, "--");
    lv_label_set_long_mode(scr_dashboard_label_JT_A, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_JT_A - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_JT_A, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_JT_A, &lv_font_MiSansVF_16_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_JT_A, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_text_YD
    scr_dashboard_label_text_YD = lv_label_create(scr_dashboard_cont_live_data);
    lv_obj_set_x(scr_dashboard_label_text_YD, 12);
    lv_obj_set_y(scr_dashboard_label_text_YD, 61);
    lv_obj_set_width(scr_dashboard_label_text_YD, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_text_YD, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_text_YD, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_text_YD, "韵达");
    lv_label_set_long_mode(scr_dashboard_label_text_YD, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_text_YD - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_text_YD, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_text_YD, &lv_font_MiSans_Heavy_16_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_text_YD, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_ZT_A
    scr_dashboard_label_ZT_A = lv_label_create(scr_dashboard_cont_live_data);
    lv_obj_set_x(scr_dashboard_label_ZT_A, 52);
    lv_obj_set_y(scr_dashboard_label_ZT_A, 36);
    lv_obj_set_width(scr_dashboard_label_ZT_A, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_ZT_A, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_ZT_A, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_ZT_A, "--");
    lv_label_set_long_mode(scr_dashboard_label_ZT_A, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_ZT_A - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_ZT_A, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_ZT_A, &lv_font_MiSansVF_16_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_ZT_A, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_YD_A
    scr_dashboard_label_YD_A = lv_label_create(scr_dashboard_cont_live_data);
    lv_obj_set_x(scr_dashboard_label_YD_A, 52);
    lv_obj_set_y(scr_dashboard_label_YD_A, 61);
    lv_obj_set_width(scr_dashboard_label_YD_A, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_YD_A, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_YD_A, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_YD_A, "--");
    lv_label_set_long_mode(scr_dashboard_label_YD_A, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_YD_A - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_YD_A, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_YD_A, &lv_font_MiSansVF_16_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_YD_A, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_slider_jt
    scr_dashboard_slider_jt = lv_slider_create(scr_dashboard_cont_live_data);
    lv_obj_set_x(scr_dashboard_slider_jt, 122);
    lv_obj_set_y(scr_dashboard_slider_jt, 16);
    lv_obj_set_width(scr_dashboard_slider_jt, 180);
    lv_obj_set_height(scr_dashboard_slider_jt, 6);
    lv_obj_set_scrollbar_mode(scr_dashboard_slider_jt, LV_SCROLLBAR_MODE_OFF);
    lv_slider_set_range(scr_dashboard_slider_jt, 0, 100);
    lv_slider_set_value(scr_dashboard_slider_jt, 0, LV_ANIM_OFF);
    // Add style for scr_dashboard_slider_jt - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_slider_jt, lv_color_hex(0x0B0E0E), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_slider_jt, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_slider_jt - LV_PART_INDICATOR | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_slider_jt, lv_color_hex(0xD13F3F), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_slider_jt, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_slider_jt - LV_PART_KNOB | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_slider_jt, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_slider_jt, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_slider_jt, LV_OBJ_FLAG_CLICKABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_slider_jt, LV_OBJ_FLAG_SNAPPABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_slider_jt, LV_OBJ_FLAG_CLICK_FOCUSABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_slider_jt, LV_OBJ_FLAG_GESTURE_BUBBLE, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_slider_zt
    scr_dashboard_slider_zt = lv_slider_create(scr_dashboard_cont_live_data);
    lv_obj_set_x(scr_dashboard_slider_zt, 122);
    lv_obj_set_y(scr_dashboard_slider_zt, 41);
    lv_obj_set_width(scr_dashboard_slider_zt, 180);
    lv_obj_set_height(scr_dashboard_slider_zt, 6);
    lv_obj_set_scrollbar_mode(scr_dashboard_slider_zt, LV_SCROLLBAR_MODE_OFF);
    lv_slider_set_range(scr_dashboard_slider_zt, 0, 100);
    lv_slider_set_value(scr_dashboard_slider_zt, 0, LV_ANIM_OFF);
    // Add style for scr_dashboard_slider_zt - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_slider_zt, lv_color_hex(0x0B0E0E), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_slider_zt, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_slider_zt - LV_PART_INDICATOR | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_slider_zt, lv_color_hex(0x3FB2D1), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_slider_zt, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_slider_zt - LV_PART_KNOB | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_slider_zt, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_slider_zt, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_slider_zt, LV_OBJ_FLAG_CLICKABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_slider_zt, LV_OBJ_FLAG_SNAPPABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_slider_zt, LV_OBJ_FLAG_CLICK_FOCUSABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_slider_zt, LV_OBJ_FLAG_GESTURE_BUBBLE, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_slider_yd
    scr_dashboard_slider_yd = lv_slider_create(scr_dashboard_cont_live_data);
    lv_obj_set_x(scr_dashboard_slider_yd, 122);
    lv_obj_set_y(scr_dashboard_slider_yd, 66);
    lv_obj_set_width(scr_dashboard_slider_yd, 180);
    lv_obj_set_height(scr_dashboard_slider_yd, 6);
    lv_obj_set_scrollbar_mode(scr_dashboard_slider_yd, LV_SCROLLBAR_MODE_OFF);
    lv_slider_set_range(scr_dashboard_slider_yd, 0, 100);
    lv_slider_set_value(scr_dashboard_slider_yd, 0, LV_ANIM_OFF);
    // Add style for scr_dashboard_slider_yd - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_slider_yd, lv_color_hex(0x0B0E0E), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_slider_yd, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_slider_yd - LV_PART_INDICATOR | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_slider_yd, lv_color_hex(0xF4CF4A), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_slider_yd, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_slider_yd - LV_PART_KNOB | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_slider_yd, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_slider_yd, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_slider_yd, LV_OBJ_FLAG_CLICKABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_slider_yd, LV_OBJ_FLAG_SNAPPABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_slider_yd, LV_OBJ_FLAG_CLICK_FOCUSABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_slider_yd, LV_OBJ_FLAG_GESTURE_BUBBLE, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_cont_circle_data
    scr_dashboard_cont_circle_data = lv_obj_create(scr_dashboard_cont_dashboard);
    lv_obj_set_x(scr_dashboard_cont_circle_data, 652);
    lv_obj_set_y(scr_dashboard_cont_circle_data, 119);
    lv_obj_set_width(scr_dashboard_cont_circle_data, 340);
    lv_obj_set_height(scr_dashboard_cont_circle_data, 120);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_circle_data, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_circle_data, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_circle_data - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_circle_data, lv_color_hex(0x1E2426), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(scr_dashboard_cont_circle_data, lv_color_hex(0x454545), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_circle_data, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_cont_circle_data, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_circle_data, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_circle_data, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_circle_data, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_circle_data, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_circle_data, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_circle_data, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_circle_data, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_circle_data, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_circle_data, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_circle_data, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_cont_cpu_02
    scr_dashboard_cont_cpu_02 = lv_obj_create(scr_dashboard_cont_circle_data);
    lv_obj_set_x(scr_dashboard_cont_cpu_02, 182);
    lv_obj_set_y(scr_dashboard_cont_cpu_02, -9);
    lv_obj_set_width(scr_dashboard_cont_cpu_02, 120);
    lv_obj_set_height(scr_dashboard_cont_cpu_02, 120);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_cpu_02, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_cpu_02, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_cpu_02 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_cpu_02, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_cpu_02, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_cpu_02, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_cpu_02, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_cpu_02, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_cpu_02, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_cpu_02, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_cpu_02, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_cpu_02, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_cpu_02, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_cpu_02, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_cpu_02, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_text_cpu_2
    scr_dashboard_label_text_cpu_2 = lv_label_create(scr_dashboard_cont_cpu_02);
    lv_obj_set_x(scr_dashboard_label_text_cpu_2, 26);
    lv_obj_set_y(scr_dashboard_label_text_cpu_2, 100);
    lv_obj_set_width(scr_dashboard_label_text_cpu_2, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_text_cpu_2, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_text_cpu_2, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_text_cpu_2, "--");
    lv_label_set_long_mode(scr_dashboard_label_text_cpu_2, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_text_cpu_2 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_text_cpu_2, lv_color_hex(0x7F98A0), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_text_cpu_2, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_text_cpu_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_arc_runtime_cpu_core2_usage
    scr_dashboard_arc_runtime_cpu_core2_usage = lv_arc_create(scr_dashboard_cont_cpu_02);
    lv_obj_set_x(scr_dashboard_arc_runtime_cpu_core2_usage, 9);
    lv_obj_set_y(scr_dashboard_arc_runtime_cpu_core2_usage, 9);
    lv_obj_set_width(scr_dashboard_arc_runtime_cpu_core2_usage, 98);
    lv_obj_set_height(scr_dashboard_arc_runtime_cpu_core2_usage, 98);
    lv_obj_set_scrollbar_mode(scr_dashboard_arc_runtime_cpu_core2_usage, LV_SCROLLBAR_MODE_OFF);
    lv_arc_set_mode(scr_dashboard_arc_runtime_cpu_core2_usage, LV_ARC_MODE_NORMAL);
    lv_arc_set_range(scr_dashboard_arc_runtime_cpu_core2_usage, 0, 100);
    lv_arc_set_bg_angles(scr_dashboard_arc_runtime_cpu_core2_usage, 135, 45);
    lv_arc_set_value(scr_dashboard_arc_runtime_cpu_core2_usage, 0);
    lv_arc_set_rotation(scr_dashboard_arc_runtime_cpu_core2_usage, 0);
    scr_dashboard_arc_runtime_cpu_core2_usage_label = lv_label_create(scr_dashboard_arc_runtime_cpu_core2_usage);
    lv_label_set_text_fmt(scr_dashboard_arc_runtime_cpu_core2_usage_label, "%ld", lv_arc_get_value(scr_dashboard_arc_runtime_cpu_core2_usage));
    lv_arc_align_obj_to_angle(scr_dashboard_arc_runtime_cpu_core2_usage, scr_dashboard_arc_runtime_cpu_core2_usage_label, 25);
    lv_obj_add_flag(scr_dashboard_arc_runtime_cpu_core2_usage_label, LV_OBJ_FLAG_HIDDEN);
    // Add style for scr_dashboard_arc_runtime_cpu_core2_usage - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_arc_color(scr_dashboard_arc_runtime_cpu_core2_usage, lv_color_hex(0x454545), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(scr_dashboard_arc_runtime_cpu_core2_usage, 155, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(scr_dashboard_arc_runtime_cpu_core2_usage, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(scr_dashboard_arc_runtime_cpu_core2_usage, true, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_arc_runtime_cpu_core2_usage - LV_PART_INDICATOR | LV_STATE_DEFAULT
    lv_obj_set_style_arc_color(scr_dashboard_arc_runtime_cpu_core2_usage, lv_color_hex(0x71CF97), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(scr_dashboard_arc_runtime_cpu_core2_usage, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(scr_dashboard_arc_runtime_cpu_core2_usage, 6, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(scr_dashboard_arc_runtime_cpu_core2_usage, true, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_arc_runtime_cpu_core2_usage - LV_PART_KNOB | LV_STATE_DEFAULT
    lv_obj_set_style_bg_opa(scr_dashboard_arc_runtime_cpu_core2_usage, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_arc_runtime_cpu_core2_usage, LV_OBJ_FLAG_CLICKABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_arc_runtime_cpu_core2_usage, LV_OBJ_FLAG_SNAPPABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_arc_runtime_cpu_core2_usage, LV_OBJ_FLAG_CLICK_FOCUSABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_arc_runtime_cpu_core2_usage, LV_OBJ_FLAG_GESTURE_BUBBLE, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_runtime_cpu_core2_usage
    scr_dashboard_label_runtime_cpu_core2_usage = lv_label_create(scr_dashboard_cont_cpu_02);
    lv_obj_set_x(scr_dashboard_label_runtime_cpu_core2_usage, 8);
    lv_obj_set_y(scr_dashboard_label_runtime_cpu_core2_usage, 51);
    lv_obj_set_width(scr_dashboard_label_runtime_cpu_core2_usage, 100);
    lv_obj_set_height(scr_dashboard_label_runtime_cpu_core2_usage, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_runtime_cpu_core2_usage, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_runtime_cpu_core2_usage, "00%");
    lv_label_set_long_mode(scr_dashboard_label_runtime_cpu_core2_usage, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_runtime_cpu_core2_usage - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_runtime_cpu_core2_usage, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_runtime_cpu_core2_usage, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_runtime_cpu_core2_usage, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_runtime_cpu_core2_usage, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_cpu_01
    scr_dashboard_cont_cpu_01 = lv_obj_create(scr_dashboard_cont_circle_data);
    lv_obj_set_x(scr_dashboard_cont_cpu_01, 35);
    lv_obj_set_y(scr_dashboard_cont_cpu_01, -9);
    lv_obj_set_width(scr_dashboard_cont_cpu_01, 120);
    lv_obj_set_height(scr_dashboard_cont_cpu_01, 120);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_cpu_01, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_cpu_01, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_cpu_01 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_cpu_01, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_cpu_01, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_cpu_01, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_cpu_01, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_cpu_01, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_cpu_01, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_cpu_01, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_cpu_01, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_cpu_01, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_cpu_01, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_cpu_01, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_cpu_01, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_arc_runtime_cpu_core1_usage
    scr_dashboard_arc_runtime_cpu_core1_usage = lv_arc_create(scr_dashboard_cont_cpu_01);
    lv_obj_set_x(scr_dashboard_arc_runtime_cpu_core1_usage, 8);
    lv_obj_set_y(scr_dashboard_arc_runtime_cpu_core1_usage, 8);
    lv_obj_set_width(scr_dashboard_arc_runtime_cpu_core1_usage, 98);
    lv_obj_set_height(scr_dashboard_arc_runtime_cpu_core1_usage, 98);
    lv_obj_set_scrollbar_mode(scr_dashboard_arc_runtime_cpu_core1_usage, LV_SCROLLBAR_MODE_OFF);
    lv_arc_set_mode(scr_dashboard_arc_runtime_cpu_core1_usage, LV_ARC_MODE_NORMAL);
    lv_arc_set_range(scr_dashboard_arc_runtime_cpu_core1_usage, 0, 100);
    lv_arc_set_bg_angles(scr_dashboard_arc_runtime_cpu_core1_usage, 135, 45);
    lv_arc_set_value(scr_dashboard_arc_runtime_cpu_core1_usage, 0);
    lv_arc_set_rotation(scr_dashboard_arc_runtime_cpu_core1_usage, 0);
    scr_dashboard_arc_runtime_cpu_core1_usage_label = lv_label_create(scr_dashboard_arc_runtime_cpu_core1_usage);
    lv_label_set_text_fmt(scr_dashboard_arc_runtime_cpu_core1_usage_label, "%ld", lv_arc_get_value(scr_dashboard_arc_runtime_cpu_core1_usage));
    lv_arc_align_obj_to_angle(scr_dashboard_arc_runtime_cpu_core1_usage, scr_dashboard_arc_runtime_cpu_core1_usage_label, 25);
    lv_obj_add_flag(scr_dashboard_arc_runtime_cpu_core1_usage_label, LV_OBJ_FLAG_HIDDEN);
    // Add style for scr_dashboard_arc_runtime_cpu_core1_usage - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_arc_color(scr_dashboard_arc_runtime_cpu_core1_usage, lv_color_hex(0x454545), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(scr_dashboard_arc_runtime_cpu_core1_usage, 155, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(scr_dashboard_arc_runtime_cpu_core1_usage, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(scr_dashboard_arc_runtime_cpu_core1_usage, true, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_arc_runtime_cpu_core1_usage - LV_PART_INDICATOR | LV_STATE_DEFAULT
    lv_obj_set_style_arc_color(scr_dashboard_arc_runtime_cpu_core1_usage, lv_color_hex(0x3FB2D1), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(scr_dashboard_arc_runtime_cpu_core1_usage, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(scr_dashboard_arc_runtime_cpu_core1_usage, 6, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(scr_dashboard_arc_runtime_cpu_core1_usage, true, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    // Add style for scr_dashboard_arc_runtime_cpu_core1_usage - LV_PART_KNOB | LV_STATE_DEFAULT
    lv_obj_set_style_bg_opa(scr_dashboard_arc_runtime_cpu_core1_usage, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_arc_runtime_cpu_core1_usage, LV_OBJ_FLAG_CLICKABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_arc_runtime_cpu_core1_usage, LV_OBJ_FLAG_SNAPPABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_arc_runtime_cpu_core1_usage, LV_OBJ_FLAG_CLICK_FOCUSABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_arc_runtime_cpu_core1_usage, LV_OBJ_FLAG_GESTURE_BUBBLE, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_runtime_cpu_core1_usage
    scr_dashboard_label_runtime_cpu_core1_usage = lv_label_create(scr_dashboard_cont_cpu_01);
    lv_obj_set_x(scr_dashboard_label_runtime_cpu_core1_usage, 8);
    lv_obj_set_y(scr_dashboard_label_runtime_cpu_core1_usage, 51);
    lv_obj_set_width(scr_dashboard_label_runtime_cpu_core1_usage, 100);
    lv_obj_set_height(scr_dashboard_label_runtime_cpu_core1_usage, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_runtime_cpu_core1_usage, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_runtime_cpu_core1_usage, "00%");
    lv_label_set_long_mode(scr_dashboard_label_runtime_cpu_core1_usage, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_runtime_cpu_core1_usage - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_runtime_cpu_core1_usage, lv_color_hex(0xDDEBE7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_runtime_cpu_core1_usage, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_runtime_cpu_core1_usage, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_runtime_cpu_core1_usage, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_text_cpu_1
    scr_dashboard_label_text_cpu_1 = lv_label_create(scr_dashboard_cont_cpu_01);
    lv_obj_set_x(scr_dashboard_label_text_cpu_1, 26);
    lv_obj_set_y(scr_dashboard_label_text_cpu_1, 99);
    lv_obj_set_width(scr_dashboard_label_text_cpu_1, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_text_cpu_1, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_text_cpu_1, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_text_cpu_1, "--");
    lv_label_set_long_mode(scr_dashboard_label_text_cpu_1, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_text_cpu_1 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_text_cpu_1, lv_color_hex(0x7F98A0), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_text_cpu_1, &lv_font_MiSansVF_14_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_text_cpu_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_cont_esp32
    scr_dashboard_cont_esp32 = lv_obj_create(scr_dashboard_cont_background2);
    lv_obj_set_x(scr_dashboard_cont_esp32, 907);
    lv_obj_set_y(scr_dashboard_cont_esp32, 15);
    lv_obj_set_width(scr_dashboard_cont_esp32, 84);
    lv_obj_set_height(scr_dashboard_cont_esp32, 28);
    lv_obj_set_scrollbar_mode(scr_dashboard_cont_esp32, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(scr_dashboard_cont_esp32, LV_LAYOUT_NONE);
    // Add style for scr_dashboard_cont_esp32 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(scr_dashboard_cont_esp32, lv_color_hex(0x454545), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr_dashboard_cont_esp32, 60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(scr_dashboard_cont_esp32, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(scr_dashboard_cont_esp32, 80, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scr_dashboard_cont_esp32, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr_dashboard_cont_esp32, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(scr_dashboard_cont_esp32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(scr_dashboard_cont_esp32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(scr_dashboard_cont_esp32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(scr_dashboard_cont_esp32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_cont_esp32, LV_OBJ_FLAG_SCROLLABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_esp32, LV_OBJ_FLAG_SCROLL_ELASTIC, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_esp32, LV_OBJ_FLAG_SCROLL_MOMENTUM, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_esp32, LV_OBJ_FLAG_SCROLL_WITH_ARROW, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_cont_esp32, LV_OBJ_FLAG_SCROLL_CHAIN, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_label_esp32
    scr_dashboard_label_esp32 = lv_label_create(scr_dashboard_cont_esp32);
    lv_obj_set_x(scr_dashboard_label_esp32, 12);
    lv_obj_set_y(scr_dashboard_label_esp32, 8);
    lv_obj_set_width(scr_dashboard_label_esp32, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_esp32, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_esp32, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_esp32, "ESP32-P4");
    lv_label_set_long_mode(scr_dashboard_label_esp32, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_esp32 - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_esp32, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(scr_dashboard_label_esp32, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_esp32, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_esp32, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_label_title_A
    scr_dashboard_label_title_A = lv_label_create(scr_dashboard_cont_background2);
    lv_obj_set_x(scr_dashboard_label_title_A, 286);
    lv_obj_set_y(scr_dashboard_label_title_A, 22);
    lv_obj_set_width(scr_dashboard_label_title_A, LV_SIZE_CONTENT);
    lv_obj_set_height(scr_dashboard_label_title_A, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(scr_dashboard_label_title_A, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(scr_dashboard_label_title_A, "基于ESP32-P4的快递智能分拣系统与YOLO轻量化部署优化");
    lv_label_set_long_mode(scr_dashboard_label_title_A, LV_LABEL_LONG_WRAP);
    // Add style for scr_dashboard_label_title_A - LV_PART_MAIN | LV_STATE_DEFAULT
    lv_obj_set_style_text_color(scr_dashboard_label_title_A, lv_color_hex(0xD1DEDE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(scr_dashboard_label_title_A, &lv_font_MiSans_Heavy_16_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_label_title_A, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Create scr_dashboard_img_runtime_ethernet_status
    scr_dashboard_img_runtime_ethernet_status = lv_imgbtn_create(scr_dashboard_cont_background2);
    lv_obj_set_x(scr_dashboard_img_runtime_ethernet_status, 769);
    lv_obj_set_y(scr_dashboard_img_runtime_ethernet_status, 12);
    lv_obj_set_width(scr_dashboard_img_runtime_ethernet_status, 126);
    lv_obj_set_height(scr_dashboard_img_runtime_ethernet_status, 36);
    lv_obj_set_scrollbar_mode(scr_dashboard_img_runtime_ethernet_status, LV_SCROLLBAR_MODE_OFF);
    scr_dashboard_img_runtime_ethernet_status_label = lv_label_create(scr_dashboard_img_runtime_ethernet_status);
    lv_label_set_text(scr_dashboard_img_runtime_ethernet_status_label, "");
    lv_label_set_long_mode(scr_dashboard_img_runtime_ethernet_status_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(scr_dashboard_img_runtime_ethernet_status_label, LV_PCT(100));
    lv_obj_set_align(scr_dashboard_img_runtime_ethernet_status_label, LV_ALIGN_CENTER);
    lv_imgbtn_set_src(scr_dashboard_img_runtime_ethernet_status, LV_IMGBTN_STATE_RELEASED, NULL, &disconnect_126x36, NULL);
    lv_imgbtn_set_src(scr_dashboard_img_runtime_ethernet_status, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &connect_126x36, NULL);
    lv_obj_add_flag(scr_dashboard_img_runtime_ethernet_status, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_set_style_text_align(scr_dashboard_img_runtime_ethernet_status, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_flag_modify(scr_dashboard_img_runtime_ethernet_status, LV_OBJ_FLAG_CLICKABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_img_runtime_ethernet_status, LV_OBJ_FLAG_SNAPPABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_img_runtime_ethernet_status, LV_OBJ_FLAG_CLICK_FOCUSABLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_img_runtime_ethernet_status, LV_OBJ_FLAG_GESTURE_BUBBLE, UI_FLAG_ACTION_REMOVE);
    ui_flag_modify(scr_dashboard_img_runtime_ethernet_status, LV_OBJ_FLAG_PRESS_LOCK, UI_FLAG_ACTION_REMOVE);
    // Create scr_dashboard_imgbtn_logo
    scr_dashboard_imgbtn_logo = lv_imgbtn_create(scr_dashboard_cont_background2);
    lv_obj_set_x(scr_dashboard_imgbtn_logo, 13);
    lv_obj_set_y(scr_dashboard_imgbtn_logo, 0);
    lv_obj_set_width(scr_dashboard_imgbtn_logo, 260);
    lv_obj_set_height(scr_dashboard_imgbtn_logo, 60);
    lv_obj_set_scrollbar_mode(scr_dashboard_imgbtn_logo, LV_SCROLLBAR_MODE_OFF);
    scr_dashboard_imgbtn_logo_label = lv_label_create(scr_dashboard_imgbtn_logo);
    lv_label_set_text(scr_dashboard_imgbtn_logo_label, "");
    lv_label_set_long_mode(scr_dashboard_imgbtn_logo_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(scr_dashboard_imgbtn_logo_label, LV_PCT(100));
    lv_obj_set_align(scr_dashboard_imgbtn_logo_label, LV_ALIGN_CENTER);
    lv_imgbtn_set_src(scr_dashboard_imgbtn_logo, LV_IMGBTN_STATE_RELEASED, NULL, &LOGO_bordered_260x60, NULL);
    lv_obj_add_flag(scr_dashboard_imgbtn_logo, LV_OBJ_FLAG_CHECKABLE);
    // Add style for scr_dashboard_imgbtn_logo - LV_PART_MAIN | LV_STATE_PRESSED
    lv_obj_set_style_bg_color(scr_dashboard_imgbtn_logo, lv_color_hex(0x111919), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_grad_color(scr_dashboard_imgbtn_logo, lv_color_hex(0x232C2C), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(scr_dashboard_imgbtn_logo, 160, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_grad_dir(scr_dashboard_imgbtn_logo, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_radius(scr_dashboard_imgbtn_logo, 20, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_text_align(scr_dashboard_imgbtn_logo, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(scr_dashboard_imgbtn_logo, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_PRESSED);
    return scr_dashboard;
}
lv_obj_t * setup_scr_dashboard(void) {
    if (scr_dashboard != NULL) {
        init_states();
        return scr_dashboard;
    }
    create_ui();
    register_ui_events();
    register_sys_events(&scr_dashboard_event_table);
    init_states();
    return scr_dashboard;
}
