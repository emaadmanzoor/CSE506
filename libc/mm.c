#include <stdlib.h>

/* First-fit circular linked list buddy allocator, based on K&R section 8.7.
 */

struct mheader {
  struct mheader *next;
  uint64_t sz;
};
typedef struct mheader mheader_t;

// initial head of the list
// (the static here is REQUIRED)
static mheader_t mh;
static mheader_t *freelist; // pointer to current freelist location

void *malloc(uint64_t sz) {
  mheader_t *cur, *prev; // loop pointers
  mheader_t *temp;
  uint64_t blksz;

  if (freelist == NULL) {
    freelist = (mheader_t *) &mh;
    mh.next = (mheader_t *) &mh;
    prev = (mheader_t *) &mh;
    mh.sz = 0;
  }

  // Nice trick from K&R:
  //  measure sz in units of sizeof(mheader)
  //  this makes sure the alignment satisfies every data type,
  //  and makes it convenient to move the header when a block is split.
  sz = (sz + sizeof(mheader_t) - 1) / sizeof(mheader_t); // how many header sizes
  sz += 1; // one header-size for the header itself

  // loops until it returns NULL or an address
  for (cur = prev->next; /* no condition */; prev = cur, cur = cur->next) {
    if (cur->sz < sz) {
      // check if end of the list
      if (cur == freelist) { // looped around
        // allocate in chunks
        blksz = sz; // measure in units of sizeof(mheader)
        if (blksz < 32) { // 32 * sizeof(mheader) = 4096
          blksz = 32;
        }

        // ask the kernel for memory
        temp = (mheader_t *) sbrk(blksz * sizeof(mheader_t));
        if (temp == NULL) { // out of memory
          return NULL;
        }

        // add the new block to the free list
        temp->sz = blksz;
        free((void*)(temp + 1)); // &temp + sizeof(mheader) bytes
        cur = freelist; // free changed *freelist to point to the new block
      }
    } else if (cur->sz == sz) {
      prev->next = cur->next; // unlink the block
      freelist = prev; // repoint last touched block
      return (void *)(cur + 1); // cur + sizeof(header), to skip header
    } else if (cur->sz > sz) {
      cur->sz -= sz;
      cur += cur->sz; // now at the newly split block
      cur->sz = sz;

      freelist = prev;
      return (void *)(cur + 1);  // skip the header
    }
  }
}

void free(void *a) {
  mheader_t *mblk = (mheader_t *) a - 1;
  mheader_t *prev = freelist;
  mheader_t *cur = freelist->next;

  // find a place to insert this block
  // (blocks are in increasing order of address)
  while (1) {
    if (prev >= cur) { // end of list
      if (mblk > prev || mblk < cur) {
        // new block lies at the start or end of the list
        break;
      }
    }
    if (prev < mblk && mblk < cur) {
      // new block lies in the list between two existing blocks
      break;
    }
    prev = cur;
    cur = cur->next;
  }

  // now place the block in between prev and cur,
  // fuse the block with its prev/cur if possible.

  // 1. check if mblk can be fused with the next block
  if (mblk + mblk->sz == cur) {
    mblk->sz += cur->sz;
    mblk->next = cur->next; // partially unlink cur
  } else {
    mblk->next = cur;
  }

  // 2. check if mblk can be fused with the previous block
  if (prev + prev->sz == mblk) {
    prev->sz += mblk->sz;
    prev->next = mblk->next;
  } else {
    prev->next = mblk;
  }

  // 3. update the freelist pointer so freelist->next = new block
  freelist = prev;
}

void memset(char *b, char c, int len) {
  char *p = b;
  while (len > 0) {
    *p = c;
    p++;
    len--;
  }
}
