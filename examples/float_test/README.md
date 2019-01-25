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
# g++-7
$ ./a.out
main.cpp:16 RAISED FLOATING-POINT OVERFLOW: d xx = inf
main.cpp:19 FE_ALL_EXCEPT SUCCESS
main.cpp:31 FE_ALL_EXCEPT SUCCESS
main.cpp:42 RAISED FLOATING-POINT UNDERFLOW: f f_underflow = 0.000000
main.cpp:44 FE_ALL_EXCEPT SUCCESS
# clang 7.0.1
$ ./a.out
main.cpp:16 RAISED FLOATING-POINT OVERFLOW: d xx = inf
main.cpp:19 FE_ALL_EXCEPT SUCCESS
main.cpp:28 RAISED FLOATING-POINT OVERFLOW: f f_overflow = inf
main.cpp:31 FE_ALL_EXCEPT SUCCESS
main.cpp:42 RAISED FLOATING-POINT UNDERFLOW: f f_underflow = 0.000000
main.cpp:44 FE_ALL_EXCEPT SUCCESS
```
### Ubuntu
```
main.cpp:16 RAISED FLOATING-POINT OVERFLOW: d xx = inf
main.cpp:19 FE_ALL_EXCEPT SUCCESS
main.cpp:28 RAISED FLOATING-POINT OVERFLOW: f f_overflow = inf
main.cpp:31 FE_ALL_EXCEPT SUCCESS
main.cpp:42 RAISED FLOATING-POINT UNDERFLOW: f f_underflow = 0.000000
main.cpp:44 FE_ALL_EXCEPT SUCCESS
```
