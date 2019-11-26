# odr-use

```cpp
#include <vector>

class A {
 public:
  static constexpr int x = 123;
};

// NOTE: S/s The symbol is in an uninitialized or zero-initialized data section for small objects.
// constexpr int A::x; // -> S A::x by nm command

int main(int argc, char* argv[]) {
  std::vector<int> v;
  v.emplace_back(A::x);
  return 0;
}
```

```
$ ./build.sh
clang version 9.0.0 (tags/RELEASE_900/final)
Undefined symbols for architecture x86_64:
  "A::x", referenced from:
      _main in main-733c34.o
ld: symbol(s) not found for architecture x86_64
clang-9: error: linker command failed with exit code 1 (use -v to see invocation)
CXX: clang++, opt: -std=c++11 -O0: NG
clang version 9.0.0 (tags/RELEASE_900/final)
Undefined symbols for architecture x86_64:
  "A::x", referenced from:
      _main in main-3537a9.o
ld: symbol(s) not found for architecture x86_64
clang-9: error: linker command failed with exit code 1 (use -v to see invocation)
CXX: clang++, opt: -std=c++11 -O1: NG
clang version 9.0.0 (tags/RELEASE_900/final)
CXX: clang++, opt: -std=c++11 -O2: OK
clang version 9.0.0 (tags/RELEASE_900/final)
CXX: clang++, opt: -std=c++11 -O3: OK
Apple LLVM version 9.1.0 (clang-902.0.39.2)
Undefined symbols for architecture x86_64:
  "A::x", referenced from:
      _main in main-be0d76.o
ld: symbol(s) not found for architecture x86_64
clang: error: linker command failed with exit code 1 (use -v to see invocation)
CXX: g++, opt: -std=c++11 -O0: NG
Apple LLVM version 9.1.0 (clang-902.0.39.2)
Undefined symbols for architecture x86_64:
  "A::x", referenced from:
      _main in main-165ca0.o
ld: symbol(s) not found for architecture x86_64
clang: error: linker command failed with exit code 1 (use -v to see invocation)
CXX: g++, opt: -std=c++11 -O1: NG
Apple LLVM version 9.1.0 (clang-902.0.39.2)
Undefined symbols for architecture x86_64:
  "A::x", referenced from:
      _main in main-718984.o
ld: symbol(s) not found for architecture x86_64
clang: error: linker command failed with exit code 1 (use -v to see invocation)
CXX: g++, opt: -std=c++11 -O2: NG
Apple LLVM version 9.1.0 (clang-902.0.39.2)
Undefined symbols for architecture x86_64:
  "A::x", referenced from:
      _main in main-5da149.o
ld: symbol(s) not found for architecture x86_64
clang: error: linker command failed with exit code 1 (use -v to see invocation)
CXX: g++, opt: -std=c++11 -O3: NG
g++-7 (Homebrew GCC 7.4.0_2) 7.4.0
Undefined symbols for architecture x86_64:
  "A::x", referenced from:
      _main in ccH5yumM.o
ld: symbol(s) not found for architecture x86_64
collect2: error: ld returned 1 exit status
CXX: g++-7, opt: -std=c++11 -O0: NG
g++-7 (Homebrew GCC 7.4.0_2) 7.4.0
Undefined symbols for architecture x86_64:
  "A::x", referenced from:
      _main in cc7OUBYI.o
ld: symbol(s) not found for architecture x86_64
collect2: error: ld returned 1 exit status
CXX: g++-7, opt: -std=c++11 -O1: NG
g++-7 (Homebrew GCC 7.4.0_2) 7.4.0
Undefined symbols for architecture x86_64:
  "A::x", referenced from:
      _main in ccDnZAZn.o
ld: symbol(s) not found for architecture x86_64
collect2: error: ld returned 1 exit status
CXX: g++-7, opt: -std=c++11 -O2: NG
g++-7 (Homebrew GCC 7.4.0_2) 7.4.0
CXX: g++-7, opt: -std=c++11 -O3: OK
g++-9 (Homebrew GCC 9.2.0) 9.2.0
Undefined symbols for architecture x86_64:
  "A::x", referenced from:
      _main in ccsPdxne.o
ld: symbol(s) not found for architecture x86_64
collect2: error: ld returned 1 exit status
CXX: g++-9, opt: -std=c++11 -O0: NG
g++-9 (Homebrew GCC 9.2.0) 9.2.0
Undefined symbols for architecture x86_64:
  "A::x", referenced from:
      _main in cckdQFiP.o
ld: symbol(s) not found for architecture x86_64
collect2: error: ld returned 1 exit status
CXX: g++-9, opt: -std=c++11 -O1: NG
g++-9 (Homebrew GCC 9.2.0) 9.2.0
Undefined symbols for architecture x86_64:
  "A::x", referenced from:
      _main in cc3v34yB.o
ld: symbol(s) not found for architecture x86_64
collect2: error: ld returned 1 exit status
CXX: g++-9, opt: -std=c++11 -O2: NG
g++-9 (Homebrew GCC 9.2.0) 9.2.0
Undefined symbols for architecture x86_64:
  "A::x", referenced from:
      _main in cciJnILX.o
ld: symbol(s) not found for architecture x86_64
collect2: error: ld returned 1 exit status
CXX: g++-9, opt: -std=c++11 -O3: NG
```

