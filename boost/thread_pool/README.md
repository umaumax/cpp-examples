# boost thread_pool

## how to build
```
# Max OS X
g++ -std=c++11 main.cpp -lboost_thread-mt
# Ubuntu 16.04
g++ -std=c++11 main.cpp -lpthread -lboost_thread -lboost_system
```

## FYI
* [スレッドプールの実装方法について]( http://mojavy.com/blog/2014/03/03/implementing-thread-pool/ )
* [\[C\+\+11\]\[Boost\] boost::asio::io\_serviceを別スレッドでrunする – No Coding, No Life\.]( https://ncnl.blog.so-net.ne.jp/2016-02-13 )
* [c\+\+ \- Boost thread error: undefined reference \- Stack Overflow]( https://stackoverflow.com/questions/3584365/boost-thread-error-undefined-reference )
* [c\+\+ \- Boost::ASIO and std::packaged\_task \- Stack Overflow]( https://stackoverflow.com/questions/35611846/boostasio-and-stdpackaged-task )
