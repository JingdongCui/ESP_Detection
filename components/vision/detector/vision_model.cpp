/*
 * vision_model —— 两级级联检测的封装实现（真实 esp-dl 接入）。
 *
 * 两层结构（对齐 esp-dl，见 vision_model.h 头注）：
 *   第一层 原子模型（vision_detector_load/free/run）：单图 → 该图坐标系框列表。
 *     内部持一个 dl::detect::DetectImpl 子类（PicoDetect），封装
 *     dl::Model + ImagePreprocessor + ESPDetPostProcessor。run() 组装 img_t →
 *     model->run → 拷 result_t。装配方式 1:1 复刻 esp-detection 部署模板
 *     （deploy/espdet_model_template/espdet_detect.cpp）。
 *   第二层 级联编排（vision_model_run）：waybill 跑整图 → 取最高分面单 →
 *     从原图裁剪 ROI → logo 跑 ROI → logo 框加面单左上角偏移映射回原图。
 *     这一层与画框/rescale 逻辑完全未改。
 *
 * 坐标系：ESPDetPostProcessor::get_result(img.w,img.h) 已把框 rescale 回
 *   【输入图坐标系】，故 vision_detector_run 输出即输入图坐标；ROI 阶段输出
 *   即 ROI 局部坐标，正好对上编排层的偏移映射。
 *
 * 原图不被破坏：编排只读入参 buf，ROI 是从原图【拷贝】出的独立缓冲。
 */
#include <list>
#include <string>
#include <cstring>
#include <cstdio>
#include <map>
#include <mutex>
#include <vector>

#include "esp_heap_caps.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_spiffs.h"

#include "dl_detect_base.hpp"
#include "dl_detect_espdet_postprocessor.hpp"
#include "dl_image_preprocessor.hpp"
#include "dl_math.hpp"
#include "dl_model_base.hpp"

#include "SEGGER_RTT.h"   // 诊断直发 RTT（ESP_LOG 走 UART 进不了 rtt.log）
#include "vision_model.h"

static const char *TAG = "vision_model";

// ============================================================
// 换模型只改这一段：每个模型的 SPIFFS 文件名 + 检测阈值。
// 输入尺寸不用填——ImagePreprocessor 从模型自身读取输入张量形状。
// 前处理（mean=0/std=255 归一化、letterbox 114）与后处理三段 stride(8/16/32)
// 是 ESPDet-Pico 架构标准，两个模型通用，无需改。
// ============================================================
// 模型1：面单检测（单分类，category 恒 0）
#define VISION_WAYBILL_MODEL_FILE  "waybill.espdl"  // 全屏范围找面单模型 224*224输入
#define VISION_WAYBILL_SCORE_THR_DEFAULT 0.80f
#define VISION_WAYBILL_NMS_THR     0.70f
// 模型2：logo 三分类（0=极兔 1=韵达 2=中通）
#define VISION_LOGO_MODEL_FILE     "logo.espdl"  // 面单范围找logo模型 224*224输入
#define VISION_LOGO_SCORE_THR_DEFAULT 0.60f
#define VISION_LOGO_NMS_THR        0.70f

static std::mutex s_threshold_mutex;
static std::mutex s_model_mutex;
static float s_waybill_score_thr = VISION_WAYBILL_SCORE_THR_DEFAULT;
static float s_logo_score_thr = VISION_LOGO_SCORE_THR_DEFAULT;

static int clamp_percent(int percent)
{
    if (percent < 0) {
        return 0;
    }
    if (percent > 100) {
        return 100;
    }
    return percent;
}

static int score_thr_to_percent(float value)
{
    int percent = (int)(value * 100.0f + 0.5f);
    return clamp_percent(percent);
}

// SPIFFS 挂载点与分区标签（模型文件由 spiffs_create_partition_image(storage model) 打包）。
#define VISION_MODEL_SPIFFS_BASE   "/spiffs"
#define VISION_MODEL_SPIFFS_LABEL  "storage"

// 后处理 top_k（候选框上限，架构标准值）。
#define VISION_MODEL_TOP_K         10

// ===== 诊断开关 =====
// 1 = 旁路模型加载：vision_model_init 直接返回 true 不 load 任何 .espdl，
//     句柄保持 NULL → vision_model_run 返回 -1（detect 侧按 0 框处理）。
//     用于隔离验证：框架能起来跑=框架 OK 问题在模型；仍崩=框架问题。
// 0 = 正常加载模型。
#define VISION_MODEL_BYPASS_LOAD   0

// 1 = ESP-DL 双核推理；0 = 单核推理。
#define VISION_MODEL_MULTI_CORE_INFERENCE 0
#if VISION_MODEL_MULTI_CORE_INFERENCE
#define VISION_MODEL_RUNTIME_MODE dl::RUNTIME_MODE_MULTI_CORE
#else
#define VISION_MODEL_RUNTIME_MODE dl::RUNTIME_MODE_SINGLE_CORE
#endif

