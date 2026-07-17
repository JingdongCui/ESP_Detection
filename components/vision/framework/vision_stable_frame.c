

#include "vision_internal.h"
#include "vision_upload.h"

#include <stdint.h>
#include <stdatomic.h>
#include <string.h>

#include "driver/ppa.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

#define STABLE_SLOT_COUNT 3
#define STABLE_ALIGNMENT 64

typedef enum {
    STABLE_SLOT_FREE,
    STABLE_SLOT_COPYING,
    STABLE_SLOT_INFERENCING,
    STABLE_SLOT_ENCODE_PENDING,
    STABLE_SLOT_ENCODING,
} stable_slot_state_t;

struct vision_stable_slot {
    uint8_t *pixels;
    stable_slot_state_t state;
    vision_upload_frame_t upload;
};

static const char *TAG = "vision_stable";
static struct vision_stable_slot s_slots[STABLE_SLOT_COUNT];
static SemaphoreHandle_t s_slot_mutex;
static ppa_client_handle_t s_copy_ppa;
static portMUX_TYPE s_callback_lock = portMUX_INITIALIZER_UNLOCKED;
static vision_upload_submit_cb_t s_submit_cb;
static void *s_submit_ctx;
static atomic_uint_least32_t s_ppa_copy_ok;
static atomic_uint_least32_t s_ppa_copy_fail;
static atomic_uint_least32_t s_rgb_slot_drop;
static atomic_uint_least32_t s_upload_rejected;

static bool transition_allowed(stable_slot_state_t from, stable_slot_state_t to)
{
    switch (from) {
    case STABLE_SLOT_FREE:
        return to == STABLE_SLOT_COPYING;
    case STABLE_SLOT_COPYING:
        return to == STABLE_SLOT_INFERENCING || to == STABLE_SLOT_FREE;
    case STABLE_SLOT_INFERENCING:
        return to == STABLE_SLOT_ENCODE_PENDING || to == STABLE_SLOT_FREE;
    case STABLE_SLOT_ENCODE_PENDING:
        return to == STABLE_SLOT_ENCODING || to == STABLE_SLOT_FREE;
    case STABLE_SLOT_ENCODING:
        return to == STABLE_SLOT_FREE;
    default:
        return false;
    }
}

static bool transition_slot_locked(vision_stable_slot_t *slot, stable_slot_state_t to)
{
    stable_slot_state_t from = slot->state;
    if (!transition_allowed(from, to)) {
        ESP_LOGE(TAG, "illegal stable slot transition %d -> %d", (int)from, (int)to);
        return false;
    }
    slot->state = to;
    return true;
}

static bool slot_pointer_valid(const vision_stable_slot_t *slot)
{
    if (!slot) {
        return false;
    }
    uintptr_t value = (uintptr_t)slot;
    uintptr_t base = (uintptr_t)&s_slots[0];
    uintptr_t end = (uintptr_t)&s_slots[STABLE_SLOT_COUNT];
    return value >= base && value < end && ((value - base) % sizeof(s_slots[0]) == 0);
}

static vision_stable_slot_t *slot_from_upload(const vision_upload_frame_t *frame)
{
    if (!frame) {
        return NULL;
    }
    for (int i = 0; i < STABLE_SLOT_COUNT; i++) {
        if (&s_slots[i].upload == frame) {
            return &s_slots[i];
        }
    }
    return NULL;
}

static int clamp_int(int value, int low, int high)
{
    if (value < low) {
        return low;
    }
    if (value > high) {
        return high;
    }
    return value;
}

static uint16_t score_to_x1000(float score)
{
    if (score <= 0.0f) {
        return 0;
    }
    if (score >= 1.0f) {
        return 1000;
    }
    return (uint16_t)(score * 1000.0f + 0.5f);
}

