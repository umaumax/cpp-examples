#include <functional>

/*
#include "hook_global_sample.cpp"
void func_ret_type_void(int a, int b) {
#include "hook_local_sample_start.cpp"
  // original function body starts here
#include "hook_local_sample_end.cpp"
}
*/

#include "hook_global_sample.cpp"
template <class T>
T func_template(T a, T b) {
#include "hook_local_sample_start.cpp"
  // original function body starts here
  return a + b;
#include "hook_local_sample_end.cpp"
}

int main(int argc, char* argv[]) {
  //   func_ret_type_void(1, 2);
  func_template(1, 2);
  return 0;
}
