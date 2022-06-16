#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <numeric>
#include <span>

namespace bin
{

template <typename T>
decltype(auto) convert_nbo(std::input_iterator auto start)
{
    auto g = [](auto prev, auto val) { return ((prev << 8) | val); };

    T init{};
    auto sz = sizeof(T);

    return std::accumulate(start, start + sz, init, g);
}

}  // namespace bin
