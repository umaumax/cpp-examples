# vector

## how to build
```
g++ -std=c++11 -O3 main.cpp -Wall -lbenchmark -lgtest
```

[Reusing a vector in C\+\+ \- Stack Overflow]( https://stackoverflow.com/questions/2426967/reusing-a-vector-in-c )

```
std::vector<int> vec;

// do processing

{
  int reuse_size = vec.size();
  vec.clear();
  vec.reserve(reuse_size);
}
```

```
[==========] Running 0 tests from 0 test suites.
[==========] 0 tests from 0 test suites ran. (1 ms total)
[  PASSED  ] 0 tests.
2019-05-07 21:06:55
Running ./a.out
Run on (4 X 1700 MHz CPU s)
CPU Caches:
  L1 Data 32K (x2)
  L1 Instruction 32K (x2)
  L2 Unified 262K (x2)
  L3 Unified 4194K (x1)
------------------------------------------------------------
Benchmark                     Time           CPU Iterations
------------------------------------------------------------
BM_vector/32              80754 ns      77951 ns       8405
BM_vector/64             166236 ns     149616 ns       4404
BM_vector/128            329513 ns     298382 ns       2259
BM_vector/256            662469 ns     619185 ns       1125
BM_vector/512           1396496 ns    1258448 ns        520
BM_vector/1024          2713835 ns    2611175 ns        269
BM_vector/2048          8618944 ns    7860227 ns         88
BM_vector/4096         19858673 ns   18153436 ns         39
BM_vector/8192         39946320 ns   37742556 ns         18
BM_vector_reuse/32        69348 ns      64381 ns      10667
BM_vector_reuse/64       139865 ns     128790 ns       5193
BM_vector_reuse/128      282472 ns     258750 ns       2667
BM_vector_reuse/256      574453 ns     517864 ns       1325
BM_vector_reuse/512     1120503 ns    1049354 ns        667
BM_vector_reuse/1024    2244584 ns    2124060 ns        334
BM_vector_reuse/2048    5009040 ns    4437622 ns        127
BM_vector_reuse/4096    8954253 ns    8592714 ns         77
BM_vector_reuse/8192   18157653 ns   17403711 ns         38
```

