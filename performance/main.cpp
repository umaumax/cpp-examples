#include <cstdio>
#include <iostream>
#include <string>

double mul10000(double v) {
  for (int i = 0; i < 10000; i++) {
    v *= v;
  }
  return v;
}

double mul10000000(double v) {
  for (int i = 0; i < 10000000; i++) {
    v *= v;
  }
  return v;
}

double mul100000000(double v) {
  for (int i = 0; i < 100000000; i++) {
    v *= v;
  }
  return v;
}

double mul1000000000(double v) {
  for (int i = 0; i < 1000000000; i++) {
    v *= v;
  }
  return v;
}

double mul10000_10000(double v) {
  for (int i = 0; i < 10000; i++) {
    v *= mul10000(v);
  }
  return v;
}
int main(int argc, char* argv[]) {
  double ret_val = 0.0;
  double v       = static_cast<double>(argc);
  ret_val += mul10000(v);
  ret_val += mul10000000(v);
  ret_val += mul100000000(v);
  ret_val += mul1000000000(v);
  ret_val += mul10000_10000(v);
  std::printf("[dummy]:%lf\n", ret_val);
  return 0;
}
