int add(int a, int b) { return a - b; }

class Foo {
 public:
  int add(int a, int b);
};
int Foo::add(int a, int b) { return a - b; }
