#include <sys/pic.h>
#include <sys/pit.h>
#include <sys/key.h>
#include <sys/asm.h>
#include <sys/sbunix.h>

static uint32_t ticks;
static uint32_t secs; // seconds since boot

void pitintr() {
  ticks++;
  if (ticks == TICKS_PER_SEC) {
    ticks = 0;
    secs++;
    printat(CLOCK_X, CLOCK_Y, secs);
  }
  eoi(IRQ_PIT);
}

void kbdintr() {
  uint8_t c = inb(KEY_DAT);
  c += 1;
  c -= 1;
  eoi(IRQ_KEY);
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
