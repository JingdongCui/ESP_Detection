/*
 * YOLOv26n 解码器 — 将模型输出 tensor 解析为检测框列表
 *
 * 支持的输出格式：
 *   [1, 4+num_classes, num_anchors]  (channel-first, 默认)
 *   [1, num_anchors, 4+num_classes]  (anchor-first, 自动检测)
 *   [4+num_classes, num_anchors]     (无 batch 维)
 *
 * ESP-DL 量化约定：
 *   float_val = int8_val × 2^exponent
 *
 * 坐标映射流程：
 *   模型输出 (letterbox 空间) → 去 letterbox → 原始图像坐标
 */

#include "yolo_decode.hpp"
#include "app_yolo.hpp"
#include "esp_log.h"
#include <cmath>
#include <algorithm>
#include <array>

static const char *TAG_DEC = "yolo_decode";

/** ESP-DL 量化：exponent → 缩放因子 = 2^exponent */
static inline float dl_scale(int exponent)
{
    return ldexpf(1.0f, exponent);
}

static inline float sigmoid(float x)
{
    if (x >= 0.0f) {
        float z = expf(-x);
        return 1.0f / (1.0f + z);
    }
    float z = expf(x);
    return z / (1.0f + z);
}

static float iou(const Detection &a, const Detection &b)
{
    float ix1 = std::max(a.x1, b.x1);
    float iy1 = std::max(a.y1, b.y1);
    float ix2 = std::min(a.x2, b.x2);
    float iy2 = std::min(a.y2, b.y2);
    float iw = std::max(0.0f, ix2 - ix1);
    float ih = std::max(0.0f, iy2 - iy1);
    float inter = iw * ih;
    float area_a = std::max(0.0f, a.x2 - a.x1) * std::max(0.0f, a.y2 - a.y1);
    float area_b = std::max(0.0f, b.x2 - b.x1) * std::max(0.0f, b.y2 - b.y1);
    float uni = area_a + area_b - inter;
    return uni > 0.0f ? inter / uni : 0.0f;
}

static bool is_nhwc(const Yolo11Output &out)
{
    int c1 = out.shape->size() == 4 ? (*out.shape)[1] : 0;
    int c3 = out.shape->size() == 4 ? (*out.shape)[3] : 0;
    return c3 == 64 || c3 == 80 || (c1 != 64 && c1 != 80);
}

static float dfl_expectation(const Yolo11Output &out,
                             int height, int width,
                             int side, int y, int x)
{
    constexpr int REG_MAX = 16;
    float scale = dl_scale(out.exponent);
    float logits[REG_MAX];
    float max_v = -1e9f;
    bool nhwc = is_nhwc(out);

    for (int b = 0; b < REG_MAX; b++) {
        int ch = side * REG_MAX + b;
        int idx = nhwc ? ((y * width + x) * 64 + ch)
                       : ((ch * height + y) * width + x);
        float v = (float)out.data[idx] * scale;
        logits[b] = v;
        max_v = std::max(max_v, v);
    }

    float sum = 0.0f;
    float weighted = 0.0f;
    for (int b = 0; b < REG_MAX; b++) {
        float p = expf(logits[b] - max_v);
        sum += p;
        weighted += p * (float)b;
    }
    return sum > 0.0f ? weighted / sum : 0.0f;
}

struct Yolo11Scale {
    const Yolo11Output *bbox = nullptr;
    const Yolo11Output *cls = nullptr;
    int height = 0;
    int width = 0;
};

static int output_channel_count(const Yolo11Output &out)
{
    if (out.shape->size() != 4 || (*out.shape)[0] != 1) return 0;
    int c1 = (*out.shape)[1];
    int c3 = (*out.shape)[3];
    if (c1 == 64 || c1 == 80) return c1;
    if (c3 == 64 || c3 == 80) return c3;
    return 0;
}

static int output_height(const Yolo11Output &out)
{
    if (out.shape->size() != 4 || (*out.shape)[0] != 1) return 0;
    return is_nhwc(out) ? (*out.shape)[1] : (*out.shape)[2];
}

