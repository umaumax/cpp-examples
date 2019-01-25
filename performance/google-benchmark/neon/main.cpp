#include <cassert>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <vector>

#include <benchmark/benchmark.h>
#include <gtest/gtest.h>

#ifdef __ARM_NEON
#include <arm_neon.h>
#endif

#ifdef __APPLE__
void* aligned_alloc(std::size_t align, std::size_t n) {
  void* p = nullptr;
  int ret = (int)posix_memalign(&p, align, n);
  (void)ret;
  //   assert((ret == 0) || !std::fprintf(stderr, "%s:%d posix_memalign failed (align: %zu, size:%zu)\n", __FILE__, __LINE__, align, n));
  return p;
}
#endif

void add_std(float* a, float* b, float* c, std::size_t size) {
  for (std::size_t i = 0; i < size; i++) {
    c[i] = a[i] + b[i];
  }
}
void mla_std(float* a, float* b, float* c, float* d, std::size_t size) {
  for (std::size_t i = 0; i < size; i++) {
    d[i] = a[i] + b[i] * c[i];
  }
}

void add_std(double* a, double* b, double* c, std::size_t size) {
  for (std::size_t i = 0; i < size; i++) {
    c[i] = a[i] + b[i];
  }
}
void mla_std(double* a, double* b, double* c, double* d, std::size_t size) {
  for (std::size_t i = 0; i < size; i++) {
    d[i] = a[i] + b[i] * c[i];
  }
}

void add_std4(float* a, float* b, float* c, std::size_t size) {
  for (std::size_t i = 0; i < size; i += 4) {
    for (std::size_t j = 0; j < 4; j++) {
      c[i + j] = a[i + j] + b[i + j];
    }
  }
}
void mla_std4(float* a, float* b, float* c, float* d, std::size_t size) {
  for (std::size_t i = 0; i < size; i += 4) {
    for (std::size_t j = 0; j < 4; j++) {
      d[i + j] = a[i + j] + b[i + j] * c[i + j];
    }
  }
}

void add_neon(float* a, float* b, float* c, std::size_t size) {
#ifdef __ARM_NEON
  float32x4_t neon_a, neon_b;
  for (std::size_t i = 0; i < size; i += 4) {
    neon_a = vld1q_f32(a);
    neon_b = vld1q_f32(b);
    vst1q_f32(c, vaddq_f32(neon_a, neon_b));
    a += 4, b += 4, c += 4;
  }
#else
#endif
}
void mla_neon(float* a, float* b, float* c, float* d, std::size_t size) {
#ifdef __ARM_NEON
  float32x4_t neon_a, neon_b, neon_c;
  for (std::size_t i = 0; i < size; i += 4) {
    // NOTE: vld1q_f32: p + 0 ~ p + 3
    neon_a = vld1q_f32(a);
    neon_b = vld1q_f32(b);
    neon_c = vld1q_f32(c);
    // NOTE: d=vmlaq_f32(a,b,c) : d =  a + b * c
    // NOTE: vst1q_f32: p + 0 ~ p + 3
    vst1q_f32(d, vmlaq_f32(neon_a, neon_b, neon_c));
    a += 4, b += 4, c += 4, d += 4;
  }
#else
#endif
}

TEST(mla_test, std_and_neon) {
  static constexpr int align = 8;
  const int n                = 1 * 4;

  alignas(align) float std_v1[n] = {1.0f, 2.0f, 3.0f, 4.0f};
  alignas(align) float std_v2[n] = {10.0f, 20.0f, 30.0f, 40.0f};
  alignas(align) float std_v3[n] = {100.0f, 200.0f, 300.0f, 400.0f};
  alignas(align) float std_v4[n] = {1000.0f, 2000.0f, 3000.0f, 4000.0f};
  mla_std(std_v1, std_v2, std_v3, std_v4, n);

  alignas(align) float neon_v1[n] = {1.0f, 2.0f, 3.0f, 4.0f};
  alignas(align) float neon_v2[n] = {10.0f, 20.0f, 30.0f, 40.0f};
  alignas(align) float neon_v3[n] = {100.0f, 200.0f, 300.0f, 400.0f};
  alignas(align) float neon_v4[n] = {1000.0f, 2000.0f, 3000.0f, 4000.0f};
  mla_neon(neon_v1, neon_v2, neon_v3, neon_v4, n);

  for (int i = 0; i < n; i++) {
    EXPECT_FLOAT_EQ(std_v1[i], neon_v1[i]);
    EXPECT_FLOAT_EQ(std_v2[i], neon_v2[i]);
    EXPECT_FLOAT_EQ(std_v3[i], neon_v3[i]);
    EXPECT_FLOAT_EQ(std_v4[i], neon_v4[i]);
  }
}

