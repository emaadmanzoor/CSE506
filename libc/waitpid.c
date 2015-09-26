#include <stdlib.h>
//#include <errno.h>
#include "syscall.h"

pid_t waitpid(pid_t pid, int *status, int options) {
  // SYS_wait4 has a 4th argument: struct rusage __user *ru
  int res = syscall_4(SYS_wait4, pid, (long) status, options, 0);
  if (res < 0) {
    //errno = -res;
    return -1;
  }
  return res;
}
