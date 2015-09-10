#include <stdlib.h>
#include "syscall.h"

off_t lseek(int fildes, off_t offset, int whence) {
  return syscall_3(SYS_lseek, fildes, offset, whence);
}
