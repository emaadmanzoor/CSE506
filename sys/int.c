#include <sys/pic.h>
#include <sys/pit.h>
#include <sys/key.h>
#include <sys/asm.h>

static uint8_t ticks;

void pitintr() {
  ticks++;
  if (ticks % INTR_FREQ == 0) {
    // print on screen
    ticks = 0;
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
