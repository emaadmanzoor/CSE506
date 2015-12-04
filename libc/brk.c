#include <stdlib.h>
#include "syscall.h"

int brk(void *end_data_segment) {
  uint64_t res = __syscall(SYS_brk, (uint64_t) end_data_segment);
  
  /* The Linux system call returns:
   *  - The new brk address, on success.
   *  - The old brk address, on failure.
   */
  if (res >= (uint64_t) end_data_segment)
    return 0;
  return -1;
}
