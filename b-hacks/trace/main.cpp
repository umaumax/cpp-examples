#include <stdio.h>

void no_trace_hello_world(void);
void hello_world(void);

__attribute__((no_instrument_function)) void no_trace_hello_world(void) {
  printf("Hello under world!\n");
}

void hello_world(void) { printf("Hello world!\n"); }

int main(void) {
  no_trace_hello_world();
  hello_world();
  return 0;
}
