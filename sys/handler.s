# Based on ideas from https://littleosbook.github.io/:
#   Section 6.4. Creating a Generic Interrupt Handler
#
# Instead of using NASM capabilities, we have hacked
# in an interrupt handler for each number 0 - 256,
# each of which does pretty much the same thing:
#   - save the error code (or 0)
#     ("The specific CPU interrupts that put an error
#       code on the stack are 8, 10, 11, 12, 13, 14
#       and 17." - Section 6.3)
#   - save the interrupt number
#   - call the generic interrupt handler

.global handler
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
