.text

# void jump_to_program(uint64_t rip, uint64_t rsp)
# sets up the stack with argc=0, so that _start
# can launch the main function without problems

.global jump_to_program 
jump_to_program:
  movq    %rsi, %rsp    # %rsp = rsp
  pushq   $0            # argc = 0
  pushq   %rdi          # %rdi = rip = address to jump to
	retq
