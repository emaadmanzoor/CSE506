#include <stdlib.h>

/*
 * First-fit (circular) linked-list memory allocator
 * Ref: K&R section 8.7, adapted here for x86_64 by
 * aligning data to 8 bytes
 */

typedef long long Align; // 8 bytes

union header {
  struct {
    union header *next;
    size_t size;
  } s;
  Align x;
};

typedef union header Header;

static Header* morecore(unsigned int nu);

static Header base;
static Header *freep;

void *malloc(size_t nbytes) {
  Header *p, *prevp; // increments will be in sizeof(Header)
  unsigned int nunits;

  // new memory is claimed from the OS in multiples of
  // the header size, which is a multiple of 8 bytes,
  // so the total size will be aligned to 8 bytes.
  nunits = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1;

  prevp = freep;
  if (freep == NULL) { // first time malloc is run
    base.s.next = &base;
    base.s.size = 0;
    freep = &base;
    prevp = &base;
  }

  // traverse the circular linked list looking for a usable block
  for (p = prevp->s.next; /* no condition */; prevp = p, p = p->s.next) {

    // a usable block is found on the list
    if (p->s.size >= nunits) {
      if (p->s.size == nunits) { // the block size matches exactly
        prevp->s.next = p->s.next; // unlink the block
      } else { // the block size is bigger, split it
        p->s.size -= nunits;
        p += p->s.size;
        p->s.size = nunits;
      }
      freep = prevp; // list start is moved to the last seen block
      return (void *)(p + 1); // skip the header block
    }

    // end of the list, no usable block found
    if (p == freep) {
      p = morecore(nunits);
      if (p == NULL)
        return NULL;
      // morecore calls free, returns ptr to the new block
      // on success (and also modifies freep to point to it)
    }
  }
}

void free(void *ap) {
  Header *bp, *p;

  bp = (Header*) ap - 1; // point to the start of the block
  for (p = freep; !(bp > p && bp < p->s.next); p = p->s.next)
    if (p >= p->s.next && (bp > p || bp < p->s.next))
      break;

  if (bp + bp->s.size == p->s.next) { // next block can be fused
    bp->s.size += p->s.next->s.size;
    bp->s.next = p->s.next->s.next;
  } else {
    bp->s.next = p->s.next;
  }

  if (p + p->s.size == bp) { // prev block can be fused
    p->s.size += bp->s.size;
    p->s.next = bp->s.next;
  } else {
    p->s.next = bp;
  }

  freep = p;
}

static Header *morecore(unsigned int nu) {
  void *p;
  Header *up;

  if (nu < 4096) // allocate in chunks to avoid sbrk calls
    nu = 4096;

  p = sbrk(nu * sizeof(Header));
  if (p == (void *)-1)
    return NULL;

  up = (Header *) p;
  up->s.size = nu;
  free((void *)(up + 1));
  return freep;
}
