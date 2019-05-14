# arm neon

## how to compile
```
g++ -std=c++11 -O3 main.cpp -lgtest -lbenchmark -mfpu=neon -march=native
g++ -std=c++11 -O3 test_main.cpp -lgtest -lbenchmark -mfpu=neon -march=native

g++ -std=c++11 -O3 image_alignment_main.cpp -lbenchmark -lpthread -lgtest -mfpu=neon -march=native -Wno-psabi
g++ -std=c++11 -O3 image_edge_detection_main.cpp -lbenchmark -lpthread -lgtest -mfpu=neon -march=native -Wno-psabi
```

## vget_low, vget_high
* 128bitから64bitを取り出す

```
|    128 bit    |
| 64bit | 64bit |
| low   | high  |
```

## vcombine
* 64bitx2を128bitへ結合する

## vdup_n
* laneの各要素に同一の値を詰める

## vrshrn
* 右シフト演算を行う
* WARN
  * 演算結果は四捨五入される

## result
### arm
neon用に16Byte alignmentする必要がある(for speedup)

```
# ./a.out
[==========] Running 2 tests from 2 test cases.
[----------] Global test environment set-up.
[----------] 1 test from mla_test
[ RUN      ] mla_test.std_and_neon
[       OK ] mla_test.std_and_neon (0 ms)
[----------] 1 test from mla_test (0 ms total)

[----------] 1 test from add_test
[ RUN      ] add_test.std_and_neon
[       OK ] add_test.std_and_neon (0 ms)
[----------] 1 test from add_test (0 ms total)

[----------] Global test environment tear-down
[==========] 2 tests from 2 test cases ran. (1 ms total)
[  PASSED  ] 2 tests.
2021-01-31 02:35:49
Running ./a.out
Run on (4 X 2989.26 MHz CPU s)
Load Average: 0.27, 0.32, 0.30
-------------------------------------------------------------------------
Benchmark                               Time             CPU   Iterations
-------------------------------------------------------------------------
BM_add_std/1                       117603 ns       117397 ns         5948
BM_add_std/2                       117570 ns       117391 ns         5956
BM_add_std/4                       117577 ns       117395 ns         5953
BM_add_std/8                       117595 ns       117393 ns         5957
BM_add_std/16                      176594 ns       176028 ns         4879
BM_add_std/32                      117841 ns       117607 ns         5937
BM_add_std/64                      117689 ns       117546 ns         5945
BM_add_std4/1                      263097 ns       262738 ns         2665
BM_add_std4/2                      263499 ns       262854 ns         2664
BM_add_std4/4                      263058 ns       262721 ns         2664
BM_add_std4/8                      263362 ns       262793 ns         2664
BM_add_std4/16                     117792 ns       117599 ns         5946
BM_add_std4/32                     117788 ns       117612 ns         5945
BM_add_std4/64                     117695 ns       117512 ns         5948
BM_add_neon/1                      300669 ns       300055 ns         2330
BM_add_neon/2                      300579 ns       299998 ns         2330
BM_add_neon/4                      300346 ns       299902 ns         2330
BM_add_neon/8                      300221 ns       299724 ns         2327
BM_add_neon/16                      93974 ns        93794 ns         7457
BM_add_neon/32                      93925 ns        93779 ns         7454
BM_add_neon/64                      93815 ns        93707 ns         7459
BM_add_std_double/1                121735 ns       121559 ns         5747
BM_add_std_double/2                121726 ns       121550 ns         5744
BM_add_std_double/4                121752 ns       121556 ns         5741
BM_add_std_double/8                121847 ns       121584 ns         5742
BM_add_std_double/16               121909 ns       121701 ns         5738
BM_add_std_double/32               121974 ns       121808 ns         5730
BM_add_std_double/64               122154 ns       121828 ns         5730
BM_mla_std/1                       530098 ns       528826 ns         1319
BM_mla_std/2                       532423 ns       530881 ns         1322
BM_mla_std/4                       530452 ns       529078 ns         1319
BM_mla_std/8                       529183 ns       527915 ns         1322
BM_mla_std/16                      158219 ns       157944 ns         4431
BM_mla_std/32                      158199 ns       157988 ns         4429
BM_mla_std/64                      158339 ns       158099 ns         4420
BM_mla_std4/1                      508862 ns       508351 ns         1369
BM_mla_std4/2                      509213 ns       508945 ns         1376
BM_mla_std4/4                      508886 ns       508620 ns         1359
BM_mla_std4/8                      509119 ns       508897 ns         1358
BM_mla_std4/16                     154779 ns       154730 ns         4522
BM_mla_std4/32                     154627 ns       154616 ns         4527
BM_mla_std4/64                     154626 ns       154610 ns         4521
BM_mla_neon_posix_mem_align/1  ERROR OCCURRED: 'memory allocate error'
BM_mla_neon_posix_mem_align/2  ERROR OCCURRED: 'memory allocate error'
BM_mla_neon_posix_mem_align/4      464640 ns       464426 ns         1508
BM_mla_neon_posix_mem_align/8      464590 ns       464394 ns         1507
BM_mla_neon_posix_mem_align/16     140510 ns       140466 ns         4979
BM_mla_neon_posix_mem_align/32     140486 ns       140475 ns         4983
BM_mla_neon_posix_mem_align/64     140382 ns       140362 ns         4979
BM_mla_neon_aligned_alloc/1        452750 ns       452534 ns         1545
BM_mla_neon_aligned_alloc/2        452237 ns       452022 ns         1544
BM_mla_neon_aligned_alloc/4        452656 ns       452430 ns         1546
BM_mla_neon_aligned_alloc/8        452464 ns       452236 ns         1548
BM_mla_neon_aligned_alloc/16       140445 ns       140379 ns         4985
BM_mla_neon_aligned_alloc/32       140435 ns       140383 ns         4986
BM_mla_neon_aligned_alloc/64       140386 ns       140340 ns         4981
BM_mla_std_double/1                188749 ns       188689 ns         3703
BM_mla_std_double/2                188783 ns       188713 ns         3705
BM_mla_std_double/4                188710 ns       188694 ns         3704
BM_mla_std_double/8                188710 ns       188694 ns         3703
BM_mla_std_double/16               189285 ns       189259 ns         3692
BM_mla_std_double/32               189284 ns       189197 ns         3694
BM_mla_std_double/64               189293 ns       189209 ns         3696
```

