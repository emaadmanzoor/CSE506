/*
 * Handle the 8253 PIT
 */

#include <sys/sbunix.h>
#include <sys/asm.h>

void init_pit() {
  /* Configure PIT channel 0 to raise interrupts.
   *    Command byte: 00 11 010 0
   *       - Channel: zero
   *       - Access mode: lobyte/hibyte
   *       - Operating mode: rate generator 
   *       - BCD/binary mode: 16-bit binary
   */
  outb(PIT_CMD, 0x34);

  // configure counter so the timer interrupts at 100Hz
  // (every 10 ms)
  uint16_t counter = TIMER_DIV(INTR_FREQ);
  outb(PIT_DAT, (uint8_t) (counter & 0xff)); // low byte
  outb(PIT_DAT, (uint8_t) ((counter >> 8) & 0xff)); // hi byte

  // enable the PIT IRQ
  enable_irq(IRQ_PIT);
}
