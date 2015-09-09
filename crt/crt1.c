#include <stdlib.h>
#include "crt_arch.h"

/*
 * AMD64 ABI: http://www.x86-64.org/documentation/abi.pdf
 *
 *  After the register manipulation in "crt_arch.h", below is
 *  the stack at process start (each element is 8 BYTES long):
 *
 *  sp+0                argc            |   ARGC
 *
 *  sp+8                argv[0]         |
 *  sp+16               argv[2]         |   ARGV
 *  ...                 ...             |
 *  sp + 8*argc         argv[argc-1]    |
 *
 *  sp + 8*argc + 8     0               |   ZERO
 *
 *  sp + 8*argc + 2*8   envp[0]         |
 *  sp + 8*argc + 3*8   envp[1]         |   ENVP
 *  ...                 ...             |
 *  sp + 8*argc + n*8   envp[n-1]       |
 *
 * Since we need to increment the stack pointer by 8 bytes
 * each time we want to refer to the next stack element, we
 * use a pointer to long type.
 */

int main(int argc, char* argv[], char* envp[]);

void __cstart(long *sp) {
  int argc = (int) sp[0];
  char **argv = (char **) &sp[1]; // sp[argc+1] = 0
  char **envp = (char **) &sp[argc+2];

  int res;
  res = main(argc, argv, envp);
  exit(res);
}
