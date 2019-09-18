# map

* mapのkeyのアクセス速度比較

## how to run
```
$ ./a.out 10 1000000
```

## result
* mapのsizeが小さい方がアクセス速度が高速
  * cache?
  * 内部のデータ構造上の理由?
  * mapのkeyとなるvectorやarrayの要素数が大きくなるとhash?の計算に時間がかかるため，アクセス速度は遅くなる

```
loop num = 1000000
ary_map size = 10
vec_map size = 10
 array:       12662us
vector:       35182us

loop num = 1000000
ary_map size = 100
vec_map size = 100
 array:       21824us
vector:       53209us

loop num = 1000000
ary_map size = 1000
vec_map size = 1000
 array:       32825us
vector:       66036us

# 結果が安定せず(メモリ確保依存?)
loop num = 1000000
ary_map size = 5000
vec_map size = 5000
 array:      100849us
vector:       88430us

loop num = 1000000
ary_map size = 10000
vec_map size = 10000
 array:      117590us
vector:      115654us
```

## FYI
* [c\+\+ \- Making a template work with both std::map and std::unordered\_map \- Stack Overflow]( https://stackoverflow.com/questions/33315837/making-a-template-work-with-both-stdmap-and-stdunordered-map )
* [qt \- C\+\+ unordered\_map fail when used with a vector as key \- Stack Overflow]( https://stackoverflow.com/questions/10405030/c-unordered-map-fail-when-used-with-a-vector-as-key )

* `map<array>`: OK
* `map<vector>`: OK
* `unordered_map<array>`: NG
* `unordered_map<vector>`: NG
