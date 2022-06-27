#pragma once
#include <boost/sml.hpp>

#include "address_def.hpp"
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

    struct EnumerationMode
    {
        auto operator()() const
        {
            using namespace sml;
            auto fn = [](std::string_view str) {
                return [str](const auto& ev, Context_t& ctx) {
                    static_cast<void>(ev);
                    static_cast<void>(ctx);
                    // ctx.out_fn_(str);
                };
            };
            auto set_address = [](Context_t& ctx, const auto& ev) {
                ctx.phId_ = ev.address;
                ctx.out_fn_("Address is set");
            };

            // clang-format off
            return make_transition_table(
                //-[CurrentState]---|------[Event]-----|---[Guard]----|--[Action]---|--Next State-----
                //*"init"_s                                       /fn("Wait for SetCmd") = WaitForSetCmd
                *WaitForSetCmd          + event<EvSetAddress>   /set_address          = WaitForDisable
                ,WaitForSetCmd          + sml::on_entry<_>      /fn("ON Entry")
                                         );
            // clang-format on
        }
    };

    constexpr static auto NoAddress          = sml::state<class NoAddress>;
    constexpr static auto WithAddress        = sml::state<class WithAddress>;
    constexpr static auto EnumerationModeSub = sml::state<EnumerationMode>;

    struct AddressSetupSM
    {

        auto operator()() const
        {
            using namespace sml;
            auto fn = [](std::string_view str) {
                return [str](const auto& ev, Context_t& ctx) {
                    static_cast<void>(ev);
                    static_cast<void>(ctx);
                    ctx.out_fn_(str);
                };
            };

            auto has_uuid = [](Context_t& ctx) -> bool { return ctx.has_id(); };
            // clang-format off
            return make_transition_table(
                //-[CurrentState]---|------[Event]-----|---[Guard]----|--[Action]---|--Next State-----
                *NoAddress         + event<EvEnableAddressSetup> /fn("Entering Enum mode")  = EnumerationModeSub
                ,EnumerationModeSub   + event<EvDisableAddrSetup>  [not has_uuid] /fn("No Address") = NoAddress
                ,EnumerationModeSub   + event<EvDisableAddrSetup>  [has_uuid]   /fn("With Address") = WithAddress

                                         );
            // clang-format on
        }

    };  // End of AddressSetupSM
 private:
    Context_t ctx_;
    sml::sm<AddressSetupSM> address_sm_;

 public:
    // TODO: Fix this
    bool has_address() const { return false; }

    void enable_address_setup() { address_sm_.process_event(EvEnableAddressSetup{}); }
    void disable_address_setup() { address_sm_.process_event(EvDisableAddrSetup{}); }
    decltype(auto) get_address() const { return ctx_.phId_; }
    bool compare_address(uint16_t rhs) const
    {
        // clang-format off
        return (not ctx_.phId_ ? false :
                *(ctx_.phId_) == rhs ? true : false);
        // clang-format on
    };
};

template <typename F>
decltype(auto) make_address_setup(AddressContext<F>&& ctx)
{
    return uart::AddressSetup{ctx};
}

template <typename Context_t>
constexpr decltype(auto) get_address_setup_callback(AddressSetup<Context_t>& address_sm)
{
    return [&address_sm](msg::OptArgs args, auto start_iter, auto end_iter) {
        static_cast<void>(args);
        static_cast<void>(start_iter);
        static_cast<void>(end_iter);
        address_sm.enable_address_setup();
    };
}

template <typename Context_t>
constexpr decltype(auto) get_disable_address_setup_callback(AddressSetup<Context_t>& address_sm)
{
    return [&address_sm](msg::OptArgs args, auto start_iter, auto end_iter) {
        static_cast<void>(args);
        static_cast<void>(start_iter);
        static_cast<void>(end_iter);
        address_sm.disable_address_setup();
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
    return [&address_sm](uint16_t address) -> bool {
        // clang-format off
        return address == BROAD_CAST_ADDRESS ? true :
                address_sm.compare_address(address) ? true : false;
        // clang-format on
    };
}

}  // namespace uart
