#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iostream>
#include <thread>
#include <vector>

pid_t gettid(void) {
#ifdef __APPLE__
  uint64_t tid;
  pthread_threadid_np(NULL, &tid);
  return tid;
#else
  return syscall(SYS_gettid);
#endif
}

volatile std::sig_atomic_t thread_flag = 0;
volatile std::sig_atomic_t main_flag   = 0;

std::vector<std::thread> threads;

void abort_handler(int sig);

void abort_handler(int sig) {
  pid_t tid = gettid();
  // NOTE: this hanlder can be success at main process thread only
  thread_flag = 1;
  std::cout << "SIGINT handler at " << tid << " start" << std::endl;
  for (auto&& th : threads) {
    if (th.joinable()) {
      th.join();
    }
  }
  std::cout << "SIGINT handler at " << tid << " end" << std::endl;
  main_flag = 1;
}

int main() {
  std::cout << "main start at" << gettid() << std::endl;

  for (int i = 0; i < 3; i++) {
    threads.emplace_back(std::thread([i]() {
      std::cout << "thread[" << i << "] id=" << gettid() << std::endl;
      while (!thread_flag) {
        std::cout << "thread[" << i << "]" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(10000));
      }
    }));
  }

  // set handler
  {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = abort_handler;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
  }

  while (!main_flag) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  std::cout << "main end" << std::endl;
  return 0;
}
