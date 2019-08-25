#include <vector>

#include <benchmark/benchmark.h>
#include <gtest/gtest.h>

#ifdef __APPLE__
void *aligned_alloc(std::size_t align, std::size_t n) {
  void *p = nullptr;
  int ret = (int)posix_memalign(&p, align, n);
  (void)ret;
  assert((ret == 0) ||
         !std::fprintf(stderr,
                       "%s:%d posix_memalign failed (align: %zu, size:%zu)\n",
                       __FILE__, __LINE__, align, n));
  return p;
}
#endif

#ifdef __ARM_NEON
#include <arm_neon.h>
#endif

#ifdef __SSE__
#include <emmintrin.h>

void sse_fill(int *start, int *end, int val) {
  const __m128i val_lane = _mm_set1_epi32(val);

  int size = (end - start);
  int index;
  const int size4 = size & ~(4 - 1);
  for (index = 0; index < size4; index += 4) {
    _mm_store_si128((__m128i *)&start[index], val_lane);
  }
  for (; index < size; index++) {
    start[index] = val;
  }
}

void sse_fill(float *start, float *end, float val) {
  const __m128 val_lane = _mm_set1_ps(val);

  int size = (end - start);
  int index;
  const int size4 = size & ~(4 - 1);
  for (index = 0; index < size4; index += 4) {
    _mm_store_ps(&start[index], val_lane);
  }
  for (; index < size; index++) {
    start[index] = val;
  }
}
#endif

static void BM_std_fill_array(benchmark::State &state) {
  const int align = 8;

  const int width  = state.range(0);
  const int height = state.range(0);

  const int n = width * height;

  float *src = static_cast<float *>(
      aligned_alloc(align, (sizeof(float) * (n) + (align - 1)) & ~(align - 1)));
  if (src == nullptr) state.SkipWithError("memory allocate error");

  for (auto _ : state) {
    // NOTE: fill(0)の場合は内部でmemset?になっているのでは?
    // binaryにしたときにmemsetを適用できるあたいかどうかで速度が決定される
    // NOTE: 高速な例
    // std::fill(src, src + n, 0);
    // std::fill(src, src + n, 1);
    // std::fill((int *)src, (int *)(src + n), 0x48484848);
    std::fill(src, src + n, 824468.5625);
    // NOTE: 例えば，0x33ではprintfで確認するときに値が小さすぎ，0.0000xxx?のようになるので，見た目は0となるので，注意
    // std::memset(src, 0x33, sizeof(float) * n);
    // std::memset(src, 0x40, sizeof(float) * n);
    // std::memset(src, 0x57, sizeof(float) * n);

    // std::fill(src, src + n, 1e6);
  }

#ifdef DEBUG
  for (int i = 0; i < 32; i++) {
    printf("%f,", src[i]);
  }
  printf("\n");
#endif

  if (src != nullptr) free(src);
}
BENCHMARK(BM_std_fill_array)->Arg(4800);

static void BM_sse_fill_array(benchmark::State &state) {
  const int align = 16;

  const int width  = state.range(0);
  const int height = state.range(0);

  const int n = width * height;

  float *src = static_cast<float *>(
      aligned_alloc(align, (sizeof(float) * (n) + (align - 1)) & ~(align - 1)));
  if (src == nullptr) state.SkipWithError("memory allocate error");

  for (auto _ : state) {
    sse_fill(src, src + n, 1e6);
  }

  if (src != nullptr) free(src);
}
BENCHMARK(BM_sse_fill_array)->Arg(4800);

#ifdef __ARM_NEON
#endif

TEST(fill_test, std_fill_normal) {
  const int align = 16;

  const int n   = 17;
  const int val = 1e6;

  int *src = static_cast<int *>(
      aligned_alloc(align, (sizeof(int) * (n) + (align - 1)) & ~(align - 1)));
  ASSERT_NE(nullptr, src);

  std::fill(src, src + n, val);
  for (int i = 0; i < n; i++) {
    EXPECT_EQ(val, src[i]);
  }

  if (src != nullptr) free(src);
}

TEST(fill_test, sse_fill_normal) {
  const int align = 16;

  const int n   = 17;
  const int val = 1e6;

  int *src = static_cast<int *>(
      aligned_alloc(align, (sizeof(int) * (n) + (align - 1)) & ~(align - 1)));
  ASSERT_NE(nullptr, src);

  sse_fill(src, src + n, val);
  for (int i = 0; i < n; i++) {
    EXPECT_EQ(val, src[i]);
  }

  if (src != nullptr) free(src);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  int gtest_ret = RUN_ALL_TESTS();

  // FYI: [c\+\+ \- Google benchmark custom main \- Stack Overflow]( https://stackoverflow.com/questions/34401965/google-benchmark-custom-main )
  ::benchmark::Initialize(&argc, argv);
  ::benchmark::RunSpecifiedBenchmarks();
  return gtest_ret;
}
