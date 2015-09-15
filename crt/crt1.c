#include <stdlib.h>

int main(int argc, char* argv[], char* envp[]);

void _start() {
  /*
   * - what this function does:
   *   - copy the stack pointer %rsp to %rdi
   *   - call _cstart, which has %rdi = %rsp as its first argument
   *   - _cstart then sets up arguments for and calls main
   *
   * notes from the AMD64 ABI: http://www.x86-64.org/documentation/abi.pdf
   *
   * - %rbp
   *   "[...] the user code should mark the deepest stack frame by setting the frame
   *    pointer to zero."
   *
   * - %rsp
   *   "[...] The stack pointer holds the address of the byte with lowest address which
   *   is part of the stack. It is guaranteed to be 16-byte aligned at process entry."
   *
   * - stack at _start:
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
   */

  __asm__("\
    xorq %rbp, %rbp \n\
    movq %rsp, %rdi \n\
    andq $-16, %rsp \n\
    call _cstart \n\
  ");
}

/*
 * Use a long pointer here, since the arguments
 * on the stack are in the form of 8-bytes.
 */
void _cstart(long *sp) {
  // sp[0] == %rbp, pushed by compiler-generated asm
  int argc = (int) sp[1];
  char **argv = (char **) &sp[2];
  // sp[argc+2] = NULL
  char **envp = (char **) &sp[argc+3];

  int res;
  res = main(argc, argv, envp);
  exit(res);
}
