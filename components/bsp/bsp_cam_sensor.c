/*
 * MIPI CSI 摄像头驱动 —— V4L2 最简实现（风格对齐 bsp_lcd.c）。
 *
 * cam_sensor_init() 一步完成：
 *   esp_video_init(csi) → open(/dev/video) → S_FMT(RGB888)
 *     → REQBUFS + mmap 多缓冲 → QBUF → STREAMON
 * SCCB 复用触摸已建好的 I2C 总线（GPIO8/7 共享）。
 * 硬件参数全部 #define 在此，system_init 只调一行 cam_sensor_init()。
 */
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include "esp_log.h"
#include "esp_video_init.h"
#include "esp_video_device.h"
#include "linux/videodev2.h"
#include "bsp_touch.h"
#include "bsp_cam_sensor.h"

// V4L2 帧缓冲数量。复刻 ESP-who 零拷贝 peek：vision 层 ringbuf 深度 = CAM_FB_COUNT-2
// （留 2 个给驱动：1 个正在 DMA 写、1 个排队待命），推理帧在被回收前的安全距离即此深度。
// 改大可线性增大防竞态时间窗口，代价为每缓冲多占 w×h×2 字节 PSRAM。详见
// docs/零拷贝peek竞态风险.md。此宏是缓冲数的唯一来源，vision 经 getter 读取。
#define CAM_FB_COUNT      5
#define CAM_FPS_DIV_MODE  0       // 0=30fps，1=15fps（CSI 驱动通过 skip_frames 只支持整除降帧）
#define CAM_SCCB_FREQ     100000  // SCCB（摄像头控制总线）时钟频率 100 kHz

#if CAM_FPS_DIV_MODE == 0
#define CAM_TARGET_FPS    30
#elif CAM_FPS_DIV_MODE == 1
#define CAM_TARGET_FPS    15
#else
#error "CAM_FPS_DIV_MODE must be 0 (30fps) or 1 (15fps)"
#endif
#define CAM_RESET_PIN   -1       // 复位引脚，-1 表示硬件未接（不由驱动控制）
#define CAM_PWDN_PIN    -1       // 掉电引脚，-1 表示硬件未接（不由驱动控制）

static const char *TAG = "bsp_cam";

static int s_fd = -1;                      // /dev/video 文件描述符，-1 表示未打开
static int s_width;                        // 摄像头输出宽度（由 G_FMT 读取）
static int s_height;                       // 摄像头输出高度（由 G_FMT 读取）
static void *s_fb[CAM_FB_COUNT];           // mmap 缓冲首地址
static size_t s_fb_len[CAM_FB_COUNT];      // mmap 缓冲字节数

// 摄像头初始化：esp_video_init(csi) → open → S_FMT(RGB888)
//   → REQBUFS + mmap 多缓冲 → QBUF → STREAMON。
// 调用前提：BSP_Touch_Init 已建好 I2C 总线（SCCB 复用之）。
esp_err_t cam_sensor_init(void)
{
    i2c_master_bus_handle_t i2c_bus = BSP_Touch_GetI2CBus();
    if (!i2c_bus) {
        ESP_LOGE(TAG, "I2C bus not ready (call BSP_Touch_Init first)");
        return ESP_ERR_INVALID_STATE;
    }

    // SCCB 复用触摸的 I2C 总线（init_sccb=false）
    esp_video_init_csi_config_t csi_config = {
        .sccb_config = {
            .init_sccb = false,
            .i2c_handle = i2c_bus,
            .freq = CAM_SCCB_FREQ,
        },
        .reset_pin = CAM_RESET_PIN,
        .pwdn_pin = CAM_PWDN_PIN,
    };
    esp_video_init_config_t cam_config = { .csi = &csi_config };
    esp_err_t ret = esp_video_init(&cam_config);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "esp_video_init failed: %s", esp_err_to_name(ret));
        return ret;
    }

    // 打开 MIPI CSI 视频设备节点（只读，仅取帧不写）
    s_fd = open(ESP_VIDEO_MIPI_CSI_DEVICE_NAME, O_RDONLY);
    if (s_fd < 0) {
        ESP_LOGE(TAG, "open %s failed", ESP_VIDEO_MIPI_CSI_DEVICE_NAME);
        return ESP_FAIL;
    }

    // 先 G_FMT 读出驱动默认分辨率，再 S_FMT 把像素格式改为 RGB888
    struct v4l2_format format = { .type = V4L2_BUF_TYPE_VIDEO_CAPTURE };
    if (ioctl(s_fd, VIDIOC_G_FMT, &format) != 0) {
        ESP_LOGE(TAG, "VIDIOC_G_FMT failed");
        return ESP_FAIL;
    }
    s_width = format.fmt.pix.width;
    s_height = format.fmt.pix.height;
    // 用 RGB888（V4L2_PIX_FMT_RGB24）而非 RGB565：让 ISP 全程按 8bit/通道输出，
    // 避免输出前砍成 16bit 再由 PPA 补零扩回 24bit（白只到 248、色阶变粗、画面发灰）。
    format.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB24;
    if (ioctl(s_fd, VIDIOC_S_FMT, &format) != 0) {
        ESP_LOGE(TAG, "VIDIOC_S_FMT RGB888 failed");
        return ESP_FAIL;
    }

    struct v4l2_streamparm parm = { .type = V4L2_BUF_TYPE_VIDEO_CAPTURE };
    parm.parm.capture.capability = V4L2_CAP_TIMEPERFRAME;
    parm.parm.capture.timeperframe.numerator = 1;
    parm.parm.capture.timeperframe.denominator = CAM_TARGET_FPS;
    if (ioctl(s_fd, VIDIOC_S_PARM, &parm) != 0) {
        ESP_LOGW(TAG, "VIDIOC_S_PARM %dfps unsupported, keep sensor default", CAM_TARGET_FPS);
    } else if (ioctl(s_fd, VIDIOC_G_PARM, &parm) != 0) {
        ESP_LOGW(TAG, "VIDIOC_G_PARM failed after setting fps");
    }

    // 申请缓冲（REQBUFS），再逐个 QUERYBUF 取偏移、mmap 映射到用户空间、
    // QBUF 入队等待填充。三步缺一不可。
    struct v4l2_requestbuffers req = {
        .count = CAM_FB_COUNT,
        .type = V4L2_BUF_TYPE_VIDEO_CAPTURE,
        .memory = V4L2_MEMORY_MMAP,
    };
    if (ioctl(s_fd, VIDIOC_REQBUFS, &req) != 0) {
        ESP_LOGE(TAG, "VIDIOC_REQBUFS failed");
        return ESP_FAIL;
    }
    for (int i = 0; i < CAM_FB_COUNT; i++) {
        struct v4l2_buffer buf = {
            .type = V4L2_BUF_TYPE_VIDEO_CAPTURE,
            .memory = V4L2_MEMORY_MMAP,
            .index = i,
        };
        if (ioctl(s_fd, VIDIOC_QUERYBUF, &buf) != 0) {
            ESP_LOGE(TAG, "VIDIOC_QUERYBUF %d failed", i);
            return ESP_FAIL;
        }
        s_fb_len[i] = buf.length;
        s_fb[i] = mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, s_fd, buf.m.offset);
        if (s_fb[i] == MAP_FAILED) {
            ESP_LOGE(TAG, "mmap %d failed", i);
            return ESP_FAIL;
        }
        if (ioctl(s_fd, VIDIOC_QBUF, &buf) != 0) {
            ESP_LOGE(TAG, "VIDIOC_QBUF %d failed", i);
            return ESP_FAIL;
        }
    }

    // 启动推流
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(s_fd, VIDIOC_STREAMON, &type) != 0) {
        ESP_LOGE(TAG, "VIDIOC_STREAMON failed");
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "camera init: %dx%d RGB888, %d buffers, %dfps, streaming",
             s_width, s_height, CAM_FB_COUNT,
             parm.parm.capture.timeperframe.denominator / parm.parm.capture.timeperframe.numerator);
    return ESP_OK;
}

