#include <sys/sbunix.h>
#include <sys/asm.h>
#include <sys/key_map.h>
#include <sys/proc.h>
#define DPL3 0x60

static uint32_t ticks;
static uint32_t secs; // seconds since boot

void pitintr() {
  int preempt = 0;
  ticks++;
  if (ticks == TICKS_PER_SEC) {
    ticks = 0;
    secs++;
    printat(CLOCK_X, CLOCK_Y, 1, secs);
    if (current_proc && current_proc->state == RUNNING && (rcs() | DPL3)) {
      preempt = 1;
    }
  }
  eoi(IRQ_PIT);
  if (preempt)
    yield();
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
    if ( keycode ==  SHIFT1 || keycode == SHIFT2 ) {
      shiftpressed = 1;
      return;
    }
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
    if (inputpos != 0) {
      inputpos--;
      inputqueue[inputpos] = 0;
      printf( "%c", c );
    }
  } else {
    printf( "%c", c);
    //printf( "%d", inputpos);
    inputqueue[ inputpos ] = c;
    inputpos++;
  }
}
void pagefault(uint32_t errcode, uint64_t rip) {
  uint64_t pfa = rcr2();

  // check if this is the stack growing
  if (pfa == current_proc->stackbottom - 8) {
    if (expandstack() == 1) {
      return;
    }
  }

  // default pagefault handling code
  printf("PAGEFAULT @0x%x: 0x%x |", rip, pfa);

  if (errcode & PF_PR) {
    printf(" BADPERM |");
  } else {
    printf(" NOTPRES |");
  }

  if (errcode & PF_US) {
    printf(" USER (KILLED) |");
  } else {
    printf(" KERN |");
  }

  if (errcode & PF_RW) {
    printf(" WR\n");
  } else {
    printf(" RD\n");
  }

  if (errcode & PF_US) {
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
