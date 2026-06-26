#include "roi_algorithm.h"

#include <string.h>

static int clamp_int(int v, int lo, int hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

roi_algorithm_config_t roi_algorithm_default_config(void) {
    roi_algorithm_config_t cfg;
    cfg.down_w = 160;
    cfg.down_h = 94;
    cfg.y_min = 150;
    cfg.sat_approx_max = 70;
    cfg.rgb_delta_max = 50;
    cfg.min_channel_min = 145;
    cfg.close_w = 5;
    cfg.close_h = 3;
    cfg.close_iter = 1;
    cfg.dilate_w = 5;
    cfg.dilate_h = 3;
    cfg.dilate_iter = 1;
    cfg.min_area_per_10000 = 120;
    cfg.max_area_per_10000 = 4500;
    cfg.min_white_per_10000 = 1800;
    cfg.padding_per_10000 = 0;
    cfg.padding_px = 0;
    cfg.min_roi_w = 20;
    cfg.min_roi_h = 20;
    return cfg;
}

static void build_mask_center_rgb888(
    const uint8_t *rgb888,
    int src_w,
    int src_h,
    int stride,
    const roi_algorithm_config_t *cfg,
    uint8_t *mask) {
    for (int dy = 0; dy < cfg->down_h; ++dy) {
        int sy = (dy * src_h + src_h / 2) / cfg->down_h;
        sy = clamp_int(sy, 0, src_h - 1);
        const uint8_t *row = rgb888 + sy * stride;
        for (int dx = 0; dx < cfg->down_w; ++dx) {
            int sx = (dx * src_w + src_w / 2) / cfg->down_w;
            sx = clamp_int(sx, 0, src_w - 1);
            const uint8_t *p = row + sx * 3;
            int r = p[0];
            int g = p[1];
            int b = p[2];
            int maxc = r > g ? r : g;
            if (b > maxc) maxc = b;
            int minc = r < g ? r : g;
            if (b < minc) minc = b;
            int delta = maxc - minc;
            int y = (77 * r + 150 * g + 29 * b) >> 8;
            int sat = maxc > 0 ? (delta * 255) / maxc : 0;
            mask[dy * cfg->down_w + dx] =
                (y >= cfg->y_min &&
                 sat <= cfg->sat_approx_max &&
                 delta <= cfg->rgb_delta_max &&
                 minc >= cfg->min_channel_min)
                    ? 255
                    : 0;
        }
    }
}

static void dilate_once(const uint8_t *src, uint8_t *dst, int w, int h, int kw, int kh) {
    int rx = kw / 2;
    int ry = kh / 2;
    memset(dst, 0, (size_t)w * (size_t)h);
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            int found = 0;
            for (int yy = y - ry; yy <= y + ry && !found; ++yy) {
                if (yy < 0 || yy >= h) continue;
                const uint8_t *row = src + yy * w;
                for (int xx = x - rx; xx <= x + rx; ++xx) {
                    if (xx < 0 || xx >= w) continue;
                    if (row[xx]) {
                        found = 1;
                        break;
                    }
                }
            }
            dst[y * w + x] = found ? 255 : 0;
        }
    }
}

static void erode_once(const uint8_t *src, uint8_t *dst, int w, int h, int kw, int kh) {
    int rx = kw / 2;
    int ry = kh / 2;
    memset(dst, 0, (size_t)w * (size_t)h);
    for (int y = ry; y < h - ry; ++y) {
        for (int x = rx; x < w - rx; ++x) {
            int all = 1;
            for (int yy = y - ry; yy <= y + ry && all; ++yy) {
                const uint8_t *row = src + yy * w;
                for (int xx = x - rx; xx <= x + rx; ++xx) {
                    if (!row[xx]) {
                        all = 0;
                        break;
                    }
                }
            }
            dst[y * w + x] = all ? 255 : 0;
        }
    }
}

static void morph_apply(uint8_t *mask, uint8_t *tmp, const roi_algorithm_config_t *cfg) {
    int w = cfg->down_w;
    int h = cfg->down_h;
    for (int i = 0; i < cfg->close_iter; ++i) {
        dilate_once(mask, tmp, w, h, cfg->close_w, cfg->close_h);
        memcpy(mask, tmp, (size_t)w * (size_t)h);
    }
    for (int i = 0; i < cfg->close_iter; ++i) {
        erode_once(mask, tmp, w, h, cfg->close_w, cfg->close_h);
        memcpy(mask, tmp, (size_t)w * (size_t)h);
    }
    for (int i = 0; i < cfg->dilate_iter; ++i) {
        dilate_once(mask, tmp, w, h, cfg->dilate_w, cfg->dilate_h);
        memcpy(mask, tmp, (size_t)w * (size_t)h);
    }
}

