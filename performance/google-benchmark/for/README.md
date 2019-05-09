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

### x86 full result
```
[==========] Running 0 tests from 0 test suites.
[==========] 0 tests from 0 test suites ran. (1 ms total)
[  PASSED  ] 0 tests.
2019-05-09 22:35:55
Running ./a.out
Run on (4 X 1700 MHz CPU s)
CPU Caches:
  L1 Data 32K (x2)
  L1 Instruction 32K (x2)
  L2 Unified 262K (x2)
  L3 Unified 4194K (x1)
-----------------------------------------------------------------------
Benchmark                                Time           CPU Iterations
-----------------------------------------------------------------------
BM_for_base/32                         437 ns        427 ns    1646900
BM_for_base/64                         870 ns        849 ns     828314
BM_for_base/128                       1798 ns       1671 ns     422435
BM_for_base/256                       3453 ns       3373 ns     205193
BM_for_base/512                       7089 ns       6868 ns     102712
BM_for_base/1024                     14144 ns      13737 ns      49486
BM_for_base/2048                     28050 ns      26589 ns      25777
BM_for_base/4096                     55411 ns      54265 ns      13308
BM_for_base/8192                    108410 ns     106109 ns       6441
BM_for_concat/32                       455 ns        426 ns    1639041
BM_for_concat/64                       860 ns        844 ns     816574
BM_for_concat/128                     1714 ns       1682 ns     411636
BM_for_concat/256                     3400 ns       3349 ns     208084
BM_for_concat/512                     7352 ns       7141 ns      95503
BM_for_concat/1024                   14265 ns      13882 ns      50802
BM_for_concat/2048                   28133 ns      26383 ns      26138
BM_for_concat/4096                   55928 ns      53467 ns      12958
BM_for_concat/8192                  113447 ns     105798 ns       6278
BM_for_horizontal_access/32            144 ns        131 ns    5328137
BM_for_horizontal_access/64            409 ns        398 ns    1779495
BM_for_horizontal_access/128          1226 ns       1170 ns     577672
BM_for_horizontal_access/256          4365 ns       4011 ns     169852
BM_for_horizontal_access/512         20191 ns      18393 ns      39217
BM_for_horizontal_access/1024        61839 ns      56428 ns      13159
BM_for_horizontal_access/2048       301393 ns     250092 ns       3432
BM_for_horizontal_access/4096       875249 ns     807266 ns        783
BM_for_horizontal_access/8192      3658447 ns    3234885 ns        234
BM_for_horizontal_access/16384    12528527 ns   12096068 ns         59
BM_for_horizontal2_access/32           123 ns        112 ns    6397660
BM_for_horizontal2_access/64           291 ns        265 ns    2686480
BM_for_horizontal2_access/128          903 ns        884 ns     821548
BM_for_horizontal2_access/256         3403 ns       2917 ns     253033
BM_for_horizontal2_access/512        11234 ns      10251 ns      69907
BM_for_horizontal2_access/1024       41929 ns      38948 ns      17998
BM_for_horizontal2_access/2048      132856 ns     123295 ns       5227
BM_for_horizontal2_access/4096      453336 ns     414844 ns       1600
BM_for_horizontal2_access/8192     1730195 ns    1568937 ns        463
BM_for_horizontal2_access/16384    6426424 ns    5933941 ns        119
BM_for_horizontal4_access/32           159 ns        148 ns    4699721
BM_for_horizontal4_access/64           298 ns        283 ns    2532818
BM_for_horizontal4_access/128          823 ns        741 ns     932326
BM_for_horizontal4_access/256         1848 ns       1758 ns     403428
BM_for_horizontal4_access/512         5020 ns       4911 ns     142791
BM_for_horizontal4_access/1024       16239 ns      15329 ns      45098
BM_for_horizontal4_access/2048       73891 ns      71533 ns      10301
BM_for_horizontal4_access/4096      236306 ns     229765 ns       3075
BM_for_horizontal4_access/8192      855346 ns     808236 ns        868
BM_for_horizontal4_access/16384    3448346 ns    3339032 ns        216
BM_for_vertical_access/32              157 ns        144 ns    4897399
BM_for_vertical_access/64              544 ns        509 ns    1378604
BM_for_vertical_access/128            1646 ns       1543 ns     457421
BM_for_vertical_access/256            8391 ns       7520 ns      94661
BM_for_vertical_access/512           21996 ns      20072 ns      35608
BM_for_vertical_access/1024          67383 ns      62122 ns      10790
BM_for_vertical_access/2048         234856 ns     215151 ns       3207
BM_for_vertical_access/4096         868365 ns     798304 ns        906
BM_for_vertical_access/8192        3092352 ns    2953060 ns        235
BM_for_vertical_access/16384      11959222 ns   11680016 ns         61
BM_for_vertical2_access/32             109 ns        101 ns    6855151
BM_for_vertical2_access/64             229 ns        222 ns    3360280
BM_for_vertical2_access/128            909 ns        851 ns     740984
BM_for_vertical2_access/256           2795 ns       2707 ns     247761
BM_for_vertical2_access/512           9350 ns       8997 ns      67178
BM_for_vertical2_access/1024         40277 ns      37511 ns      17758
BM_for_vertical2_access/2048        115412 ns     112951 ns       6071
BM_for_vertical2_access/4096        420639 ns     408664 ns       1697
BM_for_vertical2_access/8192       1620905 ns    1555858 ns        444
BM_for_vertical2_access/16384      6108104 ns    5897138 ns        123
```
