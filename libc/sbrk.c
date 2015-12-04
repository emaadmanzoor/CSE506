#include <stdlib.h>
#include "syscall.h"

void *sbrk(long inc) {
  long end_data_segment = __syscall(SYS_brk, 0);
  
  if (inc == 0)
    return (void *) end_data_segment;

  int res = brk((void *) end_data_segment + inc);
  if (res == 0)
    return (void *) end_data_segment;
  return (void *) -1;
}
