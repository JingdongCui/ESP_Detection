#include "app_yolo.hpp"
#include "roi_algorithm.h"
#include "roi_tuning.h"
#include "yolo_decode.hpp"

#include "esp_log.h"
#include "esp_cache.h"
#include "esp_heap_caps.h"
#include "esp_timer.h"
#include "esp_private/esp_cache_private.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/ppa.h"
#include "dl_model_base.hpp"
#include "dl_tensor_base.hpp"
#include "fbs_model.hpp"

#include <cstring>
#include <cstdlib>
#include <cmath>
#include <string>
#include <algorithm>
#include <new>
#include <sys/stat.h>

static const char *TAG = "app_yolo";

struct AppYOLO::Impl {
    YOLOConfig cfg;
    dl::Model *model = nullptr;
    int8_t *input_buf = nullptr;
    roi_algorithm_config_t roi_cfg = {};
    roi_algorithm_workspace_t roi_work = {};
    uint8_t *ppa_resize_buf = nullptr;
    size_t ppa_resize_buf_capacity = 0;
    size_t ppa_cache_line_size = 0;
    ppa_client_handle_t ppa = nullptr;
    std::vector<Yolo11Output> outputs;
    YOLOPerfStats last_perf;
    bool use_multi_core = true;
    bool use_ppa_preprocess = false;
    bool ppa_fallback_logged = false;
    bool ppa_scale_logged = false;
    int input_exp = 0;
    int8_t quant_lut[256] = {};
    uint32_t detect_count = 0;
    bool fallback_triggered = false;
};

struct LetterboxInfo {
    float scale;
    int pad_x;
    int pad_y;
};

struct PreprocessRegion {
    int x;
    int y;
    int w;
    int h;
};

static inline size_t align_up_size(size_t value, size_t align)
{
    return (value + align - 1) & ~(align - 1);
}

static inline int8_t quantize_u8_with_exp(uint8_t pixel, int input_exp)
{
    int q = 0;
    if (input_exp < 0 && input_exp >= -15) {
        int multiplier = 1 << (-input_exp);
        q = ((int)pixel * multiplier + 127) / 255;
    } else {
        float input_scale = ldexpf(1.0f, input_exp);
        q = (int)roundf(((float)pixel / 255.0f) / input_scale);
    }
    if (q > 127) q = 127;
    if (q < -128) q = -128;
    return (int8_t)q;
}

static void build_quant_lut(int input_exp, int8_t lut[256])
{
    for (int i = 0; i < 256; i++) {
        lut[i] = quantize_u8_with_exp((uint8_t)i, input_exp);
    }
}

static float ppa_supported_scale(float requested)
{
    // ESP32-P4 PPA SRM scale uses 8-bit integer + 4-bit fractional precision.
    if (requested < 0.0625f || requested >= 256.0f) {
        return -1.0f;
    }
    float scale_int = 0.0f;
    float scale_frac = modff(requested, &scale_int);
    scale_frac = floorf(scale_frac / 0.0625f) * 0.0625f;
    return scale_int + scale_frac;
}

AppYOLO::AppYOLO(const YOLOConfig &cfg)
{
    impl_ = new Impl();
    impl_->cfg = cfg;
}

AppYOLO::~AppYOLO()
{
    if (impl_->model) delete impl_->model;
    if (impl_->input_buf) heap_caps_free(impl_->input_buf);
    if (impl_->roi_work.mask) heap_caps_free(impl_->roi_work.mask);
    if (impl_->roi_work.tmp) heap_caps_free(impl_->roi_work.tmp);
    if (impl_->roi_work.queue) heap_caps_free(impl_->roi_work.queue);
    if (impl_->ppa_resize_buf) heap_caps_free(impl_->ppa_resize_buf);
    if (impl_->ppa) ppa_unregister_client(impl_->ppa);
    delete impl_;
}

