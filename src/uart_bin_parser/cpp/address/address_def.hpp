#pragma once
#include <concepts>
#include <cstdint>
#include <optional>
#include <span>

namespace uart
{
using OptPHId_t = std::optional<uint16_t>;

constexpr static uint16_t BROAD_CAST_ADDRESS = 0xaa'aa;

struct EvEnableAddressSetup
{};

struct EvSetAddress
{
    uint16_t address{};
};

struct EvDisableAddrSetup
{};

struct EvEnablePinTriggered
{
    bool state{};
};

///////////////////////////////////////////////////////////////////////////////
//                                  Context                                  //
///////////////////////////////////////////////////////////////////////////////

template <std::invocable<std::string_view> uart_out_t>
struct AddressContext
{

    uart_out_t out_fn_;

    OptPHId_t phId_{std::nullopt};
    bool pin_triggered_{};

    bool has_id() const { return phId_.has_value(); }
    OptPHId_t get_opt_id() const { return phId_; }
    void set_id(uint16_t val) { phId_ = val; }
    void set_pin(bool state) { pin_triggered_ = state; }
    bool get_pin_state() const { return pin_triggered_; }
};

}  // namespace uart
