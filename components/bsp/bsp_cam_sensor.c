/*
 * MIPI CSI 摄像头驱动 — ESP32-P4 平台
 *
 * 硬件数据路径：
 *   Sensor ──MIPI CSI-2──> CSI RX Controller ──> ISP(Demosaic+RGB888) ──DMA──> PSRAM 帧缓冲
 *
 * 支持的传感器（idf.py menuconfig → Espressif Camera Sensors 选择）：
 *   SC2336   2MP RAW8,  MIPI 2-lane（默认）
 *   OV5647   5MP RAW8,  MIPI 2-lane
 *   OV5645   5MP YUV/RGB, MIPI 2-lane
 *   OV5640   5MP RGB/JPEG, MIPI 2-lane
 *   OS02N10  2MP RAW8/RAW10, MIPI 2-lane
 *   SC035HGS 0.3MP RAW8, MIPI 1/2-lane
 *
 * 输出：RGB888（3 字节/像素），存储在 PSRAM 中。
 *
 * 缓冲管理：两帧交替，一帧被 capture_task 写入，另一帧被主任务读取。
 * 帧传递使用 esp_cam_ctlr_receive()（阻塞模式，与官方示例一致）。
 */

#include <string.h>

#include "bsp_cam_sensor.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include "esp_check.h"
#include "esp_heap_caps.h"
#include "esp_timer.h"
#include "esp_private/esp_cache_private.h"
#include "esp_ldo_regulator.h"
#include "driver/i2c_master.h"
#include "esp_cam_ctlr_csi.h"
#include "esp_cam_ctlr.h"
#include "driver/isp.h"

#include "esp_sccb_intf.h"
#include "esp_sccb_i2c.h"
#include "esp_cam_sensor.h"
#include "esp_cam_sensor_detect.h"

/* ============================================================
 * 传感器型号选择（menuconfig 控制）
 * ============================================================ */
#if CONFIG_CAMERA_SC2336
#  include "sc2336.h"
#  define CAM_SCCB_ADDR   SC2336_SCCB_ADDR
#  define CAM_SENSOR_NAME "SC2336"
#elif CONFIG_CAMERA_OV5647
#  include "ov5647.h"
#  define CAM_SCCB_ADDR   OV5647_SCCB_ADDR
#  define CAM_SENSOR_NAME "OV5647"
#elif CONFIG_CAMERA_OV5645
#  include "ov5645.h"
#  define CAM_SCCB_ADDR   OV5645_SCCB_ADDR
#  define CAM_SENSOR_NAME "OV5645"
#elif CONFIG_CAMERA_OV5640
#  include "ov5640.h"
#  define CAM_SCCB_ADDR   OV5640_SCCB_ADDR
#  define CAM_SENSOR_NAME "OV5640"
#elif CONFIG_CAMERA_OS02N10
#  include "os02n10.h"
#  define CAM_SCCB_ADDR   OS02N10_SCCB_ADDR
#  define CAM_SENSOR_NAME "OS02N10"
#elif CONFIG_CAMERA_SC035HGS
#  include "sc035hgs.h"
#  define CAM_SCCB_ADDR   SC035HGS_SCCB_ADDR
#  define CAM_SENSOR_NAME "SC035HGS"
#else
#  error "No MIPI CSI sensor selected. idf.py menuconfig → Espressif Camera Sensors"
#endif

#define TAG "cam_sensor"

/* ============================================================
 * [用户配置] MIPI D-PHY LDO
 * ESP32-P4 MIPI D-PHY 需要 2.5V 独立供电。
 * 默认使用内部 LDO 通道 3。若板卡用外部 LDO，此处改为 -1。
 * ============================================================ */
#define MIPI_LDO_CHAN      3
#define MIPI_LDO_MV     2500

/* 输出每像素字节数：RGB888 = 3 */
#define OUT_BPP 3

#define ALIGN_UP(num, align) (((num) + ((align) - 1)) & ~((align) - 1))

