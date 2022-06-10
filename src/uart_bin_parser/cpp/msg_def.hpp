#pragma once
#include <array>
#include <concepts>

namespace msg
{

struct EvInit
{};

struct EvMsg
{
    std::array<uint8_t, 100> data_{};
    size_t sz{};
};

template <std::invocable<uint16_t> T, std::invocable<std::string_view> Uart_Out_t>
struct SystemContext
{

    T uart_msg_init_;
    Uart_Out_t uart_msg_transmit_;
};

template <typename T>
concept is_system_context = requires(T const& sys_ctx, uint16_t val)
{
    sys_ctx.uart_msg_init(val);
};

}  // namespace msg
