# aligned allocator

[std::vectorなどでアラインされた領域を用いる \- koturnの日記]( http://koturn.hatenablog.com/entry/2016/08/12/010000 )

## vector
自作allocatorを利用した`vector`を利用する際には，その`vector`を利用する関数にて，その`vector`をtemplate化することで，利用することができるようになる
```
template <class T>
void f(const T& vec) {}
```
or
```
template <template <class...> class Vector, class Type, class Allocator>
void g(const Vector<Type, Allocator>& vec) {}
```
