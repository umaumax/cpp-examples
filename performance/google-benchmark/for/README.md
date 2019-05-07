# vector

## how to build
```
g++ -std=c++11 -O3 main.cpp -Wall -lbenchmark -lgtest
```

## NOTE
* for loopの回数が小さいときには，ifのcontinueの判定処理をひとまとめしない方がよいように思える
  * 理由は不明
  * そもそも，これは正しいのかの考察が必要

### result

```
[==========] Running 0 tests from 0 test suites.
[==========] 0 tests from 0 test suites ran. (0 ms total)
[  PASSED  ] 0 tests.
2019-05-07 23:46:44
Running ./a.out
Run on (4 X 1700 MHz CPU s)
CPU Caches:
  L1 Data 32K (x2)
  L1 Instruction 32K (x2)
  L2 Unified 262K (x2)
  L3 Unified 4194K (x1)
----------------------------------------------------------
Benchmark                   Time           CPU Iterations
----------------------------------------------------------
BM_for_base/32             22 ns         21 ns   32040279
BM_for_base/64             41 ns         39 ns   19225065
BM_for_base/128            81 ns         71 ns   10047078
BM_for_base/256           127 ns        117 ns    5713586
BM_for_base/512           243 ns        225 ns    3056702
BM_for_base/1024          461 ns        438 ns    1528121
BM_for_base/2048          933 ns        870 ns     789640
BM_for_base/4096         1806 ns       1712 ns     387719
BM_for_base/8192         4105 ns       3720 ns     200931
BM_for_concat/32           35 ns         31 ns   22489093
BM_for_concat/64           46 ns         43 ns   15233585
BM_for_concat/128          88 ns         76 ns   10249352
BM_for_concat/256         138 ns        128 ns    5372960
BM_for_concat/512         323 ns        264 ns    2679898
BM_for_concat/1024        556 ns        486 ns    1289633
BM_for_concat/2048        971 ns        888 ns     783611
BM_for_concat/4096       1754 ns       1727 ns     394031
BM_for_concat/8192       3509 ns       3460 ns     202454
```