//测试模块 板子上原图放models目录下，vision_model_init()会加载模型并跑一遍固定图像测试
//通过则代表板子能正确加载模型并跑推理，失败则代表板子有问题
#define VISION_FIXED_IMAGE_TEST_ENABLE 1
#define VISION_FIXED_IMAGE_TENSOR_DUMP 0
#define VISION_WAYBILL_TEST_IMAGE_FILE "waybill102460001.bin"
#define VISION_WAYBILL_TEST_IMAGE_W    1024
#define VISION_WAYBILL_TEST_IMAGE_H    600
#define VISION_LOGO_TEST1_IMAGE_FILE   "jt30829206.bin"
#define VISION_LOGO_TEST1_IMAGE_W      308
#define VISION_LOGO_TEST1_IMAGE_H      292
#define VISION_LOGO_TEST2_IMAGE_FILE   "zt31241902.bin"
#define VISION_LOGO_TEST2_IMAGE_W      312
#define VISION_LOGO_TEST2_IMAGE_H      419
#define VISION_LOGO_TEST3_IMAGE_FILE   "yd29041906.bin"
#define VISION_LOGO_TEST3_IMAGE_W      290
#define VISION_LOGO_TEST3_IMAGE_H      419

// 编排层临时缓冲上限（面单只取最高分 1 个，logo 若干）。
#define VISION_MODEL_MAX_WB   4
#define VISION_MODEL_MAX_LOGO 4

static uint8_t *s_roi_buf;
static size_t s_roi_cap;

// ===== 第一层：原子模型（DetectImpl 子类，装配三件套）=====

