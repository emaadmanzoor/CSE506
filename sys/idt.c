#include <sys/idt.h>
#include <sys/asm.h>

#define MAX_IDT 256

// for the config byte in an IDT entry
#define PRESENT 0x80 // present bit
#define DPL0    0x00 // DPL bits (13, 14)
#define TYPE    0x0E // 386 32-bit trap gate

// offset into the GDT (bytes)
#define GDT_KCODE_OFFSET 0x8

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

void _x86_64_asm_lidt(struct idtr_t* idtr);

void init_idt() {
  int i;

  for (i = 0; i < MAX_IDT; i++) {
    idt[i].offset_15_0 = ints[i] & 0xffff;
    idt[i].seg_selector = GDT_KCODE_OFFSET;
    idt[i].zero = 0;
    idt[i].type_attr = PRESENT | DPL0 | TYPE; 
    idt[i].offset_31_16 = (ints[i] >> 16) & 0xffff;
    idt[i].offset_63_32 = (ints[i] >> 32) & 0xffffffff;
    idt[i].zero2 = 0;
  }

  lidt((uint64_t) &idtr);

  // test software interrupt
  //__asm__ __volatile__ ("int $3"); 
}

void interrupt_handler(uint32_t intno, uint32_t errcode) {
  // TODO
}