/* ============================================================
 * [用户配置] 捕获任务参数
 *
 * CAP_TASK_STACK_DEPTH — 任务栈大小（字节）
 *   增加：栈溢出时可提高
 *   减少：节省内存
 *
 * CAP_TASK_PRIORITY — 任务优先级
 *   帧丢失时提高此值，画面卡顿时降低
 * ============================================================ */
#define CAP_TASK_STACK_DEPTH  4096
#define CAP_TASK_PRIORITY     10
#define CAP_TASK_CORE         0

/* ------------------------------------------------------------------ */
/*  内部状态结构体                                                      */
/* ------------------------------------------------------------------ */

/** CSI 传输完成回调 — 必须返回 false（官方要求） */
static bool on_trans_finished_cb(esp_cam_ctlr_handle_t handle,
                                  esp_cam_ctlr_trans_t *trans,
                                  void *user_data)
{
    return false;
}

typedef struct {
    i2c_master_bus_handle_t   i2c_bus;       /* I2C 主机总线 */
    bool                      owns_i2c_bus;  /* 是否由本组件创建 I2C 总线 */
    esp_sccb_io_handle_t      sccb_io;       /* SCCB (I2C 子集) IO 句柄 */
    esp_cam_sensor_device_t  *sensor;        /* 传感器设备对象 */
    esp_cam_ctlr_handle_t     csi_handle;    /* MIPI CSI 控制器句柄 */
    isp_proc_handle_t         isp_handle;    /* ISP 处理器句柄 */
    esp_ldo_channel_handle_t  ldo_handle;    /* MIPI PHY LDO 句柄 */

    uint8_t    *frame_bufs[CONFIG_CAM_SENSOR_FB_COUNT]; /* 帧缓冲数组（PSRAM） */
    size_t      frame_size;       /* 单帧字节数 = width × height × 3 */
    size_t      frame_buf_size;   /* CSI DMA 缓冲字节数，按 cache line 对齐 */
    size_t      cache_line_size;  /* PSRAM cache line 对齐要求 */
    int         frame_width;      /* 帧宽度 */
    int         frame_height;     /* 帧高度 */

    /* 帧缓冲轮转索引
     *   latest_buf_idx — 最近完成的帧所在缓冲区
     *   app_buf_idx    — 应用层正在使用的缓冲区（-1 = 已归还） */
    volatile int   latest_buf_idx;
    volatile int   app_buf_idx;

    SemaphoreHandle_t frame_sem;       /* 新帧就绪信号量 */
    SemaphoreHandle_t app_mutex;       /* 串行化多个应用消费者的 get/return */
    TaskHandle_t      capture_task;    /* 后台 CSI 捕获任务句柄 */
    TaskHandle_t      stop_notif_task; /* 停止通知接收任务句柄 */

    portMUX_TYPE      buf_mux;         /* 保护 latest_buf_idx / app_buf_idx */
    bool        initialized;           /* init 已完成 */
    bool        streaming;             /* 正在推流 */
} cam_ctx_t;

static cam_ctx_t s_ctx;

/* ------------------------------------------------------------------ */
/*  帧缓冲轮转 — 三缓冲管理                                            */
/* ------------------------------------------------------------------ */

/**
 * 查找空闲缓冲区：既不是最新完成帧，也不是应用层正持有的帧。
 *
 * 三缓冲机制确保：
 *   - Buf A: CSI 正在写入
 *   - Buf B: 最新完成的帧（等待应用层取走）
 *   - Buf C: 空闲，准备接收下一帧
 */
static inline int find_free_buf(const cam_ctx_t *ctx)
{
    for (int i = 0; i < CONFIG_CAM_SENSOR_FB_COUNT; i++) {
        if (i != ctx->latest_buf_idx && i != ctx->app_buf_idx)
            return i;
    }
    return 0; /* 三缓冲时不应到达此处 */
}

