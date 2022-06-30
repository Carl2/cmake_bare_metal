#include "command_parser.hpp"
#include <array>
#include <cstdint>
#include <ranges>
#include <string_view>
#include <fmt/format.h>
#include <gtest/gtest.h>

using namespace msg;

namespace
{
OptArgs last_arg;

auto callback = [](msg::OptArgs args, auto start_iter, auto end_iter) {
    uint8_t last{};
    auto add_2 = [&last](uint8_t& val) {
        val  = last + 2;
        last = val;
    };
    std::for_each(start_iter, end_iter, add_2);
    last_arg = args;
    return std::distance(start_iter, end_iter);
};

auto callback_copy = [](msg::OptArgs args, auto start_iter, auto end_iter) {
    static_cast<void>(end_iter);
    if (args)
    {
        auto span = *args;
        std::copy(span.begin(), span.begin() + span.size(), start_iter);
    }
    return (*args).size();
};

}  // namespace

TEST(Cmd_parser, GuppiCmdProtocol_create)
{
    // constexpr GuppiCmdProtocol<GuppiCmd::CMD_ENABLE_ADDRESS_SETUP, decltype(enable_address)>
    //     address_enable_item{enable_address};

    // clang-format off
    constexpr  auto  cmds = make_Guppi_protocol(
        make_cmd_item<GuppiCmd::CMD_ENABLE_ADDRESS_SETUP>(callback),
        make_cmd_item<GuppiCmd::CMD_DISABLE_ADDRESS_SETUP>(callback),
        make_cmd_item<GuppiCmd::CMD_SET_PRINTHEAD_ADDRESS>(callback)
                         );
    // clang-format on

    ASSERT_EQ(std::tuple_size<decltype(cmds)>(), 3);
}

TEST(Cmd_parser, GuppiProtocol_find)
{
    constexpr auto cmds =
        make_Guppi_protocol(make_cmd_item<GuppiCmd::CMD_ENABLE_ADDRESS_SETUP>(callback),
                            make_cmd_item<GuppiCmd::CMD_DISABLE_ADDRESS_SETUP>(callback),
                            make_cmd_item<GuppiCmd::CMD_SET_PRINTHEAD_ADDRESS>(callback_copy));

    {
        msg::RetType ret_buff{};
        auto sz = exec_cmd(cmds, GuppiCmd::CMD_DISABLE_ADDRESS_SETUP, {}, ret_buff.begin(),
                           ret_buff.end());

        ASSERT_EQ(sz, ret_buff.size());
        uint8_t last{};
        for (const auto& item : ret_buff)
        {
            auto val = last + 2;
            last     = val;
            ASSERT_EQ(item, last);
        }
    }
    std::array<uint8_t, 16> args = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
    {
        msg::RetType ret_buff{};
        auto sz = exec_cmd(cmds, GuppiCmd::CMD_SET_PRINTHEAD_ADDRESS, args, ret_buff.begin(),
                           ret_buff.begin() + args.size());

        ASSERT_EQ(sz, args.size());
        for (size_t i = 0; i < args.size(); ++i)
        {
            ASSERT_EQ(ret_buff[i], args[i]);
        }
    };
}

// }