static int output_width(const Yolo11Output &out)
{
    if (out.shape->size() != 4 || (*out.shape)[0] != 1) return 0;
    return is_nhwc(out) ? (*out.shape)[2] : (*out.shape)[3];
}

static bool is_tensor_nhwc(const Yolo11Output &out, int channels)
{
    if (out.shape->size() != 4 || (*out.shape)[0] != 1) return true;
    if ((*out.shape)[3] == channels) return true;
    if ((*out.shape)[1] == channels) return false;
    return true;
}

static int tensor_h(const Yolo11Output &out, int channels)
{
    if (out.shape->size() != 4 || (*out.shape)[0] != 1) return 0;
    return is_tensor_nhwc(out, channels) ? (*out.shape)[1] : (*out.shape)[2];
}

static int tensor_w(const Yolo11Output &out, int channels)
{
    if (out.shape->size() != 4 || (*out.shape)[0] != 1) return 0;
    return is_tensor_nhwc(out, channels) ? (*out.shape)[2] : (*out.shape)[3];
}

static int tensor_c(const Yolo11Output &out)
{
    if (out.shape->size() != 4 || (*out.shape)[0] != 1) return 0;
    int c1 = (*out.shape)[1];
    int c3 = (*out.shape)[3];
    if (c1 == 4 || c1 == 32 || c1 == 1) return c1;
    if (c3 == 4 || c3 == 32 || c3 == 1) return c3;
    if (c3 > 1 && c3 <= 128) return c3;
    if (c1 > 1 && c1 <= 128) return c1;
    return 0;
}

static float tensor_value(const Yolo11Output &out, int channels, int h, int w,
                          int y, int x, int c)
{
    bool nhwc = is_tensor_nhwc(out, channels);
    int idx = nhwc ? ((y * w + x) * channels + c)
                   : ((c * h + y) * w + x);
    return (float)out.data[idx] * dl_scale(out.exponent);
}

static float pico_dfl(const Yolo11Output &out, int h, int w, int y, int x, int side)
{
    constexpr int REG_MAX = 8;
    float logits[REG_MAX];
    float max_v = -1e9f;
    for (int b = 0; b < REG_MAX; b++) {
        float v = tensor_value(out, 32, h, w, y, x, side * REG_MAX + b);
        logits[b] = v;
        max_v = std::max(max_v, v);
    }

    float sum = 0.0f;
    float weighted = 0.0f;
    for (int b = 0; b < REG_MAX; b++) {
        float p = expf(logits[b] - max_v);
        sum += p;
        weighted += p * (float)b;
    }
    return sum > 0.0f ? weighted / sum : 0.0f;
}

static uint32_t s_espdet_decode_count = 0;

struct ESPDetScale {
    const Yolo11Output *box = nullptr;
    const Yolo11Output *score = nullptr;
    int box_channels = 0;
    int height = 0;
    int width = 0;
};

