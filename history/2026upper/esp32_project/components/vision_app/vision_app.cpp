#include "vision_app.h"

#include "app_yolo.hpp"
#include "bsp_touch.h"
#include "bsp_cam_sensor.h"
#include "roi_tuning.h"
#include "sorting_sim_control.h"
#include "vision_preview.h"

#include <vector>

#include "esp_check.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "esp_spiffs.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/idf_additions.h"
#include "freertos/task.h"

static const char *TAG = "vision_app";

#define VISION_MODEL_PATH       "/storage/roi224_515.espdl"
#define VISION_MODEL_INPUT_W    224
#define VISION_MODEL_INPUT_H    224
#define VISION_MODEL_CLASSES    3
#define VISION_MODEL_CONF       0.25f
#define VISION_TASK_STACK_BYTES (32 * 1024)
#define VISION_TASK_CORE        1
#define VISION_TASK_PRIORITY    3
#define VISION_PREVIEW_IMAGE_EVERY_N_FRAMES 1

#define VISION_CAM_SCL_PIN      8
#define VISION_CAM_SDA_PIN      7
#define VISION_CAM_PWDN_PIN     -1
#define VISION_CAM_RESET_PIN    -1
#define VISION_CAM_WIDTH        1024
#define VISION_CAM_HEIGHT       600

static TaskHandle_t s_task_handle = nullptr;

static sorter_package_class_t sorter_class_from_detection_class(int class_id)
{
    switch (class_id) {
    case 0:
        return SORTER_CLASS_1;
    case 1:
        return SORTER_CLASS_2;
    case 2:
        return SORTER_CLASS_3;
    default:
        return SORTER_CLASS_UNKNOWN;
    }
}

static void submit_best_detection_to_sorter(const std::vector<Detection> &detections)
{
    if (detections.empty()) {
        return;
    }

    const Detection *best = &detections[0];
    for (const Detection &d : detections) {
        if (d.confidence > best->confidence) {
            best = &d;
        }
    }

    sorter_package_class_t cls = sorter_class_from_detection_class(best->class_id);
    if (cls == SORTER_CLASS_UNKNOWN) {
        ESP_LOGW(TAG, "ignore detection with unknown sorter class_id=%d conf=%.3f",
                 best->class_id, (double)best->confidence);
        return;
    }
    sorting_sim_control_submit_vision_class(cls, best->confidence);
}

static esp_err_t mount_model_storage(void)
{
    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/storage",
        .partition_label = "storage",
        .max_files = 4,
        .format_if_mount_failed = false,
    };

    esp_err_t ret = esp_vfs_spiffs_register(&conf);
    if (ret == ESP_ERR_INVALID_STATE) {
        ESP_LOGW(TAG, "SPIFFS already mounted");
        return ESP_OK;
    }
    ESP_RETURN_ON_ERROR(ret, TAG, "SPIFFS mount failed");

    size_t total = 0;
    size_t used = 0;
    ESP_RETURN_ON_ERROR(esp_spiffs_info(conf.partition_label, &total, &used),
                        TAG, "SPIFFS info failed");
    ESP_LOGI(TAG, "SPIFFS mounted: %u/%u KB", (unsigned)(used / 1024), (unsigned)(total / 1024));
    return ESP_OK;
}

static esp_err_t init_camera(void)
{
    cam_sensor_config_t cam_cfg = {
        .i2c_scl_pin = VISION_CAM_SCL_PIN,
        .i2c_sda_pin = VISION_CAM_SDA_PIN,
        .pwdn_pin = VISION_CAM_PWDN_PIN,
        .reset_pin = VISION_CAM_RESET_PIN,
        .frame_width = VISION_CAM_WIDTH,
        .frame_height = VISION_CAM_HEIGHT,
        .i2c_bus = BSP_Touch_GetI2CBus(),
    };

    ESP_RETURN_ON_ERROR(cam_sensor_init(&cam_cfg), TAG, "camera init failed");
    ESP_RETURN_ON_ERROR(cam_sensor_start(), TAG, "camera start failed");
    return ESP_OK;
}