/* ------------------------------------------------------------------ */
/*  后台捕获任务 — 持续排空 CSI 队列                                     */
/* ------------------------------------------------------------------ */

static void capture_task_fn(void *arg)
{
    cam_ctx_t *ctx = (cam_ctx_t *)arg;

    while (ctx->streaming) {
        /* 临界区：读取索引，选择空闲缓冲区 */
        taskENTER_CRITICAL(&ctx->buf_mux);
        int capture_idx = find_free_buf(ctx);
        taskEXIT_CRITICAL(&ctx->buf_mux);

        esp_cam_ctlr_trans_t trans = {
            .buffer = ctx->frame_bufs[capture_idx],
            .buflen = ctx->frame_buf_size,
        };

        int64_t t_receive_start = esp_timer_get_time();
        esp_err_t ret = esp_cam_ctlr_receive(ctx->csi_handle, &trans, 200);
        int64_t receive_us = esp_timer_get_time() - t_receive_start;
        static uint32_t frame_count;
        static uint32_t timeout_count;
        static int64_t max_receive_us;
        if (ret == ESP_OK) {
            frame_count++;
            if (receive_us > max_receive_us) {
                max_receive_us = receive_us;
            }
            /* 临界区：原子更新 latest_buf_idx 并通知主任务 */
            taskENTER_CRITICAL(&ctx->buf_mux);
            ctx->latest_buf_idx = capture_idx;
            taskEXIT_CRITICAL(&ctx->buf_mux);
            xSemaphoreGive(ctx->frame_sem);
            if (frame_count <= 5 || (frame_count % 120) == 0 || receive_us > 100000) {
                // 暂时屏蔽
                // ESP_LOGI(TAG,
                //          "capture #%lu receive=%lldms max=%lldms timeouts=%lu core=%d prio=%u stack_hwm=%u",
                //          (unsigned long)frame_count,
                //          receive_us / 1000,
                //          max_receive_us / 1000,
                //          (unsigned long)timeout_count,
                //          xPortGetCoreID(),
                //          (unsigned)uxTaskPriorityGet(NULL),
                //          (unsigned)uxTaskGetStackHighWaterMark(NULL));
            }
        } else if (ret == ESP_ERR_TIMEOUT) {
            timeout_count++;
            if (timeout_count <= 5 || (timeout_count % 30) == 0) {
                ESP_LOGW(TAG, "capture timeout count=%lu core=%d prio=%u stack_hwm=%u",
                         (unsigned long)timeout_count,
                         xPortGetCoreID(),
                         (unsigned)uxTaskPriorityGet(NULL),
                         (unsigned)uxTaskGetStackHighWaterMark(NULL));
            }
        } else {
            ESP_LOGW(TAG, "capture receive failed: %s", esp_err_to_name(ret));
        }
        /* timeout 或 error → 立即重试 */
    }

    /* 通知 cam_sensor_stop() 捕获任务已退出 */
    if (ctx->stop_notif_task) {
        xTaskNotify(ctx->stop_notif_task, 1, eSetValueWithOverwrite);
    }
    vTaskDelete(NULL);
}

/* ============================================================
 *  公开 API
 * ============================================================ */

