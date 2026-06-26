#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include "bsp_encoder.h"
#include "driver/gpio.h"
#include "driver/pulse_cnt.h"
#include "esp_check.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"

#define BSP_ENCODER_NUM 3                       // 编码器数量，当前板子有 3 个编码器，ID 分别是 0、1、2
#define BSP_ENCODER_PHASE_PULSES_PER_REV 12     // 编码器单相每转 12 个脉冲
#define BSP_ENCODER_PCNT_COUNTS_PER_REV (BSP_ENCODER_PHASE_PULSES_PER_REV * 4)  // 使用 A/B 双相双边沿计数，原始 PCNT 计数约为单相脉冲数的 4 倍。
#define BSP_ENCODER_SAMPLE_PERIOD_MS 100        // 编码器速度采样周期，周期到达后读取 PCNT 计数并清零。
#define BSP_ENCODER_WHEEL_DIAMETER_MM 76        // 车轮直径，单位 mm；速度换算时用它计算轮周长。
#define BSP_ENCODER_GLITCH_FILTER_NS 1000       // PCNT 毛刺滤波时间，单位:us 过滤过短的抖动脉冲。
#define BSP_ENCODER_PI 3.14159265358979323846f

// 6 个编码器输入 GPIO
#define BSP_ENCODER0_GPIO_A 5
#define BSP_ENCODER0_GPIO_B 6

#define BSP_ENCODER1_GPIO_A -1
#define BSP_ENCODER1_GPIO_B -1

#define BSP_ENCODER2_GPIO_A -1
#define BSP_ENCODER2_GPIO_B -1

static const char *TAG = "bsp_encoder";

// 每路编码器的 A/B 相 GPIO 配置。
typedef struct {
    int gpio_a;
    int gpio_b;
} bsp_encoder_gpio_config_t;

// 每路编码器对应 1 个 PCNT unit 和 2 个 PCNT channel。
typedef struct {
    pcnt_unit_handle_t unit;
    pcnt_channel_handle_t channel_a;
    pcnt_channel_handle_t channel_b;
} bsp_encoder_pcnt_t;

// 三路编码器的 GPIO 配置表，数组下标和 bsp_encoder_id_t 一一对应。
static const bsp_encoder_gpio_config_t s_encoder_gpio_configs[BSP_ENCODER_NUM] = {
    { .gpio_a = BSP_ENCODER0_GPIO_A, .gpio_b = BSP_ENCODER0_GPIO_B },
    { .gpio_a = BSP_ENCODER1_GPIO_A, .gpio_b = BSP_ENCODER1_GPIO_B },
    { .gpio_a = BSP_ENCODER2_GPIO_A, .gpio_b = BSP_ENCODER2_GPIO_B },
};

// 保存 3 路编码器的 PCNT unit/channel 句柄，后续采样和清零都通过这些句柄访问硬件计数器。
static bsp_encoder_pcnt_t s_encoders[BSP_ENCODER_NUM];
// 缓存 3 路编码器最近一次 100ms 采样换算出的速度，单位 cm/s。
static float s_encoder_speed_cm_s[BSP_ENCODER_NUM];
// 保护 s_encoder_speed_cm_s，避免定时器回调写入时被其他任务同时读取。
static portMUX_TYPE s_speed_lock = portMUX_INITIALIZER_UNLOCKED;
// 100ms 周期采样定时器句柄，定时触发读取 PCNT、换算速度并清零计数。
static esp_timer_handle_t s_sample_timer;
// 记录编码器 BSP 是否已初始化，用于防止重复初始化和未初始化读取。
static bool s_initialized;

//前置声明一些静态函数，后面会实现它们
static esp_err_t validate_encoder_id(bsp_encoder_id_t encoder_id);
static esp_err_t init_encoder_pcnt(bsp_encoder_id_t encoder_id);
static void encoder_sample_timer_cb(void *arg);

// 初始化 3 路编码器 PCNT 资源，默认初始化后立即开始计数和周期测速。
esp_err_t bsp_encoder_init(void)
{
    if (s_initialized) {
        return ESP_OK;
    }

    // 依次初始化 3 路编码器；任意一路失败则返回错误。
    for (bsp_encoder_id_t i = BSP_ENCODER_0; i < BSP_ENCODER_NUM; ++i) {
        ESP_RETURN_ON_ERROR(init_encoder_pcnt(i), TAG, "init encoder %d PCNT failed", i);
    }

    const esp_timer_create_args_t sample_timer_args = {
        .callback = encoder_sample_timer_cb,
        .name = "encoder_sample",
    };
    ESP_RETURN_ON_ERROR(esp_timer_create(&sample_timer_args, &s_sample_timer), TAG, "create encoder sample timer failed");

    s_initialized = true;
    ESP_RETURN_ON_ERROR(esp_timer_start_periodic(s_sample_timer, BSP_ENCODER_SAMPLE_PERIOD_MS * 1000ULL),
                        TAG, "start encoder sample timer failed");
    ESP_LOGI(TAG, "encoder BSP initialized");
    return ESP_OK;
}

