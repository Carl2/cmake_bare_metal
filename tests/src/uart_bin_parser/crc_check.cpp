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

TEST(message_test, crc16_message)
{
    {
        std::array<uint8_t, 12> arr = {0xaa, 0xaa, 0x00, 0x01, 0x00, 0x06,
                                       0x01, 0x02, 0x03, 0x04, 0x05, 0x06};

        uint16_t val = msg::crc16_calc(0xcafe, arr.begin(), arr.end());
        fmt::print("One {:#x}\n", val);
    }

    {
        std::array<uint8_t, 11> arr2 = {0x00, 0x01, 0x00, 0x04, 0x00, 0x05,
                                        0x01, 0x02, 0x03, 0x04, 0x05};
        uint16_t val                 = msg::crc16_calc(0xcafe, arr2.begin(), arr2.end());
        uint16_t torsk               = crcCalculate16(0xcafe, arr2.data(), arr2.size());
        fmt::print("Two {0:#x} = {0} Torsk: {1:#x} \n", val, torsk);
    }

    {
        std::array<uint8_t, 6> hdr = {0xaa, 0xaa, 0x00, 0x01, 0x00, 0x06};
        uint16_t hdr_crc           = msg::crc16_calc(0xcafe, hdr.begin(), hdr.end());

        std::array<uint8_t, 12> arr3 = {0xaa, 0xaa, 0x00, 0x01, 0x00, 0x06,
                                        0x31, 0x32, 0x33, 0x34, 0x35, 0x36};

        uint16_t val = msg::crc16_calc(0xcafe, arr3.begin(), arr3.end());
        fmt::print("Rust {0:#x} ({1:#x},{1:} )\n", val, hdr_crc);
    }
}

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
        uint16_t val     = 0x3132;  // Little endian ....
        auto [high, low] = conv(val);
        auto crc         = msg::crc16_single(0xcafe, high);
        crc              = msg::crc16_single(crc, low);

        //  This assumes little endian byte ordered stored in mem
        auto crc16 = msg::crc16_single_little_endian(0xcafe, val);

        auto crc_8 = msg::crc16_single(0xcafe, static_cast<uint8_t>(0x31));
        crc_8      = msg::crc16_single(crc_8, static_cast<uint8_t>(0x32));

        ASSERT_EQ(crc, crc_8);
        ASSERT_EQ(crc16, crc_8);

        fmt::print("{:#x} {:#x} {:#x}", crc, crc_8, crc16);
    }
}
