#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <numeric>
#include <span>

namespace bin
{
namespace
{

auto B = [](auto i) { return 8 * (i - 1); };

auto A = [](auto x) { return 0xff << x; };

auto F = [](auto V, auto i) -> uint8_t { return static_cast<uint8_t>((V & A(B(i))) >> B(i)); };

}  // namespace

template <typename T>
decltype(auto) convert_nbo(std::input_iterator auto start)
{
    auto g = [](auto prev, auto val) { return ((prev << 8) | val); };

    T init{};
    auto sz = sizeof(T);

    return std::accumulate(start, start + sz, init, g);
}

template <typename T>
decltype(auto) to_bytes_nbo(T val)
{
    constexpr size_t sz = sizeof(T);
    static_assert(sz < 5, "Unable to use variables of size > 4");

    std::array<uint8_t, sz> out{};
    auto iter = out.begin();

    for (size_t i = sz; i > 0; --i)
    {
        *iter = F(val, i);
        ++iter;
    }

    return out;
}

}  // namespace bin
