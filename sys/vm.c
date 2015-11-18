#include <sys/sbunix.h>
#include <sys/asm.h>

int setupkvm(uint64_t physend) {
  // page table management pointers
  pte_t *pml4;
  pte_t *pdpt, *pdt, *pt;
  pte_t *pml4e, *pdpte, *pde, *pte;

  // virtual/physical addresses mapped in each loop iteration
  uint64_t va = ALIGNDN(KERNBASE);              // start virtual address
  uint64_t pa = 0;                              // start physical address
  uint64_t last_va = ALIGNDN(va + physend - 1); // end virtual address
  
  // each level of 4 level page tables is 4k, allocating pages for each
  if ((pml4 = (pte_t*) kalloc()) == 0) {
    return 0; // error allocating a page for the PML4
  }

  // map va -> pa in each iteration of this loop,
  // and increment va and pa by +PGSIZE.
  // terminates after mapping last_va.
  for(;;) {

    // Level 1. Map the PDPT entry in PML4E
    pml4e = &pml4[PML4_INDEX(va)];
    if (*pml4e & PTE_P) {
      pdpt = (pte_t*) P2V(*pml4e & ~(0x7));
    } else {

      if ((pdpt = (pte_t*) kalloc() ) == 0) {
        return 0; // error allocating a page for the PDPT
      }

      memset(pdpt, 0, PGSIZE); // kalloc does this, but just to be sure
      *pml4e = V2P(pdpt) | PTE_P | PTE_W | PTE_U;
    }

    // Level 2. Map the PDT entry in PDPT
    pdpte = &pdpt[PDPT_INDEX(va)];
    if (*pdpte & PTE_P) {
      pdt = (pte_t*) P2V(*pdpte & ~(0x7));
    } else {

      if((pdt = (pte_t*) kalloc()) == 0) {
        return 0; // error allocting a page for the PDT
      }

      memset(pdt, 0, PGSIZE); // kalloc does this, but just to tbe sure
      *pdpte = V2P(pdt) | PTE_P | PTE_W | PTE_U;
    }

    // Level 3. Map the PT entry in PDT
    pde = &pdt[PDT_INDEX(va)];
    if (*pde & PTE_P) {
      pt = (pte_t*) P2V(*pde & ~(0x7));
    } else {

      if((pt = (pte_t*) kalloc()) == 0) {
        return 0; // error allocating a page for the PT
      }

      memset(pt, 0, PGSIZE); // kalloc does this, but just to be sure
      *pde = V2P(pt) | PTE_P | PTE_W | PTE_U;
    }

    // Level 4. Map the physical address in PT
    pte = &pt[PT_INDEX(va)];

    if (*pte & PTE_P) {
      // Madness, remapping? Something bad has happened
    }

    *pte = pa | PTE_P | PTE_W | PTE_U;

    // Check for termination
    if (va == last_va)
      break;

    pa += PGSIZE;
    va += PGSIZE;
  }

  lcr3(V2P(pml4));
  return 1;
}
