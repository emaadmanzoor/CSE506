#include <stdlib.h>
//#include <errno.h>
#include "syscall.h"

int execve(const char *filename, char *const argv[], char *const envp[]) {
  int res = syscall_3(SYS_execve, (long) filename, (long) argv, (long) envp);
  if (res < 0) {
    //errno = -res;
    return -1;
  }
  return res;
}
