/*
 * Handle the Intel 8259 PIC
 *  
 *  Command specs: http://stanislavs.org/helppc/8259.html
 */

#include <sys/asm.h>
#include <sys/pic.h>

static uint8_t currmask1 = 0xfb; // PIC1
static uint8_t currmask2 = 0xfb; // PIC2

// initialise the PIC cascade
void init_pic() {
  /* ICW1: 00010001 (see specs)
   *  - Enable multiple PICs
   *  - Enable edge-sensitive IRQ (QEMU fails otherwise)
   *  - Enable sending ICW4
   */
  outb(PIC1_CMD, 0x11);
  outb(PIC2_CMD, 0x11);

  /* ICW2:
   *  The PIC defaults to sending IRQ0 to INT 0, but the first
   *  31 interrupts are reserved for hardware exceptions.
   *
   *  So we need to remap IRQs such that:
   *     - PIC1 sends IRQ 0-7 to INT 31-37
   *     - PIC2 sends IRQ 8-15 to INT 38-45
   *
   *  But the ICW must have its last 3 bits set to 0 for x86.
   *  (why? see: http://stanislavs.org/helppc/8259.html)
   *  So PIC2's IRQs will start at 40 instead. 
   *
   *  This implies setting the offset of:
   *     - PIC1 to 31 (0x20)
   *     - PIC2 to 40 (0x28)
   */ 
  outb(PIC1_DAT, 0x20);
  outb(PIC2_DAT, 0x28);

  /* ICW3: Connect the master and slave PICS
   * If the IRQ line number that PIC2 is connected
   * to on PIC1 is L:
   *  - PIC1: L as a bit mask 
   *  - PIC2: L as a number
   * We connect PIC2 to IRQ2 of PIC1 (L = 2).
   */
  outb(PIC1_DAT, 0x4);
  outb(PIC2_DAT, 0x2);

  /* ICW4: Additional options
   *  - Don't enable anything fancy here.
   *  - Only set the bit that enables x86 mode.
   */
  outb(PIC1_DAT, 0x1);
  outb(PIC2_DAT, 0x1);

  // disable all IRQ lines except IRQ2 (slave PIC)
  outb(PIC1_DAT, 0xfb);
  outb(PIC2_DAT, 0xfb);
}

void enable_irq(int irq) {
  uint8_t mask;
  if (irq >= 0 && irq <= 7) { // PIC1
    mask = ~(1 << irq);
    currmask1 &= mask;
    outb(PIC1_DAT, currmask1);
  } else if (irq >= 8 && irq <= 15) { // PIC2
    mask = ~(1 << (8 - irq));
    currmask2 &= mask;
    outb(PIC1_DAT, currmask2);
  }
}

void eoi(int irq) {
  // send the eoi byte
  if (irq >= 8) {
    outb(PIC2_CMD, EOI);
  }
  outb(PIC1_CMD, EOI);
}
