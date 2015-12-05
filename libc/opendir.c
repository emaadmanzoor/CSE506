#include <stdlib.h>
//#include errno
#include "syscall.h"

void *opendir(const char *pathname) {
  int res = __syscall(SYS_open, (uint64_t) pathname, 1);
  if (res < 0) {
    //errno = -res;
    return (void*) -1;
  }
  return (void*) (uint64_t) res;
}
