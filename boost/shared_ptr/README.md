# shared_ptr(unique_ptr)

## convert boost::shared_ptr to std::shared_ptr
[c\+\+ \- Conversion from boost::shared\_ptr to std::shared\_ptr? \- Stack Overflow]( https://stackoverflow.com/questions/6326757/conversion-from-boostshared-ptr-to-stdshared-ptr )
OK!

## convert std::shared_ptr <-> std::unique_ptr
[c\+\+ \- Does C\+\+11 unique\_ptr and shared\_ptr able to convert to each other's type? \- Stack Overflow]( https://stackoverflow.com/questions/37884728/does-c11-unique-ptr-and-shared-ptr-able-to-convert-to-each-others-type )
> In short, you can easily and efficiently convert a std::unique_ptr to std::shared_ptr but you cannot convert std::shared_ptr to std::unique_ptr.

## return std::unique_ptr
[c\+\+ \- Returning unique\_ptr from functions \- Stack Overflow]( https://stackoverflow.com/questions/4316727/returning-unique-ptr-from-functions )
[unique\_ptr で今風な C\+\+ コードを書こう！！ \- @nojima's blog]( https://nojima.hatenablog.com/entry/2014/12/10/014131 )
[return文と暗黙のムーブと型変換 \- yohhoyの日記]( http://d.hatena.ne.jp/yohhoy/20161016/p1 )

## std::shared_ptr<void>
[shared\_ptr \- cpprefjp C\+\+日本語リファレンス]( https://cpprefjp.github.io/reference/memory/shared_ptr.html )
[shared\_ptr::デストラクタ \- cpprefjp C\+\+日本語リファレンス]( https://cpprefjp.github.io/reference/memory/shared_ptr/op_destructor.html )
> shared_ptr<void>に対して任意の型Yのオブジェクトへのポインタを代入したとしても、Y型のデストラクタが正しく実行される。
