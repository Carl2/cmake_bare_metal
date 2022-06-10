#pragma once
#include <cstdint>
#include <span>
#include <string_view>
#include "bin_parser.hpp"
#include "boost/sml.hpp"
#include "msg_def.hpp"

namespace sml = boost::sml;

namespace msg
{
namespace
{}  // namespace

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
                static_cast<void>(ctx);

                auto spn = std::span(ev.data_.begin(), ev.sz);
                uint16_t id_val{};
                size_t from               = 0;
                [[maybe_unused]] auto id  = bin::create_span(id_val, from, spn);
                [[maybe_unused]] auto cmd = bin::create_span(id_val, from, spn);
                auto len                  = bin::create_span(id_val, from, spn);
                ctx.uart_msg_init_(len);
                ctx.uart_msg_transmit_("Set uart to be something");
                // std::array<uint8_t, 7> arr = {0x01, 0x01, 0x02, 0x02, 0xFF};
                // auto tp                    = bin::parse_bin(arr);
                //  bin::make_spans<uint16_t, uint8_t>(spn);
            };

            // clang-format off
        return make_transition_table(
            //-[CurrentState]---|------[Event]-----|---[Guard]----|--[Action]---|--Next State-----
            *MsgInit                                                                = WaitForHdr
            ,WaitForHdr          + event<EvMsg>                     / set_msg_len    = WaitForMsg

           );
            // clang-format on
        }

        void f() { apa++; }

        uint16_t apa{};
    };

    SysCtx ctx_;
    sml::sm<MsgMachine> msg_sm_;

 public:
    void new_message(auto& msg)
    {
        // EvMsg ev_msg{};
        // msg_sm_.process_event(EvInit{});
        msg_sm_.process_event(EvMsg{msg, 48});
    }
};

}  // namespace msg
