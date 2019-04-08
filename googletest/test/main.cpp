#include <iostream>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

// NOTE: SetUp/TearDown Environmen
class CustomEnvironment : public ::testing::Environment {
 public:
  CustomEnvironment() {
    std::cout << "CustomEnvironment() called!" << std::endl;
  }
  virtual ~CustomEnvironment() override {
    std::cout << "~CustomEnvironment called!" << std::endl;
  }
  virtual void SetUp() override {
    std::cout << "CustomEnvironment::SetUp() called!" << std::endl;
  }
  virtual void TearDown() override {
    std::cout << "CustomEnvironment::TearDown() called!" << std::endl;
  }
};

// NOTE: Fixture Test
// SetUp/TearDown
// fields
class SampleTest : public ::testing::Test {
 protected:
  virtual void SetUp() override {
    std::cout << "SampleTest::SetUp() called!" << std::endl;
    p  = static_cast<int*>(malloc(sizeof(int) * 1));
    *p = 123;
  }
  virtual void TearDown() override {
    std::cout << "SampleTest::TearDown() called!" << std::endl;
    free(p);
  }
  int* p;
};

TEST_F(SampleTest, true_test) {
  EXPECT_NE(p, nullptr);
  EXPECT_EQ(*p, 123);
}

// NOTE: Paramerter Test
class EvenParamTest : public ::testing::TestWithParam<int> {};
INSTANTIATE_TEST_SUITE_P(small_number, EvenParamTest,
                         ::testing::Values(0, 2, 4, 6));
INSTANTIATE_TEST_SUITE_P(big_number, EvenParamTest,
                         ::testing::Values(100, 102, 104, 106));
TEST_P(EvenParamTest, ok_test) {
  int i = GetParam();
  EXPECT_TRUE(i % 2 == 0);
}

TEST(expect_macro, eq) { EXPECT_EQ(1, 1); }

TEST(expect_macro, ne) { EXPECT_NE(0, 1); }

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  ::testing::AddGlobalTestEnvironment(new CustomEnvironment());
  return RUN_ALL_TESTS();
}
