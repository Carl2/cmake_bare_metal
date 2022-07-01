#pragma once
#include <boost/sml.hpp>

#include "address_def.hpp"
#include "bin_parser.hpp"
#include "command_parser.hpp"

namespace sml = boost::sml;
namespace uart
{

template <typename Context_t>
struct AddressSetup
{
    AddressSetup(Context_t&& ctx) : ctx_{ctx}, address_sm_{AddressSetupSM{}, ctx_} {}

    constexpr static auto WaitForSetCmd  = sml::state<class WaitForSetCmd>;
    constexpr static auto WaitForDisable = sml::state<class WaitForDisable>;
    constexpr static auto NoAddress      = sml::state<class NoAddress>;
    constexpr static auto WithAddress    = sml::state<class WithAddress>;

    constexpr static auto EnablePinState = sml::state<class EnablePinState>;

    struct AddressSetupSM
    {

        auto operator()() const
        {
            using namespace sml;

            auto set_enable_pin = [](const auto& ev, Context_t& ctx) { ctx.set_pin(ev.state); };
            auto is_pin_enable  = [](Context_t& ctx) -> bool { return ctx.get_pin_state(); };

            auto has_uuid = [](Context_t& ctx) -> bool { return ctx.has_id(); };

            auto set_address = [](const auto& ev, Context_t& ctx) { ctx.set_id(ev.address); };
            // clang-format off
            return make_transition_table(
                //-[CurrentState]---|------[Event]-----|---[Guard]----|--[Action]---|--Next State-----
                *NoAddress       + event<EvEnableAddressSetup>                        = WaitForSetCmd
                ,WaitForSetCmd   + event<EvDisableAddrSetup>  [not has_uuid]          = NoAddress
                ,WaitForSetCmd   + event<EvDisableAddrSetup>  [has_uuid]              = WithAddress
                ,WaitForSetCmd   + event<EvSetAddress>     [is_pin_enable] /set_address  = WaitForDisable
                ,WaitForDisable  + event<EvDisableAddrSetup>                          = WithAddress
                ,WithAddress     + event<EvEnableAddressSetup>                        = WaitForSetCmd
                //----------------------------------------
                ,*EnablePinState         + event<EvEnablePinTriggered>             /set_enable_pin
                                         );
            // clang-format on
        }

    };  // End of AddressSetupSM
 private:
    Context_t ctx_;
    sml::sm<AddressSetupSM> address_sm_;

 public:
    bool has_address() const { return ctx_.has_id(); }

    /**
     *  \brief Enable address setup
     *
     *  Generates a EvEnableAddressSetup event
     *
     *  \param nil
     *  \return nil
     */
    void enable_address_setup() { address_sm_.process_event(EvEnableAddressSetup{}); }
    /**
     *  \brief Disables address setup
     *
     *  Generates a EvDisableaddrsetup
     *
     *  \param nil
     *  \return nil
     */
    void disable_address_setup() { address_sm_.process_event(EvDisableAddrSetup{}); }

    /**
     *  \brief Sets addresss
     *
     *  Generates an event to set the address.
     *
     *  \param address (uint16_t)
     *  \return nil
     */
    void set_address(uint16_t address) { address_sm_.process_event(EvSetAddress{address}); }
    /**
     *  \brief Returns optional ph id
     *
     *  Its optional since it needs to optain an address first
     *
     *  \param nil
     *  \return Optional PhId
     */
    decltype(auto) get_address() const { return ctx_.phId_; }
    bool compare_address(uint16_t rhs) const
    {
        // clang-format off
        return (not ctx_.phId_ ? false :
                *(ctx_.get_opt_id()) == rhs ? true : false);
        // clang-format on
    };