static bool find_largest_component(
    uint8_t *mask,
    uint16_t *queue,
    const roi_algorithm_config_t *cfg,
    int *out_x,
    int *out_y,
    int *out_w,
    int *out_h,
    int *out_area,
    int *out_white_per_10000,
    int *fail_reason) {
    int w = cfg->down_w;
    int h = cfg->down_h;
    int image_area = w * h;
    int min_area = (image_area * cfg->min_area_per_10000) / 10000;
    int max_area = (image_area * cfg->max_area_per_10000) / 10000;
    int best_area = 0;
    int best_x0 = 0, best_y0 = 0, best_x1 = 0, best_y1 = 0;
    int saw_component = 0;

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            int start = y * w + x;
            if (mask[start] != 255) continue;
            saw_component = 1;

            int head = 0;
            int tail = 0;
            int area = 0;
            int x0 = x, x1 = x, y0 = y, y1 = y;
            queue[tail++] = (uint16_t)start;
            mask[start] = 128;

            while (head < tail) {
                int idx = queue[head++];
                int cy = idx / w;
                int cx = idx - cy * w;
                ++area;
                if (cx < x0) x0 = cx;
                if (cx > x1) x1 = cx;
                if (cy < y0) y0 = cy;
                if (cy > y1) y1 = cy;

                for (int oy = -1; oy <= 1; ++oy) {
                    int ny = cy + oy;
                    if (ny < 0 || ny >= h) continue;
                    for (int ox = -1; ox <= 1; ++ox) {
                        int nx = cx + ox;
                        if (ox == 0 && oy == 0) continue;
                        if (nx < 0 || nx >= w) continue;
                        int nidx = ny * w + nx;
                        if (mask[nidx] == 255) {
                            mask[nidx] = 128;
                            queue[tail++] = (uint16_t)nidx;
                        }
                    }
                }
            }

            if (area >= min_area && area <= max_area && area > best_area) {
                best_area = area;
                best_x0 = x0;
                best_y0 = y0;
                best_x1 = x1;
                best_y1 = y1;
            }
        }
    }

    if (!saw_component) {
        *fail_reason = ROI_ALGORITHM_FAIL_NO_COMPONENT;
        return false;
    }
    if (best_area == 0) {
        *fail_reason = ROI_ALGORITHM_FAIL_AREA_RANGE;
        return false;
    }

    int bw = best_x1 - best_x0 + 1;
    int bh = best_y1 - best_y0 + 1;
    int white = (best_area * 10000) / (bw * bh);
    if (white < cfg->min_white_per_10000) {
        *fail_reason = ROI_ALGORITHM_FAIL_SPARSE;
        return false;
    }

    *out_x = best_x0;
    *out_y = best_y0;
    *out_w = bw;
    *out_h = bh;
    *out_area = best_area;
    *out_white_per_10000 = white;
    *fail_reason = ROI_ALGORITHM_OK;
    return true;
}

