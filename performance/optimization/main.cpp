#include <iostream>
#include <string>

#include "f.hpp"

#ifdef SAME_FILE
int f(int x, int n) {
  if (x == 0 && n == 0) {
    return 0;
  }
  return x;
}
#endif

int main(int argc, char* argv[]) {
  float sum = 0;
  for (int i = 0; i < 100000000; i++) {
    for (int j = 0; j < 100000000; j++) {
      sum = f(0, 0);
    }
  }
  std::printf("%f\n", sum);
  return 0;
}
