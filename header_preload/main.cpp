#include <iostream>

#include "opencv2/core.hpp"

#define OPENCV_VERSION(a, b, c) (((a) << 16) + ((b) << 8) + (c))

int main(int argc, char* argv[]) {
  std::cout << "version: " << CV_VERSION << std::endl;
  return 0;
}
