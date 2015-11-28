#include <sys/sbunix.h>
#include <sys/asm.h>
#include <sys/tarfs.h>

void start(uint32_t* modulep, void* physbase, void* physfree)
{
  //pte_t* kpgdir; // DEBUG

  struct smap_t {
    uint64_t base, length;
    uint32_t type;
  }__attribute__((packed)) *smap;

  uint64_t physend = 0;

  while(modulep[0] != 0x9001) modulep += modulep[1]+2;
  for(smap = (struct smap_t*)(modulep+2); smap < (struct smap_t*)((char*)modulep+modulep[1]+2*4); ++smap) {
    if (smap->type == 1 /* memory */ && smap->length != 0) {
      printf("Available Physical Memory [%x-%x]\n", smap->base, smap->base + smap->length);
      // physend is overwritten on each loop, so its final value
      // is its value in the last loop, which is the last available
      // physical address
      physend = smap->base + smap->length;
    }
  }
  printf("tarfs in [%p:%p]\n", &_binary_tarfs_start, &_binary_tarfs_end);

  // kernel starts here
  kfree_range(P2V(physfree), P2V(physend)); // init page frame allocator
  //kpgdir = setupkvm(physend); // setup kernel page table mappings DEBUG
  setupkvm(physend); // setup kernel page table mappings

  // DEBUG program mapping from ELF binary
  //struct elfheader* eh = get_elf_header("bin/hello");
  //map_program_binary(kpgdir, eh);

  for(;;) {
    __asm__ __volatile__ ("hlt");
  }
}

#define INITIAL_STACK_SIZE 4096
char stack[INITIAL_STACK_SIZE];
uint32_t* loader_stack;
extern char kernmem, physbase;
struct tss_t tss;

void boot(void)
{
  // note: function changes rsp, local stack variables can't be practically used
  register char *s, *v;
  __asm__(
    "movq %%rsp, %0;"
    "movq %1, %%rsp;"
    :"=g"(loader_stack)
    :"r"(&stack[INITIAL_STACK_SIZE])
  );
  reload_gdt();
  setup_tss();
  init_idt();
  init_pic();
  init_pit();
  printat(CLOCK_X, CLOCK_Y, 1, 0); // 0 in the bottom-right corner
  init_kb();
  sti(); // enable hardware interrupts
  start(
    (uint32_t*)((char*)(uint64_t)loader_stack[3] + (uint64_t)&kernmem - (uint64_t)&physbase),
    &physbase,
    (void*)(uint64_t)loader_stack[4]
  );
  s = "!!!!! start() returned !!!!!";
  for(v = (char*)0xb8000; *s; ++s, v += 2) *v = *s;
  while(1);
}
