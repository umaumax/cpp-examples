# lz4

[LZ4 \- Extremely fast compression]( https://lz4.github.io/lz4/ )
[lz4/lz4: Extremely Fast Compression algorithm]( https://github.com/lz4/lz4 )

* license: BSD License
* LZ4はLinuxカーネル 3.11にもネイティブに実装
* block stream compressにも対応(プリセット辞書を利用して，blockごとに連続して圧縮する)

## compress
* `LZ4_compress`: 非推奨
* `LZ4_compress_default`: 
* `LZ4_compress_fast`: 最後に引数の`acceleration=1`とすると`LZ4_compress_default`と同等の処理

## decompress
* `LZ4_uncompress`: 非推奨
* `LZ4_decompress_safe`
* `LZ4_decompress_fast`: 非推奨

* 解凍後のサイズが不明な場合は複数回に分けて解凍処理
  * [c\+\+ \- LZ4 Decompress Binary Memory Block Of Unknown Size \- Stack Overflow]( https://stackoverflow.com/questions/38315672/lz4-decompress-binary-memory-block-of-unknown-size?rq=1 )

----

## how to install
### Max OS X
```
brew install lz4
```

## how to build
```
g++ -std=c++11 -O3 main.cpp -llz4
```

## FYI
* [LZ4 ブロック圧縮 API の使い方 \- Qiita]( https://qiita.com/dearblue/items/65e8526f47dc10a63f04 )
