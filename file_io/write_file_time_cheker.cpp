#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

bool write_binary(std::string filepath, std::vector<char>& vec) {
  std::ofstream fout(filepath, std::ios::out | std::ios::binary);
  fout.write((char*)&vec[0], vec.size() * sizeof(vec[0]));
  fout.close();
  return true;
}
int main(int argc, char* argv[]) {
  for (int i = 0; i < 2 * 1024; i++) {
    size_t size = 64 * (i + 1);
    std::vector<char> vec(size);
    std::chrono::system_clock::time_point write_start =
        std::chrono::system_clock::now();
    write_binary("tmp.bin", vec);
    double write_elapsed =
        std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::system_clock::now() - write_start)
            .count();

    printf("size[B], time[us] = %8zu, %12lf\n", size, write_elapsed);
  }
  return 0;
}
