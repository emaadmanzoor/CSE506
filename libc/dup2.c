#include <stdlib.h>
#include "syscall.h"

int dup2(int oldfd, int newfd) {
  int res = syscall_2(SYS_dup2, oldfd, newfd);
  if (res < 0) {
    return -1;
  }
  return res;
}
