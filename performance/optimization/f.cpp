#include "f.hpp"

#ifndef SAME_FILE
int f(int x, int n) {
  if (x == 0 && n == 0) {
    return 0;
  }
  return x;
}

// NOTE: このfは同一ファイル内に存在しているため最適化可能であり，f()を呼ばずinlineに展開される
int g(int x, int n) { return f(x, 0); }
#endif
