#include <sys/sbunix.h>

struct pageframe {
  struct pageframe* next;
};
struct pageframe *freelist;

// free pages in range [vstart, vend)
void kfree_range(uint64_t vstart, uint64_t vend) {
  for (char *p = (char*) ALIGNUP(vstart);
       p + PGSIZE < (char*) vend; p += PGSIZE)
    kfree(p);
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

char *kalloc() {
  struct pageframe *pf;

  if (freelist == NULL)
    return NULL;

  // pop a page from the top of the free list
  pf = freelist;
  freelist = freelist->next;
  return (char*) pf;
}

void memset(void *b, char c, int len) {
  char *p = b;
  while (len > 0) {
    *p = c;
    p++;
    len--;
  }
}
