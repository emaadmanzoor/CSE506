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
//void swtch(struct context **old, struct context *new);

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

  sp = p->kstack + KSTACKSIZE;
  sp -= sizeof *p->ucontext;
  p->ucontext = (struct usercontext*) sp;

  return p;
}

void init_user_process( char *path, uint64_t physend ) {
  struct proc* p;
  pte_t* pgdir;
  struct elfheader* eh;

  p = alloc_proc();
  pgdir = setupkvm(physend);
  loadpgdir( pgdir );
  eh = get_elf_header( path );
  map_program_binary( pgdir, eh, p );
}

void switch_user_process( struct proc* proc ) {
  // load pgdir into cr3
  // change tss.rsp0
}
void scheduler() {
  struct proc* p;
  for( ; ; ) {
    for(p = ptable.proc; p < &ptable.proc[MAX_PROC]; p++) {
      if(p->state != RUNNABLE)
	continue;
      if (first) {
	first = 0;
	current_proc = p;
	// set global kcontext
	// first process, call jump_to_user
	tss.rsp0 = (uint64_t) ( p->kstack + KSTACKSIZE );
	loadpgdir( p->pgdir );
	// store kernel context in kcontext
	init_kcontext( &kcontext );
	jump_to_user( p->ucontext->rip, p->ucontext->rsp,
		      p->ucontext->cs, p->ucontext->ss, IF );
	// not needed
      }
    }
  }
}

void yield() {
  current_proc->state = RUNNABLE;
  swtch( &(current_proc->kcontext), kcontext );
}
