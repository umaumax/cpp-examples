#include <algorithm>
#include <functional>
#include <iostream>
#include <random>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
  constexpr int N = 10;
  int loop_n      = 10;
  int top_n       = 3;

  std::vector<int> input(N);
  for (int i = 0; i < N; i++) {
    input[i] = i;
  }

  // std::cout << "input data:" << std::endl;
  // for (auto&& e : input) {
  // std::cout << e << ", ";
  // }
  // std::cout << std::endl;

  std::cout << "std::partial_sort by " << top_n << std::endl;
  for (int i = 0; i < loop_n; i++) {
    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());
    std::shuffle(input.begin(), input.end(), engine);

    std::partial_sort(input.begin(), input.begin() + top_n, input.end());
    for (auto&& e : input) {
      std::cout << e << ", ";
    }
    std::cout << std::endl;

    for (int i = 0; i < top_n; i++) {
      if (input[i] != i) {
        std::cout << "input[" << i << "] != " << i << std::endl;
      }
    }
  }

  std::cout << "std::nth_element by " << top_n << std::endl;
  for (int i = 0; i < loop_n; i++) {
    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());
    std::shuffle(input.begin(), input.end(), engine);

    std::nth_element(input.begin(), input.begin() + top_n, input.end());
    for (auto&& e : input) {
      std::cout << e << ", ";
    }
    std::cout << std::endl;

    for (int i = 0; i < top_n; i++) {
      if (input[i] > top_n) {
        std::cout << "input[" << i << "] >= " << top_n << std::endl;
      }
    }
  }
  return 0;
}
