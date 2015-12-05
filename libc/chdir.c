#include <stdlib.h>
//#include <errno.h>
#include "syscall.h"

int chdir(const char *path) {
  int res = __syscall(SYS_chdir, (uint64_t) path);
  if (res < 0) {
    //errno = -res;
    return -1;
  }
  return 0;
}
