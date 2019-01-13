# perormance examples

## TODO
google benchmark libを利用したデータの初期化方法を調べる

## google-benchmark
* [Efficient C/C\+\+ Coding Techniques]( http://www.open-std.org/Jtc1/sc22/wg21/docs/ESC_Boston_01_304_paper.pdf )
  * p.11
```
$ g++ -std=c++11 loop.cpp -O3 -lbenchmark -Wall
$ ./a.out
2019-01-13 17:37:32
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
BM_array_access_by_count_up           2445 ns       2360 ns     296501
BM_pointer_access_by_count_up         2368 ns       2313 ns     292866
BM_pointer_access_by_count_down       2345 ns       2314 ns     299410
```
