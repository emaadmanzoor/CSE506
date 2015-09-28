/*
 * Handle the Intel 8259 PIC
 *  
 *  Command specs: http://stanislavs.org/helppc/8259.html
 */

#include <sys/asm.h>

// PIC ports
#define PIC1_CMD 0x20
#define PIC1_DAT 0x21
#define PIC2_CMD 0xA0
#define PIC2_DAT 0xA1

// initialise the PIC cascade
void init_pic() {
  /* ICW1: 00010001 (see specs)
   *  - Enable multiple PICs
   *  - Enable edge-senstive IRQ (QEMU fails otherwise)
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

  // clear the interrupt mask
  outb(PIC1_DAT, 0x0);
  outb(PIC2_DAT, 0x0);
}
