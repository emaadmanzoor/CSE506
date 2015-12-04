#include <stdlib.h>
#include "syscall.h"

ssize_t write(int fd, const void *buf, size_t count) {
  return __syscall(SYS_write, fd, (char*)buf, (int)count );
}