mac

| CXX     | version                                      | opt | result |
|---------|----------------------------------------------|-----|--------|
| clang++ | clang version 9.0.0 (tags/RELEASE_900/final) | -O0 | ❌     |
| clang++ | clang version 9.0.0 (tags/RELEASE_900/final) | -O1 | ❌     |
| clang++ | clang version 9.0.0 (tags/RELEASE_900/final) | -O2 | ✔     |
| clang++ | clang version 9.0.0 (tags/RELEASE_900/final) | -O3 | ✔     |
| g++     | Apple LLVM version 9.1.0 (clang-902.0.39.2)  | -O0 | ❌     |
| g++     | Apple LLVM version 9.1.0 (clang-902.0.39.2)  | -O1 | ❌     |
| g++     | Apple LLVM version 9.1.0 (clang-902.0.39.2)  | -O2 | ❌     |
| g++     | Apple LLVM version 9.1.0 (clang-902.0.39.2)  | -O3 | ❌     |
| g++-7   | g++-7 (Homebrew GCC 7.4.0_2) 7.4.0           | -O0 | ❌     |
| g++-7   | g++-7 (Homebrew GCC 7.4.0_2) 7.4.0           | -O1 | ❌     |
| g++-7   | g++-7 (Homebrew GCC 7.4.0_2) 7.4.0           | -O2 | ❌     |
| g++-7   | g++-7 (Homebrew GCC 7.4.0_2) 7.4.0           | -O3 | ✔     |
| g++-9   | g++-9 (Homebrew GCC 9.2.0) 9.2.0             | -O0 | ❌     |
| g++-9   | g++-9 (Homebrew GCC 9.2.0) 9.2.0             | -O1 | ❌     |
| g++-9   | g++-9 (Homebrew GCC 9.2.0) 9.2.0             | -O2 | ❌     |
| g++-9   | g++-9 (Homebrew GCC 9.2.0) 9.2.0             | -O3 | ❌     |

## FYI
* [c\+\+ \- Understanding static constexpr member variables \- Stack Overflow]( https://stackoverflow.com/questions/34053606/understanding-static-constexpr-member-variables )
* [c\+\+ \- Undefined reference to static constexpr char\[\] \- Stack Overflow]( https://stackoverflow.com/questions/8016780/undefined-reference-to-static-constexpr-char )
* [C\+\+ \- 静的クラスのメンバー \| c\+\+ Tutorial]( https://riptutorial.com/ja/cplusplus/example/15150/%E9%9D%99%E7%9A%84%E3%82%AF%E3%83%A9%E3%82%B9%E3%81%AE%E3%83%A1%E3%83%B3%E3%83%90%E3%83%BC )
* [リンク時に関連するルールの話 \- ここは匣]( http://fimbul.hateblo.jp/entry/2014/12/11/000123 )
* [c\+\+ \- What does it mean to "ODR\-use" something? \- Stack Overflow]( https://stackoverflow.com/questions/19630570/what-does-it-mean-to-odr-use-something )
* [翻訳！ODR\(One\-definition rule\) \- Qiita]( https://qiita.com/alphya/items/8a0b0df3abe1e2e3b7d1 )
