#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include <array>
#include <fstream>
#include <functional>
#include <iostream>
#include <stack>
#include <string>
#include <vector>

// return value
// -1: error but not command
// 0~255: exit code
int execve_example(const std::string &command,
                   const std::vector<std::string> &args, std::string &output) {
  int pid    = 0;
  int status = 0;
  pid_t wait_result{};
  int errno_value = 0;
  int ret         = 0;

  int output_pipefd[2] = {-1, -1};
  ret                  = pipe(output_pipefd);
  errno_value          = errno;
  if (ret == -1) {
    std::cerr << "failed pipe:" << std::strerror(errno_value) << std::endl;
    return -1;
  }

  pid         = fork();
  errno_value = errno;
  if (pid == -1) {
    std::cerr << "failed fork:" << std::strerror(errno_value) << std::endl;

    ret         = close(output_pipefd[0]);
    errno_value = errno;
    if (ret == -1) {
      std::cerr << "failed read pipe close:" << std::strerror(errno_value)
                << std::endl;
    }

    ret         = close(output_pipefd[1]);
    errno_value = errno;
    if (ret == -1) {
      std::cerr << "failed write pipe close:" << std::strerror(errno_value)
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

    ret         = close(output_pipefd[0]);
    errno_value = errno;
    if (ret == -1) {
      std::cerr << "failed read pipe close:" << std::strerror(errno_value)
                << std::endl;
      ret         = close(output_pipefd[1]);
      errno_value = errno;
      if (ret == -1) {
        std::cerr << "failed write pipe close:" << std::strerror(errno_value)
                  << std::endl;
      }
      std::_Exit(-1);
    }
    ret         = dup2(output_pipefd[1], STDOUT_FILENO);
    errno_value = errno;
    if (ret == -1) {
      std::cerr << "failed dup2 write pipe:" << std::strerror(errno_value)
                << std::endl;
      ret         = close(output_pipefd[1]);
      errno_value = errno;
      if (ret == -1) {
        std::cerr << "failed write pipe close:" << std::strerror(errno_value)
                  << std::endl;
      }
      std::_Exit(-1);
    }
    ret         = dup2(output_pipefd[1], STDERR_FILENO);
    errno_value = errno;
    if (ret == -1) {
      std::cerr << "failed dup2 write pipe:" << std::strerror(errno_value)
                << std::endl;
      ret         = close(output_pipefd[1]);
      errno_value = errno;
      if (ret == -1) {
        std::cerr << "failed write pipe close:" << std::strerror(errno_value)
                  << std::endl;
      }
      std::_Exit(-1);
    }
    ret         = close(output_pipefd[1]);
    errno_value = errno;
    if (ret == -1) {
      std::cerr << "failed write pipe close:" << std::strerror(errno_value)
                << std::endl;
      std::_Exit(-1);
    }

    char *const *envp = nullptr;
    // NOTE: ret value check is always -1
    execve(command.c_str(), const_cast<char *const *>(argv), envp);
    errno_value = errno;
    delete[] argv;
    std::cerr << "failed execve:" << std::strerror(errno_value) << std::endl;
    std::_Exit(-1);
  }

  wait_result          = wait(&status);
  int wait_errno_value = errno;

  ret         = close(output_pipefd[1]);
  errno_value = errno;
  if (ret == -1) {
    std::cerr << "failed write pipe close:" << std::strerror(errno_value)
              << std::endl;

    ret         = close(output_pipefd[0]);
    errno_value = errno;
    if (ret == -1) {
      std::cerr << "failed read pipe close:" << std::strerror(errno_value)
                << std::endl;
    }
    return -1;
  }

  if (wait_result == -1) {
    std::cerr << "failed wait:" << std::strerror(wait_errno_value) << std::endl;

    ret         = close(output_pipefd[0]);
    errno_value = errno;
    if (ret == -1) {
      std::cerr << "failed read pipe close:" << std::strerror(errno_value)
                << std::endl;
    }
    return -1;
  }

  constexpr int buffer_size = 1024;
  std::array<char, buffer_size> buffer;
  int n = 0;
  while ((n = read(output_pipefd[0], buffer.data(), buffer.size())) != 0) {
    // WARN: read don't write last '\0'
    // WARN: below code maybe ignore writing '\0'
    // output += buffer.data();
    output += std::string(buffer.begin(), buffer.begin() + n);
  }

  ret         = close(output_pipefd[0]);
  errno_value = errno;
  if (ret == -1) {
    std::cerr << "failed read pipe close:" << std::strerror(errno_value)
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

int defer_test() {
  int retval = 0;
  {
    DeferExecuter defer_executer;
    defer_executer.Push([&retval]() {
      std::cout << "1" << std::endl;
      retval = 1;
    });
    // if you want to use early return, use lambda not {} block
    defer_executer.Push([&retval]() {
      std::cout << "2" << std::endl;
      retval = 2;
    });
  }
  // this will return 1
  return retval;
}

int execve_example_defer(const std::string &command,
                         const std::vector<std::string> &args,
                         std::string &output) {
  int retval = 0;

  [&] {
    int pid    = 0;
    int status = 0;
    pid_t wait_result{};
    int errno_value = 0;
    int ret         = 0;

    DeferExecuter defer_executer;

    int output_pipefd[2] = {-1, -1};
    ret                  = pipe(output_pipefd);
    errno_value          = errno;
    if (ret == -1) {
      std::cerr << "failed pipe:" << std::strerror(errno_value) << std::endl;
      retval = -1;
      return;
    }
    defer_executer.Push([&]() {
      int errno_value = 0;
      int ret         = 0;
      for (int i = 0; i < 2; i++) {
        if (output_pipefd[i] != -1) {
          ret         = close(output_pipefd[0]);
          errno_value = errno;
          if (ret == -1) {
            retval = -1;
            if (i == 0) {
              std::cerr << "failed read pipe close:"
                        << std::strerror(errno_value) << std::endl;
            } else {
              std::cerr << "failed write pipe close:"
                        << std::strerror(errno_value) << std::endl;
            }
          }
        }
      }
      return;
    });

    pid         = fork();
    errno_value = errno;
    if (pid == -1) {
      std::cerr << "failed fork:" << std::strerror(errno_value) << std::endl;
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

      ret         = dup2(output_pipefd[1], STDOUT_FILENO);
      errno_value = errno;
      if (ret == -1) {
        std::cerr << "failed dup2 write pipe:" << std::strerror(errno_value)
                  << std::endl;
        std::_Exit(-1);
      }
      ret         = dup2(output_pipefd[1], STDERR_FILENO);
      errno_value = errno;
      if (ret == -1) {
        std::cerr << "failed dup2 write pipe:" << std::strerror(errno_value)
                  << std::endl;
        std::_Exit(-1);
      }
      ret         = close(output_pipefd[1]);
      errno_value = errno;
      if (ret == -1) {
        std::cerr << "failed write pipe close:" << std::strerror(errno_value)
                  << std::endl;
        std::_Exit(-1);
      }

      // NOTE: defet doesn't work at child process
      char *const *envp = nullptr;
      // NOTE: ret value check is always -1
      execve(command.c_str(), const_cast<char *const *>(argv), envp);
      errno_value = errno;
      delete[] argv;
      std::cerr << "failed execve:" << std::strerror(errno_value) << std::endl;
      std::_Exit(-1);
    }

    wait_result          = wait(&status);
    int wait_errno_value = errno;

    // NOTE: close explicitly for avoid next read() wait
    ret              = close(output_pipefd[1]);
    output_pipefd[1] = -1;
    errno_value      = errno;
    if (ret == -1) {
      std::cerr << "failed write pipe close:" << std::strerror(errno_value)
                << std::endl;
      retval = -1;
      return;
    }

    if (wait_result == -1) {
      std::cerr << "failed wait:" << std::strerror(wait_errno_value)
                << std::endl;
      retval = -1;
      return;
    }

    constexpr int buffer_size = 512;
    std::array<char, buffer_size> buffer;
    int n = 0;
    while ((n = read(output_pipefd[0], buffer.data(), buffer.size())) != 0) {
      // WARN: read() don't write last '\0'
      // output += buffer.data();
      output += std::string(buffer.begin(), buffer.begin() + n);
    }

    if (!WIFEXITED(status)) {
      std::cerr << "failed child process exit" << std::endl;
      retval = -1;
      return;
    }
    retval = WEXITSTATUS(status);
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
  std::string output;
  // int ret = popen_example(command, output);
  std::vector<std::string> args = {command};
  // int ret = execve_example("/bin/ls", args, output);
  int ret = execve_example_defer("/bin/ls", args, output);
  if (ret != 0) {
    std::cerr << "exit_code:" << ret << std::endl;
  }
  std::cout << "command:"
            << "[" << command << "]" << std::endl;
  std::cout << "output:"
            << "[" << output << "]" << std::endl;
  return 0;
}
