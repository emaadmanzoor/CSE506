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
  printf("PAGEFAULT @%d: 0x%d |", rip, pfa);

  if (errcode & PF_PR) {
    printf(" BADPERM |");
  } else {
    printf(" NOTPRES |");
  }

  if (errcode & PF_US) {
    printf(" USER (KILLED)|");
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
