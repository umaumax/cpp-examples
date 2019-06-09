#include <cstdio>
#include <string>

void no_trace_hello_world(void);
void hello_world(void);

__attribute__((no_instrument_function)) void no_trace_hello_world(void) {
  printf("Hello under world!\n");
}

void hello_world(std::string target) {
  printf("Hello world!\n%s!\n", target.c_str());
}

int add(int a, int b) { return a + b; }
double add(double a, double b) { return a + b; }

int main(void) {
  no_trace_hello_world();
  hello_world("Nanoha");
  int a = 123;
  int b = 456;
  printf("%d+%d=%d\n", a, b, add(a, b));
  double x = 123.0;
  double y = 456.0;
  printf("%lf+%lf=%lf\n", x, y, add(x, y));
  return 0;
}
