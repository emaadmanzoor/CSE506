#include <stdlib.h>
#include "syscall.h"

ssize_t read(int fd, void *buf, size_t count) {
  return __syscall(SYS_read, fd, (char*)buf, (int) count);
}