TEST(add_test, std_and_neon) {
  static constexpr int align = 8;
  const int n                = 1 * 4;

  alignas(align) float std_v1[n] = {1.0f, 2.0f, 3.0f, 4.0f};
  alignas(align) float std_v2[n] = {10.0f, 20.0f, 30.0f, 40.0f};
  alignas(align) float std_v3[n] = {100.0f, 200.0f, 300.0f, 400.0f};
  add_std(std_v1, std_v2, std_v3, n);

  alignas(align) float neon_v1[n] = {1.0f, 2.0f, 3.0f, 4.0f};
  alignas(align) float neon_v2[n] = {10.0f, 20.0f, 30.0f, 40.0f};
  alignas(align) float neon_v3[n] = {100.0f, 200.0f, 300.0f, 400.0f};
  add_neon(neon_v1, neon_v2, neon_v3, n);

  for (int i = 0; i < n; i++) {
    EXPECT_FLOAT_EQ(std_v1[i], neon_v1[i]);
    EXPECT_FLOAT_EQ(std_v2[i], neon_v2[i]);
    EXPECT_FLOAT_EQ(std_v3[i], neon_v3[i]);
  }
}

// ====
// ====
// ====
// ====

static void BM_add_std(benchmark::State& state) {
  const int align = state.range(0);
  const int n     = 10000 * 4;

  float* v1 = static_cast<float*>(aligned_alloc(align, (sizeof(float) * n + (align - 1)) & ~(align - 1)));
  float* v2 = static_cast<float*>(aligned_alloc(align, (sizeof(float) * n + (align - 1)) & ~(align - 1)));
  float* v3 = static_cast<float*>(aligned_alloc(align, (sizeof(float) * n + (align - 1)) & ~(align - 1)));
  if (v1 == nullptr || v2 == nullptr || v3 == nullptr) state.SkipWithError("memory allocate error");

  for (auto _ : state) add_std(v1, v2, v3, n);

  if (v1 != nullptr) free(v1);
  if (v2 != nullptr) free(v2);
  if (v3 != nullptr) free(v3);
}
// NOTE: don't use align 1 or 2
BENCHMARK(BM_add_std)->Arg(1)->Arg(2)->Arg(4)->Arg(8)->Arg(16)->Arg(32)->Arg(64);

static void BM_add_std4(benchmark::State& state) {
  const int align = state.range(0);
  const int n     = 10000 * 4;

  float* v1 = static_cast<float*>(aligned_alloc(align, (sizeof(float) * n + (align - 1)) & ~(align - 1)));
  float* v2 = static_cast<float*>(aligned_alloc(align, (sizeof(float) * n + (align - 1)) & ~(align - 1)));
  float* v3 = static_cast<float*>(aligned_alloc(align, (sizeof(float) * n + (align - 1)) & ~(align - 1)));
  if (v1 == nullptr || v2 == nullptr || v3 == nullptr) state.SkipWithError("memory allocate error");

  for (auto _ : state) add_std4(v1, v2, v3, n);

  if (v1 != nullptr) free(v1);
  if (v2 != nullptr) free(v2);
  if (v3 != nullptr) free(v3);
}
// NOTE: don't use align 1 or 2
BENCHMARK(BM_add_std4)->Arg(1)->Arg(2)->Arg(4)->Arg(8)->Arg(16)->Arg(32)->Arg(64);

