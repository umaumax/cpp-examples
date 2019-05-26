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

TEST(arm_neon_test, vrshrq_n_s16) {
  std::vector<short> src_vec = {0, 1, 2, 7, 8, 14, 15, 16};
  std::vector<short> dst_vec(8);
  std::vector<short> ans_vec = {0, 0, 0, 0, 1, 1, 1, 1};
  auto src                   = src_vec.data();
  auto dst                   = dst_vec.data();
  auto ans                   = ans_vec.data();
  int16x8_t src_lane         = vld1q_s16(src);
  int16x8_t dst_lane = vrshrq_n_s16(src_lane, 4);  // (x + 8) / 16 (round off)
  vst1q_s16(dst, dst_lane);

  EXPECT_EQ(dst_vec, ans_vec);
}

TEST(arm_neon_test, vextq_s16_left) {
  std::vector<short> src_vec = {0, 1, 2, 3, 4, 5, 6, 7};
  std::vector<short> dst_vec(8);
  std::vector<short> ans_vec = {1, 2, 3, 4, 5, 6, 7, 0};
  auto src                   = src_vec.data();
  auto dst                   = dst_vec.data();
  auto ans                   = ans_vec.data();
  int16x8_t src_lane         = vld1q_s16(src);
  int16x8_t dst_lane         = vextq_s16(src_lane, src_lane, 1);
  vst1q_s16(dst, dst_lane);

  EXPECT_EQ(dst_vec, ans_vec);
}

TEST(arm_neon_test, vextq_s16_right) {
  std::vector<short> src_vec = {0, 1, 2, 3, 4, 5, 6, 7};
  std::vector<short> dst_vec(8);
  std::vector<short> ans_vec = {7, 0, 1, 2, 3, 4, 5, 6};
  auto src                   = src_vec.data();
  auto dst                   = dst_vec.data();
  auto ans                   = ans_vec.data();
  int16x8_t src_lane         = vld1q_s16(src);
  int16x8_t dst_lane         = vextq_s16(src_lane, src_lane, 8 - 1);
  vst1q_s16(dst, dst_lane);

  EXPECT_EQ(dst_vec, ans_vec);
}

TEST(arm_neon_test, vext) {
  std::vector<short> src_vec = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
  std::vector<short> dst_vec(4);
  // NOTE: last of ans_low_vec == first of ans_high_vec
  std::vector<std::vector<short>> ans_low_vec = {
      {0, 1, 2, 3}, {1, 2, 3, 4}, {2, 3, 4, 5}, {3, 4, 5, 6}, {4, 5, 6, 7},
  };
  std::vector<std::vector<short>> ans_high_vec = {
      {4, 5, 6, 7}, {5, 6, 7, 8}, {6, 7, 8, 9}, {7, 8, 9, 10}, {8, 9, 10, 11},
  };
  auto src = src_vec.data();
  auto dst = dst_vec.data();

  {
    std::vector<int16x8_t> src_lanes(5);
    for (int i = 0; i < src_lanes.size(); i++) {
      int16x8_t src_lane = vld1q_s16(src + i);
      src_lanes[i]       = src_lane;
    }

    for (int i = 0; i < ans_low_vec.size(); i++) {
      vst1_s16(dst, vget_low_s16(src_lanes[i]));
      EXPECT_EQ(dst_vec, ans_low_vec[i]);
    }
    for (int i = 0; i < ans_high_vec.size(); i++) {
      vst1_s16(dst, vget_high_s16(src_lanes[i]));
      EXPECT_EQ(dst_vec, ans_high_vec[i]);
    }
  }

  {
    int16x8_t src_low_lane    = vld1q_s16(src);
    int16x8_t &dst_low_lane_0 = src_low_lane;
    int16x8_t dst_low_lane_1  = vextq_s16(src_low_lane, src_low_lane, 1);
    int16x8_t dst_low_lane_2  = vextq_s16(src_low_lane, src_low_lane, 2);
    int16x8_t dst_low_lane_3  = vextq_s16(src_low_lane, src_low_lane, 3);
    int16x8_t dst_low_lane_4  = vextq_s16(src_low_lane, src_low_lane, 4);

    vst1_s16(dst, vget_low_s16(dst_low_lane_0));
    EXPECT_EQ(dst_vec, ans_low_vec[0]);
    vst1_s16(dst, vget_low_s16(dst_low_lane_1));
    EXPECT_EQ(dst_vec, ans_low_vec[1]);
    vst1_s16(dst, vget_low_s16(dst_low_lane_2));
    EXPECT_EQ(dst_vec, ans_low_vec[2]);
    vst1_s16(dst, vget_low_s16(dst_low_lane_3));
    EXPECT_EQ(dst_vec, ans_low_vec[3]);
    vst1_s16(dst, vget_low_s16(dst_low_lane_4));
    EXPECT_EQ(dst_vec, ans_low_vec[4]);

    int16x8_t src_high_lane    = vld1q_s16(src + 4);
    int16x8_t &dst_high_lane_0 = src_high_lane;
    int16x8_t dst_high_lane_1  = vextq_s16(src_high_lane, src_high_lane, 1);
    int16x8_t dst_high_lane_2  = vextq_s16(src_high_lane, src_high_lane, 2);
    int16x8_t dst_high_lane_3  = vextq_s16(src_high_lane, src_high_lane, 3);
    int16x8_t dst_high_lane_4  = vextq_s16(src_high_lane, src_high_lane, 4);

    vst1_s16(dst, vget_low_s16(dst_high_lane_0));
    EXPECT_EQ(dst_vec, ans_high_vec[0]);
    vst1_s16(dst, vget_low_s16(dst_high_lane_1));
    EXPECT_EQ(dst_vec, ans_high_vec[1]);
    vst1_s16(dst, vget_low_s16(dst_high_lane_2));
    EXPECT_EQ(dst_vec, ans_high_vec[2]);
    vst1_s16(dst, vget_low_s16(dst_high_lane_3));
    EXPECT_EQ(dst_vec, ans_high_vec[3]);
    vst1_s16(dst, vget_low_s16(dst_high_lane_4));
    EXPECT_EQ(dst_vec, ans_high_vec[4]);
  }
}

