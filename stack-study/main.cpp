#include <pthread.h>
#include <unistd.h>

#include <chrono>
#include <cstdint>
#include <iostream>
#include <thread>
#include <vector>

// 一般的な実装や状況によって異なり、スレッドIDはスタックアドレスと直接関連することはない
// このときに、生成されたスレッドのスタックアドレスをベースとすることで必ず一意な値となることが保証されるので、これを利用する実装が多いのではないか?
// main関数のTIDは必ずしもスタックアドレスと近い値ではないが、それ以外のスレッドはこの傾向にある
// スレッドIDの値が大きい
// スタックはアドレスのマイナス方向に積まれる

void get_stack_info() {
  void* stackaddr;
  std::size_t stacksize, guardsize;
  const int PROCES_NAME_LEN  = 16;
  char name[PROCES_NAME_LEN] = {0};
  pthread_attr_t attr;
  pthread_t self = pthread_self();
  pthread_getname_np(self, name, PROCES_NAME_LEN);
// mac
#ifdef __APPLE__
  pthread_attr_init(&attr);  // pthread_attr_getstack() will return 0x0
#else
  // ubuntu
  pthread_getattr_np(self, &attr);
#endif
  pthread_attr_getstack(&attr, &stackaddr, &stacksize);
  pthread_attr_getguardsize(&attr, &guardsize);
  printf("name=%7s range=[%p-%p] size=%zu guard size=%zu\n", name, stackaddr,
         reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(stackaddr) +
                                 stacksize - 1),
         stacksize, guardsize);
}
void* threadFunction(void* arg) {
  void* local_var1 = nullptr;
  void* local_var2 = nullptr;

  int x = *(int*)arg;

  pthread_t tid = pthread_self();
  printf("[%d]      Thread ID: %p(%lu)\n", x, (void*)tid,
         (long unsigned int)tid);

  std::cout << "       arg address: " << &arg << std::endl;
  // std::cout << "        arg value: " << arg << std::endl;
  std::cout << "local var1 address: " << &local_var1 << std::endl;
  std::cout << "local var2 address: " << &local_var2 << std::endl;

  get_stack_info();

  std::cout << "-------------------------------" << std::endl;
  if (x > 0) {
    x--;
    threadFunction(&x);
  }
  return nullptr;
}

int main(int argc, const char* argv[]) {
  std::cout << "-----main----------------------" << std::endl;
  void* local_var1 = nullptr;
  void* local_var2 = nullptr;
  pthread_t tid    = pthread_self();

  printf("         Thread ID: %p(%lu)\n", (void*)tid, (long unsigned int)tid);

  std::cout << "       arg address: " << &argc << std::endl;
  // std::cout << "        arg value: " << arg << std::endl;
  std::cout << "local var1 address: " << &local_var1 << std::endl;
  std::cout << "local var2 address: " << &local_var2 << std::endl;
  std::cout << "-------------------------------" << std::endl;

  std::cout << "-----pthread-------------------" << std::endl;
  int n = 2;
  std::vector<pthread_t> threads(n);
  int x = 1;
  for (int i = 0; i < n; i++) {
    pthread_create(&threads[i], nullptr, threadFunction, (void*)&x);
    // workaround for stdout order
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  for (int i = 0; i < n; i++) {
    pthread_join(threads[i], nullptr);
  }

  std::cout << "-----std::thread----------------" << std::endl;
  {
    std::vector<std::thread> threads;
    for (int i = 0; i < n; i++) {
      threads.emplace_back(std::thread(threadFunction, &x));
      // workaround for stdout order
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    for (int i = 0; i < n; i++) {
      threads[i].join();
    }
  }
  return 0;
}
