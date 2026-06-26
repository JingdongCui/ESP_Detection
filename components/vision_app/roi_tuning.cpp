#include "roi_tuning.h"

#include <atomic>
#include <cstring>

#include "esp_heap_caps.h"
#include "esp_log.h"

static const char *TAG = "roi_tuning";

static std::atomic<int> s_y_min{150};
static std::atomic<int> s_sat_approx_max{70};
static std::atomic<int> s_rgb_delta_max{50};
static std::atomic<int> s_min_channel_min{145};
static std::atomic<bool> s_calibration_requested{false};

static int clamp_i(int v, int lo, int hi)
{
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

static int hist_percentile(const int hist[256], int count, int percentile)
{
    if (count <= 0) return 0;
    int target = (count * percentile + 99) / 100;
    if (target < 1) target = 1;
    int sum = 0;
    for (int i = 0; i < 256; i++) {
        sum += hist[i];
        if (sum >= target) {
            return i;
        }
    }
    return 255;
}

roi_tuning_thresholds_t roi_tuning_get_thresholds(void)
{
    roi_tuning_thresholds_t out = {
        .y_min = s_y_min.load(std::memory_order_relaxed),
        .sat_approx_max = s_sat_approx_max.load(std::memory_order_relaxed),
        .rgb_delta_max = s_rgb_delta_max.load(std::memory_order_relaxed),
        .min_channel_min = s_min_channel_min.load(std::memory_order_relaxed),
    };
    return out;
}

void roi_tuning_set_thresholds(const roi_tuning_thresholds_t *thresholds)
{
    if (!thresholds) return;
    s_y_min.store(clamp_i(thresholds->y_min, 0, 255), std::memory_order_relaxed);
    s_sat_approx_max.store(clamp_i(thresholds->sat_approx_max, 0, 255), std::memory_order_relaxed);
    s_rgb_delta_max.store(clamp_i(thresholds->rgb_delta_max, 0, 255), std::memory_order_relaxed);
    s_min_channel_min.store(clamp_i(thresholds->min_channel_min, 0, 255), std::memory_order_relaxed);
}

void roi_tuning_apply_thresholds(roi_algorithm_config_t *cfg)
{
    if (!cfg) return;
    roi_tuning_thresholds_t t = roi_tuning_get_thresholds();
    cfg->y_min = t.y_min;
    cfg->sat_approx_max = t.sat_approx_max;
    cfg->rgb_delta_max = t.rgb_delta_max;
    cfg->min_channel_min = t.min_channel_min;
}

void roi_tuning_set_defaults_from_config(const roi_algorithm_config_t *cfg)
{
    if (!cfg) return;
    roi_tuning_thresholds_t t = {
        .y_min = cfg->y_min,
        .sat_approx_max = cfg->sat_approx_max,
        .rgb_delta_max = cfg->rgb_delta_max,
        .min_channel_min = cfg->min_channel_min,
    };
    roi_tuning_set_thresholds(&t);
}

void roi_tuning_request_calibration(void)
{
    s_calibration_requested.store(true, std::memory_order_release);
}

bool roi_tuning_consume_calibration_request(void)
{
    return s_calibration_requested.exchange(false, std::memory_order_acq_rel);
}

static bool alloc_workspace(const roi_algorithm_config_t *cfg, roi_algorithm_workspace_t *work)
{
    std::memset(work, 0, sizeof(*work));
    work->pixel_count = (size_t)cfg->down_w * (size_t)cfg->down_h;
    work->mask = (uint8_t *)heap_caps_malloc(work->pixel_count, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    work->tmp = (uint8_t *)heap_caps_malloc(work->pixel_count, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    work->queue = (uint16_t *)heap_caps_malloc(work->pixel_count * sizeof(uint16_t),
                                               MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    return work->mask && work->tmp && work->queue;
}

static void free_workspace(roi_algorithm_workspace_t *work)
{
    if (work->mask) heap_caps_free(work->mask);
    if (work->tmp) heap_caps_free(work->tmp);
    if (work->queue) heap_caps_free(work->queue);
    std::memset(work, 0, sizeof(*work));
}

static int roi_center_score(const roi_algorithm_result_t *roi, int src_w, int src_h)
{
    int cx = src_w / 2;
    int cy = src_h / 2;
    int rx0 = roi->x;
    int ry0 = roi->y;
    int rx1 = roi->x + roi->w;
    int ry1 = roi->y + roi->h;
    int dx = 0;
    int dy = 0;
    if (cx < rx0) dx = rx0 - cx;
    if (cx > rx1) dx = cx - rx1;
    if (cy < ry0) dy = ry0 - cy;
    if (cy > ry1) dy = cy - ry1;
    return dx + dy;
}

bool roi_tuning_calibrate_from_rgb888(
    const uint8_t *rgb888,
    int src_w,
    int src_h,
    int src_stride_bytes,
    roi_tuning_calibration_result_t *result)
{
    if (result) {
        std::memset(result, 0, sizeof(*result));
    }
    if (!rgb888 || src_w <= 0 || src_h <= 0 || src_stride_bytes < src_w * 3) {
        return false;
    }

    int region_w = clamp_i((src_w * 22 + 99) / 100, 16, src_w);
    int region_h = clamp_i((src_h * 22 + 99) / 100, 16, src_h);
    int region_x = (src_w - region_w) / 2;
    int region_y = (src_h - region_h) / 2;

    int hist_luma_all[256] = {};
    for (int y = region_y; y < region_y + region_h; y++) {
        const uint8_t *row = rgb888 + y * src_stride_bytes;
        for (int x = region_x; x < region_x + region_w; x++) {
            const uint8_t *p = row + x * 3;
            int lum = (77 * p[0] + 150 * p[1] + 29 * p[2]) >> 8;
            hist_luma_all[clamp_i(lum, 0, 255)]++;
        }
    }
    int all_count = region_w * region_h;
    int bright_floor = clamp_i(hist_percentile(hist_luma_all, all_count, 40), 35, 210);

    int hist_y[256] = {};
    int hist_sat[256] = {};
    int hist_delta[256] = {};
    int hist_min[256] = {};
    int sample_count = 0;

    for (int y = region_y; y < region_y + region_h; y++) {
        const uint8_t *row = rgb888 + y * src_stride_bytes;
        for (int x = region_x; x < region_x + region_w; x++) {
            const uint8_t *p = row + x * 3;
            int r = p[0];
            int g = p[1];
            int b = p[2];
            int maxc = r > g ? r : g;
            if (b > maxc) maxc = b;
            int minc = r < g ? r : g;
            if (b < minc) minc = b;
            int delta = maxc - minc;
            int lum = (77 * r + 150 * g + 29 * b) >> 8;
            int sat = maxc > 0 ? (delta * 255) / maxc : 0;
            bool brownish = (r > g + 18) && (g > b + 8) && (sat > 45);
            bool usable = (lum >= bright_floor) && (minc >= 35) &&
                          (sat <= 135) && (delta <= 105) && !brownish;
            if (!usable) continue;
            hist_y[clamp_i(lum, 0, 255)]++;
            hist_sat[clamp_i(sat, 0, 255)]++;
            hist_delta[clamp_i(delta, 0, 255)]++;
            hist_min[clamp_i(minc, 0, 255)]++;
            sample_count++;
        }
    }

    int min_samples = all_count / 20;
    if (sample_count < min_samples) {
        ESP_LOGW(TAG, "calibration rejected: samples=%d required=%d", sample_count, min_samples);
        return false;
    }

    int y_p10 = hist_percentile(hist_y, sample_count, 10);
    int sat_p90 = hist_percentile(hist_sat, sample_count, 90);
    int delta_p90 = hist_percentile(hist_delta, sample_count, 90);
    int min_p10 = hist_percentile(hist_min, sample_count, 10);

    roi_tuning_thresholds_t before = roi_tuning_get_thresholds();
    roi_algorithm_config_t cfg = roi_algorithm_default_config();
    roi_algorithm_workspace_t work;
    if (!alloc_workspace(&cfg, &work)) {
        free_workspace(&work);
        return false;
    }

    bool found = false;
    int best_score = -2147483647;
    roi_algorithm_result_t best_roi = {};
    roi_tuning_thresholds_t best = before;
    const int max_center_distance = (src_w + src_h) / 8;

    for (int i = 0; i < 5; i++) {
        roi_tuning_thresholds_t candidate = {
            .y_min = clamp_i(y_p10 - 18 - i * 8, 45, 220),
            .sat_approx_max = clamp_i(sat_p90 + 18 + i * 6, 35, 135),
            .rgb_delta_max = clamp_i(delta_p90 + 18 + i * 6, 25, 120),
            .min_channel_min = clamp_i(min_p10 - 18 - i * 8, 35, 220),
        };
        cfg.y_min = candidate.y_min;
        cfg.sat_approx_max = candidate.sat_approx_max;
        cfg.rgb_delta_max = candidate.rgb_delta_max;
        cfg.min_channel_min = candidate.min_channel_min;

        roi_algorithm_result_t roi = {};
        bool ok = roi_algorithm_detect(rgb888, src_w, src_h, src_stride_bytes, &cfg, &work, &roi);
        if (!ok || !roi.success) continue;
        int center_distance = roi_center_score(&roi, src_w, src_h);
        if (center_distance > max_center_distance) continue;
        if (roi.area_ratio_per_10000 < 120 || roi.area_ratio_per_10000 > 5500) continue;

        int score = roi.white_ratio_per_10000 * 4 -
                    roi.area_ratio_per_10000 -
                    center_distance * 12;
        if (!found || score > best_score) {
            found = true;
            best_score = score;
            best = candidate;
            best_roi = roi;
        }
    }

    free_workspace(&work);
    if (!found) {
        ESP_LOGW(TAG,
                 "calibration rejected: no centered ROI samples=%d p10=%d sat90=%d delta90=%d min10=%d",
                 sample_count, y_p10, sat_p90, delta_p90, min_p10);
        return false;
    }

    roi_tuning_set_thresholds(&best);
    if (result) {
        result->region_x = region_x;
        result->region_y = region_y;
        result->region_w = region_w;
        result->region_h = region_h;
        result->sample_count = sample_count;
        result->y_p10 = y_p10;
        result->sat_p90 = sat_p90;
        result->delta_p90 = delta_p90;
        result->min_channel_p10 = min_p10;
        result->before = before;
        result->after = best;
    }
    ESP_LOGI(TAG,
             "calibration ok samples=%d roi=(%d,%d %dx%d) white=%d area=%d thresholds Y=%d S=%d D=%d M=%d",
             sample_count,
             best_roi.x, best_roi.y, best_roi.w, best_roi.h,
             best_roi.white_ratio_per_10000,
             best_roi.area_ratio_per_10000,
             best.y_min,
             best.sat_approx_max,
             best.rgb_delta_max,
             best.min_channel_min);
    return true;
}
