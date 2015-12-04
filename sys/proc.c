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

void init_user_process(char *path) {
  char *argv[1] = {NULL};
  char *envp[1] = {NULL};
  current_proc = alloc_proc(); // allocate kstack and place in ptable
  current_proc->pgdir = NULL; // allocated by exec
  current_proc->ucontext->cs = UCODE | RPL_U;
  current_proc->ucontext->ss = UDATA | RPL_U;
  current_proc->ucontext->rflags = IF;
  current_proc->state = RUNNABLE;
  exec(path, argv, envp);
}

void switch_user_process( struct proc* proc ) {
  // load pgdir into cr3
  // change tss.rsp0
}

void scheduler() {
  struct proc* p;
  //int free_pages;
  while( 1 ) {
    for(p = ptable.proc; p < &ptable.proc[MAX_PROC]; p++) {
      if(p->state != RUNNABLE)
        continue;
      tss.rsp0 = (uint64_t) ( p->kstack + KSTACKSIZE );
      loadpgdir( p->pgdir );
      p->state = RUNNING;
      current_proc = p;
      //free_pages = num_free_pages();
      //printf( "Scheduler, free pages: %d\n", free_pages );
      printf( "Scheduler, pid: %d\n", current_proc->pid);
      swtch( &kcontext, p->kcontext );
      // Back in the kernel after executing the process
      // Don't think we need to switch to kernel's page tables since the process has
      // kernel mappings.
      loadpgdir(kpgdir);
      if (current_proc->killed) {
        delete_pages(current_proc->pgdir, current_proc->startva, current_proc->endva);
        delete_pages(current_proc->pgdir, current_proc->stackbottom, current_proc->stacktop);
        freepgdir(current_proc->pgdir);
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

// gets a pointer to a PT entry for va
pte_t *get_mapping(pte_t *pgdir, uint64_t va) {
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
  return pte;
}
/*
 * Copy page tables
 */
void copypgdir(pte_t *destpgdir, pte_t* srcpgdir,
                 uint64_t start, uint64_t end ) {
  uint64_t va, pa, pa_new;
  pte_t *pte;

  // start, end are page aligned
  for ( va = start; va < end; va+=PGSIZE ) {
    pte = get_mapping(srcpgdir, va);

    if (pte == 0) {
      // page not mapped
      continue;
    }

    pa = P2V(*pte & ~(0xfff)); // page physical address
    pa_new = (uint64_t) kalloc();
    memcpy((char*) pa_new, (char*) pa, PGSIZE);

    create_mapping(destpgdir, va, V2P(pa_new), PTE_W | PTE_U );
  }
}

int fork() {
  struct proc* p;
  // deep copy the current process's page tables
  p = alloc_proc();
  p->pgdir = setupkvm();
  copypgdir(p->pgdir, current_proc->pgdir,
            current_proc->startva, current_proc->endva);
  copypgdir(p->pgdir, current_proc->pgdir,
            current_proc->stackbottom, current_proc->stacktop);
  p->parent = current_proc;
  p->startva = current_proc->startva;
  p->endva = current_proc->endva;
  p->stackbottom = current_proc->stackbottom;
  p->stacktop = current_proc->stacktop;
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

// deletes the pages and the mapping
void delete_pages(pte_t* pgdir, uint64_t start, uint64_t end) {
  uint64_t va, pa;
  pte_t *pte;
  for (va = ALIGNDN(start); va < end; va+=PGSIZE) {
    pte = get_mapping(pgdir, va);

    if (pte == 0) {
      // page not mapped
      continue;
    }

    pa = (uint64_t)(*pte & ~(0xfff)); // page physical address
    *pte = 0; // delete the mapping
    kfree((char*)P2V(pa));
  }
}

// delete the page tables
void freepgdir(pte_t* pgdir) {
  int i, j, k;
  pte_t *pdpt, *pdt, *pt;
  //uint64_t va, pa;
  //int free_pages;
  //free_pages = num_free_pages();
  //printf("%d\n", free_pages);
  //delete_pages(pgdir, start, end);

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

int exec(char* path, char* argv[], char* envp[]) {
  uint64_t i, p, argc = 0, envc = 0, va = -1, pa, sp, sp2, pageoff, copyoff = 0;
  uint64_t phstartva, phendva, oldstartva, oldendva, oldstackbottom, oldstacktop;
  int len, size, argvsize = 0, envpsize = 0;
  struct elfheader *eh;
  struct progheader *ph;
  pte_t *oldpgdir, *newpgdir;

  // map the binary into the new process page table
  // (this duplicates map_program_binary becase we need
  // to modify the stack of the new process)
  newpgdir = setupkvm();
  eh = get_elf_header(path);
  ph = (struct progheader *)((uint64_t)(eh) + eh->phoff);
  current_proc->ucontext->rip = eh->entry;

  // save these to free their pages later
  oldstartva = current_proc->startva;
  oldendva = current_proc->endva;
  oldstackbottom = current_proc->stackbottom;
  oldstacktop = current_proc->stacktop;

  current_proc->startva = -1;  // 0x000 unsigned min
  current_proc->endva = 0;   // 0xfff unsigned max
  for (i = 0; i < eh->phnum; i++, ph++) {
    // iteration for each program header in the ELF
    // each ELF contains a TEXT and DATA segment

    if (ph->type != ELF_LOAD) {
      continue;
    }

    phstartva = ALIGNDN(ph->vaddr);
    phendva = ALIGNDN(ph->vaddr + ph->memsz) + PGSIZE;

    if (phstartva <= current_proc->startva) {
      current_proc->startva = phstartva;
    }

    if (phendva >= current_proc->endva) {
      current_proc->endva = phendva;
    }

    // copy data from ph->offset to ph->filesz
    // to virtual address starting at ph->vaddr
    // create page table mappings along the way
    for (va = phstartva;
         va < phendva;
         va += PGSIZE) {
      pa = V2P(kalloc());
      create_mapping(newpgdir, va, pa, PTE_W | PTE_U);

      // copy min(PGSIZE, ph->filesz) bytes into the page at pa
      for (pageoff = 0;
           copyoff <= ph->filesz && pageoff < PGSIZE;
           copyoff++, pageoff++) {
        *((char*) P2V(pa) + pageoff) = *((char*) eh + copyoff);
      }
    }
  }

  // Place the stack to grow downwards from KERNBASE
  // The stack address at this point is unmapped; however,
  // on a stack push, the pointer is FIRST decremented and
  // then written to, so it's fine.
  pa = V2P(kalloc());
  create_mapping(newpgdir, KERNBASE - PGSIZE, pa, PTE_W | PTE_U);
  sp = P2V(pa) + PGSIZE; // this is a va in the current pagedir
  sp2 = KERNBASE; // this is a va in the new pagedir
  current_proc->ucontext->rsp = sp2;

  // for auto-growing stacks (stacktop = KERNBASE)
  current_proc->stackbottom = KERNBASE - PGSIZE;
  current_proc->stacktop = KERNBASE;

  // copy the argv and envp strings to the target process'
  // user stack, because once the old process is deleted, its
  // pointers become invalid and the target process can't
  // use them.

  // argc, space needed for argv and envp
  i = 0, argvsize = 0;
  while (argv[i] != NULL) {
    len = strlen(argv[i]) + 1; // include null byte
    size = ((len - 1)/8) * 8 + 8; // round to 8-byte multiple
    argvsize += size;
    i++;
  }
  argc = i;

  i = 0, envpsize = 0;
  while (envp[i] != NULL) {
    len = strlen(envp[i]) + 1; // include null byte
    size = ((len - 1)/8) * 8 + 8; // round to 8-byte multiple
    envpsize += size;
    i++;
  }
  envc = i;

  /* Structure of the stack
   *
   *  ----------------  <-- TOP (KERNBASE)
   *  |     ...      |
   *  ----------------  argvsize bytes (8-byte multiple)
   *  |     ...      |
   *  ----------------  <-- TOP - argvsize
   *  |     ...      |
   *  ----------------  (argc+1) * 8 bytes (argc pointers + 1 NULL)
   *  |     ...      |
   *  ----------------  <-- TOP - argvsize - (argc+1) * 8
   *
   *  char **argv = TOP - argvsize - (argc + 1) * 8
   *  char **envp = argv - envpsize - (envc + 1) * 8
   *  rsp = envp
   */
  current_proc->ucontext->rdi = argc;
  current_proc->ucontext->rsi = current_proc->ucontext->rsp - argvsize - (argc+1)*8;
  current_proc->ucontext->rdx = current_proc->ucontext->rsi - envpsize - (envc+1)*8;
  current_proc->ucontext->rsp = current_proc->ucontext->rdx;

  // argv
  sp -= argvsize; // TOP - argvsize, location of first arg string
  sp2 -= argvsize;
  p = sp - 8 * (argc + 1); // TOP - argvsize - (argc+1) * 8, first arg pointer
  i = 0;
  while (argv[i] != NULL) {
    // copy the string argv[i] to the stack
    len = strlen(argv[i]);
    size = ((len - 1)/8) * 8 + 8; // round to 8-byte multiple
    strcpy((char*) sp, argv[i]);
    *((uint64_t*) p) = sp2;

    sp += size; // multiple of 8 increment
    sp2 += size;
    p += 8; // place to store the next pointer
    i++;
  }
  *((uint64_t*) p) = 0; // NULL pointer

  // envp
  sp -= (argvsize + (argc + 1) * 8 + envpsize);
  sp2 -= (argvsize + (argc + 1) * 8 + envpsize);
  p = sp - 8 * (envc + 1);
  i = 0;
  while (envp[i] != NULL) {
    len = strlen(envp[i]);
    size = ((len - 1)/8) * 8 + 8; // round to 8-byte multiple
    strcpy((char*) sp, envp[i]);
    *((uint64_t*) p) = sp2;

    sp += size; // multiple of 8 increment
    sp2 += size;
    p += 8; // place to store the next pointer
    i++;
  }
  *((uint64_t*) p) = 0; // NULL pointer

  oldpgdir = current_proc->pgdir;
  current_proc->pgdir = newpgdir;
  loadpgdir(current_proc->pgdir);

  if (oldpgdir == NULL) // first process
    return 0;

  delete_pages(oldpgdir, oldstartva, oldendva);
  delete_pages(oldpgdir, oldstackbottom, oldstacktop);
  freepgdir(oldpgdir);

  return 0;
}

uint64_t growproc(uint64_t newendva) {
  int inc, incpages, i, fail = 0;
  uint64_t pa, va;

  if (newendva <= current_proc->endva) {
    return current_proc->endva;
  }

  inc = newendva - current_proc->endva; // increase in size in bytes
  incpages = (inc + PGSIZE - 1) / PGSIZE; // in pages

  va = current_proc->endva;
  for (i = 0; i < incpages; i++) {
    pa = (uint64_t) kalloc();
    if (pa == 0) {
      fail = 1;
      break;
    }
    create_mapping(current_proc->pgdir, va, V2P(pa), PTE_W | PTE_U);
    va += PGSIZE;
  }

  if (fail) {
    // unmap pages mapped so far
    delete_pages(current_proc->pgdir, current_proc->endva, va);
    return current_proc->endva;
  }

  current_proc->endva = va;
  return current_proc->endva;
}
