#include <sys/key.h>
#include <sys/asm.h>
#include <sys/pic.h>

void init_kb() {
  enable_irq(IRQ_KEY);
}