bool roi_algorithm_detect(
    const uint8_t *rgb888,
    int src_w,
    int src_h,
    int src_stride_bytes,
    const roi_algorithm_config_t *cfg,
    roi_algorithm_workspace_t *work,
    roi_algorithm_result_t *out) {
    if (!rgb888 || !cfg || !work || !out || !work->mask || !work->tmp || !work->queue) {
        if (out) {
            memset(out, 0, sizeof(*out));
            out->fail_reason = ROI_ALGORITHM_FAIL_BAD_ARG;
        }
        return false;
    }
    int need = cfg->down_w * cfg->down_h;
    if (src_w <= 0 || src_h <= 0 || src_stride_bytes < src_w * 3 || work->pixel_count < (size_t)need) {
        memset(out, 0, sizeof(*out));
        out->fail_reason = ROI_ALGORITHM_FAIL_BAD_ARG;
        return false;
    }

    build_mask_center_rgb888(rgb888, src_w, src_h, src_stride_bytes, cfg, work->mask);
    morph_apply(work->mask, work->tmp, cfg);

    int sx = 0, sy = 0, sw = 0, sh = 0, area = 0, white = 0, fail = 0;
    if (!find_largest_component(work->mask, work->queue, cfg, &sx, &sy, &sw, &sh, &area, &white, &fail)) {
        memset(out, 0, sizeof(*out));
        out->fail_reason = fail;
        return false;
    }

    int x0 = (sx * src_w) / cfg->down_w;
    int y0 = (sy * src_h) / cfg->down_h;
    int x1 = ((sx + sw) * src_w + cfg->down_w - 1) / cfg->down_w;
    int y1 = ((sy + sh) * src_h + cfg->down_h - 1) / cfg->down_h;
    int pad_x = ((x1 - x0) * cfg->padding_per_10000 + 5000) / 10000;
    int pad_y = ((y1 - y0) * cfg->padding_per_10000 + 5000) / 10000;
    if (pad_x < cfg->padding_px) pad_x = cfg->padding_px;
    if (pad_y < cfg->padding_px) pad_y = cfg->padding_px;
    x0 = clamp_int(x0 - pad_x, 0, src_w);
    y0 = clamp_int(y0 - pad_y, 0, src_h);
    x1 = clamp_int(x1 + pad_x, 0, src_w);
    y1 = clamp_int(y1 + pad_y, 0, src_h);

    memset(out, 0, sizeof(*out));
    out->x = x0;
    out->y = y0;
    out->w = x1 - x0;
    out->h = y1 - y0;
    out->white_ratio_per_10000 = white;
    out->area_ratio_per_10000 =
        (int)(((int64_t)out->w * (int64_t)out->h * 10000) / ((int64_t)src_w * (int64_t)src_h));
    if (out->w < cfg->min_roi_w || out->h < cfg->min_roi_h) {
        out->fail_reason = ROI_ALGORITHM_FAIL_TOO_SMALL;
        return false;
    }
    out->success = true;
    out->fail_reason = ROI_ALGORITHM_OK;
    return true;
}

void roi_algorithm_letterbox(
    const uint8_t *rgb888,
    int src_w,
    int src_h,
    int src_stride_bytes,
    const roi_algorithm_result_t *roi,
    uint8_t *dst_rgb888,
    int dst_w,
    int dst_h,
    int dst_stride_bytes,
    uint8_t pad_r,
    uint8_t pad_g,
    uint8_t pad_b) {
    if (!rgb888 || !roi || !dst_rgb888 || dst_w <= 0 || dst_h <= 0 || dst_stride_bytes < dst_w * 3) {
        return;
    }

    for (int y = 0; y < dst_h; ++y) {
        uint8_t *row = dst_rgb888 + y * dst_stride_bytes;
        for (int x = 0; x < dst_w; ++x) {
            row[x * 3 + 0] = pad_r;
            row[x * 3 + 1] = pad_g;
            row[x * 3 + 2] = pad_b;
        }
    }

    if (!roi->success || roi->w <= 0 || roi->h <= 0) {
        return;
    }

    int scaled_w;
    int scaled_h;
    if (roi->w * dst_h >= roi->h * dst_w) {
        scaled_w = dst_w;
        scaled_h = (roi->h * dst_w + roi->w / 2) / roi->w;
    } else {
        scaled_h = dst_h;
        scaled_w = (roi->w * dst_h + roi->h / 2) / roi->h;
    }
    if (scaled_w < 1) scaled_w = 1;
    if (scaled_h < 1) scaled_h = 1;

    int pad_left = (dst_w - scaled_w) / 2;
    int pad_top = (dst_h - scaled_h) / 2;

    for (int dy = 0; dy < scaled_h; ++dy) {
        int sy = roi->y + (dy * roi->h + scaled_h / 2) / scaled_h;
        sy = clamp_int(sy, 0, src_h - 1);
        const uint8_t *src_row = rgb888 + sy * src_stride_bytes;
        uint8_t *dst_row = dst_rgb888 + (pad_top + dy) * dst_stride_bytes;
        for (int dx = 0; dx < scaled_w; ++dx) {
            int sx = roi->x + (dx * roi->w + scaled_w / 2) / scaled_w;
            sx = clamp_int(sx, 0, src_w - 1);
            const uint8_t *sp = src_row + sx * 3;
            uint8_t *dp = dst_row + (pad_left + dx) * 3;
            dp[0] = sp[0];
            dp[1] = sp[1];
            dp[2] = sp[2];
        }
    }
}
