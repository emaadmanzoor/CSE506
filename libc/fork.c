#include <stdlib.h>
//#include <errno.h>
#include "syscall.h"

pid_t fork(void) {
  int res = syscall_0(SYS_fork);
  if (res < 0) {
    //errno = -res;
    return -1;
  }
  return res;
}
