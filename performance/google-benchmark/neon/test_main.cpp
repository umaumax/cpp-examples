#include <vector>

#include <benchmark/benchmark.h>
#include <gtest/gtest.h>

#ifdef __ARM_NEON
#include <arm_neon.h>
#endif

#ifdef __ARM_NEON
TEST(arm_neon_test, vget_low) {
  std::vector<unsigned char> src_vec = {0, 1, 2,  3,  4,  5,  6,  7,
                                        8, 9, 10, 11, 12, 13, 14, 15};
  std::vector<unsigned char> dst_vec(8);
  std::vector<unsigned char> ans_vec = {0, 1, 2, 3, 4, 5, 6, 7};
  auto src                           = src_vec.data();
  auto dst                           = dst_vec.data();
  auto ans                           = ans_vec.data();
  uint8x16_t src_lane                = vld1q_u8(src);
  uint8x8_t low_lane                 = vget_low_u8(src_lane);
  vst1_u8(dst, low_lane);

  EXPECT_EQ(dst_vec, ans_vec);
}

TEST(arm_neon_test, vget_high) {
  std::vector<unsigned char> src_vec = {0, 1, 2,  3,  4,  5,  6,  7,
                                        8, 9, 10, 11, 12, 13, 14, 15};
  std::vector<unsigned char> dst_vec(8);
  std::vector<unsigned char> ans_vec = {8, 9, 10, 11, 12, 13, 14, 15};
  auto src                           = src_vec.data();
  auto dst                           = dst_vec.data();
  auto ans                           = ans_vec.data();
  uint8x16_t src_lane                = vld1q_u8(src);
  uint8x8_t low_lane                 = vget_high_u8(src_lane);
  vst1_u8(dst, low_lane);

  EXPECT_EQ(dst_vec, ans_vec);
}

TEST(arm_neon_test, vcombine) {
  std::vector<unsigned char> src_vec = {0, 1, 2,  3,  4,  5,  6,  7,
                                        8, 9, 10, 11, 12, 13, 14, 15};
  std::vector<unsigned char> dst_vec(16);
  std::vector<unsigned char> ans_vec = {0, 1, 2,  3,  4,  5,  6,  7,
                                        8, 9, 10, 11, 12, 13, 14, 15};
  auto src                           = src_vec.data();
  auto dst                           = dst_vec.data();
  auto ans                           = ans_vec.data();
  uint8x8_t src_low_lane             = vld1_u8(src);
  uint8x8_t src_high_lane            = vld1_u8(src + 8);
  vst1q_u8(dst, vcombine_u8(src_low_lane, src_high_lane));

  EXPECT_EQ(dst_vec, ans_vec);
}

TEST(arm_neon_test, vdup_n) {
  std::vector<unsigned char> dst_vec(8);
  std::vector<unsigned char> ans_vec = {7, 7, 7, 7, 7, 7, 7, 7};
  auto dst                           = dst_vec.data();
  auto ans                           = ans_vec.data();

  uint8x8_t lane = vdup_n_u8(7);
  vst1_u8(dst, lane);

  EXPECT_EQ(dst_vec, ans_vec);
}

TEST(arm_neon_test, vrshrn_n) {
  std::vector<unsigned short> src_vec = {0, 1, 2, 7, 8, 14, 15, 16};
  std::vector<unsigned char> dst_vec(8);
  std::vector<unsigned char> ans_vec = {0, 0, 0, 0, 1, 1, 1, 1};
  auto src                           = src_vec.data();
  auto dst                           = dst_vec.data();
  auto ans                           = ans_vec.data();
  uint16x8_t src_lane                = vld1q_u16(src);
  uint8x8_t dst_lane = vrshrn_n_u16(src_lane, 4);  // (x + 8) / 16 (round off)
  vst1_u8(dst, dst_lane);

  EXPECT_EQ(dst_vec, ans_vec);
}
#endif

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  int gtest_ret = RUN_ALL_TESTS();

  // FYI: [c\+\+ \- Google benchmark custom main \- Stack Overflow]( https://stackoverflow.com/questions/34401965/google-benchmark-custom-main )
  ::benchmark::Initialize(&argc, argv);
  ::benchmark::RunSpecifiedBenchmarks();
  return gtest_ret;
}
