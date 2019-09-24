#include <cmath>
#include <iostream>

void f(float x) {
  int ix = static_cast<int>(x);
  int fx = static_cast<int>(floor(x));
  std::cout << "val:" << x << " ";
  if (ix != fx) {
    std::cout << "int(" << x << ") = " << ix << " != floor(" << x
              << ") = " << fx;
  }
  std::cout << std::endl;
}

int main() {
  int n = 45;
  for (int i = 0; i < n; i++) {
    float v = 0.1f * (i - n / 2);
    f(v);
  }
}