bool AppYOLO::load_model(const char *model_path)
{
    ESP_LOGI(TAG, "Loading model from: %s", model_path);

    struct stat st;
    if (stat(model_path, &st) != 0) {
        ESP_LOGE(TAG, "Model file not found: %s", model_path);
        return false;
    }
    ESP_LOGI(TAG, "Model file size: %ld bytes", (long)st.st_size);

    // fbs::MODEL_LOCATION_IN_SDCARD: 从 SPIFFS/SD 卡加载 .espdl 大文件
    impl_->model = new(std::nothrow) dl::Model(model_path, fbs::MODEL_LOCATION_IN_SDCARD);
    if (!impl_->model) {
        ESP_LOGE(TAG, "Failed to allocate model object (OOM)");
        return false;
    }

    auto &inputs = impl_->model->get_inputs();
    auto &outputs = impl_->model->get_outputs();
    if (inputs.empty() || outputs.empty()) {
        ESP_LOGE(TAG, "Model load invalid: inputs=%d outputs=%d",
                 (int)inputs.size(), (int)outputs.size());
        return false;
    }

    dl::TensorBase *input_tensor = inputs.begin()->second;
    impl_->input_exp = input_tensor->exponent.get();
    build_quant_lut(impl_->input_exp, impl_->quant_lut);

    // INT8 输入缓冲: CPU fallback 使用，PPA 预处理路径会直接写入模型 input tensor
    size_t input_size = impl_->cfg.input_w * impl_->cfg.input_h * 3;
    impl_->input_buf = (int8_t *)heap_caps_malloc(input_size, MALLOC_CAP_SPIRAM);
    if (!impl_->input_buf) {
        ESP_LOGE(TAG, "Failed to allocate input buffer (%d bytes)", (int)input_size);
        return false;
    }

    impl_->roi_cfg = roi_algorithm_default_config();
    roi_tuning_set_defaults_from_config(&impl_->roi_cfg);
    impl_->roi_work.pixel_count =
        (size_t)impl_->roi_cfg.down_w * (size_t)impl_->roi_cfg.down_h;
    impl_->roi_work.mask = (uint8_t *)heap_caps_malloc(
        impl_->roi_work.pixel_count, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    impl_->roi_work.tmp = (uint8_t *)heap_caps_malloc(
        impl_->roi_work.pixel_count, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    impl_->roi_work.queue = (uint16_t *)heap_caps_malloc(
        impl_->roi_work.pixel_count * sizeof(uint16_t), MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    if (!impl_->roi_work.mask || !impl_->roi_work.tmp || !impl_->roi_work.queue) {
        ESP_LOGE(TAG, "Failed to allocate ROI workspace (%u pixels)",
                 (unsigned)impl_->roi_work.pixel_count);
        return false;
    }

    esp_err_t ppa_ready = esp_cache_get_alignment(MALLOC_CAP_SPIRAM, &impl_->ppa_cache_line_size);
    if (ppa_ready == ESP_OK) {
        impl_->ppa_resize_buf_capacity = align_up_size(input_size, impl_->ppa_cache_line_size);
        impl_->ppa_resize_buf = (uint8_t *)heap_caps_aligned_alloc(
            impl_->ppa_cache_line_size,
            impl_->ppa_resize_buf_capacity,
            MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
        if (!impl_->ppa_resize_buf) {
            ppa_ready = ESP_ERR_NO_MEM;
        }
    }
    if (ppa_ready == ESP_OK) {
        ppa_client_config_t ppa_cfg = {};
        ppa_cfg.oper_type = PPA_OPERATION_SRM;
        ppa_ready = ppa_register_client(&ppa_cfg, &impl_->ppa);
    }
    if (ppa_ready == ESP_OK) {
        impl_->use_ppa_preprocess = true;
        ESP_LOGI(TAG, "PPA preprocess ready: resize buffer=%u KB align=%uB",
                 (unsigned)(impl_->ppa_resize_buf_capacity / 1024),
                 (unsigned)impl_->ppa_cache_line_size);
    } else {
        if (impl_->ppa) {
            ppa_unregister_client(impl_->ppa);
            impl_->ppa = nullptr;
        }
        if (impl_->ppa_resize_buf) {
            heap_caps_free(impl_->ppa_resize_buf);
            impl_->ppa_resize_buf = nullptr;
        }
        impl_->ppa_resize_buf_capacity = 0;
        impl_->use_ppa_preprocess = false;
        ESP_LOGW(TAG, "PPA preprocess unavailable (%s), using CPU preprocess fallback",
                 esp_err_to_name(ppa_ready));
    }

    impl_->outputs.reserve(outputs.size());

    ESP_LOGI(TAG, "Model loaded. Input: %dx%dx3, runtime: multi-core with single-core fallback",
             impl_->cfg.input_w, impl_->cfg.input_h);

    // 一次性打印模型 I/O 信息（不在 detect() 热路径中重复输出）
    {
        int e = impl_->input_exp;
        ESP_LOGI(TAG, "Input tensor: exponent=%d  scale=%.6f", e, ldexpf(1.0f, e));
        ESP_LOGI(TAG, "Outputs (%d):", (int)outputs.size());
        for (auto &kv : outputs) {
            dl::TensorBase *t = kv.second;
            std::string s = "[";
            for (int i = 0; i < (int)t->shape.size(); i++) {
                s += std::to_string(t->shape[i]);
                if (i < (int)t->shape.size() - 1) s += ", ";
            }
            s += "]";
            ESP_LOGI(TAG, "  '%s' shape=%s exponent=%d",
                     kv.first.c_str(), s.c_str(), t->exponent.get());
        }
    }
    return true;
}

/*
 * Letterbox 预处理：双线性缩放 + 归一化 + INT8 量化
 *
 * 流程：
 *   1. 等比例缩放图像，保持宽高比，用 114/255 灰度填充空白区域
 *   2. RGB [0,255] → 归一化 [0,1] → INT8 量化 (× 1/input_scale)
 *
 * ESP-DL 量化公式: float_val = int8_val × 2^exponent
 * 因此 input_scale = 2^exponent
 */
static LetterboxInfo preprocess_letterbox(
    const uint8_t *src, int src_w, int src_h, int src_stride,
    const PreprocessRegion &region,
    int8_t *dst, int dst_w, int dst_h,
    float input_scale)
{
    float scale = std::min((float)dst_w / region.w, (float)dst_h / region.h);
    int new_w = (int)roundf(region.w * scale);
    int new_h = (int)roundf(region.h * scale);
    int pad_x = (dst_w - new_w) / 2;
    int pad_y = (dst_h - new_h) / 2;

    auto quantize = [&](float f) -> int8_t {
        int q = (int)roundf(f / input_scale);
        if (q > 127) q = 127;
        if (q < -128) q = -128;
        return (int8_t)q;
    };
    int8_t fill_q = quantize(114.0f / 255.0f);
    memset(dst, fill_q, dst_w * dst_h * 3);

    // 双线性插值缩放
    float fx_step = (float)region.w / new_w;
    float fy_step = (float)region.h / new_h;

    for (int y = 0; y < new_h; y++) {
        float fy = (y + 0.5f) * fy_step - 0.5f;
        int sy = (int)fy;
        float fy_frac = fy - sy;
        if (sy < 0) { sy = 0; fy_frac = 0; }
        if (sy >= region.h - 1) { sy = region.h - 2; fy_frac = 1.0f; }
        sy = std::max(0, std::min(region.y + sy, src_h - 1));

        int dst_row = y + pad_y;

        for (int x = 0; x < new_w; x++) {
            float fx = (x + 0.5f) * fx_step - 0.5f;
            int sx = (int)fx;
            float fx_frac = fx - sx;
            if (sx < 0) { sx = 0; fx_frac = 0; }
            if (sx >= region.w - 1) { sx = region.w - 2; fx_frac = 1.0f; }
            sx = std::max(0, std::min(region.x + sx, src_w - 1));

            int dst_col = x + pad_x;
            int dst_idx = (dst_row * dst_w + dst_col) * 3;

            for (int c = 0; c < 3; c++) {
                int sx1 = std::min(sx + 1, src_w - 1);
                int sy1 = std::min(sy + 1, src_h - 1);
                float p00 = src[sy * src_stride + sx * 3 + c];
                float p01 = src[sy * src_stride + sx1 * 3 + c];
                float p10 = src[sy1 * src_stride + sx * 3 + c];
                float p11 = src[sy1 * src_stride + sx1 * 3 + c];

                float val = p00 * (1 - fx_frac) * (1 - fy_frac)
                          + p01 * fx_frac * (1 - fy_frac)
                          + p10 * (1 - fx_frac) * fy_frac
                          + p11 * fx_frac * fy_frac;

                dst[dst_idx + c] = quantize(val / 255.0f);
            }
        }
    }

    return {scale, pad_x, pad_y};
}

static bool preprocess_letterbox_ppa(
    ppa_client_handle_t ppa,
    uint8_t *resize_buf,
    size_t resize_buf_capacity,
    size_t cache_line_size,
    const int8_t quant_lut[256],
    bool *fallback_logged,
    bool *scale_logged,
    const uint8_t *src, int src_w, int src_h,
    const PreprocessRegion &region,
    int8_t *dst, int dst_w, int dst_h,
    LetterboxInfo *lb,
    int64_t *ppa_resize_us,
    int64_t *quantize_us)
{
    if (!ppa || !resize_buf || resize_buf_capacity == 0 || cache_line_size == 0 ||
        !quant_lut || !src || !dst || src_w <= 0 || src_h <= 0 ||
        region.x < 0 || region.y < 0 || region.w <= 0 || region.h <= 0 ||
        region.x + region.w > src_w || region.y + region.h > src_h ||
        dst_w <= 0 || dst_h <= 0) {
        return false;
    }

    float requested_scale = std::min((float)dst_w / (float)region.w, (float)dst_h / (float)region.h);
    float scale = ppa_supported_scale(requested_scale);
    if (scale <= 0.0f) {
        return false;
    }
    int new_w = (int)((float)region.w * scale);
    int new_h = (int)((float)region.h * scale);
    int pad_x = (dst_w - new_w) / 2;
    int pad_y = (dst_h - new_h) / 2;
    if (new_w <= 0 || new_h <= 0 || pad_x < 0 || pad_y < 0 ||
        new_w > dst_w || new_h > dst_h) {
        return false;
    }
    if (scale_logged && !*scale_logged) {
        ESP_LOGI(TAG,
                 "PPA preprocess scale adjusted requested=%.5f actual=%.5f content=%dx%d pad=%d,%d",
                 (double)requested_scale,
                 (double)scale,
                 new_w, new_h,
                 pad_x, pad_y);
        *scale_logged = true;
    }

    size_t resize_size = (size_t)new_w * (size_t)new_h * 3;
    size_t resize_aligned_size = align_up_size(resize_size, cache_line_size);
    if (resize_aligned_size > resize_buf_capacity) {
        return false;
    }

    ppa_srm_oper_config_t srm = {};
    srm.in.buffer = src;
    srm.in.pic_w = (uint32_t)src_w;
    srm.in.pic_h = (uint32_t)src_h;
    srm.in.block_w = (uint32_t)region.w;
    srm.in.block_h = (uint32_t)region.h;
    srm.in.block_offset_x = (uint32_t)region.x;
    srm.in.block_offset_y = (uint32_t)region.y;
    srm.in.srm_cm = PPA_SRM_COLOR_MODE_RGB888;
    srm.out.buffer = resize_buf;
    srm.out.buffer_size = resize_aligned_size;
    srm.out.pic_w = (uint32_t)new_w;
    srm.out.pic_h = (uint32_t)new_h;
    srm.out.srm_cm = PPA_SRM_COLOR_MODE_RGB888;
    srm.rotation_angle = PPA_SRM_ROTATION_ANGLE_0;
    srm.scale_x = scale;
    srm.scale_y = scale;
    srm.rgb_swap = 0;
    srm.byte_swap = 0;
    srm.mode = PPA_TRANS_MODE_BLOCKING;

    int64_t t_resize_start = esp_timer_get_time();
    esp_err_t ret = ppa_do_scale_rotate_mirror(ppa, &srm);
    int64_t t_resize_end = esp_timer_get_time();
    if (ret != ESP_OK) {
        if (fallback_logged && !*fallback_logged) {
            ESP_LOGW(TAG, "PPA preprocess failed (%s), using CPU preprocess fallback",
                     esp_err_to_name(ret));
            *fallback_logged = true;
        }
        return false;
    }

    esp_cache_msync(resize_buf,
                    resize_aligned_size,
                    ESP_CACHE_MSYNC_FLAG_DIR_M2C | ESP_CACHE_MSYNC_FLAG_TYPE_DATA);

    int64_t t_quant_start = esp_timer_get_time();
    int8_t fill_q = quant_lut[114];
    memset(dst, fill_q, (size_t)dst_w * (size_t)dst_h * 3);

    const uint8_t *src_row = resize_buf;
    for (int y = 0; y < new_h; y++) {
        int8_t *dst_row = dst + (((y + pad_y) * dst_w + pad_x) * 3);
        for (int i = 0; i < new_w * 3; i++) {
            dst_row[i] = quant_lut[src_row[i]];
        }
        src_row += new_w * 3;
    }
    int64_t t_quant_end = esp_timer_get_time();

    if (lb) {
        *lb = {scale, pad_x, pad_y};
    }
    if (ppa_resize_us) {
        *ppa_resize_us = t_resize_end - t_resize_start;
    }
    if (quantize_us) {
        *quantize_us = t_quant_end - t_quant_start;
    }
    return true;
}

bool AppYOLO::detect(const uint8_t *rgb888, int src_w, int src_h,
                     std::vector<Detection> &results)
{
    int64_t t_start = esp_timer_get_time();
    results.clear();
    impl_->last_perf = {};
    if (!impl_->model || !impl_->input_buf) {
        ESP_LOGE(TAG, "Model not loaded");
        return false;
    }
    if (!rgb888 || src_w <= 0 || src_h <= 0 ||
        !impl_->roi_work.mask || !impl_->roi_work.tmp || !impl_->roi_work.queue) {
        ESP_LOGE(TAG, "Invalid detect input or ROI workspace");
        return false;
    }

    int dst_w = impl_->cfg.input_w;
    int dst_h = impl_->cfg.input_h;

    roi_algorithm_result_t roi = {};
    roi_tuning_apply_thresholds(&impl_->roi_cfg);
    int64_t t_roi_start = esp_timer_get_time();
    bool roi_ok = roi_algorithm_detect(
        rgb888, src_w, src_h, src_w * 3,
        &impl_->roi_cfg,
        &impl_->roi_work,
        &roi);
    int64_t t_roi_end = esp_timer_get_time();
    int64_t roi_detect_us = t_roi_end - t_roi_start;
    if (!roi_ok) {
        impl_->last_perf.roi_detect_us = roi_detect_us;
        impl_->last_perf.roi_success = false;
        impl_->last_perf.roi_skipped = true;
        impl_->last_perf.roi_fail_reason = roi.fail_reason;
        impl_->last_perf.detect_count = impl_->detect_count + 1;
        impl_->last_perf.total_us = t_roi_end - t_start;
        impl_->detect_count++;
        if (impl_->detect_count <= 5 || (impl_->detect_count % 30) == 0) {
            // 暂时屏蔽
            // ESP_LOGW(TAG,
            //          "ESPDet ROI skip #%lu frame=%dx%d fail=%d roi=%lldms total=%lldms",
            //          (unsigned long)impl_->detect_count,
            //          src_w, src_h,
            //          roi.fail_reason,
            //          roi_detect_us / 1000,
            //          (t_roi_end - t_start) / 1000);
        }
        return false;
    }

    PreprocessRegion region = {
        .x = roi.x,
        .y = roi.y,
        .w = roi.w,
        .h = roi.h,
    };

    // 获取输入 tensor（ESP-DL 3.3.1 可能有空 inputs，使用 begin()->second 安全访问）
    auto &all_inputs = impl_->model->get_inputs();
    if (all_inputs.empty()) {
        ESP_LOGE(TAG, "Model has 0 input tensors");
        return false;
    }
    dl::TensorBase *input_tensor = all_inputs.begin()->second;
    int input_exp = input_tensor->exponent.get();
    if (input_exp != impl_->input_exp) {
        impl_->input_exp = input_exp;
        build_quant_lut(impl_->input_exp, impl_->quant_lut);
    }
    float input_scale = ldexpf(1.0f, input_exp);
    int8_t *tensor_data = input_tensor->get_element_ptr<int8_t>();

    // 预处理 + 送入模型输入
    LetterboxInfo lb = {};
    int64_t ppa_resize_us = 0;
    int64_t quantize_us = 0;
    bool used_ppa_preprocess = preprocess_letterbox_ppa(
        impl_->use_ppa_preprocess ? impl_->ppa : nullptr,
        impl_->ppa_resize_buf,
        impl_->ppa_resize_buf_capacity,
        impl_->ppa_cache_line_size,
        impl_->quant_lut,
        &impl_->ppa_fallback_logged,
        &impl_->ppa_scale_logged,
        rgb888, src_w, src_h,
        region,
        tensor_data, dst_w, dst_h,
        &lb,
        &ppa_resize_us,
        &quantize_us);
    int64_t t_preprocess = esp_timer_get_time();
    if (!used_ppa_preprocess) {
        lb = preprocess_letterbox(
            rgb888, src_w, src_h, src_w * 3,
            region,
            impl_->input_buf, dst_w, dst_h,
            input_scale);
        t_preprocess = esp_timer_get_time();
        memcpy(tensor_data, impl_->input_buf, dst_w * dst_h * 3);
    }
    int64_t t_pre = esp_timer_get_time();

    // 推理
    bool ran_multi_core = impl_->use_multi_core;
    impl_->model->run(ran_multi_core ? dl::RUNTIME_MODE_MULTI_CORE : dl::RUNTIME_MODE_SINGLE_CORE);
    int64_t t_run = esp_timer_get_time();
    int64_t run_us = t_run - t_pre;
    bool fallback_now = false;
    if (impl_->use_multi_core && run_us > 700000) {
        impl_->use_multi_core = false;
        fallback_now = true;
        impl_->fallback_triggered = true;
        ESP_LOGW(TAG, "Multi-core run took %lldms; falling back to single-core runtime", run_us / 1000);
    }

    // ESPDet 解码：输出 tensor → 原始图像坐标系中的检测框
    DecodeConfig dcfg = {
        .input_w = dst_w,
        .input_h = dst_h,
        .num_classes = impl_->cfg.num_classes,
        .conf_threshold = impl_->cfg.conf_threshold,
    };

    auto &all_outputs = impl_->model->get_outputs();
    if (all_outputs.empty()) {
        ESP_LOGE(TAG, "No outputs found");
        return false;
    }

    impl_->outputs.clear();
    for (auto &kv : all_outputs) {
        dl::TensorBase *out = kv.second;
        impl_->outputs.push_back({
            .data = out->get_element_ptr<int8_t>(),
            .exponent = out->exponent.get(),
            .shape = &out->shape,
        });
    }

    float target_confidence = 0.0f;
    float class_prob[YOLOPerfStats::kClassProbCount] = {};
    yolo_decode_espdet(
        impl_->outputs,
        dcfg,
        lb.scale, lb.pad_x, lb.pad_y,
        region.w, region.h,
        results,
        &target_confidence,
        class_prob);
    for (Detection &d : results) {
        d.x1 = std::max(0.0f, std::min(d.x1 + (float)region.x, (float)src_w));
        d.y1 = std::max(0.0f, std::min(d.y1 + (float)region.y, (float)src_h));
        d.x2 = std::max(0.0f, std::min(d.x2 + (float)region.x, (float)src_w));
        d.y2 = std::max(0.0f, std::min(d.y2 + (float)region.y, (float)src_h));
    }
    int64_t t_decode = esp_timer_get_time();
    impl_->last_perf.roi_detect_us = roi_detect_us;
    impl_->last_perf.ppa_resize_us = ppa_resize_us;
    impl_->last_perf.quantize_us = quantize_us;
    impl_->last_perf.preprocess_us = t_preprocess - t_start;
    impl_->last_perf.input_copy_us = t_pre - t_preprocess;
    impl_->last_perf.pre_us = t_pre - t_start;
    impl_->last_perf.run_us = run_us;
    impl_->last_perf.decode_us = t_decode - t_run;
    impl_->last_perf.total_us = t_decode - t_start;
    impl_->last_perf.target_confidence = target_confidence;
    impl_->last_perf.detect_count = impl_->detect_count + 1;
    impl_->last_perf.runtime_multi_core = ran_multi_core;
    impl_->last_perf.fallback_triggered = fallback_now;
    impl_->last_perf.ppa_preprocess = used_ppa_preprocess;
    impl_->last_perf.roi_success = true;
    impl_->last_perf.roi_skipped = false;
    impl_->last_perf.roi_fail_reason = roi.fail_reason;
    impl_->last_perf.roi_x = roi.x;
    impl_->last_perf.roi_y = roi.y;
    impl_->last_perf.roi_w = roi.w;
    impl_->last_perf.roi_h = roi.h;
    for (int i = 0; i < YOLOPerfStats::kClassProbCount; i++) {
        impl_->last_perf.class_prob[i] = class_prob[i];
    }

    impl_->detect_count++;
    bool slow_run = run_us > 500000;
    if (impl_->detect_count <= 5 || (impl_->detect_count % 30) == 0 || slow_run || fallback_now) {
        // 暂时屏蔽
        // ESP_LOGI(TAG,
        //          "ESPDet detect #%lu frame=%dx%d outputs=%d pre=%lldms preprocess=%lldms ppa=%d resize=%lldms quant=%lldms copy=%lldms run=%lldms decode=%lldms total=%lldms det=%d target=%.3f runtime=%s",
        //          (unsigned long)impl_->detect_count,
        //          src_w, src_h,
        //          (int)all_outputs.size(),
        //          (t_pre - t_start) / 1000,
        //          (t_preprocess - t_start) / 1000,
        //          used_ppa_preprocess ? 1 : 0,
        //          ppa_resize_us / 1000,
        //          quantize_us / 1000,
        //          (t_pre - t_preprocess) / 1000,
        //          run_us / 1000,
        //          (t_decode - t_run) / 1000,
        //          (t_decode - t_start) / 1000,
        //          (int)results.size(),
        //          target_confidence,
        //          ran_multi_core ? "multi" : "single");
        // 暂时屏蔽
        // ESP_LOGI(TAG,
        //          "ESPDet ROI #%lu ok=1 skipped=0 fail=%d region=(%d,%d %dx%d) area=%d white=%d roi=%lldms",
        //          (unsigned long)impl_->detect_count,
        //          roi.fail_reason,
        //          roi.x, roi.y, roi.w, roi.h,
        //          roi.area_ratio_per_10000,
        //          roi.white_ratio_per_10000,
        //          roi_detect_us / 1000);
        if (slow_run || fallback_now) {
            // 暂时屏蔽
            // ESP_LOGW(TAG,
            //          "ESPDet slow run #%lu ran_multi=%d fallback_now=%d core=%d prio=%u stack_hwm=%u free_heap=%luKB internal=%luKB psram=%luKB",
            //          (unsigned long)impl_->detect_count,
            //          ran_multi_core ? 1 : 0,
            //          fallback_now ? 1 : 0,
            //          xPortGetCoreID(),
            //          (unsigned)uxTaskPriorityGet(NULL),
            //          (unsigned)uxTaskGetStackHighWaterMark(NULL),
            //          heap_caps_get_free_size(MALLOC_CAP_DEFAULT) / 1024,
            //          heap_caps_get_free_size(MALLOC_CAP_INTERNAL) / 1024,
            //          heap_caps_get_free_size(MALLOC_CAP_SPIRAM) / 1024);
        }
        if (!results.empty()) {
            // 暂时屏蔽
            // const Detection &d = results[0];
            // 暂时屏蔽
            // ESP_LOGI(TAG,
            //          "ESPDet det[0] cls=%d conf=%.3f box=(%.1f,%.1f,%.1f,%.1f) size=%.1fx%.1f",
            //          d.class_id,
            //          d.confidence,
            //          d.x1, d.y1, d.x2, d.y2,
            //          d.x2 - d.x1,
            //          d.y2 - d.y1);
        }
    }

    return true;
}

YOLOPerfStats AppYOLO::get_last_perf_stats() const
{
    return impl_->last_perf;
}
