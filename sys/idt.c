#include <sys/sbunix.h>
#include <sys/asm.h>

#define MAX_IDT 256

// for the config byte in an IDT entry
#define PRESENT     0x80 // present bit
#define DPL0        0x00 // DPL bits (13, 14)
#define DPL3        0x60 // DPL bits (13, 14) for syscalls
#define TYPE_INT    0x0E // 32-bit interrupt gate (clears IF)
#define TYPE_TRP    0x0F // 32-bit trap gate (does not clear IF)

struct idt_entry_t {
  uint16_t offset_15_0;
  uint16_t seg_selector;
  uint8_t zero;
  uint8_t type_attr;
  uint16_t offset_31_16;
  uint32_t offset_63_32;
  uint32_t zero2;
}__attribute__((packed));

struct idt_entry_t idt[MAX_IDT];
extern uint64_t ints[];

struct idtr_t {
	uint16_t size;
	uint64_t addr;
}__attribute__((packed));

static struct idtr_t idtr = {
	sizeof(idt) - 1,
	(uint64_t) idt
};

void init_idt() {
  int i;

  for (i = 0; i < MAX_IDT; i++) {
    idt[i].offset_15_0 = ints[i] & 0xffff;
    idt[i].seg_selector = KCODE;
    idt[i].zero = 0;
    idt[i].offset_31_16 = (ints[i] >> 16) & 0xffff;
    idt[i].offset_63_32 = (ints[i] >> 32) & 0xffffffff;
    idt[i].zero2 = 0;

    if (i == N_SYSCALL) {
      idt[i].type_attr = PRESENT | DPL3 | TYPE_TRP;
    } else {
      idt[i].type_attr = PRESENT | DPL0 | TYPE_INT;
    }
  }

  lidt((uint64_t) &idtr);

  // test software interrupt
  //__asm__ __volatile__ ("int $3");
}
