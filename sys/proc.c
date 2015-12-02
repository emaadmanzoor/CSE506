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

void init_user_process( char *path ) {
  struct proc* p = alloc_proc(); // allocate and place in ptable
  p->pgdir = setupkvm(); // install kernel mappings
  map_program_binary( path, p ); // install user mappings
  p->ucontext->rdi = 0; // argc
  p->ucontext->rsi = 0; // argv = NULL
  p->ucontext->rdx = 0; // envp = NULL
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

void exit(int status) {
  // exit the process
  current_proc->state = UNUSED;
  swtch( &(current_proc->kcontext), kcontext );
}

int exec(char* path, char* argv[], char* envp[]) {
  uint64_t i, p, argc, envc = 0, va = -1, pa, sp, sp2, pageoff, copyoff = 0;
  int len, size, argvsize, envpsize;
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

  for (i = 0; i < eh->phnum; i++, ph++) {
    // iteration for each program header in the ELF
    // each ELF contains a TEXT and DATA segment

    if (ph->type != ELF_LOAD) {
      continue;
    }

    // copy data from ph->offset to ph->filesz
    // to virtual address starting at ph->vaddr
    // create page table mappings along the way
    for (va = ph->vaddr;
         va < ph->vaddr + ph->memsz;
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

  // now va > ph->vaddr + ph->memsz, the last va
  // and va is page-aligned. place the stack in this page
  // (from the last va to last va + PGSIZE)
  //
  // The stack address at this point is unmapped; however,
  // on a stack push, the pointer is FIRST decremented and
  // then written to, so its fine.
  pa = V2P(kalloc());
  create_mapping(newpgdir, va, pa, PTE_W | PTE_U);
  current_proc->ucontext->rsp = va + PGSIZE;
  sp2 = current_proc->ucontext->rsp; // this is a va in the new pagedir
  sp = P2V(pa) + PGSIZE; // this is a va in the current pagedir

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
   *  ----------------  <-- TOP (0x402000)
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
  delete_mappings(oldpgdir);

  return 0;
}
