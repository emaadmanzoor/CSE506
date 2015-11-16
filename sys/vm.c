#include <sys/sbunix.h>
#include <sys/asm.h>

/*typedef struct kmap {
  void* virtualmem;
  uint64_t phys_start;
  uint64_t phys_end;
  int permission;
};*/

pte_t *pml4;
//extern void* physend;

int setupkvm( void* physend ) {
  pte_t *pdpt, *pdt, *pt;
  pte_t *pml4e, *pdpte, *pde, *pte;
  //uint64_t size = kernelmap->phys_end - kernelmap->phys_start;
  //uint64_t size = k.phys_end - k.phys_start;
  //void* va = (void*) ALIGNDN( (uint64_t)( k.virtualmem ) );
  uint64_t pa = 0;
  uint64_t size = (uint64_t) physend - 0;
  void * va = (void*) ALIGNDN( (uint64_t)KERNBASE );
  void* last_va = (void*) ALIGNDN(((uint64_t)va) + size - 1);
  int perm = 0;
  
  /*
   * Each level of 4 level page tables is 4k, allocating pages for each
   */
  if( ( pml4 = (pte_t*) kalloc()) == 0 )
    return 0;
  for( ;; ) {
    pml4e = &pml4[ PML4_INDEX(va) ];
    // There should be nothing stored at pml4e
    // assert *pml4e = NULL
    if ( *pml4e & PTE_P ) {
      pdpt = (pte_t*)P2V( pml4e );
    } else {
      if ( ( pdpt = (pte_t*)kalloc() ) == 0 )
	return 0;
      memset( pdpt, 0, PGSIZE );
      *pml4e = (pte_t)( (uint64_t)(V2P( pdpt )) | PTE_P | PTE_W | PTE_U );
    }
    pdpte = &pdpt[ PDPT_INDEX(va) ];
    if ( *pdpte & PTE_P ) {
      pdt = (pte_t*)P2V( pdpte );
    } else {
      if( ( pdt = (pte_t*)kalloc() ) == 0 )
	return 0;
      memset( pdt, 0, PGSIZE );
      *pdpte = (uint64_t)V2P( pdt ) | PTE_P | PTE_W | PTE_U;
    }
    pde = &pdt[PDT_INDEX(va)];
    if (*pde & PTE_P) {
      pt = (pte_t*)P2V(pde);
    } else {
      if( ( pt = (pte_t*)kalloc() ) == 0 )
	return 0;
      memset( pt, 0, PGSIZE );
      *pde = (uint64_t)V2P( pt ) | PTE_P | PTE_W | PTE_U;
    }
    pte = &pt[PT_INDEX(va)];
    if ( *pte & PTE_P ) {
      // Madness, remaping? Something bad has happened
    }
    *pte = pa | perm | PTE_P;
    if ( va == last_va )
      break;
    pa += PGSIZE;
    va += PGSIZE;
  }
  lcr3((uint64_t)V2P(pml4));
  return 1;
}