static void reset_upload_locked(vision_stable_slot_t *slot)
{
    memset(&slot->upload, 0, sizeof(slot->upload));
    slot->upload.rgb888 = slot->pixels;
    slot->upload.rgb_len = VISION_UPLOAD_RGB_BYTES;
    slot->upload.width = VISION_UPLOAD_WIDTH;
    slot->upload.height = VISION_UPLOAD_HEIGHT;
    slot->upload.slot_token = slot;
}

esp_err_t vision_stable_frame_init(void)
{
    if (s_slot_mutex || s_copy_ppa) {
        return ESP_ERR_INVALID_STATE;
    }

    memset(s_slots, 0, sizeof(s_slots));
    s_slot_mutex = xSemaphoreCreateMutex();
    if (!s_slot_mutex) {
        return ESP_ERR_NO_MEM;
    }

    ppa_client_config_t ppa_cfg = { .oper_type = PPA_OPERATION_SRM };
    esp_err_t ret = ppa_register_client(&ppa_cfg, &s_copy_ppa);
    if (ret != ESP_OK) {
        vSemaphoreDelete(s_slot_mutex);
        s_slot_mutex = NULL;
        return ret;
    }
    ESP_LOGI(TAG, "PPA copy client registered");

    for (int i = 0; i < STABLE_SLOT_COUNT; i++) {
        s_slots[i].pixels = heap_caps_aligned_alloc(STABLE_ALIGNMENT, VISION_UPLOAD_RGB_BYTES,
                                                    MALLOC_CAP_SPIRAM | MALLOC_CAP_DMA | MALLOC_CAP_8BIT);
        if (!s_slots[i].pixels) {
            ret = ESP_ERR_NO_MEM;
            for (int j = i - 1; j >= 0; j--) {
                heap_caps_free(s_slots[j].pixels);
                s_slots[j].pixels = NULL;
            }
            ppa_unregister_client(s_copy_ppa);
            s_copy_ppa = NULL;
            vSemaphoreDelete(s_slot_mutex);
            s_slot_mutex = NULL;
            return ret;
        }
        s_slots[i].state = STABLE_SLOT_FREE;
        reset_upload_locked(&s_slots[i]);
    }

    ESP_LOGI(TAG, "allocated %d stable RGB slots (%u bytes each)",
             STABLE_SLOT_COUNT, (unsigned)VISION_UPLOAD_RGB_BYTES);
    return ESP_OK;
}

vision_stable_slot_t *vision_stable_frame_acquire(void)
{
    if (!s_slot_mutex) {
        return NULL;
    }

    vision_stable_slot_t *slot = NULL;
    xSemaphoreTake(s_slot_mutex, portMAX_DELAY);
    for (int i = 0; i < STABLE_SLOT_COUNT; i++) {
        if (s_slots[i].state == STABLE_SLOT_FREE &&
            transition_slot_locked(&s_slots[i], STABLE_SLOT_COPYING)) {
            reset_upload_locked(&s_slots[i]);
            slot = &s_slots[i];
            break;
        }
    }
    xSemaphoreGive(s_slot_mutex);
    if (!slot) {
        atomic_fetch_add(&s_rgb_slot_drop, 1);
    }
    return slot;
}

