#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// #define CPP_VERSION

#ifdef CPP_VERSION
int Read(const std::string &filepath, std::vector<char> &data) {
  std::ifstream file(filepath, std::ios::binary);
  if (!file) {
    std::cerr << "std::ifstream.open(" << filepath << ") failed" << std::endl;
    return 1;
  }
  file.unsetf(std::ios::skipws);
  file.seekg(0, std::ios::end);
  std::streampos file_size = file.tellg();
  file.seekg(0, std::ios::beg);
  data.resize(file_size);
  file.read(data.data(), file_size);
  // data.reserve(file_size);
  // data.insert(data.begin(), std::istream_iterator<char>(file),
  // std::istream_iterator<char>());
  return 0;
}
#else
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int Read(const std::string &filepath, std::vector<char> &data) {
  int fd = open(filepath.c_str(), O_RDONLY);
  if (fd == -1) {
    std::cerr << "open(" << filepath << ") failed" << std::endl;
    return 1;
  }

  FILE *file = fdopen(fd, "rb");
  if (file == nullptr) {
    std::cerr << "fdopen(" << filepath << ") failed" << std::endl;
    return 1;
  }

  struct stat stbuf;
  if (fstat(fd, &stbuf) == -1) {
    std::cerr << "fdstat(" << filepath << ") failed" << std::endl;
    return 1;
  }

  int file_size = stbuf.st_size;
  // printf("filesize:%d\n", file_size);

  data.resize(file_size);
  constexpr int read_buf_size = 1024 * 1024;
  size_t offset               = 0;
  while (!std::feof(file) && offset < file_size) {
    int read_size = read_buf_size;
    if (offset + read_buf_size > file_size) {
      read_size = file_size - offset;
    }
    size_t size = std::fread(&data[offset], sizeof(char), read_size, file);
    offset += size;
    // printf("read size=%zu\n", size);
    // printf("next offset=%zu\n", offset);
  }
  // printf("data capacity=%zu\n", data.capacity());
  // printf("data size=%zu\n", data.size());
  int ret = fclose(file);
  if (ret != 0) {
    std::cerr << "fdclose(" << filepath << ") failed" << std::endl;
    return 1;
  }
  return 0;
}
#endif

int main(int argc, char *argv[]) {
  std::string filepath;
  if (argc == 0) {
    std::cerr << argv[0] << " <filepath>" << std::endl;
    return 0;
  }
  if (argc > 1) {
    std::string arg1(argv[1]);
    if (arg1 == "-h" || arg1 == "-help" || arg1 == "--help") {
      std::cerr << argv[0] << " <filepath>" << std::endl;
      return 0;
    }
    filepath = arg1;
  }

  std::vector<char> vec;

  std::chrono::system_clock::time_point clock_start =
      std::chrono::system_clock::now();
  {
    int ret = Read(filepath, vec);
    if (ret) {
      std::cerr << "Read " << filepath << " failed" << std::endl;
      return ret;
    }
  }

  double elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(
                            std::chrono::system_clock::now() - clock_start)
                            .count();
  std::cout << "elapsed time:" << elapsed_time << "[us]" << std::endl;
  return 0;
}
