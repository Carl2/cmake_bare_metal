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

///////////////////////////////////////////////////////////////////////////////
//                                  Context                                  //
///////////////////////////////////////////////////////////////////////////////

template <std::invocable<std::string_view> uart_out_t>
struct AddressContext
{

    uart_out_t out_fn_;

    OptPHId_t phId_{std::nullopt};

    bool has_id() const { return phId_.has_value(); }
    void set_id(uint16_t val) { phId_ = val; }
};

}  // namespace uart
