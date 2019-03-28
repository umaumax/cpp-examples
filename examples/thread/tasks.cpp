#include <algorithm>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

int main(int argc, char* argv[]) {
  if (argc < 3) {
    std::cerr << argv[0] << " <total_task_num> <parallel_num>" << std::endl;
    return 0;
  }
  const int total_task_num = std::stoi(argv[1]);
  const int parallel_num   = std::stoi(argv[2]);
  if (total_task_num < 0) {
    std::cerr << "total_task_num must be >=0" << std::endl;
    return 1;
  }
  if (parallel_num <= 0) {
    std::cerr << "parallel_num must be >0" << std::endl;
    return 1;
  }
  const int index_step             = total_task_num / parallel_num;
  const int extra_task_index_bound = total_task_num % parallel_num;

  int index_start = 0;
  int index_end;
  std::vector<std::thread> ths(parallel_num);
  for (size_t i = 0; i < ths.size(); i++) {
    int task_num = index_step + (i < extra_task_index_bound ? 1 : 0);
    index_end    = index_start + task_num;
    ths[i]       = std::thread(
        [&, i](int index_start, int index_end) {
          for (int index = index_start; index < index_end; index++) {
            std::cout << "i=" << i << ", index=" << index << std::endl;
          }
        },
        index_start, index_end);
    index_start = index_end;
    // NOTE: easy for see stdout
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  };
  for (auto&& th : ths) {
    th.join();
  }
  return 0;
}
