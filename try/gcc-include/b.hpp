#include <iostream>
#include <string>

void b() {
#ifdef FLAG
  std::cout << "b: FLAG_ON" << std::endl;
#undef FLAG
#else
  std::cout << "b: FLAG_OFF" << std::endl;
#endif
}
