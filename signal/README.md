# signal example

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
