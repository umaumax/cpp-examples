#include <benchmark/benchmark.h>

void array_access_by_count_up(int *sum, int *left, int *right, int count) {
  for (int i = 0; i < count; ++i) {
    sum[i] = left[i] + right[i];
  }
}

void pointer_access_by_count_up(int *sum, int *left, int *right, int count) {
  for (int i = 0; i < count; ++i) {
    *sum = *left + *right;
    ++right;
    ++left;
    ++sum;
  }
}
void pointer_access_by_count_down(int *sum, int *left, int *right, int count) {
  for (; count > 0; --count) {
    *sum = *left + *right;
    ++right;
    ++left;
    ++sum;
  }
}

static void BM_array_access_by_count_up(benchmark::State &state) {
  constexpr int n = 10000;
  int sum[n];
  int left[n];
  int right[n];
  int count = n;
  for (auto _ : state) {
    array_access_by_count_up(sum, left, right, count);
  }
  volatile int ret = sum[0];
  (void)ret;
}
BENCHMARK(BM_array_access_by_count_up);

static void BM_pointer_access_by_count_up(benchmark::State &state) {
  constexpr int n = 10000;
  int sum[n];
  int left[n];
  int right[n];
  int count = n;
  for (auto _ : state) {
    pointer_access_by_count_up(sum, left, right, count);
  }
  volatile int ret = sum[0];
  (void)ret;
}
BENCHMARK(BM_pointer_access_by_count_up);

static void BM_pointer_access_by_count_down(benchmark::State &state) {
  constexpr int n = 10000;
  int sum[n];
  int left[n];
  int right[n];
  int count = n;
  for (auto _ : state) {
    pointer_access_by_count_down(sum, left, right, count);
  }
  volatile int ret = sum[0];
  (void)ret;
}
BENCHMARK(BM_pointer_access_by_count_down);

BENCHMARK_MAIN();
