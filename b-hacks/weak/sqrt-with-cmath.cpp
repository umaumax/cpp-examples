#include <cmath>
#include <iostream>

extern double sqrt(double x) __attribute__((weak));

int main(int argc, char* argv[]) {
  if (sqrt) {
    std::cout << "sqrt(2)=" << sqrt(2.0) << std::endl;
  } else {
    std::cerr << "no sqrt function!" << std::endl;
    exit(1);
  }
  return 0;
}
