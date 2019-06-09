# function trace(hook)

* Ubuntu16.04: OK
* Max OS X: NG?

## how to build
```
# NOTE: add -g option to use DWARF
# readelf -S main | grep debug
# trace program
g++ -std=c++11 -fPIC -shared trace.cpp -o libtrace.so -ldl -rdynamic
# main program
g++ -std=c++11 -rdynamic -fPIC -finstrument-functions main.cpp -o main
```

NOTE: main programに`-rdynamic`を付加して，すべてのシンボルを動的シンボルテーブルに追加しないと`dladdr`で関数名が取得できない

## how to run
```
LD_PRELOAD=./libtrace.so ./main
# with demangling
LD_PRELOAD=./libtrace.so ./main | c++filt
```

### result
* `c++filt`がある場合で考えると，`std::string`が下記のように分解されるため，シンプルに利用するには工夫が必要?
```
$ LD_PRELOAD=./libtrace.so ./main
simple tracer: enter - main
Hello under world!
simple tracer: enter - _Z11hello_worldNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
Hello world!
Nanoha!
simple tracer: exit  - _Z11hello_worldNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
simple tracer: exit  - main

$ LD_PRELOAD=./libtrace.so ./main | c++filt
simple tracer: enter - main
Hello under world!
simple tracer: enter - hello_world(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >)
Hello world!
Nanoha!
simple tracer: exit  - hello_world(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >)
simple tracer: exit  - main
```

## IDEA
* 引数/戻り値のrangeの統計を取得できないかどうか
* SIMD系の関数呼び出しのhookは可能かどうか(macroの可能性調査)
  * 自動的にSIMDのアルゴリズムの図を生成して，可視化?
* manglingに必要なライブラリの環境が複雑な場合
  * 簡易的に，`nm`の結果を設定ファイルとして利用できるようにすることで回避?
* [Graphvizによるファンクション・コールの視覚化]( https://www.ibm.com/developerworks/jp/linux/library/l-graphvis/index.html )

## NOTE
* trace対象から除外
  * `__attribute__((no_instrument_function))`属性を関数に付加
* 実行時demangling
  * [C\+\+ のシンボルをデマングルする \- bkブログ]( http://0xcc.net/blog/archives/000095.html )

### `__builtin_frame_address`
* [DSAS開発者の部屋:オープンソースを楽しむエンジニア達のこだわり ～ ftrace で引数を表示する]( http://dsas.blog.klab.org/archives/51039557.html )
  * 実行時の引数の値の取得方法として，`__builtin_frame_address`を利用する
    * 問題として，corss platformに正常に動作するには`DWARF`が必要となる
      * [c\+\+ \- Find out function arguments value from stack pointer \- Stack Overflow]( https://stackoverflow.com/questions/13834341/find-out-function-arguments-value-from-stack-pointer )
    * そもそも，ABI依存でレジスタ経由で引数のデータを渡すのでは?
  * [DSAS開発者の部屋:オープンソースを楽しむエンジニア達のこだわり ～ デバッグ情報を得る]( http://dsas.blog.klab.org/archives/51043750.html )

```
if (func_name == "_Z3addii") {
  const int arg_offset   = ???;
  const int arg_offset_a = 0;
  const int arg_offset_b = sizeof(int);
  char* frame =
      reinterpret_cast<char*>(__builtin_frame_address(1)) + arg_offset;
  printf("a=%d\n", *(int*)(frame + arg_offset_a));
  printf("b=%d\n", *(int*)(frame + arg_offset_b));
  for (int i = 0; i < 24; i++) {
    printf("[%d]:%d\n", 4 * i, *(int*)(frame + 4 * i));
  }
}
```

## QUESTIONS
* 引数/戻り値の取得
  * [DSAS開発者の部屋:オープンソースを楽しむエンジニア達のこだわり ～ ftrace で引数を表示する]( http://dsas.blog.klab.org/archives/51039557.html )

## FYI
* [function hooking \- primitive: blog]( http://i-saint.hatenablog.com/entry/2013/07/19/205539 )
