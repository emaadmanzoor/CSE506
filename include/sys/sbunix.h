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

// proc.c constants
#define MAX_PROC 64

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

// kmem.c constants
#define PGSIZE    4096
#define KERNBASE  0xFFFFFFFF80000000 // from linker.script
#define V2P(m)    (((uint64_t) m) - KERNBASE)
#define P2V(m)    (((uint64_t) m) + KERNBASE)
/* Source: http://stackoverflow.com/questions/2601121/mprotect-how-aligning-to-multiple-of-pagesize-works */
#define ALIGNUP(x)  ((((uint64_t) x) + PGSIZE-1) & ~(PGSIZE-1))
#define ALIGNDN(x)  (((uint64_t) x) & ~(PGSIZE-1))

#define PML4_SHIFT 39
#define PDPT_SHIFT 30
#define PDT_SHIFT 21
#define PT_SHIFT 12

#define PML4_INDEX(va) (((uint64_t)(va) >> PML4_SHIFT ) & 0x1FF)
#define PDPT_INDEX(va) (((uint64_t)(va) >> PDPT_SHIFT ) & 0x1FF)
#define PDT_INDEX(va) (((uint64_t)(va) >> PDT_SHIFT ) & 0x1FF)
#define PT_INDEX(va) (((uint64_t)(va) >> PT_SHIFT ) & 0x1FF)

#define PTE_P           0x001   // Present
#define PTE_W           0x002   // Writeable
#define PTE_U           0x004   // User

// gdt.c
#define KCODE           8     // byte offsets into the gdt
#define KDATA           16    // for each segment selector
#define UCODE           24
#define UDATA           32

#define RPL_U           3     // require USER privilege level

#define IF              0x200   // interrupt enable flag

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
//// CPU defined
#define N_DE        0    // divide by 0
#define N_DB        1    // debug
#define N_NMI       2    // non-maskable interrupt
#define N_BP        3    // breakpoint
#define N_OF        4    // overflow
#define N_BR        5    // out of bounds
#define N_UD        6    // invalid opcode
#define N_NM        7    // device unavailable
#define N_DF        8    // double fault
#define N_CO        9    // coprocessor segment overrun
#define N_TS       10    // invalid tss
#define N_NP       11    // segment not present
#define N_SS       12    // stack fault
#define N_GP       13    // general protection fault
#define N_PF       14    // page fault
#define N_MF       16    // floating point error
#define N_AC       17    // alignment check
#define N_MC       18    // machine check
#define N_XM       19    // SIMD
//// user defined
#define N_PIT       32   // timer interrupt
#define N_KBD       33   // keyboard interrupt
#define N_SYSCALL   64   // system call

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
void printf( const char *fmt, ...);
void printat( int x, int y, int type, int val );

// kmem.c
void kfree_range(uint64_t vstart, uint64_t vend);
void kfree(char *v);
char *kalloc();

// vm.c
pte_t* setupkvm(uint64_t physend);
void create_mapping(pte_t* pml4, uint64_t va, uint64_t pa, uint32_t perm);
void loadpgdir(pte_t*);

// string.c
int strcmp(const char *, const char *);
int strlen(const char *);
void strcpy(char *, const char *);
void strncpy(char *, const char *, int);

// utils.c
int atoi(const char *);
int octtodec(int);
void panic(char *s);
void jump_to_program(uint64_t entry, uint64_t sp);
void memset(void *b, char c, int len);

// syscall.c
struct usercontext {
  // 4. pushed by the generic handler
  uint64_t r15;
  uint64_t r14;
  uint64_t r13;
  uint64_t r12;
  uint64_t r11;
  uint64_t r10;
  uint64_t r9;
  uint64_t r8;
  uint64_t rsi;
  uint64_t rdi;
  uint64_t rdx;
  uint64_t rcx;
  uint64_t rbx;
  uint64_t rax;

  // 3. pushed by the specific handler
  uint64_t intno;

  // 2. pushed by the interrupt (for X < 32)
  // or by the specific handler (for X >= 32)
  uint64_t errcode;

  // 1. pushed by the interrupt (int X)
  uint64_t rip;
  uint64_t cs;
  uint64_t rflags;
  uint64_t rsp;   // kernel stack grows downwards
  uint64_t ss;    // top of the kernel stack (high address)
};

void syscall(struct usercontext *);

// proc.c
struct proc* alloc_proc();
void init_user_process( char*, uint64_t );
void scheduler();
void yield();
void exit(int);
// global kernel page tables
pte_t* kpgdir;
#endif
