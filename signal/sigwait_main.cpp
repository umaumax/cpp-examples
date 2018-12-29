#include <signal.h>
#include <iostream>

int main() {
  sigset_t ss;
  int ret;
  int signo;

  ret = sigemptyset(&ss);
  if (ret) {
    std::cerr << "Failed call sigemptyset" << std::endl;
    return ret;
  }
  // NOTE: ここで指定したsignalのみをsigwaitの取得対象とする
  // このとき，通常のsigactionと併用する動作は未定義であるため，注意
  // sigaddsetにはsignalを1つずつ設定すること
  ret = sigaddset(&ss, SIGINT) || sigaddset(&ss, SIGHUP);
  if (ret) {
    std::cerr << "Failed call sigaddset" << std::endl;
    return ret;
  }
  ret = sigprocmask(SIG_BLOCK, &ss, nullptr);
  if (ret) {
    std::cerr << "Failed call sigprocmask" << std::endl;
    return ret;
  }

  while (true) {
    // NOTE: signalを取得するまでずっと待機するため，通常はこれを別スレッドで行うべし
    // signalはキューに貯まるため，何回も呼ばれる可能性があることに注意
    std::cerr << "[pre] sigwait" << std::endl;
    if (sigwait(&ss, &signo) == 0) {
      std::cerr << "sigwait called: signal no = " << signo << std::endl;
      switch (signo) {
        case SIGINT:
          std::cerr << "SIGINT called!" << std::endl;
      }
    }
    std::cerr << "[post] sigwait" << std::endl;
  }
  return 0;
}
