#include <stdbool.h>
#include <stdint.h>
#include "bsp_motor.h"
#include "sorter_debug_config.h"
#include "driver/gpio.h"
#include "driver/mcpwm_prelude.h"
#include "esp_check.h"
#include "esp_log.h"

// 对于单个电机 EN = 0 开启
// 对于单个电机 PWM1 = 3.3V ， PWM2 = 占空比 如果占空比20% 那么就是速度百分之20%
// PWM1和PWM2 哪个是3.3 另一个是占空比  这个点来决定正反转
// 两个3.3V 电机急刹

//给20比较值 80*3.3
#define BSP_MOTOR_MCPWM_GROUP_ID 0                           //使用的 MCPWM 组 ID (0或1)
#define BSP_MOTOR_PWM_FREQ_HZ 1000                           //PWM 频率
#define BSP_MOTOR_PWM_COMPARE_MAX 100                        //比较器的最大值
#define BSP_MOTOR_PWM_PERIOD_TICKS BSP_MOTOR_PWM_COMPARE_MAX //PWM 周期对应的计数器 ticks
#define BSP_MOTOR_PWM_TIMER_RESOLUTION_HZ (BSP_MOTOR_PWM_FREQ_HZ * BSP_MOTOR_PWM_PERIOD_TICKS) //定时器分辨率

static const char *TAG = "bsp_motor";

typedef struct {
    int pwm_a_gpio;
    int pwm_b_gpio;
} bsp_motor_gpio_config_t;

typedef struct {
    mcpwm_timer_handle_t timer;
    mcpwm_oper_handle_t oper;
    mcpwm_cmpr_handle_t cmp_a;
    mcpwm_cmpr_handle_t cmp_b;
    mcpwm_gen_handle_t gen_a;
    mcpwm_gen_handle_t gen_b;
} bsp_motor_mcpwm_t;

// 三个电机的双路 PWM 引脚配置；当前用 -1 占位，后续接线确定后改这里的宏。
static const bsp_motor_gpio_config_t s_motor_gpio_configs[3] = {
    { .pwm_a_gpio = SORTER_MOTOR0_PWM_A_GPIO, .pwm_b_gpio = SORTER_MOTOR0_PWM_B_GPIO },
    { .pwm_a_gpio = SORTER_MOTOR1_PWM_A_GPIO, .pwm_b_gpio = SORTER_MOTOR1_PWM_B_GPIO },
    { .pwm_a_gpio = SORTER_MOTOR2_PWM_A_GPIO, .pwm_b_gpio = SORTER_MOTOR2_PWM_B_GPIO },
};

static const bool s_motor_reverse[3] = {
    SORTER_MOTOR0_REVERSE != 0,
    SORTER_MOTOR1_REVERSE != 0,
    SORTER_MOTOR2_REVERSE != 0,
};

static bsp_motor_mcpwm_t s_motors[3];
static bool s_initialized;

//id编号0-2分别对应三个电机，函数内会验证id合法性
static esp_err_t validate_motor_id(uint8_t motor_id)
{
    ESP_RETURN_ON_FALSE(motor_id < 3, ESP_ERR_INVALID_ARG, TAG, "invalid motor id: %u", motor_id);
    return ESP_OK;
}

//pwm比较值的合法范围是0到BSP_MOTOR_PWM_COMPARE_MAX，函数内会验证比较值是否合法
static esp_err_t validate_compare_value(uint32_t compare)
{
    ESP_RETURN_ON_FALSE(compare <= BSP_MOTOR_PWM_COMPARE_MAX,
                        ESP_ERR_INVALID_ARG, TAG, "PWM compare value is out of range");
    return ESP_OK;
}

