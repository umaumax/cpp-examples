# function trace(hook)

* Ubuntu16.04: OK
* Max OS X: NG?

## how to build
```
# trace program
g++ -fPIC -shared trace.cpp -o libtrace.so -ldl -rdynamic
# main program
g++ -rdynamic -fPIC -finstrument-functions main.cpp  -o main
```

NOTE: main programに`-rdynamic`を付加して，すべてのシンボルを動的シンボルテーブルに追加しないと`dladdr`で関数名が取得できない

## how to run
```
LD_PRELOAD=./libtrace.so ./main
# with demangling
LD_PRELOAD=./libtrace.so ./main | c++filt
```

## IDEA
* 引数/戻り値のrangeの統計を取得できないかどうか
* SIMD系の関数呼び出しのhookは可能かどうか(macroの可能性調査)
  * 自動的にSIMDのアルゴリズムの図を生成して，可視化?

## NOTE
* trace対象から除外
  * `__attribute__((no_instrument_function))`属性を関数に付加
* 実行時demangling
  * [C\+\+ のシンボルをデマングルする \- bkブログ]( http://0xcc.net/blog/archives/000095.html )

## QUESTIONS
* 引数/戻り値の取得
  * [DSAS開発者の部屋:オープンソースを楽しむエンジニア達のこだわり ～ ftrace で引数を表示する]( http://dsas.blog.klab.org/archives/51039557.html )

## FYI
* [function hooking \- primitive: blog]( http://i-saint.hatenablog.com/entry/2013/07/19/205539 )
