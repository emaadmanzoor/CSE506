# Based on ideas from https://littleosbook.github.io/:
#   Section 6.4. Creating a Generic Interrupt Handler
# See also: ints.s
#   The individual handlers in ints.s all call
#   handler in this file.

.global handler
.align 16
handler:
  # generic interrupt handler
  # The C compiler should take care of saving
  # rbp, rbx, rsi, rdi, r12, r13, r14, r15 according to
  # the AMD64 ABI. We handle the rest.

  push %rsi
  push %rdi

  movq 24(%rsp), %rdi # int nbr
  movq 16(%rsp), %rsi # err code

  pushfq
  pushq %r11
  pushq %r10
  pushq %r9
  pushq %r8
  pushq %rax
  pushq %rcx
  pushq %rdx

  call interrupt_handler # C function, params in rdi, rsi

  popq %rdx
  popq %rcx
  popq %rax
  popq %r8
  popq %r9
  popq %r10
  popq %r11
  popfq
  popq %rdi
  popq %rsi

  addq $16, %rsp # pop int nbr, err code

  iretq
