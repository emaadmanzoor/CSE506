#include <sys/sbunix.h>
#include <sys/syscall.h>

void syscall(struct usercontext *f) {
  // call system call
  // store result in eax
  // if failure, store -1 in eax
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
    case SYS_execve:
      exec((char*) f->rdi, (char**) f->rsi, (char**) f->rdx);
      break;
    case SYS_brk:
      f->rax = growproc(f->rdi);
      break;
  }
}
