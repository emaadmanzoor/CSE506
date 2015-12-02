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
  while( 1 ) {
    for(p = ptable.proc; p < &ptable.proc[MAX_PROC]; p++) {
      if(p->state != RUNNABLE)
        continue;
      tss.rsp0 = (uint64_t) ( p->kstack + KSTACKSIZE );
      loadpgdir( p->pgdir );
      p->state = RUNNING;
      current_proc = p;
      //free_pages = num_free_pages();
      printf( "Scheduler, pid: %d\n", current_proc->pid);
      swtch( &kcontext, p->kcontext );
      // Back in the kernel after executing the process
      // Don't think we need to switch to kernel's page tables since the process has
      // kernel mappings.
      loadpgdir(kpgdir);
      if (current_proc->killed) {
	freepgdir( current_proc->pgdir, current_proc->startva, current_proc->endva );
	current_proc->state = ZOMBIE;
      }
      current_proc = 0;
    }
  }
}

void yield() {
  current_proc->state = RUNNABLE;
  swtch( &(current_proc->kcontext), kcontext );
}

void exit(int status) {
  // exit the process
  current_proc->state = ZOMBIE;
  current_proc->killed = 1;
  swtch( &(current_proc->kcontext), kcontext );
}

uint64_t get_mapping( pte_t* pgdir, uint64_t va ) {
  pte_t *pdpt, *pdt, *pt;
  pte_t *pml4e, *pdpte, *pde, *pte;

  pml4e = &pgdir[PML4_INDEX(va)];
  if (!(*pml4e & PTE_P))
    return 0;
  pdpt = (pte_t*) P2V(*pml4e & ~(0xfff));
  pdpte = &pdpt[PDPT_INDEX(va)];
  if (!(*pdpte & PTE_P))
    return 0;
  pdt = (pte_t*) P2V(*pdpte & ~(0xfff));
  pde = &pdt[PDT_INDEX(va)];
  if (!(*pde & PTE_P))
    return 0;
  pt = (pte_t*) P2V(*pde & ~(0xfff));
  pte = &pt[PT_INDEX(va)];
  if (!(*pte & PTE_P))
    return 0;
  return (uint64_t)(*pte & ~(0xfff)); // returning the physical address
}
/*
 * Return a copy of the current process's page tables
 */
pte_t* copypgdir( pte_t* pgdir, uint64_t start, uint64_t end ) {
  pte_t* new_pgdir = setupkvm( physend );
  uint64_t va, pa, pa_new;
  int i;
  // both start an end are page aligned, but for fun:
  for ( va = ALIGNDN(start); va < end; va+=PGSIZE ) {
    pa = get_mapping( pgdir, va );
    if (!pa) // did not find a mapping
      continue;

    pa_new = (uint64_t) kalloc();
    memset( (void*)pa_new, 0, PGSIZE);
    for( i=0; i<PGSIZE; i++ ) {
      *((char*)pa_new + i) = *((char*)va + i);
    }
    create_mapping( new_pgdir, va, V2P(pa_new), PTE_W | PTE_U );
  }
  return new_pgdir;
}

int fork() {
  struct proc* p;
  // deep copy the current process's page tables
  p = alloc_proc();
  p->pgdir = copypgdir( current_proc->pgdir, current_proc->startva, current_proc->endva );
  p->parent = current_proc;
  p->startva = current_proc->startva;
  p->endva = current_proc->endva;
  /*
   * IMPORTANT: want to execute the child process at the exact same place as the parent
   * so copy all the user context (includes rip)
   */
  
  *p->ucontext = *current_proc->ucontext;
  p->ucontext->rax = 0;
  current_proc->ucontext->rax = p->pid;
  p->state = RUNNABLE;
  // THIS WILL BE DISCARDED. value of $rax changed when popping stuff in handler_restore
  return p->pid;
}

void freepgdir( pte_t* pgdir, uint64_t start, uint64_t end ) {
  uint64_t va, pa;
  int i, j, k;
  pte_t *pdpt, *pdt, *pt;
  int free_pages;
  free_pages = num_free_pages();
  printf("%d\n", free_pages);
  // free the actual pages
  for ( va = ALIGNDN(start); va < end; va+=PGSIZE ) {
    pa = get_mapping( pgdir, va );
    if (!pa) // did not find a mapping
      continue;
    kfree((char*)P2V(pa));
  }

  // free the page table
  for( i=0; i<512; i++ ) {
    if (pgdir[i] & PTE_P) {
      pdpt = (pte_t*) P2V(pgdir[i] & ~(0xfff));
      for( j=0; j<512; j++) {
	if(pdpt[j] & PTE_P) {
	  pdt = (pte_t*) P2V(pdpt[j] & ~(0xfff));
	  for(k=0; k<512; k++) {
	    if( pdt[k] & PTE_P) {
	      pt = (pte_t*) P2V(pdt[k] & ~(0xfff));
	      kfree((char*)(pt));
	    }
	  }
	  kfree((char*)(pdt));
	}
      }
      kfree((char*)(pdpt));
    }
  }
  kfree((char*)(pgdir));
}