esp_err_t cam_sensor_init(const cam_sensor_config_t *config)
{
    if (s_ctx.initialized) return ESP_ERR_INVALID_STATE;
    ESP_RETURN_ON_FALSE(config, ESP_ERR_INVALID_ARG, TAG, "config is NULL");

    esp_err_t ret = ESP_OK;
    memset(&s_ctx, 0, sizeof(s_ctx));
    portMUX_INITIALIZE(&s_ctx.buf_mux);
    s_ctx.app_mutex = xSemaphoreCreateMutex();
    ESP_RETURN_ON_FALSE(s_ctx.app_mutex, ESP_ERR_NO_MEM, TAG, "create app mutex failed");

    /* ---- 0. MIPI PHY LDO（必须最先初始化）------------------------ */
    esp_ldo_channel_config_t ldo_cfg = {
        .chan_id    = MIPI_LDO_CHAN,
        .voltage_mv = MIPI_LDO_MV,
    };
    ret = esp_ldo_acquire_channel(&ldo_cfg, &s_ctx.ldo_handle);
    if (ret != ESP_OK) {
        /* 部分板卡使用固定外部 LDO，警告但不中止 */
        ESP_LOGW(TAG, "LDO ch%d acquire failed (%s) — assuming external MIPI power",
                 MIPI_LDO_CHAN, esp_err_to_name(ret));
        s_ctx.ldo_handle = NULL;
    } else {
        ESP_LOGI(TAG, "MIPI PHY LDO ch%d = %d mV", MIPI_LDO_CHAN, MIPI_LDO_MV);
    }

    /* ---- 1. I2C 主机总线（SCCB 通信）------------------------------ */
    if (config->i2c_bus) {
        s_ctx.i2c_bus = config->i2c_bus;
        s_ctx.owns_i2c_bus = false;
        ESP_LOGI(TAG, "Using shared I2C bus for SCCB");
    } else {
        i2c_master_bus_config_t i2c_cfg = {
            .clk_source          = I2C_CLK_SRC_DEFAULT,
            .i2c_port            = I2C_NUM_0,
            .scl_io_num          = config->i2c_scl_pin,
            .sda_io_num          = config->i2c_sda_pin,
            .glitch_ignore_cnt   = 7,
            .flags               = { .enable_internal_pullup = true },
        };
        ESP_GOTO_ON_ERROR(i2c_new_master_bus(&i2c_cfg, &s_ctx.i2c_bus),
                          err, TAG, "i2c_new_master_bus failed");
        s_ctx.owns_i2c_bus = true;
    }

    /* ---- 2. SCCB I/O 句柄 --------------------------------------- */
    sccb_i2c_config_t sccb_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address  = CAM_SCCB_ADDR,
        .scl_speed_hz    = 100000,
    };
    ESP_GOTO_ON_ERROR(sccb_new_i2c_io(s_ctx.i2c_bus, &sccb_cfg, &s_ctx.sccb_io),
                      err_i2c, TAG, "sccb_new_i2c_io failed");

    /* ---- 3. 上电并检测传感器 ----------------------------------- */
    esp_cam_sensor_config_t cam_cfg = {
        .sccb_handle  = s_ctx.sccb_io,
        .reset_pin    = config->reset_pin,
        .pwdn_pin     = config->pwdn_pin,
        .xclk_pin     = -1,
        .xclk_freq_hz = 0,
        .sensor_port  = ESP_CAM_SENSOR_MIPI_CSI,
    };

#if CONFIG_CAMERA_SC2336
    s_ctx.sensor = sc2336_detect(&cam_cfg);
#elif CONFIG_CAMERA_OV5647
    s_ctx.sensor = ov5647_detect(&cam_cfg);
#elif CONFIG_CAMERA_OV5645
    s_ctx.sensor = ov5645_detect(&cam_cfg);
#elif CONFIG_CAMERA_OV5640
    s_ctx.sensor = ov5640_detect(&cam_cfg);
#elif CONFIG_CAMERA_OS02N10
    s_ctx.sensor = os02n10_detect(&cam_cfg);
#elif CONFIG_CAMERA_SC035HGS
    s_ctx.sensor = sc035hgs_detect(&cam_cfg);
