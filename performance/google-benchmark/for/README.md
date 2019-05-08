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
#### x86
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

#### arm
```
[==========] Running 0 tests from 0 test cases.
[==========] 0 tests from 0 test cases ran. (1 ms total)
[  PASSED  ] 0 tests.
2021-01-31 01:18:26
Running ./a.out
Run on (4 X 2989.26 MHz CPU s)
Load Average: 0.00, 0.02, 0.00
-------------------------------------------------------------
Benchmark                   Time             CPU   Iterations
-------------------------------------------------------------
BM_for_base/32           1675 ns         1674 ns       401316
BM_for_base/64           3379 ns         3379 ns       211259
BM_for_base/128          6462 ns         6460 ns       101044
BM_for_base/256         13036 ns        13033 ns        53451
BM_for_base/512         26282 ns        26276 ns        26688
BM_for_base/1024        70852 ns        70831 ns        10195
BM_for_base/2048       189013 ns       188965 ns         3715
BM_for_base/4096       395204 ns       395083 ns         1783
BM_for_base/8192       793570 ns       793202 ns          895
BM_for_concat/32         1789 ns         1788 ns       391363
BM_for_concat/64         3583 ns         3583 ns       195372
BM_for_concat/128        7174 ns         7172 ns        97525
BM_for_concat/256       14350 ns        14349 ns        48761
BM_for_concat/512       28714 ns        28713 ns        24378
BM_for_concat/1024      70381 ns        70382 ns         9933
BM_for_concat/2048     175799 ns       175784 ns         3976
BM_for_concat/4096     362569 ns       362571 ns         1928
BM_for_concat/8192     732407 ns       732304 ns          937
```
