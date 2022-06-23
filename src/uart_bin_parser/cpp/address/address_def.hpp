#pragma once
#include <concepts>
#include <cstdint>
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

    uart_out_t out_fn_;
};

}  // namespace uart
