#pragma once
#include <boost/sml.hpp>
#include <cstdint>
#include <optional>
#include <gtest/gtest.h>
#include "msg_def.hpp"
#include "msg_sml.hpp"

struct Callback_values
{
    uint16_t recv_irq_sz{};
    std::string sent_data{};

    // std::span<const uint8_t> recv_data;
    struct Recv_data
    {
        msg::Uart_buffer_t data{};
        size_t sz{};
    };
    Recv_data recv_data;
};

namespace
{
Callback_values cb{};

auto uart_irq_fn          = [](uint16_t sz) { cb.recv_irq_sz = sz; };
auto uart_sync_send       = [](std::string_view view) { cb.sent_data = view; };
auto receive_message_data = [](std::span<const uint8_t> data) {
    // When the EvMsg dies, the data will be lost so we need to copy it.
    memcpy(cb.recv_data.data.data(), data.data(), data.size());
    cb.recv_data.sz = data.size();
};

using sysctx_t = msg::SystemContext<decltype(uart_irq_fn), decltype(uart_sync_send),
                                    decltype(receive_message_data)>;
}  // namespace

class Uart_comm_test : public ::testing::Test
{
 protected:
    Uart_comm_test() {}

    sysctx_t ctx{uart_irq_fn, uart_sync_send, receive_message_data};

    msg::MainMachine<sysctx_t> m_sm{std::move(ctx)};
    //     msg::SystemContext};
};
