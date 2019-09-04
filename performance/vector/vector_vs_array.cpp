#include <array>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

namespace std {
template <class T, size_t N>
struct static_vector : std::array<T, N> {
  static_vector() : static_vector(0) {}
  static_vector(int count) : size_(count), capacity_(count) {
    assert((count >= 0 && count <= N) && "initial size must be >=0 && <= N");
  }
  size_t size() { return size_; }
  size_t capacity() { return capacity_; }

  void resize(size_t sz) {
    // TODO: run each element constructor or destructor
    size_     = sz;
    capacity_ = sz;
  };
  void reserve(size_t sz) { capacity_ = sz; }

  template <class X>
  void push_back(X&& x) {
    this->at(size_ - 1) = x;
    size_++;
    capacity_++;
  }
  template <class X>
  void emplace_back(X&& x) {
    push_back(x);
  }
  void clear() { resize(0); }
  int size_;
  int capacity_;
};
}  // namespace std

namespace {
template <size_t N>
struct StaticVectorData {
  StaticVectorData() { vals = std::static_vector<int, N>(N); }
  std::static_vector<int, N> vals;
};
}  // namespace

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

template <class T>
void StaticVectorTest(T vec) {
  vec.data();
  vec.empty();

  vec.size();
  vec.capacity();

  vec.resize(1);
  vec.reserve(1);
  vec.push_back(1);
  vec.emplace_back(1);
  vec.clear();
}
template void StaticVectorTest(std::vector<int>);
template void StaticVectorTest(std::static_vector<int, 1>);

int main(int argc, char* argv[]) {
  constexpr int loop_num  = 1024;
  constexpr int data_num  = 10240;
  constexpr int data_size = 16;
  for (int j = 0; j < 3; j++) {
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
    if (j == 2) {
      std::cout << "static vector" << std::endl;
      std::vector<StaticVectorData<data_size>> vector_data_vec(data_num);
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
