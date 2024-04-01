#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include <array>
#include <ctime>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <stack>
#include <string>
#include <vector>

// 0~: exec command exit code
// -1: error at this function
int execve_example(const std::string &command,
                   const std::vector<std::string> &args, std::string &output) {
  int pid;
  int status;
  pid_t wait_result;
  int errno_value;
  int ret;

  int stdout_pipefd[2];
  ret         = pipe(stdout_pipefd);
  errno_value = errno;
  if (ret == -1) {
    std::cerr << "failed pipe:" << strerror(errno_value) << std::endl;
    return -1;
  }

  pid         = fork();
  errno_value = errno;
  if (pid == -1) {
    std::cerr << "failed fork:" << strerror(errno_value) << std::endl;

    ret         = close(stdout_pipefd[0]);
    errno_value = errno;
    if (ret == -1) {
      std::cerr << "failed read pipe close:" << strerror(errno_value)
                << std::endl;
    }

    ret         = close(stdout_pipefd[1]);
    errno_value = errno;
    if (ret == -1) {
      std::cerr << "failed write pipe close:" << strerror(errno_value)
                << std::endl;
    }
    return -1;
  }

  if (pid == 0) {
    // NOTE: child process

    const int argn = args.size();
    // NOTE: extra room for command path and sentinel
    const char **argv = new const char *[argn + 2];
    argv[0]           = command.c_str();
    for (int i = 0; i < argn + 1; ++i) {
      argv[i + 1] = args[i].c_str();
    }
    argv[argn + 1] = nullptr;

    ret         = close(stdout_pipefd[0]);
    errno_value = errno;
    if (ret == -1) {
      std::cerr << "failed read pipe close:" << strerror(errno_value)
                << std::endl;
      ret         = close(stdout_pipefd[1]);
      errno_value = errno;
      if (ret == -1) {
        std::cerr << "failed write pipe close:" << strerror(errno_value)
                  << std::endl;
      }
      std::_Exit(-1);
    }
    ret         = dup2(stdout_pipefd[1], 1);
    errno_value = errno;
    if (ret == -1) {
      std::cerr << "failed dup2 write pipe:" << strerror(errno_value)
                << std::endl;
      ret         = close(stdout_pipefd[1]);
      errno_value = errno;
      if (ret == -1) {
        std::cerr << "failed write pipe close:" << strerror(errno_value)
                  << std::endl;
      }
      std::_Exit(-1);
    }
    ret         = close(stdout_pipefd[1]);
    errno_value = errno;
    if (ret == -1) {
      std::cerr << "failed write pipe close:" << strerror(errno_value)
                << std::endl;
      std::_Exit(-1);
    }

    char *const *envp = nullptr;
    // NOTE: ret value check is always -1
    execve(command.c_str(), const_cast<char *const *>(argv), envp);
    errno_value = errno;
    delete[] argv;
    std::cerr << "failed execve:" << strerror(errno_value) << std::endl;
    std::_Exit(-1);
  }

  wait_result          = wait(&status);
  int wait_errno_value = errno;

  ret         = close(stdout_pipefd[1]);
  errno_value = errno;
  if (ret == -1) {
    std::cerr << "failed write pipe close:" << strerror(errno_value)
              << std::endl;

    ret         = close(stdout_pipefd[0]);
    errno_value = errno;
    if (ret == -1) {
      std::cerr << "failed read pipe close:" << strerror(errno_value)
                << std::endl;
    }
    return -1;
  }

  if (wait_result == -1) {
    std::cerr << "failed wait:" << strerror(wait_errno_value) << std::endl;

    ret         = close(stdout_pipefd[0]);
    errno_value = errno;
    if (ret == -1) {
      std::cerr << "failed read pipe close:" << strerror(errno_value)
                << std::endl;
    }
    return -1;
  }

  constexpr int buffer_size = 512;
  std::array<char, buffer_size> buffer;
  int n = 0;
  while ((n = read(stdout_pipefd[0], buffer.data(), buffer.size())) != 0) {
    // WARN: read don't write last '\0'
    // WARN: output += buffer.data(); code maybe ignore writing '\0'
    output += std::string(buffer.begin(), buffer.begin() + n);
  }

  ret         = close(stdout_pipefd[0]);
  errno_value = errno;
  if (ret == -1) {
    std::cerr << "failed read pipe close:" << strerror(errno_value)
              << std::endl;
    return -1;
  }

  if (!WIFEXITED(status)) {
    std::cerr << "failed child process exit" << std::endl;
    return -1;
  }
  return WEXITSTATUS(status);
}

class DeferExecuter {
 public:
  DeferExecuter() {}
  ~DeferExecuter() {
    while (!funcs_.empty()) {
      funcs_.top()();
      funcs_.pop();
    }
  }

  bool Push(std::function<void(void)> func) {
    funcs_.push(func);
    return true;
  }

 protected:
  std::stack<std::function<void(void)>> funcs_;
};

