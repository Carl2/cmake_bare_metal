#include <string_view>
#include <fmt/format.h>
#include <gtest/gtest.h>

//#include "motor/motor_hw.hpp"
#include "bin_parser.hpp"

/*
 using ::testing::_;
 using ::testing::Return;
 using ::testing::SetArgReferee;
 using ::testing::Throw;
 using ::testing::SaveArg;
 using ::testing::StartsWith;
*/

TEST(Initial_test, Check_Init)
{

    // std::array<uint8_t, 7> arr = {0x01, 0x01, 0x02, 0x02, 0xFF};

    // auto tpl = bin::make_spans<uint16_t, uint16_t, uint8_t>(arr);

    // fmt::print("1:st {} {} {}\n", std::get<0>(tpl), std::get<1>(tpl), std::get<2>(tpl));
    // {
    //     using array_t = std::array<uint8_t, 3>;
    //     auto tpl1     = bin::make_spans<array_t, uint16_t>(arr);
    //     auto out_arr  = std::get<0>(tpl1);
    //     fmt::print(" [1:st {:#x} {:#x} {:#x}] 2:nd {} \n", out_arr[0], out_arr[1], out_arr[2],
    //                std::get<1>(tpl1));
    // }
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    // InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
