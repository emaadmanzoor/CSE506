#include <sys/sbunix.h>
#include <sys/syscall.h>

void syscall(struct usercontext *f) {
  // call system call
  // store result in eax
  // if failrue, store -1 in eax
  switch (f->rax) {
  case SYS_yield:
    yield();
    break;
  case SYS_exit:
    exit(f->rdi);
    break;
  case SYS_fork:
    fork();
    break;
  }
}
