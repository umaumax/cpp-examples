#include <cerrno>
#include <clocale>
#include <cstring>
#include <iomanip>
#include <iostream>

int main(int argc, char const* argv[]) {
  std::setlocale(LC_MESSAGES, "en_US.utf8");
  for (int i = -1; i < 135; i++) {
    std::cout << "[" << std::setw(3) << std::setfill(' ') << i
              << "]:" << std::strerror(i) << std::endl;
  }
  return 0;
}
