#include <vector>

#include <benchmark/benchmark.h>
#include <gtest/gtest.h>

#ifdef __ARM_NEON
#include <arm_neon.h>
#endif

void image_edge_access(uint8_t *src, int16_t *dst, int width, int height) {
  // NOTE: src.width  - 1 =  dst.width
  // NOTE: src.height - 1 =  dst.height
  src += width + 1;
  uint8_t *p_u = src - width;
  uint8_t *p_l = src - 1;
  uint8_t *p_c = src;
  uint8_t *p_r = src + 1;
  uint8_t *p_d = src + width;

  dst += width + 1;
  int16_t *pxx = dst + width * height * 0;
  int16_t *pxy = dst + width * height * 1;
  int16_t *pyy = dst + width * height * 2;

  const int margin = 2;
  for (int j = 0; j < height - margin; j++) {
    int i = 0;
    for (; i < width - margin; i++) {
      int dx = ((int)(*p_l) - (int)(*p_r) + 1) >> 1;
      int dy = ((int)(*p_u) - (int)(*p_d) + 1) >> 1;
      *pxx   = (int16_t)(dx * dx);
      *pxy   = (int16_t)(dx * dy);
      *pyy   = (int16_t)(dy * dy);

      pxx += 1;
      pxy += 1;
      pyy += 1;
      p_u += 1;
      p_l += 1;
      p_c += 1;
      p_r += 1;
      p_d += 1;
    }
    pxx += margin;
    pxy += margin;
    pyy += margin;
    p_u += margin;
    p_l += margin;
    p_c += margin;
    p_r += margin;
    p_d += margin;
  }
}

static void BM_image_edge_access(benchmark::State &state) {
  const int align = 16;

  const int width  = state.range(0);
  const int height = state.range(0);

  const int n = width * height;

  uint8_t *src = static_cast<uint8_t *>(
      aligned_alloc(align, (sizeof(uint8_t) * n + (align - 1)) & ~(align - 1)));
  int16_t *dst = static_cast<int16_t *>(aligned_alloc(
      align, (sizeof(int16_t) * 3 * n + (align - 1)) & ~(align - 1)));
  if (src == nullptr || dst == nullptr)
    state.SkipWithError("memory allocate error");

  for (int i = 0; i < n; i++) {
    uint8_t val = i * i + i % 10;
    src[i]      = val;
  }

  for (auto _ : state) {
    image_edge_access(src, dst, width, height);
  }

  if (src != nullptr) free(src);
  if (dst != nullptr) free(dst);
}
BENCHMARK(BM_image_edge_access)->Arg(48)->Arg(50)->Arg(480)->Arg(960);

#ifdef __ARM_NEON
void image_edge_access_neon(uint8_t *src, int16_t *dst, int width, int height) {
  // NOTE: src.width  - 1 =  dst.width
  // NOTE: src.height - 1 =  dst.height
  src += width + 1;
  uint8_t *p_u = src - width;
  uint8_t *p_l = src - 1;
  uint8_t *p_c = src;
  uint8_t *p_r = src + 1;
  uint8_t *p_d = src + width;

  dst += width + 1;
  int16_t *pxx = dst + width * height * 0;
  int16_t *pxy = dst + width * height * 1;
  int16_t *pyy = dst + width * height * 2;

  const int margin = 2;
  for (int j = 0; j < height - margin; j++) {
    int i = 0;
    for (; i < ((width - margin) & ~(16 - 1)); i += 16) {
      uint8x16_t p_u_lane     = vld1q_u8(p_u);
      uint8x8_t p_u_low_lane  = vget_low_u8(p_u_lane);
      uint8x8_t p_u_high_lane = vget_high_u8(p_u_lane);
      uint8x16_t p_d_lane     = vld1q_u8(p_d);
      uint8x8_t p_d_low_lane  = vget_low_u8(p_d_lane);
      uint8x8_t p_d_high_lane = vget_high_u8(p_d_lane);
      uint8x16_t p_l_lane     = vld1q_u8(p_l);
      uint8x8_t p_l_low_lane  = vget_low_u8(p_l_lane);
      uint8x8_t p_l_high_lane = vget_high_u8(p_l_lane);
      uint8x16_t p_r_lane     = vld1q_u8(p_r);
      uint8x8_t p_r_low_lane  = vget_low_u8(p_r_lane);
      uint8x8_t p_r_high_lane = vget_high_u8(p_r_lane);

      // NOTE:
      // int dx = ((int)(*p_l) - (int)(*p_r) + 1) >> 1;
      // int dy = ((int)(*p_u) - (int)(*p_d) + 1) >> 1;
      uint16x8_t dx_low_lane_u  = vsubl_u8(p_l_low_lane, p_r_low_lane);
      uint16x8_t dx_high_lane_u = vsubl_u8(p_l_high_lane, p_r_high_lane);
      uint16x8_t dy_low_lane_u  = vsubl_u8(p_u_low_lane, p_d_low_lane);
      uint16x8_t dy_high_lane_u = vsubl_u8(p_u_high_lane, p_d_high_lane);

      int16x8_t dx_low_lane  = vreinterpretq_s16_u16(dx_low_lane_u);
      int16x8_t dx_high_lane = vreinterpretq_s16_u16(dx_high_lane_u);
      int16x8_t dy_low_lane  = vreinterpretq_s16_u16(dy_low_lane_u);
      int16x8_t dy_high_lane = vreinterpretq_s16_u16(dy_high_lane_u);

      dx_low_lane  = vrshrq_n_s16(dx_low_lane, 1);
      dx_high_lane = vrshrq_n_s16(dx_high_lane, 1);
      dy_low_lane  = vrshrq_n_s16(dy_low_lane, 1);
      dy_high_lane = vrshrq_n_s16(dy_high_lane, 1);

      // NOTE:
      // *pxx   = (int16_t)(dx * dx);
      // *pxy   = (int16_t)(dx * dy);
      // *pyy   = (int16_t)(dy * dy);
      int16x8_t dx_dx_low_lane  = vmulq_s16(dx_low_lane, dx_low_lane);
      int16x8_t dx_dx_high_lane = vmulq_s16(dx_high_lane, dx_high_lane);
      int16x8_t dy_dy_low_lane  = vmulq_s16(dy_low_lane, dy_low_lane);
      int16x8_t dy_dy_high_lane = vmulq_s16(dy_high_lane, dy_high_lane);
      int16x8_t dx_dy_low_lane  = vmulq_s16(dx_low_lane, dy_low_lane);
      int16x8_t dx_dy_high_lane = vmulq_s16(dx_high_lane, dy_high_lane);

      vst1q_s16(pxx, dx_dx_low_lane);
      vst1q_s16(pxx + 8, dx_dx_high_lane);
      vst1q_s16(pxy, dx_dy_low_lane);
      vst1q_s16(pxy + 8, dx_dy_high_lane);
      vst1q_s16(pyy, dy_dy_low_lane);
      vst1q_s16(pyy + 8, dy_dy_high_lane);

      pxx += 16;
      pxy += 16;
      pyy += 16;
      p_u += 16;
      p_l += 16;
      p_c += 16;
      p_r += 16;
      p_d += 16;
    }
    for (; i < width - margin; i++) {
      int dx = ((int)(*p_l) - (int)(*p_r) + 1) >> 1;
      int dy = ((int)(*p_u) - (int)(*p_d) + 1) >> 1;
      *pxx   = (int16_t)(dx * dx);
      *pxy   = (int16_t)(dx * dy);
      *pyy   = (int16_t)(dy * dy);

      pxx += 1;
      pxy += 1;
      pyy += 1;
      p_u += 1;
      p_l += 1;
      p_c += 1;
      p_r += 1;
      p_d += 1;
    }
    pxx += margin;
    pxy += margin;
    pyy += margin;
    p_u += margin;
    p_l += margin;
    p_c += margin;
    p_r += margin;
    p_d += margin;
  }
}

