#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

class Hoge {
 public:
  Hoge() {
    state = "constructor called";
    std::cout << "Hoge Constructor" << std::endl;
  }
  ~Hoge() {
    state = "destructor called";
    std::cout << "Hoge Destructor" << std::endl;
  }
  void Print() { std::cout << "Hello Hoge: state = " << state << std::endl; }

 private:
  std::string state = "default";
};

std::thread th;

void AsyncHoge(Hoge& hoge) {
  th = std::thread([&] {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    hoge.Print();
  });
}

void AsyncSharedNGHoge(std::shared_ptr<Hoge> shared_hoge) {
  std::cout << "shared_hoge count = " << shared_hoge.use_count() << std::endl;
  // NOTE: &でキャプチャーすると、引数のshared_hogeを参照するだけで、引数にスコープから抜けると参照カウントが減るので、ダングリングポインタとなる
  th = std::thread([&] {
    std::cout << "[thread]shared_hoge count = " << shared_hoge.use_count()
              << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    shared_hoge->Print();
  });
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

void AsyncSharedHoge(std::shared_ptr<Hoge> shared_hoge) {
  std::cout << "shared_hoge count = " << shared_hoge.use_count() << std::endl;
  th = std::thread([&, shared_hoge] {
    std::cout << "[thread]shared_hoge count = " << shared_hoge.use_count()
              << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    shared_hoge->Print();
  });
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

int main(int argc, char* argv[]) {
  {
    std::cout << "AsyncHoge" << std::endl;
    {
      std::shared_ptr<Hoge> shared_hoge(new Hoge());
      // ここで、*shared_hogeとするとただのHogeになり、関数側にはshared_ptrとして伝わっていないことに注意
      AsyncHoge(*shared_hoge);
    }
    th.join();
    std::cout << std::endl;
  }
  {
    std::cout << "AsyncSharedNGHoge" << std::endl;
    {
      std::shared_ptr<Hoge> shared_hoge(new Hoge());
      AsyncSharedNGHoge(shared_hoge);
    }
    th.join();
    std::cout << std::endl;
  }
  {
    std::cout << "AsyncSharedHoge" << std::endl;
    {
      std::shared_ptr<Hoge> shared_hoge(new Hoge());
      AsyncSharedHoge(shared_hoge);
    }
    th.join();
    std::cout << std::endl;
  }
  return 0;
}
