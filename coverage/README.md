# coverage

## install
### darwin
```
brew install lcov
```

## how to run
1. `-O0`,`-g`,`--coverage`付きのオプションでビルド
2. __実行ファイルを実行__
  * 複数パターンを連続で実行すればカウンタが累積する
3. `lcov`コマンド実行
4. `genhtml`コマンド実行

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

## FYI
* [GoogleTest \+ CMakeでC\+\+の実践的なユニットテスト環境を構築する：その２（カバレッジ表示） \- Qiita]( https://qiita.com/imasaaki/items/0021d1ef14660184f396 )

