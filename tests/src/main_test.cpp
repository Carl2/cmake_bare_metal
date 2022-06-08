#include <string_view>
#include <fmt/format.h>
#include <gtest/gtest.h>
//#include "motor/motor_hw.hpp"

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
    ASSERT_TRUE(true);
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    // InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
