#pragma once
#include <string_view>
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

    MainMachine(SysCtx&& ctx) : ctx_{ctx}, main_sm_{} {}

    constexpr static auto MsgInit    = sml::state<class MsgInit>;
    constexpr static auto WaitForHdr = sml::state<class WaitForHdr>;

    struct MsgMachine
    {
        auto operator()() const
        {
            using namespace sml;
            // clang-format off
        return make_transition_table(
            //-[CurrentState]---|------[Event]-----|---[Guard]----|--[Action]---|--Next State-----
            *MsgInit                                                  = WaitForHdr
           );
            // clang-format on
        }
    };

    SysCtx ctx_;
    sml::sm<MsgMachine> main_sm_;
};

}  // namespace msg
