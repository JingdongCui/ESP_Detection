/*
 * 目标检测模块 — ESP-DL INT8 量化推理
 *
 * 依赖：ESP-DL (dl::Model)、PSRAM 用于模型输入缓冲
 *
 * 用户可修改项：
 *   YOLOConfig 结构体默认值 — 模型输入尺寸、类别数、置信度阈值
 */
#pragma once

#include <cstdint>
#include <vector>

struct Detection {
    float x1, y1, x2, y2;   /* 原始图像坐标系中的边界框 */
    float confidence;        /* 置信度 (0~1) */
    int class_id;            /* 类别索引, 对应 CLASS_NAMES 数组下标 */
};

/* ============================================================
 * [用户配置] YOLO 推理参数默认值
 *
 * 实际值由 main.cpp 中的宏覆盖（INPUT_W, CONF_THRESHOLD 等）。
 * 此处定义的是 YOLOConfig 未显式初始化时的回退值。
 * ============================================================ */
struct YOLOConfig {
    int input_w = 416;          /* 模型输入宽度（必须与训练尺寸一致） */
    int input_h = 416;          /* 模型输入高度（必须与训练尺寸一致） */
    int num_classes = 1;        /* 检测类别数（必须与模型一致） */
    float conf_threshold = 0.60f; /* 置信度阈值：升高=减少误检，降低=减少漏检 */
};

struct YOLOPerfStats {
    static constexpr int kClassProbCount = 3;
    int64_t roi_detect_us = 0;
    int64_t ppa_resize_us = 0;
    int64_t quantize_us = 0;
    int64_t preprocess_us = 0;
    int64_t input_copy_us = 0;
    int64_t pre_us = 0;
    int64_t run_us = 0;
    int64_t decode_us = 0;
    int64_t total_us = 0;
    float target_confidence = 0.0f;
    uint32_t detect_count = 0;
    bool runtime_multi_core = false;
    bool fallback_triggered = false;
    bool ppa_preprocess = false;
    bool roi_success = false;
    bool roi_skipped = false;
    int roi_fail_reason = 0;
    int roi_x = 0;
    int roi_y = 0;
    int roi_w = 0;
    int roi_h = 0;
    float class_prob[kClassProbCount] = {};
    float class_prob_avg[kClassProbCount] = {};
    float class_prob_max[kClassProbCount] = {};
};

class AppYOLO {
public:
    AppYOLO(const YOLOConfig &cfg);
    ~AppYOLO();

    /** 从 SPIFFS 加载 .espdl 模型文件，分配输入缓冲 */
    bool load_model(const char *model_path);

    /**
     * 对 RGB888 图像执行目标检测
     *
     * @param rgb888    输入图像（RGB888, 逐像素 R,G,B 排列）
     * @param src_w     图像宽度
     * @param src_h     图像高度
     * @param results   输出检测结果（原始图像坐标）
     * @return          推理是否成功
     */
    bool detect(const uint8_t *rgb888, int src_w, int src_h,
                std::vector<Detection> &results);

    YOLOPerfStats get_last_perf_stats() const;

private:
    struct Impl;
    Impl *impl_;
};