//gpio合法性验证，主要验证是否配置了引脚、引脚是否合法以及两路PWM是否配置了同一个引脚（MCPWM generator不支持）。函数内会验证GPIO配置是否合法。
static esp_err_t validate_motor_gpio(uint8_t motor_id, const bsp_motor_gpio_config_t *config)
{
    // MCPWM generator 不能直接使用 -1 GPIO，这里先拦截未配置或非法引脚。
    ESP_RETURN_ON_FALSE(config->pwm_a_gpio != -1,
                        ESP_ERR_INVALID_ARG, TAG, "motor %u PWM A GPIO is not configured", motor_id);
    ESP_RETURN_ON_FALSE(config->pwm_b_gpio != -1,
                        ESP_ERR_INVALID_ARG, TAG, "motor %u PWM B GPIO is not configured", motor_id);
    ESP_RETURN_ON_FALSE(GPIO_IS_VALID_OUTPUT_GPIO(config->pwm_a_gpio),
                        ESP_ERR_INVALID_ARG, TAG, "motor %u PWM A GPIO is invalid", motor_id);
    ESP_RETURN_ON_FALSE(GPIO_IS_VALID_OUTPUT_GPIO(config->pwm_b_gpio),
                        ESP_ERR_INVALID_ARG, TAG, "motor %u PWM B GPIO is invalid", motor_id);
    ESP_RETURN_ON_FALSE(config->pwm_a_gpio != config->pwm_b_gpio,
                        ESP_ERR_INVALID_ARG, TAG, "motor %u PWM GPIOs must be different", motor_id);
    return ESP_OK;
}

// Generator 的配置：计数器归零时拉低 PWM，计数高于比较值后拉高 PWM，实现正向占空比控制。
static esp_err_t configure_generator(mcpwm_gen_handle_t gen, mcpwm_cmpr_handle_t cmp)
{
    // 计数器归零时拉低 PWM，计数高于比较值后拉高 PWM。
    ESP_RETURN_ON_ERROR(mcpwm_generator_set_action_on_timer_event(gen,
                        MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP,
                                                     MCPWM_TIMER_EVENT_EMPTY,
                                                     MCPWM_GEN_ACTION_LOW)),
                        TAG, "set generator timer action failed");
    return mcpwm_generator_set_action_on_compare_event(gen,
                        MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP,
                                                       cmp,
                                                       MCPWM_GEN_ACTION_HIGH));
}

// 初始化指定电机的 MCPWM 资源：一个 timer、一个 operator、两个 comparator 和两个 generator，并把它们正确连接起来。
static esp_err_t init_motor_mcpwm(uint8_t motor_id)
{
    // 验证电机 ID 和 GPIO 配置合法性。
    ESP_RETURN_ON_ERROR(validate_motor_id(motor_id), TAG, "motor id validation failed");

    const bsp_motor_gpio_config_t *gpio_config = &s_motor_gpio_configs[motor_id];
    bsp_motor_mcpwm_t *motor = &s_motors[motor_id];

    ESP_RETURN_ON_ERROR(validate_motor_gpio(motor_id, gpio_config), TAG, "motor GPIO validation failed");

    // Timer 是 PWM 时基：分辨率由频率和 0-1000 精度自动计算。
    mcpwm_timer_config_t timer_config = {
        .group_id = BSP_MOTOR_MCPWM_GROUP_ID,
        .clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT,
        .resolution_hz = BSP_MOTOR_PWM_TIMER_RESOLUTION_HZ,
        .period_ticks = BSP_MOTOR_PWM_PERIOD_TICKS,
        .count_mode = MCPWM_TIMER_COUNT_MODE_UP,
    };
    ESP_RETURN_ON_ERROR(mcpwm_new_timer(&timer_config, &motor->timer), TAG, "create MCPWM timer failed");

    // Operator 负责把同一个 timer 的时基转换成该电机的两路 PWM 输出。
    mcpwm_operator_config_t operator_config = {
        .group_id = BSP_MOTOR_MCPWM_GROUP_ID,
    };
    ESP_RETURN_ON_ERROR(mcpwm_new_operator(&operator_config, &motor->oper), TAG, "create MCPWM operator failed");
    ESP_RETURN_ON_ERROR(mcpwm_operator_connect_timer(motor->oper, motor->timer), TAG, "connect MCPWM timer failed");

    // 两个 comparator 分别对应电机驱动的 A/B 两路 PWM 占空比。
    mcpwm_comparator_config_t comparator_config = {
        .flags.update_cmp_on_tez = true,
    };
    ESP_RETURN_ON_ERROR(mcpwm_new_comparator(motor->oper, &comparator_config, &motor->cmp_a), TAG, "create MCPWM comparator A failed");
    ESP_RETURN_ON_ERROR(mcpwm_new_comparator(motor->oper, &comparator_config, &motor->cmp_b), TAG, "create MCPWM comparator B failed");
    // 初始比较值设为 0，计数值高于 0 后输出高电平，实现初始化 3.3V 输出。
    ESP_RETURN_ON_ERROR(mcpwm_comparator_set_compare_value(motor->cmp_a, 0), TAG, "set comparator A initial value failed");
    ESP_RETURN_ON_ERROR(mcpwm_comparator_set_compare_value(motor->cmp_b, 0), TAG, "set comparator B initial value failed");

    // Generator 把 operator 内部 PWM 信号真正路由到 GPIO 引脚。
    mcpwm_generator_config_t generator_config = {
        .gen_gpio_num = gpio_config->pwm_a_gpio,
    };
    ESP_RETURN_ON_ERROR(mcpwm_new_generator(motor->oper, &generator_config, &motor->gen_a), TAG, "create MCPWM generator A failed");
    generator_config.gen_gpio_num = gpio_config->pwm_b_gpio;
    ESP_RETURN_ON_ERROR(mcpwm_new_generator(motor->oper, &generator_config, &motor->gen_b), TAG, "create MCPWM generator B failed");

    ESP_RETURN_ON_ERROR(configure_generator(motor->gen_a, motor->cmp_a), TAG, "configure generator A failed");
    ESP_RETURN_ON_ERROR(configure_generator(motor->gen_b, motor->cmp_b), TAG, "configure generator B failed");
    // 启动 timer 后 MCPWM 硬件按比较值产生波形，不使用 force level 覆盖输出。
    ESP_RETURN_ON_ERROR(mcpwm_timer_enable(motor->timer), TAG, "enable MCPWM timer failed");
    return mcpwm_timer_start_stop(motor->timer, MCPWM_TIMER_START_NO_STOP);
}

