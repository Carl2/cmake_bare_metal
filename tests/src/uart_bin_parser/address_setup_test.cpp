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
        (void)exec_cmd(cmds, msg::GuppiCmd::CMD_ENABLE_ADDRESS_SETUP, {}, ret_buff.begin(),
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

    {  // DISABLE:  No address, and trasition back to NoAddress (no setaddress event)
        sml::sm<SM_T> address_sm_{ctx};
        ASSERT_TRUE(address_sm_.is(MAIN_T::NoAddress));
        address_sm_.process_event(EvEnableAddressSetup{});
        ASSERT_TRUE(address_sm_.is(MAIN_T::WaitForSetCmd));
        address_sm_.process_event(EvDisableAddrSetup{});
        ASSERT_TRUE(address_sm_.is(MAIN_T::NoAddress));
    }
    fmt::print("----------\n");
    {
        sml::sm<SM_T> address_sm_{ctx};
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

TEST(address_setup_test, statemachine_check_disable)
{
    using namespace uart;
    auto ctx     = uart::AddressContext{debug_msg};
    using MAIN_T = AddressSetup<decltype(ctx)>;
    using SM_T   = MAIN_T::AddressSetupSM;

    {  // DISABLE:  No address, and trasition back to NoAddress (no setaddress event)
        sml::sm<SM_T> address_sm_{ctx};
        ASSERT_TRUE(address_sm_.is(MAIN_T::NoAddress));
        address_sm_.process_event(EvEnableAddressSetup{});
        ASSERT_TRUE(address_sm_.is(MAIN_T::WaitForSetCmd));
        address_sm_.process_event(EvDisableAddrSetup{});
        ASSERT_TRUE(address_sm_.is(MAIN_T::NoAddress));
    }
    fmt::print("----------\n");
    {  // DISABLE While having, have a ID
        sml::sm<SM_T> address_sm_{ctx};
        ctx.phId_ = 1;
        ASSERT_TRUE(address_sm_.is(MAIN_T::NoAddress));
        address_sm_.process_event(EvEnableAddressSetup{});
        ASSERT_TRUE(address_sm_.is(MAIN_T::WaitForSetCmd));
        address_sm_.process_event(EvDisableAddrSetup{});
        ASSERT_TRUE(address_sm_.is(MAIN_T::WithAddress));
    }
}

TEST(address_setup_test, enable_pin)
{
    using namespace uart;
    auto ctx     = uart::AddressContext{debug_msg};
    using MAIN_T = AddressSetup<decltype(ctx)>;
    using SM_T   = MAIN_T::AddressSetupSM;

    {  // Pin triggered
        sml::sm<SM_T> address_sm_{ctx};
        ASSERT_TRUE(address_sm_.is(MAIN_T::NoAddress, MAIN_T::EnablePinState));
        ASSERT_FALSE(ctx.get_pin_state());
        address_sm_.process_event(EvEnablePinTriggered{true});
        ASSERT_TRUE(ctx.get_pin_state());
        address_sm_.process_event(EvEnablePinTriggered{false});
        ASSERT_FALSE(ctx.get_pin_state());
    }
}

TEST(address_setup_test, statemachine_check_set_address)
{
    using namespace uart;
    auto ctx     = uart::AddressContext{debug_msg};
    using MAIN_T = AddressSetup<decltype(ctx)>;
    using SM_T   = MAIN_T::AddressSetupSM;

    {  // Enable a set address sequenece with trigger pin set.
        sml::sm<SM_T> address_sm_{ctx};
        ASSERT_FALSE(ctx.get_pin_state());
        ASSERT_TRUE(address_sm_.is(MAIN_T::NoAddress));
        // Enable the address setup state
        address_sm_.process_event(EvEnableAddressSetup{});
        ASSERT_TRUE(address_sm_.is(MAIN_T::WaitForSetCmd));
        // Set the trigpin.
        address_sm_.process_event(EvEnablePinTriggered{true});
        ASSERT_TRUE(ctx.get_pin_state());
        // Send new address
        address_sm_.process_event(EvSetAddress{0xfeed});
        ASSERT_TRUE(address_sm_.is(MAIN_T::WaitForDisable));
        ASSERT_TRUE(ctx.has_id());
        ASSERT_EQ((*ctx.get_opt_id()), 0xfeed);
        // Send disable
        address_sm_.process_event(EvDisableAddrSetup{});
        ASSERT_TRUE(address_sm_.is(MAIN_T::WithAddress));
    }

    {  // Enable set address without the trigger pin set.
        // New fresh context
        ctx = uart::AddressContext{debug_msg};
        sml::sm<SM_T> address_sm_{ctx};
        //  trigger pin should not be set.
        ASSERT_FALSE(ctx.get_pin_state());
        ASSERT_TRUE(address_sm_.is(MAIN_T::NoAddress));
        // Enable the address setup state
        address_sm_.process_event(EvEnableAddressSetup{});
        ASSERT_TRUE(address_sm_.is(MAIN_T::WaitForSetCmd));
        // Send EvSetAddress (No trigg pin), no transitions should be present
        address_sm_.process_event(EvSetAddress{0xfeed});
        ASSERT_TRUE(address_sm_.is(MAIN_T::WaitForSetCmd));
        // EvDisableaddrsetup - transition to NoAddress
        address_sm_.process_event(EvDisableAddrSetup{});
        ASSERT_TRUE(address_sm_.is(MAIN_T::NoAddress));
    }

    {  // Set new address (already existing)
        ctx       = uart::AddressContext{debug_msg};
        ctx.phId_ = 0xfeed;
        sml::sm<SM_T> address_sm_{ctx};
        // Lets transition to WithAddress
        ASSERT_FALSE(ctx.get_pin_state());
        ASSERT_TRUE(address_sm_.is(MAIN_T::NoAddress));

        address_sm_.process_event(EvEnableAddressSetup{});
        ASSERT_TRUE(address_sm_.is(MAIN_T::WaitForSetCmd));

        address_sm_.process_event(EvDisableAddrSetup{});
        ASSERT_TRUE(address_sm_.is(MAIN_T::WithAddress));

        // Lets first trigger the enable pin
        address_sm_.process_event(EvEnablePinTriggered{true});
        ASSERT_TRUE(ctx.get_pin_state());
        address_sm_.process_event(EvEnableAddressSetup{});
        ASSERT_TRUE(address_sm_.is(MAIN_T::WaitForSetCmd));
        // Send EvSetaddress event
        address_sm_.process_event(EvSetAddress{0x0bad});
        ASSERT_TRUE(address_sm_.is(MAIN_T::WaitForDisable));

        // Disable the trigg pin
        address_sm_.process_event(EvEnablePinTriggered{false});
        ASSERT_FALSE(ctx.get_pin_state());
        // Send Disable Address setup
        address_sm_.process_event(EvDisableAddrSetup{});
        ASSERT_TRUE(address_sm_.is(MAIN_T::WithAddress));
    }
}

TEST(address_setup_test, fake_msg_recv_event_test)
{
    uart::AddressSetup address_sm{uart::AddressContext{debug_msg}};
    ASSERT_FALSE(address_sm.has_address());
    auto cmds = msg::make_Guppi_protocol(
        // Used for Set address
        msg::make_cmd_item<msg::GuppiCmd::CMD_ENABLE_ADDRESS_SETUP>(
            uart::get_enable_address_setup_callback(address_sm)),
        msg::make_cmd_item<msg::GuppiCmd::CMD_DISABLE_ADDRESS_SETUP>(
            uart::get_disable_address_setup_callback(address_sm)),
        msg::make_cmd_item<msg::GuppiCmd::CMD_SET_PRINTHEAD_ADDRESS>(
            uart::get_address_setup_callback(address_sm)),
        // Fake Enable pin
        msg::make_cmd_item<msg::GuppiCmd::DEBUG_ENABLE_PIN>(
            uart::get_debug_pin_toggle(address_sm)));

    ASSERT_EQ(std::tuple_size<decltype(cmds)>(), 4);
    msg::RetType ret_buff{};

    {  // Make a fake Pin Enable
        std::array<uint8_t, 1> arg = {0x01};
        exec_cmd(cmds, msg::GuppiCmd::DEBUG_ENABLE_PIN, arg, ret_buff.begin(), ret_buff.end());
        ASSERT_TRUE(address_sm.is_enable_pin());
    }

    exec_cmd(cmds, msg::GuppiCmd::CMD_ENABLE_ADDRESS_SETUP, {}, ret_buff.begin(), ret_buff.end());
    ASSERT_TRUE(address_sm.is_enable_pin());
    {
        std::array<uint8_t, 2> arg = {0xca, 0xfe};
        exec_cmd(cmds, msg::GuppiCmd::CMD_SET_PRINTHEAD_ADDRESS, arg, ret_buff.begin(),
                 ret_buff.end());
        ASSERT_TRUE(address_sm.has_address());
        auto addr = address_sm.get_address();
        ASSERT_EQ(*addr, 0xcafe);
    }

    {
        exec_cmd(cmds, msg::GuppiCmd::CMD_DISABLE_ADDRESS_SETUP, {}, ret_buff.begin(),
                 ret_buff.end());
        ASSERT_TRUE(address_sm.is_enable_pin());
    }
    {
        std::array<uint8_t, 1> arg = {0x00};
        exec_cmd(cmds, msg::GuppiCmd::DEBUG_ENABLE_PIN, arg, ret_buff.begin(), ret_buff.end());
        ASSERT_FALSE(address_sm.is_enable_pin());
    }
}
