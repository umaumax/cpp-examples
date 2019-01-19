#include <iostream>

struct Hoge {
 private:
  void func0() { std::cout << "func0:" << std::endl; }
  int func1(int a) {
    std::cout << "func1:" << a << std::endl;
    return 1;
  }
  void func2(int a, int b) { std::cout << "func2:" << a << "," << b << std::endl; }
  int member_;
};

#include "private_accesor.hpp"
// method
DECL_PRIVATE_METHOD_ACCESSOR(Hoge, func0, void())
DECL_PRIVATE_METHOD_ACCESSOR(Hoge, func1, int(int))
DECL_PRIVATE_METHOD_ACCESSOR(Hoge, func2, void(int, int))
// member
DECL_PRIVATE_MEMBER_ACCESSOR(Hoge, member_, int)

int main() {
  Hoge a;
  PRIVATE_METHOD_CALL_Hoge_func0(a);
  PRIVATE_METHOD_CALL_Hoge_func1(a, 1);
  PRIVATE_METHOD_CALL_Hoge_func2(a, 1, 2);
  PRIVATE_MEMBER_ACCESS_Hoge_member_(a) = 123;
  std::cout << "member_:" << PRIVATE_MEMBER_ACCESS_Hoge_member_(a) << std::endl;
}