    void enable_pin_state(bool state) { address_sm_.process_event(EvEnablePinTriggered{state}); }
    bool is_enable_pin() const { return ctx_.get_pin_state(); }
};

template <typename F>
decltype(auto) make_address_setup(AddressContext<F>&& ctx)
{
    return uart::AddressSetup{ctx};
}

template <typename Context_t>
constexpr decltype(auto) get_enable_address_setup_callback(AddressSetup<Context_t>& address_sm)
{
    return [&address_sm]([[maybe_unused]] msg::OptArgs args, std::input_iterator auto start_iter,
                         [[maybe_unused]] auto end_iter) {
        address_sm.enable_address_setup();
        auto opt_id = address_sm.get_address();
        if (opt_id)
        {
            return msg::make_msg_fn<msg::GuppiCmd::CMD_ACK>()(start_iter, *opt_id);
        }

        // msg::generate_msg(start_iter, msg::GuppiCmd::ACK, {});
        //  constexpr static std::string_view out = "<Enable address>\n\r";
        //  std::copy(out.begin(), out.end(), start_iter);

        return 0;
    };
}

template <typename Context_t>
constexpr decltype(auto) get_disable_address_setup_callback(AddressSetup<Context_t>& address_sm)
{
    return [&address_sm](msg::OptArgs args, auto start_iter, auto end_iter) {
        static_cast<void>(args);
        static_cast<void>(start_iter);
        static_cast<void>(end_iter);
        // constexpr static std::string_view out = "<Disable address>\n\r";
        // std::copy(out.begin(), out.end(), start_iter);
        address_sm.disable_address_setup();
        return 0;
    };
}

template <typename Context_t>
constexpr decltype(auto) get_address_setup_callback(AddressSetup<Context_t>& address_sm)
{

    return [&address_sm](msg::OptArgs args, auto start_iter, auto end_iter) {
        static_cast<void>(start_iter);
        static_cast<void>(end_iter);
        // constexpr static std::string_view out = "<address setup>\n\r";
        // std::copy(out.begin(), out.end(), start_iter);
        if (args)
        {
            auto data_span   = *args;
            uint16_t address = bin::convert_nbo<uint16_t>(data_span.begin());
            address_sm.set_address(address);
        }
        return 0;
    };
}

template <typename Context_t>
constexpr decltype(auto) get_UUID_callback(AddressSetup<Context_t>& address_sm)
{

    return [&address_sm](msg::OptArgs args, auto start_iter, auto end_iter) {
        static_cast<void>(args);
        static_cast<void>(start_iter);
        static_cast<void>(end_iter);
        auto opt_uuid = address_sm.get_address();
        if (opt_uuid)
        {
            *start_iter = static_cast<uint8_t>(((*opt_uuid) & 0xff00) >> 8);
            start_iter++;
            *start_iter = static_cast<uint8_t>((*opt_uuid) & 0x00ff);
        }

        return 2;
    };
}

template <typename Context_t>
auto get_debug_pin_toggle(AddressSetup<Context_t>& address_sm)
{

    return [&address_sm](msg::OptArgs args, auto start_iter, [[maybe_unused]] auto end_iter) {
        constexpr static std::string_view out = "<Toggle PIN>";
        size_t sz                             = 0;
        if (args)
        {
            auto pin_state = bin::convert_nbo<uint8_t>((*args).begin());
            address_sm.enable_pin_state(pin_state > 0 ? true : false);

            std::copy(out.begin(), out.end(), start_iter);
            sz = out.size();
        }
        return sz;
    };
}

/**
 *  \brief Retuns a function to check address of the message
 *
 *  When the message is received, it is split up into a payload part
 *  and a header part. Inside the header there is an ID which
 *  reprepsent where it suppose to be destined. The returning function
 *  returns a bool, the corresponds to if the address is to be parsed
 *  here.
 *
 *  \param Address statemachine reference
 *  \return Fn - function which returns bool if the address has this as destination
 */
template <typename Context_t>
auto get_check_address_fn(AddressSetup<Context_t>& address_sm)
{
    return [&address_sm](uint16_t address) -> msg::AddressMode {
        // clang-format off

        return address == BROAD_CAST_ADDRESS ? msg::AddressMode::BROADCAST :
                address_sm.compare_address(address) ?
                msg::AddressMode::TO_THIS : msg::AddressMode::NOT_APPLICABALE;
        // clang-format on
    };
}

}  // namespace uart
