#pragma once
#include <array>
#include <concepts>
#include <span>

namespace msg
{

struct EvInit
{};

struct EvMsg
{
    std::array<uint8_t, 100> data_{};
    size_t sz{};
};

// clang-format off
template <std::invocable<uint16_t> T,
          std::invocable<std::string_view> Uart_Out_t,
          std::invocable<std::span<const uint8_t>> data_recv_fn>
struct SystemContext
{
    // clang-format on
    T uart_msg_init_;
    Uart_Out_t uart_msg_transmit_;
    data_recv_fn data_recv_fn_;

    void receive_data(std::span<const uint8_t> data)
    {
        data_recv_fn_(data);
        uart_msg_transmit_("Data Received");
    }
};

template <typename T>
concept is_system_context = requires(T const& sys_ctx, uint16_t val)
{
    sys_ctx.uart_msg_init(val);
};

}  // namespace msg
