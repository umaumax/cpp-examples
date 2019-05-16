# memory access benchmark

## how to build
```
g++ -std=c++11 -O3 main.cpp -Wall -lbenchmark -lgtest
```

## NOTE
* sequential access
  * stride
* random access
* matrix mul
  * blocking

* サイズが小さい容量が少ないときには，for loopの処理負荷の方が高い
  * 最適化で自動的にmemcpyになる?(要:確認)
  * memcpyはsystem call呼び出しなので，サイズが小さいときには遅くなる

```
for (auto _ : state) {}
```
と同じ意味
```
while (state.KeepRunning()) {}
```

* forで同様な処理をserialで実行するよりもparallelで実行すると良い?
  * 考える観点としては，内部処理時間が短い場合にはただ単に，forの条件判定の回数が少なくなるだけ?
  * `BM_sequencial_access_serial`
  * `BM_sequencial_access_parallel`

### 最適化防止
```
volatile int dummy = sum;
dummy;
```

### 画像の画素に対してのグリッドアクセス
* indexの`i`,`j`と`y_grid_size`,`x_grid_size`の型が異なると最適化が行われない or castコスト?
  * `int i`と`constexpr std::size_t x_grid_size = 1;`でさえNG
* `grid_size`が中間の値の場合には速度低下
* incrementするstride幅を変数にするのは速度低下の原因?
```
for (std::size_t j = 0; j < height; j += y_grid_size) {
  for (std::size_t i = 0; i < width; i += x_grid_size) {
    const std::size_t offset = width * j + i;
    for (std::size_t j = 0; j < y_grid_size; j++) {
      for (std::size_t i = 0; i < x_grid_size; i++) {
        sum += src[offset + width * j + i];
      }
    }
  }
}
```

```
[==========] Running 0 tests from 0 test suites.
[==========] 0 tests from 0 test suites ran. (0 ms total)
[  PASSED  ] 0 tests.
2019-05-16 23:26:47
Running ./a.out
Run on (4 X 1700 MHz CPU s)
CPU Caches:
  L1 Data 32K (x2)
  L1 Instruction 32K (x2)
  L2 Unified 262K (x2)
  L3 Unified 4194K (x1)
-------------------------------------------------------------------------------------
Benchmark                                              Time           CPU Iterations
-------------------------------------------------------------------------------------
BM_image_grid_access_1_const_int/1024             105080 ns     102547 ns       6600 int
BM_image_grid_access_2_const_int/1024             223866 ns     216923 ns       3030 int
BM_image_grid_access_4_const_int/1024             177360 ns     175257 ns       3959 int
BM_image_grid_access_8_const_int/1024             357815 ns     351727 ns       2008 int
BM_image_grid_access_16_const_int/1024            167682 ns     161131 ns       4250 int
BM_image_grid_access_32_const_int/1024            140904 ns     137336 ns       5068 int
BM_image_grid_access_64_const_int/1024            125793 ns     121639 ns       5200 int
BM_image_grid_access_128_const_int/1024           118158 ns     116680 ns       5679 int
BM_image_grid_access_256_const_int/1024           111970 ns     110729 ns       5958 int
BM_image_grid_access_512_const_int/1024           113811 ns     106192 ns       6295 int
BM_image_grid_access_1024_const_int/1024           99546 ns      98523 ns       6848 int
BM_image_grid_access_1_const_size_t/1024         1239914 ns    1228337 ns        537 size_t
BM_image_grid_access_2_const_size_t/1024          403658 ns     400330 ns       1717 size_t
BM_image_grid_access_4_const_size_t/1024          359461 ns     354224 ns       1941 size_t
BM_image_grid_access_8_const_size_t/1024          356402 ns     350315 ns       2006 size_t
BM_image_grid_access_16_const_size_t/1024         166124 ns     160402 ns       4358 size_t
BM_image_grid_access_32_const_size_t/1024         142428 ns     137811 ns       4769 size_t
BM_image_grid_access_64_const_size_t/1024         126608 ns     122764 ns       5233 size_t
BM_image_grid_access_128_const_size_t/1024        120614 ns     117249 ns       5596 size_t
BM_image_grid_access_256_const_size_t/1024        116688 ns     110124 ns       5910 size_t
BM_image_grid_access_512_const_size_t/1024        112566 ns     107608 ns       6504 size_t
BM_image_grid_access_1024_const_size_t/1024       103476 ns     100145 ns       6493 size_t
BM_image_grid_access_1/1024                      2544022 ns    2510381 ns        281
BM_image_grid_access_2/1024                      1263844 ns    1209124 ns        531
BM_image_grid_access_4/1024                       924629 ns     894376 ns        760
BM_image_grid_access_8/1024                       414333 ns     407045 ns       1673
BM_image_grid_access_16/1024                      281679 ns     266960 ns       2253
BM_image_grid_access_32/1024                      170252 ns     167300 ns       3977
BM_image_grid_access_64/1024                      149818 ns     144991 ns       4835
BM_image_grid_access_128/1024                     133566 ns     129776 ns       4823
BM_image_grid_access_256/1024                     134489 ns     127906 ns       5453
BM_image_grid_access_512/1024                     118523 ns     116992 ns       5703
BM_image_grid_access_1024/1024                    106215 ns     102686 ns       6461
```