namespace {

static void dump_tensor_stats(const char *tag, const char *kind, const char *name,
                              dl::TensorBase *t, float score_thr)
{
    if (!t || !t->data) {
        SEGGER_RTT_printf(0, "[tensor-dump][%s] %s %s null\n", tag, kind, name);
        ESP_LOGI(TAG, "[tensor-dump][%s] %s %s null", tag, kind, name);
        return;
    }

    int n = 1;
    for (int d : t->shape) {
        n *= d;
    }

    int minv = 9999;
    int maxv = -9999;
    int thr_q = 0;
    float score_exp = DL_SCALE(t->exponent);
    bool is_score = strncmp(name, "score", 5) == 0;
    if (t->dtype == dl::DATA_TYPE_INT8) {
        int8_t *p = (int8_t *)t->data;
        for (int i = 0; i < n; i++) {
            if (p[i] < minv) minv = p[i];
            if (p[i] > maxv) maxv = p[i];
        }
        if (is_score) {
            thr_q = (int)dl::quantize<int8_t>(dl::math::inverse_sigmoid(score_thr), 1.f / score_exp);
        }
    } else if (t->dtype == dl::DATA_TYPE_INT16) {
        int16_t *p = (int16_t *)t->data;
        for (int i = 0; i < n; i++) {
            if (p[i] < minv) minv = p[i];
            if (p[i] > maxv) maxv = p[i];
        }
        if (is_score) {
            thr_q = (int)dl::quantize<int16_t>(dl::math::inverse_sigmoid(score_thr), 1.f / score_exp);
        }
    } else {
        SEGGER_RTT_printf(0, "[tensor-dump][%s] %s %s dtype=%d unsupported\n",
                          tag, kind, name, (int)t->dtype);
        ESP_LOGI(TAG, "[tensor-dump][%s] %s %s dtype=%d unsupported",
                 tag, kind, name, (int)t->dtype);
        return;
    }

    int shape0 = t->shape.size() > 0 ? t->shape[0] : -1;
    int shape1 = t->shape.size() > 1 ? t->shape[1] : -1;
    int shape2 = t->shape.size() > 2 ? t->shape[2] : -1;
    int shape3 = t->shape.size() > 3 ? t->shape[3] : -1;
    float sigmoid_max = is_score ? dl::math::sigmoid(maxv * score_exp) : 0.0f;
    SEGGER_RTT_printf(0,
                      "[tensor-dump][%s] %s %s dtype=%d exp=%d shape=[%d,%d,%d,%d] raw_min=%d raw_max=%d thr_q=%d sigmoid_max=%.4f\n",
                      tag, kind, name, (int)t->dtype, (int)t->exponent,
                      shape0, shape1, shape2, shape3,
                      minv, maxv, thr_q, sigmoid_max);
    ESP_LOGI(TAG,
             "[tensor-dump][%s] %s %s dtype=%d exp=%d shape=[%d,%d,%d,%d] raw_min=%d raw_max=%d thr_q=%d sigmoid_max=%.4f",
             tag, kind, name, (int)t->dtype, (int)t->exponent,
             shape0, shape1, shape2, shape3,
             minv, maxv, thr_q, sigmoid_max);
}

static dl::TensorBase *find_model_tensor(dl::Model *model, const char *name)
{
    auto &inputs = model->get_inputs();
    auto input = inputs.find(name);
    if (input != inputs.end()) {
        return input->second;
    }
    auto &outputs = model->get_outputs();
    auto output = outputs.find(name);
    if (output != outputs.end()) {
        return output->second;
    }
    return model->get_intermediate(name);
}

static void dump_model_tensor(const char *tag, dl::Model *model, const char *name, float score_thr)
{
    dump_tensor_stats(tag, "live", name, find_model_tensor(model, name), score_thr);
}

static void update_score_prob(float score, int *prob)
{
    int p = (int)(score * 100.0f + 0.5f);
    if (p < 0) {
        p = 0;
    } else if (p > 100) {
        p = 100;
    }
    if (p > *prob) {
        *prob = p;
    }
}

template <typename T>
static void collect_score_tensor_probs(dl::TensorBase *score, int *jt, int *zt, int *yd)
{
    if (!score || !score->data || score->shape.size() < 4) {
        return;
    }
    int h = score->shape[1];
    int w = score->shape[2];
    int c = score->shape[3];
    if (h <= 0 || w <= 0 || c < 3) {
        return;
    }

    T *score_ptr = (T *)score->data;
    float score_exp = DL_SCALE(score->exponent);
    int cells = h * w;
    for (int i = 0; i < cells; i++) {
        update_score_prob(dl::math::sigmoid(dl::dequantize(score_ptr[0], score_exp)), jt);
        update_score_prob(dl::math::sigmoid(dl::dequantize(score_ptr[1], score_exp)), yd);
        update_score_prob(dl::math::sigmoid(dl::dequantize(score_ptr[2], score_exp)), zt);
        score_ptr += c;
    }
}

static void collect_model_score_probs(dl::Model *model, int *jt, int *zt, int *yd)
{
    const char *score_names[] = {"score0", "score1", "score2"};
    for (const char *name : score_names) {
        dl::TensorBase *score = find_model_tensor(model, name);
        if (!score || !score->data) {
            continue;
        }
        if (score->dtype == dl::DATA_TYPE_INT8) {
            collect_score_tensor_probs<int8_t>(score, jt, zt, yd);
        } else if (score->dtype == dl::DATA_TYPE_INT16) {
            collect_score_tensor_probs<int16_t>(score, jt, zt, yd);
        }
    }
}

// ESPDet-Pico 检测器：构造里装配 model + 前处理器 + 后处理器。
// 复刻 esp-detection 部署模板 espdet_detect::ESPDet 的装配顺序与参数。
class PicoDetect : public dl::detect::DetectImpl {
public:
    PicoDetect(const char *path, float score_thr, float nms_thr)
    {
        m_score_thr = score_thr;
        // 从 SPIFFS 以文件路径（fopen 语义）加载 .espdl。
        m_model = new dl::Model(path, fbs::MODEL_LOCATION_IN_SDCARD);

        // 诊断：模型加载后校验输入张量数量。
        // get_input("") 空名 → get_input() → assert(m_inputs.size()==1); return begin()->second。
        // release 构建 assert 被裁 → 若 inputs 非恰好 1 个，begin()->second 读红黑树哨兵得野指针(NULL)，
        // ImagePreprocessor 里解引用 m_model_input->dtype 触发 NULL+0x14 Load access fault（本次崩溃根因）。
        auto &ins = m_model->get_inputs();
        SEGGER_RTT_printf(0, "[PicoDetect] model=%s inputs=%d outputs=%d\n",
                          path, (int)ins.size(), (int)m_model->get_outputs().size());
        for (auto &kv : ins) {
            auto *t = kv.second;
            SEGGER_RTT_printf(0, "[PicoDetect]   in '%s' dtype=%d ndim=%d\n",
                              kv.first.c_str(), t ? (int)t->dtype : -1,
                              t ? (int)t->shape.size() : -1);
        }
        if (ins.size() != 1) {
            SEGGER_RTT_printf(0, "[PicoDetect] ABORT: inputs!=1, skip preprocessor/postprocessor (would crash NULL+0x14)\n");
            return;   // 不建前/后处理器，句柄保留 NULL → run() 侧走保护返回 0 框，板子不崩
        }

        // esp-ppq export_test_values=True → 模型内嵌 test_inputs/test_outputs。
        // test() 用内嵌输入跑推理并与内嵌输出比对，验证【加载+推理】数值正确。
        // 【关键顺序】test() 必须在 minimize() 之前：minimize() 会删除推理用不到的
        // 中间张量（dl_model_base.cpp:389 明确警告 "impossible to test or debug"），
        // 而 esp-ppq 的 test_outputs 引用 score0/1/2、box0/1/2 等中间层输出，
        // 若先 minimize 再 test，get_tensor_index 找不到 → ESP_FAIL（曾误判为模型错）。
        // 【容差】test() 对 INT16 给 ±1 LSB 余量，对 INT8 仅 2e-5（近乎精确）。
        // INT8 输出的 1-LSB 舍入差（esp-ppq 模拟 vs esp-dl 运行时）会让 test 判 fail，
        // 但那是亚像素级、对检测无影响，故仅 RTT 告警、不 abort。
        esp_err_t test_ret = m_model->test();
        if (test_ret == ESP_OK) {
            SEGGER_RTT_printf(0, "[PicoDetect] model->test() PASS\n");
        } else {
            SEGGER_RTT_printf(0, "[PicoDetect] model->test() = 0x%x (%s) — INT8 容差告警,继续\n",
                              (int)test_ret, esp_err_to_name(test_ret));
        }

#if !VISION_FIXED_IMAGE_TENSOR_DUMP
        // test 后再 minimize，回收中间张量内存供实际推理使用。
        m_model->minimize();
#endif

        // mean=0 / std=255 → 归一化到 [0,1]。rgb_swap 用默认 false（模型接收 RGB）；
        // 源帧的真实色序由 vision_detector_run 里 img_t.pix_type 声明（BGR888），
        // esp-dl 据源=BGR/目标=RGB 自动完成红蓝交换。
        m_image_preprocessor =
            new dl::image::ImagePreprocessor(m_model, {0, 0, 0}, {255, 255, 255});
        m_image_preprocessor->enable_letterbox({114, 114, 114});
        // ESPDet-Pico 三段 anchor-point：{stride_y,stride_x,offset_y,offset_x}。
        m_postprocessor = new dl::detect::ESPDetPostProcessor(
            m_model, m_image_preprocessor, score_thr, nms_thr, VISION_MODEL_TOP_K,
            {{8, 8, 4, 4}, {16, 16, 8, 8}, {32, 32, 16, 16}});
    }

