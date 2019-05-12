#include <iostream>
#include <string>

void calc_alignment() {
  std::printf("x & ~(16 - 1) = y\n");
  for (int i = -(16 + 1); i < 16 + 1; i++) {
    std::printf("(x,y)=(%3d,%3d)\n", i, i & ~(16 - 1));
  }
}

int main(int argc, char* argv[]) {
  calc_alignment();
  return 0;
}
