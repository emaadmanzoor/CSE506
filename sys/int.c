#include <sys/sbunix.h>
#include <sys/asm.h>
#include <sys/key_map.h>
#include <sys/proc.h>

static uint32_t ticks;
static uint32_t secs; // seconds since boot

void pitintr() {
  //int free_pages;
  ticks++;
  if (ticks == TICKS_PER_SEC) {
    ticks = 0;
    secs++;
    printat(CLOCK_X, CLOCK_Y, 1, secs);
    //free_pages = num_free_pages();
    //printat(0, 24, 1, free_pages );
  }
  eoi(IRQ_PIT);
}

void kbdintr() {
  char c;
  char keycode = '\0';
  unsigned char status;
  static int shiftpressed = 0;

  eoi( IRQ_KEY );
  status = inb( KEY_STATUS );
  if ( status & 0x01 ) {
    keycode = inb( KEY_DAT );
    if ( keycode ==  SHIFT1 || keycode == SHIFT2 )
      shiftpressed = 1;
    if( keycode < 0 ) {
      shiftpressed = 0;
      return;
    }
  }
  if ( shiftpressed ) {
    c = shift_key_map[ (unsigned char) keycode ];
  } else {
    c = key_map[ (unsigned char) keycode ];
  }
  if (c == '\b') {
    inputpos--;
    inputqueue[inputpos] = 0;
    printf( "%c", c );
  } else {
    printf( "%c", c);
    inputqueue[ inputpos ] = c;
    inputpos++;
  }
  //printat( CHAR_X, CHAR_Y, 0, c );
}

void pagefault(uint32_t errcode, uint64_t rip) {
  uint64_t pfa = rcr2();
  uint64_t pa, pa_new;
  pte_t *pte;

  // check if this a COW operation
  if ((pte = get_mapping(current_proc->pgdir, pfa)) != 0) {
    if (*pte & PTE_C) {
      pa = *pte & ~(0xfff);
      //printf("COW %x\n", pfa);

      // cow page with no sharing
      if (getref(pa) == 1) {
        //printf("refcount = 1, clearing PTE_C, setting PTE_W\n");
        *pte = *pte | PTE_W; // set write
        *pte = *pte & (~PTE_C); // clear cow
        loadpgdir(current_proc->pgdir);
        return;
      }

      // cow page with sharing
      if ((pa_new = (uint64_t) kalloc()) != 0) {
        decref(pa);
        pa_new = V2P(pa_new);
        memcpy((char*) P2V(pa_new), (char*) P2V(pa), PGSIZE);
        *pte = 0; // delete mapping
        create_mapping(current_proc->pgdir, ALIGNDN(pfa), pa_new, PTE_U | PTE_W);
        loadpgdir(current_proc->pgdir); // flush tlb
        return;
      }
    }
  }

  // check if this is the stack growing
  if (pfa == current_proc->stackbottom - 8) {
    if (expandstack() == 1) {
      return;
    }
  }

  // default pagefault handling code
  printf("PAGEFAULT @%d: 0x%d |", rip, pfa);

  if (errcode & PF_PR) {
    printf(" BADPERM |");
  } else {
    printf(" NOTPRES |");
  }

  if (errcode & PF_US) {
    printf(" USER (KILLED PID %d) |", current_proc->pid);
  } else {
    printf(" KERN |");
  }

  if (errcode & PF_RW) {
    printf(" WR\n");
  } else {
    printf(" RD\n");
  }

  if (errcode & PF_US) {
    //panic("User pagefault\n");
    current_proc->killed = 1;
    yield();
  } else {
    panic("Kernel pagefault!\n");
  }
}

void interrupt_handler(struct usercontext* f) {
  switch (f->intno) {
    case N_PIT:
      pitintr();
      break;
    case N_KBD:
      kbdintr();
      break;
    case N_SYSCALL:
      syscall(f);
      break;
    case N_PF:
      pagefault(f->errcode, f->rip);
      break;
    default:
      break; 
  }
}