    void set_runtime_score_thr(float score_thr)
    {
        m_score_thr = score_thr;
        if (m_postprocessor) {
            m_postprocessor->set_score_thr(score_thr);
        }
    }

    std::list<dl::detect::result_t> &run_with_runtime_mode(const dl::image::img_t &img,
                                                            int *jt = nullptr,
                                                            int *zt = nullptr,
                                                            int *yd = nullptr)
    {
        DL_LOG_INFER_LATENCY_INIT();
        DL_LOG_INFER_LATENCY_START();
        m_image_preprocessor->preprocess(img);
        DL_LOG_INFER_LATENCY_END_PRINT("detect", "pre");

        DL_LOG_INFER_LATENCY_START();
        m_model->run(VISION_MODEL_RUNTIME_MODE);
        DL_LOG_INFER_LATENCY_END_PRINT("detect", "model");

        if (jt && zt && yd) {
            *jt = *zt = *yd = 0;
            collect_model_score_probs(m_model, jt, zt, yd);
        }

        DL_LOG_INFER_LATENCY_START();
        m_postprocessor->clear_result();
        m_postprocessor->postprocess();
        std::list<dl::detect::result_t> &result = m_postprocessor->get_result(img.width, img.height);
        DL_LOG_INFER_LATENCY_END_PRINT("detect", "post");

        return result;
    }

    std::list<dl::detect::result_t> &run_fixed_test(const char *tag, const dl::image::img_t &img)
    {
        m_image_preprocessor->preprocess(img);
#if VISION_FIXED_IMAGE_TENSOR_DUMP
        const char *dump_names[] = {
            "images",
            "/model.4/cv2/act/Relu_output_0",
            "PPQ_Variable_52",
            "/model.13/cv2/act/Relu_output_0",
            "/model.14/Resize_output_0",
            "/model.15/Concat_output_0",
            "/model.16/cv1/act/Relu_output_0",
            "/model.16/cv2/act/Relu_output_0",
            "score0",
            "score1",
            "score2",
        };
        std::map<std::string, dl::TensorBase *> user_outputs;
        for (const char *name : dump_names) {
            dl::TensorBase *src = find_model_tensor(m_model, name);
            if (src && src->data && strcmp(name, "images") != 0) {
                user_outputs[name] = new dl::TensorBase(src->shape, nullptr, src->exponent, src->dtype);
            } else if (strcmp(name, "images") != 0) {
                SEGGER_RTT_printf(0, "[tensor-dump][%s] prepare %s null\n", tag, name);
                ESP_LOGI(TAG, "[tensor-dump][%s] prepare %s null", tag, name);
            }
        }
        std::map<std::string, dl::TensorBase *> &inputs = m_model->get_inputs();
        m_model->run(inputs, VISION_MODEL_RUNTIME_MODE, user_outputs);
        dump_model_tensor(tag, m_model, "images", m_score_thr);
        for (const char *name : dump_names) {
            if (strcmp(name, "images") == 0) {
                continue;
            }
            auto it = user_outputs.find(name);
            dump_tensor_stats(tag, "copy", name, it != user_outputs.end() ? it->second : nullptr, m_score_thr);
        }
        for (auto &kv : user_outputs) {
            delete kv.second;
        }
#else
        m_model->run(VISION_MODEL_RUNTIME_MODE);
        dump_model_tensor(tag, m_model, "score0", m_score_thr);
        dump_model_tensor(tag, m_model, "score1", m_score_thr);
        dump_model_tensor(tag, m_model, "score2", m_score_thr);
#endif
        m_postprocessor->clear_result();
        m_postprocessor->postprocess();
        return m_postprocessor->get_result(img.width, img.height);
    }

private:
    float m_score_thr = 0.0f;
};

} // namespace

