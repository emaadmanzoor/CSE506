#include <stdlib.h>
//#include <errno.h>
#include "syscall.h"

char *getcwd(char *buf, size_t size) {
  int res = __syscall(SYS_getcwd, (uint64_t) buf, size);
  if (res < 0) {
    //errno = -res;
    return NULL;
  }
  return buf;
}
