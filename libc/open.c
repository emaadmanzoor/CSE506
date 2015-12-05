#include <stdlib.h>
//#include errno
#include "syscall.h"

int open(const char *pathname, int flags) {
  int res = __syscall(SYS_open, (uint64_t) pathname, 0);
  if (res < 0) {
    //errno = -res;
    return -1;
  }
  return res;
}
