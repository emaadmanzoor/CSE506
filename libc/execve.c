#include <stdlib.h>
//#include <errno.h>
#include "syscall.h"

int execve(const char *filename, char *const argv[], char *const envp[]) {
  int res = __syscall(SYS_execve, (uint64_t) filename,
                                  (uint64_t) argv,
                                  (uint64_t) envp);
  if (res < 0) {
    //errno = -res;
    return -1;
  }
  return res;
}
