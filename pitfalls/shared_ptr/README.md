# shared_ptr pitfalls

``` bash
$ g++ -std=c++11 shared_ptr_main.cpp -lpthread
$ ./a.out
AsyncHoge
Hoge Constructor
Hoge Destructor
Hello Hoge: state = 1

AsyncSharedNGHoge
Hoge Constructor
shared_hoge count = 2
[thread]shared_hoge count = 2
Hoge Destructor
Hello Hoge: state = 1

AsyncSharedHoge
Hoge Constructor
shared_hoge count = 2
[thread]shared_hoge count = 3
Hello Hoge: state = constructor called
Hoge Destructor
```
