#include <stdlib.h>
//#include <errno.h>
#include "syscall.h"

pid_t waitpid(pid_t pid, int *status, int options) {
  // SYS_wait4 has a 4th argument: struct rusage __user *ru
  int res = __syscall(SYS_wait4, pid );
  if (res < 0) {
    return -1;
  }
  return res;
}
