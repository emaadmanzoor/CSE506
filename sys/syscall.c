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
    case SYS_read:
      // set the return value to the value returned by read
      f->rax = read((int)f->rdi, (char*)f->rsi, (int)f->rdx);
      break;
    case SYS_write:
      f->rax = write((int)f->rdi, (char*)f->rsi, (uint64_t)f->rdx);
      break;
    case SYS_getpid:
      f->rax = getpid();
      break;
    case SYS_getppid:
      f->rax = getppid();
      break;
    case SYS_wait4:
      f->rax = waitpid((int)f->rdi);
      break;
  }
}
