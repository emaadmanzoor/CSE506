#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[], char* envp[]) {
  int testvar;
  testvar = 42;
  while( 1 ) {
    testvar = testvar + 2;
    __asm__ __volatile__ ("movq $3, %rax\n\
                         int $64");
  }

  return testvar;
}
