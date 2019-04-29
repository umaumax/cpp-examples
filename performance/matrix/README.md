# bosot serialization

```
# オリジナルの関数での出力を保存
FUNC_TEST_OUTPUT=out FUNC_TEST_SAVE=1 ./a.out
# オリジナルの関数の出力をloadして，それとの演算結果を比較して，テスト
FUNC_TEST_OUTPUT=out FUNC_TEST=1 ./a.out

# ブロッキングを利用した関数のテスト
MATRIX_MUL_TARGET=blocking FUNC_TEST_OUTPUT=out FUNC_TEST=1 ./a.out

FUNC_TEST_OUTPUT=no_gzip_out FUNC_TEST_GZIP="" FUNC_TEST_SAVE=1 ./a.out
MATRIX_MUL_TARGET=blocking FUNC_TEST_OUTPUT=no_gzip_out FUNC_TEST_GZIP="" FUNC_TEST=1 ./a.out
```

## I/O形式
下記の3(+2)種類
* binary
* text
  * wide text
* xml
  * wide text

## array(raw pointer)

[Serialization \- Serialization Wrappers]( https://www.boost.org/doc/libs/1_49_0/libs/serialization/doc/wrappers.html#arrays )

> Note that it doesn't allocated any storage or create any objects

e.g.
### load
```
std::string filename = "tmp.out";
boost::archive::text_iarchive tia(ifs);

std::size_t size = 10;
tia >> size;
int * raw_pointer = new int(size);
tia >> boost::serialization::make_array(raw_pointer, size);
delete[] raw_pointer;
```

### save
```
std::string filename = "tmp.out";
std::fstream ofs(filename, std::fstream::out | std::ios::binary | std::fstream::app);

boost::archive::text_oarchive toa(ofs);
std::size_t size = 10;
int * raw_pointer = new int(size);
toa << size;
toa << boost::serialization::make_array(raw_pointer, size);
delete[] raw_pointer;
```

## binary
[Serialization \- Serialization Wrappers]( https://www.boost.org/doc/libs/1_49_0/libs/serialization/doc/wrappers.html#binaryobjects )

> Note that it doesn't allocated any storage or create any objects. Its sole purpose is to pass the data size and address as pair to the archive class.

binaryで保存しても，arrayで保存しても，ファイルサイズは大きく変化しない

## ifstream to stringstream

```
std::stringstream ss;
ss << ifs.rdbuf();
```
