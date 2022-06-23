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

    constexpr static auto NoAddress       = sml::state<class NoAddress>;
    constexpr static auto WithAddress     = sml::state<class WithAddress>;
    constexpr static auto EnumerationMode = sml::state<class EnumerationMode>;

    struct AddressSetupSM
    {

        auto operator()() const
        {
            using namespace sml;
            auto go = [](const auto& ev, Context_t& ctx) {
                static_cast<void>(ev);
                static_cast<void>(ctx);
                ctx.out_fn_("Entering enumeration mode");
            };

            auto has_uuid = [](Context_t& ctx) -> bool { return ctx.has_id(); };
            // clang-format off
            return make_transition_table(
                //-[CurrentState]---|------[Event]-----|---[Guard]----|--[Action]---|--Next State-----
                *"init"_s                                                           = NoAddress
                ,NoAddress         + event<EvEnableAddressSetup>      /go           = EnumerationMode
                ,EnumerationMode   + event<EvDisableAddrSetup>  [not has_uuid]  /go = NoAddress
                ,EnumerationMode   + event<EvDisableAddrSetup>  [has_uuid]      /go = WithAddress

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

}  // namespace uart
