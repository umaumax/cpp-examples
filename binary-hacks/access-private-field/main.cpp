#include <iostream>

struct Hoge {
 private:
  void func0() { std::cout << "func0:" << std::endl; }
  void func1(int a) { std::cout << "func1:" << a << std::endl; }
  void func2(int a, int b) { std::cout << "func2:" << a << "," << b << std::endl; }
  int member_;
};

#include "private_accesor.hpp"
// method
DECL_PRIVATE_METHOD_ACCESSOR(Hoge, func0, void, ())
DECL_PRIVATE_METHOD_ACCESSOR(Hoge, func1, void, (int))
DECL_PRIVATE_METHOD_ACCESSOR(Hoge, func2, void, (int, int))
#define PRIVATE_CALL_Hoge_func0(x) ((x).*result<Hoge_func0>::ptr)
#define PRIVATE_CALL_Hoge_func1(x) ((x).*result<Hoge_func1>::ptr)
#define PRIVATE_CALL_Hoge_func2(x) ((x).*result<Hoge_func2>::ptr)
// member
DECL_PRIVATE_MEMBER_ACCESSOR(Hoge, member_, int)

int main() {
  Hoge a;
  PRIVATE_CALL_Hoge_func0(a)();
  PRIVATE_CALL_Hoge_func1(a)(1);
  PRIVATE_CALL_Hoge_func2(a)(1, 2);
  PRIVATE_ACCESS_Hoge_member_(a) = 123;
  std::cout << "member_:" << PRIVATE_ACCESS_Hoge_member_(a) << std::endl;
}
