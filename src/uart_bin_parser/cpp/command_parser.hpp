#pragma once
#include <cstdint>
#include <optional>
#include <span>
#include <tuple>
#include <variant>

namespace msg
{

enum class CmdNr
{
    CMD_ENABLE_ADDRESS_SETUP  = 12,
    CMD_DISABLE_ADDRESS_SETUP = 13,
    CMD_SET_PRINTHEAD_ADDRESS = 14
};
using OptArgs = std::optional<std::span<const uint8_t>>;
using RetType = std::array<uint8_t, 50>;

namespace impl
{

template <typename Tuple, typename Ret_t, typename MatchFN_t, size_t... Is>
constexpr decltype(auto) tuple_for_each(Tuple tpl, MatchFN_t match_fn, OptArgs&& args, Ret_t& ret,
                                        std::index_sequence<Is...>)
{

    // size_t index{};
    auto f_single = [match_fn, args, &ret](auto single) {
        if (match_fn(single.CmdNum))
        {
            single.callbackFn_(args, ret);
        }
    };

    (f_single(std::get<Is>(tpl)), ...);
}

}  // namespace impl

template <CmdNr Nr, std::invocable<OptArgs, RetType&> Fn>
struct GuppiCmdProtocol
{

    using callback_type           = Fn;
    constexpr static CmdNr CmdNum = Nr;

    constexpr GuppiCmdProtocol(Fn fn) : callbackFn_{fn} {}

    Fn callbackFn_;
};

template <typename... Args>
constexpr auto make_Guppi_protocol(Args... item)
{
    return std::make_tuple(item...);
}

template <CmdNr cmdId, std::invocable<OptArgs, RetType&> Fn>
constexpr auto make_cmd_item(Fn callback)
{
    return GuppiCmdProtocol<cmdId, Fn>{callback};
}

template <typename RetVal_t, typename... Tpls>
constexpr decltype(auto) exec_cmd(std::tuple<Tpls...> cmds, CmdNr cmdId, OptArgs&& args,
                                  RetVal_t& ret_val)
{
    auto match_fn = [cmdId](CmdNr current) { return cmdId == current ? true : false; };

    return impl::tuple_for_each(cmds, match_fn, std::move(args), ret_val,
                                std::index_sequence_for<Tpls...>{});
}

}  // namespace msg
