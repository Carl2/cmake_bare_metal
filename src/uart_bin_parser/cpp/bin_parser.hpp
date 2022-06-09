#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>

namespace bin
{

template <typename T>
decltype(auto) merge_span(std::span<const uint8_t> data)
{
    T out{};

    memcpy(&out, data.data(), sizeof(T));
    return out;
}

// const auto merge_span_16 = merge_span<uint16_t>();
// const auto merge_span_32 = merge_span<uint32_t>();

auto create_span(auto val, size_t& from, const auto& raw)
{
    auto sz = sizeof(val);
    auto sp = std::span(raw.begin() + from, sz);
    from += sz;
    return merge_span<decltype(val)>(sp);
}

template <typename... Args>
auto make_spans(std::span<uint8_t> raw_data)
{
    size_t i = 0;
    return std::tuple{create_span(Args{}, i, raw_data)...};
    // auto out_tpl = std::tuple{create_span(Args{}, i, raw_data)...};
    // return out_tpl;
}

template <size_t N>
decltype(auto) parse_bin(std::array<uint8_t, N> arr)
{
    return make_spans<uint8_t, uint16_t, uint8_t>(arr);
}

}  // namespace bin