// 读取最近一次采样换算出的车轮速度，单位 cm/s。
esp_err_t bsp_encoder_get_speed_cm_s(bsp_encoder_id_t encoder_id, float *speed_cm_s)
{
    ESP_RETURN_ON_FALSE(s_initialized, ESP_ERR_INVALID_STATE, TAG, "encoder BSP is not initialized");
    ESP_RETURN_ON_FALSE(speed_cm_s, ESP_ERR_INVALID_ARG, TAG, "speed pointer is NULL");
    ESP_RETURN_ON_ERROR(validate_encoder_id(encoder_id), TAG, "encoder id validation failed");

    portENTER_CRITICAL(&s_speed_lock);
    *speed_cm_s = s_encoder_speed_cm_s[encoder_id];
    portEXIT_CRITICAL(&s_speed_lock);
    return ESP_OK;
}

// 验证编码器 ID 的合法性，必须在 0 到 BSP_ENCODER_NUM-1 之间。
static esp_err_t validate_encoder_id(bsp_encoder_id_t encoder_id)
{
    ESP_RETURN_ON_FALSE(encoder_id >= BSP_ENCODER_0 && encoder_id < BSP_ENCODER_NUM,
                        ESP_ERR_INVALID_ARG, TAG, "invalid encoder id: %d", encoder_id);
    return ESP_OK;
}

// 验证编码器 GPIO 配置的合法性：GPIO 必须有效且 A/B 相不能接在同一个 GPIO 上
static esp_err_t validate_encoder_gpio(bsp_encoder_id_t encoder_id, const bsp_encoder_gpio_config_t *config)
{
    // -1 表示暂未接线，ESP-IDF PCNT 会使用 virtual IO；非 -1 时必须是有效 GPIO。
    ESP_RETURN_ON_FALSE(config->gpio_a == -1 || GPIO_IS_VALID_GPIO(config->gpio_a),
                        ESP_ERR_INVALID_ARG, TAG, "encoder %d GPIO A is invalid", encoder_id);
    ESP_RETURN_ON_FALSE(config->gpio_b == -1 || GPIO_IS_VALID_GPIO(config->gpio_b),
                        ESP_ERR_INVALID_ARG, TAG, "encoder %d GPIO B is invalid", encoder_id);
    ESP_RETURN_ON_FALSE(config->gpio_a == -1 || config->gpio_b == -1 || config->gpio_a != config->gpio_b,
                        ESP_ERR_INVALID_ARG, TAG, "encoder %d GPIOs must be different", encoder_id);
    return ESP_OK;
}

// 将原始 PCNT 计数转换为车轮速度，单位 cm/s。
static float encoder_count_to_speed_cm_s(int count)
{
    const float wheel_circumference_cm = (float)BSP_ENCODER_WHEEL_DIAMETER_MM * BSP_ENCODER_PI / 10.0f;
    const float sample_period_s = (float)BSP_ENCODER_SAMPLE_PERIOD_MS / 1000.0f;
    return (float)count * wheel_circumference_cm / (float)BSP_ENCODER_PCNT_COUNTS_PER_REV / sample_period_s;
}

// 更新指定编码器的速度值，使用临界区保护共享变量。
static void update_encoder_speed(bsp_encoder_id_t encoder_id, float speed_cm_s)
{
    portENTER_CRITICAL(&s_speed_lock);
    s_encoder_speed_cm_s[encoder_id] = speed_cm_s;
    portEXIT_CRITICAL(&s_speed_lock);
}

// 配置单路 PCNT channel 的边沿/电平动作，实现正交编码器的计数方向判断。
static esp_err_t configure_quadrature_channel(pcnt_channel_handle_t channel,
                                              pcnt_channel_edge_action_t positive_edge_action,
                                              pcnt_channel_edge_action_t negative_edge_action)
{
    // edge action 决定本相上升沿/下降沿时计数加减。
    ESP_RETURN_ON_ERROR(pcnt_channel_set_edge_action(channel, positive_edge_action, negative_edge_action),
                        TAG, "set PCNT edge action failed");
    // level action 使用另一相电平决定是否反转方向，从而实现正交编码器方向判断。
    return pcnt_channel_set_level_action(channel,
                                         PCNT_CHANNEL_LEVEL_ACTION_KEEP,
                                         PCNT_CHANNEL_LEVEL_ACTION_INVERSE);
}

