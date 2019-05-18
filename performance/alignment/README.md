# alignment

## NOTE
* `std::vector`や`new`でmemoryを確保するときの型とalignmentの関係
  * 基本的に，`64B`のalignmentで確保されている
  * 型のサイズが小さいほど，`64B`で確保される確率が低い
  * `std::vector`や`new`である程度確保サイズが大きい場合には有意差はなさそう

```
small_n:100
vector: int8_t alignment: 8, cnt:100.0%
vector: int8_t alignment:16, cnt:100.0%
vector: int8_t alignment:32, cnt: 55.0%
vector: int8_t alignment:64, cnt: 27.0%
vector:int16_t alignment: 8, cnt:100.0%
vector:int16_t alignment:16, cnt:100.0%
vector:int16_t alignment:32, cnt: 49.0%
vector:int16_t alignment:64, cnt: 25.0%
vector:int32_t alignment: 8, cnt:100.0%
vector:int32_t alignment:16, cnt:100.0%
vector:int32_t alignment:32, cnt: 50.0%
vector:int32_t alignment:64, cnt: 26.0%
vector:int64_t alignment: 8, cnt:100.0%
vector:int64_t alignment:16, cnt:100.0%
vector:int64_t alignment:32, cnt: 49.0%
vector:int64_t alignment:64, cnt: 27.0%
   new: int8_t alignment: 8, cnt:100.0%
   new: int8_t alignment:16, cnt:100.0%
   new: int8_t alignment:32, cnt: 25.0%
   new: int8_t alignment:64, cnt:  2.0%
   new:int16_t alignment: 8, cnt:100.0%
   new:int16_t alignment:16, cnt:100.0%
   new:int16_t alignment:32, cnt: 25.0%
   new:int16_t alignment:64, cnt:  2.0%
   new:int32_t alignment: 8, cnt:100.0%
   new:int32_t alignment:16, cnt:100.0%
   new:int32_t alignment:32, cnt: 30.0%
   new:int32_t alignment:64, cnt:  3.0%
   new:int64_t alignment: 8, cnt:100.0%
   new:int64_t alignment:16, cnt:100.0%
   new:int64_t alignment:32, cnt: 30.0%
   new:int64_t alignment:64, cnt:  3.0%
big_n:10000
vector: int8_t alignment: 8, cnt:100.0%
vector: int8_t alignment:16, cnt:100.0%
vector: int8_t alignment:32, cnt: 99.4%
vector: int8_t alignment:64, cnt: 99.0%
vector:int16_t alignment: 8, cnt:100.0%
vector:int16_t alignment:16, cnt:100.0%
vector:int16_t alignment:32, cnt: 99.4%
vector:int16_t alignment:64, cnt: 99.0%
vector:int32_t alignment: 8, cnt:100.0%
vector:int32_t alignment:16, cnt:100.0%
vector:int32_t alignment:32, cnt: 99.3%
vector:int32_t alignment:64, cnt: 99.1%
vector:int64_t alignment: 8, cnt:100.0%
vector:int64_t alignment:16, cnt:100.0%
vector:int64_t alignment:32, cnt: 99.3%
vector:int64_t alignment:64, cnt: 99.0%
   new: int8_t alignment: 8, cnt:100.0%
   new: int8_t alignment:16, cnt:100.0%
   new: int8_t alignment:32, cnt: 99.1%
   new: int8_t alignment:64, cnt: 98.7%
   new:int16_t alignment: 8, cnt:100.0%
   new:int16_t alignment:16, cnt:100.0%
   new:int16_t alignment:32, cnt: 99.1%
   new:int16_t alignment:64, cnt: 98.8%
   new:int32_t alignment: 8, cnt:100.0%
   new:int32_t alignment:16, cnt:100.0%
   new:int32_t alignment:32, cnt: 99.1%
   new:int32_t alignment:64, cnt: 99.1%
   new:int64_t alignment: 8, cnt:100.0%
   new:int64_t alignment:16, cnt:100.0%
   new:int64_t alignment:32, cnt: 99.1%
   new:int64_t alignment:64, cnt: 98.7%
```
