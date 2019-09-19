#include <iostream>
#include <string>

class Hoge {
 public:
  Hoge() : val_(123) {}
  ~Hoge() {}

  void Print() { std::cout << val_ << std::endl; }
  void SetVal(int val) { val_ = val; }

 private:
  int val_;
};

template <class T>
class Singleton {
 public:
  static inline T& GetInstance() {
    static T instance;
    return instance;
  }

 private:
  Singleton()  = default;
  ~Singleton() = default;

  Singleton(const Singleton&) = delete;
  Singleton& operator=(const Singleton&) = delete;
  Singleton(Singleton&&)                 = delete;
  Singleton& operator=(Singleton&&) = delete;
};

int main(int argc, char* argv[]) {
  {
    Hoge& hoge = Singleton<Hoge>::GetInstance();
    hoge.Print();
    hoge.SetVal(456);
  }
  {
    Hoge& hoge = Singleton<Hoge>::GetInstance();
    hoge.Print();
  }
  return 0;
}