static void BM_add_neon(benchmark::State& state) {
  const int align = state.range(0);
  const int n     = 10000 * 4;

  float* v1 = static_cast<float*>(aligned_alloc(align, (sizeof(float) * n + (align - 1)) & ~(align - 1)));
  float* v2 = static_cast<float*>(aligned_alloc(align, (sizeof(float) * n + (align - 1)) & ~(align - 1)));
  float* v3 = static_cast<float*>(aligned_alloc(align, (sizeof(float) * n + (align - 1)) & ~(align - 1)));
  if (v1 == nullptr || v2 == nullptr || v3 == nullptr) state.SkipWithError("memory allocate error");

  for (auto _ : state) add_neon(v1, v2, v3, n);

  if (v1 != nullptr) free(v1);
  if (v2 != nullptr) free(v2);
  if (v3 != nullptr) free(v3);
}
// NOTE: don't use align 1 or 2
BENCHMARK(BM_add_neon)->Arg(1)->Arg(2)->Arg(4)->Arg(8)->Arg(16)->Arg(32)->Arg(64);

static void BM_add_std_double(benchmark::State& state) {
  const int align = state.range(0);
  const int n     = 10000 * 4;

  double* v1 = static_cast<double*>(aligned_alloc(align, (sizeof(double) * n + (align - 1)) & ~(align - 1)));
  double* v2 = static_cast<double*>(aligned_alloc(align, (sizeof(double) * n + (align - 1)) & ~(align - 1)));
  double* v3 = static_cast<double*>(aligned_alloc(align, (sizeof(double) * n + (align - 1)) & ~(align - 1)));
  if (v1 == nullptr || v2 == nullptr || v3 == nullptr) state.SkipWithError("memory allocate error");

  for (auto _ : state) add_std(v1, v2, v3, n);

  if (v1 != nullptr) free(v1);
  if (v2 != nullptr) free(v2);
  if (v3 != nullptr) free(v3);
}
// NOTE: don't use align 1 or 2
BENCHMARK(BM_add_std_double)->Arg(1)->Arg(2)->Arg(4)->Arg(8)->Arg(16)->Arg(32)->Arg(64);

static void BM_mla_std(benchmark::State& state) {
  const int align = state.range(0);
  const int n     = 10000 * 4;

  float* v1 = static_cast<float*>(::aligned_alloc(align, (sizeof(float) * n + (align - 1)) & ~(align - 1)));
  float* v2 = static_cast<float*>(::aligned_alloc(align, (sizeof(float) * n + (align - 1)) & ~(align - 1)));
  float* v3 = static_cast<float*>(::aligned_alloc(align, (sizeof(float) * n + (align - 1)) & ~(align - 1)));
  float* v4 = static_cast<float*>(::aligned_alloc(align, (sizeof(float) * n + (align - 1)) & ~(align - 1)));
  if (v1 == nullptr || v2 == nullptr || v3 == nullptr || v4 == nullptr) state.SkipWithError("memory allocate error");

  for (auto _ : state) mla_std(v1, v2, v3, v4, n);

  if (v1 != nullptr) free(v1);
  if (v2 != nullptr) free(v2);
  if (v3 != nullptr) free(v3);
  if (v4 != nullptr) free(v4);
}
// NOTE: don't use align 1 or 2
BENCHMARK(BM_mla_std)->Arg(1)->Arg(2)->Arg(4)->Arg(8)->Arg(16)->Arg(32)->Arg(64);

static void BM_mla_std4(benchmark::State& state) {
  const int align = state.range(0);
  const int n     = 10000 * 4;

  float* v1 = static_cast<float*>(::aligned_alloc(align, (sizeof(float) * n + (align - 1)) & ~(align - 1)));
  float* v2 = static_cast<float*>(::aligned_alloc(align, (sizeof(float) * n + (align - 1)) & ~(align - 1)));
  float* v3 = static_cast<float*>(::aligned_alloc(align, (sizeof(float) * n + (align - 1)) & ~(align - 1)));
  float* v4 = static_cast<float*>(::aligned_alloc(align, (sizeof(float) * n + (align - 1)) & ~(align - 1)));
  if (v1 == nullptr || v2 == nullptr || v3 == nullptr || v4 == nullptr) state.SkipWithError("memory allocate error");

  for (auto _ : state) mla_std4(v1, v2, v3, v4, n);

  if (v1 != nullptr) free(v1);
  if (v2 != nullptr) free(v2);
  if (v3 != nullptr) free(v3);
  if (v4 != nullptr) free(v4);
}
// NOTE: don't use align 1 or 2
BENCHMARK(BM_mla_std4)->Arg(1)->Arg(2)->Arg(4)->Arg(8)->Arg(16)->Arg(32)->Arg(64);

