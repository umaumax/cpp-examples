# c++

## 関数の公開範囲制限
* [ライブラリの外に公開するシンボルを制限する \- bkブログ]( http://0xcc.net/blog/archives/000108.html )
* [LinuxのC言語共有ライブラリでの、公開するAPI制限方法 \- Qiita]( https://qiita.com/developer-kikikaikai/items/a1871e1d8a3b6488e42e )

```
$ g++ -fPIC -c foo.cpp
$ g++ -fPIC -c bar.cpp
$ g++ -shared -fPIC -o libfoo.so foo.o bar.o
$ nm -D libfoo.so | grep -e 'foo' -e 'bar'
000000000000065c T _Z3barv
0000000000000650 T _Z3foov

$ g++ -shared -fPIC -o libfoo.so foo.o bar.o -Wl,--version-script,libfoo.map
$ nm -D libfoo.so | grep -e 'foo' -e 'bar'
0000000000000550 T _Z3foov
```




