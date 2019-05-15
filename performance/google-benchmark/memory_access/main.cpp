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
