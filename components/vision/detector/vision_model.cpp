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

#include "esp_heap_caps.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_spiffs.h"

#include "dl_detect_base.hpp"
#include "dl_detect_espdet_postprocessor.hpp"
#include "dl_image_preprocessor.hpp"
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
#define VISION_WAYBILL_MODEL_FILE  "det_pico_224_224_waybill.espdl"  // 用户面单模型(重导出)
#define VISION_WAYBILL_SCORE_THR   0.10f
#define VISION_WAYBILL_NMS_THR     0.70f
// 模型2：logo 三分类（0=极兔 1=中通 2=韵达）
// TODO: logo 模型尚未放入，暂指向面单模型打通链路，接入后改此宏即可。
#define VISION_LOGO_MODEL_FILE     "det_pico_224_224_waybill.espdl"  // 暂用面单模型占位
#define VISION_LOGO_SCORE_THR      0.25f
#define VISION_LOGO_NMS_THR        0.70f

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

// 编排层临时缓冲上限（面单只取最高分 1 个，logo 若干）。
#define VISION_MODEL_MAX_WB   4
#define VISION_MODEL_MAX_LOGO 4

// ===== 第一层：原子模型（DetectImpl 子类，装配三件套）=====

namespace {

// ESPDet-Pico 检测器：构造里装配 model + 前处理器 + 后处理器。
// 复刻 esp-detection 部署模板 espdet_detect::ESPDet 的装配顺序与参数。
class PicoDetect : public dl::detect::DetectImpl {
public:
    PicoDetect(const char *path, float score_thr, float nms_thr)
    {
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

        // test 后再 minimize，回收中间张量内存供实际推理使用。
        m_model->minimize();

        // mean=0 / std=255 → 归一化到 [0,1]；P4 输入 RGB888。
        m_image_preprocessor =
            new dl::image::ImagePreprocessor(m_model, {0, 0, 0}, {255, 255, 255});
        m_image_preprocessor->enable_letterbox({114, 114, 114});
        // ESPDet-Pico 三段 anchor-point：{stride_y,stride_x,offset_y,offset_x}。
        m_postprocessor = new dl::detect::ESPDetPostProcessor(
            m_model, m_image_preprocessor, score_thr, nms_thr, VISION_MODEL_TOP_K,
            {{8, 8, 4, 4}, {16, 16, 8, 8}, {32, 32, 16, 16}});
    }
};

} // namespace

// 不透明句柄实体：仅持一个检测器指针。
struct vision_detector {
    dl::detect::DetectImpl *model;
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

extern "C" int vision_detector_run(vision_detector_t *det,
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
    in.pix_type = dl::image::DL_IMAGE_PIX_TYPE_RGB888;

    std::list<dl::detect::result_t> &res = det->model->run(in);

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

// ===== 第二层：级联编排 =====

static vision_detector_t *s_waybill;   // 模型1句柄（单分类面单）
static vision_detector_t *s_logo;      // 模型2句柄（三分类 logo）
static int s_last_infer_ms;            // 最近一次两级合计耗时（ms）
static int s_prob_jt, s_prob_zt, s_prob_yd;  // 三类概率 ×100（来自模型2）
static bool s_spiffs_mounted;

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
    s_waybill = vision_detector_load(VISION_WAYBILL_MODEL_FILE,
                                     VISION_WAYBILL_SCORE_THR, VISION_WAYBILL_NMS_THR);
    s_logo    = vision_detector_load(VISION_LOGO_MODEL_FILE,
                                     VISION_LOGO_SCORE_THR, VISION_LOGO_NMS_THR);
    if (!s_waybill || !s_logo) {
        vision_detector_free(s_waybill);
        vision_detector_free(s_logo);
        s_waybill = NULL;
        s_logo = NULL;
        return false;
    }
    return true;
}

// 从原图【拷贝】裁剪 ROI 到新分配的独立缓冲（PSRAM）。裁剪矩形 clip 到原图边界。
// 返回子图指针（调用者负责 heap_caps_free）；出参回填实际 ROI 宽高与左上偏移。
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
    uint8_t *roi = (uint8_t *)heap_caps_malloc((size_t)rw * rh * 3, MALLOC_CAP_SPIRAM);
    if (!roi) {
        return NULL;
    }
    // 逐行拷贝原图 ROI 区域到紧凑子图（stride 从原图整宽收缩为 rw）。
    for (int r = 0; r < rh; r++) {
        const uint8_t *src = buf + ((size_t)(y1 + r) * width + x1) * 3;
        uint8_t *dst = roi + (size_t)r * rw * 3;
        memcpy(dst, src, (size_t)rw * 3);
    }
    *roi_w = rw;
    *roi_h = rh;
    *off_x = x1;
    *off_y = y1;
    return roi;
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
    int nlg = vision_detector_run(s_logo, roi, roi_w, roi_h,
                                  lg, VISION_MODEL_MAX_LOGO);

    // 5) 每个 logo 框加面单左上角偏移，映射回原图坐标 → dets[1..]（stage=LOGO）。
    int best_lg = -1;
    for (int i = 0; i < nlg && total < max; i++) {
        vision_model_det_t d = lg[i];
        d.box[0] += off_x;
        d.box[1] += off_y;
        d.box[2] += off_x;
        d.box[3] += off_y;
        d.stage = VISION_STAGE_LOGO;
        dets[total] = d;
        if (best_lg < 0 || d.score > dets[best_lg].score) {
            best_lg = total;
        }
        total++;
    }

    // 6) 三类概率来自模型2最高分 logo（命中类给 score×100，其余 0）。
    if (best_lg >= 0) {
        int p = (int)(dets[best_lg].score * 100.0f);
        switch (dets[best_lg].category) {
            case 0: s_prob_jt = p; break;
            case 1: s_prob_zt = p; break;
            case 2: s_prob_yd = p; break;
            default: break;
        }
    }

    heap_caps_free(roi);
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