esp_err_t vision_stable_frame_copy_from_ref(vision_stable_slot_t *slot,
                                             const vision_frame_ref_t *ref)
{
    if (!slot_pointer_valid(slot) || !ref || !ref->acquired || !ref->frame.buf || !s_slot_mutex || !s_copy_ppa) {
        return ESP_ERR_INVALID_ARG;
    }
    if (ref->frame.width != VISION_UPLOAD_WIDTH || ref->frame.height != VISION_UPLOAD_HEIGHT ||
        ref->frame.len < VISION_UPLOAD_RGB_BYTES) {
        return ESP_ERR_INVALID_SIZE;
    }

    xSemaphoreTake(s_slot_mutex, portMAX_DELAY);
    if (slot->state != STABLE_SLOT_COPYING) {
        xSemaphoreGive(s_slot_mutex);
        return ESP_ERR_INVALID_STATE;
    }

    ppa_srm_oper_config_t cfg = {
        .in = {
            .buffer = (void *)ref->frame.buf,
            .pic_w = VISION_UPLOAD_WIDTH,
            .pic_h = VISION_UPLOAD_HEIGHT,
            .block_w = VISION_UPLOAD_WIDTH,
            .block_h = VISION_UPLOAD_HEIGHT,
            .srm_cm = PPA_SRM_COLOR_MODE_RGB888,
        },
        .out = {
            .buffer = slot->pixels,
            .buffer_size = VISION_UPLOAD_RGB_BYTES,
            .pic_w = VISION_UPLOAD_WIDTH,
            .pic_h = VISION_UPLOAD_HEIGHT,
            .srm_cm = PPA_SRM_COLOR_MODE_RGB888,
        },
        .rotation_angle = PPA_SRM_ROTATION_ANGLE_0,
        .scale_x = 1.0f,
        .scale_y = 1.0f,
        .mode = PPA_TRANS_MODE_BLOCKING,
    };

    esp_err_t ret = ppa_do_scale_rotate_mirror(s_copy_ppa, &cfg);
    if (ret == ESP_OK) {
        atomic_fetch_add(&s_ppa_copy_ok, 1);
        slot->upload.frame_id = ref->frame.frame_id;
        slot->upload.capture_timestamp_us = ref->frame.timestamp;
        slot->upload.width = ref->frame.width;
        slot->upload.height = ref->frame.height;
        slot->upload.rgb_len = VISION_UPLOAD_RGB_BYTES;
    } else {
        atomic_fetch_add(&s_ppa_copy_fail, 1);
    }
    xSemaphoreGive(s_slot_mutex);
    return ret;
}

uint8_t *vision_stable_frame_pixels(vision_stable_slot_t *slot)
{
    if (!slot_pointer_valid(slot)) {
        return NULL;
    }
    return slot->pixels;
}

bool vision_stable_frame_set_inferencing(vision_stable_slot_t *slot)
{
    if (!slot_pointer_valid(slot) || !s_slot_mutex) {
        return false;
    }
    xSemaphoreTake(s_slot_mutex, portMAX_DELAY);
    bool ok = transition_slot_locked(slot, STABLE_SLOT_INFERENCING);
    xSemaphoreGive(s_slot_mutex);
    return ok;
}

void vision_upload_set_submit_callback(vision_upload_submit_cb_t cb, void *ctx)
{
    portENTER_CRITICAL(&s_callback_lock);
    s_submit_cb = cb;
    s_submit_ctx = ctx;
    portEXIT_CRITICAL(&s_callback_lock);
}

