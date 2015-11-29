#include <sys/sbunix.h>

void syscall(struct trapframe *f) {
  // call system call
  // store result in eax
  // if failrue, store -1 in eax
}
