#include <sys/sbunix.h>

void syscall() {
  // read proc->tf->eax to find syscall number
  // call system call
  // store result in eax
  // if failrue, store -1 in eax
}
