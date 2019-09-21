#include <iostream>
#include <string>

#include "b.hpp"

void f();
void a();
void b();

#ifndef DECL_FUNC_F_FLAG
inline void f() {
#ifdef FLAG
  std::cout << "f: FLAG_ON" << std::endl;
#undef FLAG
#else
  std::cout << "f: FLAG_OFF" << std::endl;
#endif
}
#endif

int main(int argc, char* argv[]) {
  f();
  a();
  b();
  return 0;
}
