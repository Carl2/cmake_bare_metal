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

    struct MsgMachine
    {
        using self = MsgMachine;

        constexpr static auto MsgInit    = sml::state<class MsgInit>;
        constexpr static auto WaitForHdr = sml::state<class WaitForHdr>;
        constexpr static auto WaitForMsg = sml::state<class WaitForMsg>;

        auto operator()() noexcept
        {
            using namespace sml;
            auto set_msg_len = [](const auto& ev, SysCtx& ctx) {
                ctx.receive_header(std::span(ev.data_.begin(), ev.sz));
            };

            auto ignore_message = [](SysCtx& ctx) {
                ctx.uart_msg_init_(HDR_SZ);
            };
            auto receive_message = [](auto ev, SysCtx& ctx) {
                auto [sz, buffer] = ctx.receive_data(std::move(ev.data_), ev.sz);
                // If there is no reply , then size is 0.
                if (sz != 0)
                {
                    auto crc       = crc16_calc(crcRegInit, buffer.begin(), buffer.begin() + sz);
                    buffer[sz]     = static_cast<uint8_t>((crc & 0xff00) >> 8);
                    buffer[sz + 1] = static_cast<uint8_t>(crc & 0xff);
                    //   Send ACK/NACK/GET message PACK together with exec_data
                    ctx.uart_msg_transmit_(std::span<uint8_t>(buffer.data(), sz + 2));
                }
            };

            auto check_address = [](SysCtx& ctx) {
                static_cast<void>(ctx);
                auto address_mode = ctx.is_our_msg();
                ctx.set_address_mode(address_mode);
                // Only messsages that are destined to us are important
                return (address_mode == msg::AddressMode::BROADCAST or
                        address_mode == msg::AddressMode::TO_THIS);
            };

            auto check_crc = [](const auto& ev, SysCtx& ctx) {
                auto view_data = std::span<const uint8_t>(ev.data_.begin(), ev.sz);

                return ctx.is_correct_crc(view_data);
            };

            // clang-format off
        return make_transition_table(
            //-[CurrentState]---|------[Event]-----|---[Guard]----|--[Action]---|--Next State-----
            *MsgInit                                                                       = WaitForHdr
            ,WaitForHdr          + event<EvMsg>                          / set_msg_len     = WaitForMsg
            ,WaitForMsg   + event<EvMsg> [check_address and check_crc]   / receive_message = WaitForHdr
            ,WaitForMsg          + event<EvMsg>      [not check_address] / ignore_message  = WaitForHdr

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
};

}  // namespace msg
