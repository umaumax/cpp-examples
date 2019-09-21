#include <iostream>
#include <string>

void a() {
#ifdef FLAG
  std::cout << "a: FLAG_ON" << std::endl;
#undef FLAG
#else
  std::cout << "a: FLAG_OFF" << std::endl;
#endif
}
