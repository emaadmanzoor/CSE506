#include <stdlib.h>
#include "syscall.h"

int open(const char *pathname, int flags, int mode) {
  return syscall_3(SYS_open, (long) pathname, flags, mode);
}
