#include <stdlib.h>
#include "syscall.h"

int brk(void *end_data_segment) {
  long res = syscall_1(SYS_brk, (long) end_data_segment);
  
  /* The Linux system call returns:
   *  - The new brk address, on success.
   *  - The old brk address, on failure.
   */
  if (res == (long) end_data_segment)
    return 0;
  return -1;
}