inline void BM_func_vld1q_s16(int16_t *src, int16_t *dst) {
  for (int i = 0; i < 5; i++) {
    int16x8_t src_lane = vld1q_s16(src + i);
    vst1_s16(dst, vget_low_s16(src_lane));
    vst1_s16(dst + 4, vget_high_s16(src_lane));
    dst += 8;
  }
}

// NOTE: さすがに，ローテーションの処理を行ったほうがわずかに高速(cacheに依存)
inline void BM_func_vext(int16_t *src, int16_t *dst) {
  int16x8_t src_low_lane    = vld1q_s16(src);
  int16x8_t &dst_low_lane_0 = src_low_lane;
  int16x8_t dst_low_lane_1  = vextq_s16(src_low_lane, src_low_lane, 1);
  int16x8_t dst_low_lane_2  = vextq_s16(src_low_lane, src_low_lane, 2);
  int16x8_t dst_low_lane_3  = vextq_s16(src_low_lane, src_low_lane, 3);
  int16x8_t dst_low_lane_4  = vextq_s16(src_low_lane, src_low_lane, 4);

  vst1_s16(dst, vget_low_s16(dst_low_lane_0));
  vst1_s16(dst + 4, vget_low_s16(dst_low_lane_1));
  vst1_s16(dst + 8, vget_low_s16(dst_low_lane_2));
  vst1_s16(dst + 12, vget_low_s16(dst_low_lane_3));
  vst1_s16(dst + 16, vget_low_s16(dst_low_lane_4));

  int16x8_t src_high_lane    = vld1q_s16(src + 4);
  int16x8_t &dst_high_lane_0 = src_high_lane;
  int16x8_t dst_high_lane_1  = vextq_s16(src_high_lane, src_high_lane, 1);
  int16x8_t dst_high_lane_2  = vextq_s16(src_high_lane, src_high_lane, 2);
  int16x8_t dst_high_lane_3  = vextq_s16(src_high_lane, src_high_lane, 3);
  int16x8_t dst_high_lane_4  = vextq_s16(src_high_lane, src_high_lane, 4);

  vst1_s16(dst + 20, vget_low_s16(dst_high_lane_0));
  vst1_s16(dst + 24, vget_low_s16(dst_high_lane_1));
  vst1_s16(dst + 28, vget_low_s16(dst_high_lane_2));
  vst1_s16(dst + 32, vget_low_s16(dst_high_lane_3));
  vst1_s16(dst + 36, vget_low_s16(dst_high_lane_4));
}

static void BM_vext(benchmark::State &state) {
  const int align  = 64;
  const int loop_n = 1000;
  const int src_n  = 8 + 4;
  const int dst_n  = 4 * 5 * 2;
  int16_t *src     = static_cast<int16_t *>(aligned_alloc(
      align, (sizeof(int16_t) * src_n * loop_n + (align - 1)) & ~(align - 1)));
  int16_t *dst     = static_cast<int16_t *>(aligned_alloc(
      align, (sizeof(int16_t) * dst_n + (align - 1)) & ~(align - 1)));
  if (src == nullptr || dst == nullptr)
    state.SkipWithError("memory allocate error");

  for (int i = 0; i < src_n * loop_n; i++) {
    src[i] = i;
  }

  for (auto _ : state) {
    for (int i = 0; i < loop_n; i++) {
      BM_func_vext(src + src_n * i, dst);
    }
  }

  volatile int dummy = dst[0];
  dummy;

  if (src != nullptr) free(src);
  if (dst != nullptr) free(dst);
}
BENCHMARK(BM_vext)->Arg(0);

static void BM_vld1q_s16(benchmark::State &state) {
  const int align  = 64;
  const int loop_n = 1000;
  const int src_n  = 8 + 4;
  const int dst_n  = 4 * 5 * 2;
  int16_t *src     = static_cast<int16_t *>(aligned_alloc(
      align, (sizeof(int16_t) * src_n * loop_n + (align - 1)) & ~(align - 1)));
  int16_t *dst     = static_cast<int16_t *>(aligned_alloc(
      align, (sizeof(int16_t) * dst_n + (align - 1)) & ~(align - 1)));
  if (src == nullptr || dst == nullptr)
    state.SkipWithError("memory allocate error");

  for (int i = 0; i < src_n * loop_n; i++) {
    src[i] = i;
  }

  for (auto _ : state) {
    for (int i = 0; i < loop_n; i++) {
      BM_func_vld1q_s16(src + src_n * i, dst);
    }
  }

  volatile int dummy = dst[0];
  dummy;

  if (src != nullptr) free(src);
  if (dst != nullptr) free(dst);
}
BENCHMARK(BM_vld1q_s16)->Arg(0);
#endif

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  int gtest_ret = RUN_ALL_TESTS();

  // FYI: [c\+\+ \- Google benchmark custom main \- Stack Overflow]( https://stackoverflow.com/questions/34401965/google-benchmark-custom-main )
  ::benchmark::Initialize(&argc, argv);
  ::benchmark::RunSpecifiedBenchmarks();
  return gtest_ret;
}
