/*
 * vision_model —— 两级级联推理与模型热切换的 C 接口。
 *
 * vision_model_run 依次执行面单整图检测、最高分面单 ROI 裁剪、Logo 检测，
 * 并将 Logo 框映射回原图坐标系。模型通过分阶段只读注册表和稳定 ID 选择；
 * 调用方只投递切换请求，视觉推理任务在两次完整级联之间执行句柄替换。
 *
 * box 坐标系约定：vision_model_run 输出为【原图坐标系】，rescale 由 vision_detect.c 负责。
 */
#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

// 检测框所属的级联阶段。
typedef enum {
    VISION_STAGE_WAYBILL = 0,   // 模型1：面单框（单分类，category 恒 0）
    VISION_STAGE_LOGO    = 1,   // 模型2：logo 框（三分类，category 0=极兔 1=韵达 2=中通）
} vision_stage_t;

#define VISION_MODEL_ID_INVALID UINT32_MAX

typedef uint32_t vision_model_id_t;

enum {
    VISION_MODEL_ID_WAYBILL_1 = 0x00010001u,
    VISION_MODEL_ID_WAYBILL_2 = 0x00010002u,
    VISION_MODEL_ID_WAYBILL_3 = 0x00010003u,
    VISION_MODEL_ID_LOGO_1 = 0x00020001u,
    VISION_MODEL_ID_LOGO_2 = 0x00020002u,
    VISION_MODEL_ID_LOGO_3 = 0x00020003u,
};

#define VISION_MODEL_ID_WAYBILL_DEFAULT VISION_MODEL_ID_WAYBILL_1
#define VISION_MODEL_ID_LOGO_DEFAULT VISION_MODEL_ID_LOGO_1

typedef struct {
    vision_model_id_t id;
    vision_stage_t stage;
    const char *display_name;
} vision_model_entry_t;

typedef enum {
    VISION_MODEL_SWITCH_IDLE = 0,
    VISION_MODEL_SWITCH_QUEUED,
    VISION_MODEL_SWITCH_SWITCHING,
    VISION_MODEL_SWITCH_SUCCEEDED,
    VISION_MODEL_SWITCH_FAILED_RESTORED,
    VISION_MODEL_SWITCH_FAILED_UNAVAILABLE,
} vision_model_switch_state_t;

typedef struct {
    vision_model_switch_state_t state;
    vision_stage_t stage;
    vision_model_id_t requested_id;
    vision_model_id_t active_id;
    esp_err_t error;
} vision_model_switch_status_t;

// 单个检测框。box = [left_up_x, left_up_y, right_down_x, right_down_y]，使用原图坐标系。
typedef struct {
    int box[4];            // 边界框：左上 x,y 右下 x,y
    int category;          // 类别索引
    float score;           // 置信度 0~1
    vision_stage_t stage;  // 所属级联阶段（画框据此分色）
} vision_model_det_t;

// 挂载模型 SPIFFS，恢复 NVS 选择并建立两级模型句柄。
// 须在 vision_start 单线程阶段、detect 任务创建前调用。
bool vision_model_init(void);

// 对一帧 RGB888 原图跑两级级联推理，框写入 dets（最多 max 个），返回框数；<0 表示失败。
// buf 为 width×height×3 RGB888 裸指针（原图坐标系）。输出含 stage 标记（面单 + logo）。
int vision_model_run(const uint8_t *buf, int width, int height,
                     vision_model_det_t *dets, int max);

// 最近一次 vision_model_run 的两级合计推理耗时（毫秒）。
int vision_model_last_infer_ms(void);
int vision_model_last_waybill_infer_ms(void);
int vision_model_last_logo_infer_ms(void);

// 取三类（极兔/韵达/中通）概率 ×100（来自模型2 Logo 分类）。
void vision_model_get_class_probs(int *jt, int *zt, int *yd);

int vision_model_get_waybill_score_threshold_percent(void);
int vision_model_get_logo_score_threshold_percent(void);
void vision_model_set_waybill_score_threshold_percent(int percent);
void vision_model_set_logo_score_threshold_percent(int percent);

// 返回两个阶段的活动模型显示名，中间用 " / " 分隔；返回值为当前任务的只读快照。
// 阶段句柄不可用时显示 "Unavailable"，供 UI 关于页“模型信息”标签显示。
const char *vision_model_get_model_info_string(void);

const vision_model_entry_t *vision_model_registry_get(
    vision_stage_t stage, size_t *count);
esp_err_t vision_model_request_switch(
    vision_stage_t stage, vision_model_id_t model_id);
esp_err_t vision_model_get_switch_status(
    vision_model_switch_status_t *out_status);
vision_model_id_t vision_model_get_active_id(vision_stage_t stage);

// 仅由视觉推理任务在完整两级级联之间调用。
void vision_model_process_pending_switch(void);

#ifdef __cplusplus
}
#endif
