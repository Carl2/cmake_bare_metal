#include "FreeRTOS.h"
#include "main.h"
#include "timers.h"

void timer_callback(xTimerHandle)
{
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
}

TimerHandle_t timer;

extern "C" void main_loop()
{
    timer = xTimerCreate("TestTimer", pdMS_TO_TICKS(200), pdTRUE, NULL, timer_callback);
    xTimerStart(timer, 0);
    vTaskStartScheduler();
    while (1)
    {}
}
