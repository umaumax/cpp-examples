#include <cassert>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <vector>

#include <benchmark/benchmark.h>
#include <gtest/gtest.h>

#define DECL_BM_image_grid_access_const_image_n (1024)
#define DECL_BM_image_grid_access_const_n(const_type, n)                        \
  static void BM_image_grid_access_##n##_const_##const_type(                    \
      benchmark::State& state) {                                                \
    constexpr const_type x_grid_size = n;                                       \
    constexpr const_type y_grid_size = n;                                       \
    constexpr const_type width       = DECL_BM_image_grid_access_const_image_n; \
    constexpr const_type height      = DECL_BM_image_grid_access_const_image_n; \
    assert(width % x_grid_size == 0 &&                                          \
           "width must be multiple of x_grid_size");                            \
    assert(height % y_grid_size == 0 &&                                         \
           "height must be multiple of y_grid_size");                           \
    std::size_t size = width * height;                                          \
    int sum          = 0;                                                       \
    uint8_t* src     = new uint8_t[size];                                       \
    while (state.KeepRunning()) {                                               \
      for (int j = 0; j < height; j += y_grid_size) {                           \
        for (int i = 0; i < width; i += x_grid_size) {                          \
          const std::size_t offset = width * j + i;                             \
          for (int j = 0; j < y_grid_size; j++) {                               \
            for (int i = 0; i < x_grid_size; i++) {                             \
              sum += src[offset + width * j + i];                               \
            }                                                                   \
          }                                                                     \
        }                                                                       \
      }                                                                         \
    }                                                                           \
    delete[] src;                                                               \
    volatile int dummy = sum;                                                   \
    dummy;                                                                      \
    state.SetLabel(#const_type);                                                \
  }                                                                             \
  BENCHMARK(BM_image_grid_access_##n##_const_##const_type)                      \
      ->Arg(DECL_BM_image_grid_access_const_image_n);
DECL_BM_image_grid_access_const_n(int, 1);
DECL_BM_image_grid_access_const_n(int, 2);
DECL_BM_image_grid_access_const_n(int, 4);
DECL_BM_image_grid_access_const_n(int, 8);
DECL_BM_image_grid_access_const_n(int, 16);
DECL_BM_image_grid_access_const_n(int, 32);
DECL_BM_image_grid_access_const_n(int, 64);
DECL_BM_image_grid_access_const_n(int, 128);
DECL_BM_image_grid_access_const_n(int, 256);
DECL_BM_image_grid_access_const_n(int, 512);
DECL_BM_image_grid_access_const_n(int, 1024);

DECL_BM_image_grid_access_const_n(size_t, 1);
DECL_BM_image_grid_access_const_n(size_t, 2);
DECL_BM_image_grid_access_const_n(size_t, 4);
DECL_BM_image_grid_access_const_n(size_t, 8);
DECL_BM_image_grid_access_const_n(size_t, 16);
DECL_BM_image_grid_access_const_n(size_t, 32);
DECL_BM_image_grid_access_const_n(size_t, 64);
DECL_BM_image_grid_access_const_n(size_t, 128);
DECL_BM_image_grid_access_const_n(size_t, 256);
DECL_BM_image_grid_access_const_n(size_t, 512);
DECL_BM_image_grid_access_const_n(size_t, 1024);
//
static void BM_image_grid_access(benchmark::State& state, int x_grid_size,
                                 int y_grid_size) {
  std::size_t width  = state.range(0);
  std::size_t height = state.range(0);
  assert(width % x_grid_size == 0 && "width must be multiple of x_grid_size");
  assert(height % y_grid_size == 0 && "height must be multiple of y_grid_size");
  std::size_t size = width * height;

  int sum      = 0;
  uint8_t* src = new uint8_t[size];
  while (state.KeepRunning()) {
    // NOTE: width,heightはconstではないので，最適化が無効?
    for (std::size_t j = 0; j < height; j += y_grid_size) {
      for (std::size_t i = 0; i < width; i += x_grid_size) {
        const std::size_t offset = width * j + i;
        // NOTE: grid_sizeはconstなので，最適化が有効
        for (std::size_t j = 0; j < y_grid_size; j++) {
          for (std::size_t i = 0; i < x_grid_size; i++) {
            sum += src[offset + width * j + i];
          }
        }
      }
    }
  }
  delete[] src;

  volatile int dummy = sum;
  dummy;
}

#define DECL_BM_image_grid_access_n(n)                            \
  static void BM_image_grid_access_##n(benchmark::State& state) { \
    BM_image_grid_access(state, n, n);                            \
  }                                                               \
  BENCHMARK(BM_image_grid_access_##n)->Arg(1024);

DECL_BM_image_grid_access_n(1);
DECL_BM_image_grid_access_n(2);
DECL_BM_image_grid_access_n(4);
DECL_BM_image_grid_access_n(8);
DECL_BM_image_grid_access_n(16);
DECL_BM_image_grid_access_n(32);
DECL_BM_image_grid_access_n(64);
DECL_BM_image_grid_access_n(128);
DECL_BM_image_grid_access_n(256);
DECL_BM_image_grid_access_n(512);
DECL_BM_image_grid_access_n(1024);

static void BM_sequencial_access_1_seq(benchmark::State& state) {
  std::size_t width  = state.range(0);
  std::size_t height = 8;
  std::size_t size   = width * height;
  int sum            = 0;
  uint8_t* src0      = new uint8_t[size];
  while (state.KeepRunning()) {
    for (int i = 0; i < size; i++) {
      sum += src0[i];
    }
  }
  delete[] src0;

  volatile int dummy = sum;
  dummy;
}

static void BM_sequencial_access_2_seq(benchmark::State& state) {
  std::size_t width  = state.range(0);
  std::size_t height = 8;
  std::size_t size   = width * height;
  int sum            = 0;
  uint8_t* src0      = new uint8_t[size];
  uint8_t* src1      = src0;
  uint8_t* src2      = src0 + width * height / 2;
  int sum1           = 0;
  int sum2           = 0;
  while (state.KeepRunning()) {
    for (int i = 0; i < width * height / 2; i++) {
      sum1 += src1[i];
      sum2 += src2[i];
    }
  }
  delete[] src0;

  sum                = sum1 + sum2;
  volatile int dummy = sum;
  dummy;
}

static void BM_sequencial_access_4_seq(benchmark::State& state) {
  std::size_t width  = state.range(0);
  std::size_t height = 8;
  std::size_t size   = width * height;
  int sum            = 0;
  uint8_t* src0      = new uint8_t[size];
  uint8_t* src1      = src0;
  uint8_t* src2      = src0 + width * height / 4 * 1;
  uint8_t* src3      = src0 + width * height / 4 * 2;
  uint8_t* src4      = src0 + width * height / 4 * 3;
  int sum1           = 0;
  int sum2           = 0;
  int sum3           = 0;
  int sum4           = 0;
  while (state.KeepRunning()) {
    for (int i = 0; i < width * height / 4; i++) {
      sum1 += src1[i];
      sum2 += src2[i];
      sum3 += src3[i];
      sum4 += src4[i];
    }
  }
  delete[] src0;

  sum                = sum1 + sum2 + sum3 + sum4;
  volatile int dummy = sum;
  dummy;
}

static void BM_sequencial_access_8_seq(benchmark::State& state) {
  std::size_t width  = state.range(0);
  std::size_t height = 8;
  std::size_t size   = width * height;
  int sum            = 0;
  uint8_t* src0      = new uint8_t[size];
  uint8_t* src1      = src0;
  uint8_t* src2      = src0 + width * height / 8 * 1;
  uint8_t* src3      = src0 + width * height / 8 * 2;
  uint8_t* src4      = src0 + width * height / 8 * 3;
  uint8_t* src5      = src0 + width * height / 8 * 4;
  uint8_t* src6      = src0 + width * height / 8 * 5;
  uint8_t* src7      = src0 + width * height / 8 * 6;
  uint8_t* src8      = src0 + width * height / 8 * 7;
  int sum1           = 0;
  int sum2           = 0;
  int sum3           = 0;
  int sum4           = 0;
  int sum5           = 0;
  int sum6           = 0;
  int sum7           = 0;
  int sum8           = 0;
  while (state.KeepRunning()) {
    for (int i = 0; i < width * height / 8; i++) {
      sum1 += src1[i];
      sum2 += src2[i];
      sum3 += src3[i];
      sum4 += src4[i];
      sum5 += src5[i];
      sum6 += src6[i];
      sum7 += src7[i];
      sum8 += src8[i];
    }
  }
  delete[] src0;

  sum                = sum1 + sum2 + sum3 + sum4 + sum5 + sum6 + sum7 + sum8;
  volatile int dummy = sum;
  dummy;
}

// NOTE: benchmarkの実行順番にも影響がある(cache)
// NOTE: -O0で実行すれば，8,4,2,1の順番に高速となるが，-O3では1,2,4がほぼ同じで8が遅い(8本分のcacheができていない?)
BENCHMARK(BM_sequencial_access_1_seq)->Arg(80000);
BENCHMARK(BM_sequencial_access_2_seq)->Arg(80000);
BENCHMARK(BM_sequencial_access_4_seq)->Arg(80000);
BENCHMARK(BM_sequencial_access_8_seq)->Arg(80000);

static void BM_sequencial_access_1_seq_skip_n(benchmark::State& state,
                                              const int n) {
  std::size_t width  = state.range(0);
  std::size_t height = 8;
  std::size_t size   = width * height;
  int sum            = 0;
  uint8_t* src0      = new uint8_t[size];
  while (state.KeepRunning()) {
    for (int i = 0; i < size; i += n) {
      sum += src0[i];
    }
  }
  delete[] src0;

  volatile int dummy = sum;
  dummy;
}

#define DECL_BM_sequencial_access_1_seq_skip_n_const(n)    \
  static void BM_sequencial_access_1_seq_skip_##n##_const( \
      benchmark::State& state) {                           \
    std::size_t width  = state.range(0);                   \
    std::size_t height = 8;                                \
    std::size_t size   = width * height;                   \
    int sum            = 0;                                \
    uint8_t* src0      = new uint8_t[size];                \
    while (state.KeepRunning()) {                          \
      for (int i = 0; i < size; i += n) {                  \
        sum += src0[i];                                    \
      }                                                    \
    }                                                      \
    delete[] src0;                                         \
    volatile int dummy = sum;                              \
    dummy;                                                 \
  }

DECL_BM_sequencial_access_1_seq_skip_n_const(1);
DECL_BM_sequencial_access_1_seq_skip_n_const(2);
DECL_BM_sequencial_access_1_seq_skip_n_const(3);
DECL_BM_sequencial_access_1_seq_skip_n_const(4);
DECL_BM_sequencial_access_1_seq_skip_n_const(5);
DECL_BM_sequencial_access_1_seq_skip_n_const(6);
DECL_BM_sequencial_access_1_seq_skip_n_const(7);
DECL_BM_sequencial_access_1_seq_skip_n_const(8);

static void BM_sequencial_access_1_seq_skip_1(benchmark::State& state) {
  BM_sequencial_access_1_seq_skip_n(state, 1);
}
static void BM_sequencial_access_1_seq_skip_2(benchmark::State& state) {
  BM_sequencial_access_1_seq_skip_n(state, 2);
}
static void BM_sequencial_access_1_seq_skip_3(benchmark::State& state) {
  BM_sequencial_access_1_seq_skip_n(state, 3);
}
static void BM_sequencial_access_1_seq_skip_4(benchmark::State& state) {
  BM_sequencial_access_1_seq_skip_n(state, 4);
}
static void BM_sequencial_access_1_seq_skip_5(benchmark::State& state) {
  BM_sequencial_access_1_seq_skip_n(state, 5);
}
static void BM_sequencial_access_1_seq_skip_6(benchmark::State& state) {
  BM_sequencial_access_1_seq_skip_n(state, 6);
}
static void BM_sequencial_access_1_seq_skip_7(benchmark::State& state) {
  BM_sequencial_access_1_seq_skip_n(state, 7);
}
static void BM_sequencial_access_1_seq_skip_8(benchmark::State& state) {
  BM_sequencial_access_1_seq_skip_n(state, 8);
}

// NOTE
// iのインクリメントがconstの方が高速
// インクリメントが1以外の場合(特に3)のときには，なぜか処理をskipしているのにもかかわらず，速度低下
// 4,8のskipが効率的
// つまり，処理をskipした倍数分高速化するわけではない
// 上記の理由はコンパイラの最適化によるものでは?
// 試しに，-O0とすると，狙い通りの実行時間(比例)となるため
// uint8_tを16,32,64にすると面白い
// ちなみに，ループアンローリングをすると遅くなる why?
BENCHMARK(BM_sequencial_access_1_seq_skip_1_const)->Arg(80000);
BENCHMARK(BM_sequencial_access_1_seq_skip_2_const)->Arg(80000);
BENCHMARK(BM_sequencial_access_1_seq_skip_3_const)->Arg(80000);
BENCHMARK(BM_sequencial_access_1_seq_skip_4_const)->Arg(80000);
BENCHMARK(BM_sequencial_access_1_seq_skip_5_const)->Arg(80000);
BENCHMARK(BM_sequencial_access_1_seq_skip_6_const)->Arg(80000);
BENCHMARK(BM_sequencial_access_1_seq_skip_7_const)->Arg(80000);
BENCHMARK(BM_sequencial_access_1_seq_skip_8_const)->Arg(80000);
BENCHMARK(BM_sequencial_access_1_seq_skip_1)->Arg(80000);
BENCHMARK(BM_sequencial_access_1_seq_skip_2)->Arg(80000);
BENCHMARK(BM_sequencial_access_1_seq_skip_3)->Arg(80000);
BENCHMARK(BM_sequencial_access_1_seq_skip_4)->Arg(80000);
BENCHMARK(BM_sequencial_access_1_seq_skip_5)->Arg(80000);
BENCHMARK(BM_sequencial_access_1_seq_skip_6)->Arg(80000);
BENCHMARK(BM_sequencial_access_1_seq_skip_7)->Arg(80000);
BENCHMARK(BM_sequencial_access_1_seq_skip_8)->Arg(80000);

static void BM_sequencial_access_serial(benchmark::State& state) {
  std::size_t size = state.range(0);
  int sum          = 0;
  int pow_n        = 10000;
  uint8_t* src1    = new uint8_t[size];
  uint8_t* src2    = new uint8_t[size];
  uint8_t* src3    = new uint8_t[size];
  while (state.KeepRunning()) {
    for (int i = 0; i < size; i++) {
      sum += std::pow(src1[i], pow_n);
    }
    for (int i = 0; i < size; i++) {
      sum += std::pow(src2[i], pow_n);
    }
    for (int i = 0; i < size; i++) {
      sum += std::pow(src3[i], pow_n);
    }
  }
  delete[] src1;
  delete[] src2;
  delete[] src3;

  volatile int dummy = sum;
  dummy;
}
static void BM_sequencial_access_parallel(benchmark::State& state) {
  std::size_t size = state.range(0);
  int sum          = 0;
  int pow_n        = 10000;
  uint8_t* src1    = new uint8_t[size];
  uint8_t* src2    = new uint8_t[size];
  uint8_t* src3    = new uint8_t[size];
  while (state.KeepRunning()) {
    for (int i = 0; i < size; i++) {
      sum += std::pow(src1[i], pow_n);
      sum += std::pow(src2[i], pow_n);
      sum += std::pow(src3[i], pow_n);
    }
  }
  delete[] src1;
  delete[] src2;
  delete[] src3;

  volatile int dummy = sum;
  dummy;
}

BENCHMARK(BM_sequencial_access_serial)->Arg(8000)->Arg(16000)->Arg(80000);
BENCHMARK(BM_sequencial_access_parallel)->Arg(8000)->Arg(16000)->Arg(80000);

// NOTE: readよりもwriteのほうが高速
// NOTE: readでsumとメモリコピーとでは，メモリコピーのほうが高速(writeを含んでいるが)
static void BM_sequencial_access_read(benchmark::State& state) {
  std::size_t size = state.range(0);
  int sum          = 0;
  uint8_t* src     = new uint8_t[size];
  while (state.KeepRunning()) {
    for (int i = 0; i < size; i++) {
      sum += src[i];
    }
  }
  delete[] src;

  volatile int dummy = sum;
  dummy;
}
BENCHMARK(BM_sequencial_access_read)->Arg(1024);
BENCHMARK(BM_sequencial_access_read)->Arg(1024 * 1024);

static void BM_sequencial_access_write(benchmark::State& state) {
  std::size_t size = state.range(0);
  int val          = size;
  uint8_t* dst     = new uint8_t[size];
  while (state.KeepRunning()) {
    for (int i = 0; i < size; i++) {
      dst[i] = val;
    }
  }

  volatile int dummy = dst[0];
  dummy;

  delete[] dst;
}
BENCHMARK(BM_sequencial_access_write)->Arg(1024);
BENCHMARK(BM_sequencial_access_write)->Arg(1024 * 1024);

void BM_sequencial_access_Util(benchmark::State& state, std::size_t size) {
  const int loop_num = 1;
  std::size_t n      = 0;

  uint8_t* src = new uint8_t[size];
  uint8_t* dst = new uint8_t[size];
  std::chrono::system_clock::time_point benchmark_start =
      std::chrono::system_clock::now();
  while (state.KeepRunning()) {
    for (int i = 0; i < loop_num; i++) {
      for (int index = 0; index < size / (int)sizeof(std::size_t); index++) {
        ((std::size_t*)dst)[index] = ((std::size_t*)src)[index];
      }
      // for (int index = 0; index < size / (int)sizeof(uint8_t); index++) {
      // ((uint8_t*)dst)[index] = ((uint8_t*)src)[index];
      // }
    }
    // std::memcpy(dst, src, size);
    n++;
  }
  double benchmark_elapsed =
      std::chrono::duration_cast<std::chrono::nanoseconds>(
          std::chrono::system_clock::now() - benchmark_start)
          .count();

  delete[] src;
  delete[] dst;
  state.counters["N"]        = n;
  state.counters["total[s]"] = (benchmark_elapsed / 1000.0 / 1000.0 / 1000.0);
  // NOTE: 自動表示の単位が1024ではなく，1000なのでは?
  state.counters["B/sec"] =
      (double)size * n / (benchmark_elapsed / 1000.0 / 1000.0 / 1000.0);
  // NOTE: for avoiding optimization
  state.counters["dummy"] = n + dst[size - 1];

  state.SetBytesProcessed((double)size * n);
  state.SetItemsProcessed(n);
}

static void BM_sequencial_access_B(benchmark::State& state) {
  const int size_b = (1 << 0) * state.range(0);
  BM_sequencial_access_Util(state, size_b);
}

static void BM_sequencial_access_KB(benchmark::State& state) {
  const int size_kb = (1 << 10) * state.range(0);
  BM_sequencial_access_Util(state, size_kb);
}

static void BM_sequencial_access_MB(benchmark::State& state) {
  const int size_mb = (1 << 20) * state.range(0);
  BM_sequencial_access_Util(state, size_mb);
}

// NOTE: 1B~512B
BENCHMARK(BM_sequencial_access_B)
    // ->Arg(1)
    // ->Arg(2)
    // ->Arg(4)
    ->Arg(8)
    ->Arg(16)
    ->Arg(32)
    ->Arg(64)
    ->Arg(128)
    ->Arg(256)
    ->Arg(512);
// NOTE: 1KB~512KB
BENCHMARK(BM_sequencial_access_KB)
    ->Arg(1)
    ->Arg(2)
    ->Arg(4)
    ->Arg(8)
    ->Arg(16)
    ->Arg(32)
    ->Arg(64)
    ->Arg(128)
    ->Arg(256)
    ->Arg(512);
// NOTE: 1MB~64MB
BENCHMARK(BM_sequencial_access_MB)
    ->Unit(benchmark::kMicrosecond)
    ->Arg(1)
    ->Arg(2)
    ->Arg(4)
    ->Arg(8)
    ->Arg(16)
    ->Arg(32)
    ->Arg(64);

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  int gtest_ret = RUN_ALL_TESTS();

  // FYI: [c\+\+ \- Google benchmark custom main \- Stack Overflow]( https://stackoverflow.com/questions/34401965/google-benchmark-custom-main )
  ::benchmark::Initialize(&argc, argv);
  ::benchmark::RunSpecifiedBenchmarks();
  return gtest_ret;
}
