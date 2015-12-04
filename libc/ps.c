#include <stdlib.h>
#include "syscall.h"

int ps() {
  return __syscall(SYS_ps);
}
