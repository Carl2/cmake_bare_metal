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

///////////////////////////////////////////////////////////////////////////////
//                              Message headers/payload                      //
///////////////////////////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////////////////////////
//                        Events for the statemachine                        //
///////////////////////////////////////////////////////////////////////////////
struct EvInit
{};

struct EvMsg
{
    Uart_buffer_t data_{};
    size_t sz{};
};

///////////////////////////////////////////////////////////////////////////////
//                                  Concepts
//    Primarily used to check the context callback functionality
///////////////////////////////////////////////////////////////////////////////
// clang-format off
template<typename F>
concept is_address_check = requires(F& fn,uint16_t address)
{
    {fn(address)} -> std::convertible_to<bool>;
};
// clang-format on

///////////////////////////////////////////////////////////////////////////////
//                              Message context                               //
///////////////////////////////////////////////////////////////////////////////

// clang-format off
template <std::invocable<uint16_t> message_size_fn_t,
          std::invocable<std::string_view> uart_out_t,
          std::invocable<std::span<const uint8_t>> data_recv_fn,
          is_address_check address_check_fn >
struct SystemContext
{
    // clang-format on
    /// message_size_fn_t - a callback function that
    /// sends the message length to the subscriber.
    message_size_fn_t uart_msg_init_;
    /// uart_out_t - Uart out function, used for debugging.
    /// Will be used to send back information, for example
    /// ack and nack
    uart_out_t uart_msg_transmit_;
    /// data_recv_fn - callback that sends back a span of the
    /// message payload includeing (for now) the CRC (2bytes @ end)
    data_recv_fn data_recv_fn_;
    /// Address check - a callback that sends the address part of the
    /// message, expects a bool if the function is destined for this
    /// machine.
    address_check_fn address_check_;

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

    bool is_our_msg() const { return address_check_(hdr.id); }

    Header hdr{};
    Payload msg_data{};
};

}  // namespace msg