#endif

    if (!s_ctx.sensor) {
        ESP_LOGE(TAG, "Sensor " CAM_SENSOR_NAME " not detected");
        ret = ESP_ERR_NOT_FOUND;
        goto err_sccb;
    }
    ESP_LOGI(TAG, "Detected: %s", esp_cam_sensor_get_name(s_ctx.sensor));

    /* ---- 4. 选择输出格式 — 匹配用户在 main.cpp 中设置的分辨率 ---- */
    esp_cam_sensor_format_array_t fmts = {};
    ESP_GOTO_ON_ERROR(esp_cam_sensor_query_format(s_ctx.sensor, &fmts),
                      err_sensor, TAG, "query_format failed");

    const esp_cam_sensor_format_t *picked = &fmts.format_array[0];
    bool fmt_found = false;
    for (int i = 0; i < (int)fmts.count; i++) {
        const esp_cam_sensor_format_t *f = &fmts.format_array[i];
        if (f->port   == ESP_CAM_SENSOR_MIPI_CSI &&
            f->width  == (uint16_t)config->frame_width &&
            f->height == (uint16_t)config->frame_height) {
            picked = f;
            fmt_found = true;
            break;
        }
    }
    if (!fmt_found) {
        ESP_LOGW(TAG, "请求分辨率 %dx%d 未找到，回退至 %ux%u",
                 config->frame_width, config->frame_height,
                 picked->width, picked->height);
    }
    ESP_LOGI(TAG, "Format: %s  %ux%u  %ufps", picked->name,
             picked->width, picked->height, picked->fps);
    ESP_GOTO_ON_ERROR(esp_cam_sensor_set_format(s_ctx.sensor, picked),
                      err_sensor, TAG, "set_format failed");

    s_ctx.frame_width  = picked->width;
    s_ctx.frame_height = picked->height;
    s_ctx.frame_size   = (size_t)picked->width * picked->height * OUT_BPP;
    ESP_GOTO_ON_ERROR(esp_cache_get_alignment(MALLOC_CAP_SPIRAM, &s_ctx.cache_line_size),
                      err_sensor, TAG, "esp_cache_get_alignment failed");
    s_ctx.frame_buf_size = ALIGN_UP(s_ctx.frame_size, s_ctx.cache_line_size);

    /* ---- 5. MIPI CSI 控制器（在 ISP 之前创建并启用）------------- */
    /* 注意：input_data_color_type 必须与传感器实际输出格式一致。
     *       SC2336 输出 RAW8，因此设为 CAM_CTLR_COLOR_RAW8。
     *       若更换为输出 RGB 的传感器，需改为 CAM_CTLR_COLOR_RGB888。 */
    esp_cam_ctlr_csi_config_t csi_cfg = {
        .ctlr_id                = 0,
        .clk_src                = MIPI_CSI_PHY_CLK_SRC_DEFAULT,
        .h_res                  = s_ctx.frame_width,
        .v_res                  = s_ctx.frame_height,
        .data_lane_num          = (int)picked->mipi_info.lane_num,
        .lane_bit_rate_mbps     = picked->mipi_info.mipi_clk / 1000000,
        .input_data_color_type  = CAM_CTLR_COLOR_RAW8,
        .output_data_color_type = CAM_CTLR_COLOR_RAW8,  /* 保持 RAW8，IOS 做 demosaic */
        .byte_swap_en           = false,
        .queue_items            = 1,
    };
    ESP_GOTO_ON_ERROR(esp_cam_new_csi_ctlr(&csi_cfg, &s_ctx.csi_handle),
                      err_sensor, TAG, "esp_cam_new_csi_ctlr failed");

    /* ---- 6. 在 PSRAM 中分配帧缓冲 ------------------------------ */
    for (int i = 0; i < CONFIG_CAM_SENSOR_FB_COUNT; i++) {
        s_ctx.frame_bufs[i] = (uint8_t *)heap_caps_aligned_alloc(
                                    s_ctx.cache_line_size, s_ctx.frame_buf_size, MALLOC_CAP_SPIRAM);
        if (!s_ctx.frame_bufs[i]) {
            ESP_LOGE(TAG, "PSRAM alloc failed for buf[%d] (need %u KB)",
                     i, (unsigned)(s_ctx.frame_buf_size / 1024));
            ret = ESP_ERR_NO_MEM;
            goto err_bufs;
        }
    }
    ESP_LOGI(TAG, "%d × %u KB RGB888 frame buffers in PSRAM (cache line %uB)",
             CONFIG_CAM_SENSOR_FB_COUNT,
             (unsigned)(s_ctx.frame_buf_size / 1024),
             (unsigned)s_ctx.cache_line_size);
    s_ctx.latest_buf_idx = 0;
    s_ctx.app_buf_idx    = -1;

    /* ---- 7. 注册回调 ------------------------------------------- */
    /* 使用 esp_cam_ctlr_receive() 时 on_get_new_trans 必须为 NULL */
    esp_cam_ctlr_evt_cbs_t cbs = {
        .on_get_new_trans  = NULL,
        .on_trans_finished = on_trans_finished_cb,
    };
    ESP_GOTO_ON_ERROR(
        esp_cam_ctlr_register_event_callbacks(s_ctx.csi_handle, &cbs, &s_ctx),
        err_bufs, TAG, "register_event_callbacks failed");

    /* ---- 8. 启用 CSI 控制器（必须在 ISP 之前） ----------------- */
    ESP_GOTO_ON_ERROR(esp_cam_ctlr_enable(s_ctx.csi_handle),
                      err_bufs, TAG, "esp_cam_ctlr_enable failed");

    /* ---- 9. ISP 处理器（CSI 启用后，CSI 启动前）---------------- */
    esp_isp_processor_cfg_t isp_cfg = {
        .clk_src                = ISP_CLK_SRC_DEFAULT,
        .clk_hz                 = 80 * 1000000,
        .input_data_source      = ISP_INPUT_DATA_SOURCE_CSI,
        .input_data_color_type  = ISP_COLOR_RAW8,
        .output_data_color_type = ISP_COLOR_RGB888,
        .has_line_start_packet  = false,
        .has_line_end_packet    = false,
        .h_res                  = (uint32_t)s_ctx.frame_width,
        .v_res                  = (uint32_t)s_ctx.frame_height,
        /* ============================================================
         * [用户配置] Bayer 阵列顺序
         * 必须与传感器数据手册一致，否则颜色错误。
         *   SC2336 → BGGR
         *   OV5647 → RGGB
         *   其他 → 查阅数据手册
         * ============================================================ */
#if CONFIG_CAMERA_SC2336
        .bayer_order            = COLOR_RAW_ELEMENT_ORDER_BGGR,
#elif CONFIG_CAMERA_OV5647
        .bayer_order            = COLOR_RAW_ELEMENT_ORDER_RGGB,
#else
        .bayer_order            = COLOR_RAW_ELEMENT_ORDER_BGGR,
#endif
    };
    ESP_GOTO_ON_ERROR(esp_isp_new_processor(&isp_cfg, &s_ctx.isp_handle),
                      err_csi_en, TAG, "esp_isp_new_processor failed");
    ESP_GOTO_ON_ERROR(esp_isp_enable(s_ctx.isp_handle),
                      err_isp, TAG, "esp_isp_enable failed");

    s_ctx.initialized = true;
    ESP_LOGI(TAG, "cam_sensor_init OK  [%dx%d RGB888, %u KB/frame]",
             s_ctx.frame_width, s_ctx.frame_height,
             (unsigned)(s_ctx.frame_size / 1024));
    return ESP_OK;

    /* ---- 错误回滚：按初始化逆序清理资源 ------------------------ */
