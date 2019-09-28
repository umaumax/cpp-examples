#include <algorithm>
#include <array>
#include <functional>
#include <iostream>
#include <random>
#include <string>

int main(int argc, char* argv[]) {
  constexpr int N = 10;
  int top_n       = 3;

  std::array<int, N> input;
  for (int i = 0; i < N; i++) {
    input[i] = i;
  }

  std::random_device seed_gen;
  std::mt19937 engine(seed_gen());
  std::shuffle(input.begin(), input.end(), engine);

  std::cout << "input data:" << std::endl;
  for (auto&& e : input) {
    std::cout << e << ", ";
  }
  std::cout << std::endl;

  std::partial_sort(input.begin(), input.begin() + top_n, input.end());
  std::cout << "std::partial_sort by " << top_n << std::endl;
  for (auto&& e : input) {
    std::cout << e << ", ";
  }
  std::cout << std::endl;

  for (int i = 0; i < top_n; i++) {
    if (input[i] != i) {
      std::cout << "input[" << i << "] != " << i << std::endl;
    }
  }
  return 0;
}