int execve_example_defer(const std::string &command,
                         const std::vector<std::string> &args,
                         std::string &output) {
  int retval = 0;

  [&]() {
    int pid;
    int status;
    pid_t wait_result;
    int errno_value;
    int ret;

    DeferExecuter defer_executer;

    int stdout_pipefd[2] = {-1, -1};
    ret                  = pipe(stdout_pipefd);
    errno_value          = errno;
    if (ret == -1) {
      std::cerr << "failed pipe:" << strerror(errno_value) << std::endl;
      retval = -1;
      return;
    }
    // WARN: only for debug
    std::cerr << "[LOG]stdout_pipefd[0]" << stdout_pipefd[0] << std::endl;
    std::cerr << "[LOG]stdout_pipefd[1]" << stdout_pipefd[1] << std::endl;
    defer_executer.Push([&]() {
      int errno_value;
      int ret;
      for (int i = 0; i < 2; i++) {
        if (stdout_pipefd[i] != -1) {
          ret         = close(stdout_pipefd[0]);
          errno_value = errno;
          if (ret == -1) {
            retval = -1;
            if (i == 0) {
              std::cerr << "failed read pipe close:" << strerror(errno_value)
                        << std::endl;
            } else {
              std::cerr << "failed write pipe close:" << strerror(errno_value)
                        << std::endl;
            }
          }
        }
        // WARN: only for debug
        if (i == 0) {
          std::cout << "pipe read close log" << std::endl;
        } else {
          std::cout << "pipe write close log" << std::endl;
        }
      }
      // WARN: only for debug
      {
        auto t = std::time(nullptr);
        struct tm timeinfo;
        ::localtime_r(&t, &timeinfo);
        std::ofstream("tmp.log", std::ios_base::app)
            << std::put_time(&timeinfo, "%Y/%m/%d %H:%M:%S ") << ": hello"
            << std::endl;
      }
      return;
    });

    pid         = fork();
    errno_value = errno;
    if (pid == -1) {
      std::cerr << "failed fork:" << strerror(errno_value) << std::endl;
      return;
    }

    if (pid == 0) {
      // NOTE: child process

      const int argn = args.size();
      // NOTE: extra room for command path and sentinel
      const char **argv = new const char *[argn + 2];
      argv[0]           = command.c_str();
      for (int i = 0; i < argn + 1; ++i) {
        argv[i + 1] = args[i].c_str();
      }
      argv[argn + 1] = nullptr;

      ret         = dup2(stdout_pipefd[1], 1);
      errno_value = errno;
      if (ret == -1) {
        std::cerr << "failed dup2 write pipe:" << strerror(errno_value)
                  << std::endl;
        std::_Exit(-1);
      }
      ret         = close(stdout_pipefd[1]);
      errno_value = errno;
      if (ret == -1) {
        std::cerr << "failed write pipe close:" << strerror(errno_value)
                  << std::endl;
        std::_Exit(-1);
      }

      char *const *envp = nullptr;
      // NOTE: ret value check is always -1
      execve(command.c_str(), const_cast<char *const *>(argv), envp);
      errno_value = errno;
      delete[] argv;
      std::cerr << "failed execve:" << strerror(errno_value) << std::endl;
      std::_Exit(-1);
    }

    wait_result          = wait(&status);
    int wait_errno_value = errno;

    // NOTE: このcloseを行わないと次のreadが待ちとなるのでこのcloseは明示的に行う必要がある
    ret              = close(stdout_pipefd[1]);
    stdout_pipefd[1] = -1;
    errno_value      = errno;
    if (ret == -1) {
      std::cerr << "failed write pipe close:" << strerror(errno_value)
                << std::endl;
      retval = -1;
      return;
    }

    if (wait_result == -1) {
      std::cerr << "failed wait:" << strerror(wait_errno_value) << std::endl;
      retval = -1;
      return;
    }

    constexpr int buffer_size = 512;
    std::array<char, buffer_size> buffer;
    int n = 0;
    while ((n = read(stdout_pipefd[0], buffer.data(), buffer.size())) != 0) {
      // WARN: read don't write last '\0'
      // WARN: output += buffer.data(); code maybe ignore writing '\0'
      output += std::string(buffer.begin(), buffer.begin() + n);
    }

    if (!WIFEXITED(status)) {
      std::cerr << "failed child process exit" << std::endl;
      retval = -1;
      return;
    }
    retval = WEXITSTATUS(status);
    std::cerr << "retval:" << retval << std::endl;
    return;
  }();
  return retval;
}

int popen_example(const std::string command, std::string &output) {
  std::array<char, 128> buffer;
  int status = -1;
  auto pipe  = popen(command.c_str(), "r");
  if (!pipe) {
    return WEXITSTATUS(status);
  } else {
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
      output += buffer.data();
    }
  }
  status = pclose(pipe);
  return WEXITSTATUS(status);
}

int main(int argc, char *argv[]) {
  std::string command = "ls";
  if (argc >= 2) {
    command = std::string(argv[1]);
  }
  for (int i = 0; i < 100; i++) {
    std::string output;
    std::vector<std::string> args = {command};
    // pattern 1
    // int ret = popen_example(command, output);
    // pattern 2
    // int ret                       = execve_example("/bin/ls", args, output);
    // pattern 3
    int ret = execve_example_defer("/bin/ls", args, output);
    // WARN: This output variable does not include stderr.
    if (ret != 0) {
      std::cerr << "exit_code=" << ret << std::endl;
    }
    std::cout << "command:"
              << "[" << command << "]" << std::endl;
    std::cout << "output:"
              << "[" << output << "]" << std::endl;
  }
  return 0;
}
