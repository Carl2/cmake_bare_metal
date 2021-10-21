#pragma once
//#include <type_traits>
// #ifdef __cplusplus
// extern "C"
// {
// #endif

#include <string_view>
#include "boost/sml.hpp"
#include "main.h"
#include "stm32f4xx_hal.h"

namespace sml = boost::sml;
extern UART_HandleTypeDef huart1;

struct evStartBlink
{};
struct evStopBlink
{};
struct evBlinkTimer
{};

namespace
{

constexpr std::string_view msg_start = "start called\n\r";
constexpr std::string_view msg_stop  = "stop called\n\r";

auto write_uart = [](auto ev) {
    // static_cast<void>(ev);

    if (std::is_same_v<decltype(ev), evStartBlink>)
    {
        HAL_UART_Transmit(&huart1, (uint8_t*)msg_start.data(), msg_start.size(), 10);
    }
    else if (std::is_same_v<decltype(ev), evStopBlink>)
    {
        HAL_UART_Transmit(&huart1, (uint8_t*)msg_stop.data(), msg_stop.size(), 10);
    }
};

auto toggle_pin = [](auto ev) {
    static_cast<void>(ev);
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
};

auto make_uart_msg_f = [](auto& x) {
    return [&x](auto ev) {
        static_cast<void>(ev);
        HAL_UART_Transmit(&huart1, (uint8_t*)msg_stop.data(), msg_stop.size(), 10);
        x++;
    };
};

}  // namespace

struct Blinker_sm
{
    auto operator()()
    {

        using namespace sml;
        // clang-format off
        return make_transition_table(
            *"init"_s                                             = "not_blinking"_s,
            "not_blinking"_s + event<evStartBlink> / make_uart_msg_f(x_)    = "blinking"_s,
            "blinking"_s + event<evBlinkTimer>     / toggle_pin            = "blinking"_s,
            "blinking"_s + event<evStopBlink>      / write_uart            = "not_blinking"_s

        );
    }


    int x_{};

};

//static sml::sm<Blinker_sm> bl_sm;


void start_simple_state_machine(UART_HandleTypeDef& huart);

void timer_interrupt();
