#include <sys/sbunix.h>
#include <sys/defs.h>
#include <sys/proc.h>
#include <sys/tarfs.h>

int nextpid = 1;
int first = 1;
struct {
  struct proc proc[MAX_PROC];
} ptable;

void jump_to_user(uint64_t rip, uint64_t rsp,
                  uint64_t cs, uint64_t ds, uint64_t flags); // for debugging
void init_kcontext( struct context** kcontext );
extern void handler_restore();

struct proc *alloc_proc() {
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
  memset(p->kstack, 0, KSTACKSIZE);

  sp = p->kstack + KSTACKSIZE;
  sp -= sizeof *p->ucontext;
  p->ucontext = (struct usercontext*) sp;

  sp -= sizeof(struct context);
  p->kcontext = (struct context *) sp;
  p->kcontext->rip = (uint64_t) &handler_restore;

  return p;
}

void init_user_process( char *path, uint64_t physend ) {
  struct proc* p = alloc_proc(); // allocate and place in ptable
  p->pgdir = setupkvm(physend); // install kernel mappings
  map_program_binary( path, p ); // install user mappings
  p->ucontext->rsp -= 8;  // make space for argc on the user stack
  p->ucontext->cs = UCODE | RPL_U;
  p->ucontext->ss = UDATA | RPL_U;
  p->ucontext->rflags = IF;
  p->state = RUNNABLE;
}

void switch_user_process( struct proc* proc ) {
  // load pgdir into cr3
  // change tss.rsp0
}

void scheduler() {
  struct proc* p;
  for( ; ; ) {
    for(p = ptable.proc; p < &ptable.proc[MAX_PROC]; p++) {
      printf( "In the scheduler\n" );
      if(p->state != RUNNABLE)
        continue;
      //if (first) {
      //first = 0;
        current_proc = p;
        current_proc->state = RUNNING;
        tss.rsp0 = (uint64_t) ( p->kstack + KSTACKSIZE );
        loadpgdir( p->pgdir );
        //jump_to_user( p->ucontext->rip, p->ucontext->rsp,
        //              p->ucontext->cs, p->ucontext->ss, IF );
	printf( "Switching to process pid %d\n", p->pid );
        swtch( &kcontext, p->kcontext );
      //}
    }
  }
}

void yield() {
  current_proc->state = RUNNABLE;
  swtch( &(current_proc->kcontext), kcontext );
}