// 定时器回调函数，周期到达后读取 PCNT 计数、换算速度、清零计数，为下一个周期做准备。
static void encoder_sample_timer_cb(void *arg)
{
    (void)arg;

    for (bsp_encoder_id_t i = BSP_ENCODER_0; i < BSP_ENCODER_NUM; ++i) {
        int count = 0;
        esp_err_t ret = pcnt_unit_get_count(s_encoders[i].unit, &count);
        if (ret == ESP_OK) {
            update_encoder_speed(i, encoder_count_to_speed_cm_s(count));
            ESP_LOGD(TAG, "encoder %d count: %d, speed: %.2f cm/s", i, count, s_encoder_speed_cm_s[i]);
            ret = pcnt_unit_clear_count(s_encoders[i].unit);
        }
        if (ret != ESP_OK) {
            ESP_LOGW(TAG, "sample encoder %d failed: %s", i, esp_err_to_name(ret));
        }
    }
}

// 初始化单路编码器的 PCNT 资源，包括 unit、channel 配置和启动计数。
static esp_err_t init_encoder_pcnt(bsp_encoder_id_t encoder_id)
{
    ESP_RETURN_ON_ERROR(validate_encoder_id(encoder_id), TAG, "encoder id validation failed");

    const bsp_encoder_gpio_config_t *gpio_config = &s_encoder_gpio_configs[encoder_id];
    bsp_encoder_pcnt_t *encoder = &s_encoders[encoder_id];

    ESP_RETURN_ON_ERROR(validate_encoder_gpio(encoder_id, gpio_config), TAG, "encoder GPIO validation failed");

    // 创建 PCNT unit：每路编码器独占一个计数器，计数值由 100ms 采样定时器读取并清零。
    pcnt_unit_config_t unit_config = {
        .high_limit = INT16_MAX,
        .low_limit = INT16_MIN,
    };
    ESP_RETURN_ON_ERROR(pcnt_new_unit(&unit_config, &encoder->unit), TAG, "create PCNT unit failed");

    // 设置毛刺滤波，减少机械编码器抖动导致的误计数。
    pcnt_glitch_filter_config_t filter_config = {
        .max_glitch_ns = BSP_ENCODER_GLITCH_FILTER_NS,
    };
    ESP_RETURN_ON_ERROR(pcnt_unit_set_glitch_filter(encoder->unit, &filter_config),
                        TAG, "set PCNT glitch filter failed");

    // A 相 channel：A 相作为边沿输入，B 相作为方向判断电平输入。
    pcnt_chan_config_t channel_a_config = {
        .edge_gpio_num = gpio_config->gpio_a,
        .level_gpio_num = gpio_config->gpio_b,
        .flags.virt_edge_io_level = 0,
        .flags.virt_level_io_level = 0,
    };
    ESP_RETURN_ON_ERROR(pcnt_new_channel(encoder->unit, &channel_a_config, &encoder->channel_a),
                        TAG, "create PCNT channel A failed");

    // B 相 channel：B 相作为边沿输入，A 相作为方向判断电平输入。
    pcnt_chan_config_t channel_b_config = {
        .edge_gpio_num = gpio_config->gpio_b,
        .level_gpio_num = gpio_config->gpio_a,
        .flags.virt_edge_io_level = 0,
        .flags.virt_level_io_level = 0,
    };
    ESP_RETURN_ON_ERROR(pcnt_new_channel(encoder->unit, &channel_b_config, &encoder->channel_b),
                        TAG, "create PCNT channel B failed");

    // 两个 channel 的加减方向相反，组合后完成 AB 相双边沿正交解码。
    ESP_RETURN_ON_ERROR(configure_quadrature_channel(encoder->channel_a,
                                                     PCNT_CHANNEL_EDGE_ACTION_DECREASE,
                                                     PCNT_CHANNEL_EDGE_ACTION_INCREASE),
                        TAG, "configure PCNT channel A failed");
    ESP_RETURN_ON_ERROR(configure_quadrature_channel(encoder->channel_b,
                                                     PCNT_CHANNEL_EDGE_ACTION_INCREASE,
                                                     PCNT_CHANNEL_EDGE_ACTION_DECREASE),
                        TAG, "configure PCNT channel B failed");

    // 使能、清零、启动后，PCNT 硬件会自动跟随 AB 相变化累计原始计数。
    ESP_RETURN_ON_ERROR(pcnt_unit_enable(encoder->unit), TAG, "enable PCNT unit failed");
    ESP_RETURN_ON_ERROR(pcnt_unit_clear_count(encoder->unit), TAG, "clear PCNT unit failed");
    return pcnt_unit_start(encoder->unit);
}

