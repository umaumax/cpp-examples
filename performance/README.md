# gprof

## memo
* gprofの特徴として，コンパイル時に`-pg`を付加し，その実行ファイルをするだけで，`gmon.out`が生成され，`gprof`コマンドで解析が可能
  * つまり，既存のソースコードを変更する必要がない

* 仕組み
  * [プロファイラのしくみ \- Backnumbers: Steps to Phantasien]( http://steps.dodgson.org/bn/2007/08/23/ )

> コンパイラは各関数の冒頭に "mcount" という名前の関数呼出しを加える. リンクする C のランタイムも専用バージョン (gcrt0.o) に差し替わる. このランタイムは裏で profil() 関数を使いタイマを仕掛ける. そのタイマは発火毎に現在のプログラム・カウンタ(PC)のレジスタを記録する

関数に埋め込んでいるので，call回数は正確だが，時間は誤差がある

他のライブラリを含まず，自分でコンパイルしている単位の関数でのパフォーマンスがわかるので，
それほどまで深いコールグラフとはならないので`perf`コマンドよりも粒度が粗いが全体を理解しやすい

`perf`コマンドの場合，デバッグ情報(frame info)の不足で，完全なコールスタックを生成できないことが多く，わかりにくいことがある

## how to build
* add `-pg` option
```
g++ -std=c++11 -pg -g main.cpp
```

## flat profile
```
Each sample counts as 0.01 seconds.
  %   cumulative   self              self     total
 time   seconds   seconds    calls   s/call   s/call  name
 83.19      4.55     4.55        1     4.55     4.55  mul1000000000(double)
  8.47      5.01     0.46    10001     0.00     0.00  mul10000(double)
  8.28      5.47     0.45        1     0.45     0.45  mul100000000(double)
  0.74      5.51     0.04        1     0.04     0.04  mul10000000(double)
  0.00      5.51     0.00        1     0.00     0.00  _GLOBAL__sub_I__Z8mul10000d
  0.00      5.51     0.00        1     0.00     0.46  mul10000_10000(double)
  0.00      5.51     0.00        1     0.00     0.00  __static_initialization_and_destruction_0(int, int)
```

* sample counts: 分解能
* time(%),self(sec): ここの欄は純粋にその関数frameの処理時間(つまり，子関数はcount外)
* cumulative(sec): 累計時間: 一つ前のランキングにselfを加算した結果(一番下の値は実行時間にほぼ等しい)
* total(s/call): __関数が呼び出すサブ関数の時間を含む__(`total >= self * call`)
* self(sec),nameの順でソートされるので，この場合例えば，main()が上位にはこない
  * 足切りの基準は不明

## call graph
> This table describes the call tree of the program, and was sorted by the total amount of time spent in each function and its children.

つまり，self+childrenの時間の順番
* `[]`内の数値はただの通し番号，cumulative(sec)のDESC
  * たまに番号が飛んでいるが，これは時間が短いorサンプリングできなかったので，省略されているだけ(-zで表示可能)

* 右側の関数の順番はcall stackの順番だと思われる(基準より上ならばそれが基準の関数を呼び，下ならば基準の関数がそれを呼ぶ(要するに，call graphのちょうど上と下に対応)(基準以外の関数の並びは基本的に意味がなさそう))
  * これは時間でソートするから，自然とそうなる気がする

```
index % time    self  children    called     name
                                                 <spontaneous>
[1]    100.0    0.00    5.51                 main [1]
                4.55    0.00       1/1           mul1000000000(double) [2]
                0.00    0.46       1/1           mul10000_10000(double) [4]
                0.45    0.00       1/1           mul100000000(double) [5]
                0.04    0.00       1/1           mul10000000(double) [6]
                0.00    0.00       1/10001       mul10000(double) [3]
-----------------------------------------------
                4.55    0.00       1/1           main [1]
[2]     82.6    4.55    0.00       1         mul1000000000(double) [2]
-----------------------------------------------
                0.00    0.00       1/10001       main [1]
                0.46    0.00   10000/10001       mul10000_10000(double) [4]
[3]      8.4    0.46    0.00   10001         mul10000(double) [3]
-----------------------------------------------
                0.00    0.46       1/1           main [1]
[4]      8.4    0.00    0.46       1         mul10000_10000(double) [4]
                0.46    0.00   10000/10001       mul10000(double) [3]
-----------------------------------------------
                0.45    0.00       1/1           main [1]
[5]      8.2    0.45    0.00       1         mul100000000(double) [5]
-----------------------------------------------
                0.04    0.00       1/1           main [1]
[6]      0.7    0.04    0.00       1         mul10000000(double) [6]
-----------------------------------------------
                0.00    0.00       1/1           __libc_csu_init [20]
[13]     0.0    0.00    0.00       1         _GLOBAL__sub_I__Z8mul10000d [13]
                0.00    0.00       1/1           __static_initialization_and_destruction_0(int, int) [14]
-----------------------------------------------
                0.00    0.00       1/1           _GLOBAL__sub_I__Z8mul10000d [13]
[14]     0.0    0.00    0.00       1         __static_initialization_and_destruction_0(int, int) [14]
-----------------------------------------------
```

### 可視化
```
gprof ./a.out | gprof2dot | dot -Tsvg -o output.svg
```

## tips
### プログラム全体の処理時間が短い時のTips
* [gprofを使いこなす \- minus9d's diary]( https://minus9d.hatenablog.com/entry/20140112/1389502918 )

> プログラム全体の処理時間がgprofの分解能(今回の場合では0.01秒)に比べてあまり大きくない時は、プログラムを違うデータで何回が実行し、出力された複数のgmon.outを結合する技が使える。結合にはgprofの-sオプションを使う。詳しくはGNU gprofの"Statistical Inaccuracy of gprof Output"を参照。

### ソースコードのどの行が何回実行されたかを表示
* `-A` option

----

## FYI
* [gprof\.pdf]( http://mikilab.doshisha.ac.jp/dia/seminar/2002/pdf/gprof.pdf )

* [tuning\.pdf]( http://apollon.issp.u-tokyo.ac.jp/~watanabe/pdf/tuning.pdf )

> gprof の出力を見ると、Each sample counts as 0.01 seconds. という記述がある。これは 0.01 秒ごとに割り込みをかけて調べており、調べた時に実行中だった関数を 0.01 秒かかったと見なした、という意味であり、実際の実行時間は、ほぼ 0 から 0.02 秒までの幅があり得る。サンプリング型のプロファイラでは、このように観測時間を「量子化」して測定するため、実行時間が短いと正しく統計データが出ない。大雑把には、サンプリング間隔の 100 倍くらいの実行時間であればだいたい信頼できる、と覚えておけば良い。

> Mac OS X では gprof を使う事ができない。-pg つきでコンパイル、実行すると gmon.out は出力されるのだが、gprof で出力された結果には何も表示されない。Mac でプロファイルデータが欲しい場合には、Xcode の Shark を使うと良い。

## Question
* `-c`の効果がいまいちわかっていない
* call graphが分裂してしまう場合がある(このときは，`-z`を追加すれば良い?)
