#include <iostream>
int main() {
  constexpr auto foo = [](int v) { return v * v; };
  std::cout << foo(7) << std::endl;
  return 0;
}
