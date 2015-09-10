#include <stdlib.h>
#include "syscall.h"


ssize_t write(int fd, const void *buf, size_t count)
{
  return syscall_3(SYS_write, fd, (long)buf, (long)count);
}
