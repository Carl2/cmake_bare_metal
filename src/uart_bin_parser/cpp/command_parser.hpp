#pragma once
#include <cstdint>
#include <optional>
#include <span>
#include <tuple>
#include <variant>

namespace msg
{

enum class GuppiCmd
{
    CMD_ENABLE_ADDRESS_SETUP  = 12,
    CMD_DISABLE_ADDRESS_SETUP = 13,
    CMD_SET_PRINTHEAD_ADDRESS = 14
};

using OptArgs          = std::optional<std::span<const uint8_t>>;
using RetType          = std::array<uint8_t, 50>;
using RetType_Iterator = RetType::iterator;

namespace impl
{

template <typename Tuple, std::output_iterator<uint8_t> Iter, typename MatchFN_t, size_t... Is>
constexpr decltype(auto) tuple_for_each(Tuple tpl, MatchFN_t match_fn, OptArgs&& args,
                                        Iter start_in, Iter end_in, std::index_sequence<Is...>)
{

    auto f_single = [match_fn, args, start_in, end_in](auto single) {
        if (match_fn(single.CmdNum))
        {
            single.callbackFn_(args, start_in, end_in);
        }
    };

    (f_single(std::get<Is>(tpl)), ...);
}

}  // namespace impl

template <GuppiCmd Nr, std::output_iterator<uint8_t> Iter, std::invocable<OptArgs, Iter, Iter> Fn>
struct GuppiCmdProtocol
{

    using callback_type              = Fn;
    constexpr static GuppiCmd CmdNum = Nr;

    constexpr GuppiCmdProtocol(Fn fn) : callbackFn_{fn} {}

    Fn callbackFn_;
};

template <typename... Args>
constexpr auto make_Guppi_protocol(Args... item)
{
    return std::make_tuple(item...);
}

template <GuppiCmd cmdId, typename Fn>
constexpr auto make_cmd_item(Fn callback)
{
    return GuppiCmdProtocol<cmdId, RetType_Iterator, Fn>{callback};
}

template <std::output_iterator<uint8_t> Iter, typename... Tpls>
constexpr decltype(auto) exec_cmd(std::tuple<Tpls...> cmds, GuppiCmd cmdId, OptArgs&& args,
                                  Iter start_iter, Iter end_iter)
{
    auto match_fn = [cmdId](GuppiCmd current) { return cmdId == current ? true : false; };

    return impl::tuple_for_each(cmds, match_fn, std::move(args), start_iter, end_iter,
                                std::index_sequence_for<Tpls...>{});
}

}  // namespace msg
