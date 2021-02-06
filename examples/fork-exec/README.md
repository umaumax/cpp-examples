# fork-exec

## fork後のpipeなどの後始末の方法
execしたときにdeferは呼ばれないので，明示的にファイルをcloseする必要がある

* [How to make destructor\-friendly call to exec\(\) in C\+\+? \- Stack Overflow]( https://stackoverflow.com/questions/17134884/how-to-make-destructor-friendly-call-to-exec-in-c )
* [FIO22\-C\. プロセスを生成する前にファイルをクローズする]( https://www.jpcert.or.jp/sc-rules/c-fio22-c.html )
* [ERR04\-C\. プログラムの適切な終了方法を選択する]( https://www.jpcert.or.jp/sc-rules/c-err04-c.html )
* [quick\_exit \- cpprefjp C\+\+日本語リファレンス]( https://cpprefjp.github.io/reference/cstdlib/quick_exit.html )
* [Man page of \_EXIT]( https://linuxjm.osdn.jp/html/LDP_man-pages/man2/exit.2.html )
* [c \- What is the difference between using \_exit\(\) & exit\(\) in a conventional Linux fork\-exec? \- Stack Overflow]( https://stackoverflow.com/questions/5422831/what-is-the-difference-between-using-exit-exit-in-a-conventional-linux-fo )

* 例えば，`ulimit -n 30`ほどにして、わざとcloseしないようにすると，`failed pipe:Too many open files`となる
* execの失敗のさせ方は実行できないコマンドのファイルパスを指定すればOK

## セキュアなコマンドの実行方法
* [IPA ISEC　セキュア・プログラミング講座：C/C\+\+言語編　第10章 著名な脆弱性対策：コマンド注入攻撃対策]( https://www.ipa.go.jp/security/awareness/vendor/programmingv2/contents/c909.html )
より、`execve(path, argv, envp)`を利用する

> * `v`: 起動するプログラムに与える引数をベクトル argv として与える。引数の並びの最後は argv[n] == 0（NULLでもよい）で示す
> * `e`: 環境変数を現在のプログラムから継承させず、環境変数プール envp を与える

* [c\+\+ \- How to pass a vector of strings to execv \- Stack Overflow]( https://stackoverflow.com/questions/5797837/how-to-pass-a-vector-of-strings-to-execv )

* [Man page of EXEC]( https://linuxjm.osdn.jp/html/LDP_man-pages/man3/exec.3.html )
> exec() 群の関数が復帰するのは、エラーが発生した場合のみである。 返り値は -1 で、 errno にエラーの内容がセットされる。

## stdout/stderrへの出力方法
* [fork execしたプロセスの標準出力をリダイレクトする \- Qiita]( https://qiita.com/yohm/items/2c372a61f1b5d0694ec3 )
* [c \- Redirecting exec output to a buffer or file \- Stack Overflow]( https://stackoverflow.com/questions/2605130/redirecting-exec-output-to-a-buffer-or-file )
* [linux \- How to combine stderr and stdout to single C\+\+ string? \- Stack Overflow]( https://stackoverflow.com/questions/53401095/how-to-combine-stderr-and-stdout-to-single-c-string )

## FYI
* [cpp\-subprocess/subprocess\.hpp at master · tsaarni/cpp\-subprocess]( https://github.com/tsaarni/cpp-subprocess/blob/master/include/subprocess.hpp )
