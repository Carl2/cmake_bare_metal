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
    msg::Uart_buffer_view sent_data{};

    // std::span<const uint8_t> recv_data;
    struct Recv_data
    {
        // msg::Uart_buffer_t data{};
        std::span<const uint8_t> data_span{};
        msg::Header hdr;
        // size_t sz{};
    };
    Recv_data recv_data;
    uint16_t address{};
    // Return from address check
    msg::AddressMode ret_address_check{msg::AddressMode::TO_THIS};
};

namespace
{
Callback_values cb{};
msg::Uart_buffer_t ret_val = {0x00, 0x00, 0x00, 0x00};

auto uart_irq_fn    = [](uint16_t sz) { cb.recv_irq_sz = sz; };
auto uart_sync_send = [](msg::Uart_buffer_view view) {
    // fmt::print(" {}", view);
    // TODO: Create string_view from the buffer_view
    cb.sent_data = view;
};
auto receive_message_data = [](const msg::Header& hdr, std::span<const uint8_t> data) {
    // When a new message is received, this will be switched out.
    // So this data is only valid until a new payload is recieved.
    cb.recv_data.hdr       = hdr;
    cb.recv_data.data_span = data;
    return std::make_pair(4, ret_val);
};

auto check_address = [](uint16_t address) {
    cb.address = address;
    return cb.ret_address_check;
};

}  // namespace

using sysctx_t = msg::SystemContext<decltype(uart_irq_fn), decltype(uart_sync_send),
                                    decltype(receive_message_data), decltype(check_address)>;

class Uart_comm_test : public ::testing::Test
{
 protected:
    Uart_comm_test() {}

    sysctx_t ctx{uart_irq_fn, uart_sync_send, receive_message_data, check_address};

    msg::UartPacketHandler<sysctx_t> m_sm{std::move(ctx)};
    //     msg::SystemContext};
};
