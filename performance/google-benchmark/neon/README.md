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
  * メモリアドレス的にlow,highな方向
  * little endianの場合には，直感と合うはず

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

## unsignedの型同士の減算

[ARM NEONの使い方 減算編 \- おぺんcv]( http://atkg.hatenablog.com/entry/2016/10/16/180214 )

「符号なしベクタの減算について」の項目を参照

### unsignedの型同士の減算(実は絶対値で十分な場合)
`abd`を利用すれば良い

## cast
[ARM Information Center]( http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0472jj/chr1359125040547_00003.html )
> 以下のコンパイラ組み込み関数は、4 つの符号付き 16 ビット整数を持つベクタを、4 つの符号無し整数を持つベクタに変換します。
> uint16x4_t vreinterpret_u16_s16(int16x4_t a);
> 以下のコンパイラ組み込み関数は、4 つの 32 ビット浮動小数点整数を持つベクタを、4 つの符号付き整数を持つベクタに変換します。
> int8x16_t vreinterpretq_s8_f32(float32x4_t a);
> これらの変換では、ベクタによって表現されるビットパターンは変わりません。

また，コンパイル時のフラグに`-flax-vector-conversions`を追加すると，
例えば，`uint16x8_t`から`int16x8_t`は自動変換され(前述の通りビットパターンに変化がない)，
エラーがなくなるが，可読性の観点から推奨しない
```
note: use -flax-vector-conversions to permit conversions between vectors with differing element types or numbers of subparts
```

## vextq
* 同一のレーンを指定すれば，ベクトル要素のローテーションが可能となる(変数ではなく定数値のみのローテーション)

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
```

* `+1`した箇所を境界として，アクセスを行うため，`8-1`,`16-1`が高速となる
  * しかし，srcは1Bであり，なぜ，`8`の場合になぜ，このような結果となるかが不明，`16`ならばまだわかるが
* width,heightと8の倍数以外にすると，dstとなる3個分のalignmentの2個目と3個目がずれてしまうため．半端なalignmentの方が高速に動作する計測結果となっている
* 通常の演算--2倍-->行ごとのNEON--2倍-->連続領域のNEON演算
  * 連続領域として演算する利点
    * 圧倒的にalignmentの調整が容易
    * 不定値に当たる箇所の余計な演算を含んでいたとしても，高速
  * vextやループアンローリングは多少の高速化に貢献する程度
```
BM_image_edge_access_seq_neon/64/7             9620 ns         9618 ns        72793
BM_image_edge_access_seq_neon/64/15            9613 ns         9613 ns        72823
BM_image_edge_access_seq_neon/64/31            9613 ns         9613 ns        72818
BM_image_edge_access_seq_neon/64/63            9616 ns         9616 ns        72791

BM_image_edge_access_seq_vext_neon/64/7        8889 ns         8889 ns        78755
BM_image_edge_access_seq_vext_neon/64/15       8890 ns         8890 ns        78731
BM_image_edge_access_seq_vext_neon/64/31       8888 ns         8888 ns        78760
BM_image_edge_access_seq_vext_neon/64/63       8895 ns         8894 ns        78708
```

```
$ ./a.out
[==========] Running 3 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 3 tests from std_and_neon_test
[ RUN      ] std_and_neon_test.image_edge_access_std_vs_neon
[       OK ] std_and_neon_test.image_edge_access_std_vs_neon (1 ms)
[ RUN      ] std_and_neon_test.image_edge_access_std_vs_seq_neon
[       OK ] std_and_neon_test.image_edge_access_std_vs_seq_neon (0 ms)
[ RUN      ] std_and_neon_test.image_edge_access_std_vs_seq_vext_neon
[       OK ] std_and_neon_test.image_edge_access_std_vs_seq_vext_neon (0 ms)
[----------] 3 tests from std_and_neon_test (4 ms total)

[----------] Global test environment tear-down
[==========] 3 tests from 1 test suite ran. (4 ms total)
[  PASSED  ] 3 tests.
2019-06-02 11:01:08
Running ./a.out
Run on (4 X 1200 MHz CPU s)
Load Average: 0.52, 0.54, 0.38
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
-----------------------------------------------------------------------------------
Benchmark                                         Time             CPU   Iterations
-----------------------------------------------------------------------------------
BM_image_edge_access_std/40/0                 20735 ns        20734 ns        33742
BM_image_edge_access_std/40/0                 20744 ns        20739 ns        33757
BM_image_edge_access_std/40/1                 20766 ns        20732 ns        33768
BM_image_edge_access_std/40/2                 20737 ns        20736 ns        33752
BM_image_edge_access_std/40/3                 20737 ns        20735 ns        33741
BM_image_edge_access_std/40/4                 20741 ns        20739 ns        33760
BM_image_edge_access_std/40/5                 20743 ns        20739 ns        33756
BM_image_edge_access_std/40/6                 20742 ns        20735 ns        33757
BM_image_edge_access_std/40/7                 20751 ns        20740 ns        33756
BM_image_edge_access_std/40/8                 20737 ns        20736 ns        33758
BM_image_edge_access_std/40/9                 20735 ns        20732 ns        33774
BM_image_edge_access_std/40/10                20735 ns        20734 ns        33758
BM_image_edge_access_std/40/11                20741 ns        20737 ns        33757
BM_image_edge_access_std/40/12                20738 ns        20736 ns        33740
BM_image_edge_access_std/40/13                20735 ns        20734 ns        33756
BM_image_edge_access_std/40/14                20742 ns        20738 ns        33758
BM_image_edge_access_std/40/15                20735 ns        20734 ns        33757
BM_image_edge_access_std/40/16                20744 ns        20740 ns        33759
BM_image_edge_access_std/40/31                20736 ns        20734 ns        33757
BM_image_edge_access_std/40/63                20741 ns        20737 ns        33753
BM_image_edge_access_neon/40/0                 9269 ns         9268 ns        75520
BM_image_edge_access_neon/40/0                 9269 ns         9268 ns        75521
BM_image_edge_access_neon/40/1                 9399 ns         9397 ns        74503
BM_image_edge_access_neon/40/2                 9398 ns         9396 ns        74489
BM_image_edge_access_neon/40/3                 9396 ns         9396 ns        74506
BM_image_edge_access_neon/40/4                 9397 ns         9396 ns        74488
BM_image_edge_access_neon/40/5                 9396 ns         9396 ns        74507
BM_image_edge_access_neon/40/6                 9271 ns         9270 ns        75527
BM_image_edge_access_neon/40/7                 8823 ns         8821 ns        79375
BM_image_edge_access_neon/40/8                 9269 ns         9268 ns        75522
BM_image_edge_access_neon/40/9                 9399 ns         9398 ns        74481
BM_image_edge_access_neon/40/10                9396 ns         9396 ns        74479
BM_image_edge_access_neon/40/11                9399 ns         9398 ns        74499
BM_image_edge_access_neon/40/12                9396 ns         9396 ns        74495
BM_image_edge_access_neon/40/13                9404 ns         9398 ns        74518
BM_image_edge_access_neon/40/14                9273 ns         9270 ns        75517
BM_image_edge_access_neon/40/15                8822 ns         8821 ns        79363
BM_image_edge_access_neon/40/16                9273 ns         9270 ns        75533
BM_image_edge_access_neon/40/31                8822 ns         8821 ns        79347
BM_image_edge_access_neon/40/63                8836 ns         8823 ns        79359
BM_image_edge_access_seq_neon/40/0             4359 ns         4358 ns       160572
BM_image_edge_access_seq_neon/40/0             4358 ns         4358 ns       160619
BM_image_edge_access_seq_neon/40/1             4518 ns         4518 ns       154905
BM_image_edge_access_seq_neon/40/2             4520 ns         4519 ns       154947
BM_image_edge_access_seq_neon/40/3             4518 ns         4517 ns       154967
BM_image_edge_access_seq_neon/40/4             4520 ns         4519 ns       154947
BM_image_edge_access_seq_neon/40/5             4518 ns         4518 ns       154947
BM_image_edge_access_seq_neon/40/6             4358 ns         4358 ns       160588
BM_image_edge_access_seq_neon/40/7             3720 ns         3719 ns       188206
BM_image_edge_access_seq_neon/40/8             4359 ns         4358 ns       160584
BM_image_edge_access_seq_neon/40/9             4519 ns         4518 ns       154949
BM_image_edge_access_seq_neon/40/10            4518 ns         4518 ns       154950
BM_image_edge_access_seq_neon/40/11            4519 ns         4519 ns       154954
BM_image_edge_access_seq_neon/40/12            4518 ns         4517 ns       154954
BM_image_edge_access_seq_neon/40/13            4518 ns         4518 ns       154903
BM_image_edge_access_seq_neon/40/14            4358 ns         4358 ns       160620
BM_image_edge_access_seq_neon/40/15            3719 ns         3719 ns       188147
BM_image_edge_access_seq_neon/40/16            4359 ns         4359 ns       160632
BM_image_edge_access_seq_neon/40/31            3719 ns         3719 ns       188198
BM_image_edge_access_seq_neon/40/63            3721 ns         3720 ns       188192
BM_image_edge_access_seq_vext_neon/40/0        4615 ns         4614 ns       151711
BM_image_edge_access_seq_vext_neon/40/0        4616 ns         4615 ns       151711
BM_image_edge_access_seq_vext_neon/40/1        4615 ns         4614 ns       151697
BM_image_edge_access_seq_vext_neon/40/2        4615 ns         4614 ns       151664
BM_image_edge_access_seq_vext_neon/40/3        4615 ns         4615 ns       151713
BM_image_edge_access_seq_vext_neon/40/4        4614 ns         4614 ns       151701
BM_image_edge_access_seq_vext_neon/40/5        4616 ns         4615 ns       151705
BM_image_edge_access_seq_vext_neon/40/6        4615 ns         4615 ns       151711
BM_image_edge_access_seq_vext_neon/40/7        3626 ns         3626 ns       193054
BM_image_edge_access_seq_vext_neon/40/8        4614 ns         4614 ns       151731
BM_image_edge_access_seq_vext_neon/40/9        4615 ns         4614 ns       151671
BM_image_edge_access_seq_vext_neon/40/10       4615 ns         4614 ns       151700
BM_image_edge_access_seq_vext_neon/40/11       4614 ns         4614 ns       151678
BM_image_edge_access_seq_vext_neon/40/12       4616 ns         4615 ns       151709
BM_image_edge_access_seq_vext_neon/40/13       4614 ns         4614 ns       151711
BM_image_edge_access_seq_vext_neon/40/14       4615 ns         4614 ns       151677
BM_image_edge_access_seq_vext_neon/40/15       3625 ns         3625 ns       193097
BM_image_edge_access_seq_vext_neon/40/16       4614 ns         4614 ns       151657
BM_image_edge_access_seq_vext_neon/40/31       3627 ns         3626 ns       193087
BM_image_edge_access_seq_vext_neon/40/63       3625 ns         3625 ns       193097
BM_image_edge_access_std/41/0                 21812 ns        21806 ns        32100
BM_image_edge_access_std/41/0                 21803 ns        21802 ns        32106
BM_image_edge_access_std/41/1                 21804 ns        21803 ns        32092
BM_image_edge_access_std/41/2                 21810 ns        21806 ns        32088
BM_image_edge_access_std/41/3                 21805 ns        21803 ns        32098
BM_image_edge_access_std/41/4                 21811 ns        21807 ns        32102
BM_image_edge_access_std/41/5                 21808 ns        21804 ns        32093
BM_image_edge_access_std/41/6                 21812 ns        21808 ns        32103
BM_image_edge_access_std/41/7                 21805 ns        21804 ns        32107
BM_image_edge_access_std/41/8                 21813 ns        21809 ns        32104
BM_image_edge_access_std/41/9                 21807 ns        21805 ns        32102
BM_image_edge_access_std/41/10                21815 ns        21810 ns        32104
BM_image_edge_access_std/41/11                21804 ns        21803 ns        32102
BM_image_edge_access_std/41/12                21809 ns        21805 ns        32094
BM_image_edge_access_std/41/13                21809 ns        21806 ns        32079
BM_image_edge_access_std/41/14                21805 ns        21804 ns        32102
BM_image_edge_access_std/41/15                21811 ns        21807 ns        32105
BM_image_edge_access_std/41/16                21805 ns        21803 ns        32106
BM_image_edge_access_std/41/31                21811 ns        21807 ns        32103
BM_image_edge_access_std/41/63                21804 ns        21803 ns        32101
BM_image_edge_access_neon/41/0                10054 ns        10052 ns        69770
BM_image_edge_access_neon/41/0                10054 ns        10053 ns        69608
BM_image_edge_access_neon/41/1                10042 ns        10041 ns        69747
BM_image_edge_access_neon/41/2                10044 ns        10042 ns        69767
BM_image_edge_access_neon/41/3                10044 ns        10044 ns        69657
BM_image_edge_access_neon/41/4                10037 ns        10036 ns        69652
BM_image_edge_access_neon/41/5                10048 ns        10046 ns        69665
BM_image_edge_access_neon/41/6                10053 ns        10052 ns        69699
BM_image_edge_access_neon/41/7                10056 ns        10054 ns        69596
BM_image_edge_access_neon/41/8                10065 ns        10064 ns        69641
BM_image_edge_access_neon/41/9                10044 ns        10043 ns        69615
BM_image_edge_access_neon/41/10               10050 ns        10047 ns        69754
BM_image_edge_access_neon/41/11               10041 ns        10041 ns        69791
BM_image_edge_access_neon/41/12               10042 ns        10039 ns        69809
BM_image_edge_access_neon/41/13               10044 ns        10042 ns        69776
BM_image_edge_access_neon/41/14               10044 ns        10043 ns        69750
BM_image_edge_access_neon/41/15               10054 ns        10052 ns        69797
BM_image_edge_access_neon/41/16               10057 ns        10055 ns        69714
BM_image_edge_access_neon/41/31               10058 ns        10055 ns        69744
BM_image_edge_access_neon/41/63               10055 ns        10054 ns        69626
BM_image_edge_access_seq_neon/41/0             4909 ns         4908 ns       142658
BM_image_edge_access_seq_neon/41/0             4908 ns         4907 ns       142657
BM_image_edge_access_seq_neon/41/1             4909 ns         4908 ns       142634
BM_image_edge_access_seq_neon/41/2             4907 ns         4907 ns       142653
BM_image_edge_access_seq_neon/41/3             4907 ns         4907 ns       142619
BM_image_edge_access_seq_neon/41/4             4743 ns         4742 ns       147663
BM_image_edge_access_seq_neon/41/5             4245 ns         4245 ns       164920
BM_image_edge_access_seq_neon/41/6             4588 ns         4587 ns       152607
BM_image_edge_access_seq_neon/41/7             4575 ns         4574 ns       153045
BM_image_edge_access_seq_neon/41/8             4908 ns         4907 ns       142621
BM_image_edge_access_seq_neon/41/9             4907 ns         4907 ns       142633
BM_image_edge_access_seq_neon/41/10            4909 ns         4907 ns       142622
BM_image_edge_access_seq_neon/41/11            4910 ns         4908 ns       142661
BM_image_edge_access_seq_neon/41/12            4741 ns         4741 ns       147654
BM_image_edge_access_seq_neon/41/13            4289 ns         4286 ns       163377
BM_image_edge_access_seq_neon/41/14            4739 ns         4739 ns       147724
BM_image_edge_access_seq_neon/41/15            4576 ns         4574 ns       152994
BM_image_edge_access_seq_neon/41/16            4909 ns         4908 ns       142657
BM_image_edge_access_seq_neon/41/31            4575 ns         4574 ns       152986
BM_image_edge_access_seq_neon/41/63            4576 ns         4575 ns       153033
BM_image_edge_access_seq_vext_neon/41/0        4988 ns         4988 ns       140307
BM_image_edge_access_seq_vext_neon/41/0        4989 ns         4988 ns       140296
BM_image_edge_access_seq_vext_neon/41/1        4990 ns         4989 ns       140316
BM_image_edge_access_seq_vext_neon/41/2        4989 ns         4988 ns       140342
BM_image_edge_access_seq_vext_neon/41/3        4992 ns         4989 ns       140335
BM_image_edge_access_seq_vext_neon/41/4        4784 ns         4782 ns       146374
BM_image_edge_access_seq_vext_neon/41/5        4619 ns         4617 ns       151558
BM_image_edge_access_seq_vext_neon/41/6        4659 ns         4658 ns       150321
BM_image_edge_access_seq_vext_neon/41/7        4825 ns         4824 ns       145106
BM_image_edge_access_seq_vext_neon/41/8        4990 ns         4989 ns       140318
BM_image_edge_access_seq_vext_neon/41/9        4990 ns         4988 ns       140339
BM_image_edge_access_seq_vext_neon/41/10       4990 ns         4988 ns       140295
BM_image_edge_access_seq_vext_neon/41/11       4989 ns         4988 ns       140347
BM_image_edge_access_seq_vext_neon/41/12       4803 ns         4802 ns       145763
BM_image_edge_access_seq_vext_neon/41/13       4579 ns         4577 ns       152930
BM_image_edge_access_seq_vext_neon/41/14       4658 ns         4657 ns       150319
BM_image_edge_access_seq_vext_neon/41/15       4823 ns         4823 ns       145094
BM_image_edge_access_seq_vext_neon/41/16       4989 ns         4989 ns       140335
BM_image_edge_access_seq_vext_neon/41/31       4824 ns         4823 ns       145129
BM_image_edge_access_seq_vext_neon/41/63       4826 ns         4825 ns       145130
BM_image_edge_access_std/42/0                 22901 ns        22899 ns        30561
BM_image_edge_access_std/42/0                 22909 ns        22904 ns        30560
BM_image_edge_access_std/42/1                 22883 ns        22881 ns        30590
BM_image_edge_access_std/42/2                 22907 ns        22903 ns        30568
BM_image_edge_access_std/42/3                 22898 ns        22896 ns        30569
BM_image_edge_access_std/42/4                 22902 ns        22899 ns        30570
BM_image_edge_access_std/42/5                 22888 ns        22883 ns        30573
BM_image_edge_access_std/42/6                 22905 ns        22904 ns        30567
BM_image_edge_access_std/42/7                 22910 ns        22904 ns        30571
BM_image_edge_access_std/42/8                 22900 ns        22899 ns        30563
BM_image_edge_access_std/42/9                 22891 ns        22886 ns        30592
BM_image_edge_access_std/42/10                22900 ns        22899 ns        30563
BM_image_edge_access_std/42/11                22906 ns        22902 ns        30570
BM_image_edge_access_std/42/12                22901 ns        22899 ns        30566
BM_image_edge_access_std/42/13                22892 ns        22888 ns        30587
BM_image_edge_access_std/42/14                22903 ns        22900 ns        30567
BM_image_edge_access_std/42/15                22906 ns        22903 ns        30568
BM_image_edge_access_std/42/16                22900 ns        22899 ns        30565
BM_image_edge_access_std/42/31                22900 ns        22899 ns        30570
BM_image_edge_access_std/42/63                22900 ns        22898 ns        30556
BM_image_edge_access_neon/42/0                10903 ns        10902 ns        64522
BM_image_edge_access_neon/42/0                10905 ns        10903 ns        64547
BM_image_edge_access_neon/42/1                10771 ns        10770 ns        65115
BM_image_edge_access_neon/42/2                10887 ns        10886 ns        64416
BM_image_edge_access_neon/42/3                10770 ns        10769 ns        65035
BM_image_edge_access_neon/42/4                10886 ns        10885 ns        64489
BM_image_edge_access_neon/42/5                10775 ns        10772 ns        65091
BM_image_edge_access_neon/42/6                10886 ns        10884 ns        64314
BM_image_edge_access_neon/42/7                10749 ns        10749 ns        65101
BM_image_edge_access_neon/42/8                10885 ns        10883 ns        64445
BM_image_edge_access_neon/42/9                10778 ns        10778 ns        64973
BM_image_edge_access_neon/42/10               10882 ns        10880 ns        64518
BM_image_edge_access_neon/42/11               10789 ns        10788 ns        64971
BM_image_edge_access_neon/42/12               10877 ns        10876 ns        64544
BM_image_edge_access_neon/42/13               10793 ns        10791 ns        64957
BM_image_edge_access_neon/42/14               10882 ns        10882 ns        64274
BM_image_edge_access_neon/42/15               10794 ns        10791 ns        65011
BM_image_edge_access_neon/42/16               10870 ns        10869 ns        64484
BM_image_edge_access_neon/42/31               10782 ns        10781 ns        65066
BM_image_edge_access_neon/42/63               10779 ns        10775 ns        65078
BM_image_edge_access_seq_neon/42/0             4999 ns         4991 ns       140324
BM_image_edge_access_seq_neon/42/0             4999 ns         4991 ns       140214
BM_image_edge_access_seq_neon/42/1             4620 ns         4620 ns       151024
BM_image_edge_access_seq_neon/42/2             4970 ns         4970 ns       140855
BM_image_edge_access_seq_neon/42/3             4794 ns         4794 ns       146016
BM_image_edge_access_seq_neon/42/4             4794 ns         4794 ns       146018
BM_image_edge_access_seq_neon/42/5             4618 ns         4618 ns       151574
BM_image_edge_access_seq_neon/42/6             4794 ns         4794 ns       146020
BM_image_edge_access_seq_neon/42/7             4794 ns         4794 ns       146018
BM_image_edge_access_seq_neon/42/8             4970 ns         4970 ns       140856
BM_image_edge_access_seq_neon/42/9             4664 ns         4664 ns       150099
BM_image_edge_access_seq_neon/42/10            4970 ns         4969 ns       140854
BM_image_edge_access_seq_neon/42/11            4794 ns         4794 ns       146019
BM_image_edge_access_seq_neon/42/12            4794 ns         4794 ns       146016
BM_image_edge_access_seq_neon/42/13            4618 ns         4618 ns       151573
BM_image_edge_access_seq_neon/42/14            4794 ns         4794 ns       146018
BM_image_edge_access_seq_neon/42/15            4794 ns         4794 ns       146014
BM_image_edge_access_seq_neon/42/16            4970 ns         4970 ns       140853
BM_image_edge_access_seq_neon/42/31            4794 ns         4794 ns       146023
BM_image_edge_access_seq_neon/42/63            4794 ns         4794 ns       146016
BM_image_edge_access_seq_vext_neon/42/0        5057 ns         5057 ns       138402
BM_image_edge_access_seq_vext_neon/42/0        5057 ns         5057 ns       138407
BM_image_edge_access_seq_vext_neon/42/1        4840 ns         4840 ns       144629
BM_image_edge_access_seq_vext_neon/42/2        5057 ns         5057 ns       138402
BM_image_edge_access_seq_vext_neon/42/3        4883 ns         4883 ns       143344
BM_image_edge_access_seq_vext_neon/42/4        5057 ns         5057 ns       138422
BM_image_edge_access_seq_vext_neon/42/5        4490 ns         4490 ns       155907
BM_image_edge_access_seq_vext_neon/42/6        5057 ns         5057 ns       138384
BM_image_edge_access_seq_vext_neon/42/7        4884 ns         4883 ns       143344
BM_image_edge_access_seq_vext_neon/42/8        5058 ns         5057 ns       138367
BM_image_edge_access_seq_vext_neon/42/9        4796 ns         4796 ns       145948
BM_image_edge_access_seq_vext_neon/42/10       5057 ns         5057 ns       138408
BM_image_edge_access_seq_vext_neon/42/11       4883 ns         4883 ns       143345
BM_image_edge_access_seq_vext_neon/42/12       5057 ns         5057 ns       138407
BM_image_edge_access_seq_vext_neon/42/13       4512 ns         4512 ns       155153
BM_image_edge_access_seq_vext_neon/42/14       5057 ns         5057 ns       138400
BM_image_edge_access_seq_vext_neon/42/15       4883 ns         4883 ns       143345
BM_image_edge_access_seq_vext_neon/42/16       5057 ns         5057 ns       138381
BM_image_edge_access_seq_vext_neon/42/31       4883 ns         4883 ns       143347
BM_image_edge_access_seq_vext_neon/42/63       4883 ns         4883 ns       143343
BM_image_edge_access_std/44/0                 25080 ns        25080 ns        27911
BM_image_edge_access_std/44/0                 25092 ns        25082 ns        27911
BM_image_edge_access_std/44/1                 25080 ns        25080 ns        27911
BM_image_edge_access_std/44/2                 25079 ns        25079 ns        27910
BM_image_edge_access_std/44/3                 25080 ns        25080 ns        27911
BM_image_edge_access_std/44/4                 25080 ns        25080 ns        27911
BM_image_edge_access_std/44/5                 25080 ns        25079 ns        27910
BM_image_edge_access_std/44/6                 25080 ns        25080 ns        27911
BM_image_edge_access_std/44/7                 25079 ns        25079 ns        27911
BM_image_edge_access_std/44/8                 25080 ns        25079 ns        27910
BM_image_edge_access_std/44/9                 25080 ns        25079 ns        27911
BM_image_edge_access_std/44/10                25080 ns        25080 ns        27909
BM_image_edge_access_std/44/11                25079 ns        25079 ns        27911
BM_image_edge_access_std/44/12                25080 ns        25080 ns        27911
BM_image_edge_access_std/44/13                25078 ns        25078 ns        27911
BM_image_edge_access_std/44/14                25080 ns        25080 ns        27911
BM_image_edge_access_std/44/15                25080 ns        25079 ns        27911
BM_image_edge_access_std/44/16                25080 ns        25079 ns        27910
BM_image_edge_access_std/44/31                25080 ns        25080 ns        27911
BM_image_edge_access_std/44/63                25080 ns        25080 ns        27911
BM_image_edge_access_neon/44/0                12646 ns        12646 ns        55359
BM_image_edge_access_neon/44/0                12645 ns        12645 ns        55350
BM_image_edge_access_neon/44/1                12716 ns        12716 ns        55039
BM_image_edge_access_neon/44/2                12645 ns        12645 ns        55354
BM_image_edge_access_neon/44/3                12388 ns        12388 ns        56506
BM_image_edge_access_neon/44/4                12643 ns        12643 ns        55369
BM_image_edge_access_neon/44/5                12715 ns        12714 ns        55043
BM_image_edge_access_neon/44/6                12644 ns        12643 ns        55356
BM_image_edge_access_neon/44/7                12384 ns        12384 ns        56528
BM_image_edge_access_neon/44/8                12644 ns        12644 ns        55361
BM_image_edge_access_neon/44/9                12715 ns        12715 ns        55044
BM_image_edge_access_neon/44/10               12643 ns        12643 ns        55366
BM_image_edge_access_neon/44/11               12386 ns        12386 ns        56514
BM_image_edge_access_neon/44/12               12645 ns        12645 ns        55360
BM_image_edge_access_neon/44/13               12716 ns        12716 ns        55039
BM_image_edge_access_neon/44/14               12646 ns        12646 ns        55353
BM_image_edge_access_neon/44/15               12388 ns        12388 ns        56502
BM_image_edge_access_neon/44/16               12646 ns        12646 ns        55355
BM_image_edge_access_neon/44/31               12387 ns        12386 ns        56520
BM_image_edge_access_neon/44/63               12386 ns        12386 ns        56509
BM_image_edge_access_seq_neon/44/0             5544 ns         5544 ns       126223
BM_image_edge_access_seq_neon/44/0             5544 ns         5544 ns       126228
BM_image_edge_access_seq_neon/44/1             5544 ns         5544 ns       126250
BM_image_edge_access_seq_neon/44/2             5352 ns         5352 ns       130783
BM_image_edge_access_seq_neon/44/3             4966 ns         4966 ns       140943
BM_image_edge_access_seq_neon/44/4             5351 ns         5351 ns       130813
BM_image_edge_access_seq_neon/44/5             5544 ns         5544 ns       126273
BM_image_edge_access_seq_neon/44/6             5544 ns         5544 ns       126258
BM_image_edge_access_seq_neon/44/7             5159 ns         5159 ns       135672
BM_image_edge_access_seq_neon/44/8             5544 ns         5544 ns       126276
BM_image_edge_access_seq_neon/44/9             5544 ns         5544 ns       126254
BM_image_edge_access_seq_neon/44/10            5352 ns         5351 ns       130757
BM_image_edge_access_seq_neon/44/11            4967 ns         4967 ns       140948
BM_image_edge_access_seq_neon/44/12            5351 ns         5351 ns       130789
BM_image_edge_access_seq_neon/44/13            5544 ns         5544 ns       126250
BM_image_edge_access_seq_neon/44/14            5544 ns         5544 ns       126249
BM_image_edge_access_seq_neon/44/15            5159 ns         5159 ns       135687
BM_image_edge_access_seq_neon/44/16            5544 ns         5544 ns       126249
BM_image_edge_access_seq_neon/44/31            5159 ns         5159 ns       135647
BM_image_edge_access_seq_neon/44/63            5159 ns         5159 ns       135682
BM_image_edge_access_seq_vext_neon/44/0        5618 ns         5618 ns       124585
BM_image_edge_access_seq_vext_neon/44/0        5618 ns         5618 ns       124581
BM_image_edge_access_seq_vext_neon/44/1        5618 ns         5618 ns       124601
BM_image_edge_access_seq_vext_neon/44/2        5618 ns         5618 ns       124577
BM_image_edge_access_seq_vext_neon/44/3        4807 ns         4807 ns       145618
BM_image_edge_access_seq_vext_neon/44/4        5618 ns         5618 ns       124606
BM_image_edge_access_seq_vext_neon/44/5        5618 ns         5618 ns       124601
BM_image_edge_access_seq_vext_neon/44/6        5618 ns         5618 ns       124584
BM_image_edge_access_seq_vext_neon/44/7        5237 ns         5237 ns       133652
BM_image_edge_access_seq_vext_neon/44/8        5618 ns         5618 ns       124581
BM_image_edge_access_seq_vext_neon/44/9        5618 ns         5618 ns       124562
BM_image_edge_access_seq_vext_neon/44/10       5618 ns         5618 ns       124603
BM_image_edge_access_seq_vext_neon/44/11       4831 ns         4831 ns       144911
BM_image_edge_access_seq_vext_neon/44/12       5618 ns         5618 ns       124578
BM_image_edge_access_seq_vext_neon/44/13       5618 ns         5618 ns       124579
BM_image_edge_access_seq_vext_neon/44/14       5618 ns         5618 ns       124569
BM_image_edge_access_seq_vext_neon/44/15       5237 ns         5237 ns       133652
BM_image_edge_access_seq_vext_neon/44/16       5618 ns         5618 ns       124583
BM_image_edge_access_seq_vext_neon/44/31       5237 ns         5237 ns       133655
BM_image_edge_access_seq_vext_neon/44/63       5237 ns         5237 ns       133673
BM_image_edge_access_std/48/0                 29936 ns        29935 ns        23384
BM_image_edge_access_std/48/0                 29935 ns        29935 ns        23383
BM_image_edge_access_std/48/1                 29923 ns        29923 ns        23394
BM_image_edge_access_std/48/2                 29935 ns        29935 ns        23384
BM_image_edge_access_std/48/3                 29934 ns        29934 ns        23385
BM_image_edge_access_std/48/4                 29935 ns        29935 ns        23384
BM_image_edge_access_std/48/5                 29935 ns        29935 ns        23384
BM_image_edge_access_std/48/6                 29935 ns        29935 ns        23384
BM_image_edge_access_std/48/7                 29935 ns        29935 ns        23384
BM_image_edge_access_std/48/8                 29935 ns        29934 ns        23384
BM_image_edge_access_std/48/9                 29924 ns        29924 ns        23392
BM_image_edge_access_std/48/10                29935 ns        29935 ns        23384
BM_image_edge_access_std/48/11                29935 ns        29935 ns        23383
BM_image_edge_access_std/48/12                29935 ns        29935 ns        23384
BM_image_edge_access_std/48/13                29935 ns        29935 ns        23384
BM_image_edge_access_std/48/14                29935 ns        29935 ns        23384
BM_image_edge_access_std/48/15                29935 ns        29935 ns        23384
BM_image_edge_access_std/48/16                29934 ns        29934 ns        23385
BM_image_edge_access_std/48/31                29934 ns        29934 ns        23383
BM_image_edge_access_std/48/63                29934 ns        29934 ns        23384
BM_image_edge_access_neon/48/0                16112 ns        16112 ns        43444
BM_image_edge_access_neon/48/0                16112 ns        16112 ns        43448
BM_image_edge_access_neon/48/1                16266 ns        16266 ns        43031
BM_image_edge_access_neon/48/2                16266 ns        16266 ns        43035
BM_image_edge_access_neon/48/3                16266 ns        16265 ns        43034
BM_image_edge_access_neon/48/4                16266 ns        16266 ns        43034
BM_image_edge_access_neon/48/5                16266 ns        16266 ns        43035
BM_image_edge_access_neon/48/6                16112 ns        16112 ns        43445
BM_image_edge_access_neon/48/7                15572 ns        15572 ns        44949
BM_image_edge_access_neon/48/8                16130 ns        16130 ns        43397
BM_image_edge_access_neon/48/9                16284 ns        16284 ns        42978
BM_image_edge_access_neon/48/10               16284 ns        16284 ns        42986
BM_image_edge_access_neon/48/11               16284 ns        16284 ns        42985
BM_image_edge_access_neon/48/12               16289 ns        16285 ns        42985
BM_image_edge_access_neon/48/13               16285 ns        16285 ns        42986
BM_image_edge_access_neon/48/14               16129 ns        16129 ns        43395
BM_image_edge_access_neon/48/15               15572 ns        15572 ns        44953
BM_image_edge_access_neon/48/16               16112 ns        16112 ns        43448
BM_image_edge_access_neon/48/31               15572 ns        15572 ns        44949
BM_image_edge_access_neon/48/63               15573 ns        15572 ns        44949
BM_image_edge_access_seq_neon/48/0             6287 ns         6287 ns       111345
BM_image_edge_access_seq_neon/48/0             6287 ns         6287 ns       111345
BM_image_edge_access_seq_neon/48/1             6518 ns         6518 ns       107398
BM_image_edge_access_seq_neon/48/2             6518 ns         6518 ns       107401
BM_image_edge_access_seq_neon/48/3             6518 ns         6518 ns       107384
BM_image_edge_access_seq_neon/48/4             6519 ns         6518 ns       107385
BM_image_edge_access_seq_neon/48/5             6518 ns         6518 ns       107401
BM_image_edge_access_seq_neon/48/6             6287 ns         6287 ns       111345
BM_image_edge_access_seq_neon/48/7             5363 ns         5362 ns       130540
BM_image_edge_access_seq_neon/48/8             6287 ns         6287 ns       111330
BM_image_edge_access_seq_neon/48/9             6518 ns         6518 ns       107385
BM_image_edge_access_seq_neon/48/10            6518 ns         6518 ns       107382
BM_image_edge_access_seq_neon/48/11            6518 ns         6518 ns       107400
BM_image_edge_access_seq_neon/48/12            6518 ns         6518 ns       107368
BM_image_edge_access_seq_neon/48/13            6518 ns         6518 ns       107369
BM_image_edge_access_seq_neon/48/14            6287 ns         6287 ns       111305
BM_image_edge_access_seq_neon/48/15            5362 ns         5362 ns       130487
BM_image_edge_access_seq_neon/48/16            6287 ns         6287 ns       111327
BM_image_edge_access_seq_neon/48/31            5364 ns         5363 ns       130541
BM_image_edge_access_seq_neon/48/63            5363 ns         5363 ns       130541
BM_image_edge_access_seq_vext_neon/48/0        6409 ns         6409 ns       109205
BM_image_edge_access_seq_vext_neon/48/0        6409 ns         6409 ns       109187
BM_image_edge_access_seq_vext_neon/48/1        6409 ns         6409 ns       109201
BM_image_edge_access_seq_vext_neon/48/2        6409 ns         6409 ns       109219
BM_image_edge_access_seq_vext_neon/48/3        6409 ns         6409 ns       109221
BM_image_edge_access_seq_vext_neon/48/4        6409 ns         6409 ns       109222
BM_image_edge_access_seq_vext_neon/48/5        6410 ns         6409 ns       109222
BM_image_edge_access_seq_vext_neon/48/6        6409 ns         6409 ns       109206
BM_image_edge_access_seq_vext_neon/48/7        4963 ns         4963 ns       141053
BM_image_edge_access_seq_vext_neon/48/8        6410 ns         6409 ns       109200
BM_image_edge_access_seq_vext_neon/48/9        6410 ns         6409 ns       109218
BM_image_edge_access_seq_vext_neon/48/10       6410 ns         6409 ns       109218
BM_image_edge_access_seq_vext_neon/48/11       6409 ns         6409 ns       109218
BM_image_edge_access_seq_vext_neon/48/12       6409 ns         6409 ns       109217
BM_image_edge_access_seq_vext_neon/48/13       6409 ns         6409 ns       109176
BM_image_edge_access_seq_vext_neon/48/14       6409 ns         6409 ns       109180
BM_image_edge_access_seq_vext_neon/48/15       4963 ns         4963 ns       141052
BM_image_edge_access_seq_vext_neon/48/16       6409 ns         6409 ns       109218
BM_image_edge_access_seq_vext_neon/48/31       4963 ns         4963 ns       141054
BM_image_edge_access_seq_vext_neon/48/63       4963 ns         4963 ns       141053
BM_image_edge_access_std/64/0                 53684 ns        53684 ns        13040
BM_image_edge_access_std/64/0                 53682 ns        53682 ns        13040
BM_image_edge_access_std/64/1                 53586 ns        53586 ns        13064
BM_image_edge_access_std/64/2                 53690 ns        53690 ns        13036
BM_image_edge_access_std/64/3                 53676 ns        53676 ns        13040
BM_image_edge_access_std/64/4                 53682 ns        53682 ns        13037
BM_image_edge_access_std/64/5                 53679 ns        53678 ns        13038
BM_image_edge_access_std/64/6                 53680 ns        53679 ns        13041
BM_image_edge_access_std/64/7                 53680 ns        53679 ns        13041
BM_image_edge_access_std/64/8                 53678 ns        53678 ns        13039
BM_image_edge_access_std/64/9                 53595 ns        53594 ns        13061
BM_image_edge_access_std/64/10                53694 ns        53693 ns        13037
BM_image_edge_access_std/64/11                53693 ns        53692 ns        13038
BM_image_edge_access_std/64/12                53693 ns        53693 ns        13038
BM_image_edge_access_std/64/13                53693 ns        53692 ns        13038
BM_image_edge_access_std/64/14                53693 ns        53692 ns        13038
BM_image_edge_access_std/64/15                53693 ns        53692 ns        13038
BM_image_edge_access_std/64/16                53692 ns        53692 ns        13038
BM_image_edge_access_std/64/31                53691 ns        53690 ns        13035
BM_image_edge_access_std/64/63                53696 ns        53695 ns        13034
BM_image_edge_access_neon/64/0                24517 ns        24517 ns        28552
BM_image_edge_access_neon/64/0                24517 ns        24517 ns        28551
BM_image_edge_access_neon/64/1                24829 ns        24828 ns        28192
BM_image_edge_access_neon/64/2                24816 ns        24816 ns        28207
BM_image_edge_access_neon/64/3                24815 ns        24815 ns        28208
BM_image_edge_access_neon/64/4                24817 ns        24817 ns        28206
BM_image_edge_access_neon/64/5                24814 ns        24813 ns        28209
BM_image_edge_access_neon/64/6                24498 ns        24498 ns        28571
BM_image_edge_access_neon/64/7                23450 ns        23450 ns        29849
BM_image_edge_access_neon/64/8                24502 ns        24502 ns        28569
BM_image_edge_access_neon/64/9                24816 ns        24816 ns        28206
BM_image_edge_access_neon/64/10               24816 ns        24816 ns        28208
BM_image_edge_access_neon/64/11               24816 ns        24816 ns        28207
BM_image_edge_access_neon/64/12               24816 ns        24816 ns        28208
BM_image_edge_access_neon/64/13               24816 ns        24816 ns        28207
BM_image_edge_access_neon/64/14               24503 ns        24503 ns        28569
BM_image_edge_access_neon/64/15               23448 ns        23448 ns        29852
BM_image_edge_access_neon/64/16               24507 ns        24507 ns        28562
BM_image_edge_access_neon/64/31               23458 ns        23458 ns        29841
BM_image_edge_access_neon/64/63               23455 ns        23455 ns        29841
BM_image_edge_access_seq_neon/64/0            11279 ns        11279 ns        62064
BM_image_edge_access_seq_neon/64/0            11279 ns        11279 ns        62054
BM_image_edge_access_seq_neon/64/1            11698 ns        11697 ns        59835
BM_image_edge_access_seq_neon/64/2            11697 ns        11697 ns        59844
BM_image_edge_access_seq_neon/64/3            11698 ns        11698 ns        59842
BM_image_edge_access_seq_neon/64/4            11697 ns        11697 ns        59837
BM_image_edge_access_seq_neon/64/5            11697 ns        11697 ns        59831
BM_image_edge_access_seq_neon/64/6            11280 ns        11279 ns        62052
BM_image_edge_access_seq_neon/64/7             9620 ns         9618 ns        72793
BM_image_edge_access_seq_neon/64/8            11282 ns        11281 ns        62052
BM_image_edge_access_seq_neon/64/9            11702 ns        11702 ns        59818
BM_image_edge_access_seq_neon/64/10           11702 ns        11702 ns        59814
BM_image_edge_access_seq_neon/64/11           11702 ns        11702 ns        59811
BM_image_edge_access_seq_neon/64/12           11702 ns        11702 ns        59812
BM_image_edge_access_seq_neon/64/13           11702 ns        11702 ns        59818
BM_image_edge_access_seq_neon/64/14           11281 ns        11281 ns        62045
BM_image_edge_access_seq_neon/64/15            9613 ns         9613 ns        72823
BM_image_edge_access_seq_neon/64/16           11278 ns        11278 ns        62061
BM_image_edge_access_seq_neon/64/31            9613 ns         9613 ns        72818
BM_image_edge_access_seq_neon/64/63            9616 ns         9616 ns        72791
BM_image_edge_access_seq_vext_neon/64/0       11491 ns        11491 ns        60909
BM_image_edge_access_seq_vext_neon/64/0       11491 ns        11491 ns        60915
BM_image_edge_access_seq_vext_neon/64/1       11491 ns        11491 ns        60916
BM_image_edge_access_seq_vext_neon/64/2       11491 ns        11491 ns        60921
BM_image_edge_access_seq_vext_neon/64/3       11491 ns        11491 ns        60910
BM_image_edge_access_seq_vext_neon/64/4       11492 ns        11491 ns        60910
BM_image_edge_access_seq_vext_neon/64/5       11492 ns        11492 ns        60915
BM_image_edge_access_seq_vext_neon/64/6       11491 ns        11491 ns        60914
BM_image_edge_access_seq_vext_neon/64/7        8889 ns         8889 ns        78755
BM_image_edge_access_seq_vext_neon/64/8       11494 ns        11494 ns        60906
BM_image_edge_access_seq_vext_neon/64/9       11494 ns        11494 ns        60895
BM_image_edge_access_seq_vext_neon/64/10      11494 ns        11494 ns        60893
BM_image_edge_access_seq_vext_neon/64/11      11494 ns        11494 ns        60903
BM_image_edge_access_seq_vext_neon/64/12      11494 ns        11494 ns        60907
BM_image_edge_access_seq_vext_neon/64/13      11494 ns        11494 ns        60906
BM_image_edge_access_seq_vext_neon/64/14      11494 ns        11494 ns        60896
BM_image_edge_access_seq_vext_neon/64/15       8890 ns         8890 ns        78731
BM_image_edge_access_seq_vext_neon/64/16      11492 ns        11492 ns        60905
BM_image_edge_access_seq_vext_neon/64/31       8888 ns         8888 ns        78760
BM_image_edge_access_seq_vext_neon/64/63       8895 ns         8894 ns        78708
```


```
# ./a.out
[==========] Running 0 tests from 0 test cases.
[==========] 0 tests from 0 test cases ran. (1 ms total)
[  PASSED  ] 0 tests.
2021-05-08 03:54:44
Running ./a.out
Run on (4 X 2989.26 MHz CPU s)
Load Average: 0.04, 0.08, 0.02
-----------------------------------------------------------------------------
Benchmark                                   Time             CPU   Iterations
-----------------------------------------------------------------------------
BM_image_vertical_access/48/0            5263 ns         5260 ns       131999
BM_image_vertical_access/48/2            6177 ns         6173 ns       112570
BM_image_vertical_access/50/0            6403 ns         6396 ns       109108
BM_image_vertical_access/50/2            5915 ns         5909 ns       118212
BM_image_vertical_access/50/4            6408 ns         6404 ns       109648
BM_image_vertical_access/50/6            5920 ns         5917 ns       118420
BM_image_vertical_access/64/0            9042 ns         9034 ns        77428
BM_image_vertical_access/64/2           10320 ns        10312 ns        67893
BM_image_vertical_access/70/0           11701 ns        11691 ns        59857
BM_image_vertical_access/70/2           10359 ns        10352 ns        67510
BM_image_vertical_access_neon/48/0       5260 ns         5258 ns       132538
BM_image_vertical_access_neon/48/2       6172 ns         6161 ns       113599
BM_image_vertical_access_neon/50/0       6373 ns         6367 ns       109799
BM_image_vertical_access_neon/50/2       5910 ns         5905 ns       118507
BM_image_vertical_access_neon/50/4       6378 ns         6377 ns       109853
BM_image_vertical_access_neon/50/6       5908 ns         5904 ns       118538
BM_image_vertical_access_neon/64/0       9036 ns         9028 ns        77258
BM_image_vertical_access_neon/64/2      10315 ns        10306 ns        68002
BM_image_vertical_access_neon/70/0      11693 ns        11685 ns        59767
BM_image_vertical_access_neon/70/2      10359 ns        10353 ns        67668
```

```
---------------------------------------------------------
Benchmark               Time             CPU   Iterations
---------------------------------------------------------
BM_vext/0           19011 ns        19009 ns        36809
BM_vld1q_s16/0      26677 ns        26674 ns        26244
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
