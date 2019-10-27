#include <bitset>

#include "benchmark/benchmark.h"

unsigned int hammingDistance1(unsigned int x, unsigned int y) {
  unsigned int z = x ^ y;
  unsigned int r = 0;
  for (; z > 0; z >>= 1) {
    r += z & 1;
  }
  return r;
}
unsigned int hammingDistance2(unsigned int x, unsigned int y) {
  return std::bitset<32>(x ^ y).count();
}
unsigned int hammingDistance3(unsigned int x, unsigned int y) {
  return __builtin_popcount(x ^ y);
}
unsigned int hammingDistance4(unsigned int x, unsigned int y) {
  unsigned int i = x ^ y;
  i              = i - ((i >> 1) & 0x55555555);
  i              = (i & 0x33333333) + ((i >> 2) & 0x33333333);
  i              = (i + (i >> 4)) & 0x0f0f0f0f;
  i              = i + (i >> 8);
  i              = i + (i >> 16);
  return i & 0x3f;
}

static void BM_LongTest1(benchmark::State& state) {
  double tracker = 0.0;
  while (state.KeepRunning())
    hammingDistance1(state.range(), 2 * state.range());
  //     for (int i = 0; i < state.range(); ++i) tracker += i;
}
BENCHMARK(BM_LongTest1)->Range(1 << 16, 1 << 17);

static void BM_LongTest2(benchmark::State& state) {
  double tracker = 0.0;
  while (state.KeepRunning())
    hammingDistance2(state.range(), 2 * state.range());
  //     for (int i = 0; i < state.range(); ++i) tracker += i;
}
BENCHMARK(BM_LongTest2)->Range(1 << 16, 1 << 17);

static void BM_LongTest3(benchmark::State& state) {
  double tracker = 0.0;
  while (state.KeepRunning())
    hammingDistance3(state.range(), 2 * state.range());
  //     for (int i = 0; i < state.range(); ++i) tracker += i;
}
BENCHMARK(BM_LongTest3)->Range(1 << 16, 1 << 17);

static void BM_LongTest4(benchmark::State& state) {
  double tracker = 0.0;
  while (state.KeepRunning())
    hammingDistance4(state.range(), 2 * state.range());
  //     for (int i = 0; i < state.range(); ++i) tracker += i;
}
BENCHMARK(BM_LongTest4)->Range(1 << 16, 1 << 17);

int main(int argc, char** argv) {
  benchmark::Initialize(&argc, argv);

  benchmark::RunSpecifiedBenchmarks();
  return EXIT_SUCCESS;
}
