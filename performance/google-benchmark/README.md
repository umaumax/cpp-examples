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
$ g++ -std=c++11 main.cpp -lbenchmark -lpthread -lgtest
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

## how to build
### host machine build
* [google/benchmark: A microbenchmark support library]( https://github.com/google/benchmark )

### cross compile
set below 2 values
```
ROOT_PATH_OF_TOOLCHAIN=/opt/xxx
PATH_TO_CMAKE_TOOLCHAIN=xxx.cmake

function install_google_benchmark() {
  [[ -z $ROOT_PATH_OF_TOOLCHAIN ]] && echo "set ROOT_PATH_OF_TOOLCHAIN" && return 1
  [[ -z $PATH_TO_CMAKE_TOOLCHAIN ]] && echo "set PATH_TO_CMAKE_TOOLCHAIN" && return 1

  git clone https://github.com/google/benchmark.git
  pushd benchmark

  git clone https://github.com/google/googletest.git
  pushd googletest
  mkdir build
  pushd build
  mkdir local
  cmake .. -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_TOOLCHAIN_FILE=$PATH_TO_CMAKE_TOOLCHAIN -DCMAKE_INSTALL_PREFIX=./local
  make -j
  make install
  [[ ! -e $ROOT_PATH_OF_TOOLCHAIN/usr/include/gtest ]] && sudo cp -Rv local $ROOT_PATH_OF_TOOLCHAIN/usr/
  popd
  popd

  mkdir build
  pushd build
  cmake .. -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_TOOLCHAIN_FILE=$PATH_TO_CMAKE_TOOLCHAIN -DCMAKE_INSTALL_PREFIX=./local -DHAVE_POSIX_REGEX=0 -DHAVE_STD_REGEX=0 -DHAVE_STEADY_CLOCK=0
  make -j
  make install
  sudo cp -Rv local $ROOT_PATH_OF_TOOLCHAIN/usr/
  popd
  popd
}
install_google_benchmark
```

## NOTE
* [google/benchmark: A microbenchmark support library]( https://github.com/google/benchmark#runtime-and-reporting-considerations )
> The number of iterations to run is determined dynamically by running the benchmark a few times and measuring the time taken and ensuring that the ultimate result will be statistically stable.
  * `BENCHMARK`で指定した関数は何回か実行され，最終的な実行回数が決定される
    * これは，`std::cout`を挿入するとわかりやすい
    * 最終的な実行回数は`state.iterations()`or`state.max_iterations`で取得可能

* [google/benchmark: A microbenchmark support library]( https://github.com/google/benchmark#user-defined-counters )
  * 結果表示の場所にユーザ定義の変数の表示が可能だが，ベンチマーク結果の取得(e.g. 実行時間)はできないので注意

### 無理やり計測時間を取得する
```
std::chrono::system_clock::time_point benchmark_start = std::chrono::system_clock::now();
for (auto _ : state) {
  // do processing
}
double benchmark_elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now() - benchmark_start).count();
```

### 処理バイト数や処理数を秒単位で出力する
下記を設定することで，結果の出力時に，適切な単位で，補足情報が出力される
```
state.SetBytesProcessed(bytes);
state.SetItemsProcessed(items);
```

e.g.
```
-----------------------------------------------------------------------------------
Benchmark                            Time           CPU Iterations UserCounters...
-----------------------------------------------------------------------------------
BM_sequencial_access_B/8             3 ns          3 ns  237975441 2.79234GB/s    357.42M items/s
BM_sequencial_access_B/16            3 ns          3 ns  252944089 5.58779GB/s   357.619M items/s
```

### ベンチマークする関数の最低実行時間制御
```
BENCHMARK(BM_test)->MinTime(2.0); // Run for at least 2 seconds.
```

実行時引数
```
[--benchmark_min_time=<min_time>]
```

Q. 最大実行時間の制御は?
A. ない
無理やり止める方法はある

ベンチマークを一時停止している処理が長いときに，ベンチマーク自体の時間としては換算されないため，ベンチマークに非常に時間がかかってしまうため，この機能が欲しい

FYI:
* [Fine\-grained iteration count control · Issue \#370 · google/benchmark]( https://github.com/google/benchmark/issues/370 )
* [Support for benchmarking externally timed events, such as GPU code · Issue \#198 · google/benchmark]( https://github.com/google/benchmark/issues/198 )

### ベンチマーク全体の実行回数制御
```
[--benchmark_repetitions=<num_repetitions>]
```

### ベンチマークの一時停止
e.g. キャッシュラインをフラッシュさせたり，データをinitする場合など?
```
// You might have a microbenchmark that depends on two inputs.  For
// example, the following code defines a family of microbenchmarks for
// measuring the speed of set insertion.
static void BM_SetInsert(benchmark::State& state) {
  set<int> data;
  for (auto _ : state) {
    state.PauseTiming();
    data = ConstructRandomSet(state.range(0));
    state.ResumeTiming();
    for (int j = 0; j < state.range(1); ++j)
      data.insert(RandomNumber());
  }
}
```

[\`state\.PauseTiming\(\)\` and state\.ResumeTiming\(\) take a long time · Issue \#179 · google/benchmark]( https://github.com/google/benchmark/issues/179 )

e.g. 2000nsほど?
