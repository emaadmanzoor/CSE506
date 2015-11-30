# Based on ideas from https://littleosbook.github.io/:
#   Section 6.4. Creating a Generic Interrupt Handler
# See also: ints.s
#   The individual handlers in ints.s all call
#   handler in this file.

.global handler
.align 16
handler:
  pushq %rax
  pushq %rbx
  pushq %rcx
  pushq %rdx
  pushq %rdi
  pushq %rsi
  pushq %r8
  pushq %r9
  pushq %r10
  pushq %r11
  pushq %r12
  pushq %r13
  pushq %r14
  pushq %r15

  movq %rsp, %rdi
  callq interrupt_handler # interrupt_handler(struct trapframe* f)

.global handler_restore
.align 16
handler_restore:
  popq %r15
  popq %r14
  popq %r13
  popq %r12
  popq %r11
  popq %r10
  popq %r9
  popq %r8
  popq %rsi
  popq %rdi
  popq %rdx
  popq %rcx
  popq %rbx
  popq %rax

  addq $16, %rsp # pop int nbr, err code

  iretq          # pops the remaining elements on the stack
                 # (TOP ss, rsp, rflags, cs, rip, BOTTOM)
