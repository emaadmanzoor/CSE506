#include <sys/sbunix.h>
#include <sys/tarfs.h>
#include <sys/proc.h>

// observation
//  after the last file in tarfs, there are
//  21 empty blocks for some reason
struct elfheader *get_elf_header(char *path) {
  char *p; // points to an address in tarfs
  struct posix_header_ustar* phu;
  struct elfheader* eh;
  int size;

  p = &_binary_tarfs_start;
  while (p < &_binary_tarfs_end) {
    phu = (struct posix_header_ustar*) p;
    p += sizeof(struct posix_header_ustar);

    eh = (struct elfheader*) p;
    if (strcmp(path, phu->name) == 0) {
      return eh;
    }

    size = octtodec(atoi(phu->size));

    if (size == 0) {
      // p already points to the next header address
      continue;
    }

    // the next file header will start at a 512-byte-aligned
    // address, so align the size up to 512 bytes
    p += (size + (BLKSIZE - (size % BLKSIZE)));
  }

  return NULL;
}

// OBSOLETE! (unmaintained code since our kernel changed)
// map the program pointed to by the ELF to the
// given page table
void map_program_binary(char *path, struct proc* proc) {
  int i = 0;
  struct elfheader *eh = get_elf_header(path);
  struct progheader *ph = (struct progheader *)((uint64_t)(eh) + eh->phoff);
  __volatile__ uint64_t va = -1, pa;
  uint64_t pageoff, copyoff = 0;
  uint64_t sp;

  loadpgdir(proc->pgdir); // temporarily to enable copying binary data
  for (; i < eh->phnum; i++, ph++) {
    // iteration for each program header in the ELF
    // each ELF contains a TEXT and DATA segment

    if (ph->type != ELF_LOAD) {
      continue;
    }

    // copy data from ph->offset to ph->filesz
    // to virtual address starting at ph->vaddr 
    // create page table mappings along the way
    if (i == 0) {
      proc->startva = ph->vaddr;
    }
    for (va = ph->vaddr;
         va < ph->vaddr + ph->memsz;
         va += PGSIZE) {
      pa = (uint64_t) kalloc();
      create_mapping(proc->pgdir, va, V2P(pa), PTE_W | PTE_U);

      // copy min(PGSIZE, ph->filesz) bytes into the page at va
      for (pageoff = 0;
           copyoff <= ph->filesz && pageoff < PGSIZE;
           copyoff++, pageoff++) {
        *((char*) va + pageoff) = *((char*) eh + copyoff);
      }
    }
  }
  
  // now va > ph->vaddr + ph->memsz, the last va
  // and va is page-aligned. place the stack in this page
  // (from the last va to last va + PGSIZE)
  //
  // The stack address at this point is unmapped; however,
  // on a stack push, the pointer is FIRST decremented and
  // then written to, so its fine.
  pa = (uint64_t) kalloc();
  memset((void *) pa, 0, PGSIZE);
  create_mapping(proc->pgdir, va, V2P(pa), PTE_W | PTE_U);
  sp = va + PGSIZE;
  proc->endva = sp;
  proc->ucontext->rip = eh->entry;
  proc->ucontext->rsp = sp;

  loadpgdir(kpgdir); // restore kernel page table
}
