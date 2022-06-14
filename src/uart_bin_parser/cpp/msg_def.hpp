#pragma once
#include <array>
#include <concepts>
#include <cstdint>
#include <span>
#include "bin_parser.hpp"

namespace msg
{
// TODO: Does this really need that big?
// Max size of payload is 50 byte :O + crc (2 bytes)

constexpr static size_t CRC_SIZE             = 2;
constexpr static size_t UART_BUFFER_MAX_SIZE = 50 + CRC_SIZE;
constexpr static uint8_t HDR_SZ              = 6;
using Uart_buffer_t                          = std::array<uint8_t, UART_BUFFER_MAX_SIZE>;

struct EvInit
{};

struct EvMsg
{
    Uart_buffer_t data_{};
    size_t sz{};
};

///////////////////////////////////////////////////////////////////////////////
//                                  Uart message struct                      //
///////////////////////////////////////////////////////////////////////////////

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

    template <size_t N>
    void receive_data(std::array<uint8_t, N>&& message, size_t sz)
    {

        msg_data.payload = message;
        std::span<const uint8_t> recv_span(msg_data.payload.begin(), sz);
        data_recv_fn_(recv_span);
        uart_msg_transmit_("Data Received\n\r");
        uart_msg_init_(HDR_SZ);
    }

    void receive_header(std::span<const uint8_t> data)
    {
        uint16_t id_val{};
        size_t from = 0;
        hdr.id      = bin::create_span(id_val, from, data);
        hdr.cmd     = bin::create_span(id_val, from, data);
        hdr.len     = bin::create_span(id_val, from, data);
        uart_msg_init_(hdr.len + 2);  // Setting the message part
    }

    struct Header
    {
        uint16_t id;
        uint16_t cmd;
        uint16_t len;
    };

    struct Payload
    {
        Uart_buffer_t payload{};
        std::span<const uint8_t> crc{};
    };

    Header hdr{};
    Payload msg_data{};
};

template <typename T>
concept is_system_context = requires(T const& sys_ctx, uint16_t val)
{
    sys_ctx.uart_msg_init(val);
};

}  // namespace msg
