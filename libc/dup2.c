#include <stdlib.h>
#include "syscall.h"

int dup2(int oldfd, int newfd) {
  return syscall_2(SYS_dup2, oldfd, newfd);
}
