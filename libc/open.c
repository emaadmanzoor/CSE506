#include <stdlib.h>
//#include errno
#include "syscall.h"

int open(const char *pathname, int flags) {
  int res = syscall_2(SYS_open, (long) pathname, flags);
  if (res < 0) {
    //errno = -res;
    return -1;
  }
  return res;
}
