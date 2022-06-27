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
        // TODO: Check output, input
    }

    address_sm.enable_address_setup();
    address_sm.disable_address_setup();
}

TEST(address_setup_test, address_check_test)
{
    {
        uart::AddressContext ctx{debug_msg, 0x00'01};  // Setting address to 1..
        uart::AddressSetup address_sm{std::move(ctx)};

        auto fn = uart::get_check_address_fn(address_sm);

        ASSERT_TRUE(fn(uart::BROAD_CAST_ADDRESS));
        ASSERT_TRUE(fn(0x00'01));
        ASSERT_FALSE(fn(0x00'02));
    }
    {
        uart::AddressContext ctx{debug_msg};  // Setting address to 1..
        uart::AddressSetup address_sm{std::move(ctx)};
        auto fn = uart::get_check_address_fn(address_sm);
        ASSERT_TRUE(fn(uart::BROAD_CAST_ADDRESS));
        ASSERT_FALSE(fn(0x00'01));
    }
}

TEST(address_setup_test, statemachine_check)
{
    using namespace uart;
    auto ctx     = uart::AddressContext{debug_msg};
    using MAIN_T = AddressSetup<decltype(ctx)>;
    using SM_T   = MAIN_T::AddressSetupSM;

    {
        sml::sm<SM_T> address_sm_{ctx};
        ASSERT_TRUE(address_sm_.is(MAIN_T::NoAddress));
        address_sm_.process_event(EvEnableAddressSetup{});
        // ASSERT_TRUE(address_sm_.is(MAIN_T::EnumerationMode));
        address_sm_.process_event(EvDisableAddrSetup{});
        ASSERT_TRUE(address_sm_.is(MAIN_T::NoAddress));

        fmt::print("----------\n");
        ctx.phId_ = 1;
        ASSERT_TRUE(address_sm_.is(MAIN_T::NoAddress));
        address_sm_.process_event(EvEnableAddressSetup{});
        // ASSERT_TRUE(address_sm_.is(MAIN_T::EnumerationMode));
        address_sm_.process_event(EvDisableAddrSetup{});
        ASSERT_TRUE(address_sm_.is(MAIN_T::WithAddress));
    }

    fmt::print("----------\n");
    {
        ctx.phId_ = std::nullopt;
        sml::sm<SM_T> address_sm_{ctx};
        ASSERT_TRUE(address_sm_.is(MAIN_T::NoAddress));
        address_sm_.process_event(EvEnableAddressSetup{});
        // ASSERT_TRUE(address_sm_.is(MAIN_T::EnumerationMode));

        address_sm_.process_event(EvSetAddress{0xabcd});
        address_sm_.process_event(EvDisableAddrSetup{});
    }
}

TEST(address_setup_test, command_transform_test)
{
    ASSERT_EQ(msg::command_transform(12), msg::GuppiCmd::CMD_ENABLE_ADDRESS_SETUP);
    ASSERT_EQ(msg::command_transform(13), msg::GuppiCmd::CMD_DISABLE_ADDRESS_SETUP);
    ASSERT_EQ(msg::command_transform(14), msg::GuppiCmd::CMD_SET_PRINTHEAD_ADDRESS);
    auto apa = msg::command_transform(15);
    fmt::print("{}\n", apa);
}
