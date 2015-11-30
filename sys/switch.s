.text
	
.globl swtch
.align 16
swtch:
	  pushq %rbp
	  pushq %rbx
	  pushq %r11
	  pushq %r12
	  pushq %r13
	  pushq %r14
	  pushq %r15

	  # Switch stacks
	  movq %rsp, (%rdi)
	  movq %rsi, %rsp

	  popq %r15
	  popq %r14
	  popq %r13
	  popq %r12
	  popq %r11
	  popq %rbx
	  popq %rbp

	  retq