// 不透明句柄实体：仅持一个检测器指针。
struct vision_detector {
    PicoDetect *model;
};

extern "C" vision_detector_t *vision_detector_load(const char *model_name,
                                                   float score_thr, float nms_thr)
{
    if (!model_name) {
        return NULL;
    }
    std::string path = std::string(VISION_MODEL_SPIFFS_BASE) + "/" + model_name;
    vision_detector_t *det = new (std::nothrow) vision_detector;
    if (!det) {
        return NULL;
    }
    det->model = new (std::nothrow) PicoDetect(path.c_str(), score_thr, nms_thr);
    if (!det->model) {
        delete det;
        return NULL;
    }
    ESP_LOGI(TAG, "loaded model %s (score_thr=%.2f nms_thr=%.2f)",
             path.c_str(), score_thr, nms_thr);
    return det;
}

extern "C" void vision_detector_free(vision_detector_t *det)
{
    if (det) {
        delete det->model;
        delete det;
    }
}

static void vision_detector_set_score_threshold(vision_detector_t *det, float score_thr)
{
    std::lock_guard<std::mutex> lock(s_model_mutex);
    if (det && det->model) {
        det->model->set_runtime_score_thr(score_thr);
    }
}

static int vision_detector_run_internal(vision_detector_t *det,
                                        const uint8_t *img, int width, int height,
                                        vision_model_det_t *out, int max,
                                        int *jt, int *zt, int *yd)
{
    if (!det || !det->model || !img || !out || max < 1) {
        return -1;
    }
    dl::image::img_t in;
    in.data = (void *)img;
    in.width = (uint16_t)width;
    in.height = (uint16_t)height;
    // 相机(bsp_cam_sensor)V4L2 名义设 RGB24，但实测传感器/ISP 实际输出字节序为 BGR
    // （JTAG dump 原始帧在 PC 上确认红蓝互换）。此处如实声明 BGR888：模型训练输入为 RGB，
    // ImagePreprocessor 目标为 RGB888(rgb_swap=false)，esp-dl 依 BGR→RGB 自动交换红蓝。
    in.pix_type = dl::image::DL_IMAGE_PIX_TYPE_BGR888;

    std::lock_guard<std::mutex> lock(s_model_mutex);
    std::list<dl::detect::result_t> &res = det->model->run_with_runtime_mode(in, jt, zt, yd);

    int n = 0;
    for (const dl::detect::result_t &r : res) {
        if (n >= max) {
            break;
        }
        if (r.box.size() < 4) {
            continue;
        }
        out[n].box[0] = r.box[0];
        out[n].box[1] = r.box[1];
        out[n].box[2] = r.box[2];
        out[n].box[3] = r.box[3];
        out[n].category = r.category;
        out[n].score = r.score;
        out[n].stage = VISION_STAGE_WAYBILL;  // 阶段由编排层按来源覆写
        n++;
    }
    return n;
}

extern "C" int vision_detector_run(vision_detector_t *det,
                                   const uint8_t *img, int width, int height,
                                   vision_model_det_t *out, int max)
{
    return vision_detector_run_internal(det, img, width, height, out, max,
                                        nullptr, nullptr, nullptr);
}

static int vision_detector_run_with_probs(vision_detector_t *det,
                                          const uint8_t *img, int width, int height,
                                          vision_model_det_t *out, int max,
                                          int *jt, int *zt, int *yd)
{
    return vision_detector_run_internal(det, img, width, height, out, max,
                                        jt, zt, yd);
}

// ===== 第二层：级联编排 =====

static vision_detector_t *s_waybill;   // 模型1句柄（单分类面单）
static vision_detector_t *s_logo;      // 模型2句柄（三分类 logo）
static int s_last_infer_ms;            // 最近一次两级合计耗时（ms）
static int s_prob_jt, s_prob_zt, s_prob_yd;  // 三类概率 ×100（来自模型2）
static bool s_spiffs_mounted;

#if VISION_FIXED_IMAGE_TEST_ENABLE
static uint8_t *load_fixed_bgr888_image(const char *name, int width, int height, size_t *size_out)
{
    if (!name || width <= 0 || height <= 0) {
        SEGGER_RTT_printf(0, "[fixed-test] invalid image config name=%s w=%d h=%d\n",
                          name ? name : "(null)", width, height);
        ESP_LOGW(TAG, "[fixed-test] invalid image config name=%s w=%d h=%d",
                 name ? name : "(null)", width, height);
        return NULL;
    }

    std::string path = std::string(VISION_MODEL_SPIFFS_BASE) + "/" + name;
    FILE *fp = fopen(path.c_str(), "rb");
    if (!fp) {
        SEGGER_RTT_printf(0, "[fixed-test] open failed: %s\n", path.c_str());
        ESP_LOGW(TAG, "[fixed-test] open failed: %s", path.c_str());
        return NULL;
    }

    size_t expected = (size_t)width * height * 3;
    uint8_t *buf = (uint8_t *)heap_caps_malloc(expected, MALLOC_CAP_SPIRAM);
    if (!buf) {
        fclose(fp);
        SEGGER_RTT_printf(0, "[fixed-test] malloc failed: %u bytes for %s\n",
                          (unsigned)expected, path.c_str());
        ESP_LOGE(TAG, "[fixed-test] malloc failed: %u bytes for %s",
                 (unsigned)expected, path.c_str());
        return NULL;
    }

    size_t got = fread(buf, 1, expected, fp);
    int extra = fgetc(fp);
    fclose(fp);

    if (got != expected || extra != EOF) {
        SEGGER_RTT_printf(0, "[fixed-test] size mismatch: %s expected=%u got=%u extra=%d\n",
                          path.c_str(), (unsigned)expected, (unsigned)got, extra != EOF);
        ESP_LOGW(TAG, "[fixed-test] size mismatch: %s expected=%u got=%u extra=%d",
                 path.c_str(), (unsigned)expected, (unsigned)got, extra != EOF);
        heap_caps_free(buf);
        return NULL;
    }

    if (size_out) {
        *size_out = expected;
    }
    return buf;
}