bool vision_stable_frame_submit(vision_stable_slot_t *slot,
                                const vision_model_det_t *dets,
                                int det_count,
                                uint16_t primary_class_id,
                                uint16_t primary_confidence_x1000,
                                uint16_t infer_time_ms)
{
    if (!slot_pointer_valid(slot) || !s_slot_mutex || det_count < 0 || (det_count > 0 && !dets)) {
        return false;
    }

    vision_upload_submit_cb_t cb;
    void *ctx;
    xSemaphoreTake(s_slot_mutex, portMAX_DELAY);
    if (slot->state != STABLE_SLOT_INFERENCING) {
        transition_slot_locked(slot, STABLE_SLOT_ENCODE_PENDING);
        xSemaphoreGive(s_slot_mutex);
        return false;
    }

    slot->upload.box_count = 0;
    int limit = det_count < VISION_UPLOAD_MAX_BOXES ? det_count : VISION_UPLOAD_MAX_BOXES;
    for (int i = 0; i < limit; i++) {
        int x1 = clamp_int(dets[i].box[0], 0, VISION_UPLOAD_WIDTH - 1);
        int y1 = clamp_int(dets[i].box[1], 0, VISION_UPLOAD_HEIGHT - 1);
        int x2 = clamp_int(dets[i].box[2], 0, VISION_UPLOAD_WIDTH - 1);
        int y2 = clamp_int(dets[i].box[3], 0, VISION_UPLOAD_HEIGHT - 1);
        if (x1 > x2 || y1 > y2) {
            continue;
        }
        vision_upload_box_t *box = &slot->upload.boxes[slot->upload.box_count++];
        box->stage = (uint8_t)clamp_int((int)dets[i].stage, 0, UINT8_MAX);
        box->category = (uint8_t)clamp_int(dets[i].category, 0, UINT8_MAX);
        box->score_x1000 = score_to_x1000(dets[i].score);
        box->x1 = (uint16_t)x1;
        box->y1 = (uint16_t)y1;
        box->x2 = (uint16_t)x2;
        box->y2 = (uint16_t)y2;
    }
    slot->upload.primary_class_id = primary_class_id;
    slot->upload.primary_confidence_x1000 = primary_confidence_x1000 > 1000 ? 1000 : primary_confidence_x1000;
    slot->upload.infer_time_ms = infer_time_ms;
    if (!transition_slot_locked(slot, STABLE_SLOT_ENCODE_PENDING)) {
        xSemaphoreGive(s_slot_mutex);
        return false;
    }
    xSemaphoreGive(s_slot_mutex);

    portENTER_CRITICAL(&s_callback_lock);
    cb = s_submit_cb;
    ctx = s_submit_ctx;
    portEXIT_CRITICAL(&s_callback_lock);

    bool accepted = cb && cb(&slot->upload, ctx);
    if (!accepted) {
        xSemaphoreTake(s_slot_mutex, portMAX_DELAY);
        if (slot->state == STABLE_SLOT_ENCODE_PENDING) {
            transition_slot_locked(slot, STABLE_SLOT_FREE);
        } else {
            ESP_LOGE(TAG, "submit rejected after slot state advanced to %d", slot->state);
        }
        xSemaphoreGive(s_slot_mutex);
        atomic_fetch_add(&s_upload_rejected, 1);
        return false;
    }
    return true;
}

bool vision_stable_frame_discard(vision_stable_slot_t *slot)
{
    if (!slot_pointer_valid(slot) || !s_slot_mutex) {
        return false;
    }
    xSemaphoreTake(s_slot_mutex, portMAX_DELAY);
    bool ok = transition_slot_locked(slot, STABLE_SLOT_FREE);
    xSemaphoreGive(s_slot_mutex);
    return ok;
}

bool vision_upload_frame_begin_encoding(const vision_upload_frame_t *frame)
{
    vision_stable_slot_t *slot = slot_from_upload(frame);
    if (!slot || !s_slot_mutex) {
        ESP_LOGE(TAG, "invalid upload frame token for begin_encoding");
        return false;
    }

    bool ok = false;
    xSemaphoreTake(s_slot_mutex, portMAX_DELAY);
    if (slot->upload.slot_token == slot) {
        ok = transition_slot_locked(slot, STABLE_SLOT_ENCODING);
    } else {
        ESP_LOGE(TAG, "invalid upload slot token for begin_encoding");
    }
    xSemaphoreGive(s_slot_mutex);
    return ok;
}

bool vision_upload_frame_release(const vision_upload_frame_t *frame)
{
    vision_stable_slot_t *slot = slot_from_upload(frame);
    if (!slot || !s_slot_mutex) {
        ESP_LOGE(TAG, "invalid upload frame token for release");
        return false;
    }

    bool ok = false;
    xSemaphoreTake(s_slot_mutex, portMAX_DELAY);
    if (slot->upload.slot_token == slot) {
        ok = transition_slot_locked(slot, STABLE_SLOT_FREE);
    } else {
        ESP_LOGE(TAG, "invalid upload slot token for release");
    }
    xSemaphoreGive(s_slot_mutex);
    return ok;
}
