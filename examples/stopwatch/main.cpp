#include <iostream>

#include <cassert>
#include <chrono>
#include <cstdio>
#include <limits>
#include <map>
#include <string>

#include "stopwatch.hpp"

void f(int n) {
  static StopWatch stopwatch;
  stopwatch.SetDelayPrintFlag();
  stopwatch.Register("total");
  stopwatch.Register("for-loop");

  stopwatch.Clear("for-loop");
  stopwatch.Start("total");
  int sum = 0;
  for (int i = 0; i < n; i++) {
    stopwatch.Toggle("for-loop");
    sum += i;
    stopwatch.Toggle("for-loop");
  }
  stopwatch.Stop("total");

  // NOTE: you can print each key or all keys
  //   stopwatch.Print("total");
  //   stopwatch.Print("for-loop");
  stopwatch.PrintAll();
}

void g() {
  StopWatch stopwatch;

  stopwatch.Start("1");
  stopwatch.Start("1-1");
  stopwatch.Stop("1-1");
  stopwatch.Start("1-2");
  stopwatch.Start("1-2-1");
  stopwatch.Stop("1-2-1");
  stopwatch.Stop("1-2");
  stopwatch.Toggle("1-3");
  stopwatch.Toggle("1-3");
  stopwatch.Stop("1");

  // stopwatch.PrintAll();
}

int main(int argc, char* argv[]) {
  f(1000000);
  f(1000000);
  f(1000000);
  f(1000000);

  g();
  g();
  return 0;
}
