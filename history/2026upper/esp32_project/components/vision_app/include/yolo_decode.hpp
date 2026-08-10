/*
 * YOLO 解码器接口。
 *
 * 将 ESP-DL 模型输出 tensor 解析为 Detection 列表。
 */
#pragma once

#include <cstdint>
#include <vector>

struct Detection;

struct Yolo11Output {
    const int8_t *data;
    int exponent;
    const std::vector<int> *shape;
};

/* YOLO 解码配置 — 对齐 main.cpp 中的 YOLOConfig */
struct DecodeConfig {
    int input_w;           /* 模型输入宽度（必须与训练尺寸一致） */
    int input_h;           /* 模型输入高度（必须与训练尺寸一致） */
    int num_classes;       /* 检测类别数 */
    float conf_threshold;  /* 置信度阈值 */
};

/**
 * 解码 ultralytics YOLO (v8/v26) 单 tensor 输出。
 *
 * 模型输出格式（自动检测）：
 *   [1, 4+num_classes, num_anchors]    — channel-first（默认）
 *   [1, num_anchors, 4+num_classes]    — anchor-first
 *
 * 各通道含义：
 *   [0..3]    — bbox (cx, cy, w, h) letterbox 空间（DFL 已解码）
 *   [4..]     — 各类置信度（ultralytics 导出时已 sigmoid）
 *
 * ESP-DL 量化: float_val = int8_val × 2^exponent
 *
 * 坐标映射:   orig_coord = (lb_coord - pad) / scale
 *
 * @param data         output0 的 int8 数据
 * @param exponent     output0 tensor 的 exponent 值
 * @param shape        output0 形状, e.g. [1,5,8400] 或 [5,8400]
 * @param cfg          解码配置（类别数、阈值、输入尺寸）
 * @param lb_scale     letterbox 缩放比例 (= min(input_w/orig_w, input_h/orig_h))
 * @param lb_pad_x     letterbox 水平填充量（模型输入空间像素）
 * @param lb_pad_y     letterbox 垂直填充量（模型输入空间像素）
 * @param orig_w       原始图像宽度（用于 clamp）
 * @param orig_h       原始图像高度（用于 clamp）
 * @param results      输出检测结果列表（原始图像坐标）
 */
void yolo_decode_v26(
    const int8_t *data, int exponent,
    const std::vector<int> &shape,
    const DecodeConfig &cfg,
    float lb_scale, int lb_pad_x, int lb_pad_y,
    int orig_w, int orig_h,
    std::vector<Detection> &results);

/**
 * 解码 ESP-DL YOLO11n COCO Detect 六输出格式。
 *
 * 输出为三组尺度，每组包含：
 *   bbox DFL: [1, 64, H, W]  (4 sides * reg_max=16)
 *   cls logits: [1, 80, H, W]
 *
 * sigmoid 和 bbox decode 在后处理里完成，随后做同类 NMS。
 */
void yolo_decode_yolo11(
    const std::vector<Yolo11Output> &outputs,
    const DecodeConfig &cfg,
    float lb_scale, int lb_pad_x, int lb_pad_y,
    int orig_w, int orig_h,
    std::vector<Detection> &results);

/**
 * 解码 ESPDet / ESPDet-Pico 三尺度输出。
 *
 * 支持：
 *   ESPDet:      box*  [1,H,W,4]   + score* [1,H,W,C]
 *   ESPDet-Pico: bbox* [1,H,W,32]  + score* [1,H,W,C], DFL reg_max=8
 */
void yolo_decode_espdet(
    const std::vector<Yolo11Output> &outputs,
    const DecodeConfig &cfg,
    float lb_scale, int lb_pad_x, int lb_pad_y,
    int orig_w, int orig_h,
    std::vector<Detection> &results,
    float *best_confidence = nullptr,
    float *class_prob = nullptr);