static void BM_image_edge_access_neon(benchmark::State &state) {
  const int align  = 64;
  const int width  = state.range(0);
  const int height = state.range(0);
  const int n      = width * height;

  uint8_t *src = static_cast<uint8_t *>(
      aligned_alloc(align, (sizeof(uint8_t) * n + (align - 1)) & ~(align - 1)));
  int16_t *dst = static_cast<int16_t *>(aligned_alloc(
      align, (sizeof(int16_t) * 3 * n + (align - 1)) & ~(align - 1)));
  if (src == nullptr || dst == nullptr)
    state.SkipWithError("memory allocate error");

  for (int i = 0; i < n; i++) {
    uint8_t val = i * i + i % 10;
    src[i]      = val;
  }

  for (auto _ : state) {
    image_edge_access_neon(src, dst, width, height);
  }

  if (src != nullptr) free(src);
  if (dst != nullptr) free(dst);
}
BENCHMARK(BM_image_edge_access_neon)->Arg(48)->Arg(50)->Arg(480)->Arg(960);

TEST(std_and_neon_test, image_edge_access) {
  const int align  = 64;
  const int width  = 32;
  const int height = 32;
  const int n      = width * height;

  uint8_t *src_std = static_cast<uint8_t *>(
      aligned_alloc(align, (sizeof(uint8_t) * n + (align - 1)) & ~(align - 1)));
  int16_t *dst_std  = static_cast<int16_t *>(aligned_alloc(
      align, (sizeof(int16_t) * 3 * n + (align - 1)) & ~(align - 1)));
  uint8_t *src_neon = static_cast<uint8_t *>(
      aligned_alloc(align, (sizeof(uint8_t) * n + (align - 1)) & ~(align - 1)));
  int16_t *dst_neon = static_cast<int16_t *>(aligned_alloc(
      align, (sizeof(int16_t) * 3 * n + (align - 1)) & ~(align - 1)));

  for (int i = 0; i < n; i++) {
    uint8_t val = i * i + i % 10;
    src_std[i]  = val;
    src_neon[i] = val;
  }
  // NOTE: init value(becasuse edge of matrix is init value)
  for (int i = 0; i < 3 * n; i++) {
    dst_std[i]  = 0;
    dst_neon[i] = 0;
  }

  image_edge_access(src_std, dst_std, width, height);
  image_edge_access_neon(src_neon, dst_neon, width, height);

  for (int i = 0; i < 3 * n; i++) {
    EXPECT_EQ(dst_std[i], dst_neon[i]);
  }
  // NOTE: dump
  for (int k = 0; k < 3; k++) {
    for (int j = 0; j < height; j++) {
      for (int i = 0; i < width; i++) {
        int index = k * width * height + j * width + i;
        if (dst_std[index] != dst_neon[index]) {
          std::printf("%4d(%d)", dst_std[index], dst_neon[index]);
        }
        std::printf(",");
      }
      std::printf("\n");
    }
    std::printf("\n");
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
