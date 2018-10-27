#include <Eigen/Core>
#include <Eigen/LU>  // for inverse
#include <iostream>

int main(int argc, char* argv[]) {
  Eigen::Matrix3f m;
  m << 0, -1, 0,  //
      0, 0, -1,   //
      1, 0, 0;    //

  std::cout << "x-forward(ROS) to z-forward (normal)" << std::endl;
  std::cout << m << std::endl;
  //  0 -1  0
  //  0  0 -1
  //  1  0  0
  // X' = -Y
  // Y' = -Z
  // Z' = X
  std::cout << std::endl;

  std::cout << "z-forward to x-forward(ROS) (inverse)" << std::endl;
  std::cout << m.inverse() << std::endl;
  //  0  0  1
  // -1  0  0
  //  0 -1  0
  // X' = Z
  // Y' = -X
  // Z' = -Y
  std::cout << std::endl;

  std::cout << "no meaning coincidence (transpose)" << std::endl;
  std::cout << m.transpose() << std::endl;
  return 0;
}
