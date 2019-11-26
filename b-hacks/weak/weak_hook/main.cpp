#include <cstdio>
#include <cstdlib>

#include "add_hook.hpp"
int add(int a, int b) { return a + b; }
class Foo {
 protected:
  // NOTE: No need to use __attribute__((weak))
  //   int add(int a, int b) { return a + b; }

  // NOTE: You need to annotate in class not outside of class
  int add(int a, int b) __attribute__((weak));
};
int Foo::add(int a, int b) { return a + b; }

int main() {
  std::printf("%d\n", add(1, 2));
  Foo foo;
  std::printf("%d\n", foo.add(1, 2));
  return 0;
}
