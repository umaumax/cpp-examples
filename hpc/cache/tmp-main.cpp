#include <iostream>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {
  int size = 10;
  std::vector<int> vec(size);
  auto start = vec.data();
  auto end   = start + size;
  __builtin___clear_cache((char *)start, (char *)end);
  return 0;
}
