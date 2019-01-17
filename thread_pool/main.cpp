#include <iostream>
#include <string>

#include "thread_pool.hpp"

int main(int argc, char* argv[]) {
  constexpr int n_thread = 4;
  constexpr int n_loop   = 10;

  //   auto&& poolp = std::shared_ptr<ThreadPool>(new ThreadPool(n_thread));
  auto&& poolp = std::shared_ptr<ThreadPool>(new ThreadPool(n_thread, [](int i) { std::printf("[%d] init\n", i); }));
  auto&& pool  = *poolp;

  std::vector<std::future<std::string>> rets(n_loop);
  for (int i = 0; i < n_loop; i++) {
    rets[i] = pool.PushTask([&, i]() {
      std::printf("[%d] run\n", i);
      return std::string("[" + std::to_string(i) + "] ret");
    });
  }
  // NOTE: join() with return value
  for (int i = 0; i < n_loop; i++) {
    auto ret = rets[i].get();
    std::cout << ret << std::endl;
  }

  {
    auto&& poolp = std::shared_ptr<ThreadPool>(new ThreadPool(n_thread, [](int i) { std::printf("[%d] init\n", i); }));
    auto&& pool  = *poolp;
    std::printf("sleep func for loop end\n");
    for (int i = 0; i < n_loop; i++) {
      pool.PushTask([&, i]() {
        std::printf("[%d] run sleep start\n", i);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::printf("[%d] run sleep end\n", i);
        return std::string("[" + std::to_string(i) + "] ret");
      });
    }
    std::printf("sleep func for loop end\n");
  }
  return 0;
}
