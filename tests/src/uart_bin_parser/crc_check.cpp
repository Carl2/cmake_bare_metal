#include <array>
#include <cstdint>
#include <ranges>
#include <string_view>
#include <fmt/format.h>
#include <gtest/gtest.h>
#include "crc16_calc.hpp"

namespace
{

// C way of doing it... Not what I want.
uint16_t crcCalculate16(uint16_t initData, const uint8_t* c_ptr, size_t len)
{
    const uint8_t* c = (uint8_t*)c_ptr;
    uint16_t crc     = initData;

    while (len--)
    {
        crc = ((crc << 8) ^ (msg::crc16_ph_table[((crc >> 8) ^ *c++)]));
    }

    return crc;
}

}  // namespace

TEST(crc_test, crc16_test)
{
    std::array<uint8_t, 9> arr = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};
    uint16_t x{0xcafe};
    uint16_t val   = msg::crc16_calc(x, arr.begin(), arr.end());
    uint16_t torsk = crcCalculate16(x, arr.data(), arr.size());

    ASSERT_EQ(val, torsk);
    fmt::print("{0:#x} {1:#x} {0} {1}", val, torsk);
}

TEST(crc_test, broken_up)
{
    std::array<uint8_t, 9> arr_template = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};
    uint16_t val_template = msg::crc16_calc(0xcafe, arr_template.begin(), arr_template.end());

    uint16_t x{0xcafe};
    std::array<uint8_t, 9> arr = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};
    uint16_t crc1              = msg::crc16_calc(x, arr.begin(), arr.begin() + 5);
    uint16_t crc2              = msg::crc16_calc(crc1, arr.begin() + 5, arr.end());
    ASSERT_EQ(val_template, crc2);
    fmt::print("{:#x}", crc2);
}

TEST(crc, single_test)
{
    std::array<uint8_t, 9> arr_template = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};
    uint16_t val_template = msg::crc16_calc(0xcafe, arr_template.begin(), arr_template.end());

    {
        auto crc = msg::crc16_single(0xcafe, static_cast<uint8_t>(0x31));
        crc      = msg::crc16_single(crc, static_cast<uint8_t>(0x32));
        crc      = msg::crc16_single(crc, static_cast<uint8_t>(0x33));
        crc      = msg::crc16_single(crc, static_cast<uint8_t>(0x34));
        crc      = msg::crc16_single(crc, static_cast<uint8_t>(0x35));
        crc      = msg::crc16_single(crc, static_cast<uint8_t>(0x36));
        crc      = msg::crc16_single(crc, static_cast<uint8_t>(0x37));
        crc      = msg::crc16_single(crc, static_cast<uint8_t>(0x38));
        crc      = msg::crc16_single(crc, static_cast<uint8_t>(0x39));

        fmt::print("crc {:#x}\n", crc);

        ASSERT_EQ(crc, val_template);
    }

    {
        auto crc = msg::crc16_single(0xcafe, static_cast<uint8_t>(0x31));
        crc      = msg::crc16_single(crc, static_cast<uint8_t>(0x32));
        crc      = msg::crc16_single(crc, static_cast<uint8_t>(0x33));
        crc      = msg::crc16_single(crc, static_cast<uint8_t>(0x34));

        std::array<uint8_t, 5> arr = {0x35, 0x36, 0x37, 0x38, 0x39};
        uint16_t val               = msg::crc16_calc(crc, arr.begin(), arr.end());

        ASSERT_EQ(val, val_template);
        // fmt::print("APA: {:#x}\n", val);
    }

    auto conv = [](uint16_t val) {
        uint8_t high = static_cast<uint8_t>((val & 0xFF00) >> 8);
        uint8_t low  = static_cast<uint8_t>((val & 0x00FF));
        return std::make_tuple(high, low);
    };

    {
        uint16_t val     = 0x3231;  // Little endian ....
        auto [high, low] = conv(val);
        auto crc         = msg::crc16_single(0xcafe, low);
        crc              = msg::crc16_single(crc, high);

        //  This assumes little endian byte ordered stored in mem
        auto crc16 = msg::crc16_single(0xcafe, val);

        auto crc_8 = msg::crc16_single(0xcafe, static_cast<uint8_t>(0x31));
        crc_8      = msg::crc16_single(crc_8, static_cast<uint8_t>(0x32));

        ASSERT_EQ(crc, crc_8);
        ASSERT_EQ(crc16, crc_8);

        fmt::print("{:#x} {:#x} {:#x}", crc, crc_8, crc16);
    }
}