static void vision_task(void *arg)
{
    AppYOLO *yolo = static_cast<AppYOLO *>(arg);
    int64_t fps_timer = esp_timer_get_time();
    int frame_count = 0;
    int image_update_count = 0;
    float fps_current = 0.0f;
    int perf_samples = 0;
    int ppa_preprocess_count = 0;
    int64_t sum_cam_wait_us = 0;
    int64_t sum_ppa_resize_us = 0;
    int64_t sum_quantize_us = 0;
    int64_t sum_preprocess_us = 0;
    int64_t sum_input_copy_us = 0;
    int64_t sum_pre_us = 0;
    int64_t sum_run_us = 0;
    int64_t sum_decode_us = 0;
    int64_t sum_preview_us = 0;
    int64_t sum_loop_us = 0;
    int64_t max_run_us = 0;
    int64_t max_loop_us = 0;
    int prob_samples = 0;
    float prob_sum[YOLOPerfStats::kClassProbCount] = {};
    float prob_max[YOLOPerfStats::kClassProbCount] = {};
    float prob_display_avg[YOLOPerfStats::kClassProbCount] = {};
    float prob_display_max[YOLOPerfStats::kClassProbCount] = {};
    std::vector<Detection> detections;
    detections.reserve(10);

    ESP_LOGI(TAG, "vision task started core=%d priority=%u stack_hwm=%u",
             xPortGetCoreID(),
             (unsigned)uxTaskPriorityGet(NULL),
             (unsigned)uxTaskGetStackHighWaterMark(NULL));

    while (true) {
        int64_t loop_start = esp_timer_get_time();
        uint8_t *frame = nullptr;
        size_t frame_size = 0;
        int frame_w = 0;
        int frame_h = 0;

        esp_err_t err = cam_sensor_get_frame(&frame, &frame_size, &frame_w, &frame_h, 1000);
        if (err == ESP_ERR_TIMEOUT) {
            ESP_LOGW(TAG, "camera frame timeout");
            continue;
        }
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "camera frame failed: %s", esp_err_to_name(err));
            vTaskDelay(pdMS_TO_TICKS(1000));
            continue;
        }
        int64_t t_frame = esp_timer_get_time();

        if (roi_tuning_consume_calibration_request()) {
            roi_tuning_calibration_result_t cal = {};
            bool cal_ok = roi_tuning_calibrate_from_rgb888(
                frame, frame_w, frame_h, frame_w * 3, &cal);
            vision_preview_set_calibration_result(cal_ok);
            ESP_LOGI(TAG,
                     "ROI calibration %s sample=(%d,%d %dx%d) samples=%d before=(Y%d S%d D%d M%d) after=(Y%d S%d D%d M%d)",
                     cal_ok ? "OK" : "FAIL",
                     cal.region_x, cal.region_y, cal.region_w, cal.region_h,
                     cal.sample_count,
                     cal.before.y_min,
                     cal.before.sat_approx_max,
                     cal.before.rgb_delta_max,
                     cal.before.min_channel_min,
                     cal.after.y_min,
                     cal.after.sat_approx_max,
                     cal.after.rgb_delta_max,
                     cal.after.min_channel_min);
        }

        detections.clear();
        bool ok = yolo->detect(frame, frame_w, frame_h, detections);
        YOLOPerfStats perf = yolo->get_last_perf_stats();
        if (ok) {
            submit_best_detection_to_sorter(detections);
        }
        if (perf.roi_success) {
            for (int i = 0; i < YOLOPerfStats::kClassProbCount; i++) {
                prob_sum[i] += perf.class_prob[i];
                if (perf.class_prob[i] > prob_max[i]) {
                    prob_max[i] = perf.class_prob[i];
                }
                perf.class_prob_avg[i] = prob_display_avg[i];
                perf.class_prob_max[i] = prob_display_max[i];
            }
            prob_samples++;
        } else {
            prob_samples = 0;
            for (int i = 0; i < YOLOPerfStats::kClassProbCount; i++) {
                prob_sum[i] = 0.0f;
                prob_max[i] = 0.0f;
                prob_display_avg[i] = 0.0f;
                prob_display_max[i] = 0.0f;
                perf.class_prob_avg[i] = 0.0f;
                perf.class_prob_max[i] = 0.0f;
            }
        }
        int64_t t_detect = esp_timer_get_time();
