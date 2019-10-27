#include <chrono>
#include <cmath>
#include <cstdio>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

// NOTE: ポイントは関数呼び出しがあるかいなかでは?
// 関数呼び出しがなければ，最適化によって自動的に，次の添字の計算も実行されるようなasmなのでは?

// NOTE: おそらく，absがinline展開される?ので速度は通常記法が高速
// using calc_type = int;
// int calc_type_abs(int x) { return abs(x); }

// NOTE: おそらく，関数として呼び出されるので，擬似並列化がよさげ(stride==2)
// using calc_type = float;
// float calc_type_abs(float x) { return fabs(x); }

using calc_type = double;
double calc_type_abs(double x) { return fabs(x); }

calc_type orig_sum(calc_type* a, calc_type* b, int num) {
  calc_type cost = 0.0;
  for (int n = 0; n < num; n++) {
    cost += calc_type_abs(a[n] - b[n]);
  }
  return cost;
}
#define def_new_sum_func_asc(stride_)                               \
  calc_type new_sum##stride_(calc_type* a, calc_type* b, int num) { \
    constexpr int stride = (stride_);                               \
    calc_type costs[stride];                                        \
    for (int i = 0; i < stride; ++i) {                              \
      costs[i] = 0;                                                 \
    }                                                               \
    int n;                                                          \
    for (n = 0; n <= num - stride; n += stride) {                   \
      for (int i = 0; i < stride; ++i) {                            \
        costs[i] += calc_type_abs(a[n + i] - b[n + i]);             \
      }                                                             \
    }                                                               \
    for (int i = 0; n + i < num; ++i) {                             \
      costs[i] += calc_type_abs(a[n + i] - b[n + i]);               \
    }                                                               \
    calc_type cost = 0;                                             \
    for (int i = 0; i < stride; ++i) {                              \
      cost += costs[i];                                             \
    }                                                               \
    return cost;                                                    \
  }

#define def_new_sum_func_desc(stride_)                              \
  calc_type new_sum##stride_(calc_type* a, calc_type* b, int num) { \
    constexpr int stride = (stride_);                               \
    calc_type costs[stride];                                        \
    for (int i = stride - 1; i >= 0; --i) {                         \
      costs[i] = 0;                                                 \
    }                                                               \
    int n;                                                          \
    for (n = num - 1; n >= stride - 1; n -= stride) {               \
      for (int i = stride - 1; i >= 0; --i) {                       \
        costs[i] += calc_type_abs(a[n - i] - b[n - i]);             \
      }                                                             \
    }                                                               \
    for (int i = n; i >= 0; --i) {                                  \
      costs[i] += calc_type_abs(a[n - i] - b[n - i]);               \
    }                                                               \
    calc_type cost = 0;                                             \
    for (int i = stride - 1; i >= 0; --i) {                         \
      cost += costs[i];                                             \
    }                                                               \
    return cost;                                                    \
  }

// NOTE: 基本的に，配列のアクセスがあるので，ascの方が高速
#define def_new_sum_func(stride_) def_new_sum_func_asc(stride_)

def_new_sum_func(1);
def_new_sum_func(2);
def_new_sum_func(3);
def_new_sum_func(4);
def_new_sum_func(5);
def_new_sum_func(6);
def_new_sum_func(7);
def_new_sum_func(8);

int main(int argc, char* argv[]) {
  if (argc <= 2) {
    std::cerr << "array_num loop_num" << std::endl;
    return 1;
  }
  int array_num = atoi(argv[1]);
  int loop_num  = atoi(argv[2]);
  calc_type* a  = (calc_type*)malloc(sizeof(calc_type) * array_num);
  calc_type* b  = (calc_type*)malloc(sizeof(calc_type) * array_num);

  auto init_data = [&]() {
    for (int i = array_num - 1; i >= 0; --i) {
      a[i] = 2 * i;
      b[i] = i;
    }
  };

  std::map<std::string, std::function<calc_type(calc_type*, calc_type*, int)>>
      funcmap = {
          {"0 orig", orig_sum},  //
          {"1 new1", new_sum1},  //
          {"2 new2", new_sum2},  //
          {"3 new3", new_sum3},  //
          {"4 new4", new_sum4},  //
          {"5 new5", new_sum5},  //
          {"6 new6", new_sum6},  //
          {"7 new7", new_sum7},  //
          {"8 new8", new_sum8},  //
      };
  for (auto& x : funcmap) {
    auto& key = x.first;
    auto& f   = x.second;
    init_data();
    std::chrono::system_clock::time_point start =
        std::chrono::system_clock::now();
    calc_type ret = 0.0;
    for (int i = 0; i < loop_num; i++) {
      ret += f(a, b, array_num);
    }
    double elapsed = std::chrono::duration_cast<std::chrono::microseconds>(
                         std::chrono::system_clock::now() - start)
                         .count();
    std::printf("%s result[%s] %lf usec\n", key.c_str(),
                std::to_string(ret).c_str(), elapsed);
  }
  return 0;
}
