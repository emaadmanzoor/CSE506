// PIC ports
#define PIC1_CMD 0x20
#define PIC1_DAT 0x21
#define PIC2_CMD 0xA0
#define PIC2_DAT 0xA1

// IRQs
#define IRQ_PIT  0
#define IRQ_KEY  1

void init_pic();
