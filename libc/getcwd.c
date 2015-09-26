#include <stdlib.h>
//#include <errno.h>
#include "syscall.h"

char *getcwd(char *buf, size_t size) {
  int res = syscall_2(SYS_getcwd, (long) buf, size);
  if (res < 0) {
    //errno = -res;
    return NULL;
  }
  return buf;
}
