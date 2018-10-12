# destructor

[C\+\+ destruction order: Calling a field destructor before the class destructor \- Stack Overflow]( https://stackoverflow.com/questions/45331271/c-destruction-order-calling-a-field-destructor-before-the-class-destructor )

output
```
Big destructor2
Small destructor:2-2
Small destructor:2-0
Small destructor:2-3
Big destructor1
Small destructor:1-2
Small destructor:1-0
Small destructor:1-3
```

* classのfieldは上から順番に評価される(通常のブロックでの宣言時と同様)
* classのfieldがshared_ptrのときに，この順番を変更したい場合には`reset(nullptr)`を利用する(`release()`では解放責任自体を放棄するため，注意)

[コンストラクタ、デストラクタが呼ばれる順番 \- 揮発性のメモ２]( https://iww.hateblo.jp/entry/20160923/destructor )

## shared_pointer release and reset
[c\+\+ \- Does unique\_ptr::release\(\) call the destructor? \- Stack Overflow]( https://stackoverflow.com/questions/25609457/does-unique-ptrrelease-call-the-destructor )
