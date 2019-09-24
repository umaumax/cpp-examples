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

----

## floor and cast(int)

```
g++ -std=c++11 -O3 floor_and_cast.cpp -o floor_and_cast
```

```
$ ./floor_and_cast
val:-2.2 int(-2.2) = -2 != floor(-2.2) = -3
val:-2.1 int(-2.1) = -2 != floor(-2.1) = -3
val:-2 
val:-1.9 int(-1.9) = -1 != floor(-1.9) = -2
val:-1.8 int(-1.8) = -1 != floor(-1.8) = -2
val:-1.7 int(-1.7) = -1 != floor(-1.7) = -2
val:-1.6 int(-1.6) = -1 != floor(-1.6) = -2
val:-1.5 int(-1.5) = -1 != floor(-1.5) = -2
val:-1.4 int(-1.4) = -1 != floor(-1.4) = -2
val:-1.3 int(-1.3) = -1 != floor(-1.3) = -2
val:-1.2 int(-1.2) = -1 != floor(-1.2) = -2
val:-1.1 int(-1.1) = -1 != floor(-1.1) = -2
val:-1 
val:-0.9 int(-0.9) = 0 != floor(-0.9) = -1
val:-0.8 int(-0.8) = 0 != floor(-0.8) = -1
val:-0.7 int(-0.7) = 0 != floor(-0.7) = -1
val:-0.6 int(-0.6) = 0 != floor(-0.6) = -1
val:-0.5 int(-0.5) = 0 != floor(-0.5) = -1
val:-0.4 int(-0.4) = 0 != floor(-0.4) = -1
val:-0.3 int(-0.3) = 0 != floor(-0.3) = -1
val:-0.2 int(-0.2) = 0 != floor(-0.2) = -1
val:-0.1 int(-0.1) = 0 != floor(-0.1) = -1
val:0 
val:0.1 
val:0.2 
val:0.3 
val:0.4 
val:0.5 
val:0.6 
val:0.7 
val:0.8 
val:0.9 
val:1 
val:1.1 
val:1.2 
val:1.3 
val:1.4 
val:1.5 
val:1.6 
val:1.7 
val:1.8 
val:1.9 
val:2 
val:2.1 
val:2.2 
```
