#include <array>
#include <cstddef>
#include <string_view>
#include "FreeRTOS.h"
#include "main.h"
#include "portmacro.h"
#include "projdefs.h"
#include "stm32f4xx_hal_uart.h"
#include "task.h"
#include "timers.h"

namespace
{

TaskHandle_t taskHandle1 = nullptr;
TaskHandle_t taskHandle2 = nullptr;
}  // namespace

extern UART_HandleTypeDef huart1;
void timer_callback(xTimerHandle)
{
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
}

void vTask1([[maybe_unused]] void* pvParam)
{
    // auto* otherTaskHandler = static_cast<TaskHandle_t*>(pvParam);
    while (true)
    {
        uint8_t data[] = "HELLO ";
        // do somehting useful
        HAL_UART_Transmit(&huart1, data, sizeof(data), 100);
        // HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        xTaskNotifyGive(taskHandle2);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void vTask2([[maybe_unused]] void* pvParam)
{

    while (true)
    {

        if (ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(100)) == pdTRUE)
        {

            // do somehting useful
            uint8_t data[] = "WORLD \r\n";
            HAL_UART_Transmit(&huart1, data, sizeof(data), 100);
        }
        else
        {
            uint8_t data[] = "TimedOut!! \r\n";
            HAL_UART_Transmit(&huart1, data, sizeof(data), 100);
        }
        // HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    }
}

TimerHandle_t timer;

extern "C" void main_loop()
{
    // constexpr static std::string_view msg = "Hello from task1\n\r";
    // HAL_UART_Transmit(&huart1, (unsigned char*)msg.data(), msg.size(), 100);

    timer = xTimerCreate("TestTimer", pdMS_TO_TICKS(1200), pdTRUE, NULL, timer_callback);
    xTimerStart(timer, 0);

    xTaskCreate(vTask2, "Tasker2", 1000, nullptr, 1, &taskHandle2);
    xTaskCreate(vTask1, "Tasker1", 1000, nullptr, 1, &taskHandle1);

    vTaskStartScheduler();
}
