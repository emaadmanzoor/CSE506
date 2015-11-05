#include <sys/sbunix.h>

struct pageframe {
  struct pageframe* next;
};
struct pageframe *freelist;

// free pages in range [vstart, vend)
void kfree_range(void *vstart, void *vend) {
  for (char *p = (char*) ALIGNUP((uint64_t) vstart);
       p + PGSIZE < (char*) vend; p += PGSIZE)
    kfree(p);
}

// v must be page-aligned
void kfree(char *v) {
  struct pageframe *pf;

  // write over existing data to catch bugs quickly
  for (char *p = v; p < v + PGSIZE; p++)
    *p = 0xff;

  // add v to the head of the free list
  pf = (struct pageframe*) v;
  pf->next = freelist;
  freelist = pf;
}
