#include <sys/sbunix.h>
#include <sys/asm.h>

/* adapted from Chris Stones, shovelos */

#define MAX_GDT 32

// this just has the executable bit set in the access type byte
#define GDT_CS        (0x00180000000000)  /*** code segment descriptor ***/
// this has the executable bit cleared in the access type byte
#define GDT_DS        (0x00100000000000)  /*** data segment descriptor ***/

#define C             (0x00040000000000)  /*** conforming ***/
#define DPL0          (0x00000000000000)  /*** descriptor privilege level 0 ***/
#define DPL1          (0x00200000000000)  /*** descriptor privilege level 1 ***/
#define DPL2          (0x00400000000000)  /*** descriptor privilege level 2 ***/
#define DPL3          (0x00600000000000)  /*** descriptor privilege level 3 ***/
#define P             (0x00800000000000)  /*** present ***/
#define L             (0x20000000000000)  /*** long mode ***/
#define D             (0x40000000000000)  /*** default op size ***/
#define W             (0x00020000000000)  /*** writable data segment ***/

struct sys_segment_descriptor {
	uint64_t sd_lolimit:16;/* segment extent (lsb) */
	uint64_t sd_lobase:24; /* segment base address (lsb) */
	uint64_t sd_type:5;    /* segment type */
	uint64_t sd_dpl:2;     /* segment descriptor priority level */
	uint64_t sd_p:1;       /* segment descriptor present */
	uint64_t sd_hilimit:4; /* segment extent (msb) */
	uint64_t sd_xx1:3;     /* avl, long and def32 (not used) */
	uint64_t sd_gran:1;    /* limit granularity (byte/page) */
	uint64_t sd_hibase:40; /* segment base address (msb) */
	uint64_t sd_xx2:8;     /* reserved */
	uint64_t sd_zero:5;    /* must be zero */
	uint64_t sd_xx3:19;    /* reserved */
}__attribute__((packed));

/* All segments overlap and span the entire address space; they are
 * used only to manage permissions between user/kernel space.
 */
uint64_t gdt[MAX_GDT] = {
	0,                      /*** NULL descriptor ***/
	GDT_CS | P | DPL0 | L,  /*** kernel code segment descriptor ***/
	GDT_DS | P | W | DPL0,  /*** kernel data segment descriptor ***/
	GDT_CS | P | DPL3 | L,  /*** user code segment descriptor ***/
	GDT_DS | P | W | DPL3,  /*** user data segment descriptor ***/
	0, 0,                   /*** TSS ***/
};

struct gdtr_t {
	uint16_t size;
	uint64_t addr;
}__attribute__((packed));

static struct gdtr_t gdtr = {
	sizeof(gdt),
	(uint64_t)gdt,
};

void _x86_64_asm_lgdt(struct gdtr_t* gdtr, uint64_t cs_idx, uint64_t ds_idx);

void reload_gdt() {
  // CS offset is 8 since NULL is at offset 0.
  // DS offset is at 16, right after CS.
	_x86_64_asm_lgdt(&gdtr, 8, 16);
}

void setup_tss() {
	struct sys_segment_descriptor* sd = (struct sys_segment_descriptor*)&gdt[5]; // 6th&7th entry in GDT
	sd->sd_lolimit = sizeof(struct tss_t)-1;
	sd->sd_lobase = ((uint64_t)&tss);
	sd->sd_type = 9; // 386 TSS
	sd->sd_dpl = 0;
	sd->sd_p = 1;
	sd->sd_hilimit = 0;
	sd->sd_gran = 0;
	sd->sd_hibase = ((uint64_t)&tss) >> 24;

  memset((void*)&tss, 0, sizeof(struct tss_t));
  ltr(40); // byte index into gdt for gdt[5]
}
