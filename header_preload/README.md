

```
g++ -std=c++11 main.cpp $(pkg-config opencv4 --libs) $(pkg-config opencv4 --cflags)
```

-I.としなくても./のほうが強い...

```
g++ -I. -std=c++11 main.cpp
```

header preloadの場合，そのヘッダの中身をすべてreplaceできなければエラー...?


