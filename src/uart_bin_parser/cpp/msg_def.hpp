#pragma once
#include <array>
#include <concepts>
#include <cstdint>
#include <span>
#include <string>
#include "bin_parser.hpp"
#include "crc16_calc.hpp"

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

    // The size of the header..
    constexpr static size_t size = sizeof(id) + sizeof(cmd) + sizeof(len);
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

template<typename F>
concept is_data_receive_fn = requires(F& fn, const msg::Header& hdr,  std::span<const uint8_t> data, std::span<uint8_t> ret_val)
{
    requires std::invocable<F,const msg::Header& , std::span<const uint8_t>>;
    {fn(hdr, data )} -> std::convertible_to<std::span<const uint8_t>>;
};
// clang-format on

///////////////////////////////////////////////////////////////////////////////
//                              Message context                               //
///////////////////////////////////////////////////////////////////////////////

// clang-format off
template <std::invocable<uint16_t> message_size_fn_t,
          std::invocable<std::string_view> uart_out_t,
          is_data_receive_fn data_recv_fn,
          //std::invocable<const msg::Header& , std::span<const uint8_t>> data_recv_fn,
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
    decltype(auto) receive_data(std::array<uint8_t, N>&& message, size_t sz)
    {

        msg_data.payload = message;
        std::span<const uint8_t> recv_span(msg_data.payload.begin(), sz);
        auto exec_data = data_recv_fn_(hdr, recv_span);
        uart_msg_transmit_("Data Received\n\r");
        uart_msg_init_(HDR_SZ);
        return exec_data;
    }

    void receive_header(std::span<const uint8_t> data)
    {
        hdr.id  = bin::convert_nbo<uint16_t>(data.begin());
        hdr.cmd = bin::convert_nbo<uint16_t>(data.begin() + 2);
        hdr.len = bin::convert_nbo<uint16_t>(data.begin() + 4);

        uart_msg_transmit_("HEADER RECEIVED\n\r");
        // uart_msg_transmit_(std::to_string(hdr.len));
        // uart_msg_transmit_("\n\r");
        uart_msg_init_(hdr.len + 2);  // Setting the message part
    }

    bool is_correct_crc(std::span<const uint8_t> msg_payload) const
    {
        // We need both the hdr and the message in one array
        // Fortunatly this can be done in sequence
        // The CRC is in network byte order
        auto crc_vals     = msg_payload.last<2>();
        auto expected_crc = bin::convert_nbo<uint16_t>(crc_vals.begin());
        uart_msg_transmit_(std::to_string(expected_crc));
        uart_msg_transmit_("\n\r");

        auto crc = msg::crc16_single(0xcafe, hdr.id);
        crc      = msg::crc16_single(crc, hdr.cmd);
        crc      = msg::crc16_single(crc, hdr.len);
        uart_msg_transmit_(std::to_string(crc));
        uart_msg_transmit_("\n\r");

        auto payload = msg_payload.first(msg_payload.size() - 2);
        crc          = crc16_calc(crc, payload.begin(), payload.end());
        uart_msg_transmit_(std::to_string(crc));
        uart_msg_transmit_("\n\r");
        auto isCrcOk = (crc == expected_crc);

        uart_msg_transmit_((isCrcOk ? "True" : "False"));
        uart_msg_transmit_("\n\r");
        return isCrcOk;
    }

    bool is_our_msg() const { return address_check_(hdr.id); }

    Header hdr{};
    Payload msg_data{};
};

}  // namespace msg
