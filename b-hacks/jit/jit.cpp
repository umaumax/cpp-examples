#include <errno.h>
#include <sys/mman.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

int main(int argc, const char *argv[]) {
  long page_size  = sysconf(_SC_PAGESIZE);
  uint8_t _code[] = {
      0x55,                    // push rbp
      0x48, 0x89, 0xe5,        // mov rbp, rsp
      0x48, 0x89, 0xf8,        // mov rax, rdi
      0x48, 0x0f, 0xaf, 0xc7,  // imul rax, rdi
      0x48, 0x89, 0xec,        // mov rsp, rbp
      0x5d,                    // pop rbp
      0xc3                     // ret
  };
  size_t code_len = sizeof(_code) / sizeof(_code[0]);

  uint8_t *code;
  if ((posix_memalign((void **)&code, page_size, code_len))) {
    std::fprintf(stderr, "ERROR - posix_memalign\n");
    std::exit(EXIT_FAILURE);
  }
  std::memcpy(code, _code, code_len);

  if ((mprotect((void *)code, code_len, PROT_WRITE | PROT_EXEC)) == -1) {
    switch (errno) {
      case EACCES:
        std::fprintf(stderr, "ERROR - EACCES\n");
        break;
      case EINVAL:
        std::fprintf(stderr, "ERROR - EINVAL\n");
        break;
      case ENOMEM:
        std::fprintf(stderr, "ERROR - ENOMEM\n");
        break;
      default:
        std::fprintf(stderr, "ERROR - UNKNOWN ERROR\n");
    }
    std::exit(EXIT_FAILURE);
  }

  int (*sq)(int) = (int (*)(int))code;
  std::printf("%d\n", sq(12));

  return 0;
}
