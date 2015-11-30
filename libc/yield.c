#include <stdlib.h>
#include "syscall.h"

void yield() {
  __syscall(SYS_yield);
}
