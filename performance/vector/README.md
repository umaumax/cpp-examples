# vector

## structのfieldの型(vector vs. array)
* struct自体の配列の長さやそのstructが持つfieldのarray(or vector)の長さに起因するが，例えば，下記のようにcacheが効いている条件下でも10倍以上アクセス速度さが生じる
  * `array`: structのarrayの構造がそのまま連続領域となる
  * `vector`: structのvectorが指し示す先のアドレスにアクセスをする必要性があるため，その参照時間がかかる
```
array
calc elapsed time:    111090[us]
vector
calc elapsed time:   1812451[us]
```

```
g++ -std=c++11 -O3 vector_vs_array.cpp
```
