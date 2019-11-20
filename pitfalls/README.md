# c++ pitfalls

## クラスメンバにconstexpr static変数を利用する
(コンパイル時に決定される定数として利用したい)

TL;DR
* hppにマクロを指定してそもそもクラスメンバとしない
* hppに`inline`で変数を定義する(warning: inline variables are a C++17 extension [-Wc++17-extensions])
* hppに`static constexpr`で変数を定義し，cppにて宣言する

FYI
* [c\+\+ \- Undefined reference to static constexpr char\[\] \- Stack Overflow]( https://stackoverflow.com/questions/8016780/undefined-reference-to-static-constexpr-char )
* [C\+\+ \- 静的クラスのメンバー \| c\+\+ Tutorial]( https://riptutorial.com/ja/cplusplus/example/15150/%E9%9D%99%E7%9A%84%E3%82%AF%E3%83%A9%E3%82%B9%E3%81%AE%E3%83%A1%E3%83%B3%E3%83%90%E3%83%BC )
* [クラスメンバに constexpr static 変数はおすすめしない ― 現象と対策 \- Qiita]( https://qiita.com/Nabetani/items/d8a3ebccaef03cd18d81 )
* [静的なメンバ定数を参照するとリンクエラーとなる \- Faith and Brave \- C\+\+で遊ぼう]( https://faithandbrave.hateblo.jp/entry/2017/10/16/160146 )

e.g. NG pattern

`foo.hpp`
```
class Foo {
private:
  static constexpr int baz_ = 123;
};
```

e.g. OK pattern

add below code to `foo.cpp`
```
constexpr Foo::baz_;
```
privateでも宣言だけは普通に記述可能
