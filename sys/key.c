#include <sys/key.h>
#include <sys/asm.h>
#include <sys/pic.h>

void init_kb() {
  outb(PIC1_DAT, ~(1 << IRQ_KEY)); // enable keyboard IRQ line
}
