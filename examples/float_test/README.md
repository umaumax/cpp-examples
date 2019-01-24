# float test

[FE\_DIVBYZERO, FE\_INEXACT, FE\_INVALID, FE\_OVERFLOW, FE\_UNDERFLOW, FE\_ALL\_EXCEPT \- cppreference\.com]( https://ja.cppreference.com/w/cpp/numeric/fenv/FE_exceptions )

## NOTE
* お試しときには`-O0`とすること

## FE_INEXACT
* 割り算でほぼ必ず(加算や乗算はOK)
  * `std::cout << f;`: maybe has division?

## FE_OVERFLOW
Mac OS Xでは起きなかったが，Ubuntuでは起きた(OSというよりかはCPUかな)
```
float result = std::numeric_limits<double>::max();
```

## result
### Max OS X
```
$ ./a.out
main.cpp:18 RAISED FLOATING-POINT OVERFLOW: d xx = inf
main.cpp:21 FE_ALL_EXCEPT SUCCESS
main.cpp:33 FE_ALL_EXCEPT SUCCESS
main.cpp:45 FE_ALL_EXCEPT SUCCESS
```
