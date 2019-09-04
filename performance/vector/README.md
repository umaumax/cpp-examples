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

* static_vector
  * 実は固定長で運用可能な`std::vector`のコードを簡易的に置き換えることはできないかどうかという試み
  * structのfieldのこの型を他の関数の呼び出しで利用している場合には，単純に置換しただけでは不十分な挙動となる
  * [【C\+\+】既存のクラスを拡張する方法【拡張メソッド／カテゴリ】 \| MaryCore]( https://marycore.jp/prog/cpp/class-extension-methods/ )
