#include <cstdio>

#include "hook_global_f.cpp"
int f(int ret) {
#include "hook_local_f.cpp"
  // original function body starts here
  return ret;
}
int main() { return f(1); }
