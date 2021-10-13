#pragma once
//#include <type_traits>
// #ifdef __cplusplus
// extern "C"
// {
// #endif

#include "boost/sml.hpp"
#include "stm32f4xx_hal.h"

namespace sml = boost::sml;

namespace
{

struct evStartBlink
{};
struct evStopBlink
{};

// clang-format off
auto blinkHandler = []() {
    return [](auto ev) {
        auto isBlinking = false;

        // if( std::is_same< decltype(ev), evStartBlink>() )
        // {
        //     isBlinking = true;
        // }else
        // {
        //     isBlinking = false;
        // }

    };
};
// clang-format on

}  // namespace

struct Blinker_sm
{
    auto operator()() const
    {

        using namespace sml;
        // clang-format off
        return make_transition_table(
            *"init"_s                                                         =
            "not_blinking"_s, "not_blinking"_s + event<evStartBlink> / blinkHandler =
            "blinking"_s, "blinking"_s + event<evStopBlink>      / blinkHandler =
            "not_blinking"_s
        );
    }

    bool isBlinking_{false};
};

static sml::sm<Blinker_sm> bl_sm;


void start_simple_state_machine(UART_HandleTypeDef& huart);

void timer_interrupt();
