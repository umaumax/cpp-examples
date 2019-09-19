# stopwatch library

* 特定関数内で完結する便利な簡易パフォーマンス計測ライブラリ

* time unit is `us`, but print unit is `ms`
* multi-thread非対応
* 関数内で宣言可能にするために`template`不使用

## how to use
* `static`で変数宣言
* `Start`,`Stop`メソッドを利用する
* `Clear`メソッドを呼ばない限り，時間情報が蓄積される
* 各メソッドは文字列をキーとして扱うため，同一キーを設定する必要がある

```cpp
static StopWatch stopwatch;

stopwatch.Clear("for-loop");
stopwatch.Start("total");
int sum = 0;
for (int i = 0; i < n; i++) {
  stopwatch.Start("for-loop");
  sum += i;
  stopwatch.Stop("for-loop");
}
stopwatch.Stop("total");

// NOTE: you can print each key or all keys
//   stopwatch.Print("total");
//   stopwatch.Print("for-loop");
stopwatch.PrintAll();
```
