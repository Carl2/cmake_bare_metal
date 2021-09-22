#include <gtest/gtest.h>

/*
 using ::testing::_;
 using ::testing::Return;
 using ::testing::SetArgReferee;
 using ::testing::Throw;
 using ::testing::SaveArg;
 using ::testing::StartsWith;
*/


class MainTest : public ::testing::Test
{
 public:
  MainTest() = default;

  int i_{123};
};

TEST_F(MainTest, init )
{
  EXPECT_EQ(i_,123);
}


int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  //InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}