// 取一帧：VIDIOC_DQBUF 从队列出队一个已填充的缓冲。
// 返回的 data 指向 mmap 区域（零拷贝），用完必须调 cam_sensor_return_frame 归还。
// timeout_ms 当前未使用——V4L2 DQBUF 会阻塞至帧就绪。
esp_err_t cam_sensor_get_frame(uint8_t **data, size_t *size,
                               int *width, int *height,
                               uint32_t timeout_ms)
{
    (void)timeout_ms;  // V4L2 DQBUF 阻塞至帧就绪
    struct v4l2_buffer buf = {
        .type = V4L2_BUF_TYPE_VIDEO_CAPTURE,
        .memory = V4L2_MEMORY_MMAP,
    };
    if (ioctl(s_fd, VIDIOC_DQBUF, &buf) != 0) {
        return ESP_FAIL;
    }
    if (data)   *data = (uint8_t *)s_fb[buf.index];
    if (size)   *size = buf.bytesused;
    if (width)  *width = s_width;
    if (height) *height = s_height;
    return ESP_OK;
}

// 归还帧：按缓冲地址反查 index，VIDIOC_QBUF 重新入队供后续复用。
// data 必须是 cam_sensor_get_frame 返回过的指针，否则返回 ESP_ERR_INVALID_ARG。
esp_err_t cam_sensor_return_frame(uint8_t *data)
{
    for (int i = 0; i < CAM_FB_COUNT; i++) {
        if (s_fb[i] == (void *)data) {
            struct v4l2_buffer buf = {
                .type = V4L2_BUF_TYPE_VIDEO_CAPTURE,
                .memory = V4L2_MEMORY_MMAP,
                .index = i,
            };
            return ioctl(s_fd, VIDIOC_QBUF, &buf) == 0 ? ESP_OK : ESP_FAIL;
        }
    }
    return ESP_ERR_INVALID_ARG;
}

// 返回 V4L2 帧缓冲总数（CAM_FB_COUNT）。vision 层据此推导 ringbuf 深度 = 返回值-2，
// 把缓冲数的唯一定义留在本文件，避免两处宏不一致。
int cam_sensor_get_fb_count(void)
{
    return CAM_FB_COUNT;
}

// 反初始化：STREAMOFF 停流 → munmap 释放所有缓冲 → close → esp_video_deinit。
// 即使部分步骤失败也尽量继续清理，保证资源不泄漏。
esp_err_t cam_sensor_deinit(void)
{
    if (s_fd >= 0) {
        enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        ioctl(s_fd, VIDIOC_STREAMOFF, &type);
    }
    for (int i = 0; i < CAM_FB_COUNT; i++) {
        if (s_fb[i] && s_fb[i] != MAP_FAILED) {
            munmap(s_fb[i], s_fb_len[i]);
            s_fb[i] = NULL;
        }
    }
    if (s_fd >= 0) {
        close(s_fd);
        s_fd = -1;
    }
    esp_video_deinit();
    return ESP_OK;
}
