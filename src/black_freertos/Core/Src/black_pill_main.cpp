#include "FreeRTOS.h"
#include "main.h"
#include "timers.h"

extern void SystemClock_Config(void);
extern void MX_GPIO_Init(void);
extern void StartDefaultTask(void* argument);

void timer_callback(xTimerHandle)
{
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
}
/* USER CODE END 0 */
TimerHandle_t timer;

int main(void)
{
    timer = xTimerCreate("TestTimer", pdMS_TO_TICKS(500), pdTRUE, NULL, timer_callback);

    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    xTimerStart(timer, 0);
    vTaskStartScheduler();

    while (1)
    {}
}
