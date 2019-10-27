#include <cstring>
#include <iostream>
#include <string>

void hook() {}

int main(int argc, char* argv[]) {
  unsigned char jumpcode[14];
  unsigned long addr = (unsigned long)hook;

  // NOTE: error: taking address of temporary array
  // memcpy(jumpcode, (unsigned char[]){0xff, 0x25, 0, 0, 0, 0}, 6);        //jmp *(%rip)
  memcpy(jumpcode, (const unsigned char[]){0xff, 0x25, 0, 0, 0, 0},
         6);  //jmp *(%rip)
  memcpy(jumpcode + 6, &addr, sizeof(addr));

  return 0;
}
