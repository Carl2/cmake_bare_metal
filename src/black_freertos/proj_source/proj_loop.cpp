#include <cstddef>
#include <string_view>
#include "FreeRTOS.h"
#include "main.h"
#include "projdefs.h"
#include "stm32f4xx_hal_uart.h"
#include "task.h"
#include "timers.h"

extern UART_HandleTypeDef huart1;
void timer_callback(xTimerHandle)
{
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
}

void vTask1([[maybe_unused]] void* pvParam)
{
    constexpr static std::string_view msg = "Hello from task1\n\r";
    while (true)
    {
        uint8_t data[] = "HELLO WORLD \r\n";
        // do somehting useful
        HAL_UART_Transmit(&huart1, data, sizeof(data), 100);
        // HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

TimerHandle_t timer;

extern "C" void main_loop()
{
    // constexpr static std::string_view msg = "Hello from task1\n\r";
    // HAL_UART_Transmit(&huart1, (unsigned char*)msg.data(), msg.size(), 100);

    timer = xTimerCreate("TestTimer", pdMS_TO_TICKS(1200), pdTRUE, NULL, timer_callback);
    xTimerStart(timer, 0);

    xTaskCreate(vTask1, "Tasker", 1000, nullptr, 1, nullptr);
    vTaskStartScheduler();
}
