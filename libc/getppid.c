#include <stdlib.h>
#include "syscall.h"

pid_t getppid(void) {
  return syscall_0(SYS_getppid);
}
