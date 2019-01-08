#include <iostream>
#include <string>

inline int inline_add(int a, int b) {
  static int cnt = 0;
  cnt++;
  return cnt + a + b;
}
int no_inline_add(int a, int b) {
  static int cnt = 0;
  cnt++;
  return cnt + a + b;
}

int main(int argc, char* argv[]) {
  {
    int a = 1;
    int b = 2;
    int c = inline_add(a, b);
  }
  {
    int a = 1;
    int b = 2;
    int c = no_inline_add(a, b);
  }
  return 0;
}
