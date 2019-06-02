#include <iomanip>
#include <sstream>
#include <vector>

#include <benchmark/benchmark.h>
#include <gtest/gtest.h>

#ifdef __ARM_NEON
#include <arm_neon.h>
#endif

// ---- util funcsions ---- start ----

uint64_t popcnt(uint64_t n) {
  uint64_t c = 0;
  c          = (n & 0x5555555555555555) + ((n >> 1) & 0x5555555555555555);
  c          = (c & 0x3333333333333333) + ((c >> 2) & 0x3333333333333333);
  c          = (c & 0x0f0f0f0f0f0f0f0f) + ((c >> 4) & 0x0f0f0f0f0f0f0f0f);
  c          = (c & 0x00ff00ff00ff00ff) + ((c >> 8) & 0x00ff00ff00ff00ff);
  c          = (c & 0x0000ffff0000ffff) + ((c >> 16) & 0x0000ffff0000ffff);
  c          = (c & 0x00000000ffffffff) + ((c >> 32) & 0x00000000ffffffff);
  return (c);
}

template <class T>
T *aligned_alloc_wrapper(std::size_t n, std::size_t align) {
  assert(popcnt(align) == 1 ||
         !(std::cerr << "align must be 2^n: align=" << align << std::endl));
  return static_cast<T *>(
      aligned_alloc(align, (sizeof(T) * n + (align - 1)) & ~(align - 1)));
}

// ---- util funcsions ---- end ----

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

#define DECL_BM_image_edge_access(name, func)                          \
  static void BM_image_edge_access_##name(benchmark::State &state) {   \
    const int align   = 64;                                            \
    const int width   = state.range(0);                                \
    const int height  = state.range(0);                                \
    const int padding = state.range(1);                                \
    const int n       = width * height;                                \
                                                                       \
    auto src = aligned_alloc_wrapper<uint8_t>(n + padding, align);     \
    auto dst = aligned_alloc_wrapper<int16_t>(3 * n + padding, align); \
    if (src == nullptr || dst == nullptr)                              \
      state.SkipWithError("memory allocate error");                    \
    src += padding;                                                    \
    dst += padding;                                                    \
                                                                       \
    for (int i = 0; i < n; i++) {                                      \
      uint8_t val = i * i + i % 10;                                    \
      src[i]      = val;                                               \
    }                                                                  \
                                                                       \
    for (auto _ : state) {                                             \
      func(src, dst, width, height);                                   \
    }                                                                  \
                                                                       \
    src -= padding;                                                    \
    dst -= padding;                                                    \
    if (src != nullptr) free(src);                                     \
    if (dst != nullptr) free(dst);                                     \
  }                                                                    \
  const int BM_image_edge_access_##name##_size = 64;                   \
  BENCHMARK(BM_image_edge_access_##name)                               \
      ->Args({BM_image_edge_access_##name##_size, 0})                  \
      ->Args({BM_image_edge_access_##name##_size, 0})                  \
      ->Args({BM_image_edge_access_##name##_size, 1})                  \
      ->Args({BM_image_edge_access_##name##_size, 2})                  \
      ->Args({BM_image_edge_access_##name##_size, 3})                  \
      ->Args({BM_image_edge_access_##name##_size, 4})                  \
      ->Args({BM_image_edge_access_##name##_size, 5})                  \
      ->Args({BM_image_edge_access_##name##_size, 6})                  \
      ->Args({BM_image_edge_access_##name##_size, 7})                  \
      ->Args({BM_image_edge_access_##name##_size, 8})                  \
      ->Args({BM_image_edge_access_##name##_size, 9})                  \
      ->Args({BM_image_edge_access_##name##_size, 10})                 \
      ->Args({BM_image_edge_access_##name##_size, 11})                 \
      ->Args({BM_image_edge_access_##name##_size, 12})                 \
      ->Args({BM_image_edge_access_##name##_size, 13})                 \
      ->Args({BM_image_edge_access_##name##_size, 14})                 \
      ->Args({BM_image_edge_access_##name##_size, 15})                 \
      ->Args({BM_image_edge_access_##name##_size, 16});

DECL_BM_image_edge_access(std, image_edge_access);
DECL_BM_image_edge_access(neon, image_edge_access_neon);

TEST(std_and_neon_test, image_edge_access) {
  const int align  = 64;
  const int width  = 32;
  const int height = 32;
  const int n      = width * height;

  auto src_std  = aligned_alloc_wrapper<uint8_t>(n, align);
  auto dst_std  = aligned_alloc_wrapper<int16_t>(3 * n, align);
  auto src_neon = aligned_alloc_wrapper<uint8_t>(n, align);
  auto dst_neon = aligned_alloc_wrapper<int16_t>(3 * n, align);

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
  bool fail_flag = false;
  std::stringstream ss;
  for (int k = 0; k < 3; k++) {
    for (int j = 0; j < height; j++) {
      for (int i = 0; i < width; i++) {
        int index = k * width * height + j * width + i;
        if (dst_std[index] != dst_neon[index]) {
          ss << std::setw(4) << dst_std[index] << "(" << dst_neon[index] << ")";
          fail_flag = true;
        }
        ss << ",";
      }
      ss << std::endl;
    }
    ss << std::endl;
  }
  if (fail_flag) {
    std::cerr << ss.str();
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
