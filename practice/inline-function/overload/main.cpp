#include <iostream>
#include <string>

#include "main.h"

int add(int a, int b) { return a + b; }

int main(int argc, char* argv[]) {
  add(1, 2);
  add(1.0, 2.0);
  return 0;
}