### raspberry pi

image_alignment_main.cpp

```
$ ./a.out
[==========] Running 0 tests from 0 test suites.
[==========] 0 tests from 0 test suites ran. (1 ms total)
[  PASSED  ] 0 tests.
2019-05-14 15:34:55
Running ./a.out
Run on (4 X 1200 MHz CPU s)
Load Average: 0.17, 0.21, 0.16
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
-----------------------------------------------------------------------------
Benchmark                                   Time             CPU   Iterations
-----------------------------------------------------------------------------
BM_image_vertical_access/48/0           12173 ns        12161 ns        57585
BM_image_vertical_access/48/2           11840 ns        11834 ns        59159
BM_image_vertical_access/50/0           11470 ns        11469 ns        60994
BM_image_vertical_access/50/2           10201 ns        10201 ns        68621
BM_image_vertical_access/50/4           11514 ns        11513 ns        60800
BM_image_vertical_access/50/6           10065 ns        10047 ns        69703
BM_image_vertical_access/64/0           18537 ns        18528 ns        37772
BM_image_vertical_access/64/2           18861 ns        18853 ns        37091
BM_image_vertical_access/70/0           20448 ns        20428 ns        34312
BM_image_vertical_access/70/2           19885 ns        19884 ns        35178
BM_image_vertical_access_neon/48/0       9443 ns         9442 ns        74137
BM_image_vertical_access_neon/48/2       8256 ns         8255 ns        84828
BM_image_vertical_access_neon/50/0       7064 ns         7063 ns        99102
BM_image_vertical_access_neon/50/2       6942 ns         6942 ns       100849
BM_image_vertical_access_neon/50/4       7020 ns         7017 ns        99782
BM_image_vertical_access_neon/50/6       6955 ns         6951 ns       100755
BM_image_vertical_access_neon/64/0      16156 ns        16154 ns        43339
BM_image_vertical_access_neon/64/2      13851 ns        13850 ns        50523
BM_image_vertical_access_neon/70/0      15673 ns        15665 ns        44725
BM_image_vertical_access_neon/70/2      18546 ns        18532 ns        37754
$ ./a.out
[==========] Running 1 test from 1 test suite.
[----------] Global test environment set-up.
[----------] 1 test from std_and_neon_test
[ RUN      ] std_and_neon_test.image_edge_access
[       OK ] std_and_neon_test.image_edge_access (1 ms)
[----------] 1 test from std_and_neon_test (1 ms total)

[----------] Global test environment tear-down
[==========] 1 test from 1 test suite ran. (1 ms total)
[  PASSED  ] 1 test.
2019-05-14 15:32:16
Running ./a.out
Run on (4 X 1200 MHz CPU s)
Load Average: 0.38, 0.29, 0.17
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------
Benchmark                              Time             CPU   Iterations
------------------------------------------------------------------------
BM_image_edge_access/480          703607 ns       703539 ns          995
BM_image_edge_access/960         2864188 ns      2863903 ns          243
BM_image_edge_access_neon/480     471831 ns       471747 ns         1481
BM_image_edge_access_neon/960    1879477 ns      1879295 ns          369
```

## FYI
* [google/benchmark: A microbenchmark support library]( https://github.com/google/benchmark#passing-arguments )
`Args`を利用すると，下記の`note`が出現するが，`-Wno-psabi`をコンパイルオプションに付加すると一時的に消去可能

```
note: parameter passing for argument of type 'xxx' will change in GCC 7.1
```

* [c\+\+ \- What does the the gcc warning "project parameter passing for X changed in GCC 7\.1" mean? \- Stack Overflow]( https://stackoverflow.com/questions/48149323/what-does-the-the-gcc-warning-project-parameter-passing-for-x-changed-in-gcc-7 )

----

* [ARM NEONの使い方 加算編 \- おぺんcv]( http://atkg.hatenablog.com/entry/2016/10/09/173928 )
* [ARM NEONの使い方 減算編 \- おぺんcv]( http://atkg.hatenablog.com/entry/2016/10/16/180214 )
* [ARM NEONの使い方 乗算編 \- おぺんcv]( http://atkg.hatenablog.com/entry/2016/11/06/201835 )