err_isp:
    esp_isp_del_processor(s_ctx.isp_handle); s_ctx.isp_handle = NULL;
err_csi_en:
    esp_cam_ctlr_disable(s_ctx.csi_handle);
err_bufs:
    for (int i = 0; i < CONFIG_CAM_SENSOR_FB_COUNT; i++) {
        if (s_ctx.frame_bufs[i]) { heap_caps_free(s_ctx.frame_bufs[i]); s_ctx.frame_bufs[i] = NULL; }
    }
    esp_cam_ctlr_del(s_ctx.csi_handle); s_ctx.csi_handle = NULL;
err_sensor:
    esp_cam_sensor_del_dev(s_ctx.sensor); s_ctx.sensor = NULL;
err_sccb:
    esp_sccb_del_i2c_io(s_ctx.sccb_io); s_ctx.sccb_io = NULL;
err_i2c:
    if (s_ctx.owns_i2c_bus) {
        i2c_del_master_bus(s_ctx.i2c_bus);
    }
    s_ctx.i2c_bus = NULL;
err:
    if (s_ctx.ldo_handle) { esp_ldo_release_channel(s_ctx.ldo_handle); s_ctx.ldo_handle = NULL; }
    if (s_ctx.app_mutex) { vSemaphoreDelete(s_ctx.app_mutex); s_ctx.app_mutex = NULL; }
    return ret;
}

