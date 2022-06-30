#pragma once
#include <cstdint>
#include <ranges>
#include <span>
#include <string_view>
#include "bin_parser.hpp"
#include "boost/sml.hpp"
#include "msg_def.hpp"

namespace sml = boost::sml;

namespace msg
{

template <typename SysCtx>
struct MainMachine
{

    MainMachine(SysCtx&& ctx) : ctx_{ctx}, msg_sm_{MsgMachine{}, ctx_} {}

    constexpr static auto WaitForHdr = sml::state<class WaitForHdr>;
    constexpr static auto WaitForMsg = sml::state<class WaitForMsg>;

    struct MsgMachine
    {
        using self = MsgMachine;

        constexpr static auto MsgInit    = sml::state<class MsgInit>;

        auto operator()() noexcept
        {
            using namespace sml;
            auto process_hdr = [](const auto& ev, SysCtx& ctx) {
                ctx.receive_header(std::span(ev.data_.begin(), ev.sz));
            };

            auto process_body = [](auto ev, SysCtx& ctx) {
                auto [sz, buffer] = ctx.receive_data(std::move(ev.data_), ev.sz);
                auto crc          = crc16_calc(crcRegInit, buffer.begin(), buffer.begin() + sz);

                buffer[sz]     = static_cast<uint8_t>((crc & 0xff00) >> 8);
                buffer[sz + 1] = static_cast<uint8_t>(crc & 0xff);
                //   Send ACK/NACK/GET message PACK together with exec_data
                ctx.uart_msg_transmit_(std::span<uint8_t>(buffer.data(), sz + 2));
            };

            auto start_timer = [](SysCtx& ctx) {
                ctx.toggle_timer_fn_(true);
                // ctx.uart_msg_transmit_("start timer\r\n");
            };

            auto stop_timer = [](SysCtx& ctx) {
                ctx.toggle_timer_fn_(false);
                // ctx.uart_msg_transmit_("stop timer\r\n");
            };

            auto check_addr = [](SysCtx& ctx) {
                static_cast<void>(ctx);
                // Only messsages that are destined to us are important
                return ctx.is_our_msg();
            };

            auto check_crc = [](const auto& ev, SysCtx& ctx) {
                auto view_data = std::span<const uint8_t>(ev.data_.begin(), ev.sz);
                return ctx.is_correct_crc(view_data);
            };

            auto init_hdr_rx = [](SysCtx& ctx) {
                ctx.init_rx();
            };

            auto reset_rx = [](SysCtx& ctx) {
                ctx.abort_uart_rx_();
                // ctx.uart_msg_transmit_("abort rx\r\n");
            };

            auto handle_timeout = [](SysCtx& ctx) {
                ctx.uart_msg_transmit_("timed out\r\n");
            };

            // clang-format off
        return make_transition_table(
            //-[CurrentState]---|------[Event]-----|---[Guard]----|--[Action]---|--Next State-----
            *MsgInit                                                                         = WaitForHdr
            ,WaitForHdr          + event<EvMsg>                        / process_hdr         = WaitForMsg
            ,WaitForMsg   + event<EvMsg> [check_addr and check_crc]    / process_body        = WaitForHdr
            ,WaitForMsg          + event<EvMsg>    [not check_addr]                          = WaitForHdr
            // ------------------------------------------------------------------------------------
            ,WaitForHdr          + sml::on_entry<_> / init_hdr_rx
            ,WaitForMsg          + sml::on_entry<_> / start_timer
            ,WaitForMsg          + event<EvTimeout> / (handle_timeout, reset_rx)             = WaitForHdr
            ,WaitForMsg          + sml::on_exit<_>  / stop_timer
           );
            // clang-format on
        }
    };

    SysCtx ctx_;
    sml::sm<MsgMachine> msg_sm_;

 public:
    // TODO: How the data is handeld should be thought of very carefully
    // When the Event dies the data will dissapear.
    // We could treat iut as a
    void new_message(auto data, size_t sz)
    {
        // EvMsg ev_msg{};
        // msg_sm_.process_event(EvInit{});
        msg_sm_.process_event(EvMsg{std::move(data), sz});
    }

    void timeout()
    {
        msg_sm_.process_event(EvTimeout{});
    }

    bool is_wait4header() const
    {
        return msg_sm_.is(WaitForHdr);
    }

    bool is_wait4msg() const
    {
        return msg_sm_.is(WaitForMsg);
    }
};

}  // namespace msg
