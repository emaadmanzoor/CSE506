#include <sys/sbunix.h>
#include <sys/defs.h>
#include <sys/proc.h>

int nextpid = 1;
struct {
  struct proc proc[MAX_PROC];
} ptable;

struct proc* alloc_proc() {
  int found = 0;
  struct proc *p;
  char *sp;

  for(p = ptable.proc; p < &ptable.proc[MAX_PROC]; p++) {
    if(p->state == UNUSED) {
      found = 1;
      break;
    }
  }

  if (found) {
    p->state = USED;
    p->pid = nextpid++;
  } else {
    return 0;
  }

  if((p->kstack = kalloc()) == 0){
    p->state = UNUSED;
    return 0;
  }

  sp = p->kstack + KSTACKSIZE;
  sp -= sizeof *p->tf;
  p->tf = (struct trapframe*)sp;

  return p;
}
