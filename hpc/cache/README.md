# hpc

## TODO
* mmapを利用した場合に`__builtin___clear_cache`を利用する
* メモリサイズとキャッシュフラッシュの処理時間の相関関係を調べる

## cache
### キャッシュラインフラッシュ
* [c\+\+ \- How does \_\_builtin\_\_\_clear\_cache work? \- Stack Overflow]( https://stackoverflow.com/questions/35741814/how-does-builtin-clear-cache-work )
  * gcc依存だが，cross platform
    * しかし，下記の投稿より，`mmap()`されたデータでのみ有効?
  * [How clear and invalidate ARM v7 processor cache from User Mode on Linux 2\.6\.35 \- Stack Overflow]( https://stackoverflow.com/questions/6046716/how-clear-and-invalidate-arm-v7-processor-cache-from-user-mode-on-linux-2-6-35 )
    > I had a similar problem myself. __clear_cache() works, but only if the memory area in question was allocated using mmap() with PROT_EXEC set. Linux will not flush the instruction cache if you provide it with a memory range that comes from regular malloc()ed memory, even if the processor seems to be happy to execute code from malloc()ed memory.