esp_err_t bsp_motor_init(void)
{
    // 对外的 BSP 初始化入口：依次初始化三个电机的 MCPWM 资源。
    if (s_initialized) {
        return ESP_OK;
    }

    for (uint8_t i = 0; i < 3; ++i) {
        ESP_RETURN_ON_ERROR(init_motor_mcpwm(i), TAG, "init motor %u MCPWM failed", i);
    }

    s_initialized = true;
    ESP_LOGI(TAG, "motor BSP initialized");
    return ESP_OK;
}

// 设置指定电机的两路 PWM 比较值，范围是 0 到 BSP_MOTOR_PWM_COMPARE_MAX。函数内会验证电机 ID 和比较值是否合法。
esp_err_t bsp_motor_set_compare(uint8_t motor_id, uint32_t pwm_a_compare, uint32_t pwm_b_compare)
{
    ESP_RETURN_ON_FALSE(s_initialized, ESP_ERR_INVALID_STATE, TAG, "motor BSP is not initialized");
    ESP_RETURN_ON_ERROR(validate_motor_id(motor_id), TAG, "motor id validation failed");
    ESP_RETURN_ON_ERROR(validate_compare_value(pwm_a_compare), TAG, "PWM A compare validation failed");
    ESP_RETURN_ON_ERROR(validate_compare_value(pwm_b_compare), TAG, "PWM B compare validation failed");

    bsp_motor_mcpwm_t *motor = &s_motors[motor_id];
    ESP_RETURN_ON_ERROR(mcpwm_comparator_set_compare_value(motor->cmp_a, pwm_a_compare), TAG, "set PWM A compare failed");
    return mcpwm_comparator_set_compare_value(motor->cmp_b, pwm_b_compare);
}

// 设置指定电机的速度和方向，direction为0正转、1反转。
esp_err_t bsp_motor_set_speed_direction(uint8_t motor_id, uint32_t speed, uint8_t direction)
{
    ESP_RETURN_ON_FALSE(direction <= 1, ESP_ERR_INVALID_ARG, TAG, "invalid motor direction: %u", direction);
    ESP_RETURN_ON_ERROR(validate_compare_value(speed), TAG, "motor speed validation failed");
    ESP_RETURN_ON_ERROR(validate_motor_id(motor_id), TAG, "motor id validation failed");

    if (s_motor_reverse[motor_id]) {
        direction = direction ? 0 : 1;
    }

    if (direction == 0) {
        return bsp_motor_set_compare(motor_id, 0, speed);
    }
    return bsp_motor_set_compare(motor_id, speed, 0);
}
