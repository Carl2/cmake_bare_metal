#include <array>
#include <cstdint>
#include <ranges>
#include <string_view>
#include <fmt/format.h>
#include <gtest/gtest.h>
#include "address/address_setup_com.hpp"
#include "command_parser.hpp"

namespace
{

auto debug_msg = [](std::string_view str) { fmt::print("<debug>: {}\n", str); };

}  // namespace

TEST(address_setup_test, init)
{
    uart::AddressSetup address_sm{uart::AddressContext{debug_msg}};

    auto address_setup_item = msg::make_cmd_item<msg::GuppiCmd::CMD_ENABLE_ADDRESS_SETUP>(
        get_address_setup_callback(address_sm));

    auto cmds = msg::make_Guppi_protocol(address_setup_item);
    ASSERT_EQ(std::tuple_size<decltype(cmds)>(), 1);

    {
        msg::RetType ret_buff{};
        exec_cmd(cmds, msg::GuppiCmd::CMD_ENABLE_ADDRESS_SETUP, {}, ret_buff.begin(),
                 ret_buff.end());
    }

    address_sm.enable_address_setup();
    address_sm.disable_address_setup();
}
