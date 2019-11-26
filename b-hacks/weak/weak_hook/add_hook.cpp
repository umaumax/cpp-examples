int add(int a, int b) { return a - b; }

class Foo {
 protected:
  int add(int a, int b);
};
int Foo::add(int a, int b) { return a - b; }
