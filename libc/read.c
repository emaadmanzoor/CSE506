#include <stdlib.h>
#include "syscall.h"

ssize_t read(int fd, void *buf, size_t count) {
  return syscall_3(SYS_read, fd, (long) buf, (long) count);
}
