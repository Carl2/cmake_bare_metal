#pragma once
#include <concepts>

namespace msg
{

struct EvInit
{};

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
