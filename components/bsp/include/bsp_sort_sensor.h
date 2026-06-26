#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    BSP_SORT_SENSOR_S1 = 1,
    BSP_SORT_SENSOR_S2 = 2,
    BSP_SORT_SENSOR_S3 = 3,
    BSP_SORT_SENSOR_S4 = 4,
} bsp_sort_sensor_id_t;

esp_err_t bsp_sort_sensor_init(void);
esp_err_t bsp_sort_sensor_get_state(bsp_sort_sensor_id_t sensor_id, bool *active);

#ifdef __cplusplus
}
#endif
