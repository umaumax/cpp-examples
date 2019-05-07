#include <vector>

#include <benchmark/benchmark.h>
#include <gtest/gtest.h>

static void BM_vector(benchmark::State& state) {
  const int size = state.range(0) * 1000;
  for (auto _ : state) {
    std::vector<int> vec;
    for (int i = 0; i < size; i++) {
      vec.emplace_back(0);
    }
  }
}
BENCHMARK(BM_vector)
    ->Arg(32)
    ->Arg(64)
    ->Arg(128)
    ->Arg(256)
    ->Arg(512)
    ->Arg(1024)
    ->Arg(2048)
    ->Arg(4096)
    ->Arg(8192);

static void BM_vector_reuse(benchmark::State& state) {
  const int size = state.range(0) * 1000;
  std::vector<int> vec;
  for (auto _ : state) {
    for (int i = 0; i < size; i++) {
      vec.emplace_back(0);
    }
    vec.clear();
    vec.reserve(size);
  }
}
BENCHMARK(BM_vector_reuse)
    ->Arg(32)
    ->Arg(64)
    ->Arg(128)
    ->Arg(256)
    ->Arg(512)
    ->Arg(1024)
    ->Arg(2048)
    ->Arg(4096)
    ->Arg(8192);

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  int gtest_ret = RUN_ALL_TESTS();

  // FYI: [c\+\+ \- Google benchmark custom main \- Stack Overflow]( https://stackoverflow.com/questions/34401965/google-benchmark-custom-main )
  ::benchmark::Initialize(&argc, argv);
  ::benchmark::RunSpecifiedBenchmarks();
  return gtest_ret;
}
