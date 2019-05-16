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
