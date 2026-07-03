/*
 * vision_model —— 推理模型 C 封装层接口（两级级联，模型留桩阶段）。
 *
 * 两层设计（可扩展性核心，对齐 esp-dl）：
 *   第一层 原子模型接口（单图 → 结果）：复刻 dl::detect::Detect::run(img_t)。
 *     每个模型是一个 vision_detector_t 句柄，输入单张 RGB888 图、输出该图坐标系的框。
 *     后续真实模型只替换 vision_detector_load / vision_detector_run 的桩体
 *     （C++ 薄封装 extern "C"：组装 img_t → model->run → 拷 result_t），编排层不动。
 *   第二层 级联编排（vision_model_run）：复刻 esp-dl HumanFaceDetect 的内部级联——
 *     模型1(waybill) 跑整图 → 取最高分面单 → 从原图裁剪 ROI → 模型2(logo) 跑 ROI →
 *     logo 框加面单左上角偏移映射回原图。对外仍是签名不变的一次调用。
 *
 * box 坐标系约定：vision_model_run 输出为【原图坐标系】，rescale 由 vision_detect.c 负责。
 */
#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// 检测框所属的级联阶段。
typedef enum {
    VISION_STAGE_WAYBILL = 0,   // 模型1：面单框（单分类，category 恒 0）
    VISION_STAGE_LOGO    = 1,   // 模型2：logo 框（三分类，category 0=极兔 1=韵达 2=中通）
} vision_stage_t;

// 单个检测框。box = [left_up_x, left_up_y, right_down_x, right_down_y]。
// 原子接口输出时为【输入图坐标系】；经 vision_model_run 映射后为【原图坐标系】。
typedef struct {
    int box[4];            // 边界框：左上 x,y 右下 x,y
    int category;          // 类别索引
    float score;           // 置信度 0~1
    vision_stage_t stage;  // 所属级联阶段（画框据此分色）
} vision_model_det_t;

// ===== 第一层：原子模型接口（单图 → 结果，对齐 dl::detect::Detect::run(img_t)）=====

// 不透明模型句柄。桩阶段为占位（携带模型标识 + 桩状态）；
// 真实阶段内部持 dl::detect::Detect* 及其前后处理器。
typedef struct vision_detector vision_detector_t;

// 加载一个检测模型。model_name 为 SPIFFS 文件名（须 ASCII，如 "pico_416_p4.espdl"）；
// score_thr/nms_thr 为该模型的检测/NMS 阈值。输入尺寸不需传——ImagePreprocessor
// 从模型自身读取输入张量形状。内部 new PicoDetect(装配 dl::Model+前后处理器)。
// 返回 NULL 表示失败。
vision_detector_t *vision_detector_load(const char *model_name, float score_thr, float nms_thr);

// 释放模型句柄。
void vision_detector_free(vision_detector_t *det);

// 对单张 RGB888 图片跑推理，框写入 out（最多 max 个），返回框数；<0 表示失败。
// img 为 width×height×3 RGB888 裸指针，输出 box 为【输入图坐标系】。
// 真实阶段桩体替换为：组装 img_t{img,width,height,RGB888} → model->run(img)
//                    → 遍历 std::list<result_t> 拷进 out[]。
int vision_detector_run(vision_detector_t *det,
                        const uint8_t *img, int width, int height,
                        vision_model_det_t *out, int max);

// ===== 第二层：级联编排 =====

// 加载模型（挂 SPIFFS + 建两级模型句柄）。桩恒返回 true。
// 须在 vision_start 单线程阶段、detect 任务创建前调用。
bool vision_model_init(void);

// 对一帧 RGB888 原图跑两级级联推理，框写入 dets（最多 max 个），返回框数；<0 表示失败。
// buf 为 width×height×3 RGB888 裸指针（原图坐标系）。输出含 stage 标记（面单 + logo）。
int vision_model_run(const uint8_t *buf, int width, int height,
                     vision_model_det_t *dets, int max);

// 最近一次 vision_model_run 的两级合计推理耗时（毫秒）。
int vision_model_last_infer_ms(void);

// 取三类（极兔/韵达/中通）概率 ×100（来自模型2 logo 分类）。桩填占位值。
void vision_model_get_class_probs(int *jt, int *zt, int *yd);

#ifdef __cplusplus
}
#endif
