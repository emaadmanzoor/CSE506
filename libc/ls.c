#include <stdlib.h>
//#include <errno.h>
#include "syscall.h"

int ls(char *path) {
  int res = __syscall(SYS_ls, (uint64_t) path);
  if (res < 0) {
    //errno = -res;
    return -1;
  }
  return res;
}
