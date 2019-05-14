#include <vector>

#include <benchmark/benchmark.h>
#include <gtest/gtest.h>

#ifdef __ARM_NEON
#include <arm_neon.h>
#endif

#ifdef __ARM_NEON
void image_vertical_access(uint16_t *src, uint16_t *dst, int width, int height,
                           int width_stride) {
  int padding  = 2;
  uint16_t *p0 = src + width_stride * 0 + padding;
  uint16_t *p1 = src + width_stride * 1 + padding;
  uint16_t *p2 = src + width_stride * 2 + padding;
  uint16_t *p3 = src + width_stride * 3 + padding;
  uint16_t *p4 = src + width_stride * 4 + padding;
  dst += width_stride * 2 + padding;
  int width_padding = width_stride - width;

  for (int j = padding; j < height - padding; j++) {
    int i = padding;
    for (; i < width - padding; i++) {
      *dst = *p0 + *p1 + *p2 + *p3 + *p4;
      dst += 1;
      p0 += 1;
      p1 += 1;
      p2 += 1;
      p3 += 1;
      p4 += 1;
    }
    p0 += width_padding + padding * 2;
    p1 += width_padding + padding * 2;
    p2 += width_padding + padding * 2;
    p3 += width_padding + padding * 2;
    p4 += width_padding + padding * 2;
    dst += width_padding + padding * 2;
  }
}
void image_vertical_access_neon(uint16_t *src, uint16_t *dst, int width,
                                int height, int width_stride) {
  int padding  = 2;
  uint16_t *p0 = src + width_stride * 0 + padding;
  uint16_t *p1 = src + width_stride * 1 + padding;
  uint16_t *p2 = src + width_stride * 2 + padding;
  uint16_t *p3 = src + width_stride * 3 + padding;
  uint16_t *p4 = src + width_stride * 4 + padding;
  dst += width_stride * 2 + padding;
  int width_padding = width_stride - width;

  for (int j = padding; j < height - padding; j++) {
    int i = padding;
    for (; i < ((width - padding) & ~(8 - 1)); i += 8) {
      uint16x8_t neon_p0, neon_p1, neon_p2, neon_p3, neon_p4;
      uint16x8_t neon_dst;
      neon_p0  = vld1q_u16(p0);
      neon_p1  = vld1q_u16(p1);
      neon_p2  = vld1q_u16(p2);
      neon_p3  = vld1q_u16(p3);
      neon_p4  = vld1q_u16(p4);
      neon_dst = vaddq_u16(neon_p0, neon_p1);
      neon_dst = vaddq_u16(neon_dst, neon_p2);
      neon_dst = vaddq_u16(neon_dst, neon_p3);
      neon_dst = vaddq_u16(neon_dst, neon_p4);
      vst1q_u16(dst, neon_dst);
      p0 += 8;
      p1 += 8;
      p2 += 8;
      p3 += 8;
      p4 += 8;
      dst += 8;
    }
    for (; i < width - padding; i++) {
      *dst = *p0 + *p1 + *p2 + *p3 + *p4;
      dst += 1;
      p0 += 1;
      p1 += 1;
      p2 += 1;
      p3 += 1;
      p4 += 1;
    }
    p0 += width_padding + padding * 2;
    p1 += width_padding + padding * 2;
    p2 += width_padding + padding * 2;
    p3 += width_padding + padding * 2;
    p4 += width_padding + padding * 2;
    dst += width_padding + padding * 2;
  }
}

static void BM_image_vertical_access(benchmark::State &state) {
  const int align = 16;

  const int width        = state.range(0);
  const int height       = state.range(0);
  const int width_stride = width + state.range(1);

  const int n = width_stride * height;

  int padding=align/sizeof(uint16_t)-2;
  uint16_t *src = static_cast<uint16_t *>(aligned_alloc(
      align, (sizeof(uint16_t) * (n+padding) + (align - 1)) & ~(align - 1)));
  uint16_t *dst = static_cast<uint16_t *>(aligned_alloc(
      align, (sizeof(uint16_t) * (n+padding) + (align - 1)) & ~(align - 1)));
  if (src == nullptr || dst == nullptr)
    state.SkipWithError("memory allocate error");

  for (auto _ : state) {
    image_vertical_access(src+padding, dst+padding, width, height, width_stride);
  }

  if (src != nullptr) free(src);
  if (dst != nullptr) free(dst);
}
BENCHMARK(BM_image_vertical_access)
    ->Args({48, 0})
    ->Args({48, 2})
    ->Args({50, 0})
    ->Args({50, 2})
    ->Args({50, 4})
    ->Args({50, 6})
    ->Args({64, 0})
    ->Args({64, 2})
    ->Args({70, 0})
    ->Args({70, 2});

static void BM_image_vertical_access_neon(benchmark::State &state) {
  const int align = 16;

  const int width        = state.range(0);
  const int height       = state.range(0);
  const int width_stride = width + state.range(1);

  const int n = width_stride * height;

  int padding=align/sizeof(uint16_t)-2;
  uint16_t *src = static_cast<uint16_t *>(aligned_alloc(
      align, (sizeof(uint16_t) * (n+padding) + (align - 1)) & ~(align - 1)));
  uint16_t *dst = static_cast<uint16_t *>(aligned_alloc(
      align, (sizeof(uint16_t) * (n+padding) + (align - 1)) & ~(align - 1)));
  if (src == nullptr || dst == nullptr)
    state.SkipWithError("memory allocate error");

  for (auto _ : state) {
    image_vertical_access_neon(src+padding, dst+padding, width, height, width_stride);
  }

  if (src != nullptr) free(src);
  if (dst != nullptr) free(dst);
}
BENCHMARK(BM_image_vertical_access_neon)
    ->Args({48, 0})
    ->Args({48, 2})
    ->Args({50, 0})
    ->Args({50, 2})
    ->Args({50, 4})
    ->Args({50, 6})
    ->Args({64, 0})
    ->Args({64, 2})
    ->Args({70, 0})
    ->Args({70, 2});
#endif

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  int gtest_ret = RUN_ALL_TESTS();

  // FYI: [c\+\+ \- Google benchmark custom main \- Stack Overflow]( https://stackoverflow.com/questions/34401965/google-benchmark-custom-main )
  ::benchmark::Initialize(&argc, argv);
  ::benchmark::RunSpecifiedBenchmarks();
  return gtest_ret;
}
