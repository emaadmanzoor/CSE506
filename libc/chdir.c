#include <stdlib.h>
#include "syscall.h"

int chdir(const char *path) {
  int res = syscall_1(SYS_chdir, (long) path);
  if (res < 0)
    return -1;
  return 0;
}