#if VISION_PREVIEW_IMAGE_EVERY_N_FRAMES <= 1
        bool update_preview_image = true;
#else
        bool update_preview_image = ((perf.detect_count % VISION_PREVIEW_IMAGE_EVERY_N_FRAMES) == 0);
#endif
        esp_err_t preview_ret = update_preview_image
            ? vision_preview_update(frame, frame_w, frame_h, detections, fps_current, perf, ok)
            : vision_preview_update_status(detections, fps_current, perf, ok);
        int64_t t_preview = esp_timer_get_time();
        cam_sensor_return_frame(frame);
        bool image_updated = update_preview_image && (preview_ret == ESP_OK);
        if (image_updated) {
            image_update_count++;
        }
        int64_t cam_wait_us = t_frame - loop_start;
        int64_t preview_us = t_preview - t_detect;
        int64_t loop_us = t_preview - loop_start;
        sum_cam_wait_us += cam_wait_us;
        sum_ppa_resize_us += perf.ppa_resize_us;
        sum_quantize_us += perf.quantize_us;
        sum_preprocess_us += perf.preprocess_us;
        sum_input_copy_us += perf.input_copy_us;
        sum_pre_us += perf.pre_us;
        sum_run_us += perf.run_us;
        sum_decode_us += perf.decode_us;
        sum_preview_us += preview_us;
        sum_loop_us += loop_us;
        if (perf.run_us > max_run_us) {
            max_run_us = perf.run_us;
        }
        if (loop_us > max_loop_us) {
            max_loop_us = loop_us;
        }
        if (perf.ppa_preprocess) {
            ppa_preprocess_count++;
        }
        perf_samples++;

        frame_count++;
        vTaskDelay(pdMS_TO_TICKS(1));

        int64_t now = esp_timer_get_time();
        if (now - fps_timer >= 1000000LL) {
            float fps = (float)frame_count * 1000000.0f / (float)(now - fps_timer);
            fps_current = fps;
            if (prob_samples > 0) {
                for (int i = 0; i < YOLOPerfStats::kClassProbCount; i++) {
                    prob_display_avg[i] = prob_sum[i] / (float)prob_samples;
                    prob_display_max[i] = prob_max[i];
                }
            }
            // 暂时屏蔽
            // ESP_LOGI(TAG, "AI FPS: %.1f detect_ok:%d detections:%d target:%.3f images:%d free_heap:%luKB psram:%luKB",
            //          fps_current,
            //          ok ? 1 : 0,
            //          (int)detections.size(),
            //          perf.target_confidence,
            //          image_update_count,
            //          heap_caps_get_free_size(MALLOC_CAP_DEFAULT) / 1024,
            //          heap_caps_get_free_size(MALLOC_CAP_SPIRAM) / 1024);
            // 暂时屏蔽
            // ESP_LOGI(TAG,
            //          "AI perf latest #%lu runtime=%s fallback=%d ppa=%d image=%d cam_wait=%lldms pre=%lldms preprocess=%lldms resize=%lldms quant=%lldms copy=%lldms run=%lldms decode=%lldms preview=%lldms loop=%lldms core=%d prio=%u stack_hwm=%u internal=%luKB",
            //          (unsigned long)perf.detect_count,
            //          perf.runtime_multi_core ? "multi" : "single",
            //          perf.fallback_triggered ? 1 : 0,
            //          perf.ppa_preprocess ? 1 : 0,
            //          image_updated ? 1 : 0,
            //          cam_wait_us / 1000,
            //          perf.pre_us / 1000,
            //          perf.preprocess_us / 1000,
            //          perf.ppa_resize_us / 1000,
            //          perf.quantize_us / 1000,
            //          perf.input_copy_us / 1000,
            //          perf.run_us / 1000,
            //          perf.decode_us / 1000,
            //          preview_us / 1000,
            //          loop_us / 1000,
            //          xPortGetCoreID(),
            //          (unsigned)uxTaskPriorityGet(NULL),
            //          (unsigned)uxTaskGetStackHighWaterMark(NULL),
            //          heap_caps_get_free_size(MALLOC_CAP_INTERNAL) / 1024);
            if (perf_samples > 0) {
                // 暂时屏蔽
                // ESP_LOGI(TAG,
                //          "AI perf avg samples=%d fps=%.2f ppa=%d images=%d cam_wait=%.1fms pre=%.1fms preprocess=%.1fms resize=%.1fms quant=%.1fms copy=%.1fms run=%.1fms decode=%.1fms preview=%.1fms loop=%.1fms max_run=%lldms max_loop=%lldms",
                //          perf_samples,
                //          fps_current,
                //          ppa_preprocess_count,
                //          image_update_count,
                //          (double)sum_cam_wait_us / (double)perf_samples / 1000.0,
                //          (double)sum_pre_us / (double)perf_samples / 1000.0,
                //          (double)sum_preprocess_us / (double)perf_samples / 1000.0,
                //          (double)sum_ppa_resize_us / (double)perf_samples / 1000.0,
                //          (double)sum_quantize_us / (double)perf_samples / 1000.0,
                //          (double)sum_input_copy_us / (double)perf_samples / 1000.0,
                //          (double)sum_run_us / (double)perf_samples / 1000.0,
                //          (double)sum_decode_us / (double)perf_samples / 1000.0,
                //          (double)sum_preview_us / (double)perf_samples / 1000.0,
                //          (double)sum_loop_us / (double)perf_samples / 1000.0,
                //          max_run_us / 1000,
                //          max_loop_us / 1000);
            }
            frame_count = 0;
            image_update_count = 0;
            perf_samples = 0;
            ppa_preprocess_count = 0;
            prob_samples = 0;
            sum_cam_wait_us = 0;
            sum_ppa_resize_us = 0;
            sum_quantize_us = 0;
            sum_preprocess_us = 0;
            sum_input_copy_us = 0;
            sum_pre_us = 0;
            sum_run_us = 0;
            sum_decode_us = 0;
            sum_preview_us = 0;
            sum_loop_us = 0;
            max_run_us = 0;
            max_loop_us = 0;
            for (int i = 0; i < YOLOPerfStats::kClassProbCount; i++) {
                prob_sum[i] = 0.0f;
                prob_max[i] = 0.0f;
            }
            fps_timer = now;
        }
    }
}

