#include <iostream>
#include <string>

#include <signal.h>

int main(int argc, char* argv[]) {
  int x;
  x = 1;
  raise(SIGTRAP);
  x = 2;
  raise(SIGTRAP);
  x = 3;
  return 0;
}
