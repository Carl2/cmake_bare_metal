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
        // msg::Uart_buffer_t data{};
        std::span<const uint8_t> data_span{};
        // size_t sz{};
    };
    Recv_data recv_data;
    uint16_t address{};
    bool ret_address_check{true};  // Return from address check
};

namespace
{
Callback_values cb{};

auto uart_irq_fn          = [](uint16_t sz) { cb.recv_irq_sz = sz; };
auto uart_sync_send       = [](std::string_view view) { cb.sent_data = view; };
auto receive_message_data = [](std::span<const uint8_t> data) {
    // When a new message is received, this will be switched out.
    // So this data is only valid until a new payload is recieved.
    cb.recv_data.data_span = data;
};

auto check_address = [](uint16_t address) {
    cb.address = address;
    return cb.ret_address_check;
};

using sysctx_t = msg::SystemContext<decltype(uart_irq_fn), decltype(uart_sync_send),
                                    decltype(receive_message_data), decltype(check_address)>;
}  // namespace

class Uart_comm_test : public ::testing::Test
{
 protected:
    Uart_comm_test() {}

    sysctx_t ctx{uart_irq_fn, uart_sync_send, receive_message_data, check_address};

    msg::MainMachine<sysctx_t> m_sm{std::move(ctx)};
    //     msg::SystemContext};
};