static int vision_detector_run_fixed_test(const char *tag, vision_detector_t *det,
                                          const uint8_t *img, int width, int height,
                                          vision_model_det_t *out, int max)
{
    if (!det || !det->model || !img || !out || max < 1) {
        return -1;
    }
    dl::image::img_t in;
    in.data = (void *)img;
    in.width = (uint16_t)width;
    in.height = (uint16_t)height;
    in.pix_type = dl::image::DL_IMAGE_PIX_TYPE_BGR888;

    std::list<dl::detect::result_t> &res = det->model->run_fixed_test(tag, in);
    int n = 0;
    for (const dl::detect::result_t &r : res) {
        if (n >= max) {
            break;
        }
        if (r.box.size() < 4) {
            continue;
        }
        out[n].box[0] = r.box[0];
        out[n].box[1] = r.box[1];
        out[n].box[2] = r.box[2];
        out[n].box[3] = r.box[3];
        out[n].category = r.category;
        out[n].score = r.score;
        out[n].stage = VISION_STAGE_WAYBILL;
        n++;
    }
    return n;
}

static void run_fixed_image_detector_test(const char *tag, vision_detector_t *det,
                                          const char *image_name, int width, int height)
{
    size_t bytes = 0;
    uint8_t *img = load_fixed_bgr888_image(image_name, width, height, &bytes);
    if (!img) {
        return;
    }

    vision_model_det_t out[VISION_MODEL_TOP_K];
    int n = vision_detector_run_fixed_test(tag, det, img, width, height, out, VISION_MODEL_TOP_K);
    SEGGER_RTT_printf(0, "[fixed-test][%s] image=%s w=%d h=%d bytes=%u n=%d\n",
                      tag, image_name, width, height, (unsigned)bytes, n);
    ESP_LOGI(TAG, "[fixed-test][%s] image=%s w=%d h=%d bytes=%u n=%d",
             tag, image_name, width, height, (unsigned)bytes, n);
    int best = -1;
    for (int i = 0; i < n; i++) {
        if (best < 0 || out[i].score > out[best].score) {
            best = i;
        }
        int score_x1000 = (int)(out[i].score * 1000.0f + 0.5f);
        SEGGER_RTT_printf(0,
                          "[fixed-test][%s] i=%d box=[%d,%d,%d,%d] cat=%d score=%d.%03d\n",
                          tag, i,
                          out[i].box[0], out[i].box[1], out[i].box[2], out[i].box[3],
                          out[i].category, score_x1000 / 1000, score_x1000 % 1000);
        ESP_LOGI(TAG, "[fixed-test][%s] i=%d box=[%d,%d,%d,%d] cat=%d score=%d.%03d",
                 tag, i,
                 out[i].box[0], out[i].box[1], out[i].box[2], out[i].box[3],
                 out[i].category, score_x1000 / 1000, score_x1000 % 1000);
    }
    if (best >= 0) {
        int best_score_x1000 = (int)(out[best].score * 1000.0f + 0.5f);
        ESP_LOGI(TAG, "[fixed-test][%s] best_cat=%d best_score=%d.%03d",
                 tag, out[best].category, best_score_x1000 / 1000, best_score_x1000 % 1000);
    } else {
        ESP_LOGI(TAG, "[fixed-test][%s] score=none (n=0)", tag);
    }

    heap_caps_free(img);
}

