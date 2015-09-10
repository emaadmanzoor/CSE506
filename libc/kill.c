#include <stdlib.h>
#include "syscall.h"

int kill(pid_t pid, int sig) {
  return syscall_2(SYS_kill, pid, sig);
}
