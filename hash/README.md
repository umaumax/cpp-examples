# hash

* e.g.
  * あるデータがコード編集の前後で変化していないかどうかの簡易判定用途
    * 計算順序に対して，可換性を持たないことことが重要
      * つまり，交換法則が成り立たない(交換律が成り立たない)ことを満たせば良い
    * 結果のファイル書き込みが困難な場合(書き込み速度，書き込み量など)
    * ただし，`float`と`double`で型を変更して演算した場合には完全一致しないと無理
      * その場合には，ある程度丸めて，`std::string`として回避可能
        * e.g. `std::setprecision(5)`程度ならば，一致した(演算依存)

## WARN
* `std:hash`や`boost::hash`の結果は`std:size_t`であり，32bitと64bitのマシン環境で結果が異なってしまうことに注意
  * [c\+\+ \- Get 32\-bit hash value from boost::hash \- Stack Overflow]( https://stackoverflow.com/questions/17426493/get-32-bit-hash-value-from-boosthash )
  * そのため，`boost`の`md5`や`sha1`がおすすめ(これらのは`-I`や`-l`を追加せずとも使用可能)

## hash list
[hashアルゴリズムとハッシュ値の長さ一覧 \- Qiita]( https://qiita.com/KEINOS/items/c92268386d265042ea16 )

| algo | length |
|------|--------|
| md5  | 16B    |
| sha1 | 20B    |

----

## how to build
```
g++ -std=c++11 main.cpp -lgtest
```

## FYI
### checksum
[チェックサム \- Wikipedia]( https://ja.wikipedia.org/wiki/%E3%83%81%E3%82%A7%E3%83%83%E3%82%AF%E3%82%B5%E3%83%A0 )

> 単純な加算であるため、語を保持したまま列の順序のみ変化した場合には同じ値を示す。また、それ以外の誤りに対しても、符号値が同じになる確率は決して低くなく、誤り検出の方式としての信頼性は高くない。