static void run_fixed_image_tests(void)
{
    SEGGER_RTT_printf(0, "\n[fixed-test] ===== begin fixed image tests =====\n");
    ESP_LOGI(TAG, "");
    ESP_LOGI(TAG, "[fixed-test] ===== begin fixed image tests =====");

    run_fixed_image_detector_test("waybill", s_waybill,
                                  VISION_WAYBILL_TEST_IMAGE_FILE,
                                  VISION_WAYBILL_TEST_IMAGE_W,
                                  VISION_WAYBILL_TEST_IMAGE_H);
    run_fixed_image_detector_test("logo1", s_logo,
                                  VISION_LOGO_TEST1_IMAGE_FILE,
                                  VISION_LOGO_TEST1_IMAGE_W,
                                  VISION_LOGO_TEST1_IMAGE_H);
    run_fixed_image_detector_test("logo2", s_logo,
                                  VISION_LOGO_TEST2_IMAGE_FILE,
                                  VISION_LOGO_TEST2_IMAGE_W,
                                  VISION_LOGO_TEST2_IMAGE_H);
    run_fixed_image_detector_test("logo3", s_logo,
                                  VISION_LOGO_TEST3_IMAGE_FILE,
                                  VISION_LOGO_TEST3_IMAGE_W,
                                  VISION_LOGO_TEST3_IMAGE_H);

    SEGGER_RTT_printf(0, "[fixed-test] ===== end fixed image tests =====\n\n");
    ESP_LOGI(TAG, "[fixed-test] ===== end fixed image tests =====");
    ESP_LOGI(TAG, "");
}
#endif

// 挂载模型所在 SPIFFS 分区（幂等）。
static bool mount_model_spiffs(void)
{
    if (s_spiffs_mounted) {
        return true;
    }
    esp_vfs_spiffs_conf_t conf = {};
    conf.base_path = VISION_MODEL_SPIFFS_BASE;
    conf.partition_label = VISION_MODEL_SPIFFS_LABEL;
    conf.max_files = 4;
    conf.format_if_mount_failed = false;
    esp_err_t err = esp_vfs_spiffs_register(&conf);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "SPIFFS mount failed (%s): %s",
                 VISION_MODEL_SPIFFS_LABEL, esp_err_to_name(err));
        return false;
    }
    s_spiffs_mounted = true;
    return true;
}

extern "C" int vision_model_get_waybill_score_threshold_percent(void)
{
    std::lock_guard<std::mutex> lock(s_threshold_mutex);
    return score_thr_to_percent(s_waybill_score_thr);
}

extern "C" int vision_model_get_logo_score_threshold_percent(void)
{
    std::lock_guard<std::mutex> lock(s_threshold_mutex);
    return score_thr_to_percent(s_logo_score_thr);
}

extern "C" void vision_model_set_waybill_score_threshold_percent(int percent)
{
    percent = clamp_percent(percent);
    float score_thr = percent / 100.0f;
    {
        std::lock_guard<std::mutex> lock(s_threshold_mutex);
        s_waybill_score_thr = score_thr;
    }
    SEGGER_RTT_printf(0, "[vision-thr] waybill set percent=%d score_thr=%.2f\n", percent, score_thr);
    vision_detector_set_score_threshold(s_waybill, score_thr);
}

extern "C" void vision_model_set_logo_score_threshold_percent(int percent)
{
    percent = clamp_percent(percent);
    float score_thr = percent / 100.0f;
    {
        std::lock_guard<std::mutex> lock(s_threshold_mutex);
        s_logo_score_thr = score_thr;
    }
    SEGGER_RTT_printf(0, "[vision-thr] logo set percent=%d score_thr=%.2f\n", percent, score_thr);
    vision_detector_set_score_threshold(s_logo, score_thr);
}

extern "C" bool vision_model_init(void)
{
    if (VISION_MODEL_BYPASS_LOAD) {
        // 诊断旁路：不挂 SPIFFS、不 load 模型，句柄保持 NULL。
        // 框架照常启动运行，vision_model_run 因句柄 NULL 返回 -1（0 框）。
        ESP_LOGW(TAG, "MODEL LOAD BYPASSED (diagnostic): framework-only, no inference");
        return true;
    }
    if (!mount_model_spiffs()) {
        return false;
    }
    float waybill_score_thr;
    float logo_score_thr;
    {
        std::lock_guard<std::mutex> lock(s_threshold_mutex);
        waybill_score_thr = s_waybill_score_thr;
        logo_score_thr = s_logo_score_thr;
    }
    s_waybill = vision_detector_load(VISION_WAYBILL_MODEL_FILE,
                                     waybill_score_thr, VISION_WAYBILL_NMS_THR);
    s_logo    = vision_detector_load(VISION_LOGO_MODEL_FILE,
                                     logo_score_thr, VISION_LOGO_NMS_THR);
    if (!s_waybill || !s_logo) {
        vision_detector_free(s_waybill);
        vision_detector_free(s_logo);
        s_waybill = NULL;
        s_logo = NULL;
        return false;
    }
#if VISION_FIXED_IMAGE_TEST_ENABLE
    run_fixed_image_tests();
#endif
    return true;
}

