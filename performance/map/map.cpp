#include <array>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

template <template <typename...> class Map, typename T, typename U, int N>
void run(int elem_num, int loop_num) {
  std::array<T, N> ary;
  std::vector<T> vec(N);

  Map<std::array<T, N>, U> ary_map;
  Map<std::vector<T>, U> vec_map;

  for (int i = 0; i < elem_num; i++) {
    for (int j = 0; j < N; j++) {
      ary[j] = static_cast<T>(i);
    }
    ary_map[ary] = 0;
  }

  for (int i = 0; i < elem_num; i++) {
    for (int j = 0; j < N; j++) {
      vec[j] = static_cast<T>(i);
    }
    vec_map[vec] = 0;
  }

  std::cout << "loop num = " << loop_num << std::endl;
  std::cout << "ary_map size = " << ary_map.size() << std::endl;
  std::cout << "vec_map size = " << vec_map.size() << std::endl;

  {
    std::chrono::system_clock::time_point ary_map_start =
        std::chrono::system_clock::now();

    for (int i = 0; i < loop_num; i++) {
      ary_map[ary] = 0;
    }

    double ary_map_elapsed =
        std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::system_clock::now() - ary_map_start)
            .count();
    std::cout << " array:" << std::setw(12) << std::right << ary_map_elapsed
              << "us" << std::endl;
  }

  {
    std::chrono::system_clock::time_point vec_map_start =
        std::chrono::system_clock::now();

    for (int i = 0; i < loop_num; i++) {
      vec_map[vec] = 0;
    }

    double vec_map_elapsed =
        std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::system_clock::now() - vec_map_start)
            .count();
    std::cout << "vector:" << std::setw(12) << std::right << vec_map_elapsed
              << "us" << std::endl;
  }

  std::vector<U> elems(elem_num);
  {
    std::chrono::system_clock::time_point vec_map_start =
        std::chrono::system_clock::now();

    for (int i = 0; i < loop_num; i++) {
      for (int j = 0; j < N; j++) {
        vec[j] = static_cast<T>(i);
      }
      elems[0] = i;
    }

    double vec_map_elapsed =
        std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::system_clock::now() - vec_map_start)
            .count();
    std::ofstream("/dev/null") << elems[0];
    std::cout << "vector linear access:" << std::setw(12) << std::right
              << vec_map_elapsed << "us" << std::endl;
  }
}

int main(int argc, char* argv[]) {
  constexpr int N = 6;

  int elem_num = 1000;
  int loop_num = 1000000;

  if (argc > 1) {
    std::vector<std::string> args(argc);
    for (int i = 0; i < argc; i++) {
      args[i] = std::string(argv[i]);
    }
    if (args[1] == "-h" || args[1] == "--help") {
      std::cerr << args[0] << " [elem_num] [loop_num]" << std::endl;
      return 1;
    }
    elem_num = std::stoi(args[1]);
    if (argc > 2) {
      loop_num = std::stoi(args[2]);
    }
  }

  std::cout << "map" << std::endl;
  run<std::map, int, int, N>(elem_num, loop_num);
  // NOTE: unordered_map cannot treat unhashable type (e.g. array, vector)
  // std::cout << "unordered_map" << std::endl;
  // run<std::unordered_map, int, int, N>(elem_num, loop_num);
  return 0;
}
