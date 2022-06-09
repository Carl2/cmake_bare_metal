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

template <std::invocable<uint16_t> T>
struct SystemContext
{

    T uart_msg_init_;
};

template <typename T>
concept is_system_context = requires(T const& sys_ctx, uint16_t val)
{
    sys_ctx.uart_msg_init(val);
};

}  // namespace msg
