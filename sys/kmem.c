#include <sys/sbunix.h>

struct pageframe {
  struct pageframe* next;
};
struct pageframe *freelist = NULL;
// uint8_t refcount[MAX_PHYS_PAGES] is a global

void meminit() {
  int i;
  char *p;

  for (i = 0; i < MAX_PHYS_PAGES; i++) {
    refcount[i] = 0;
  }

  for (p = (char*) ALIGNUP(P2V(kphysfree));
       p + PGSIZE < (char*) P2V(physend); p += PGSIZE) {
    kfree(p);
  }
}

uint8_t getref(uint64_t pa) {
  // pa is page aligned
  int byteoffset = pa - kphysfree;
  int pagenumber = byteoffset / PGSIZE;
  if ((pa & 0xfff) != 0 || pagenumber >= MAX_PHYS_PAGES) {
    panic("refcount bug\n");
  }
  return refcount[pagenumber];
}

void incref(uint64_t pa) {
  // pa is page aligned
  int byteoffset = pa - kphysfree;
  int pagenumber = byteoffset / PGSIZE;
  if ((pa & 0xfff) != 0 || pagenumber >= MAX_PHYS_PAGES) {
    panic("refcount bug\n");
  }
  refcount[pagenumber]++;
  //printf("refcount of %x = %d\n", pa, refcount[pagenumber]);
}

void decref(uint64_t pa) {
  // pa is page aligned
  int byteoffset = pa - kphysfree;
  int pagenumber = byteoffset / PGSIZE;
  if ((pa & 0xfff) != 0 || pagenumber >= MAX_PHYS_PAGES) {
    panic("refcount bug\n");
  }
  refcount[pagenumber]--;
  //printf("refcount of %x = %d\n", pa, refcount[pagenumber]);
}

// v must be page-aligned
void kfree(char *v) {
  struct pageframe *pf;

  // write over existing data to catch bugs quickly
  for (char *p = v; p < v + PGSIZE; p++)
    *p = 0;

  // add v to the head of the free list
  pf = (struct pageframe*) v;
  pf->next = freelist;
  freelist = pf;
}

int num_free_pages() {
  int count = 0;
  struct pageframe *pf;
  pf = freelist;
  while( pf ) {
    count++;
    pf = pf->next;
  }
  return count;
}

char *kalloc() {
  struct pageframe *pf;

  if (freelist == NULL)
    return NULL;

  // pop a page from the top of the free list
  pf = freelist;
  freelist = freelist->next;
  memset((void*) pf, 0, PGSIZE);

  return (char*) pf;
}
