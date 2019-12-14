#include <functional>
#include <iostream>
#include <stack>

class DeferExecuter {
 public:
  DeferExecuter() {}
  ~DeferExecuter() {
    while (!funcs_.empty()) {
      funcs_.top()();
      funcs_.pop();
    }
  }

  bool Push(std::function<void(void)> func) {
    funcs_.push(func);
    return true;
  }

 protected:
  std::stack<std::function<void(void)>> funcs_;
};
int main(int argc, char* argv[]) {
  int retval = 0;
  [&retval]() {
    DeferExecuter defer_executer;
    defer_executer.Push([&retval]() {
      std::cout << "1" << std::endl;
      retval = 1;
    });
    defer_executer.Push([&retval]() {
      std::cout << "2" << std::endl;
      retval = 2;
    });
  }();
  return retval;
}
