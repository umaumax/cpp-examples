#include <fstream>
#include <iostream>
#include <string>
#include <vector>

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
  unsigned long long size = 10 * 1024 * 1024;
  if (argc > 1) {
    std::string arg1(argv[1]);
    if (arg1 == "-h" || arg1 == "-help" || arg1 == "--help") {
      std::cerr << argv[0] << " <size>" << std::endl;
      return 0;
    }
    size = std::stoull(arg1);
  }
  std::vector<char> vec(size, 0x01);
  std::string filepath = ".out";

  std::chrono::system_clock::time_point clock_start =
      std::chrono::system_clock::now();
  {
    int ret = Write(filepath, vec);
    if (ret) {
      std::cerr << "Write " << filepath << " failed" << std::endl;
      return ret;
    }
  }

  double elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(
                            std::chrono::system_clock::now() - clock_start)
                            .count();
  std::cout << "elapsed time:" << elapsed_time << "[us]" << std::endl;
  return 0;
}
