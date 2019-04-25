# arm neon

## how to compile
```
g++ -std=c++11 test_main.cpp -lgtest -lbenchmark
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
