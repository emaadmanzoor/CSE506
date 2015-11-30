#include <sys/sbunix.h>

void syscall(struct usercontext *f) {
  // call system call
  // store result in eax
  // if failrue, store -1 in eax
  if ( f->rax == 3 ) {
    // to yield
    yield();
  }
}
