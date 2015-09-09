#include<stdlib.h>
#include "syscall.h"

pid_t getpid(void) {
  return syscall_0(SYS_getpid);
}