void yolo_decode_espdet(
    const std::vector<Yolo11Output> &outputs,
    const DecodeConfig &cfg,
    float lb_scale, int lb_pad_x, int lb_pad_y,
    int orig_w, int orig_h,
    std::vector<Detection> &results,
    float *best_confidence,
    float *class_prob)
{
    results.clear();
    if (best_confidence) {
        *best_confidence = 0.0f;
    }
    if (class_prob) {
        for (int c = 0; c < cfg.num_classes; c++) {
            class_prob[c] = 0.0f;
        }
    }

    constexpr float NMS_THRESHOLD = 0.70f;
    constexpr int MAX_DETECTIONS = 10;
    std::array<ESPDetScale, 3> scales = {};

    for (const Yolo11Output &out : outputs) {
        int c = tensor_c(out);
        if (c != 4 && c != 32 && c != cfg.num_classes) continue;

        int h = tensor_h(out, c);
        int w = tensor_w(out, c);
        if (h <= 0 || w <= 0) continue;

        ESPDetScale *slot = nullptr;
        for (auto &s : scales) {
            if ((s.height == h && s.width == w) || s.height == 0) {
                slot = &s;
                if (s.height == 0) {
                    s.height = h;
                    s.width = w;
                }
                break;
            }
        }
        if (!slot) continue;

        if (c == 4 || c == 32) {
            slot->box = &out;
            slot->box_channels = c;
        } else {
            slot->score = &out;
        }
    }

    static std::vector<Detection> candidates;
    candidates.clear();
    if (candidates.capacity() < 32) {
        candidates.reserve(32);
    }

    float espdet_logit_thr = logf(cfg.conf_threshold / (1.0f - cfg.conf_threshold));
    float pico_score_thr = cfg.conf_threshold * cfg.conf_threshold;
    float best_score = 0.0f;
    float best_raw_seen = -1e9f;
    int best_cls_seen = 0;
    int best_x_seen = 0;
    int best_y_seen = 0;
    int best_w_seen = 0;
    int best_h_seen = 0;

    for (const auto &s : scales) {
        if (!s.box || !s.score) {
            if (s.height != 0) {
                ESP_LOGW(TAG_DEC, "Incomplete ESPDet output pair for %dx%d", s.width, s.height);
            }
            continue;
        }

        int stride_x = cfg.input_w / s.width;
        int stride_y = cfg.input_h / s.height;
        if (stride_x <= 0 || stride_y <= 0) continue;

        for (int y = 0; y < s.height; y++) {
            for (int x = 0; x < s.width; x++) {
                int best_cls = 0;
                float best_raw = -1e9f;
                for (int c = 0; c < cfg.num_classes; c++) {
                    float raw = tensor_value(*s.score, cfg.num_classes, s.height, s.width, y, x, c);
                    if (class_prob) {
                        float class_score = (s.box_channels == 32)
                            ? sqrtf(std::max(0.0f, raw))
                            : sigmoid(raw);
                        if (class_score > class_prob[c]) {
                            class_prob[c] = class_score;
                        }
                    }
                    if (raw > best_raw) {
                        best_raw = raw;
                        best_cls = c;
                    }
                }

                float score = 0.0f;
                if (s.box_channels == 32) {
                    score = sqrtf(std::max(0.0f, best_raw));
                    if (score > best_score) {
                        best_score = score;
                        best_raw_seen = best_raw;
                        best_cls_seen = best_cls;
                        best_x_seen = x;
                        best_y_seen = y;
                        best_w_seen = s.width;
                        best_h_seen = s.height;
                    }
                    if (best_raw < pico_score_thr) continue;
                } else {
                    score = sigmoid(best_raw);
                    if (score > best_score) {
                        best_score = score;
                        best_raw_seen = best_raw;
                        best_cls_seen = best_cls;
                        best_x_seen = x;
                        best_y_seen = y;
                        best_w_seen = s.width;
                        best_h_seen = s.height;
                    }
                    if (best_raw < espdet_logit_thr) continue;
                }

                float left = 0.0f;
                float top = 0.0f;
                float right = 0.0f;
                float bottom = 0.0f;
                if (s.box_channels == 32) {
                    left = pico_dfl(*s.box, s.height, s.width, y, x, 0);
                    top = pico_dfl(*s.box, s.height, s.width, y, x, 1);
                    right = pico_dfl(*s.box, s.height, s.width, y, x, 2);
                    bottom = pico_dfl(*s.box, s.height, s.width, y, x, 3);
                } else {
                    left = tensor_value(*s.box, 4, s.height, s.width, y, x, 0);
                    top = tensor_value(*s.box, 4, s.height, s.width, y, x, 1);
                    right = tensor_value(*s.box, 4, s.height, s.width, y, x, 2);
                    bottom = tensor_value(*s.box, 4, s.height, s.width, y, x, 3);
                }

                float cx = (float)(x * stride_x + stride_x / 2);
                float cy = (float)(y * stride_y + stride_y / 2);
                float x1_in = cx - left * (float)stride_x;
                float y1_in = cy - top * (float)stride_y;
                float x2_in = cx + right * (float)stride_x;
                float y2_in = cy + bottom * (float)stride_y;

                float x1 = (x1_in - lb_pad_x) / lb_scale;
                float y1 = (y1_in - lb_pad_y) / lb_scale;
                float x2 = (x2_in - lb_pad_x) / lb_scale;
                float y2 = (y2_in - lb_pad_y) / lb_scale;

                x1 = std::max(0.0f, std::min(x1, (float)orig_w));
                y1 = std::max(0.0f, std::min(y1, (float)orig_h));
                x2 = std::max(0.0f, std::min(x2, (float)orig_w));
                y2 = std::max(0.0f, std::min(y2, (float)orig_h));
                if (x2 <= x1 || y2 <= y1) continue;

                Detection det;
                det.x1 = x1;
                det.y1 = y1;
                det.x2 = x2;
                det.y2 = y2;
                det.confidence = score;
                det.class_id = best_cls;
                candidates.push_back(det);
            }
        }
    }
    if (best_confidence) {
        *best_confidence = best_score;
    }

    std::sort(candidates.begin(), candidates.end(),
              [](const Detection &a, const Detection &b) {
                  return a.confidence > b.confidence;
              });

    for (const Detection &candidate : candidates) {
        bool suppressed = false;
        for (const Detection &kept : results) {
            if (candidate.class_id == kept.class_id &&
                iou(candidate, kept) > NMS_THRESHOLD) {
                suppressed = true;
                break;
            }
        }
        if (!suppressed) {
            results.push_back(candidate);
            if ((int)results.size() >= MAX_DETECTIONS) break;
        }
    }

    s_espdet_decode_count++;
    if (s_espdet_decode_count <= 5 || (s_espdet_decode_count % 30) == 0) {
        // 暂时屏蔽
        // ESP_LOGI(TAG_DEC, "ESPDet decode #%lu candidates=%d kept=%d best=%.3f raw=%.3f cls=%d at=%dx%d/%dx%d",
        //          (unsigned long)s_espdet_decode_count,
        //          (int)candidates.size(), (int)results.size(),
        //          best_score, best_raw_seen, best_cls_seen,
        //          best_x_seen, best_y_seen, best_w_seen, best_h_seen);
    }
}

