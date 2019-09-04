#include <array>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

template <size_t N>
struct VectorData {
  VectorData() { vals = std::vector<int>(N); }
  std::vector<int> vals;
};

template <size_t N>
struct ArrayData {
  ArrayData() { vals.fill(0); }
  std::array<int, N> vals;
};

template <class T>
int Sum(T datas) {
  int val = 0;
  for (auto&& data : datas) {
    for (auto&& e : data.vals) {
      val += e;
    }
  }
  return val;
}

int main(int argc, char* argv[]) {
  constexpr int loop_num  = 1024;
  constexpr int data_num  = 10240;
  constexpr int data_size = 16;
  for (int j = 0; j < 2; j++) {
    std::chrono::system_clock::time_point calc_start =
        std::chrono::system_clock::now();
    if (j == 0) {
      std::cout << "array" << std::endl;
      std::vector<ArrayData<data_size>> array_data_vec(data_num);
      for (int i = 0; i < loop_num; i++) {
        // NOTE: for avoiding optimization
        std::ofstream("/dev/null") << Sum(array_data_vec);
      }
    }
    if (j == 1) {
      std::cout << "vector" << std::endl;
      std::vector<VectorData<data_size>> vector_data_vec(data_num);
      for (int i = 0; i < loop_num; i++) {
        // NOTE: for avoiding optimization
        std::ofstream("/dev/null") << Sum(vector_data_vec);
      }
    }
    double calc_elapsed_time =
        std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::system_clock::now() - calc_start)
            .count();
    std::cout << "calc elapsed time:" << std::setw(10)
              << static_cast<uint64_t>(calc_elapsed_time) << "[us]"
              << std::endl;
  }
  return 0;
}
