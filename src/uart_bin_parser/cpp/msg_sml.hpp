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
            auto set_msg_len = [this](const auto& ev, SysCtx& ctx) {
                ctx.receive_header(std::span(ev.data_.begin(), ev.sz));
            };

            auto receive_message = [](auto ev, SysCtx& ctx) {
                ctx.receive_data(std::move(ev.data_), ev.sz);
            };

            auto check_address = [](SysCtx& ctx) {
                static_cast<void>(ctx);
                // Only messsages that are destined to us are important
                return ctx.is_our_msg();
            };

            auto check_crc = [](const auto& ev, SysCtx& ctx) {
                static_cast<void>(ev);
                static_cast<void>(ctx);
                return true;
            };

            // clang-format off
        return make_transition_table(
            //-[CurrentState]---|------[Event]-----|---[Guard]----|--[Action]---|--Next State-----
            *MsgInit                                                                   = WaitForHdr
            ,WaitForHdr          + event<EvMsg>                      / set_msg_len     = WaitForMsg
            ,WaitForMsg   + event<EvMsg> [check_address and check_crc] / receive_message = WaitForHdr
            ,WaitForMsg          + event<EvMsg>      [not check_address]               = WaitForHdr

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
