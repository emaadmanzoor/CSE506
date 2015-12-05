#include <stdlib.h>
#include "syscall.h"

unsigned int sleep(unsigned int time) {
  return __syscall(SYS_nanosleep, time);
}
