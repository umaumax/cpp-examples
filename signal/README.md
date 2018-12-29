# signal example

## there is two way to control signal handler
1. use sigaction
  - 非同期的な処理
  - 呼び出してはならない処理が存在する(e.g. `printf`, `malloc`)
1. use sigwait
  - 同期的な処理
  - sigactionよりも安全に処理を行うことができる
  - [シグナルハンドラを使わないでシグナルをハンドルする \- memologue]( http://d.hatena.ne.jp/yupo5656/20060114/p1 )

## how to set signal handler
```
{
  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = xxx_handler;
  sigemptyset(&sa.sa_mask);
  sigaction(SIGINT, &sa, NULL);
}
```

スコープやmemset後にも正常に動作してそうな挙動なので，おそらく，一旦セットすればOK
```
memset(&sa, 0, sizeof(sa));
```

## how to get singal handler
```
struct sigaction old_sa;
sigaction(SIGINT, NULL, &old_sa);
xxx_sighandler = old_sa.sa_handler;
```

初期設定では，`(bool)xxx_handler` is false

## easy setting
```
if (signal(SIGINT, xxx_handler) == SIG_ERR) {
  exit(1);
}
```

## how to delete handler?
WIP

## tips
* sigactionでsignalは上書きされる

## FYI
* [c \- Is it valid to have multiple signal handlers for same signal? \- Stack Overflow]( https://stackoverflow.com/questions/17102919/is-it-valid-to-have-multiple-signal-handlers-for-same-signal )
* [C/C\+\+ signal handling]( http://www.yolinux.com/TUTORIALS/C++Signals.html )
* [安全なシグナルハンドラを実装するには　――C/C\+\+セキュアコーディング入門（4）：CodeZine（コードジン）]( https://codezine.jp/article/detail/4700 )
  * malloc()は非同期シグナル安全でない
    * つまり，内部でmallocを呼び出すような関数はNG(もちろん，printfも)
