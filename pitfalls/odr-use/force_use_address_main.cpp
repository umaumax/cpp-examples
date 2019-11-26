#include <vector>

class A {
 public:
  static constexpr int x = 123;
};

// NOTE: S/s The symbol is in an uninitialized or zero-initialized data section for small objects.
// constexpr int A::x;  // -> S A::x by nm command

extern "C" {
int f(const int& x) __attribute__((weak));
int f(const int& x) { return x; }
}

int main(int argc, char* argv[]) {
  std::vector<int> v;
  v.emplace_back(A::x);
  f(A::x);
  return 0;
}