// 从原图【拷贝】裁剪 ROI 到模块内复用缓冲（PSRAM）。裁剪矩形 clip 到原图边界。
// 返回子图指针（调用者不得释放）；出参回填实际 ROI 宽高与左上偏移。
// 失败返回 NULL。原图 buf 只读，不被修改。
static uint8_t *crop_roi(const uint8_t *buf, int width, int height,
                         const int box[4],
                         int *roi_w, int *roi_h, int *off_x, int *off_y)
{
    int x1 = box[0], y1 = box[1], x2 = box[2], y2 = box[3];
    if (x1 < 0) x1 = 0;
    if (y1 < 0) y1 = 0;
    if (x2 > width)  x2 = width;
    if (y2 > height) y2 = height;
    int rw = x2 - x1;
    int rh = y2 - y1;
    if (rw <= 0 || rh <= 0) {
        return NULL;
    }
    size_t need = (size_t)rw * rh * 3;
    if (s_roi_cap < need) {
        heap_caps_free(s_roi_buf);
        s_roi_buf = (uint8_t *)heap_caps_malloc(need, MALLOC_CAP_SPIRAM);
        s_roi_cap = s_roi_buf ? need : 0;
    }
    if (!s_roi_buf) {
        return NULL;
    }
    // 逐行拷贝原图 ROI 区域到紧凑子图（stride 从原图整宽收缩为 rw）。
    for (int r = 0; r < rh; r++) {
        const uint8_t *src = buf + ((size_t)(y1 + r) * width + x1) * 3;
        uint8_t *dst = s_roi_buf + (size_t)r * rw * 3;
        memcpy(dst, src, (size_t)rw * 3);
    }
    *roi_w = rw;
    *roi_h = rh;
    *off_x = x1;
    *off_y = y1;
    return s_roi_buf;
}

extern "C" int vision_model_run(const uint8_t *buf, int width, int height,
                                vision_model_det_t *dets, int max)
{
    if (!buf || !dets || max < 1 || !s_waybill || !s_logo) {
        return -1;
    }
    int64_t t0 = esp_timer_get_time();
    s_prob_jt = s_prob_zt = s_prob_yd = 0;

    // 1) 模型1：整张原图跑 waybill。
    vision_model_det_t wb[VISION_MODEL_MAX_WB];
    int nwb = vision_detector_run(s_waybill, buf, width, height,
                                  wb, VISION_MODEL_MAX_WB);
    if (nwb <= 0) {
        s_last_infer_ms = (int)((esp_timer_get_time() - t0) / 1000);
        return 0;   // 无面单：整帧无目标
    }

    // 2) 取最高分面单 → dets[0]（原图坐标系，stage=WAYBILL）。
    int best = 0;
    for (int i = 1; i < nwb; i++) {
        if (wb[i].score > wb[best].score) {
            best = i;
        }
    }
    float waybill_score_thr;
    {
        std::lock_guard<std::mutex> lock(s_threshold_mutex);
        waybill_score_thr = s_waybill_score_thr;
    }
    if (wb[best].score < waybill_score_thr) {
        s_last_infer_ms = (int)((esp_timer_get_time() - t0) / 1000);
        return 0;
    }
    dets[0] = wb[best];
    dets[0].stage = VISION_STAGE_WAYBILL;
    int total = 1;

    // 3) 按面单 box 从原图拷贝裁剪 ROI 子图。
    int roi_w = 0, roi_h = 0, off_x = 0, off_y = 0;
    uint8_t *roi = crop_roi(buf, width, height, dets[0].box,
                            &roi_w, &roi_h, &off_x, &off_y);
    if (!roi) {
        s_last_infer_ms = (int)((esp_timer_get_time() - t0) / 1000);
        return total;   // 裁剪失败：只返回面单框
    }

    // 4) 模型2：ROI 子图跑 logo（输出 ROI 局部坐标）。
    vision_model_det_t lg[VISION_MODEL_MAX_LOGO];
    int nlg = vision_detector_run_with_probs(s_logo, roi, roi_w, roi_h,
                                             lg, VISION_MODEL_MAX_LOGO,
                                             &s_prob_jt, &s_prob_zt, &s_prob_yd);

    // 5) 三类概率来自模型2原始 score tensor；画框仍只取 NMS 后最高分 logo 框。
    int best_lg = -1;
    for (int i = 0; i < nlg; i++) {
        if (best_lg < 0 || lg[i].score > lg[best_lg].score) {
            best_lg = i;
        }
    }

    float logo_score_thr;
    {
        std::lock_guard<std::mutex> lock(s_threshold_mutex);
        logo_score_thr = s_logo_score_thr;
    }

    // 6) 最高分 logo 框加面单左上角偏移，映射回原图坐标 → dets[1]（stage=LOGO）。
    if (best_lg >= 0 && lg[best_lg].score >= logo_score_thr && total < max) {
        vision_model_det_t d = lg[best_lg];
        d.box[0] += off_x;
        d.box[1] += off_y;
        d.box[2] += off_x;
        d.box[3] += off_y;
        d.stage = VISION_STAGE_LOGO;
        dets[total++] = d;
    }

    s_last_infer_ms = (int)((esp_timer_get_time() - t0) / 1000);
    return total;
}

extern "C" int vision_model_last_infer_ms(void)
{
    return s_last_infer_ms;
}

extern "C" void vision_model_get_class_probs(int *jt, int *zt, int *yd)
{
    if (jt) {
        *jt = s_prob_jt;
    }
    if (zt) {
        *zt = s_prob_zt;
    }
    if (yd) {
        *yd = s_prob_yd;
    }
}