esp_err_t vision_app_start(void)
{
    if (s_task_handle) {
        ESP_LOGW(TAG, "vision app already started");
        return ESP_OK;
    }

    ESP_RETURN_ON_ERROR(mount_model_storage(), TAG, "model storage failed");

    YOLOConfig cfg;
    cfg.input_w = VISION_MODEL_INPUT_W;
    cfg.input_h = VISION_MODEL_INPUT_H;
    cfg.num_classes = VISION_MODEL_CLASSES;
    cfg.conf_threshold = VISION_MODEL_CONF;

    AppYOLO *yolo = new AppYOLO(cfg);
    if (!yolo) {
        return ESP_ERR_NO_MEM;
    }
    if (!yolo->load_model(VISION_MODEL_PATH)) {
        delete yolo;
        return ESP_FAIL;
    }

    ESP_RETURN_ON_ERROR(init_camera(), TAG, "camera setup failed");

    BaseType_t ok = xTaskCreatePinnedToCoreWithCaps(
        vision_task,
        "vision_task",
        VISION_TASK_STACK_BYTES,
        yolo,
        VISION_TASK_PRIORITY,
        &s_task_handle,
        VISION_TASK_CORE,
        MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    if (ok != pdPASS) {
        delete yolo;
        return ESP_ERR_NO_MEM;
    }
    ESP_LOGI(TAG, "vision task created priority=%d core=%d stack=%dB",
             VISION_TASK_PRIORITY, VISION_TASK_CORE, VISION_TASK_STACK_BYTES);

    return ESP_OK;
}
