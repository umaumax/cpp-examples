# hamming_distance

[Hamming Distance between Two Integers \| Technology of Computing]( https://helloacm.com/hamming-distance-between-two-integers/ )

## how to build
```
g++ -std=c++11 -O3 -g main.cpp -lbenchmark -lpthread
```

## result
ubuntu
```sh
$ ./a.out                                                                                                                                                                         V
2019-01-15 19:00:21
Running ./a.out
Run on (12 X 4600 MHz CPU s)
CPU Caches:
  L1 Data 32K (x6)
  L1 Instruction 32K (x6)
  L2 Unified 256K (x6)
  L3 Unified 12288K (x1)
Load Average: 0.82, 0.76, 0.42
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
--------------------------------------------------------------
Benchmark                    Time             CPU   Iterations
--------------------------------------------------------------
BM_LongTest1/65536        5.45 ns         5.45 ns    122308578
BM_LongTest1/131072       6.04 ns         6.04 ns    117575824
BM_LongTest2/65536       0.394 ns        0.394 ns   1000000000
BM_LongTest2/131072      0.393 ns        0.393 ns   1000000000
BM_LongTest3/65536       0.387 ns        0.387 ns   1000000000
BM_LongTest3/131072      0.395 ns        0.395 ns   1000000000
BM_LongTest4/65536       0.397 ns        0.397 ns   1000000000
BM_LongTest4/131072      0.398 ns        0.398 ns   1000000000
```
