#include <vector>

#include <benchmark/benchmark.h>
#include <gtest/gtest.h>

static void BM_for_base(benchmark::State& state) {
  const int size = state.range(0) * 10;
  std::vector<int> vec;
  for (int i = 0; i < size; i++) {
    vec.emplace_back((i * i) % 10);
  }
  const int th = 3;
  std::vector<int> dst(size);
  int dummy = 0;
  for (auto _ : state) {
    for (int i = 2; i < size - 2; i++) {
      if (vec[i] < th) {
        continue;
      }
      if (vec[i] < vec[i - 2]) {
        continue;
      }
      if (vec[i] < vec[i - 1]) {
        continue;
      }
      if (vec[i] < vec[i + 1]) {
        continue;
      }
      if (vec[i] < vec[i + 2]) {
        continue;
      }
      // dst[i] = i;
      dummy++;
    }
  }
}
BENCHMARK(BM_for_base)
    ->Arg(32)
    ->Arg(64)
    ->Arg(128)
    ->Arg(256)
    ->Arg(512)
    ->Arg(1024)
    ->Arg(2048)
    ->Arg(4096)
    ->Arg(8192);

static void BM_for_concat(benchmark::State& state) {
  const int size = state.range(0) * 10;
  std::vector<int> vec;
  for (int i = 0; i < size; i++) {
    vec.emplace_back((i * i) % 10);
  }
  const int th = 3;
  std::vector<int> dst(size);
  int dummy = 0;
  for (auto _ : state) {
    for (int i = 2; i < size - 2; i++) {
      if (vec[i] < th || vec[i] < vec[i - 2] || vec[i] < vec[i - 1] ||
          vec[i] < vec[i + 1] || vec[i] < vec[i + 2]) {
        continue;
      }
      // dst[i] = i;
      dummy++;
    }
  }
}
BENCHMARK(BM_for_concat)
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
