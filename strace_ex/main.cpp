#include <fcntl.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <unistd.h>

#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <thread>

void peek_and_output(pid_t pid, int fd, uintptr_t addr, size_t size);

int main(int argc, char* argv[]) {
  long ret;
  if (argc < 2) {
    fprintf(stderr, "specify pid\n");
    exit(1);
  }

  // no buffering
  setbuf(stdout, nullptr);
  setbuf(stderr, nullptr);

  pid_t pid = atoi(argv[1]);
  printf("attach to %d\n", pid);

  ret = ptrace(PTRACE_ATTACH, pid, nullptr, nullptr);
  if (ret < 0) {
    perror("failed to attach");
    exit(1);
  }
  printf("attached to %d (ret: %ld)\n", pid, ret);

  int status;

#ifdef __x86_64__
  struct user_regs_struct regs;
#elif __i386__
#error no support architecture register
#elif __arm__
  struct user_regs regs;
#elif __aarch64__
#error no support architecture register
#else
#error no support architecture register
#endif

  // NOTE: stop process for set ptrace options
  waitpid(pid, &status, 0);
  // NOTE: PTRACE_O_TRACESYSGOOD: return (SIGTRAP | 0x80) instead of SIGTRAP
  ret = ptrace(PTRACE_SETOPTIONS, pid, nullptr, PTRACE_O_TRACESYSGOOD);
  if (ret == -1) {
    perror("failed to set PTRACE_O_TRACESYSGOOD");
    return 1;
  }
  // restart process
  ptrace(PTRACE_SYSCALL, pid, nullptr, nullptr);

  int is_enter_stop  = 0;
  long prev_orig_rax = -1;

  printf("start loop\n");

  while (true) {
    waitpid(pid, &status, 0);

    if (WIFEXITED(status)) {
      break;
    } else if (WIFSIGNALED(status)) {
      printf("terminated by signal %d\n", WTERMSIG(status));
    } else if (WIFSTOPPED(status) && WSTOPSIG(status) == (SIGTRAP | 0x80)) {
      // printf("stopped by signal %d\n", WSTOPSIG(status));

      ptrace(PTRACE_GETREGS, pid, nullptr, &regs);

#ifdef __x86_64__
      // NOTE: in this way, syscall-enter-stopかsyscall-exit-stopのどちらかにhitし続けるように思える(確率的には正しくenterを拾えている可能性が高そう)
      // NOTE: syscall-enter-stop: maybe rax = -38(-ENOSYS) (18446744073709551578)
      // enterの場合のraxの値は-38(-ENOSYS) (18446744073709551578)
      // exitの場合のraxの値は返り値
      // それ以外のregisterの値は変化がないように見える
      is_enter_stop = prev_orig_rax == regs.orig_rax ? !is_enter_stop : 1;
      prev_orig_rax = regs.orig_rax;
      is_enter_stop = 1;
#elif __arm__
      // NOTE: some sanity checks to figure out whether it's really a syscall entry.
      // r12: ip(intra-procedure call scratch register): 0(enter) or 1(exit)
      is_enter_stop = !regs.uregs[12];
#endif

      if (is_enter_stop) {
#ifdef __x86_64__
        if (regs.orig_rax == SYS_write) {
          // NOTE: case for write()
          // orig_rax: system call number = 4
          // rdi: fd
          // rsi: buf
          // rdx: count
          // rcx, r8d, r9d
          peek_and_output(pid, (int)regs.rdi, regs.rsi, regs.rdx);
        }
// dump registers
// for (int i = 0;
// i < sizeof(user_regs_struct) / sizeof(unsigned long long int);
// i++) {
// printf("%lu, ", ((uint64_t*)(&regs))[i]);
// }
// printf("\n");
#elif __arm__
        if (regs.uregs[7] == SYS_write) {
          // NOTE: case for write()
          // orig_rax: system call number = 4
          // r0: fd
          // r1: buf
          // r2: count
          peek_and_output(pid, (int)regs.uregs[0], regs.uregs[1],
                          regs.uregs[2]);
        }
// dump registers
// for (int i = 0; i < 18; i++) {
// printf("%lu, ", regs.uregs[i]);
// }
// printf("\n");
#endif
      }
    }

    // restart process
    ptrace(PTRACE_SYSCALL, pid, nullptr, nullptr);
  }

  // TODO: run below code at ctrl-c
  ret = ptrace(PTRACE_DETACH, pid, nullptr, nullptr);
  if (ret < 0) {
    perror("failed to detach");
    exit(1);
  }
  printf("detached from %d (ret: %ld)\n", pid, ret);

  return 0;
}

void peek_and_output(pid_t pid, int fd, uintptr_t addr, size_t size) {
  // printf("fd=%d, addr=%p, count=%zu\n", fd, (void*)addr, size);
  if (fd != 1 && fd != 2) {
    return;
  }
  char* bytes = reinterpret_cast<char*>(malloc(size + sizeof(long)));
  // NOTE: you can read /proc/xxx/mem for more efficient
  int i;
  for (i = 0; i < size; i += sizeof(long)) {
    long data = ptrace(PTRACE_PEEKDATA, pid, addr + i, nullptr);
    if (data == -1) {
      printf("failed to peek data\n");
      free(bytes);
      return;
    }
    memcpy(bytes + i, &data, sizeof(long));
  }
  bytes[size] = '\0';
  write(fd == 2 ? 2 : 1, bytes, size);
  fflush(fd == 2 ? stderr : stdout);
  free(bytes);
}
