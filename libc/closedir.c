#include <stdlib.h>
//#include <errno.h>
#include "syscall.h"

int closedir(void *dir) {
  int res = __syscall(SYS_close, (uint64_t) dir);
  if (res < 0) {
    //errno = -res;
    return -1;
  }
  return res;
}
