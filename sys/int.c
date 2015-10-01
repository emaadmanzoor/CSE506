#include <sys/pic.h>
#include <sys/pit.h>
#include <sys/key.h>
#include <sys/asm.h>
#include <sys/sbunix.h>
#include <sys/key_map.h>

static uint32_t ticks;
static uint32_t secs; // seconds since boot

void pitintr() {
  ticks++;
  if (ticks == TICKS_PER_SEC) {
    ticks = 0;
    secs++;
    printat(CLOCK_X, CLOCK_Y, 1, secs);
  }
  eoi(IRQ_PIT);
}

void kbdintr() {
  char c;
  char keycode;
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
  printat( CHAR_X, CHAR_Y, 0, c );
 }

void interrupt_handler(uint32_t intno, uint32_t errcode) {
  switch (intno) {
    case IRQ_OFF + IRQ_PIT:
      pitintr();
      break;
    case IRQ_OFF + IRQ_KEY:
      kbdintr();
      break;
    default:
      break; 
  }
}
