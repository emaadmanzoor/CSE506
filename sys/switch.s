.text
	
.globl swtch
.align 16
swtch:
	  # Save old callee-save registers
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

	  # Load new callee-save registers
	  popq %r15
	  popq %r14
	  popq %r13
	  popq %r12
	  popq %r11
	  popq %rbx
	  popq %rbp

	  retq