static void BM_mla_neon_posix_mem_align(benchmark::State& state) {
  const int align = state.range(0);
  const int n     = 10000 * 4;

  float *v1 = nullptr, *v2 = nullptr, *v3 = nullptr, *v4 = nullptr;
  int ret_v1 = (int)posix_memalign(reinterpret_cast<void**>(&v1), align, sizeof(float) * n);
  int ret_v2 = (int)posix_memalign(reinterpret_cast<void**>(&v2), align, sizeof(float) * n);
  int ret_v3 = (int)posix_memalign(reinterpret_cast<void**>(&v3), align, sizeof(float) * n);
  int ret_v4 = (int)posix_memalign(reinterpret_cast<void**>(&v4), align, sizeof(float) * n);
  if (ret_v1 || ret_v2 || ret_v3 || ret_v4) state.SkipWithError("memory allocate error");

  for (auto _ : state) mla_neon(v1, v2, v3, v4, n);

  if (v1 != nullptr) free(v1);
  if (v2 != nullptr) free(v2);
  if (v3 != nullptr) free(v3);
  if (v4 != nullptr) free(v4);
}
// NOTE: don't use align 1 or 2
BENCHMARK(BM_mla_neon_posix_mem_align)->Arg(1)->Arg(2)->Arg(4)->Arg(8)->Arg(16)->Arg(32)->Arg(64);

static void BM_mla_neon_aligned_alloc(benchmark::State& state) {
  const int align = state.range(0);
  const int n     = 10000 * 4;

  float* v1 = static_cast<float*>(::aligned_alloc(align, (sizeof(float) * n + (align - 1)) & ~(align - 1)));
  float* v2 = static_cast<float*>(::aligned_alloc(align, (sizeof(float) * n + (align - 1)) & ~(align - 1)));
  float* v3 = static_cast<float*>(::aligned_alloc(align, (sizeof(float) * n + (align - 1)) & ~(align - 1)));
  float* v4 = static_cast<float*>(::aligned_alloc(align, (sizeof(float) * n + (align - 1)) & ~(align - 1)));
  if (v1 == nullptr || v2 == nullptr || v3 == nullptr || v4 == nullptr) state.SkipWithError("memory allocate error");

  for (auto _ : state) mla_neon(v1, v2, v3, v4, n);

  if (v1 != nullptr) free(v1);
  if (v2 != nullptr) free(v2);
  if (v3 != nullptr) free(v3);
  if (v4 != nullptr) free(v4);
}
// NOTE: don't use align 1 or 2
BENCHMARK(BM_mla_neon_aligned_alloc)->Arg(1)->Arg(2)->Arg(4)->Arg(8)->Arg(16)->Arg(32)->Arg(64);

static void BM_mla_std_double(benchmark::State& state) {
  const int align = state.range(0);
  const int n     = 10000 * 4;

  double* v1 = static_cast<double*>(::aligned_alloc(align, (sizeof(double) * n + (align - 1)) & ~(align - 1)));
  double* v2 = static_cast<double*>(::aligned_alloc(align, (sizeof(double) * n + (align - 1)) & ~(align - 1)));
  double* v3 = static_cast<double*>(::aligned_alloc(align, (sizeof(double) * n + (align - 1)) & ~(align - 1)));
  double* v4 = static_cast<double*>(::aligned_alloc(align, (sizeof(double) * n + (align - 1)) & ~(align - 1)));
  if (v1 == nullptr || v2 == nullptr || v3 == nullptr || v4 == nullptr) state.SkipWithError("memory allocate error");

  for (auto _ : state) mla_std(v1, v2, v3, v4, n);

  if (v1 != nullptr) free(v1);
  if (v2 != nullptr) free(v2);
  if (v3 != nullptr) free(v3);
  if (v4 != nullptr) free(v4);
}
// NOTE: don't use align 1 or 2
BENCHMARK(BM_mla_std_double)->Arg(1)->Arg(2)->Arg(4)->Arg(8)->Arg(16)->Arg(32)->Arg(64);

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  int gtest_ret = RUN_ALL_TESTS();

  // FYI: [c\+\+ \- Google benchmark custom main \- Stack Overflow]( https://stackoverflow.com/questions/34401965/google-benchmark-custom-main )
  ::benchmark::Initialize(&argc, argv);
  ::benchmark::RunSpecifiedBenchmarks();
  return gtest_ret;
}
