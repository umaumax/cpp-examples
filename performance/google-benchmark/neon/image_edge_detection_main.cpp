#include <vector>

#include <benchmark/benchmark.h>
#include <gtest/gtest.h>

#ifdef __ARM_NEON
#include <arm_neon.h>
#endif

void image_edge_access(int8_t *src, int16_t *dst, int width, int height) {
  // NOTE: src.width  - 1 =  dst.width
  // NOTE: src.height - 1 =  dst.height
  src += width + 1;
  int8_t *p_u = src - width;
  int8_t *p_l = src - 1;
  int8_t *p_c = src;
  int8_t *p_r = src + 1;
  int8_t *p_d = src + width;

  for (int j = 1; j < height - 1; j++) {
    int i = 1;
    for (; i < width - 1; i++) {
      int16_t dx = *p_l - *p_r;
      int16_t dy = *p_u - *p_d;
      // NOTE: 3分割で代入するように
      *dst = dx * dx + dy * dy + dx * dy;
      dst += 1;
      p_u += 1;
      p_l += 1;
      p_c += 1;
      p_r += 1;
      p_d += 1;
    }
  }
}

static void BM_image_edge_access(benchmark::State &state) {
  const int align = 16;

  const int width  = state.range(0);
  const int height = state.range(0);

  const int n = width * height;

  int8_t *src = static_cast<int8_t *>(
      aligned_alloc(align, (sizeof(int8_t) * n + (align - 1)) & ~(align - 1)));
  int16_t *dst = static_cast<int16_t *>(
      aligned_alloc(align, (sizeof(int16_t) * n + (align - 1)) & ~(align - 1)));
  if (src == nullptr || dst == nullptr)
    state.SkipWithError("memory allocate error");

  for (int i = 0; i < n; i++) {
    src[i] = i;
  }

  for (auto _ : state) {
    image_edge_access(src, dst, width, height);
  }

  if (src != nullptr) free(src);
  if (dst != nullptr) free(dst);
}
BENCHMARK(BM_image_edge_access)->Arg(480)->Arg(960);

#ifdef __ARM_NEON
void image_edge_access_neon(int8_t *src, int16_t *dst, int width, int height) {
  // NOTE: src.width  - 1 =  dst.width
  // NOTE: src.height - 1 =  dst.height
  src += width + 1;
  int8_t *p_u = src - width;
  int8_t *p_l = src - 1;
  int8_t *p_c = src;
  int8_t *p_r = src + 1;
  int8_t *p_d = src + width;

  for (int j = 1; j < height - 1; j++) {
    int i = 1;
    for (; i < (width - 1) & ~(16 - 1); i += 16) {
      int8x16_t p_u_lane     = vld1q_s8(p_u);
      int8x8_t p_u_low_lane  = vget_low_s8(p_u_lane);
      int8x8_t p_u_high_lane = vget_low_s8(p_u_lane);
      int8x16_t p_d_lane     = vld1q_s8(p_d);
      int8x8_t p_d_low_lane  = vget_low_s8(p_d_lane);
      int8x8_t p_d_high_lane = vget_low_s8(p_d_lane);
      int8x16_t p_l_lane     = vld1q_s8(p_l);
      int8x8_t p_l_low_lane  = vget_low_s8(p_l_lane);
      int8x8_t p_l_high_lane = vget_low_s8(p_l_lane);
      int8x16_t p_r_lane     = vld1q_s8(p_r);
      int8x8_t p_r_low_lane  = vget_low_s8(p_r_lane);
      int8x8_t p_r_high_lane = vget_low_s8(p_r_lane);

      int16x8_t dx_low_lane  = vsubl_s8(p_l_low_lane, p_r_low_lane);
      int16x8_t dx_high_lane = vsubl_s8(p_l_high_lane, p_r_high_lane);
      int16x8_t dy_low_lane  = vsubl_s8(p_u_low_lane, p_d_low_lane);
      int16x8_t dy_high_lane = vsubl_s8(p_u_high_lane, p_d_high_lane);

      int16x8_t dx_dx_low_lane  = vmulq_s16(dx_low_lane, dx_low_lane);
      int16x8_t dx_dx_high_lane = vmulq_s16(dx_high_lane, dx_high_lane);
      int16x8_t dy_dy_low_lane  = vmulq_s16(dy_low_lane, dy_low_lane);
      int16x8_t dy_dy_high_lane = vmulq_s16(dy_high_lane, dy_high_lane);
      int16x8_t dx_dy_low_lane  = vmulq_s16(dx_low_lane, dy_low_lane);
      int16x8_t dx_dy_high_lane = vmulq_s16(dx_high_lane, dy_high_lane);
      int16x8_t dst_low_lane =
          vaddq_s16(vaddq_s16(dx_dx_low_lane, dy_dy_low_lane), dx_dy_low_lane);
      int16x8_t dst_high_lane = vaddq_s16(
          vaddq_s16(dx_dx_high_lane, dy_dy_high_lane), dx_dy_high_lane);

      vst1q_s16(dst, dst_low_lane);
      vst1q_s16(dst + 8, dst_high_lane);

      dst += 16;
      p_u += 16;
      p_l += 16;
      p_c += 16;
      p_r += 16;
      p_d += 16;
    }
    for (; i < width - 1; i++) {
      int16_t dx = *p_l - *p_r;
      int16_t dy = *p_u - *p_d;
      // NOTE: 3分割で代入するように
      *dst = dx * dx + dy * dy + dx * dy;
      dst += 1;
      p_u += 1;
      p_l += 1;
      p_c += 1;
      p_r += 1;
      p_d += 1;
    }
  }
}

