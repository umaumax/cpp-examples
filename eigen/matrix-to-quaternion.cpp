#include <Eigen/Core>
#include <Eigen/Geometry>  // for quaternion
#include <Eigen/LU>        // for inverse
#include <iostream>

int main(int argc, char* argv[]) {
  if (argc != 1 + 9) {
    std::cerr << argv[0] << ": rotation matrix(9 args) is required"
              << std::endl;
    std::cerr << "(a00 a01 a02  a10 a11 a12  a20 a21 a22)" << std::endl;
    std::cerr << "e.g. " << std::endl;
    std::cerr << argv[0] << " "
              << "0.0 -1.0 0.0  0.0 0.0 -1.0  1.0 0.0 0.0" << std::endl;
    std::cerr << "HINT:" << std::endl;
    std::cerr << "only quaternion value: add next commands after this command: "
                 "| grep -v '#'"
              << std::endl;
    return 1;
  }

  Eigen::Matrix3f m;
  //   m << a00, a01, a02,
  //        a10, a11, a12,
  //        a20, a21, a22;
  for (auto i = 0; i < 9; i++) {
    m(i % 3, i / 3) = std::atof(argv[i + 1]);
  }

  Eigen::Quaternionf q(m);
  q.normalize();
  std::cout << "# q(x,y,z,w)=(" << std::endl
            << q.x() << " " << q.y() << " " << q.z() << " " << q.w() << " "
            << std::endl
            << "# )" << std::endl;
  return 0;
}