void yolo_decode_yolo11(
    const std::vector<Yolo11Output> &outputs,
    const DecodeConfig &cfg,
    float lb_scale, int lb_pad_x, int lb_pad_y,
    int orig_w, int orig_h,
    std::vector<Detection> &results)
{
    results.clear();

    constexpr int BBOX_CHANNELS = 64;
    constexpr float NMS_THRESHOLD = 0.70f;
    constexpr int MAX_DETECTIONS = 20;
    std::array<Yolo11Scale, 3> scales = {};

    for (const Yolo11Output &out : outputs) {
        int c = output_channel_count(out);
        int h = output_height(out);
        int w = output_width(out);
        if (h <= 0 || w <= 0) continue;

        Yolo11Scale *slot = nullptr;
        for (auto &s : scales) {
            if ((s.height == h && s.width == w) || s.height == 0) {
                slot = &s;
                if (s.height == 0) {
                    s.height = h;
                    s.width = w;
                }
                break;
            }
        }
        if (!slot) continue;

        if (c == BBOX_CHANNELS) {
            slot->bbox = &out;
        } else if (c == cfg.num_classes) {
            slot->cls = &out;
        }
    }

    std::vector<Detection> candidates;
    candidates.reserve(64);

    for (const auto &s : scales) {
        if (!s.bbox || !s.cls) {
            if (s.height != 0) {
                ESP_LOGW(TAG_DEC, "Incomplete YOLO11 output pair for %dx%d", s.width, s.height);
            }
            continue;
        }

        int stride_x = cfg.input_w / s.width;
        int stride_y = cfg.input_h / s.height;
        if (stride_x <= 0 || stride_y <= 0) continue;

        float cls_scale = dl_scale(s.cls->exponent);
        const int8_t *cls_data = s.cls->data;
        bool cls_nhwc = is_nhwc(*s.cls);
        float score_logit_thr = logf(cfg.conf_threshold / (1.0f - cfg.conf_threshold));

        for (int y = 0; y < s.height; y++) {
            for (int x = 0; x < s.width; x++) {
                int best_cls = 0;
                float best_logit = -1e9f;
                for (int c = 0; c < cfg.num_classes; c++) {
                    int idx = cls_nhwc ? ((y * s.width + x) * cfg.num_classes + c)
                                       : ((c * s.height + y) * s.width + x);
                    float logit = (float)cls_data[idx] * cls_scale;
                    if (logit > best_logit) {
                        best_logit = logit;
                        best_cls = c;
                    }
                }
                if (best_logit < score_logit_thr) continue;
                float best_score = sigmoid(best_logit);

                float left = dfl_expectation(*s.bbox, s.height, s.width, 0, y, x);
                float top = dfl_expectation(*s.bbox, s.height, s.width, 1, y, x);
                float right = dfl_expectation(*s.bbox, s.height, s.width, 2, y, x);
                float bottom = dfl_expectation(*s.bbox, s.height, s.width, 3, y, x);

                float cx = ((float)x + 0.5f) * (float)stride_x;
                float cy = ((float)y + 0.5f) * (float)stride_y;
                float x1_in = cx - left * (float)stride_x;
                float y1_in = cy - top * (float)stride_y;
                float x2_in = cx + right * (float)stride_x;
                float y2_in = cy + bottom * (float)stride_y;

                float x1 = (x1_in - lb_pad_x) / lb_scale;
                float y1 = (y1_in - lb_pad_y) / lb_scale;
                float x2 = (x2_in - lb_pad_x) / lb_scale;
                float y2 = (y2_in - lb_pad_y) / lb_scale;

                x1 = std::max(0.0f, std::min(x1, (float)orig_w));
                y1 = std::max(0.0f, std::min(y1, (float)orig_h));
                x2 = std::max(0.0f, std::min(x2, (float)orig_w));
                y2 = std::max(0.0f, std::min(y2, (float)orig_h));
                if (x2 <= x1 || y2 <= y1) continue;

                Detection det;
                det.x1 = x1;
                det.y1 = y1;
                det.x2 = x2;
                det.y2 = y2;
                det.confidence = best_score;
                det.class_id = best_cls;
                candidates.push_back(det);
            }
        }
    }

    std::sort(candidates.begin(), candidates.end(),
              [](const Detection &a, const Detection &b) {
                  return a.confidence > b.confidence;
              });

    for (const Detection &candidate : candidates) {
        bool suppressed = false;
        for (const Detection &kept : results) {
            if (candidate.class_id == kept.class_id &&
                iou(candidate, kept) > NMS_THRESHOLD) {
                suppressed = true;
                break;
            }
        }
        if (!suppressed) {
            results.push_back(candidate);
            if ((int)results.size() >= MAX_DETECTIONS) break;
        }
    }

    ESP_LOGD(TAG_DEC, "YOLO11 candidates=%d, kept=%d",
             (int)candidates.size(), (int)results.size());
}

