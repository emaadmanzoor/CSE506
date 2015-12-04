#include <stdlib.h>
//#include <errno.h>
#include "syscall.h"

int close(int fd) {
  int res = __syscall(SYS_close, fd);
  if (res < 0) {
    //errno = -res;
    return -1;
  }
  return res;
}
