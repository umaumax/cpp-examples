#include <iostream>
#include <map>
#include <string>
#include <typeinfo>
#include <vector>

#define DECL_vector_alignment_test(type)                                      \
  void vector_alignment_test_##type(int n) {                                  \
    std::map<int, int> m;                                                     \
    std::vector<std::vector<type>> vecs(n);                                   \
    for (int i = 0; i < n; i++) {                                             \
      std::vector<type> v(i * 8 / sizeof(type));                              \
      vecs[i] = v;                                                            \
      for (int i = 0; i < 4; i++) {                                           \
        int align = (8 << i);                                                 \
        m[align] += !(((int64_t)v.data()) % align);                           \
      }                                                                       \
    }                                                                         \
    for (auto& v : m) {                                                       \
      std::printf("vector:%7s alignment:%2d, cnt:%5.1lf%%\n", #type, v.first, \
                  (double)v.second / m[8] * 100.0);                           \
    }                                                                         \
  }
#define DECL_new_alignment_test(type)                                         \
  void new_alignment_test_##type(int n) {                                     \
    std::map<int, int> m;                                                     \
    std::vector<type*> vecs(n);                                               \
    for (int i = 0; i < n; i++) {                                             \
      type* v = new type[i * 8 / sizeof(type)];                               \
      vecs[i] = v;                                                            \
      for (int i = 0; i < 4; i++) {                                           \
        int align = (8 << i);                                                 \
        m[align] += !(((int64_t)v) % align);                                  \
      }                                                                       \
    }                                                                         \
    for (auto& v : vecs) {                                                    \
      delete[] v;                                                             \
    }                                                                         \
    for (auto& v : m) {                                                       \
      std::printf("   new:%7s alignment:%2d, cnt:%5.1lf%%\n", #type, v.first, \
                  (double)v.second / m[8] * 100.0);                           \
    }                                                                         \
  }

DECL_vector_alignment_test(int8_t);
DECL_vector_alignment_test(int16_t);
DECL_vector_alignment_test(int32_t);
DECL_vector_alignment_test(int64_t);
DECL_new_alignment_test(int8_t);
DECL_new_alignment_test(int16_t);
DECL_new_alignment_test(int32_t);
DECL_new_alignment_test(int64_t);

int main(int argc, char* argv[]) {
  int small_n = 100;
  int big_n   = 10000;
  std::cout << "small_n:" << small_n << std::endl;
  vector_alignment_test_int8_t(small_n);
  vector_alignment_test_int16_t(small_n);
  vector_alignment_test_int32_t(small_n);
  vector_alignment_test_int64_t(small_n);
  new_alignment_test_int8_t(small_n);
  new_alignment_test_int16_t(small_n);
  new_alignment_test_int32_t(small_n);
  new_alignment_test_int64_t(small_n);
  std::cout << "big_n:" << big_n << std::endl;
  vector_alignment_test_int8_t(big_n);
  vector_alignment_test_int16_t(big_n);
  vector_alignment_test_int32_t(big_n);
  vector_alignment_test_int64_t(big_n);
  new_alignment_test_int8_t(big_n);
  new_alignment_test_int16_t(big_n);
  new_alignment_test_int32_t(big_n);
  new_alignment_test_int64_t(big_n);
  return 0;
}
