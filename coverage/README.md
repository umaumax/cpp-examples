# coverage

## install
### darwin
```
brew install lcov
```

## NOTE
* `.gcno`: buildときに生成
* `.gcda`: 実行時に生成
* `.info`: `lcov`で生成
* `.gcov`: `gcov`で生成: human readable, ファイルパス,行,実行回数の組は取得可能だが，どの関数かどうかを自動的に抽出は難しい

* `gcov` output: ファイルパス,関数,カバレッジの組は取得可能だが，余計な関数も含まれ，その関数のlocationが不明
  * `global`コマンドを利用した逆引きは難しそう

## how to run
0. `-O0`,`-g`,`--coverage`付きのオプションでビルド

1. `lcov --directory . --zerocounters`: counterリセット(`.gcda`ファイル削除)
2. __実行ファイルを実行__
  * 実行するごとにカウンタが累積する
3. `lcov`コマンド実行
4. `genhtml`コマンド実行

```
$ lcov --directory . --zerocounters
```

```
$ g++ -std=c++11 -g -O0 --coverage main.cpp
# 'main.gcno: GCC gcno coverage (-ftest-coverage), version 4.2'

$ rm -rf *.gcda
$ ./a.out
$ ./a.out 'hello'
# 'main.gcda: GCC gcda coverage (-fprofile-arcs), version 4.2'

$ lcov --directory . --capture -o coverage.info
# 'coverage.info: LCOV coverage tracefile'

$ lcov --remove coverage.info "*/googletest/*" "test/*" "*/c++/*" -o coverage_filtered.info
# 'coverage_filtered.info: LCOV coverage tracefile'

$ genhtml -o lcov_html --num-spaces 4 -s --legend coverage_filtered.info
# 'lcov_html/: directory'

$ open lcov_html/coverage/index.html
```

`genhtml`で生成されるディレクトリ構成はもともとのディレクトリ構成に一致

### for debug
```
$ lcov --list coverage.info
Reading tracefile coverage.info
                |Lines       |Functions  |Branches    
Filename        |Rate     Num|Rate    Num|Rate     Num
======================================================
[/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/]
__locale        | 100%      2| 100%     2|    -      0
__string        | 100%      3| 100%     3|    -      0
ios             |73.3%     15|77.8%     9|    -      0
iterator        | 100%      2| 100%     3|    -      0
locale          |53.6%     28| 100%     1|    -      0
memory          |80.0%     10|81.8%    11|    -      0
ostream         |78.3%     23| 100%     5|    -      0
streambuf       | 100%      1| 100%     1|    -      0
string          |69.0%     29|58.8%    17|    -      0
type_traits     | 0.0%      1| 0.0%     1|    -      0

[/Users/uma/github.com/cpp-examples/coverage/]
main.cpp        |70.0%     20| 100%     2|    -      0
======================================================
          Total:|70.1%    134|78.2%    55|    -      0
```

## gcov
```
$ gcov main
# or gcov main.cpp
# or gcov main.gcno
# gcov main.cpp
$ ls *.gcov
__locale.gcov
__string.gcov
ios.gcov
iterator.gcov
locale.gcov
main.cpp.gcov
memory.gcov
ostream.gcov
streambuf.gcov
string.gcov
type_traits.gcov
```

```
gcov -f main.cpp | ./gcov_filter.py /dev/stdin -v --function-format=mac| c++filt
```

* maybe you can use `gtags -v`, `global -f xxx` to search func location?

## Q&A
### 異なるプラットフォームでの実行は?
例えば，クロスビルドしたとき異なるplatformでcoverageを集計したい場合には、build時の絶対パスに一致するように`.gcda`ファイルが自動生成される

1. scpで転送
2. 対象ファイルを実行する
3.  xxx.gcdaファイルが勝手にbuild時のファイル構成で作成
  * ディレクトリが存在しない場合には自動的に作成される
4. 手元にscpで転送し，通常時と同様に計測

リモートで自動作成された際に，対象ディレクトリが存在しなかった場合には`.gcda`ファイルのみが生成されるので，
フィルター不要で単純に`scp`で取得すれば問題なく，手元に配置される
```
scp -r $remote:"$PWD" "$PWD/.."
```
(`/..`を付加すると，現在のパスを基準にして意図通りに`.gcda`ファイルが配置される)

### カバレッジで未使用のファイル群が表示されない場合
* 実行したファイルのgcdaのみを参照しているため
  * 各build済みのファイルに対応するgcdaを個別に指定して，`.info`ファイルを生成する必要がある

### lcov: WARNING: negative counts found in tracefile at xxx.info
```
Writing data to yyy.info
lcov: ERROR: cannot write to yyy.info!
```
となり，書き込めなくなる

おそらく，最適化によって，カウンタの値の不整合が発生する現象は，下記の方法で無理やり回避可能
```
sed -i -E 's/,\-([0-9]+)$/,\1/g' xxx.info
```

* [Lcov inconsistent coverage \- Stack Overflow]( https://stackoverflow.com/questions/25585895/lcov-inconsistent-coverage )
* [Linux Test Project / Re: \[Ltp\-coverage\] Regarding warning of lcov]( https://sourceforge.net/p/ltp/mailman/message/33951540/ )

### 一度に.infoファイルを処理すると正常に出力できないケースがある
原因は不明
```
for INFO_FILE in $(ls *.info)
do
    if [[ -s $INFO_FILE ]]; then
        echo "# archive $INFO_FILE"
        lcov -r $INFO_FILE -o filtered_$INFO_FILE '/usr/*' '/opt/*' '*/external/*' '*/test/*'
    fi
done
lcov $(ls filtered_*.info | awk '{printf "-a %s ", $0}') -o ${SCRIPT_DIR}/all.info
```

### ファイルが見つからないエラー発生
* `-b`オプションを適切に設定し，各ビルドをしたディレクトリで`lcov`コマンドを個別に実行する
```
lcov -c -d ./xxx -b ./xxx -o xxx.info
```

### カバレッジ対象の実行ファイルは同時実行させてはならない気がする...?
`.gcda`への並列書き込みの挙動は...?

## FYI
* [GoogleTest \+ CMakeでC\+\+の実践的なユニットテスト環境を構築する：その２（カバレッジ表示） \- Qiita]( https://qiita.com/imasaaki/items/0021d1ef14660184f396 )
* [gcov の使い方 \- まめめも]( https://mametter.hatenablog.com/entry/20090721/p1 )
  * おすすめ!
* [gcovの要約情報から欲しいものだけ抜き出す \- 新・日々録 by TRASH BOX@Eel]( https://eel3.hatenablog.com/entry/20110417/1303048082 )
  * おすすめ!
