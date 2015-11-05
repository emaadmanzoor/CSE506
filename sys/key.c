#include <sys/sbunix.h>

void init_kb() {
  enable_irq(IRQ_KEY);
}