static void BM_image_edge_access_neon(benchmark::State &state) {
  const int align = 16;

  const int width  = state.range(0);
  const int height = state.range(0);

  const int n = width * height;

  int8_t *src = static_cast<int8_t *>(
      aligned_alloc(align, (sizeof(int8_t) * n + (align - 1)) & ~(align - 1)));
  int16_t *dst = static_cast<int16_t *>(
      aligned_alloc(align, (sizeof(int16_t) * n + (align - 1)) & ~(align - 1)));
  if (src == nullptr || dst == nullptr)
    state.SkipWithError("memory allocate error");

  for (int i = 0; i < n; i++) {
    src[i] = i;
  }

  for (auto _ : state) {
    image_edge_access_neon(src, dst, width, height);
  }

  if (src != nullptr) free(src);
  if (dst != nullptr) free(dst);
}
BENCHMARK(BM_image_edge_access_neon)->Arg(480)->Arg(960);

TEST(std_and_neon_test, image_edge_access) {
  const int align  = 16;
  const int width  = 48;
  const int height = 48;
  const int n      = width * height;

  int8_t *src_std = static_cast<int8_t *>(
      aligned_alloc(align, (sizeof(int8_t) * n + (align - 1)) & ~(align - 1)));
  int16_t *dst_std = static_cast<int16_t *>(
      aligned_alloc(align, (sizeof(int16_t) * n + (align - 1)) & ~(align - 1)));
  int8_t *src_neon = static_cast<int8_t *>(
      aligned_alloc(align, (sizeof(int8_t) * n + (align - 1)) & ~(align - 1)));
  int16_t *dst_neon = static_cast<int16_t *>(
      aligned_alloc(align, (sizeof(int16_t) * n + (align - 1)) & ~(align - 1)));

  for (int i = 0; i < n; i++) {
    src_std[i]  = i;
    src_neon[i] = i;
  }

  image_edge_access(src_std, dst_std, width, height);
  image_edge_access_neon(src_neon, dst_neon, width, height);

  for (int i = 0; i < n; i++) {
    EXPECT_FLOAT_EQ(dst_std[i], dst_neon[i]);
  }

  if (src_std != nullptr) free(src_std);
  if (dst_std != nullptr) free(dst_std);
  if (src_neon != nullptr) free(src_neon);
  if (dst_neon != nullptr) free(dst_neon);
}
#endif

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  int gtest_ret = RUN_ALL_TESTS();

  // FYI: [c\+\+ \- Google benchmark custom main \- Stack Overflow]( https://stackoverflow.com/questions/34401965/google-benchmark-custom-main )
  ::benchmark::Initialize(&argc, argv);
  ::benchmark::RunSpecifiedBenchmarks();
  return gtest_ret;
}
