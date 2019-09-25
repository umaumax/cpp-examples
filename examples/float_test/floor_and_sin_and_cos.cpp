#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>

int main() {
  std::cout.precision(std::numeric_limits<float>::max_digits10 + 1);
  float angle = 0.0f;
  int n       = 20000 + 100;
  for (int i = 0; i < n; i++) {
    angle += M_PI / 10000.0;
    float sin_val = sin(angle);
    float cos_val = cos(angle);
    int m         = 10;
    bool hit_flag = false;
    for (int j = 0; j < m; j++) {
      float v            = j;
      int sin_val_floor1 = v + floor(sin_val);
      int cos_val_floor1 = v + floor(cos_val);
      int sin_val_floor2 = floor(v + sin_val);
      int cos_val_floor2 = floor(v + cos_val);
      if (sin_val_floor1 != sin_val_floor2) {
        std::cout << std::fixed << "[" << i << "]"
                  << "[" << j << "] "
                  << "sin(" << angle << ")=" << sin_val << std::endl;
        std::cout << std::fixed << "  " << v << "+"
                  << "floor(" << sin_val << ") = " << sin_val_floor1
                  << std::endl;
        std::cout << std::fixed << "  "
                  << "floor(" << v << "+" << sin_val << "=" << (v + sin_val)
                  << ") = " << sin_val_floor2 << std::endl;
        hit_flag = true;
      }
      if (cos_val_floor1 != cos_val_floor2) {
        std::cout << std::fixed << "[" << i << "]"
                  << "[" << j << "] "
                  << "cos(" << angle << ")=" << cos_val << std::endl;
        std::cout << std::fixed << "  " << v << "+"
                  << "floor(" << cos_val << ") = " << cos_val_floor1
                  << std::endl;
        std::cout << std::fixed << "  "
                  << "floor(" << v << "+" << cos_val << "=" << (v + cos_val)
                  << ") = " << cos_val_floor2 << std::endl;
        hit_flag = true;
      }
    }
    if (hit_flag) {
      std::cout << std::endl;
    }
  }
}
