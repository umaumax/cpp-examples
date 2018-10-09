#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <vector>

volatile std::sig_atomic_t e_flag = 0;

void abrt_handler(int sig);
void abrt_handler2(int sig);

std::vector<std::function<void(int)>> signal_handlers;

static void aggregate_handler(int signum) {
  for (auto&& handler : signal_handlers) {
    handler(signum);
  }
}

int main() {
  std::cout << "start main" << std::endl;

  // get handler
  {
    struct sigaction old;
    sigaction(SIGINT, NULL, &old);
    auto& sighandler = old.sa_handler;
    if (sighandler) signal_handlers.emplace_back(sighandler);
  }

  // set handler
  {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = abrt_handler2;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
  }

  // get handler
  {
    struct sigaction old;
    sigaction(SIGINT, NULL, &old);
    auto& sighandler = old.sa_handler;
    if (sighandler) signal_handlers.emplace_back(sighandler);
  }

  // overwrite
  // set handler
  {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = abrt_handler;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
  }

  // get handler
  {
    struct sigaction old;
    sigaction(SIGINT, NULL, &old);
    auto& sighandler = old.sa_handler;
    if (sighandler) signal_handlers.emplace_back(sighandler);
  }

  // set handler
  {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = aggregate_handler;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
  }

  // NOTE: 簡単に設定可能
  //   if (signal(SIGINT, abrt_handler) == SIG_ERR) {
  //     exit(1);
  //   }

  while (!e_flag) {
  }

  std::cout << "start end" << std::endl;
  return 0;
}

void abrt_handler(int sig) {
  e_flag = 1;
  std::cout << "handler1" << std::endl;
}

void abrt_handler2(int sig) { std::cout << "handler2" << std::endl; }
