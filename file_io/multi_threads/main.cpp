#include <iostream>
#include <string>
#include <thread>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int Fibonacci(int n) {
  switch (n) {
    case 0:
      return 0;
    case 1:
      return 1;
    default:
      return Fibonacci(n - 2) + Fibonacci(n - 1);
  }
}

int Write(const std::string& filepath, const std::vector<char>& data) {
  std::ofstream ofs;
  ofs.open(filepath, std::ios::out | std::ios::binary | std::ios::trunc);
  if (!ofs) {
    std::cerr << "std::ofstream.open(" << filepath << ") failed" << std::endl;
    return 1;
  }
  ofs.write(reinterpret_cast<const char*>(data.data()), data.size());
  return 0;
}

int main(int argc, char* argv[]) {
  unsigned long long size     = 10 * 1024 * 1024;
  unsigned long long calc_num = 20;
  if (argc > 1) {
    std::string arg1(argv[1]);
    if (arg1 == "-h" || arg1 == "-help" || arg1 == "--help") {
      std::cerr << argv[0] << " <size> <calc num(fibonacci value)>"
                << std::endl;
      return 0;
    }
    size = std::stoull(arg1);
    if (argc > 2) {
      calc_num = std::stoull(argv[2]);
    }
  }
  std::vector<char> vec(size, 0x01);
  std::string filepath = ".out";

  std::thread th;

  th = std::thread([&]() {
    std::chrono::system_clock::time_point write_clock_start =
        std::chrono::system_clock::now();
    int ret = Write(filepath, vec);
    if (ret) {
      std::cerr << "Write " << filepath << " failed" << std::endl;
      return ret;
    }
    double elapsed_time =
        std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::system_clock::now() - write_clock_start)
            .count();
    std::cout << "write elapsed time:" << elapsed_time << "[us]" << std::endl;
    return 0;
  });

  std::chrono::system_clock::time_point calc_clock_start =
      std::chrono::system_clock::now();
  int val             = Fibonacci(calc_num);
  double elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(
                            std::chrono::system_clock::now() - calc_clock_start)
                            .count();
  std::cout << "calc elapsed time:" << elapsed_time << "[us]" << std::endl;

  th.join();
  std::cout << "dummy val:" << val << std::endl;
  return 0;
}
