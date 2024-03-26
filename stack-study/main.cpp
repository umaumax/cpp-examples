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
  // ubuntu
  // pthread_getattr_np(self, &attr);
  // mac
  pthread_attr_init(&attr);
  // mac return 0x0 as stackaddr
  pthread_attr_getstack(&attr, &stackaddr, &stacksize);
  pthread_attr_getguardsize(&attr, &guardsize);
  printf("name=%.4s range=[%p-%p] size=%zu guard size=%zu\n", name, stackaddr,
         reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(stackaddr) +
                                 stacksize - 1),
         stacksize, guardsize);
}
void* threadFunction(void* arg) {
  // way 0
  void* local_var1 = nullptr;
  void* local_var2 = nullptr;

  // way 1
  pthread_t tid = pthread_self();
  // void* stackAddr = pthread_getattr_np(pthread_self(), PTHREAD_ATTR_STACKADDR);
  //
  // std::cout << "         Thread ID: " << tid << std::endl;
  printf("         Thread ID: %p(%lld)\n", (void*)tid, (uint64_t)tid);
  // std::cout << "Stack Address: " << stackAddr << std::endl;

  std::cout << "       arg address: " << &arg << std::endl;
  // std::cout << "        arg value: " << arg << std::endl;
  std::cout << "local var1 address: " << &local_var1 << std::endl;
  std::cout << "local var2 address: " << &local_var2 << std::endl;

  // way 2
  pthread_attr_t tattr;
  void* base;
  int ret;

  get_stack_info();
  /* getting a new address */
  pthread_attr_init(&tattr);

  size_t size;
  ret = pthread_attr_getstacksize(&tattr, &size);
  std::cout << "       stack address size: " << size << std::endl;

  std::cout << "-------------------------------" << std::endl;
  int x = *(int*)arg;
  if (x > 0) {
    x = -1;
    threadFunction(&x);
  }
  return nullptr;
}

int main(int argc, const char* argv[]) {
  std::cout << "-----main----------------------" << std::endl;
  // way 0
  void* local_var1 = nullptr;
  void* local_var2 = nullptr;
  // way 1
  pthread_t tid = pthread_self();
  // void* stackAddr = pthread_getattr_np(pthread_self(), PTHREAD_ATTR_STACKADDR);
  //
  std::cout << "         Thread ID: " << tid << std::endl;
  // std::cout << "Stack Address: " << stackAddr << std::endl;

  std::cout << "       arg address: " << &argc << std::endl;
  // std::cout << "        arg value: " << arg << std::endl;
  std::cout << "local var1 address: " << &local_var1 << std::endl;
  std::cout << "local var2 address: " << &local_var2 << std::endl;
  std::cout << "-------------------------------" << std::endl;

  std::cout << "-----pthread-------------------" << std::endl;
  int n = 4;
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