/*
 * YOLO 解码主函数
 *
 * 步骤：
 *   1. 解析 output0 shape，自动判断 is [C,N] or [N,C]
 *   2. 遍历所有 anchor，对每个 anchor：
 *      a. 找置信度最高的类别
 *      b. 若低于阈值则跳过
 *      c. 解析 cx,cy,w,h（模型输出，已在 letterbox 空间）
 *      d. 转 xyxy 坐标：x1 = cx - w/2, y1 = cy - h/2 ...
 *      e. 从 letterbox 空间映射回原始图像坐标
 *      f. 检查边界并 clamp
 *   3. 输出 Detection 列表（原始图像坐标，无 NMS）
 */
void yolo_decode_v26(
    const int8_t *data, int exponent,
    const std::vector<int> &shape,
    const DecodeConfig &cfg,
    float lb_scale, int lb_pad_x, int lb_pad_y,
    int orig_w, int orig_h,
    std::vector<Detection> &results)
{
    results.clear();

    /* 1. 解析 output 形状 — 自动检测 [C,N] 或 [N,C] 布局 -------
     *
     * YOLOv26n One2One 头输出单 tensor:
     *   C = 4(xywh) + num_classes
     *   N = num_anchors (取决于输入尺寸:
     *       640² → 8400, 320² → 2100)
     *
     * 判断逻辑：C (4 + classes) 总是远小于 N (anchors)，
     * 因此以较小维度为 C。
     */
    int num_channels = 0;
    int num_anchors = 0;
    bool transposed = false;  /* true = 布局为 [N, C] 而非 [C, N] */

    if (shape.size() == 3 && shape[0] == 1) {
        /* [1, C, N] 或 [1, N, C] */
        if (shape[1] < shape[2]) {
            num_channels = shape[1];
            num_anchors = shape[2];
        } else {
            num_channels = shape[2];
            num_anchors = shape[1];
            transposed = true;
        }
    } else if (shape.size() == 2) {
        /* [C, N] 或 [N, C] */
        if (shape[0] < shape[1]) {
            num_channels = shape[0];
            num_anchors = shape[1];
        } else {
            num_channels = shape[1];
            num_anchors = shape[0];
            transposed = true;
        }
    } else {
        ESP_LOGE(TAG_DEC, "Unsupported shape (ndim=%d)", (int)shape.size());
        return;
    }

    /* 实际类别数 = channels - 4 (xywh) */
    int nc = num_channels - 4;
    if (nc != cfg.num_classes) {
        ESP_LOGW(TAG_DEC, "num_classes mismatch: tensor has %d classes, config expects %d. Using tensor's.",
                 nc, cfg.num_classes);
    }
    if (nc <= 0) {
        ESP_LOGE(TAG_DEC, "Invalid channel count: %d", num_channels);
        return;
    }

    float scale = dl_scale(exponent);

    /* 索引辅助函数：兼容 [C,N] 和 [N,C] 两种布局 */
    auto get_val = [&](int ch, int anchor) -> float {
        int idx = transposed ? (anchor * num_channels + ch)
                             : (ch * num_anchors + anchor);
        return (float)data[idx] * scale;
    };

    /* 2. 主解码循环 -------------------------------------------- */
    int kept = 0;
    for (int i = 0; i < num_anchors; i++) {
        /* 2a. 找最佳类别 */
        int best_cls = 0;
        float max_conf = -1e9f;
        for (int c = 0; c < nc; c++) {
            float v = get_val(4 + c, i);
            if (v > max_conf) {
                max_conf = v;
                best_cls = c;
            }
        }

        /* 2b. 置信度过滤 */
        if (max_conf < cfg.conf_threshold) continue;

        /* 2c. 解码 bbox (cx, cy, w, h) — 模型空间像素值 */
        float cx = get_val(0, i);
        float cy = get_val(1, i);
        float w  = get_val(2, i);
        float h  = get_val(3, i);

        /* 2d. 转 xyxy 坐标 (letterbox 空间) */
        float x1_in = cx - w * 0.5f;
        float y1_in = cy - h * 0.5f;
        float x2_in = cx + w * 0.5f;
        float y2_in = cy + h * 0.5f;

        /* 2e. 从 letterbox 空间映射回原始图像坐标 */
        float x1 = (x1_in - lb_pad_x) / lb_scale;
        float y1 = (y1_in - lb_pad_y) / lb_scale;
        float x2 = (x2_in - lb_pad_x) / lb_scale;
        float y2 = (y2_in - lb_pad_y) / lb_scale;

        /* 2f. Clamp 到图像边界 */
        x1 = std::max(0.0f, std::min(x1, (float)orig_w));
        y1 = std::max(0.0f, std::min(y1, (float)orig_h));
        x2 = std::max(0.0f, std::min(x2, (float)orig_w));
        y2 = std::max(0.0f, std::min(y2, (float)orig_h));

        if (x2 <= x1 || y2 <= y1) continue;

        Detection det;
        det.x1 = x1;
        det.y1 = y1;
        det.x2 = x2;
        det.y2 = y2;
        det.confidence = max_conf;
        det.class_id = best_cls;
        results.push_back(det);
        kept++;
    }

    ESP_LOGI(TAG_DEC, "Anchors=%d, kept=%d (conf>=%.2f)",
             num_anchors, kept, cfg.conf_threshold);
}