esp_err_t cam_sensor_start(void)
{
    ESP_RETURN_ON_FALSE(s_ctx.initialized, ESP_ERR_INVALID_STATE, TAG, "not initialized");
    ESP_RETURN_ON_FALSE(!s_ctx.streaming,  ESP_ERR_INVALID_STATE, TAG, "already streaming");

    /* 帧就绪通知信号量 */
    s_ctx.frame_sem = xSemaphoreCreateBinary();
    if (!s_ctx.frame_sem) {
        ESP_LOGE(TAG, "Failed to create frame semaphore");
        return ESP_ERR_NO_MEM;
    }

    s_ctx.latest_buf_idx = 0;
    s_ctx.app_buf_idx    = -1;

    /* 启动 CSI 控制器 → 启动传感器推流 → 启动后台捕获任务 */
    ESP_RETURN_ON_ERROR(esp_cam_ctlr_start(s_ctx.csi_handle), TAG, "ctlr_start failed");

    int stream_on = 1;
    ESP_RETURN_ON_ERROR(
        esp_cam_sensor_ioctl(s_ctx.sensor, ESP_CAM_SENSOR_IOC_S_STREAM, &stream_on),
        TAG, "sensor stream-on failed");

    s_ctx.streaming = true;

    if (xTaskCreatePinnedToCore(capture_task_fn, "cam_cap", CAP_TASK_STACK_DEPTH,
                                &s_ctx, CAP_TASK_PRIORITY, &s_ctx.capture_task,
                                CAP_TASK_CORE) != pdPASS) {
        s_ctx.streaming = false;
        esp_cam_ctlr_stop(s_ctx.csi_handle);
        vSemaphoreDelete(s_ctx.frame_sem);
        s_ctx.frame_sem = NULL;
        ESP_LOGE(TAG, "Failed to create capture task");
        return ESP_ERR_NO_MEM;
    }

    ESP_LOGI(TAG, "Streaming started (capture task priority=%d core=%d)",
             CAP_TASK_PRIORITY, CAP_TASK_CORE);
    return ESP_OK;
}

/**
 * 停止推流 — 安全等待后台捕获任务退出（最多 400ms），超时则强制删除。
 */
esp_err_t cam_sensor_stop(void)
{
    ESP_RETURN_ON_FALSE(s_ctx.streaming, ESP_ERR_INVALID_STATE, TAG, "not streaming");

    s_ctx.stop_notif_task = xTaskGetCurrentTaskHandle();
    s_ctx.streaming = false;

    if (s_ctx.capture_task) {
        if (ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(400)) == 0) {
            ESP_LOGW(TAG, "Capture task 未在超时内退出，强制删除");
            vTaskDelete(s_ctx.capture_task);
        }
        s_ctx.capture_task    = NULL;
        s_ctx.stop_notif_task = NULL;
    }

    int off = 0;
    esp_cam_sensor_ioctl(s_ctx.sensor, ESP_CAM_SENSOR_IOC_S_STREAM, &off);
    esp_cam_ctlr_stop(s_ctx.csi_handle);

    if (s_ctx.frame_sem) {
        vSemaphoreDelete(s_ctx.frame_sem);
        s_ctx.frame_sem = NULL;
    }

    return ESP_OK;
}

