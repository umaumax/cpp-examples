# c++ weak

[リンクと同名シンボル: weak シンボル編 \- bkブログ]( http://0xcc.net/blog/archives/000062.html )
weak symbolを利用すると定義の数が1以外の場合(0または複数の場合に対応可能)

```
$ g++ -std=c++11 sqrt-with-cmath.cpp
$ ./a.out
sqrt(2)=1.41421
```

```
$ g++ -std=c++11 sqrt-without-cmath.cpp
$ ./a.out
no sqrt function!
```

* you can also use clang++
