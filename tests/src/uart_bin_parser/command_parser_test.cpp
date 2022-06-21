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

auto enable_address = [](msg::OptArgs args, auto& retBuffer) {
    last_arg     = args;
    retBuffer[0] = 0x01;
};
auto alt_callback = [](msg::OptArgs args, auto& retBuffer) {
    last_arg     = args;
    retBuffer[0] = 0xff;
};

}  // namespace

TEST(Cmd_parser, GuppiCmdProtocol_create)
{
    // constexpr GuppiCmdProtocol<CmdNr::CMD_ENABLE_ADDRESS_SETUP, decltype(enable_address)>
    //     address_enable_item{enable_address};

    // clang-format off
    constexpr  auto  cmds = make_Guppi_protocol(
        make_cmd_item<CmdNr::CMD_ENABLE_ADDRESS_SETUP>(enable_address),
        make_cmd_item<CmdNr::CMD_DISABLE_ADDRESS_SETUP>(enable_address),
        make_cmd_item<CmdNr::CMD_SET_PRINTHEAD_ADDRESS>(alt_callback)
                         );
    // clang-format on

    ASSERT_EQ(std::tuple_size<decltype(cmds)>(), 3);
}

TEST(Cmd_parser, GuppiProtocol_find)
{

    constexpr auto cmds =
        make_Guppi_protocol(make_cmd_item<CmdNr::CMD_ENABLE_ADDRESS_SETUP>(enable_address),
                            make_cmd_item<CmdNr::CMD_DISABLE_ADDRESS_SETUP>(alt_callback),
                            make_cmd_item<CmdNr::CMD_SET_PRINTHEAD_ADDRESS>(alt_callback));

    // ASSERT_EQ(std::tuple_size<decltype(cmds)>(), 3);
    [[maybe_unused]] std::array<uint8_t, 16> args = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
                                                     0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b,
                                                     0x0c, 0x0d, 0x0e, 0x0f};
    {
        msg::RetType ret_buff{};
        exec_cmd(cmds, CmdNr::CMD_DISABLE_ADDRESS_SETUP, {}, ret_buff);

        // auto out_arr = std::get<my_ret_type>(output);
        // fmt::print("size: {}\n", out_arr.size());
    }

    {

        //    Arg1: Commands tuple , arg2: Commmand , argument
        // auto output = exec_cmd(cmds, CmdNr::CMD_ENABLE_ADDRESS_SETUP, {});

        // auto out = std::get<uint8_t>(output);
        // fmt::print("size: {}\n", out);
    }
}
