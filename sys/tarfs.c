#include <sys/sbunix.h>
#include <sys/tarfs.h>
#include <sys/proc.h>

// observation
//  after the last file in tarfs, there are
//  21 empty blocks for some reason
struct posix_header_ustar *get_file(char *path) {
  char *p; // points to an address in tarfs
  struct posix_header_ustar* phu;
  int size;

  p = &_binary_tarfs_start;
  while (p < &_binary_tarfs_end) {
    phu = (struct posix_header_ustar*) p;

    if (strcmp(path, phu->name) == 0) {
      return phu;
    }

    size = octtodec(atoi(phu->size));

    p += sizeof(struct posix_header_ustar);

    if (size == 0) {
      // p already points to the next header address
      continue;
    }

    // the next file header will start at a 512-byte-aligned
    // address, so align the size up to 512 bytes
    size = ((size + BLKSIZE - 1)/BLKSIZE) * BLKSIZE;
    p += size;
  }

  return NULL;
}

struct elfheader *get_elf_header(char *path) {
  struct posix_header_ustar *phu = get_file(path);
  if (phu == NULL)
    return NULL;
  return (struct elfheader *) ((char *) phu + sizeof(struct posix_header_ustar));
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

int ls(char *path) {
  char *p; // points to an address in tarfs
  char prefix[200];
  struct posix_header_ustar* phu;
  int i, size;

  p = &_binary_tarfs_start;
  while (p < &_binary_tarfs_end) {
    phu = (struct posix_header_ustar*) p;

    size = octtodec(atoi(phu->size));

    // get the prefix of the name before the first slash
    if (path[0] == '/' && path[1] == 0) { // root
      if (strlen(phu->name) > 0)
        printf("%s\n", phu->name);
    } else {
      strcpy(prefix, phu->name);
      for (i = 0; i < strlen(prefix) && prefix[i] != '/'; i++);
      if (i != strlen(prefix)) {
        prefix[i+1] = 0;
        if (strcmp(path, prefix) == 0) {
          printf("%s\n", phu->name);
        }
      }
    }

    p += sizeof(struct posix_header_ustar);

    if (size == 0) {
      // p already points to the next header address
      continue;
    }

    // the next file header will start at a 512-byte-aligned
    // address, so align the size up to 512 bytes
    size = ((size + BLKSIZE - 1)/BLKSIZE) * BLKSIZE;
    p += size;
  }

  return 0;
}
