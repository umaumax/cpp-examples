# file_io

## how to build
```
g++ -std=c++11 write.cpp -o write

g++ -std=c++11 read.cpp -o read
```

## how to run
```
./write 10000000
# output filename is `.out`
./read .out
```

## NOTE
* writeはcpp styleでも特に速度低下はない?(書き込みサイズが既知だから?)
* readはc styleでないcpp styleでは遅い(size指定すれば高速になった)

## FYI
* [規格違反にならないC言語でのファイルサイズ取得 ｜POSIX fstat 関数使用 \| 組込みエンジニアの思うところ]( https://nagayasu-shinya.com/cpp-get-filesize/ )
* [C/C\+\+のファイル操作速度比較 \- Qiita]( https://qiita.com/kurasho/items/c2abb79b4db516491fa4 )
