#pragma once
#include <concepts>
#include <cstdint>
#include <optional>
#include <span>

namespace uart
{

struct EvEnableAddressSetup
{};

struct EvSetAddress
{};

struct EvDisableAddrSetup
{};

///////////////////////////////////////////////////////////////////////////////
//                                  Context                                  //
///////////////////////////////////////////////////////////////////////////////

template <std::invocable<std::string_view> uart_out_t>
struct AddressContext
{
    using OptPHId_t = std::optional<uint16_t>;

    uart_out_t out_fn_;

    OptPHId_t phId_{std::nullopt};

    bool has_id() const { return phId_.has_value(); }
    void set_id(uint16_t val) { phId_ = val; }
};

}  // namespace uart
