/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <array>
#include <string_view>
#include "address/address_setup_com.hpp"
#include "msg_def.hpp"
#include "msg_sml.hpp"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;
UART_HandleTypeDef huart1;

// sml::sm<Blinker_sm> bl_sm{};
/* USER CODE BEGIN PV */

namespace
{

msg::Uart_buffer_t uart_data;
// uart::AddressSetup address_sm{uart::AddressContext{}};

struct Receive_data
{
    msg::Uart_buffer_t uart_msg_data;
    std::size_t sz;
};

Receive_data recv_data;

volatile bool msg_recv_flag{};

auto uart_sync_send = [](std::string_view view) {
    msg::Uart_buffer_t buffer;
    memcpy(buffer.data(), view.data(), view.size());
    HAL_UART_Transmit(&huart1, buffer.data(), static_cast<uint16_t>(view.size()), 10);
};

// Setting up Address statemachine
// TODO: This should be moved to some other nice place.
// uart::AddressContext addr_ctx{uart_sync_send};
// auto address_sm         = uart::AddressSetup<decltype(addr_ctx)>{std::move(addr_ctx)};
auto address_sm = uart::AddressSetup{uart::AddressContext{uart_sync_send}};
// auto address_setup_item = msg::make_cmd_item<msg::GuppiCmd::CMD_ENABLE_ADDRESS_SETUP>(
//     uart::get_address_setup_callback(address_sm));
// clang-format off
auto cmds = msg::make_Guppi_protocol(
    // Used for Set address
    msg::make_cmd_item<msg::GuppiCmd::CMD_ENABLE_ADDRESS_SETUP>(
        uart::get_enable_address_setup_callback(address_sm)),
    msg::make_cmd_item<msg::GuppiCmd::CMD_DISABLE_ADDRESS_SETUP>(
        uart::get_disable_address_setup_callback(address_sm)),
    msg::make_cmd_item<msg::GuppiCmd::CMD_SET_PRINTHEAD_ADDRESS>(
        uart::get_address_setup_callback(address_sm)),
    // Get uuid
    msg::make_cmd_item<msg::GuppiCmd::CMD_GET_UUID>(
        uart::get_UUID_callback(address_sm)),
    // Fake Enable pin
    msg::make_cmd_item<msg::GuppiCmd::DEBUG_ENABLE_PIN >(
        uart::get_debug_pin_toggle(address_sm))

                                     );
// clang-format on

// When a recieve message data is called, the message has this as destination.
// TODO: see if matching hdr.len == view.size()...
auto receive_message_data = [](const msg::Header& hdr, std::span<const uint8_t> view) {
    // msg::Uart_buffer_t buffer;
    msg::RetType ret_buff{};
    msg::OptArgs optArg((hdr.len > 0 ? std::optional(view) : std::nullopt));
    auto sz = exec_cmd(cmds, msg::command_transform(hdr.cmd), std::move(optArg), ret_buff.begin(),
                       ret_buff.end());

    HAL_UART_Transmit(&huart1, ret_buff.data(), static_cast<uint16_t>(sz), 10);

    // The command parser handler will redirect the message to
    // its rightful owner.
    // auto val = cmd_parser(hdr.cmd, view);
    return view;
};
auto uart_irq_fn = [](uint16_t sz) {
    // TODO: Need to check that size is reasonable.
    HAL_UART_Receive_IT(&huart1, uart_data.data(), sz);
};

}  // namespace

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
    // clang-format off
    msg::MainMachine m_sm{
        msg::SystemContext{uart_irq_fn, uart_sync_send, receive_message_data, get_check_address_fn(address_sm)}
    };
    // clang-format on
    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_TIM2_Init();
    MX_USART1_UART_Init();
    /* USER CODE BEGIN 2 */
    MX_USART1_UART_Init();
    HAL_TIM_Base_Start_IT(&htim2);

    HAL_GPIO_EXTI_Callback(GPIO_PIN_2);

    uart_irq_fn(msg::HDR_SZ);
    // HAL_UART_Receive_IT(&huart1, uart_data.data(), 6);
    //  start_simple_state_machine(huart1);
    //  char msg[]="Im alive\n\r";
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        // HAL_Delay(500);
        // HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), 10);
        if (msg_recv_flag)
        {
            m_sm.new_message(recv_data.uart_msg_data, recv_data.sz);
            msg_recv_flag = false;
            // HAL_UART_Receive_IT(&huart1, uart_data.data(), 1);
        }
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct{};
    RCC_ClkInitTypeDef RCC_ClkInitStruct{};

    /** Configure the main internal regulator output voltage
     */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM       = 25;
    RCC_OscInitStruct.PLL.PLLN       = 168;
    RCC_OscInitStruct.PLL.PLLP       = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ       = 4;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType =
        RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
 * @brief TIM2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM2_Init(void)
{

    /* USER CODE BEGIN TIM2_Init 0 */

    /* USER CODE END TIM2_Init 0 */

    TIM_ClockConfigTypeDef sClockSourceConfig{};
    TIM_MasterConfigTypeDef sMasterConfig{};

    /* USER CODE BEGIN TIM2_Init 1 */

    /* USER CODE END TIM2_Init 1 */
    htim2.Instance               = TIM2;
    htim2.Init.Prescaler         = 84 - 1;
    htim2.Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim2.Init.Period            = 100000 - 1;
    htim2.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
    {
        Error_Handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN TIM2_Init 2 */

    /* USER CODE END TIM2_Init 2 */
}

/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART1_UART_Init(void)
{

    /* USER CODE BEGIN USART1_Init 0 */

    /* USER CODE END USART1_Init 0 */

    /* USER CODE BEGIN USART1_Init 1 */

    /* USER CODE END USART1_Init 1 */
    huart1.Instance          = USART1;
    huart1.Init.BaudRate     = 19200;
    huart1.Init.WordLength   = UART_WORDLENGTH_8B;
    huart1.Init.StopBits     = UART_STOPBITS_1;
    huart1.Init.Parity       = UART_PARITY_NONE;
    huart1.Init.Mode         = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN USART1_Init 2 */

    /* USER CODE END USART1_Init 2 */
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct{};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);

    /*Configure GPIO pin : PC13 */
    GPIO_InitStruct.Pin   = GPIO_PIN_13;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
    if (htim == &htim2)
    {
        // bl_sm.process_event(evBlinkTimer{});
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
{
    std::memcpy(recv_data.uart_msg_data.data(), uart_data.data(), huart->RxXferSize);
    recv_data.sz = huart->RxXferSize;

    msg_recv_flag = true;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_2)
    {
        __NOP();
    }
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {}
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
    static_cast<void>(file);
    static_cast<void>(line);

    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