/**
 * 获取帧 — 阻塞等待新帧就绪。
 *
 * @param timeout_ms  0 = 永久等待；>0 = 超时返回 ESP_ERR_TIMEOUT
 *
 * 调用者必须在处理完毕后调用 cam_sensor_return_frame() 释放帧。
 */
esp_err_t cam_sensor_get_frame(uint8_t **data, size_t *size,
                                int *width, int *height,
                                uint32_t timeout_ms)
{
    ESP_RETURN_ON_FALSE(s_ctx.streaming, ESP_ERR_INVALID_STATE, TAG, "not streaming");
    ESP_RETURN_ON_FALSE(data,            ESP_ERR_INVALID_ARG,   TAG, "data is NULL");

    TickType_t ticks = (timeout_ms == 0) ? portMAX_DELAY
                                          : pdMS_TO_TICKS(timeout_ms);
    if (xSemaphoreTake(s_ctx.app_mutex, ticks) != pdTRUE) {
        return ESP_ERR_TIMEOUT;
    }

    if (xSemaphoreTake(s_ctx.frame_sem, ticks) != pdTRUE) {
        xSemaphoreGive(s_ctx.app_mutex);
        return ESP_ERR_TIMEOUT;
    }

    /* 原子更新：读取 latest_buf_idx，标记 app_buf_idx */
    taskENTER_CRITICAL(&s_ctx.buf_mux);
    s_ctx.app_buf_idx = s_ctx.latest_buf_idx;
    int snap_idx = s_ctx.app_buf_idx;
    taskEXIT_CRITICAL(&s_ctx.buf_mux);

    *data = s_ctx.frame_bufs[snap_idx];
    if (size)   *size   = s_ctx.frame_size;
    if (width)  *width  = s_ctx.frame_width;
    if (height) *height = s_ctx.frame_height;
    return ESP_OK;
}

/**
 * 归还帧 — 释放当前持有的缓冲区，允许捕获任务将其用于下一帧。
 */
esp_err_t cam_sensor_return_frame(uint8_t *data)
{
    (void)data;
    taskENTER_CRITICAL(&s_ctx.buf_mux);
    s_ctx.app_buf_idx = -1;
    taskEXIT_CRITICAL(&s_ctx.buf_mux);
    if (s_ctx.app_mutex) {
        xSemaphoreGive(s_ctx.app_mutex);
    }
    return ESP_OK;
}

/**
 * 释放所有资源 — 按初始化逆序销毁。
 */
esp_err_t cam_sensor_deinit(void)
{
    if (!s_ctx.initialized) return ESP_OK;

    if (s_ctx.streaming) cam_sensor_stop();

    /* 逆序：先关下游 ISP，再关上游 CSI */
    esp_isp_disable(s_ctx.isp_handle);
    esp_isp_del_processor(s_ctx.isp_handle);
    esp_cam_ctlr_disable(s_ctx.csi_handle);
    esp_cam_ctlr_del(s_ctx.csi_handle);
    esp_cam_sensor_del_dev(s_ctx.sensor);
    esp_sccb_del_i2c_io(s_ctx.sccb_io);
    if (s_ctx.owns_i2c_bus) {
        i2c_del_master_bus(s_ctx.i2c_bus);
    }

    for (int i = 0; i < CONFIG_CAM_SENSOR_FB_COUNT; i++) {
        if (s_ctx.frame_bufs[i]) heap_caps_free(s_ctx.frame_bufs[i]);
    }

    if (s_ctx.app_mutex) vSemaphoreDelete(s_ctx.app_mutex);
    if (s_ctx.ldo_handle) esp_ldo_release_channel(s_ctx.ldo_handle);

    memset(&s_ctx, 0, sizeof(s_ctx));
    ESP_LOGI(TAG, "Deinitialized");
    return ESP_OK;
}
