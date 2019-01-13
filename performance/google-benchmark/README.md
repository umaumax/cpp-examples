# google-benchmark

[google/benchmark: A microbenchmark support library]( https://github.com/google/benchmark )

```
#include <benchmark/benchmark.h>

static void BM_StringCreation(benchmark::State& state) {
  for (auto _ : state) std::string empty_string;
}
// Register the function as a benchmark
BENCHMARK(BM_StringCreation);

// Define another benchmark
static void BM_StringCopy(benchmark::State& state) {
  std::string x = "hello";
  for (auto _ : state) std::string copy(x);
}
BENCHMARK(BM_StringCopy);

BENCHMARK_MAIN();
```

```
$ g++ -std=c++11 -lbenchmark main.cpp
$ ./a.out
2019-01-13 17:27:19
Running ./a.out
Run on (4 X 1700 MHz CPU s)
CPU Caches:
  L1 Data 32K (x2)
  L1 Instruction 32K (x2)
  L2 Unified 262K (x2)
  L3 Unified 4194K (x1)
---------------------------------------------------------
Benchmark                  Time           CPU Iterations
---------------------------------------------------------
BM_StringCreation         23 ns         22 ns   32453082
BM_StringCopy             11 ns         11 ns   62763945
```
