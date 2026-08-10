#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "SEGGER_RTT.h"
#include "system_init.h"
#include "system_monitor.h"

void app_main(void)
{
    System_Init();
    system_monitor_init();

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}
