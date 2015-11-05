#ifndef __SBUNIX_H
#define __SBUNIX_H

#include <sys/defs.h>

// key.c constants
#define KEY_DAT 0x60
#define KEY_STATUS 0x64

// pic.c constants
#define PIC1_CMD 0x20
#define PIC1_DAT 0x21
#define PIC2_CMD 0xA0
#define PIC2_DAT 0xA1
#define IRQ_OFF  32
#define IRQ_PIT  0
#define IRQ_KEY  1
#define EOI      0x20 // end-of-interrupt byte

// pit.c constants
#define PIT_DAT 0x40 // use channel 0
#define PIT_CMD 0x43

/* Source: FreeBSD
 * Frequency of all three count-down timers; (TIMER_FREQ/freq) is the
 * appropriate count to generate a frequency of freq hz.
 *
 * Note (us):
 *   The strange frequency division is probably to mitigate
 *   floating point error propagation (check).
 */
#define TIMER_FREQ    1193182
#define TIMER_DIV(x)  ((TIMER_FREQ+(x)/2)/(x))
#define INTR_FREQ     100
#define TICKS_PER_SEC TIMER_DIV(INTR_FREQ)/INTR_FREQ

// print.c constants
#define CLOCK_X 75  // location of boot time, leave some space for digits
#define CLOCK_Y 24
#define CHAR_X ( CLOCK_X - 1 )
#define CHAR_Y CLOCK_Y

// gdt.c
struct tss_t {
    uint32_t reserved;
      uint64_t rsp0;
        uint32_t unused[11];
}__attribute__((packed));

extern struct tss_t tss;
extern uint64_t gdt[];
void reload_gdt();
void setup_tss();

// idt.c
void init_idt();

// key.c
void init_kb();

// pit.c
void init_pit();

// pic.c
void init_pic();
void enable_irq(int irq);
void eoi(int irq);

// print.c
void printf(const char *fmt, ...);
void printat( int x, int y, int type, int val );

#endif
