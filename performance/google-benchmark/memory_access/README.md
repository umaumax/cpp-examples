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
