#include <vector>

class A {
 public:
  static constexpr int x = 123;
};

int main(int argc, char* argv[]) {
  std::vector<int> v;
  v.emplace_back(A::x);
  return 0;
}
