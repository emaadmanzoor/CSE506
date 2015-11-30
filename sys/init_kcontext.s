.text
	
.globl init_kcontext
.align 16
init_kcontext:
	# Save old callee-save registers
	popq %rsi
	addq $8, %rsi
	pushq %rsi
	//movq (%rsp), %rsi
	//addq $8, %rsi
	pushq %rbp
	pushq %rbx
	pushq %r11
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	movq %rsp, (%rdi)
	subq $8, %rsi
	pushq %rsi
	retq
